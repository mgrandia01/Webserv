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
	ServerConfig(const std::string& host, int port, const std::string& serverName, const std::string& root,
				int clientHeaderTimeout, int clientBodyTimeout, int sendTimeout, int keepAliveTimeout);
	~ServerConfig();
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& rhs);
	
	const std::string&	getHost() const;
	int			getPort() const;
	const std::string&	getServerName() const;
	const std::string&	getRoot() const;

	
	const std::vector<LocationConfig>& getLocations() const;

	int getClientHeaderTimeout() const;
	int getClientBodyTimeout() const;
	int getSendTimeout() const;
	int getKeepAliveTimeout() const;

private:

	ServerConfig();

	//void resolveTimeouts(const GlobalConfig& global);
	// rellenar segun los timeouts globales sin necesidad de guardar los globales
	
	std::string	_host;
	int		_port;
	std::string	_serverName;
	std::string	_root;
	std::string	_att1;
	std::string	_att2;
	std::vector<LocationConfig> _locations;

	int _clientHeaderTimeout;
	int _clientBodyTimeout;
    int _sendTimeout;
    int _keepAliveTimeout;
	


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

	
	//timeouts globales. No vale la pena guardarlos, si cada server no tiene
	// valor de timeout se asigna el default y ya esta, nunca mas se consultaran
	//esto se resuelve la primera vez que se rellena la clase de configuracion leyendo el fichero

	/*int defaultClientHeaderTimeout;
    int defaultSendTimeout;
    int defaultKeepAliveTimeout;*/

};

#endif
