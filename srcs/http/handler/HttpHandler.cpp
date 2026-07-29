/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/29 08:29:02 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"

HttpResponse HttpHandler::handleGet(const HttpRequest& request)
{
	std::string root = "./www";//TODO esto esta parcheado
	std::string fullPath = root + request.path;
	HttpResponse response;

	//TODO usar stat() por si el usuario quiere un abrir un directorio
	//y no directamente un fichero, ya que tendra que mirar index, autoindex...
	int fd = open(fullPath.c_str(), O_RDONLY);

	if (fd == -1)
	{
		response.statusCode = 404;
		response.reasonPhrase = "Not Found";
		response.body = "404 Not Found";
		return response;
	}

	//TODO extraer esto  una funcion
	char buffer[1024];
	while (true)
	{
		size_t bytesRead = read(fd, buffer, sizeof(buffer));
		if (bytesRead < 0)
		{
			close(fd);
			response.statusCode = 500;
			return response;
		}

		if (bytesRead == 0)
			break;
		response.body.append(buffer, bytesRead);
	
	}
	close(fd);
	
	response.statusCode = 200;
	response.reasonPhrase = "OK";
	return response;
}

HttpResponse HttpHandler::handlePost(const HttpRequest&)
{
	HttpResponse response;

	// TODO: Implement POST

	return response;
}

HttpResponse HttpHandler::handleDelete(const HttpRequest&)
{
	HttpResponse response;

	// TODO: Implement DELETE

	return response;
}

HttpResponse HttpHandler::handle(const HttpRequest& request)
{
	if (request.method == "GET")
		return handleGet(request);

	else if (request.method == "POST")
		return handlePost(request);

	else if (request.method == "DELETE")
		return handleDelete(request);

	assert(false && "Unexpected HTTP method");
	std::abort();
}



