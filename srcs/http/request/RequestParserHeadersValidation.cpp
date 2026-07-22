/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserHeadersValidation.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:24:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 12:03:38 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

#include <limits>


bool RequestParser::validateFramingHeaders()
{
	if (_request.headerOccurrences["content-length"] > 0 &&
			_request.headerOccurrences["transfer-encoding"] > 0)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	return true;
}

bool RequestParser::validateTransferEncoding()
{
	size_t occurrences = _request.headerOccurrences["transfer-encoding"];
	if (occurrences > 1)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}

	if (occurrences == 1)
	{
		if (_request.headers["transfer-encoding"].empty())
		{
			_errorCode = BAD_REQUEST;
			return false;
		}
		if (_request.headers["transfer-encoding"] != "chunked")
		{
			_errorCode = NOT_IMPLEMENTED;
			return false;
		}
	}
	return true;
}


//TODO anyadir a la classe
bool RequestParser::isValidContentLength(const std::string &value)
{
	size_t result = 0;

	for (size_t i = 0; i < value.size(); i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(value[i])))
			return false;

		size_t digit = value[i] - '0';
		if (result > (std::numeric_limits<size_t>::max() - digit) / 10)
			return false;

		result = result * 10 + (value[i] - '0');
	}
	_contentLength = result;
	// TODO: Detectar overflow de Content-Length
	return true;
}
bool RequestParser::validateContentLength()
{
	size_t occurrences = _request.headerOccurrences["content-length"];
	//entero decimal, no negativo, no desborde, no vacio
	if (occurrences > 1)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (occurrences == 1)
	{
		if (_request.headers["content-length"].empty())
		{
			_errorCode = BAD_REQUEST;
			return false;
		}
		//validar que sea un numero...
		
		if (!isValidContentLength(_request.headers["content-length"]))
		{
			_errorCode = BAD_REQUEST;
			return false;

		}
	}
	return true;
}


bool RequestParser::validateHost()
{
	if (_request.headerOccurrences["host"] != 1)
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	if (_request.headers["host"].empty())
	{
		_errorCode = BAD_REQUEST;
		return false;
	}
	return true;
}

bool RequestParser::validateHeaders()
{
	if (!validateHost())
		return false;

	//error de framing, si tenemos content-length y transfer encoding chunked a la vez
	if (!validateFramingHeaders())
		return false;

	if (!validateContentLength())
		return false;

	if (!validateTransferEncoding())
		return false;
	return true;
}


