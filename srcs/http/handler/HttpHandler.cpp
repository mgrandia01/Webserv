/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/20 08:37:55 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Response.hpp"
#include <cerrno>
#include <sstream>
#include <cstdio>
#include <iostream>


Response HttpHandler::handleGet(const HttpRequest& request, const LocationConfig& location)
{


	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);


	std::string root = location.getRoot(); 
	std::string fullPath = root + request.path;
	Response response;
	
	std::cout << "==Full path ==" << fullPath << std::endl;

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

	response.setHeaders(getContentType(fullPath));
	return response;
}

Response HttpHandler::handlePost(const HttpRequest& request, const LocationConfig& location)
{
	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);

	(void)location;
	Response response;

	std::string uploadStore = "./uploads";//TODO desparchear
	
	//TODO debe crear con el nombre que me pasa por terminal... no?
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
		response.setHeaders("text/plain");
	}
	else
		response.body = status.defaultBody;

	return response;
}

Response HttpHandler::handleDelete(const HttpRequest& request, const LocationConfig& location)
{
	(void)location;
	Response response;

	std::string root = location.getRoot();// "./www";TODO esto esta parcheado
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

Response Response::createRedirect(int code, const std::string& target)
{
	Response response;

	response.statusCode = code;


	if (code == 301)
		response.reasonPhrase = "Moved Permanently";
	else if (code == 302)
		response.reasonPhrase = "Found";

	response.headers["Location"] = target;
	response.headers["Content-Length"] = "0";
	response.body = "";

	return response;

}

Response HttpHandler::handle(const HttpRequest& request, const ServerConfig& server)
{

	const LocationConfig* location = findLocation(request, server);
        if (!location)
		return Response::createError(NOT_FOUND);
	
	else if (request.method == "GET" && !location->getMethodGet())
			return Response::createError(METHOD_NOT_ALLOWED);
	else if (request.method == "POST" && !location->getMethodPost())
			return Response::createError(METHOD_NOT_ALLOWED);
	else if (request.method == "DELETE" && !location->getMethodDelete())
			return Response::createError(METHOD_NOT_ALLOWED);
	else
		return Response::createError(NOT_IMPLEMENTED);

	if (location->getIsEnabledReturn())
	{
		const t_return& redirect = location->getReturn();
		return Response::createRedirect(redirect.code,redirect.target);
	}

	if (request.method == "GET")
		return handleGet(request, *location);
	if (request.method == "POST")
		return handlePost(request, *location);
	if (request.method == "DELETE")
		return handleDelete(request, *location);
	return Response::createError(NOT_IMPLEMENTED);
}



