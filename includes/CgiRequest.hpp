/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:58:54 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/10/10 11:55:40 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
# include "http/HttpHandler.hpp"
# include "http/HttpStatus.hpp"
# include <iostream>

class	CgiRequest {

	public:
		CgiRequest();
		~CgiRequest();

		const std::string&				getPathname() const;
		const std::vector<std::string>&	getArgv() const;
		const std::vector<std::string>&	getEnv() const;
		const std::string&				getBody() const;
		const size_t&					getMethod() const;
		const size_t&					getContentLength() const;

		HttpStatus	build(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request);
	
	private:
		std::string					_pathname;
		std::vector<std::string>	_argv;
		std::vector<std::string>	_env;
		std::string					_body;
		size_t						_method;
		size_t						_contentLength;

		HttpStatus	validatePathname(const std::string& compiler);
		HttpStatus	validateArguments(const std::string& root, const std::string& uriPath, std::string& cgiFile);
		HttpStatus	buildPathname(const LocationConfig& location, const HttpRequest& request);
		HttpStatus	buildArguments(const LocationConfig& location, const HttpRequest& request);
		HttpStatus	buildEnvironment(const ServerConfig& server, const HttpRequest& request);

		void	ContentLengthCgiFunc(const HttpRequest& request);
		void	MethodCgiFunc(const HttpRequest& request);
		void	BodyCgiFunc(const HttpRequest& request);
};
#endif
