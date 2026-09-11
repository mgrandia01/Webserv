/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:58:54 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/09/11 21:06:54 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
# include "http/HttpHandler.hpp"
#include <iostream>

class	CgiRequest {

	public:
		CgiRequest();
		~CgiRequest();

		const std::string&				getPathname() const;
		const std::vector<std::string>&	getArgv() const;
		const std::vector<std::string>&	getEnv() const;
		const std::string&				getBody() const;

		bool	build(const ServerConfig& server, const LocationConfig& location, const HttpRequest& request);
	
	private:
		std::string					_pathname;
		std::vector<std::string>	_argv;
		std::vector<std::string>	_env;
		std::string					_body;

		bool	validatePathname(const std::string& compiler);
		bool	validateArguments(const std::string& root, const std::string& uriPath, std::string& cgiFile);
		//validateEnvironment();
		bool	buildPathname(const LocationConfig& location, const HttpRequest& request);
		bool	buildArguments(const LocationConfig& location, const HttpRequest& request);
		bool	buildEnvironment(const ServerConfig& server, const HttpRequest& request);
};
#endif
