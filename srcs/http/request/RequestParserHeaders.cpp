/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserHeaders.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:14:21 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/20 11:33:56 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

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
	if (!validateRequestLineStructure(line))
		return false;

	size_t firstSpace = line.find(' ');
	size_t secondSpace = line.find(' ', firstSpace + 1);

	std::string method = line.substr(0, firstSpace);
	std::string target = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
	std::string version = line.substr(secondSpace + 1);

	// Comprobar que no haya más espacios
	if (version.find(' ') != std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	if (!validateMethod(method))
		return false;
	
	if(!validateTarget(target))
		return false;

	if (!validateVersion(version))
		return false;

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
