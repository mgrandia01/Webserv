/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserHeaders.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:14:21 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 15:17:21 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

bool RequestParser::parseRequestLine(const std::string &line)
{
	size_t firstSpace = line.find(' ');

	if (firstSpace == std::string::npos)
		return false;

	size_t secondSpace = line.find(' ', firstSpace + 1);

	if (secondSpace == std::string::npos)
		return false;

	std::string method = line.substr(0, firstSpace);

	std::string target = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);

	std::string version = line.substr(secondSpace + 1);

	// Comprobar que no haya más espacios
	if (version.find(' ') != std::string::npos)
		return false;

	// Aquí irán las comprobaciones
	// if (!isValidMethod(method))
	//	 return false;

	// if (version != "HTTP/1.1")
	//	 return false;
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
