/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:25:50 by mgrandia          #+#    #+#             */
/*   Updated: 2026/09/15 11:03:15 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

#include "http/HttpStatus.hpp"
#include "CGI.hpp"

class ServerConfig;

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
		
		static Response createError(HttpStatus status, const ServerConfig& server);
		static void applyConfiguredErrorPage(Response& response, const ServerConfig& server, HttpStatus status);
		std::string serialize() const;

		const std::string& getStream() const;
		
		void setHeaders(const std::string& contentType);
		static Response createRedirect(int code, const std::string& target);
	
		//	const Cgi_execve* getCgi_execve() const;//FIXME descomentar para el objeto
		//  borrar el de arriba si solo era orientativo y no hay mas partes de codigo que tengan referencias a el 
		//  usamos el get de abajo. Si el de arriba tiene mas referencias, cambiarlas de forma analoga con el de abajo
		CGI* getCgi() const;
    	void setCgi(CGI* cgi);
    	void clearCgi();
	private:
		
		mutable std::string _stream;
		//	Cgi_execve* _cgi_execve;//FIXME object CGI
		//  borrar el de arriba si solo era orientativo y no hay mas partes de codigo que tengan referencias a el
		// usamos el de abajo. Si el de arriba tiene mas referencias, cambiarlas de forma analoga con el de abajo
		CGI* _cgi;
	
};

#endif
