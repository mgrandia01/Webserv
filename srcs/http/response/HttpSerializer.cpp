/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSerializer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 15:24:29 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/10 14:44:31 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpSerializer.hpp"
#include "Response.hpp"
#include <sstream>

std::string intToString(int n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

std::string HttpSerializer::serialize(const Response& response)
{
	std::string raw;

	raw += "HTTP/1.1 ";
	raw += intToString(response.statusCode);
	raw += " ";
	raw += response.reasonPhrase;
	raw += "\r\n";

	std::map<std::string, std::string>::const_iterator it;

	for (it = response.headers.begin(); it != response.headers.end(); ++it)
	{
		std::string key = it->first;
		std::string value = it->second;

		raw += key;
		raw += ": ";
		raw += value;
		raw += "\r\n";
	}

	raw += "\r\n";
	raw += response.body;
    // status line
    // headers
    // línea vacía
    // body
	
	return raw;
}
