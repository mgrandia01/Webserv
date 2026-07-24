/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserStartLineValidation.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:19:10 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/24 11:47:03 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

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
	if (version != "HTTP/1.1" && version != "HTTP/1.0")
	{
		_errorCode = HTTP_VERSION_NOT_SUPPORTED;
		return false;
	}
	return true;
}

bool RequestParser::validateMethod(const std::string &method)
{
	if (method == "GET")
		return true;

	if (method == "POST")
		return true;

	if (method == "DELETE")
		return true;

	_errorCode = NOT_IMPLEMENTED;
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


