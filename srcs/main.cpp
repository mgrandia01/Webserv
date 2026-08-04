/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/04 11:45:29 by mgrandia         ###   ########.fr       */
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


int main()
{
 /*   std::string rawRequest =
        "GET /noexist.html HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
*/
    // Para probar POST:
   
   /* std::string rawRequest =
        "POST /upload HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello World!";
    
*/
    // Para probar DELETE:
   
    std::string rawRequest =
        "DELETE /index.html HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
   

    RequestParser parser;
    parser.feed(rawRequest.c_str(), rawRequest.size());

    HttpRequest request = parser.getRequest();

    HttpHandler handler;
    HttpResponse response = handler.handle(request);

    std::cout << "===== REQUEST =====" << std::endl;
    std::cout << "Method: " << request.method << std::endl;
    std::cout << "Path:   " << request.path << std::endl;
    std::cout << "Query:  " << request.query << std::endl;

    std::cout << "\n===== RESPONSE =====" << std::endl;
    std::cout << "Status: "
              << response.statusCode
              << " "
              << response.reasonPhrase
              << std::endl;

    std::cout << "\nHeaders:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = response.headers.begin();
         it != response.headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "\nBody:" << std::endl;
    std::cout << response.body << std::endl;

    return 0;
}



/*
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
*/
