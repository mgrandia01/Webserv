/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/13 15:37:54 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include "http/HttpSerializer.hpp"

Response::Response(): statusCode(0), reasonPhrase(""), headers(), body(), _stream(){}

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
	statusCode = other.statusCode;
	reasonPhrase = other.reasonPhrase;
	headers = other.headers;
	body = other.body;
	_stream = other._stream;
    }
    return *this;
}

const std::string& Response::getStream() const
{
	_stream = HttpSerializer::serialize(*this);
	return _stream;
}

void Response::setHeaders(const std::string& contentType)
{
        headers["Content-Type"] = contentType;

        std::stringstream ss;
        ss << body.size();
        headers["Content-Length"] = ss.str();

        // TODO
        // response.headers["Date"] = ...
        // response.headers["Server"] = ...
        // response.headers["Connection"] = ...
}


Response Response::createError(HttpStatus status)
{
	Response response;

	HttpStatusInfo info = getStatusInfo(status);

	response.statusCode = status;
	response.reasonPhrase = info.reasonPhrase;
	response.body = info.defaultBody;

	response.setHeaders("text/html");

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
