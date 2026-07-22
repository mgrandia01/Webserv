/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserHeaders.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:14:21 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 08:32:41 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

//TODO utils
std::string RequestParser::trimWhitespace(const std::string &str)
{
	size_t start = 0;
	size_t end = str.length();

	while (start < end && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	return str.substr(start, end - start);
}

//TODO utils i posar al hpp
std::string toLower(const std::string &str)
{
    std::string result = str;

    for (size_t i = 0; i < result.size(); ++i)
        result[i] = std::tolower(result[i]);

    return result;
}

bool RequestParser::parseHeaderLine(const std::string &line)
{
	size_t pos = line.find(':');

	if (pos == std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	//TODO: en un futuro rechazar la key si hay espacios en su interior?
	//HTTP/1.1 dice que los headers no son snsibles a las mayusculas, asi que todo es valido
	//con trasfer encodint tbbn pasa
	std::string key = toLower(line.substr(0, pos));
	std::string value = trimWhitespace(line.substr(pos + 1));

	if (key == "transfer-encoding")
		value = toLower(value);

	if (key.empty())
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	_request.headerOccurrences[key]++; //TODO: gracias a esto podremos ver que pasa 
					    //con los headers duplicados
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
			if (!validateHeaders())
			{
				_state = ERROR;
				return;

			}
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
