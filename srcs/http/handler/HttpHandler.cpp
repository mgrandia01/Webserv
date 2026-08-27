/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:07:45 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/27 15:12:04 by mgrandia         ###   ########.fr       */
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
#include <fstream>

//FIXME web.cpp
void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	size_t position = 0;

	while (position != std::string::npos)
	{
		position = str.find(from, position);

		if (position == std::string::npos)
			break;

		str.replace(position, from.length(), to);
		position += to.length();
	}
}

//FIXME utils.cpp
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

//FIXME utils.cpp
Response HttpHandler::serveDirectory(const std::string& fullPath, const LocationConfig& location, const std::string& requestPath, const ServerConfig& server)
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
				return Response::createError(FORBIDDEN, server);	
			return Response::createError(INTERNAL_SERVER_ERROR, server);	
		}

		if (S_ISREG(indexInfo.st_mode))
			return serveFile(indexPath);
	}
	if (location.getAutoindex())
		return generateAutoindex(fullPath, requestPath, server);

	return Response::createError(FORBIDDEN, server);
}

//FIXME utils.cpp
std::string HttpHandler::createAutoindexHtml(const std::string& requestPath, const std::vector<std::string>& entries)
{
	std::ifstream file("www/autoindex.html");

	if(!file.is_open())
		return "";
	std::string html;
	std::string line;

	while (std::getline(file, line))
	{
		html += line;
		html += "\n";
	}
	file.close();

	std::string entriesHtml;
	std::string path = requestPath;

	if (path[path.size() - 1] != '/')
		path += '/';

	for (size_t i = 0; i < entries.size(); i++)
	{
		std::string href = path + entries[i];
		
		entriesHtml += "<li><a href=\"" + href + "\">";
		entriesHtml += entries[i];
		entriesHtml += "</a></li>\n";
	}

	replaceAll(html, "{{PATH}}", requestPath);
	replaceAll(html, "{{ENTRIES}}", entriesHtml);

	return html;
}

//FIXME utils.cpp
Response HttpHandler::generateAutoindex(const std::string& fullPath, const std::string& requestPath, const ServerConfig& server)
{
	DIR* dir = opendir(fullPath.c_str());

	if (dir == NULL)
	{
		if (errno == EACCES)
			return Response::createError(FORBIDDEN, server);

		return Response::createError(INTERNAL_SERVER_ERROR, server);
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

//FIXME utils.cpp
bool HttpHandler::isCgi(const HttpRequest& request, const LocationConfig& location) const
{

	const std::map<std::string, std::string> cgi = location.getCgi();
	if(cgi.empty())
		return (false); //FIXME si no hay es empty?

	std::string path = request.path;
	std::size_t pos = path.rfind('.');

	if (pos == std::string::npos)
		return (false);

	std::string extension = path.substr(pos);

	if (cgi.find(extension) != cgi.end())
		return (true);

	return (false);
}

#include <dirent.h>

bool HttpHandler::isImage(const std::string& filename)
{
	size_t pos = filename.find_last_of('.');

	if (pos == std::string::npos)
		return false;

	std::string extension = filename.substr(pos + 1);

	return extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "webp";
}
//FIXME web.cpp
std::string HttpHandler::createGalleryHtml(const std::string& uploadPath)
{
	DIR* dir = opendir(uploadPath.c_str());

	if (!dir)
		return "";

	std::string html;
	struct dirent* entry;
	
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;

		if (name == "." || name == "..")
			continue;
		if(!isImage(name))
			continue;

		html += "<div class=\"photo\">\n";
		html += "    <img src=\"/uploads/" + name + "\" alt=\"" + name + "\">\n";
		html += "    <p>" + name + "</p>\n";
		html += "    <button class=\"delete-photo\" data-image=\"" + name + "\">x</button>\n";//FIXME
		html += "</div>\n";
	}
	
	closedir(dir);
	
	return html;
}

Response HttpHandler::handleGet(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server)
{
	

	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);

	//if de si es .py .php 
	//passar a Martha SERVER, PATH, QUERY Y LOCATION
	//lo que devuelve el cgi es un string, que tendremos que parsear para devolver como respuesta
	
	if(isCgi(request, location))
	{
		std::cout << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiisCgi torna TRUE" <<std::endl;
		//llamar al cgi
		//parsear resultado
		//devoler response
	
	}
//TODO sacarlo a una externa

	if(request.path == "/my_web.html")
	{
		std::ifstream file("www/my_web.html");

		if (!file.is_open())
		return Response::createError(INTERNAL_SERVER_ERROR, server);

		std::string html;
		std::string line;

		while (std::getline(file, line))
		{
			html += line;
			html += "\n";
		}

		file.close();

		std::string imagesHtml = createGalleryHtml("./www/uploads");

		replaceAll(html, "{{IMAGES}}", imagesHtml);
		
		Response response;
		response.statusCode = 200;
		response.reasonPhrase = "OK";
		response.body = html;
		response.setHeaders("text/html");

		return response;
	}

	std::string root = location.getRoot(); 
	std::string fullPath = root + request.path;
	
	struct stat fileInfo;

	if (stat(fullPath.c_str(), &fileInfo) == -1)
	{
		if (errno == ENOENT)
			return Response::createError(NOT_FOUND, server);	
		else if (errno == EACCES)
			return Response::createError(FORBIDDEN, server);	
		else
			return Response::createError(INTERNAL_SERVER_ERROR, server);	
	}
	if (S_ISREG(fileInfo.st_mode))
		return serveFile(fullPath);
	else if (S_ISDIR(fileInfo.st_mode))
	{
		if (request.path[request.path.size() - 1] != '/')
			return Response::createRedirect(301, request.path + "/");
		return serveDirectory(fullPath, location, request.path, server);
	}
	return Response::createError(FORBIDDEN, server);
}

