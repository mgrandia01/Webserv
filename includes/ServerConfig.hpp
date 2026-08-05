/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:16:27 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/05 18:18:45 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "LocationConfig.hpp"
# include "structs.hpp"

class ServerConfig
{
	public:
		//CONSTRUCTOR
		ServerConfig(std::vector<t_directive>& tokensStruct);
		~ServerConfig();

		//GETTERS
		const std::string&				getHost() const;
		const int&						getPort() const;
		const bool&						getDefaultServer() const;
		const std::vector<std::string>&	getServerName() const;
		const std::vector<t_errorPage>&	getErrorPage() const;
		const size_t&					getClientMaxBodySize() const;
		const std::vector<std::string>&	getIndex() const;
		const int&						getClientHeaderTimeout() const;
		const int&						getClientBodyTimeout() const;
		const int&						getSendTimeout() const;
		const int&						getKeepAliveTimeout() const;

		//EXCEPTIONS
		class ServerConfigSemicolonPosException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Semicolon is at wrong position.");}
		};

		class ServerConfigNotSemicolonException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("There is not semicolon.");}
		};

		class ServerConfigMissedDirectiveException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Unknown directive.");}
		};

		class ServerConfigInsufArgsException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Insufficient arguments.");}
		};

		class ServerConfigErrorCodeOutLimitsException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("A error code is out of limits.");}
		};
		
		class ServerConfigUnisgnedNumberException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Numeric values must be non-negative.");}
		};

		class ServerConfigInvalidUnitException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Invalid unit suffix.");}
		};

		class ServerConfigRootException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Location has no \'root\' and server does not provide a default \'root\'.");}
		};
	
		class ServerConfigIndexException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Location has no \'index\' and server does not provide a default \'index\'.");}
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
		void	listenDirective(const t_directive& tk);//atoi?
		void	serverNameDirective(const t_directive& tk);
		void	errorPageDirective(const t_directive& tk);//atoi?
		void	clientMaxBodySizeDirective(const t_directive& tk);
		void	rootDirective(const t_directive& tk);
		void	indexDirective(const t_directive& tk);
		void	locationDirective(const t_directive& tk);
		void	clientHeaderTimeOut(const t_directive& tk);
		void	clientBodyTimeOut(const t_directive& tk);
		void	sendTimeOut(const t_directive& tk);
		void	keepAliveTimeOut(const t_directive& tk);

		void	timeoutParser(int& target, const t_directive& tk);
		void	resolveConfigDefaults();
};

#endif
