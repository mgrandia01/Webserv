/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:59:00 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/09/11 21:08:06 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"
#include "CgiRequest.hpp"
#include <iostream>

/* ***************************** constr & destr ***************************** */

CgiRequest::CgiRequest(){}

CgiRequest::~CgiRequest(){}

/* ******************************** get & set ******************************* */

const std::string&	CgiRequest::getPathname() const {return (_pathname);}

const std::vector<std::string>&	CgiRequest::getArgv() const {return (_argv);}

const std::vector<std::string>&	CgiRequest::getEnv() const {return (_env);}

const std::string&	CgiRequest::getBody() const {return (_body);}

/* ************************* member funcs / methods ************************* */

bool	CgiRequest::build(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	if (!buildPathname(location, request))
		return (false);//Que devolver si falla, throw?
	if (!buildArguments(location, request))
		return (false);
	if (!buildEnvironment(server, request))
		return (false);
	_body = request.body;

	return (true);
}


bool	CgiRequest::buildPathname(const LocationConfig& location, const HttpRequest& request)
{
	const std::map<std::string, std::string>&   cgiMap = location.getCgi();
	const std::string&							compiler = request.path;
	std::string::size_type						pos = compiler.find_last_of('.');

	if (pos == std::string::npos)
		return (false);//no hay '.' para una extension

	std::string										extension = compiler.substr(pos);
	std::map<std::string, std::string>::const_iterator	it = cgiMap.find(extension);

	if (it == cgiMap.end())
		return (false);//NO hay esa extension

	_pathname = it->second;
	return (true);
}

bool	CgiRequest::validatePathname(const std::string& compiler)
{
	if (access(compiler.c_str(), F_OK) != 0)
		return (false);//NO existe el compiler
	else if(access(compiler.c_str(), R_OK | X_OK) != 0)
		return (false);//NO puede ejecutar el compiler
	
	return (true);
}

bool	CgiRequest::buildArguments(const LocationConfig& location, const HttpRequest& request)
{
	const std::string&			compiler = getPathname();
	const std::string			root = location.getRoot();
	const std::string&			uriPath =  request.path;
	std::string					cgiFile =  root + uriPath;

	if (!validateArguments(root, uriPath, cgiFile))
		return (false);

	_argv.push_back(compiler);
	_argv.push_back(cgiFile);

	return (true);
}

bool	CgiRequest::validateArguments(const std::string& root,
									const std::string& uriPath,
									std::string& cgiFile)
{
	if (root.size() == 0)
		return (false);
	else if (root.compare(0, 1, ".") != 0 && root.compare(0, 1, "/") != 0 && root.compare(0, 2, "./") != 0)
		return (false);
	else if (root.compare(0, 2, "./") == 0)
		cgiFile = "." + uriPath;

	if (uriPath.size() == 0)
		return (false);
	else if (uriPath.compare(0, 1, "/") != 0)
		return (false);

	if (access(cgiFile.c_str(), F_OK) != 0)
		return (false);//NO existe esta file
	else if (access(cgiFile.c_str(), X_OK) != 0) 
		return (false);//NO puede ejecutar la file

	return (true);
}

bool	CgiRequest::buildEnvironment(const ServerConfig& server, const HttpRequest& request)
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
	return (true);
}

