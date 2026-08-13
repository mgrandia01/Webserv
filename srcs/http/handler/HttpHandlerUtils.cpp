/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandlerUtils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:57:57 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/13 15:29:29 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp" //TODO 
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

#include <sstream>
#include <vector>
#include <iostream>

const LocationConfig* HttpHandler::findLocation(const HttpRequest& request, const ServerConfig& server) const
{
	const std::vector<LocationConfig>& locations = server.getLocations();
	const LocationConfig* bestMatch = NULL;
	std::size_t bestLength = 0;

	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end();++it)
	{
		const LocationConfig& location = *it;
		const std::string& locationUri = location.getUri();
		
		if (request.path.compare(0, locationUri.length(), locationUri) == 0)
		{
			if (locationUri.length() > bestLength)
			{
				bestMatch = &location;
				bestLength = locationUri.length();
			}
		}
	}	
	return bestMatch;
}

/*void HttpHandler::setHeaders(Response& response, const std::string& contentType)
{
	response.headers["Content-Type"] = contentType;

	std::stringstream ss;
	ss << response.body.size();
	response.headers["Content-Length"] = ss.str();

	// TODO
	// response.headers["Date"] = ...
	// response.headers["Server"] = ...
	// response.headers["Connection"] = ...
}*/


bool HttpHandler::saveFile(const std::string& path, const std::string& buffer)
{
	int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return false;
	ssize_t bytesWritten = write(fd, buffer.c_str(), buffer.size());

	close(fd);

	if (bytesWritten < 0)
		return false;

	if (static_cast<ssize_t>(buffer.size()) != bytesWritten)
		return false;
	return true;
}

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
