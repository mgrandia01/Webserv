/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/31 12:52:18 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sstream>
#include <ctime>
#include "http/HttpSerializer.hpp"
#include "ServerConfig.hpp"
#include "Response.hpp"

Response::Response(): statusCode(0), reasonPhrase(""), headers(), body(), _stream(){}

Response::Response(std::string stream) : _stream(stream){}

Response::~Response(){}

Response::Response(const Response& other)
{

	*this = other;
}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
	statusCode = other.statusCode;
	reasonPhrase = other.reasonPhrase;
	headers = other.headers;
	body = other.body;
	_stream = other._stream;
    }
    return *this;
}

const std::string& Response::getStream() const
{
	_stream = HttpSerializer::serialize(*this);

	return _stream;
}

std::string getDateHeader()
{
	char buffer[100];

	std::time_t now = std::time(NULL);
	std:: tm *gmt = std::gmtime(&now);

	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	return std::string(buffer);
}

void Response::setHeaders(const std::string& contentType)
{
        headers["Content-Type"] = contentType;

        std::stringstream ss;
        ss << body.size();
        headers["Content-Length"] = ss.str();

	headers["Date"] = getDateHeader();
}

Response Response::createError(HttpStatus status, const ServerConfig& server)
{
	Response response;

	HttpStatusInfo info = getStatusInfo(status);

	response.statusCode = status;
	response.reasonPhrase = info.reasonPhrase;
	response.body = info.defaultBody;

	applyConfiguredErrorPage(response, server, status);
	response.setHeaders("text/html");

	return response;
}

void Response::applyConfiguredErrorPage(Response& response, const ServerConfig& server, HttpStatus status)
{
	const std::vector<t_errorPage>& errorPages = server.getErrorPage();

	for (size_t i = 0; i < errorPages.size(); i++)
	{
		for (size_t j = 0; j < errorPages[i].codes.size(); j++)
		{
			if (errorPages[i].codes[j] == static_cast<int>(status))
			{
				std::string filePath = server.getRoot() + errorPages[i].errorFile;
				std::ifstream file(filePath.c_str());
			if (file.is_open())
			{
				std::stringstream buffer;
				buffer << file.rdbuf();

				response.body = buffer.str();
			}
			return;
			}
		}
	}

}
