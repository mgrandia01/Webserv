/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 12:07:15 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <string>
#include "Config.hpp"
#include "Response.hpp"
#include "ServerManager.hpp"
#include "RequestParser.hpp"
#include <map>


/*
void test(const std::string &title, const std::string &req)
{
	std::cout << "==============================" << std::endl;
	std::cout << title << std::endl;
	std::cout << "==============================" << std::endl;

	RequestParser parser;

	parser.feed(req.c_str(), req.size());

	if (parser.hasError())
	{
		std::cout << "ERROR CODE: "
		          << parser.getErrorCode()
		          << std::endl;
		std::cout << std::endl;
		return;
	}

	if (!parser.isComplete())
	{
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << std::endl;
		return;
	}

	Request r = parser.getRequest();

	std::cout << "Method : " << r.method << std::endl;
	std::cout << "Target : " << r.target << std::endl;
	std::cout << "Version: " << r.version << std::endl;

	std::cout << "\nHeaders:" << std::endl;

	for (std::map<std::string, std::string>::const_iterator it =
			r.headers.begin();
			it != r.headers.end(); ++it)
	{
		std::cout << "  [" << it->first << "] = ["
		          << it->second << "]"
		          << std::endl;
	}

	std::cout << "\nOccurrences:" << std::endl;

	for (std::map<std::string, size_t>::const_iterator it =
			r.headerOccurrences.begin();
			it != r.headerOccurrences.end(); ++it)
	{
		if (it->second > 0)
		{
			std::cout << "  [" << it->first << "] = "
			          << it->second
			          << std::endl;
		}
	}

	std::cout << std::endl;
}*/

/*
int main()
{
	*
	 * ===========================
	 * HOST
	 * ===========================
	 

	test("VALID HOST",
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"\r\n");


	test("MISSING HOST",
		"GET / HTTP/1.1\r\n"
		"User-Agent: curl\r\n"
		"\r\n");


	test("EMPTY HOST",
		"GET / HTTP/1.1\r\n"
		"Host:\r\n"
		"\r\n");


	test("DUPLICATE HOST",
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Host: example.com\r\n"
		"\r\n");


	*
	 * ===========================
	 * CONTENT LENGTH
	 * ===========================
	 *

	test("VALID CONTENT LENGTH",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 42\r\n"
		"\r\n");


	test("ZERO CONTENT LENGTH",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 0\r\n"
		"\r\n");


	test("EMPTY CONTENT LENGTH",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length:\r\n"
		"\r\n");


	test("CONTENT LENGTH LETTERS",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: abc\r\n"
		"\r\n");


	test("CONTENT LENGTH NEGATIVE",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: -10\r\n"
		"\r\n");


	test("DUPLICATE CONTENT LENGTH",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 10\r\n"
		"Content-Length: 20\r\n"
		"\r\n");


	test("CONTENT LENGTH OVERFLOW",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 999999999999999999999999999999999999999999999\r\n"
		"\r\n");


	*
	 * ===========================
	 * TRANSFER ENCODING
	 * ===========================
	 *

	test("VALID TRANSFER ENCODING",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n");


	test("EMPTY TRANSFER ENCODING",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Transfer-Encoding:\r\n"
		"\r\n");


	test("INVALID TRANSFER ENCODING",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Transfer-Encoding: gzip\r\n"
		"\r\n");


	test("DUPLICATE TRANSFER ENCODING",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Transfer-Encoding: chunked\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n");


	*
	 * ===========================
	 * FRAMING CONFLICT
	 * ===========================
	 *

	test("CONTENT LENGTH + TRANSFER ENCODING",
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 10\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n");


	return 0;
}*/

/*void test(const std::string &title, const std::string &req)
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
}*/

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
*/
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

