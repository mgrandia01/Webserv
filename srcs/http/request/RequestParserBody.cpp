/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBody.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:18:28 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/24 12:29:31 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

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

void RequestParser::parseBody()
{
	if (_chunked)
	{
		std::cout << "Chunked!" << std::endl;
		//parseChunkedBody();
		//TODO
		return; 
	}
	else
		parseContentLengthBody();
}

