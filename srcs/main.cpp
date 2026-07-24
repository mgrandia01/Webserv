/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/24 12:32:29 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <string>
#include "Config.hpp"
#include "ServerManager.hpp"
#include "RequestParser.hpp"
#include <map>

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
}*/

#include <cstring>

static void printResult(const std::string &title, RequestParser &parser)
{
    std::cout << "==============================\n";
    std::cout << title << std::endl;
    std::cout << "Complete : " << parser.isComplete() << std::endl;
    std::cout << "Error    : " << parser.hasError() << std::endl;
    std::cout << "Code     : " << parser.getErrorCode() << std::endl;

    if (parser.isComplete())
        std::cout << "Body      : [" << parser.getRequest().body << "]" << std::endl;

    std::cout << "==============================\n\n";
}

int main()
{
    /*
    **********************************************
    ** TEST 1 - Complete Content-Length body
    **********************************************
    */

    {
        RequestParser parser;

        const char *req =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 5\r\n"
            "\r\n"
            "Hello";

        parser.feed(req, strlen(req));

        printResult("TEST 1 - Complete body", parser);
    }

    /*
    **********************************************
    ** TEST 2 - Incomplete body
    **********************************************
    */

    {
        RequestParser parser;

        const char *part1 =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "Hello";

        parser.feed(part1, strlen(part1));

        printResult("TEST 2 - Incomplete body", parser);

        std::cout << "Sending remaining bytes...\n\n";

        parser.feed("World", 5);

        printResult("TEST 2 - After second feed", parser);
    }

    /*
    **********************************************
    ** TEST 3 - Empty body
    **********************************************
    */

    {
        RequestParser parser;

        const char *req =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 0\r\n"
            "\r\n";

        parser.feed(req, strlen(req));

        printResult("TEST 3 - Empty body", parser);
    }

    /*
    **********************************************
    ** TEST 4 - Chunked
    **********************************************
    */

    {
        RequestParser parser;

        const char *req =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n";

        parser.feed(req, strlen(req));

        printResult("TEST 4 - Chunked", parser);
    }

    /*
    **********************************************
    ** TEST 5 - Payload Too Large
    **********************************************
    */

    {
        RequestParser parser;

        const char *req =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 999999999\r\n"
            "\r\n";

        parser.feed(req, strlen(req));

        printResult("TEST 5 - Payload Too Large", parser);
    }

    return 0;
}
