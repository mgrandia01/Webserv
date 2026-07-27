/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:16:27 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/07/27 20:45:59 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <exception>

/*typedef struct	s_location
{
	bool		cgi;
	std::string	name;
	std::string	type;
	std::string	path;
}	t_location;*/


typedef struct	s_directive
{
	bool						isBlock;
	std::string					name;
	std::vector<std::string>	args;
	std::vector<s_directive>	children;
}	t_directive;

class ServerConfig
{
	public:
		ServerConfig(const std::vector<std::string>& lines, size_t start, size_t end);
		~ServerConfig();

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

		const std::string&	getHost() const;
		int					getPort() const;
	
	private:
		ServerConfig();
		//ServerConfig(const ServerConfig& src);
		//ServerConfig& operator=(const ServerConfig& rhs);

		std::string	_host;
		int			_port;

		//listen_directive(std::string& str);//

};

/*class ServerConfig {

public:
	ServerConfig(const std::string& host, int port, const std::string& serverName, const std::string& root);
	~ServerConfig();
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& rhs);
	
	const std::string&	getHost() const;
	int			getPort() const;
	const std::string&	getServerName() const;
	const std::string&	getRoot() const;

	
	const std::vector<LocationConfig>& getLocations() const;

private:

	ServerConfig();


	std::string	_host;
	int		_port;
	std::string	_serverName;
	std::string	_root;
	std::string	_att1;
	std::string	_att2;
	std::vector<LocationConfig> _locations;
	


};*/

#endif
