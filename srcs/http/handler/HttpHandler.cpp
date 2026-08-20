/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/20 13:01:40 by mgrandia         ###   ########.fr       */
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


Response HttpHandler::serveFile(const std::string& fullPath)
{
	Response response;
	int fd = open(fullPath.c_str(), O_RDONLY);
	int statusCode = OK;

	if (fd == -1)
	{
		if (errno == ENOENT)
			statusCode = NOT_FOUND;	
		else if (errno == EACCES)
			statusCode = FORBIDDEN;
		else
			statusCode = INTERNAL_SERVER_ERROR;
	}
	
	else
	{
		if (!readFile(fd, response.body))
			statusCode = INTERNAL_SERVER_ERROR;

		close(fd);
	}

	
	HttpStatusInfo status = getStatusInfo(statusCode);
	response.statusCode = statusCode;
	response.reasonPhrase = status.reasonPhrase;
	
	if (statusCode != OK)
		response.body = status.defaultBody;

	response.setHeaders(getContentType(fullPath));
	return response;
}

Response HttpHandler::serveDirectory(const std::string& fullPath, const LocationConfig& location, const std::string& requestPath)
{
	const std::vector<std::string>& indexes = location.getIndex();

	for (size_t i = 0; i< indexes.size(); i++)
	{
		std::string indexPath = fullPath + "/" + indexes[i];

		struct stat indexInfo;
		if (stat(indexPath.c_str(), &indexInfo) == -1)
		{
			if (errno == ENOENT)
				continue;
			else if (errno == EACCES)
				return Response::createError(FORBIDDEN);	
			return Response::createError(INTERNAL_SERVER_ERROR);	
		}

		if (S_ISREG(indexInfo.st_mode))
			return serveFile(indexPath);
	}

	if (location.getAutoindex())
		return generateAutoindex(fullPath, requestPath);

	return Response::createError(FORBIDDEN);
}

std::string HttpHandler::createAutoindexHtml(const std::string& requestPath, const std::vector<std::string>& entries)
{
	//TODO ara mateix es una mica cutreeee es veu al fer:
	//http://localhost:8080/downloads
	std::string path = requestPath;

	if (path[path.size() - 1] != '/')
		path += '/';
	std::string html;

	html += "<html>\n";
	html += "<head><title>Index of " + requestPath + "</title></head>\n";
	html += "<body>\n";
	html += "<h1>Index of " + requestPath + "</h1>\n";
	html += "<ul>\n";

	for (size_t i = 0; i < entries.size(); i++)
	{
		std::string href = path + entries[i];
		
	        html += "<li><a href=\"" + href + "\">";
	        html += entries[i];
	        html += "</a></li>\n";
	}

	html += "</ul>\n";
	html += "</body>\n";
	html += "</html>\n";

	return html;
}

Response HttpHandler::generateAutoindex(const std::string& fullPath, const std::string& requestPath)
{
	DIR* dir = opendir(fullPath.c_str());

	if (dir == NULL)
	{
		if (errno == EACCES)
			return Response::createError(FORBIDDEN);

		return Response::createError(INTERNAL_SERVER_ERROR);
	}

	std::vector<std::string> entries;
	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		std::string entryPath = fullPath + "/" + name;
		struct stat entryInfo;
		if (stat(entryPath.c_str(), &entryInfo) == -1)
			continue;
		if (S_ISDIR(entryInfo.st_mode))
			name += "/";
		entries.push_back(name);
	}

	closedir(dir);

	Response response;
	response.statusCode = OK;
	response.reasonPhrase = getStatusInfo(OK).reasonPhrase;
	response.body = createAutoindexHtml(requestPath, entries);
	response.setHeaders("text/html");

	return response;
}


Response HttpHandler::handleGet(const HttpRequest& request, const LocationConfig& location)
{


	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);


	std::string root = location.getRoot(); 
	std::string fullPath = root + request.path;
	
	struct stat fileInfo;

	if (stat(fullPath.c_str(), &fileInfo) == -1)
	{
		if (errno == ENOENT)
			return Response::createError(NOT_FOUND);	
		else if (errno == EACCES)
			return Response::createError(FORBIDDEN);	
		else
			return Response::createError(INTERNAL_SERVER_ERROR);	
		return Response::createError(NOT_FOUND);
	}
	if (S_ISREG(fileInfo.st_mode))
		return serveFile(fullPath);
	else if (S_ISDIR(fileInfo.st_mode))
		return serveDirectory(fullPath, location, request.path);
	return Response::createError(FORBIDDEN);
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



