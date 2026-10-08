/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecve.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:13:31 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/08 13:26:33 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_EXECVE_HPP
# define CGI_EXECVE_HPP

# include "CgiRequest.hpp"
# include "ServerConfig.hpp"

typedef enum	e_pipe
{
	READ_PIPE = 0,
	WRITE_PIPE
}	t_pipe;

class	CgiExecve
{
	public:
		CgiExecve(const ServerConfig& server);
		~CgiExecve();

		CgiRequest&			getVars();
		ssize_t				getPid();
		int					getClientFd();
		int					getWriteFd();
		int					getReadFd();
		size_t				getBytesWritten();
		const std::string&	getResponseBuffer() const;
		bool				getInputClosed();
		bool				getIsFinished();
		time_t				getStartTime();

		void	setClientFd(int fd);

		bool	writeToCgi();
		bool	readFromCgi();
		void	collectProcess();
		void	closeInFd();
		void	closeOutFd();
	
		HttpStatus	cgiExecveFunc();
		void		cgiOutputParser(Response& cgiResponse);

		//#PARCHE
		void	addBytesWritten(size_t bytes);
		void	feed(const char* buffer, size_t size);


	private:

		const ServerConfig&	_server;	
		CgiRequest			_vars;	
		pid_t				_pid;
		int					_clientFd;
		int					_serverToCgi[2];
		int					_cgiToServer[2];
		int					_inFd;//_serverToCgi[WRITE]
		int					_outFd;//_cgiToServer[READ]
		size_t      		_bytesWritten;
		std::string			_responseBuffer;
		bool				_inputClosed;
		bool				_isFinished;
		time_t				_startTime;

		void	childManager();
		void	parentManager();

		std::vector<char *>	vectorToCharPtr(const std::vector<std::string>& vec);

		std::vector<std::string>	strSplitStr(std::string& headersLine, std::string& delimiter);//
		std::map<std::string, std::string>	headersToMap(std::vector<std::string> headers);
		void	cgiHeader(std::string& headersLine, std::string delimiter, Response& cgiResponse);
		void	cgiStatus(Response& cgiResponse);
		void	cgiBody(std::string& body, Response& cgiResponse);
};

#endif
