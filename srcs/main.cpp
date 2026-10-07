/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/10/07 13:06:11 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <map>
#include <string>

#include "Config.hpp"
#include "ParserUtils.hpp"
#include "Response.hpp"
#include "ServerManager.hpp"


#include "http/RequestParser.hpp"
#include "http/HttpSerializer.hpp"
#include "CgiExecve.hpp"

#include <cstring>

//1. el main has de restaurarlo
//2. Cgi meterla en un include
//3. Que Cgi funcione con HTTP Request
//4. Que Cgi std::string path, std::string query sean const
//
void    cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request);
//EL listen se sobreescribe, ojo ahi

int main(int argc, char **argv)
{
	if ((argc > 2))
	{
		std::cerr << "Usage: ./webserv [config.conf]\n";
		return (1);
	}
	
	try
	{
		const char *fileName;

		if (argc == 1)
		{
			std::cout << "Loading default file configuration" << std::endl;
			fileName = "config/default.conf";
		}
		else
		{
			std::cout << "Loading " << argv[1] << " configuration file..." << std::endl;
			fileName = argv[1];
		}
		
		Config 		config(fileName);
		/*HttpRequest	tmpRequest;

GET /cgi-bin/hello.py?name=Pepe HTTP/1.1
Host: localhost:8080
Content-Type: application/x-www-form-urlencoded
Content-Length: 9
Connection: keep-alive

name=Pepe
 
		tmpRequest.method = "GET";
		tmpRequest.target = "/cgi-bin/hello.py?name=Pepe";
		tmpRequest.path = "/cgi-bin/hello.py";
		tmpRequest.query = "name=Pepe";
		tmpRequest.version = "HTTP/1.1";

		tmpRequest.headers["host"]           = "localhost:8080";
		tmpRequest.headers["content-type"]   = "application/x-www-form-urlencoded";
		tmpRequest.headers["content-length"] = "9";
		tmpRequest.headers["connection"]     = "keep-alive";

		tmpRequest.headerOccurrences["host"]           = 1;
		tmpRequest.headerOccurrences["content-type"]   = 1;
		tmpRequest.headerOccurrences["content-length"] = 1;
		tmpRequest.headerOccurrences["connection"]     = 1;

		tmpRequest.body = "name=Pepe";
		//std::cout << config << std::endl;
	
		cgiManager(config.getServers()[0], config.getServers()[0].getLocations()[1], tmpRequest);*/

		ServerManager manager(config);
		manager.init();
		manager.printSockets();
		manager.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

/*

bytes = recv(fd, buffer, sizeof(buffer),0));
parser.feed(buffer, bytes, server);

if (parser.hasError())
{
	HttpResponse response = HttpResponse::createError(parser.getErrorCode(), server);
	//TODO mirar si existe una pagina de error para este codigo
	response.applyConfiguredErrorPage(server);
	//TODO serializer:
	std::string raw = HttpSerializer::serialize(response);
	send(fd, raw.c_str(), raw.size(),0);	
}
else if (parser.isComplete())
{
	HttpRequest request = parser.getRequest();
	HttpResponse response = handler.handle(request, server);
	//TODO mirar si existe una pagina de error para este codigo
	response.applyConfiguredErrorPage(server);
	//TODO serializer
	send(respone)
}


 * */
