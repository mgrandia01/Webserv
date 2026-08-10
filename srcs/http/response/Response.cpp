/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/05 15:27:01 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "http/HttpResponse.hpp"
#include <string>

Response::Response(){}

Response::Response(std::string stream) : _stream(stream){}

Response::~Response(){}

Response::Response(const Response& other)
{

	*this = other;
}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
        _stream = other._stream;
    }
    return *this;
}

const std::string& Response::getStream() const
{
	return _stream;
}

HttpResponse HttpResponse::createError(HttpStatus status)
{
	HttpResponse response;

	HttpStatusInfo info = getStatusInfo(status);

	response.statusCode = status;
	response.reasonPhrase = info.reasonPhrase;
	response.body = info.defaultBody;

	return response;
}

/*
void HttpResponse::applyConfiguredErrorPage(const ServerConfig& server)
{

	// Buscar si server tiene una error_page para mi status
	// Si existe:
	//      _body = contenido del fichero
	// Si no:
	//      dejar _body como está (la página por defecto)

}*/
