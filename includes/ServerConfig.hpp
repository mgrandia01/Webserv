/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:16:27 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/25 20:47:27 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "LocationConfig.hpp"
# include "ParserUtils.hpp"
# include "ParserExceptions.hpp"

class ServerConfig
{
	public:
		//CONSTRUCTOR
		ServerConfig(std::vector<t_directive>& tokensStruct);
		~ServerConfig();

		//GETTERS
		const std::string&					getHost() const;
		const int&							getPort() const;
		const bool&							getDefaultServer() const;
		const std::vector<std::string>&		getServerName() const;
		const std::vector<t_errorPage>&		getErrorPage() const;
		const size_t&						getClientMaxBodySize() const;
		const std::string&					getRoot() const;
		const std::vector<std::string>&		getIndex() const;
		const int&							getClientHeaderTimeout() const;
		const int&							getClientBodyTimeout() const;
		const int&							getSendTimeout() const;
		const int&							getKeepAliveTimeout() const;
		const std::vector<LocationConfig>&	getLocations() const;

		//EXCEPTIONS

		class ServerConfigIpException : public std::runtime_error
		{
			public:
				ServerConfigIpException(std::string msg) : std::runtime_error(msg){}
		};
	
		class ServerConfigProvideDirectiveException : public std::runtime_error
		{
			public:
				ServerConfigProvideDirectiveException(std::string directive)
										: std::runtime_error(
										"Location has no \'" + directive +
										"\' and server does not provide a default \'" + directive +
										"\'."){}
		};

	private:
		ServerConfig();
		//ServerConfig(const ServerConfig& src);
		//ServerConfig& operator=(const ServerConfig& rhs);

		typedef void (ServerConfig::*directiveFunc)(const t_directive&);

		//VARIABLES
		std::string					_host;
		int							_port;
		bool						_defaultServer;
		std::vector<std::string>	_serverName;
		std::vector<t_errorPage>	_errorPage;
		size_t						_clientMaxBodySize;
		std::string					_root;
		std::vector<std::string>	_index;
		int							_clientHeaderTimeout;
		int							_clientBodyTimeout;
		int							_sendTimeout;
		int							_keepAliveTimeout;
		std::vector<LocationConfig>	_locations;

		//FUNCTIONS
		void	listenDirective(const t_directive& tk);
		void	serverNameDirective(const t_directive& tk);
		void	errorPageDirective(const t_directive& tk);
		void	clientMaxBodySizeDirective(const t_directive& tk);
		void	rootDirective(const t_directive& tk);
		void	indexDirective(const t_directive& tk);
		void	locationDirective(const t_directive& tk);
		void	clientHeaderTimeout(const t_directive& tk);
		void	clientBodyTimeout(const t_directive& tk);
		void	sendTimeout(const t_directive& tk);
		void	keepAliveTimeout(const t_directive& tk);

		void	checkIp(std::string ip);
		void	timeoutParser(int& target, const t_directive& tk);
		void	resolveConfigDefaults();
};

std::ostream& operator<<(std::ostream &out, const ServerConfig& server);

#endif
