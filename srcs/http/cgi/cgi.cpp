/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 17:54:20 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/10 12:01:43 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include "CgiRequest.hpp"
#include "CgiExecve.hpp"
#include "Response.hpp"

Response cgiManager(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request)
{
	HttpStatus	statusCode;
	Response	responseCgi;
	CgiExecve	*cgi = new CgiExecve(server);

	statusCode = cgi->getVars().build(server, location, request);
	if (statusCode != OK)
		return responseCgi.createError(statusCode, server);
	
	statusCode = cgi->cgiExecveFunc();
	if (statusCode != OK)
		return responseCgi.createError(statusCode, server);

	responseCgi.setCgi(cgi);
	
	return (responseCgi);
}
