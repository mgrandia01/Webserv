/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/04 13:17:50 by mgrandia         ###   ########.fr       */
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
