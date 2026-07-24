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

#include "RequestParser.hpp"
#include "Response.hpp"




class Client
{
public:

    Client(int fd);
    Client(const Client& other);
    Client& operator=(const Client& other);
    ~Client();

    int getFd() const;

    RequestParser& getParser();

    Response getResponse() const;
    void setResponse(Response response);

    /*bool getKeepAlive() const;
    void setKeepAlive(bool keepAlive);
*/    
    bool receive();
    bool hasResponse() const;
    bool getKeepAlive() const;
    void clearResponse();

private:

    Client();
    

    int     _fd;
    bool     _hasResponse;
    bool    _keepAlive;

    RequestParser   _parser;
    Response     _response;

    
};

#endif
