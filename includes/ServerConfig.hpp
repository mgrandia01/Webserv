/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:16:27 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/07/31 13:54:40 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <exception>

typedef struct	s_directive
{
	bool						isBlock;
	std::string					name;
	std::vector<std::string>	args;
	std::vector<s_directive>	children;
}	t_directive;

typedef struct	s_errorPage
{
	std::vector<int>	codes;
	std::string			src;
}	t_errorPage;

class ServerConfig
{
	public:
		//CONSTRUCTOR
		ServerConfig(const std::vector<std::string>& lines, size_t start, size_t end);
		~ServerConfig();

		//GETTERS
		const std::string&				getHost() const;
		const int&						getPort() const;
		const bool&						getDefaultServer() const;
		const std::vector<std::string>&	getServerName() const;
		const std::vector<t_errorPage>&	getErrorPage() const;
		const size_t&					getClientMaxBodySize() const;
		const std::vector<std::string>&	getIndex() const;

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
		
		class ServerConfigBodySizeException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Client max body size must be an unsigned number.");}
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
		std::vector<ServerConfig>	_locations;

		//FUNCTIONS
		void	listenDirective(const t_directive& tk);
		void	serverNameDirective(const t_directive& tk);
		void	errorPageDirective(const t_directive& tk);
		void	clientMaxBodySizeDirective(const t_directive& tk);
		void	rootDirective(const t_directive& tk);
		void	indexDirective(const t_directive& tk);
};

#endif
