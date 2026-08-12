/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/12 11:11:15 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHANDLER_HPP
#define HTTPHANDLER_HPP

#include "HttpRequest.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

#include <cassert>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
		Response handleGet(const HttpRequest& request, const LocationConfig& location);
		Response handlePost(const HttpRequest& request, const LocationConfig& location);
		Response handleDelete(const HttpRequest& request, const LocationConfig& location);

		//HttpHandlerUtils.cpp
		void setHeaders(Response& response, const std::string& contentType);
		std::string getContentType(const std::string& path);
		bool readFile(int fd, std::string& body);
		bool saveFile(const std::string& path, const std::string& buffer);
		const LocationConfig* findLocation(const HttpRequest& request, const ServerConfig& server) const;

};


#endif
