/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBody.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:18:28 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/27 11:14:09 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

bool RequestParser::hasBody() const
{
	std::map<std::string, std::string>::const_iterator it;

	it = _request.headers.find("content-length");
	if (it != _request.headers.end())
		return true;

	it = _request.headers.find("transfer-encoding");
	if (it != _request.headers.end())
		return true;

	return false;
}

void RequestParser::parseContentLengthBody()
{
	if (_stream.size() < _contentLength)
		return ;

	_request.body = _stream.substr(0, _contentLength);
	_stream.erase(0, _contentLength);
	_state = COMPLETE;
}


void RequestParser::parseChunkedBody()
{
	while (true)
	{
		size_t pos = _stream.find("\r\n");
		if (pos == std::string::npos)
			return;
	
		std::string hexSize = _stream.substr(0,pos);
		if (!isValidHexSize(hexSize))
		{
			_errorCode = BAD_REQUEST;
			_state = ERROR;
			return;
		}
 
		size_t chunkSize = hexToDecimal(hexSize);
		if (_stream.size() < pos + 2 + chunkSize + 2)
			return;

		if (chunkSize == 0)
		{
			size_t trailersEnd = _stream.find("\r\n\r\n");
			if (trailersEnd == std::string::npos)
 				return;
			_stream.erase(0, trailersEnd + 4);
			_state = COMPLETE;
			return ;
		}
  		
		_request.body += _stream.substr(pos + 2, chunkSize);

		size_t end = pos + 2 + chunkSize;
		if (_stream.compare(end, 2, "\r\n") != 0)
		{
			_errorCode = BAD_REQUEST;
			_state = ERROR;
			return;
		}

		_stream.erase(0, pos + 2 + chunkSize + 2);

	}
}

void RequestParser::parseBody()
{
	if (_chunked)
	{
		parseChunkedBody();
		return; 
	}
	else
		parseContentLengthBody();
}

