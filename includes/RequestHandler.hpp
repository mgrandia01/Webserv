/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:40:09 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/01 11:40:13 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "http/HttpRequest.hpp"
#include "Response.hpp"
#include "Config.hpp"

class RequestHandler
{
	public:
		
		RequestHandler();
		RequestHandler(const RequestHandler &other);
		RequestHandler &operator=(const RequestHandler &other);
		~RequestHandler();

		void handle(const HttpRequest& request, const ServerConfig& server, Response& response);

	private:

		
};
#endif
