/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:15:23 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/07 13:36:28 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <cstring>
#include <stdlib.h>
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

/* ***************************** constr & destr ***************************** */

ServerConfig::ServerConfig(){}

ServerConfig::ServerConfig(std::vector<t_directive>& tokensStruct) :
										_host("0.0.0.0"),
										_port(0),
										_defaultServer(false),
										_clientMaxBodySize(1048576),
										_clientHeaderTimeout(60 * 1000),
										_clientBodyTimeout(60 * 1000),
										_sendTimeout(60 * 1000 ),
										_keepAliveTimeout(75 * 1000)
{
	std::map<std::string, directiveFunc>	tkFuncMap;

	tkFuncMap["listen"] = &ServerConfig::listenDirective;
	tkFuncMap["server_name"] = &ServerConfig::serverNameDirective;
	tkFuncMap["error_page"] = &ServerConfig::errorPageDirective;
	tkFuncMap["client_max_body_size"] = &ServerConfig::clientMaxBodySizeDirective;
	tkFuncMap["root"] = &ServerConfig::rootDirective;
	tkFuncMap["index"] = &ServerConfig::indexDirective;
	tkFuncMap["client_header_timeout"] = &ServerConfig::clientHeaderTimeOut;
	tkFuncMap["client_body_timeout"] = &ServerConfig::clientBodyTimeOut;
	tkFuncMap["send_timeout"] = &ServerConfig::sendTimeOut;
	tkFuncMap["keepalive_timeout"] = &ServerConfig::keepAliveTimeOut;
	tkFuncMap["location"] = &ServerConfig::locationDirective;
	
	for (std::vector<t_directive>::iterator it = tokensStruct.begin();
		it != tokensStruct.end(); it++)
	{
		std::map<std::string, directiveFunc>::iterator func;
		
		func = tkFuncMap.find(it->name);
		if (func == tkFuncMap.end())
			throw ServerConfigMissedDirectiveException();
		(this->*(func->second))(*it);
	}
	
	resolveConfigDefaults();
}

ServerConfig::~ServerConfig(){}

/* ******************************** operators ******************************* */

std::ostream& operator<<(std::ostream &out, const ServerConfig& server)
{
	//std::right << std::setw(10)
	//std::left	
	std::cout << "\n\tSERVER" << std::endl;

	out << "\tIP: " << server.getHost() << std::endl;
	out << "\tPort:" << server.getPort() << std::endl;
	
	if (server.getDefaultServer())
		out << "\tDefault: true" << std::endl;
	else
		out << "\tDefault: false" << std::endl;

	std::vector<std::string>	serverName = server.getServerName();
	out << "\tServer name: ";
	for (size_t k = 0; k < serverName.size(); k++)
		out << serverName[k] << " ";
	out << std::endl;

	std::vector<t_errorPage> errorPage = server.getErrorPage();
	for (size_t k = 0; k < errorPage.size(); k++)
	{
		out << "\tError page: ";
		for (size_t l = 0; l < errorPage[k].codes.size(); l++)
			out << errorPage[k].codes[l] << " ";
		out << errorPage[k].errorFile << std::endl;
	}

	out << "\tClient max body size: " << server.getClientMaxBodySize() <<std::endl;
	
	out << "\tRoot: " << server.getRoot() <<std::endl;
	
	std::vector<std::string>	index = server.getIndex();
	out << "\tIndex: ";
	for (size_t k = 0; k < index.size(); k++)
		out << index[k] << " ";
	out << std::endl;
	
	out << "\tClient header timeout: " << server.getClientHeaderTimeout() <<std::endl;
	
	out << "\tClient body timeout: " <<  server.getClientBodyTimeout() <<std::endl;
	
	out << "\tSend timeout: " << server.getSendTimeout() <<std::endl;
	
	out << "\tKeep alive timeout: " << server.getKeepAliveTimeout() <<std::endl;

	std::vector<LocationConfig>	location = server.getLocations();
	for (std::vector<LocationConfig>::const_iterator it = location.begin();
			it != location.end(); it++)
			out << *it;

	return (out);
}


/* ******************************** get & set ******************************* */

const std::string&	ServerConfig::getHost() const {return _host;}

const int&	ServerConfig::getPort() const {return _port;}

const bool&	ServerConfig::getDefaultServer() const {return _defaultServer;}

const std::vector<std::string>&	ServerConfig::getServerName() const {return (_serverName);}

const std::vector<t_errorPage>&	ServerConfig::getErrorPage() const {return (_errorPage);}

const size_t&	ServerConfig::getClientMaxBodySize() const {return (_clientMaxBodySize);}

const std::string&	ServerConfig::getRoot() const {return (_root);}

const std::vector<std::string>&	ServerConfig::getIndex() const {return _index;}

const int&	ServerConfig::getClientHeaderTimeout() const {return _clientHeaderTimeout;}

const int&	ServerConfig::getClientBodyTimeout() const {return _clientBodyTimeout;}

const int&	ServerConfig::getSendTimeout() const {return _sendTimeout;}

const int&	ServerConfig::getKeepAliveTimeout() const {return _keepAliveTimeout;}

const std::vector<LocationConfig>&	ServerConfig::getLocations() const {return (_locations);}

