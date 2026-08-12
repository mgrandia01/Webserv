/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/10 14:42:42 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <map>
#include <string>

#include "Config.hpp"
#include "Response.hpp"
#include "ServerManager.hpp"


#include "http/RequestParser.hpp"
#include "http/HttpHandler.hpp"
#include "http/HttpSerializer.hpp"

#include <cstring>


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
		
		Config config(fileName);

		std::cout << config << std::endl;


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
