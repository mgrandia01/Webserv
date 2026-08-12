/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 13:37:22 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/22 13:37:23 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "http/RequestParser.hpp"
#include "http/HttpResponse.hpp"
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

    //void setHasResponse(bool flag);
    
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
    
    // TO DO se instancia desde el parserrrrrrrrrrrrrrrrrrrrrrrrrrr
    bool    _keepAlive;

    size_t  _bytesSent;


    time_t _lastActivity;
    TimeoutState _timeoutState;

    RequestParser   _parser;
    Response     _response;

    //TODO hay uno, pero habra que poner un vector de candidatos
    const ServerConfig* _serverConfig;

    

    
};

#endif
