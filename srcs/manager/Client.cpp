/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 13:40:12 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/22 13:40:13 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "Client.hpp"
#include <iostream>

Client::Client(int fd) : _fd(fd), _hasResponse(false), _keepAlive (0) {}
Client::~Client() {}

Client::Client(const Client& other)
{
    *this = other;
} 

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _hasResponse = other._hasResponse;
        _keepAlive = other._keepAlive;
        _parser = other._parser;
        _response = other._response;
    }
    return *this;
}

/*bool Client::receive()
{
    char buffer[4096];

    int bytes = recv(_fd, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
        return false;

    _parser.feed(buffer, bytes);

    return (_parser.isComplete() || _parser.hasError());
}*/

bool Client::receive()
{
    
    // en el Config puden venir hasta 4 timeouts, uno sera el last activity para el recv
    // investigar lo delos timeout

    char buffer[4096];

    int bytes = recv(_fd, buffer, sizeof(buffer), 0);

    std::cout << "Client fd " << _fd << " recv returned " << bytes << std::endl;

    if (bytes <= 0)
        return false;

    std::cout << "Received:\n";
    std::cout.write(buffer, bytes);
    std::cout << std::endl;

    _parser.feed(buffer, bytes);

    return true;
}

RequestParser& Client::getParser()
{
    return _parser;
}

Response Client::getResponse() const
{
    return _response;
}


void Client::setResponse(Response response)
{

    _response = response;
}

bool Client::hasResponse() const
{
    return _hasResponse;
}

bool Client::getKeepAlive() const
{
    return _keepAlive;
}

void Client::clearResponse()
{
    _hasResponse = false;
}