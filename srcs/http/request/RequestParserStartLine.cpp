/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserStartLine.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:24:28 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/23 11:13:13 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

void RequestParser::parseRequestTarget(const std::string &target)
{
	size_t pos = target.find('?');

	if (pos == std::string::npos)
	{
		_request.path = target;
		_request.query.clear();
	}
	else
	{
		_request.path = target.substr(0,pos);
		_request.query = target.substr(pos + 1);
	}

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

	parseRequestTarget(target);

	return true;
}



