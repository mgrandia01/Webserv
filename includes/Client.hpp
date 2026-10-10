/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 13:37:22 by arcmarti          #+#    #+#             */
/*   Updated: 2026/10/10 11:45:12 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "http/RequestParser.hpp"
#include "Config.hpp"
#include "Response.hpp"

enum TimeoutState
{
    WAITING_REQUEST,
    SENDING_RESPONSE,
    KEEP_ALIVE
};


class Client
{
public:

    Client(int fd);
    Client(const Client& other);
    Client& operator=(const Client& other);
    ~Client();

    int getFd() const;

    RequestParser& getParser();

    const Response& getResponse() const;
    Response& getResponse();
    void setResponse(const Response& response);

    bool getKeepAlive() const;
    void setKeepAlive(bool keepAlive);
    
    bool receive();
    bool hasResponse() const;
    void clearResponse();

    bool isRequestComplete() const;
    bool hasParserError() const;

    void setServerConfig(const ServerConfig* config);
    const ServerConfig* getServerConfig() const;

    
    size_t getBytesSent() const;
    void addBytesSent(size_t bytes);
    void resetBytesSent();

    time_t getLastActivity() const;
    void setLastActivity();

    
    TimeoutState getTimeoutState() const;
    void setTimeoutState(TimeoutState state);

private:

    Client();
    

    int     _fd;
    bool     _hasResponse;
    
    bool    _keepAlive;

    size_t  _bytesSent;


    time_t _lastActivity;
    TimeoutState _timeoutState;

    RequestParser   _parser;
    Response     _response;

    const ServerConfig* _serverConfig;

    

    
};

#endif
