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
#include <ctime>

Client::Client(int fd) : _fd(fd), _hasResponse(false), _keepAlive (false), _bytesSent(0), _lastActivity(time(NULL)),
                        _timeoutState(WAITING_HEADERS), _parser(), _response(), _serverConfig(NULL){}
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
        _bytesSent = other._bytesSent;
        _lastActivity = other._lastActivity;
        _timeoutState = other._timeoutState;
        _parser = other._parser;
        _response = other._response;
        _serverConfig = other._serverConfig;
    }
    return *this;
}

int Client::getFd() const
{
    return _fd;
}

bool Client::receive()
{
    
    char buffer[4096];

    int bytes = recv(_fd, buffer, sizeof(buffer), 0);

    std::cout << "Client fd " << _fd << " recv returned " << bytes << std::endl;

    if (bytes <= 0)
        return false;

    std::cout << "Received:\n";
    std::cout.write(buffer, bytes);
    std::cout << std::endl;

    _parser.feed(buffer, bytes);
    setLastActivity();

    if (!_parser.isComplete())
        _timeoutState = WAITING_HEADERS;
    
    //para el CGI podria ser necesario diferenciar el body y entonces aprovchamos aqui
    /*else if (_parser.hasBody())
        _timeoutState = RECEIVING_BODY;
    else
        _timeoutState = WAITING_HEADERS;*/

    return true;
}

RequestParser& Client::getParser()
{
    return _parser;
}

const Response& Client::getResponse() const
{
    return _response;
}


void Client::setResponse(const Response& response)
{
    _response = response;
    _hasResponse = true;
    _bytesSent = 0;
    _timeoutState = SENDING_RESPONSE;
}

bool Client::hasResponse() const
{
    return _hasResponse;
}

bool Client::getKeepAlive() const
{
    return _keepAlive;
}

void Client::setKeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}

void Client::clearResponse()
{
    _hasResponse = false;
}

bool Client::isRequestComplete() const
{
    return _parser.isComplete();
}

bool Client::hasParserError() const
{
    return _parser.hasError();
}

void Client::setServerConfig(const ServerConfig* config)
{
    _serverConfig = config;
}


const ServerConfig* Client::getServerConfig() const
{
    return _serverConfig;
}

size_t Client::getBytesSent() const
{
    return _bytesSent;
}

void Client::addBytesSent(size_t bytes)
{
    _bytesSent += bytes;
}

void Client::resetBytesSent()
{
    _bytesSent = 0;
}

time_t Client::getLastActivity() const
{
    return _lastActivity;
}


void Client::setLastActivity()
{
    _lastActivity = time(NULL);
}

TimeoutState Client::getTimeoutState() const
{
    return _timeoutState;
}


void Client::setTimeoutState(TimeoutState state)
{
    _timeoutState = state;
}