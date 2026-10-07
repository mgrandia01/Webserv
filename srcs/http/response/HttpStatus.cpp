/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:13:43 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/31 15:31:36 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpStatus.hpp"
#include <sstream>

HttpStatusInfo::HttpStatusInfo(const std::string& reason, const std::string& body)
{
	reasonPhrase = reason;
	defaultBody = body;
}

std::string generateErrorPage(const std::string& message)
{
	std::stringstream ss;

	ss << "<!DOCTYPE html>"
	<< "<html>"
	<< "<head>"
	<< "<title>" << message << "</title>"
	<< "<link rel=\"stylesheet\" href=\"/style.css\">"
	<< "</head>"
	<< "<body>"
	<< "<h1>" << message << "</h1>"
	<< "</body>"
	<< "</html>";

	return ss.str();
}

HttpStatusInfo getStatusInfo(int statusCode)
{
	switch (statusCode)
	{
		case 200:
			return HttpStatusInfo("OK", "OK");
		case 201:
			return HttpStatusInfo("Created", "Created");
		case 400:
			return HttpStatusInfo("Bad Request", generateErrorPage("400 Bad Request"));
		case 403:
			return HttpStatusInfo("Forbidden", generateErrorPage("403 Forbidden"));
		case 404:
			return HttpStatusInfo("Not Found", generateErrorPage("404 Not Found"));
		case 405:
			return HttpStatusInfo("Method Not Allowed", generateErrorPage("405 Method Not Allowed"));
		case 408: // TODO add control errors on ServerManager, example one 408
			return HttpStatusInfo("Request Time-out", generateErrorPage("408 Request Time-out"));
		case 413:
			return HttpStatusInfo("Payload Too Large", generateErrorPage("413 Payload Too Large"));
		case 500:
			return HttpStatusInfo("Internal Server Error", generateErrorPage("500 Internal Server Error"));
		case 501:
			return HttpStatusInfo("Not Implemented", generateErrorPage("501 Not Implemented"));
		case 504:
			return HttpStatusInfo("Gateway Timeout", generateErrorPage("504 Gateway Timeout"));
		default:
			return HttpStatusInfo("Internal Server Error", generateErrorPage("500 Internal Server Error"));
    }
}
