/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBody.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:18:28 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/23 14:53:10 by mgrandia         ###   ########.fr       */
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

void RequestParser::parseBody()
{
  //TODO:

//	_request.body = _stream;
//	_stream.clear();

//	_state = COMPLETE;
}


