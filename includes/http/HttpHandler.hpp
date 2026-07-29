/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/29 11:19:16 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHANDLER_HPP
#define HTTPHANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <cassert>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class HttpHandler
{
	public:
		HttpResponse handle(const HttpRequest& request);

	private:
		HttpResponse handleGet(const HttpRequest& request);
		HttpResponse handlePost(const HttpRequest& request);
		HttpResponse handleDelete(const HttpRequest& request);

		//HandlerGet.cpp
		std::string getContentType(const std::string& path);
		bool readFile(int fd, std::string& body);
};


#endif
