/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserHeaders.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:14:21 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/24 12:37:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

bool RequestParser::parseHeaderLine(const std::string &line)
{
	size_t pos = line.find(':');

	if (pos == std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	std::string key = toLower(line.substr(0, pos));
	std::string value = trimWhitespace(line.substr(pos + 1));
	
	if (key.find(' ') != std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (!isValidHeaderName(key))
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (key == "transfer-encoding")
		value = toLower(value);

	if (key.empty())
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	_request.headerOccurrences[key]++;
	_request.headers[key] = value;

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
			if (!validateHeaders())
			{
				_state = ERROR;
				return;

			}
			
			if (!validateBodySize())
			{
				_errorCode = PAYLOAD_TOO_LARGE;
				_state = ERROR;
				return;

			}

			if (hasBody())
				_state = PARSING_BODY;
			else
				_state = COMPLETE;
			return;
		}

		if (!parseHeaderLine(line))
		{
			_state = ERROR;
			return;
		}
	}
}
