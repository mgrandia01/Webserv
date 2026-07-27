/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:25:50 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/27 12:59:03 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>

class HttpRequest
{
	public:
		std::string method;
		std::string target;
		std::string path;
		std::string query;
		std::string version;

		std::map<std::string, std::string> headers;
		std::map<std::string, size_t> headerOccurrences;

		std::string body;
};

#endif
