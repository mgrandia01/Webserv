/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/05 13:05:21 by mgrandia         ###   ########.fr       */
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
#include "http/HttpResponse.hpp"
#include "http/HttpHandler.hpp"


#include <cstring>

int main(int argc, char **argv)
{
    


	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv config.conf\n";
		return (1);
	}
	
	try
	{
		Config config(argv[1]);
		
		const std::vector<ServerConfig>& servers = config.getServers();
		
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::cout << "Server " << i << std::endl;
			std::cout << "Host : " << servers[i].getHost() << std::endl;
			std::cout << "Port : " << servers[i].getPort() << std::endl;
			std::cout << "Name : " << servers[i].getServerName() << std::endl;
			std::cout << "Root : " << servers[i].getRoot() << std::endl;
			std::cout << std::endl;
		}
		
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
	HttpResponse response = HttpResponse::createError(parser.getErrorCode(server));
	//TODO mirar si existe una pagina de error para este codigo
	ResponseFinalizer::finalize(response, server);
	//TODO serializer:
	std::string raw = HttpSerializer::serialize(response);
	send(fd, raw.c_str(), raw.size(),0);	
}
else if (parser.isComplete())
{
	HttpRequest request = parser.getRequest();
	HttpResponse response = handler.handle(request, server);
	//TODO mirar si existe una pagina de error para este codigo
	ResponseFinalizer::finalize(response, server);
	//TODO serializer
	send(respone)
}


 * */
