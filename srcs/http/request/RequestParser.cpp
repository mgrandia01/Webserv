/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 15:19:18 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include <iostream>

RequestParser::RequestParser()
{
	_state = PARSING_HEADERS;
	_requestLineParsed = false;
	_contentLength = 0;
	_chunked = false;
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
		_stream = other._stream;
		_request = other._request;
		_requestLineParsed = other._requestLineParsed;
		_contentLength = other._contentLength;
		_chunked = other._chunked;
	}
	return *this;
}

RequestParser::~RequestParser()
{
}

void RequestParser::reset()
{
  //TODO:
  //
	_state = PARSING_HEADERS;

	_requestLineParsed = false;

	_stream.clear();

	_request = Request();

	_contentLength = 0;

	_chunked = false;
}

Request RequestParser::getRequest() const
{
  //TODO:
  //
	return _request;
}
bool RequestParser::hasError() const
{
  //TODO:
  //
	return (_state == ERROR);
}

bool RequestParser::isComplete() const
{
  //TODO:
  //
	return (_state == COMPLETE);
}



void RequestParser::feed(const char *buffer, size_t bytes)
{
	_stream.append(buffer, bytes);

	if (_state == PARSING_HEADERS)
		parseHeaders();

	if (_state == PARSING_BODY)
		parseBody();
}
