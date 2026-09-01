/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 17:54:20 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/09/01 20:13:32 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"

//cgiManager(server, location, request._request.path, request._request.query)
//Response	cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)

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

	//tmp.push_back("=" + request.);

	return (tmp);
}

void	cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	std::vector<std::string>	environment = obtainEnvVars(server, request);

	std::cout << location.getUri() << std::endl;
	for (size_t i = 0; i < environment.size(); i++)
		std::cout << environment[i] << std::endl;
	/*int	stdin_pipe[2];
	int	stdout_pipe[2];

	pipe(stdin_pipe);
	pipe(stdout_pipe);
	if (method == GET)
	{
		
	}*/
	//pipefd[0] // read
	//pipefd[1] // write
	/*stdin_pipe:
    [0] READ
    [1] WRITE

	stdout_pipe:
    [0] READ
    [1] WRITE*/
}