/* ************************* member funcs / methods ************************* */

void	ServerConfig::listenDirective(const t_directive& tk)//atoi?
{
	if (tk.args.size() > 2)
		throw ServerConfigInsufArgsException();

	for (size_t j = 0; j < tk.args.size(); j++)
	{
		if (tk.args[j] == "default_server")
			_defaultServer = true;
		else
		{
			size_t	pos = tk.args[j].find(":");
			std::string	tmp;
			char		*end;
			if (pos != std::string::npos)//x.x.x.x
			{
				if (tk.args[j] == "localhost")
					_host = "127.0.0.1";
				else
					_host = tk.args[j].substr(0, pos);
				tmp = tk.args[j].substr(pos + 1);
				_port = std::strtol(tmp.c_str(), &end, 10);
			}
			else
				_port = std::strtol(tk.args[j].c_str(), &end, 10);
			if (*end != '\0')
				throw ServerConfigUnisgnedNumberException();
		}
	}
}

void	ServerConfig::serverNameDirective(const t_directive& tk)
{	
	if (tk.args.size() < 1)
		throw ServerConfigInsufArgsException();

	_serverName = tk.args;
}

void	ServerConfig::errorPageDirective(const t_directive& tk)
{
	t_errorPage	nd;

	if (tk.args.size() < 2)
		throw ServerConfigInsufArgsException();

	nd.errorFile = tk.args.back();

	for (size_t j = 0; j < tk.args.size() - 1; j++)
	{
		char	*end;
		std::string	tmp = tk.args[j];
		long	value = std::strtol(tmp.c_str(), &end, 10);
		
		if (*end != '\0')
			throw ServerConfigUnisgnedNumberException();
		else if (value < 300 || value > 599)
			throw ServerConfigErrorCodeOutLimitsException();
		nd.codes.push_back(value);
	}

	_errorPage.push_back(nd);
}

void	ServerConfig::clientMaxBodySizeDirective(const t_directive& tk)
{
	if (tk.args.size() != 1)
		throw ServerConfigInsufArgsException();
	
	char		*end;
	std::string	tmp = tk.args[0];
	long		value = std::strtol(tmp.c_str(), &end, 10);
	std::string	unit = tmp.substr(end - tmp.c_str());

	if (unit == "")
		_clientMaxBodySize = value;
	else if (unit == "k")
		_clientMaxBodySize = value * 1000;
	else if (unit == "M")
		_clientMaxBodySize = value * 1048576;
	else
		throw ServerConfigInvalidUnitException();
}

void	ServerConfig::rootDirective(const t_directive& tk)
{
	if (tk.args.size() != 1)
		throw ServerConfigInsufArgsException();

	_root = tk.args[0];
}

void	ServerConfig::indexDirective(const t_directive& tk)
{
	_index = tk.args;
}

void	ServerConfig::timeoutParser(int& target, const t_directive& tk)
{
	if (tk.args.size() != 1)
		throw ServerConfigInsufArgsException();

	char		*end;
	std::string	tmp = tk.args[0];
	long		value = std::strtol(tmp.c_str(), &end, 10);
	std::string	unit = tmp.substr(end - tmp.c_str());

	//NGINX uses ms and Arcadio uses seconds. Check it.
	if (unit == "")
		target = value;
	else if (unit == "ms")
		target = value;
	else if (unit == "s")
		target = value * 1000;
	else if (unit == "m")
		target = value * 60 * 1000;
	else
		throw ServerConfigInvalidUnitException();
}

void	ServerConfig::clientHeaderTimeOut(const t_directive& tk){timeoutParser(_clientHeaderTimeout, tk);}

void	ServerConfig::clientBodyTimeOut(const t_directive& tk){timeoutParser(_clientBodyTimeout, tk);}

void	ServerConfig::sendTimeOut(const t_directive& tk){timeoutParser(_sendTimeout, tk);}

void	ServerConfig::keepAliveTimeOut(const t_directive& tk){timeoutParser(_keepAliveTimeout, tk);}

void	ServerConfig::locationDirective(const t_directive& tk)
{
	LocationConfig	tmp(tk);

	_locations.push_back(tmp);
}

void	ServerConfig::resolveConfigDefaults()
{
	for (std::vector<LocationConfig>::iterator LocationIt = _locations.begin();
			LocationIt != _locations.end(); LocationIt++)
	{
		const bool&	 returnDir = LocationIt->getIsEnabledReturn();
		
		const std::string&	locationRoot = LocationIt->getRoot();
		const std::map<std::string, std::string> locationCgi = LocationIt->getCgi();
		if (_root.empty())
		{
			if (locationRoot.empty() && (returnDir == false || locationCgi.size() > 0))
				throw ServerConfigRootException();
		}
		else
			if (locationRoot.empty() && returnDir == false)
				LocationIt->setRoot(_root);

		const std::vector<std::string>&	locationIndex = LocationIt->getIndex();
		if (_index.size() == 0)
		{
			if (locationIndex.size() == 0 && returnDir == false)
				throw ServerConfigIndexException();
		}
		else
			if (locationIndex.size() == 0 && returnDir == false)
				LocationIt->setIndex(_index);
	}
}
