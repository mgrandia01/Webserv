/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:29:23 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/18 09:29:25 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Config.hpp"

Config::Config(const std::string& file)
{
	(void) file;


	//TODO parcheado para testing
	//ServerConfig server("IP", port, "host", "root", timeoutHeader, timeoutBody, timeoutSend, timeoutKeepAlive);
	
	ServerConfig server("127.0.0.1", 8080, "webserv", "./www", 5, 15, 1, 25);
	ServerConfig server2("127.0.0.1", 8081, "webserv2", "./www2", 10, 15, 20, 25);
	
	_servers.push_back(server);
	_servers.push_back(server2);
}

Config::~Config(){}

const std::vector<ServerConfig>& Config::getServers() const
{
    return (_servers);
}








ServerConfig::ServerConfig(const std::string& host, int port, const std::string& serverName, const std::string& root, int clientHeaderTimeout,
							int clientBodyTimeout, int sendTimeout, int keepAliveTimeout) : _host(host), _port(port),  _serverName(serverName),
							_root(root), _clientHeaderTimeout(clientHeaderTimeout), _clientBodyTimeout(clientBodyTimeout), _sendTimeout(sendTimeout),
							_keepAliveTimeout(keepAliveTimeout) {};
ServerConfig::~ServerConfig(){}
ServerConfig::ServerConfig(const ServerConfig& other)
{
 	*this = other;
}

ServerConfig&	ServerConfig::operator=(const ServerConfig& rhs)
{
	if (this != &rhs)
	{
        	_host = rhs._host;
		_port = rhs._port;
		_serverName = rhs._serverName;
		_root = rhs._root;
		_att1 = rhs._att1;
		_att2 = rhs._att2;
	    _locations = rhs._locations;
	    _clientHeaderTimeout = rhs._clientHeaderTimeout;
	    _clientBodyTimeout = rhs._clientBodyTimeout;
	    _sendTimeout = rhs._sendTimeout;
	    _keepAliveTimeout = rhs._keepAliveTimeout;
	}
	
	return *this;
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const
{
    return (_locations);
}

const std::string&	ServerConfig::getHost() const
{
	return _host;
}

int			ServerConfig::getPort() const
{
	return _port;
}

const std::string&	ServerConfig::getServerName() const
{
	return _serverName;
}

const std::string&	ServerConfig::getRoot() const
{
	return _root;
}

int ServerConfig::getClientHeaderTimeout() const
{
    return _clientHeaderTimeout;
}

int ServerConfig::getClientBodyTimeout() const
{
    return _clientBodyTimeout;
}

int ServerConfig::getSendTimeout() const
{
    return _sendTimeout;
}

int ServerConfig::getKeepAliveTimeout() const
{
    return _keepAliveTimeout;
}




LocationConfig::LocationConfig(){}
LocationConfig::~LocationConfig(){}
LocationConfig::LocationConfig(const LocationConfig& other)
{
 	*this = other;
}

LocationConfig&	LocationConfig::operator=(const LocationConfig& rhs)
{
	if (this != &rhs)
	{
        	_att1 = rhs._att1;
		_att2 = rhs._att2;
	        
	}
	
	return *this;
}


