/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/28 11:12:55 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"
#include <cassert>
#include <cstdlib>

HttpResponse HttpHandler::handleGet(const HttpRequest&)
{
    HttpResponse response;

    // TODO: Implement GET

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

	//NUNCA llega aqui
	assert(false && "Unexpected HTTP method");
	std::abort();
}

