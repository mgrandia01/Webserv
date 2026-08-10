/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:13:43 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/05 12:55:06 by mgrandia         ###   ########.fr       */
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
	//TODO si pasan el server, buscar las paginas de error para dar la info de estas si existe
	//si no existeuna por defoult como body
	switch (statusCode)
	{
		case 200:
			return HttpStatusInfo("OK", "OK");
		case 201:
			return HttpStatusInfo("Created", "Created");
		case 400:
			return HttpStatusInfo("Bad Request", "400 Bad Request");
		case 403:
			return HttpStatusInfo("Forbidden", "403 Forbidden");
		case 404:
			return HttpStatusInfo("Not Found", "404 Not Found");
		case 405:
			return HttpStatusInfo("Method Not Allowed", "405 Method Not Allowed");
		case 413:
			return HttpStatusInfo("Payload Too Large", "413 Payload Too Large");
		case 500:
			return HttpStatusInfo("Internal Server Error", "500 Internal Server Error");
		default:
			return HttpStatusInfo("Internal Server Error", "500 Internal Server Error");
    }
}
