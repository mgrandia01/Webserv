/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:13:43 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/24 11:52:31 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpStatus.hpp"

HttpStatusInfo::HttpStatusInfo(const std::string& reason, const std::string& body)
{
	reasonPhrase = reason;
	defaultBody = body;
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
			return HttpStatusInfo("Bad Request", "400 Bad Request");//        "<html><body><h1>400 Bad Request</h1></body></html>"
		case 403:
			return HttpStatusInfo("Forbidden", "403 Forbidden");
		case 404:
			return HttpStatusInfo("Not Found", "404 Not Found");
		case 405:
			return HttpStatusInfo("Method Not Allowed", "405 Method Not Allowed");
		case 408: // TODO add control errors on ServerManager, example one 408
			return HttpStatusInfo("Request Time-out", "408 Request Time-out");
		case 413:
			return HttpStatusInfo("Payload Too Large", "413 Payload Too Large");
		case 500:
			return HttpStatusInfo("Internal Server Error", "500 Internal Server Error");
		default:
			return HttpStatusInfo("Internal Server Error", "500 Internal Server Error");
    }
}
