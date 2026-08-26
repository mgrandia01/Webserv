/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserStartLineValidation.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:19:10 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/11 10:48:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"
#include "http/HttpStatus.hpp"

bool RequestParser::validateTarget(const std::string &target)
{
	if (target.empty())
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	if (target[0] != '/')
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	for(size_t i = 0; i < target.size(); i++)
	{
		if (iscntrl(target[i]))
		{
			_errorCode = BAD_REQUEST;
			return false;
		}
	}

	for (size_t i = 0; i < target.size(); ++i)
	{
		if (target[i] == '%')
		{
			if (i + 2 >= target.size())
			{
			_errorCode = BAD_REQUEST;
			return false;
			}

			if (!isxdigit(static_cast<unsigned char>(target[i + 1])) || !isxdigit(static_cast<unsigned char>(target[i + 2])))
			{
				_errorCode = BAD_REQUEST;
				return false;
			}
			i += 2;
		}
	}
	return true;
}

bool RequestParser::validateVersion(const std::string &version)
{
	if (version != "HTTP/1.1")
	{
		_errorCode = HTTP_VERSION_NOT_SUPPORTED;
		return false;
	}
	return true;
}

bool RequestParser::validateMethod(const std::string &method)
{/*
	const LocationConfig* location = findLocation();
	if (!location)
	{
		std::cout << "EROOOOORRRRR----------------------metodo usado: " <<method << std::endl;
		_errorCode = NOT_FOUND;
		return false;
	}
	if (method == "GET")
	{
		std::cout << method << std::endl;
		if (location->getMethodGet())
			return true;
		_errorCode = METHOD_NOT_ALLOWED;
		return false;
	}

	if (method == "POST")
	{
		std::cout << "metodo usado: " <<method << std::endl;
		if (location->getMethodPost())
			return true;
		_errorCode = METHOD_NOT_ALLOWED;
		return false;
	}

	if (method == "DELETE")
	{
		std::cout << method << std::endl;
		if (location->getMethodDelete())
			return true;
		_errorCode = METHOD_NOT_ALLOWED;
		return false;
	}
*/
	if (method == "GET")
		return true;
	if (method == "POST")
		return true;
	if (method == "DELETE")
		return true;

	_errorCode = NOT_IMPLEMENTED;
	//TODO el servidor lo acepta?
	return false;
}

bool RequestParser::validateRequestLineStructure(const std::string &line)
{
	if (line.empty())
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	if (line[0] == ' ')
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	if (line[line.size() - 1] == ' ')
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	size_t firstSpace = line.find(' ');
	if (firstSpace == std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	size_t secondSpace = line.find(' ', firstSpace + 1);
	if (secondSpace == std::string::npos)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (line[firstSpace + 1] == ' ')
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (line[secondSpace + 1] == ' ')
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	return true;
}


