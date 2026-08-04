/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 11:55:22 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/31 12:07:08 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP

#include <string>
#include <map>

enum HttpStatus
{
	OK = 200,

	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	PAYLOAD_TOO_LARGE = 413,

	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

struct HttpStatusInfo
{
	std::string reasonPhrase;
	std::string defaultBody;

	HttpStatusInfo();
	HttpStatusInfo(const std::string& reason, const std::string& body);
};

HttpStatusInfo getStatusInfo(int statusCode);

#endif
