/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/20 12:42:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHANDLER_HPP
#define HTTPHANDLER_HPP

#include "RequestParser.hpp"
#include "Response.hpp"

#include <cassert>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

class ServerConfig;
class LocationConfig;

class HttpHandler
{
	public:
		//TODO al hacer la integracion sera algo como:
		//HttpResponse HttpHandler::handle
		//(const HttpRequest& request,const LocationConfig& location)
		Response handle(const HttpRequest& request, const ServerConfig& server);

	private:
		//HttpHandler.cpp
		Response serveFile(const std::string& fullPath);
		Response serveDirectory(const std::string& fullPath, const LocationConfig& location, const std::string& requestPath);
		std::string createAutoindexHtml(const std::string& requestPath, const std::vector<std::string>& entries);
		Response generateAutoindex(const std::string& fullPath, const std::string& requestPath);
		Response handleGet(const HttpRequest& request, const LocationConfig& location);
		Response handlePost(const HttpRequest& request, const LocationConfig& location);
		Response handleDelete(const HttpRequest& request, const LocationConfig& location);

		//HttpHandlerUtils.cpp
		//void setHeaders(Response& response, const std::string& contentType);
		std::string getContentType(const std::string& path);
		bool readFile(int fd, std::string& body);
		bool saveFile(const std::string& path, const std::string& buffer);
		const LocationConfig* findLocation(const HttpRequest& request, const ServerConfig& server) const;

};


#endif
