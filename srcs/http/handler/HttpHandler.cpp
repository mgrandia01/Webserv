/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/04 11:21:36 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include <cerrno>
#include <sstream>
#include <cstdio>

HttpResponse HttpHandler::handleGet(const HttpRequest& request)
{
	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);




	std::string root = "./www";//TODO esto esta parcheado
	std::string fullPath = root + request.path;
	HttpResponse response;

	//TODO usar stat() por si el usuario quiere un abrir un directorio
	//y no directamente un fichero, ya que tendra que mirar index, autoindex...
	int fd = open(fullPath.c_str(), O_RDONLY);
	int statusCode = 200;

	if (fd == -1)
	{
		if (errno == ENOENT)
			statusCode = 404;	
		else if (errno == EACCES)
			statusCode = 403;
		else
			statusCode = 500;
	
	}
	
	else
	{
		if (!readFile(fd, response.body))
			statusCode = 500;
	
		close(fd);
	}

	
	HttpStatusInfo status = getStatusInfo(statusCode);
	response.statusCode = statusCode;
	response.reasonPhrase = status.reasonPhrase;
	
	if (statusCode != 200)
		response.body = status.defaultBody;

	setHeaders(response, getContentType(fullPath));
	return response;
}

HttpResponse HttpHandler::handlePost(const HttpRequest& request)
{
	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);


	HttpResponse response;

	std::string uploadStore = "./uploads";//TODO desparchear
	
	std::string filename = uploadStore + "/upload.txt";

	int statusCode = 201;

	if (!saveFile(filename, request.body))
		statusCode = 500;

	HttpStatusInfo status = getStatusInfo(statusCode);

	response.statusCode = statusCode;
	response.reasonPhrase = status.reasonPhrase;

	if (statusCode == 201)
	{
		response.body = "Upload successful";
		setHeaders(response, "text/plain");
	}
	else
		response.body = status.defaultBody;

	return response;
}

HttpResponse HttpHandler::handleDelete(const HttpRequest& request)
{
	HttpResponse response;

	std::string root = "./www";//TODO esto esta parcheado
	std::string fullPath = root + request.path;

	int result = std::remove(fullPath.c_str());
	int statusCode = 200;

	if (result != 0)
	{
		if (errno == ENOENT)
			statusCode = 404;
		else if (errno == EACCES || errno == EPERM)
			statusCode = 403;	
		else
			statusCode = 500;	
	}

	HttpStatusInfo status = getStatusInfo(statusCode);
	response.statusCode = statusCode;
	response.reasonPhrase = status.reasonPhrase;
	response.body = status.defaultBody;
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



