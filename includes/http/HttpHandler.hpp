/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:55:34 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/27 12:57:13 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpHandler
{
	public:
		HttpResponse handle(const HttpRequest& request);

	private:
		HttpResponse handleGet(const HttpRequest& request);
		HttpResponse handlePost(const HttpRequest& request);
		HttpResponse handleDelete(const HttpRequest& request);
};


#endif
