/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:25:50 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/11 14:20:42 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

#include "http/HttpStatus.hpp"

class Response
{
	public:
		Response();
		Response(std::string data);
		~Response();
		Response& operator=(const Response& rhs);
		Response(const Response& other);

		int statusCode;
		std::string reasonPhrase;
		std::map<std::string, std::string> headers;
		std::string body;
		
		static Response createError(HttpStatus status);
		std::string serialize() const;

		const std::string& getStream() const;
		
	private:
		
		
		
		mutable std::string _stream;//FIXME mutable??
	
};

#endif
