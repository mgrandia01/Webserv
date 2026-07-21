/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/18 09:26:04 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <exception>

class LocationConfig {

public:
	LocationConfig();
	~LocationConfig();
	LocationConfig(const LocationConfig& other);
	LocationConfig& operator=(const LocationConfig& rhs);
	

private:


	
	std::string	_att1;
	int		_att2;

};

class ServerConfig {

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
	


};

class Config {

public:
	Config(const std::string& file);
	~Config();
	
	const std::vector<ServerConfig>& getServers() const;
	
	class ConfigException : public std::exception
	{

	public:

		virtual const char *what() const throw()
		{
			return ("Configuration file error");
		}
    	};

private:

	Config();
	Config(const Config& other);
	Config& operator=(const Config& rhs);
	
	std::vector<ServerConfig> _servers;
	std::string	_att1;
	std::string	_att2;
	int		_att3;
	int		_att4;
};

#endif
