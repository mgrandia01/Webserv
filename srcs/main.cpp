/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/21 13:55:56 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <string>
#include "Config.hpp"
#include "ServerManager.hpp"
#include "RequestParser.hpp"

#include <iostream>
#include <map>

#include "RequestParser.hpp"

#include <iostream>
#include "RequestParser.hpp"

void test(const std::string &title, const std::string &req)
{
    std::cout << "==============================" << std::endl;
    std::cout << title << std::endl;
    std::cout << "==============================" << std::endl;

    RequestParser parser;

    parser.feed(req.c_str(), req.size());

    if (parser.hasError())
    {
        std::cout << "ERROR " << parser.getErrorCode() << std::endl;
        return;
    }

    if (parser.isComplete())
    {
        Request r = parser.getRequest();

        std::cout << "Method : " << r.method << std::endl;
        std::cout << "Target : " << r.target << std::endl;
        std::cout << "Version: " << r.version << std::endl;

        std::cout << "\nHeaders:" << std::endl;
        for (std::map<std::string, std::string>::const_iterator it = r.headers.begin();
             it != r.headers.end(); ++it)
        {
            std::cout << "  [" << it->first << "] = [" << it->second << "]" << std::endl;
        }

        std::cout << "\nHeader occurrences:" << std::endl;
        for (std::map<std::string, size_t>::const_iterator it = r.headerOccurrences.begin();
             it != r.headerOccurrences.end(); ++it)
        {
            std::cout << "  [" << it->first << "] = " << it->second << std::endl;
        }
    }
    else
    {
        std::cout << "INCOMPLETE" << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    // Caso válido
    test("VALID REQUEST",
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "User-Agent: curl\r\n"
        "\r\n");

    // Host duplicado
    test("DUPLICATE HOST",
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Host: example.com\r\n"
        "\r\n");

    // Sin Host
    test("MISSING HOST",
        "GET / HTTP/1.1\r\n"
        "User-Agent: curl\r\n"
        "\r\n");

    return 0;
}

/*
void test(const std::string &req)
{
	RequestParser parser;

	parser.feed(req.c_str(), req.size());

	if (parser.hasError())
	{
		std::cout << "ERROR " << parser.getErrorCode() << std::endl;
		return;
	}

	if (parser.isComplete())
	{
		Request r = parser.getRequest();

		std::cout << "Method : " << r.method << std::endl;
		std::cout << "Target : " << r.target << std::endl;
		std::cout << "Version: " << r.version << std::endl;

		std::cout << "Headers:" << std::endl;

		for (std::map<std::string, std::string>::const_iterator it = r.headers.begin();
			it != r.headers.end(); ++it)
		{
			std::cout << "  [" << it->first << "] = [" << it->second << "]" << std::endl;
		}
	}
	else
	{
		std::cout << "INCOMPLETE" << std::endl;
	}
}

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
}*/

