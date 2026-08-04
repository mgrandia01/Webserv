/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:56:09 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/04 13:14:52 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/HttpStatus.hpp"

class HttpResponse
{
	public:
		int statusCode;
		std::string reasonPhrase;

		std::map<std::string, std::string> headers;

		std::string body;

		static HttpResponse createError(HttpStatus status);
};

#endif
