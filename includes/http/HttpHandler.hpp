/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/26 15:36:09 by mgrandia         ###   ########.fr       */
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
		Response handle(const HttpRequest& request, const ServerConfig& server);

	private:
		//HttpHandler.cpp
		////TODO ordenar a utils
		Response serveFile(const std::string& fullPath);
		Response serveDirectory(const std::string& fullPath, const LocationConfig& location, const std::string& requestPath, const ServerConfig& server);
		
		std::string createAutoindexHtml(const std::string& requestPath, const std::vector<std::string>& entries);
		Response generateAutoindex(const std::string& fullPath, const std::string& requestPath,  const ServerConfig& server);
		
		bool isCgi(const HttpRequest& request, const LocationConfig& location) const;
		int validatePostPath(const std::string& path);
		bool isPathSafe(const std::string& path);
		
		Response handleGet(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server);
		Response handlePost(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server);
		Response handleDelete(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server);

		bool isImage(const std::string& filename);
		//HttpHandlerUtils.cpp
		std::string getContentType(const std::string& path);
		bool readFile(int fd, std::string& body);
		bool saveFile(const std::string& path, const std::string& buffer);
		const LocationConfig* findLocation(const HttpRequest& request, const ServerConfig& server) const;
		std::string createGalleryHtml(const std::string& uploadPath);
};



#endif
