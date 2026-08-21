/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:16:27 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/21 20:14:49 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "LocationConfig.hpp"
# include "utils.hpp"

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

		class ServerConfigDirectiveUnknowException : public std::runtime_error
		{
			public:
				ServerConfigDirectiveUnknowException(std::string& directive)
										: std::runtime_error(
										"\'" + directive + "\' unknown directive."){}
		};

		class ServerConfigArgsException : public std::runtime_error
		{
			public:
				ServerConfigArgsException(std::string& directive,
										char sign,
										int expected,		
										const std::vector<std::string>& args)
										: std::runtime_error(
										"Directive " + directive +
										" expects " + std::string(1, sign) + intToString(expected) +
										" arguments, but " + intToString(args.size()) +
										" were provided: " + vectorToString(args)){}

		};

		class ServerConfigIpException : public std::runtime_error
		{
			public:
				ServerConfigIpException(std::string msg) : std::runtime_error(msg){}
		};

		class ServerConfigErrorCodeOutLimitsException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("A error code is out of limits.");}
		};
		
		class ServerConfigUnsignedNumberException : public std::runtime_error
		{
			public:
				ServerConfigUnsignedNumberException (const std::string& directive,
													const std::string& token)
													: std::runtime_error(
													"Directive " + directive +
													" \'" + token +
													"\' has invalid value, numeric values must be non-negative."){}
		};

		class ServerConfigInvalidUnitException : public std::runtime_error
		{
			public:
				ServerConfigInvalidUnitException(std::string& directive, std::string& arg, std::string& unit)
												: std::runtime_error(
												"Directive " + directive +
												" in arg \'" + arg +
												"\' contains an invalid \'" + unit +
												"\' unit suffix."){}

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

		class ServerConfigWrongChildrenException : public std::runtime_error
		{
			public:
				ServerConfigWrongChildrenException(const t_directive& directive, int lvl)
												: std::runtime_error(
												"Some children do not belong to this directive.\n" + directiveToString(directive, lvl)){}

		};

		class ServerConfigServerNameRegex : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Regex is not supported for directive 'server_name'");}
		};

		class ServerConfigSlashException : public std::runtime_error
		{
			public:
				ServerConfigSlashException(std::string& directive)
												: std::runtime_error(
												"Directive \'" + directive +
												"\' must start with \'/\'."){}
		};

		class	ServerConfigDupException : public std::runtime_error
		{
			public:
				ServerConfigDupException(std::string& directive)
												: std::runtime_error(
												"Directive \'"+ directive +
												"\' is duplicated."){}
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