int HttpHandler::validatePostPath(const std::string& path)
{
	if (path.empty() || path == "/")
		return 400;

	if (path.find('/', 1) != std::string::npos)
		return 400;

	if (path == "/.." ||
		path.find("/../") != std::string::npos)
		return 400;

	return 0;
}

bool HttpHandler::isPathSafe(const std::string& path)
{
	if (path == "/.." || path.find("/../") != std::string::npos)
		return false;

	return true;
}

Response HttpHandler::handlePost(const HttpRequest& request, const LocationConfig& location,  const ServerConfig& server)
{
	std::cout << "===== POST =====" << std::endl;
std::cout << "Path: [" << request.path << "]" << std::endl;
std::cout << "Body size: " << request.body.size() << std::endl;

	(void)server;
	//TODO server para codgo de error
	//TODO cgi
	//if (config.isCGI(request.path))
	//	return cgiHandler.execute(request);
	
	

	Response response;
	std::string uploadStore = location.getUploadStore();
	
	if (uploadStore.empty())
	{
		//TODO portque no llamo a createError?
		HttpStatusInfo status = getStatusInfo(403);

		response.statusCode = 403;
		response.reasonPhrase = status.reasonPhrase;
		response.body = status.defaultBody;
		response.setHeaders("text/plain");

		return response;
	}
	 int pathError = validatePostPath(request.path);
	if (pathError != 0)
	{
	
		HttpStatusInfo status = getStatusInfo(pathError);

		response.statusCode = pathError;
		response.reasonPhrase = status.reasonPhrase;
		response.body = status.defaultBody;
		response.setHeaders("text/plain");

		return response;
	}

	std::string filename = uploadStore + request.path;
	int statusCode = 201;

	if (!saveFile(filename, request.body))
		statusCode = 500;

	HttpStatusInfo status = getStatusInfo(statusCode);

	response.statusCode = statusCode;
	response.reasonPhrase = status.reasonPhrase;

	if (statusCode == 201)
	{
		//TODO succesful?
		response.body = "Upload successful";
		response.setHeaders("text/plain");
	}
	else
	{
		response.body = status.defaultBody;
		response.setHeaders("text/plain");
	}

	return response;
}

Response HttpHandler::handleDelete(const HttpRequest& request, const LocationConfig& location,  const ServerConfig& server)
{
	(void)server;
	Response response;

	std::string root = location.getRoot();
	std::string fullPath = root + request.path;
	if (!isPathSafe(request.path))
	{
		HttpStatusInfo status = getStatusInfo(400);

		response.statusCode = 400;
		response.reasonPhrase = status.reasonPhrase;
		response.body = status.defaultBody;
		response.setHeaders("text/plain");

		return response;
	}
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
	response.setHeaders("text/plain");
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
		return Response::createError(NOT_FOUND, server);
	
	else if (request.method == "GET" && !location->getMethodGet())
			return Response::createError(METHOD_NOT_ALLOWED, server);
	else if (request.method == "POST" && !location->getMethodPost())
			return Response::createError(METHOD_NOT_ALLOWED, server);
	else if (request.method == "DELETE" && !location->getMethodDelete())
			return Response::createError(METHOD_NOT_ALLOWED, server);
	if (location->getIsEnabledReturn())
	{
		const t_return& redirect = location->getReturn();
		return Response::createRedirect(redirect.code,redirect.target);
	}

	if (request.method == "GET")
		return handleGet(request, *location, server);
	if (request.method == "POST")
		return handlePost(request, *location, server);
	if (request.method == "DELETE")
		return handleDelete(request, *location, server);
	return Response::createError(NOT_IMPLEMENTED, server);
}



