/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserValidation.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 12:41:35 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 10:43:12 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"
#include <cctype>


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

	//Validar lo de % puede ser %00-%FF ASCII	
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


