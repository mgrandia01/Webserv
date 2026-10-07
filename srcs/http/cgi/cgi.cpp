/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 17:54:20 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/02 18:02:28 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include "CgiRequest.hpp"
#include "CgiExecve.hpp"
#include "Response.hpp"

Response cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	/*
		Has de crear los datos
		Hacer ejecucion del hijo
		Anexar el new cgi al Response para poder comunicarte con el Server non blocking
		Devolver la Response (Preguntar a Marta como montarla bien)	
	*/
	HttpStatus	statusCode;
	Response	responseCgi;
	CgiExecve	*cgi = new CgiExecve();

	statusCode = cgi->getVars().build(server, location, request);
	if (statusCode != OK)
		return responseCgi.createError(statusCode, server);
	
	statusCode = cgi->cgiExecveFunc();
	if (statusCode != OK)
		return responseCgi.createError(statusCode, server);

	//Montar Response y anadir el cgi, quizas encapsularlo
	responseCgi.setCgi(cgi);
	/*
											cgi trigger (aqui)
												↑
		ServerManger	-> HttpHandler -> genera una Response ↙
						-> Client <- se le asigna la Response
		
		ServerManger usa el cgi que voy a crear aqui
		Comunicar a Arcadio que he de crearlo yo porque ha de anexarlo al Response

	*/
	return (responseCgi);
	/*
		Hacer writtenFunc para cuando tenga permisos
		Hacer ReadFunc para cuando tenga permisos
		Matar al hijo en el detructor

		Comunicar a Arcadio que yo hago el new
		Preguntar a Marta como hacer bien la Reponse(no solo Response::error)
	*/
}
	
/*#include <iostream>
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"
#include <signal.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>

std::string	obtainExecPathname(const std::string& path, const std::map<std::string, std::string>& cgi)
{
	std::string::size_type	pos = path.find_last_of('.');

	if (pos == std::string::npos)
		return ("");//no hay '.' para una extension

	std::string										tmpExt = path.substr(pos);
	std::map<std::string, std::string>::const_iterator	it = cgi.find(tmpExt);

	if (it == cgi.end())
		return ("");//NO hay esa extension

	return (it->second);
}

std::vector<char *>	obtainEnvChar(std::vector<std::string>& environment)
{
	std::vector<char *>	tmpEnv;


	for (std::vector<std::string>::iterator it = environment.begin();
			it != environment.end(); it++)
		tmpEnv.push_back(const_cast<char *>(it->c_str()));
	tmpEnv.push_back(NULL);
	return (tmpEnv);
}

std::vector<std::string>	obtainEnvVars(const ServerConfig& server, const HttpRequest& request)
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

	return (tmp);
}

//cgiManager(server, location, request._request.path, request._request.query)
//Response	cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
void	cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	//OJO!!!
	//comprobar antes si el request.paht pide cgi y si el servidor maneja cgi
	std::vector<std::string>	environment = obtainEnvVars(server, request);

	int		serverToCgi[2];
	int		cgiToServer[2];
	pid_t	pid;

	pipe(serverToCgi);
	pipe(cgiToServer);
	pid = fork();

	std::cout << "PID: " << pid << std::endl;
	if (pid == 0)
	{
		dup2(serverToCgi[READ_PIPE], STDIN_FILENO);
		dup2(cgiToServer[WRITE_PIPE], STDOUT_FILENO);

		close(serverToCgi[READ_PIPE]);
		close(serverToCgi[WRITE_PIPE]);

		close(cgiToServer[READ_PIPE]);
		close(cgiToServer[WRITE_PIPE]);

		const std::map<std::string, std::string>&	tmpCgi = location.getCgi();
		std::string									pathname = obtainExecPathname(request.path, tmpCgi);
		char										*uriArgv[3];
		std::vector<char *>							tmpEnv = obtainEnvChar(environment);

	
		uriArgv[0] = const_cast<char *>(pathname.c_str());
		uriArgv[1] = const_cast<char *>(request.path.c_str());
		uriArgv[2] = NULL;

		execve(pathname.c_str(), uriArgv, &tmpEnv[0]);
		std::cerr << getpid() << "HELLO execve failed: " << strerror(errno) << std::endl;
		exit(1);	
	}
	else
	{
		close(serverToCgi[READ_PIPE]);
		close(serverToCgi[WRITE_PIPE]);

		close(cgiToServer[READ_PIPE]);
		close(cgiToServer[WRITE_PIPE]);
	}

	std::cerr << getpid() << " waitpid\n";
	if (pid > 0)
	{
		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);
	}

	//stdin_pipe:
    //[0] READ_PIPE
    //[1] WRITE_PIPE
	//stdout_pipe:
    //[0] READ_PIPE
    //[1] WRITE_PIPE
}*/
