/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/29 15:56:13 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHANDLER_HPP
#define HTTPHANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Response.hpp"

#include <cassert>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class HttpHandler
{
	public:
		//TODO al hacer la integracion sera algo como:
		//HttpResponse HttpHandler::handle
		//(const HttpRequest& request,const LocationConfig& location)
		Response handle(const HttpRequest& request);

	private:
		//HttpHandler.cpp
		HttpResponse handleGet(const HttpRequest& request);
		HttpResponse handlePost(const HttpRequest& request);
		HttpResponse handleDelete(const HttpRequest& request);

		//HttpHandlerUtils.cpp
		void setHeaders(HttpResponse& response, const std::string& contentType);
		std::string getContentType(const std::string& path);
		bool readFile(int fd, std::string& body);
		bool saveFile(const std::string& path, const std::string& buffer);
};


#endif
