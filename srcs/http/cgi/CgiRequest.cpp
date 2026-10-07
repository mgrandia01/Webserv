/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:59:00 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/01 16:58:34 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include "CgiRequest.hpp"
#include "ParserUtils.hpp"
#include <iostream>

/* ***************************** constr & destr ***************************** */

CgiRequest::CgiRequest(){}

CgiRequest::~CgiRequest(){}

/* ******************************** get & set ******************************* */

const std::string&	CgiRequest::getPathname() const {return (_pathname);}

const std::vector<std::string>&	CgiRequest::getArgv() const {return (_argv);}

const std::vector<std::string>&	CgiRequest::getEnv() const {return (_env);}

const std::string&	CgiRequest::getBody() const {return (_body);}

const size_t&	CgiRequest::getMethod() const {return (_method);}

const size_t&	CgiRequest::getContentLength() const {return (_contentLength);}

/* ************************* member funcs / methods ************************* */

HttpStatus	CgiRequest::build(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	HttpStatus	statusCode;

	statusCode = buildPathname(location, request);
	if (statusCode != OK)
		return (statusCode);

	statusCode = buildArguments(location, request);
	if (statusCode != OK)
		return (statusCode);

	statusCode = buildEnvironment(server, request);
	if (statusCode != OK)
		return (statusCode);

	BodyCgiFunc(request);		
	MethodCgiFunc(request);
	ContentLengthCgiFunc(request);

	return (statusCode);
}


HttpStatus	CgiRequest::buildPathname(const LocationConfig& location, const HttpRequest& request)
{
	const std::map<std::string, std::string>&   cgiMap = location.getCgi();
	const std::string&							fileCgi = request.path;
	std::string::size_type						pos = fileCgi.find_last_of('.');

	if (pos == std::string::npos)
		return (BAD_REQUEST);//no hay '.' para una extension

	std::string										extension = fileCgi.substr(pos);
	std::map<std::string, std::string>::const_iterator	it = cgiMap.find(extension);

	if (it == cgiMap.end())
		return (NOT_IMPLEMENTED);//NO hay esa extension


	HttpStatus	statusCode = validatePathname(it->second);

	if (statusCode != OK)
		return (statusCode);

	_pathname = it->second;

	return (OK);
}

HttpStatus	CgiRequest::validatePathname(const std::string& compiler)
{
	if (access(compiler.c_str(), F_OK) != 0)
		return (INTERNAL_SERVER_ERROR);//NO existe el compiler
	else if(access(compiler.c_str(), R_OK | X_OK) != 0)
		return (INTERNAL_SERVER_ERROR);//NO puede ejecutar el compiler
	
	return (OK);
}

HttpStatus	CgiRequest::buildArguments(const LocationConfig& location, const HttpRequest& request)
{
	const std::string&			compiler = getPathname();
	const std::string			root = location.getRoot();
	const std::string&			uriPath =  request.path;
	std::string					cgiFile =  root + uriPath;

	HttpStatus	statusCode = validateArguments(root, uriPath, cgiFile);

	if (statusCode != OK)
		return (statusCode);

	_argv.push_back(compiler);
	_argv.push_back(cgiFile);

	return (OK);
}

HttpStatus	CgiRequest::validateArguments(const std::string& root,
									const std::string& uriPath,
									std::string& cgiFile)
{
	if (root.size() == 0)
		return (INTERNAL_SERVER_ERROR);
	else if (root.compare(0, 1, ".") != 0 && root.compare(0, 1, "/") != 0 && root.compare(0, 2, "./") != 0)
		return (INTERNAL_SERVER_ERROR);
	else if (root.compare(0, 2, "./") == 0)
		cgiFile = "." + uriPath;

	if (uriPath.size() == 0)
		return (BAD_REQUEST);
	else if (uriPath.compare(0, 1, "/") != 0)
		return (BAD_REQUEST);

	if (access(cgiFile.c_str(), F_OK) != 0)
		return (NOT_FOUND);//NO existe esta file
	else if (access(cgiFile.c_str(), X_OK) != 0) 
		return (FORBIDDEN);//NO puede ejecutar la file

	return (OK);
}

HttpStatus	CgiRequest::buildEnvironment(const ServerConfig& server, const HttpRequest& request)
{
	std::vector<std::string>							tmp;
	std::map<std::string, std::string>::const_iterator	it;

	tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");

	tmp.push_back("REQUEST_URI=" + request.target);
	tmp.push_back("REQUEST_METHOD=" + request.method);
	tmp.push_back("QUERY_STRING=" + request.query);
	tmp.push_back("SERVER_PROTOCOL=" + request.version);
	tmp.push_back("SCRIPT_NAME=" + request.path);//ver que el request.path coincida con el location block que me han pasado

	it = request.headers.find("host");
	if (it != request.headers.end())
		tmp.push_back("HTTP_HOST=" + it->second);
	else
		tmp.push_back("HTTP_HOST=");
	size_t	pos = it->second.find(':');
	tmp.push_back("SERVER_NAME=" + it->second.substr(0, pos));//VER que el el host(antes de ':') de client request coincida con algun server_name del server
	tmp.push_back("SERVER_PORT=" + it->second.substr(pos + 1));


	it = request.headers.find("content-type");
	if (it != request.headers.end())
		tmp.push_back("CONTENT_TYPE=" + it->second);
	else
		tmp.push_back("CONTENT_TYPE=");

	it = request.headers.find("content-length");
	if (it != request.headers.end())
		tmp.push_back("CONTENT_LENGTH=" + it->second);
	else
		tmp.push_back("CONTENT_LENGTH=");

	tmp.push_back("SERVER_SOFTWARE=Webserv");
	tmp.push_back("REMOTE_ADDR=" + server.getHost());

	_env = tmp;
	return (OK);
}

void    CgiRequest::BodyCgiFunc(const HttpRequest& request)
{
	_body = request.body;
}

void    CgiRequest::MethodCgiFunc(const HttpRequest& request)
{
	if (request.method == "GET" || request.method == "get")
		_method = GET;
	else if (request.method == "POST" || request.method == "post")
		_method = POST;
	else if (request.method == "DELETE" || request.method == "delete")
		_method = DELETE;
}

void	CgiRequest::ContentLengthCgiFunc(const HttpRequest& request)
{
	std::string	str;
	std::map<std::string, std::string>::const_iterator it = request.headers.find("content-length");
	
	if (it != request.headers.end())
		str = it->second;
	
	if (!str.empty())
	{
		size_t	start = str.find_first_of("0123456789");
		
		if (start != std::string::npos)
		{
			size_t	end = str.find_first_not_of("0123456789");

			std::string	num = str.substr(start, end - start);
			_contentLength = std::strtoul(num.c_str(), NULL, 10);
		}
		
	}
	else
		_contentLength = 0;

}
