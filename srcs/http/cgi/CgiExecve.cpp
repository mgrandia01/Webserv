/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecve.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:13:23 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/08 16:12:27 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiExecve.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* ***************************** constr & destr ***************************** */

CgiExecve::CgiExecve(const ServerConfig& server) : _server(server) {}

CgiExecve::~CgiExecve()
{
	if (_pid > 0)
	{
		kill(_pid, SIGKILL);

		int	status;

		waitpid(_pid, &status, 0);
	}
}

/* ******************************** get & set ******************************* */

CgiRequest&    CgiExecve::getVars() {return (_vars);}

ssize_t	CgiExecve::getPid() {return (_pid);}

int	CgiExecve::getClientFd() {return (_clientFd);}

int	CgiExecve::getWriteFd() {return (_inFd);}//{return (_serverToCgi[WRITE_PIPE]);}

int	CgiExecve::getReadFd() {return (_outFd);}//{return (_cgiToServer[READ_PIPE]);}

size_t	CgiExecve::getBytesWritten() {return (_bytesWritten);}

const std::string&	CgiExecve::getResponseBuffer() const {return (_responseBuffer);}

bool	CgiExecve::getInputClosed() {return (_inputClosed);}

bool	CgiExecve::getIsFinished() {return (_isFinished);}

time_t	CgiExecve::getStartTime() {return (_startTime);}


void	CgiExecve::setClientFd(int fd) {_clientFd = fd;}

/* ************************* member funcs / methods ************************* */

HttpStatus	CgiExecve::cgiExecveFunc()
{

	if (pipe(_serverToCgi) < 0)
		return (INTERNAL_SERVER_ERROR);
	if (pipe(_cgiToServer))
	{
		close(_serverToCgi[READ_PIPE]);
		close(_serverToCgi[WRITE_PIPE]);
		return (INTERNAL_SERVER_ERROR);
	}

	_pid = fork();

	if (_pid == 0)
		childManager();
	else if (_pid > 0)
		parentManager();
	else
	{
		close(_serverToCgi[READ_PIPE]);
		close(_serverToCgi[WRITE_PIPE]);
		close(_cgiToServer[READ_PIPE]);
		close(_cgiToServer[WRITE_PIPE]);
		return (INTERNAL_SERVER_ERROR);
	}
	return (OK);
}


std::vector<char *>	CgiExecve::vectorToCharPtr(const std::vector<std::string>& vec)
{
	std::vector<char *>	tmp;

	for (std::vector<std::string>::const_iterator it = vec.begin();
			it != vec.end(); it++)
		tmp.push_back(const_cast<char *>(it->c_str())); 
	tmp.push_back(NULL);

	return (tmp);
}

void	CgiExecve::childManager()
{
	dup2(_serverToCgi[READ_PIPE], STDIN_FILENO);
	dup2(_cgiToServer[WRITE_PIPE], STDOUT_FILENO);

	close(_serverToCgi[READ_PIPE]);//ya estan duplicados
	close(_serverToCgi[WRITE_PIPE]);//nunca los necesite
	close(_cgiToServer[READ_PIPE]);//nunca los neeite
	close(_cgiToServer[WRITE_PIPE]);//ya estan duplicados

	const char					*pathname = _vars.getPathname().c_str();

	std::vector<char *>			argvTmp = vectorToCharPtr(_vars.getArgv());

	std::vector<char *>			envTmp = vectorToCharPtr(_vars.getEnv());

	std::cerr << "PATHNAME: " << pathname << "   " << std::endl;
	for (size_t i = 0; argvTmp[i] != NULL; i++)
		std::cerr << "ARGV: " << argvTmp[i] << std::endl;
	for (size_t i = 0; envTmp[i] != NULL; i++)
		std::cerr << "ENV: " << envTmp[i] << std::endl;

	execve(pathname, &argvTmp[0], &envTmp[0]);
	
	exit(1);
}

void	CgiExecve::parentManager()
{
	close(_serverToCgi[READ_PIPE]);
	close(_cgiToServer[WRITE_PIPE]);
	//ServerToCgi[WRITE_PIPE]   // escribir body hacia CGI
	//CgiToServer[READ_PIPE]   // leer respuesta del CGI
	
	_inFd = _serverToCgi[WRITE_PIPE];
	_outFd = _cgiToServer[READ_PIPE];

	fcntl(_inFd, F_SETFL, O_NONBLOCK);
	fcntl(_outFd, F_SETFL, O_NONBLOCK);

	_bytesWritten = 0;
	_inputClosed = false;
	_isFinished = false;
	_startTime = time(NULL);

	if (_vars.getMethod() != POST || _vars.getBody().empty())
	{
		close(_serverToCgi[WRITE_PIPE]);
		_inputClosed = true;
	}
}

bool	CgiExecve::writeToCgi()
{
	const std::string&	body = _vars.getBody();

	if (_inputClosed || body.empty() || _bytesWritten >= body.size())
	{
		_inputClosed = true;
		return (true);
	}

	const char	*start = body.c_str() + _bytesWritten;
	size_t	len = body.size() - _bytesWritten;
	ssize_t	bytes = write(_inFd, start, len);

	if (bytes > 0)
		_bytesWritten += static_cast<size_t>(bytes);
	else if (bytes == -1)
		return (false);
	
	if (_bytesWritten == body.size())
	{
		close(_inFd);
		_inputClosed = true;
		return (true);
	}
	
	return (false);
}

