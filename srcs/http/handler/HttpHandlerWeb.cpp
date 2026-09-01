/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandlerWeb.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:58:23 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/31 13:06:43 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"

void HttpHandler::replaceAll(std::string& str, const std::string& from, const std::string& to)
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

bool HttpHandler::isImage(const std::string& filename)
{
	size_t pos = filename.find_last_of('.');

	if (pos == std::string::npos)
		return false;

	std::string extension = filename.substr(pos + 1);

	return extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "webp";
}
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
		html += "    <button class=\"delete-photo\" data-image=\"" + name + "\">x</button>\n";
		html += "</div>\n";
	}
	
	closedir(dir);
	
	return html;
}


