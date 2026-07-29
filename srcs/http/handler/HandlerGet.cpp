/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerGet.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:57:57 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/29 11:18:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"

std::string HttpHandler::getContentType(const std::string& path)
{
	if (path.find(".html") != std::string::npos)
		return "text/html";

	if (path.find(".css") != std::string::npos)
		return "text/css";

	if (path.find(".js") != std::string::npos)
		return "application/javascript";

	if (path.find(".png") != std::string::npos)
		return "image/png";

	if (path.find(".jpg") != std::string::npos
	      	||path.find(".jpeg") != std::string::npos)
		return "image/jpeg";

	return "application/octet-stream";
}

bool HttpHandler::readFile(int fd, std::string& body)
{
	char buffer[1024];
	while (true)
	{
		ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
		if (bytesRead < 0)
			return (false);

		if (bytesRead == 0)
			break;
		body.append(buffer, bytesRead);
	
	}
	return (true);
}