bool	CgiExecve::readFromCgi()
{
	size_t	len = 4096;
	char	buffer[len];
	
	ssize_t	bytes = read(_outFd, buffer, len);

	if (bytes > 0)
	{
		_responseBuffer.append(buffer, bytes);
		return (false);
	}

	if (bytes == 0)
	{
		_isFinished = true;
		return (true);
	}

	return (false);
}

void	CgiExecve::collectProcess()
{
	if (_pid == 0)
	{
		int		status;
		pid_t	result = waitpid(_pid, &status, WNOHANG);

		if (result > 0)
			_pid = -1;
	}
}

void	CgiExecve::closeInFd()
{
    if (_inFd != -1)
    {
        close(_inFd);
        _inFd = -1;
    }
}

void	CgiExecve::closeOutFd()
{
    if (_outFd != -1)
    {
        close(_outFd);
        _outFd = -1;
    }
}

std::map<std::string, std::string>	CgiExecve::headersToMap(std::vector<std::string> headers)
{
	std::map<std::string, std::string>	map;

	for	(std::vector<std::string>::iterator it = headers.begin();
			it != headers.end(); it++)
	{
		std::string	token = *it;
		size_t		colonPos = token.find(':');

		if (colonPos == std::string::npos)
			return (std::map<std::string, std::string> ());

		std::string	tag	= token.substr(0, colonPos);
		std::string	content = token.substr(colonPos + 1);

		if (!content.empty() && content[0] == ' ')
			content.erase(0, 1);

		if (map.find(tag) != map.end())
			map[tag] = " " + content;
		else
			map[tag] = content;
	}

	return (map);
}

std::vector<std::string>	CgiExecve::strSplitStr(std::string& headersLine, std::string& delimiter)
{
	std::vector<std::string>	words;
	size_t	dLen = delimiter.size();
	size_t	start = 0;
	size_t	dPos = 1;

	while (dPos != std::string::npos)
	{
		dPos = headersLine.find(delimiter, start);

		size_t	tokenLen = dPos - start;
		std::string	token = headersLine.substr(start, tokenLen);

		words.push_back(token);
		start = dPos + dLen;
	}
	return (words);
}

void	CgiExecve::cgiHeader(std::string& headersLine, std::string delimiter, Response& cgiResponse)
{
	std::vector<std::string>	headers = strSplitStr(headersLine, delimiter);

	if (headers.empty())
		return ;
	
	std::map<std::string, std::string>	headersMap = headersToMap(headers);

	if (headersMap.empty())
		return ;

	cgiResponse.headers = headersMap;
}

void	CgiExecve::cgiBody(std::string& body, Response& cgiResponse){cgiResponse.body = body;}

void	CgiExecve::cgiStatus(Response& cgiResponse)
{
	std::map<std::string, std::string>::iterator	it = cgiResponse.headers.find("Status");

	if (it != cgiResponse.headers.end())
	{
		std::string	token = it->second;
		size_t		codeLen = token.find(' ');
		std::string	codeStr = token.substr(0, codeLen);
		int			code = std::atoi(codeStr.c_str());
		
		cgiResponse.statusCode = code;
		cgiResponse.reasonPhrase = token.substr(codeLen + 1);
	}
	else
	{	
		cgiResponse.statusCode = 200;
		cgiResponse.reasonPhrase = "OK";
	}
}

void	CgiExecve::cgiOutputParser(Response& cgiResponse)//<-response.processCGIOutput();
{
	std::string	delimiter;
	size_t		lbLen = 0;
	size_t		lbPos;

	if (_responseBuffer.empty())
	{
		cgiResponse.createError(INTERNAL_SERVER_ERROR, _server);
		return ;
	}
	delimiter = "\r\n\r\n";
	lbLen = 4;
	lbPos = _responseBuffer.find(delimiter);
	if (lbPos == std::string::npos)
	{
		delimiter = "\n\n";
		lbLen = 2;
		lbPos = _responseBuffer.find(delimiter);
		if (lbPos == std::string::npos)
		{
			cgiResponse.createError(INTERNAL_SERVER_ERROR, _server);
			return	;
		}
	}

	if (delimiter == "\r\n\r\n")
		delimiter = "\r\n";
	else if (delimiter == "\n\n")
		delimiter = "\n";

	std::string	headersLine = _responseBuffer.substr(0, lbPos);
	std::string	body = _responseBuffer.substr(lbPos + lbLen);

	cgiHeader(headersLine, delimiter, cgiResponse);
	cgiStatus(cgiResponse);
	cgiBody(body, cgiResponse);
	
	std::cout << "C'est fine. Cgi wo warimashita." << std::endl;
}

//#PARCHE

/*void	CgiExecve::addBytesWritten(size_t bytes)
{
    _bytesWritten += bytes;
}

void	CgiExecve::feed(const char* buffer, size_t size)
{
    // De momento no necesitamos almacenar el output
    // para probar el networking.
    
    //_output.append(buffer, size);
    
    (void)buffer;
    (void)size;
    
    std::cout << "CGI-> Feed: " << size << " bytes" << std::endl;
    std::cout.write(buffer, size);
    std::cout << std::endl;
}*/

