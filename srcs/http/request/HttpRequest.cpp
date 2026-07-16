/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:16:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 11:50:05 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <iostream>

RequestParser::RequestParser()
{
    _state = PARSING_HEADERS;
    _requestLineParsed = false;
    _contentLength = 0;
    _chunked = false;
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

void RequestParser::parseBody()
{
  //TODO:

    _request.body = _stream;
    _stream.clear();

    _state = COMPLETE;
}

bool RequestParser::parseHeaderLine(const std::string &line)
{
  //TODO:
    size_t pos = line.find(':');

    if (pos == std::string::npos)
        return false;

    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    _request.headers[key] = value;

    return true;
}

bool RequestParser::parseRequestLine(const std::string &line)
{
    size_t firstSpace = line.find(' ');

    if (firstSpace == std::string::npos)
        return false;

    size_t secondSpace = line.find(' ', firstSpace + 1);

    if (secondSpace == std::string::npos)
        return false;

    std::string method = line.substr(0, firstSpace);

    std::string target = line.substr(firstSpace + 1,
                                  secondSpace - firstSpace - 1);

    std::string version = line.substr(secondSpace + 1);

    // Comprobar que no haya más espacios
    if (version.find(' ') != std::string::npos)
        return false;

    // Aquí irán las comprobaciones
    // if (!isValidMethod(method))
    //     return false;

    // if (version != "HTTP/1.1")
    //     return false;
  //
  //TODO:
// - Validate supported HTTP methods.
// - Validate HTTP version.
// - Validate request-target according to RFC 9112.

    _request.method = method;
    _request.target = target;
    _request.version = version;

    return true;
}

void RequestParser::parseHeaders()
{
	while (true)
	{
		size_t pos = _stream.find("\r\n");

		if (pos == std::string::npos)
			return;

		std::string line = _stream.substr(0, pos);

		_stream.erase(0, pos + 2);

		if (!_requestLineParsed)
		{
			if (!parseRequestLine(line))
			{
				_state = ERROR;
				return;
			}

			_requestLineParsed = true;
			continue;
		}

		if (line.empty())
		{
			_state = PARSING_BODY;
			return;
		}

		if (!parseHeaderLine(line))
		{
			_state = ERROR;
			return;
		}
	}
}

void RequestParser::feed(const char *buffer, size_t bytes)
{
	_stream.append(buffer, bytes);

	if (_state == PARSING_HEADERS)
		parseHeaders();

	if (_state == PARSING_BODY)
		parseBody();
}
