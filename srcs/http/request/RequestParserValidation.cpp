/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserValidation.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 12:41:35 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/17 14:39:47 by mgrandia         ###   ########.fr       */
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
	// TODO:
	// Validate origin-form according to RFC 9112.
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


