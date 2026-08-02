/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:39:28 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/01 11:39:30 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <sstream>
#include "RequestHandler.hpp"


RequestHandler::RequestHandler()
{
	
}
	
RequestHandler::RequestHandler(const RequestHandler &other)
{
	*this = other;
}

RequestHandler &RequestHandler::operator=(const RequestHandler &other)
{
	if (this != &other)
	{
		
	}
	return *this;
}

RequestHandler::~RequestHandler()
{
}

void RequestHandler::handle(const Request& request, const ServerConfig& server, Response& response)
{
    (void)request;


    std::cout << "Handling request with server: " << server.getServerName() << std::endl;

    /*std::string body(100 * 1024 * 1024, 'A');

std::ostringstream oss;
oss << "HTTP/1.1 200 OK\r\n";
oss << "Content-Length: " << body.size() << "\r\n";
oss << "Connection: close\r\n";
oss << "\r\n";
oss << body;

response = Response(oss.str());*/

    // temporal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    response = Response("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello");
}