/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/07 11:19:49 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"
#include <iostream>

RequestParser::RequestParser()
{
	_state = PARSING_HEADERS;
	_errorCode = NO_ERROR;
	_requestLineParsed = false;
	_contentLength = 0;
	_chunked = false;
	_clientMaxBodySize = 1024*1024;//TODO es un parcheeee
	_server = NULL;
}
	
RequestParser::RequestParser(const RequestParser &other)
{
	*this = other;
}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
	if (this != &other)
	{
		_state = other._state;
		_errorCode = other._errorCode;
		_stream = other._stream;
		_request = other._request;
		_requestLineParsed = other._requestLineParsed;
		_contentLength = other._contentLength;
		_chunked = other._chunked;
		_server = other._server;
	}
	return *this;
}

RequestParser::~RequestParser()
{
}

//TODO lo uso? 
void RequestParser::reset()
{
	_state = PARSING_HEADERS;
	_errorCode = NO_ERROR;
	_requestLineParsed = false;
	_stream.clear();
	_request = HttpRequest();
	_contentLength = 0;
	_chunked = false;
	_server = NULL;
}

HttpRequest RequestParser::getRequest() const
{
	return _request;
}

HttpStatus RequestParser::getErrorCode() const
{
	return _errorCode;
}

bool RequestParser::hasError() const
{
	return (_state == ERROR);
}

bool RequestParser::isComplete() const
{
	return (_state == COMPLETE);
}

void RequestParser::feed(const char *buffer, size_t bytes, const ServerConfig& server)
{
	_server = &server;
	_stream.append(buffer, bytes);

	if (_state == PARSING_HEADERS)
		parseHeaders();

	if (_state == PARSING_BODY)
		parseBody();
}
