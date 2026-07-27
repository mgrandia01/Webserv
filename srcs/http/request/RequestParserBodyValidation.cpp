/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBodyValidation.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 11:46:22 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/27 10:55:54 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

bool RequestParser::validateBodySize() const
{

//TODO if (request.contentLength > serverConfig.clientMaxBodySize) TODO mas adelante
	if (_contentLength > _clientMaxBodySize)
		return false;
	return true;
}

bool RequestParser::isValidHexSize(const std::string &hexSize)
{
	if (hexSize.empty())
		return false;

	size_t i = 0;
	while (i < hexSize.size())
	{
		char c = hexSize[i];

		
		if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
			return false;
		i++;
	}

	return true;
}

size_t RequestParser::hexToDecimal(const std::string &hexSize)
{
	size_t result = 0;
	size_t i = 0;

	while (i < hexSize.size())
	{
		char c = hexSize[i];
		int value;

		if (c >= '0' && c <= '9')
			value = c - '0';
		else if (c >= 'A' && c <= 'F')
			value = c - 'A' + 10;
		else
			value = c - 'a' + 10;

		result = result * 16 + value;
		i++;
	}
	return (result);
}
