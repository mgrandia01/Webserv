/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:15:23 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/03 21:14:20 by mcuenca-         ###   ########.fr       */
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
										_clientMaxBodySize(100),
										_clientHeaderTimeout(100),
										_clientBodyTimeout(100),
										_sendTimeout(100),
										_keepAliveTimeout(100)
{
	std::map<std::string, directiveFunc>	tkFuncMap;

	tkFuncMap["listen"] = &ServerConfig::listenDirective;
	tkFuncMap["server_name"] = &ServerConfig::serverNameDirective;
	tkFuncMap["error_page"] = &ServerConfig::errorPageDirective;
	tkFuncMap["client_max_body_size"] = &ServerConfig::clientMaxBodySizeDirective;
	tkFuncMap["root"] = &ServerConfig::rootDirective;
	tkFuncMap["index"] = &ServerConfig::indexDirective;
	tkFuncMap["client_header_timeout"] = &ServerConfig::clientHeaderTimeOut;
	tkFuncMap["client_body_timeout"] = &ServerConfig::clientHeaderTimeOut;
	tkFuncMap["send_timeout"] = &ServerConfig::clientHeaderTimeOut;
	tkFuncMap["keepalive_timeout"] = &ServerConfig::clientHeaderTimeOut;
	tkFuncMap["location"] = &ServerConfig::locationDirective;
	
	for (std::vector<t_directive>::iterator tk = tokensStruct.begin();
		tk != tokensStruct.end(); tk++)
	{
		std::map<std::string, directiveFunc>::iterator func;
		
		func = tkFuncMap.find(tk->name);
		if (func == tkFuncMap.end())
			throw ServerConfigMissedDirectiveException();
		(this->*(func->second))(*tk);
	}
}

ServerConfig::~ServerConfig(){}

/* ******************************** get & set ******************************* */

const std::string&	ServerConfig::getHost() const{return _host;}

const int&	ServerConfig::getPort() const{return _port;}

const bool&	ServerConfig::getDefaultServer() const{return _defaultServer;}

const std::vector<std::string>&	ServerConfig::getServerName() const {return (_serverName);}

const std::vector<t_errorPage>&	ServerConfig::getErrorPage() const {return (_errorPage);}

const size_t&	ServerConfig::getClientMaxBodySize() const{return (_clientMaxBodySize);}

const std::vector<std::string>&	ServerConfig::getIndex() const{return _index;}

const int&	ServerConfig::getClientHeaderTimeout() const{return _clientHeaderTimeout;}

const int&	ServerConfig::getClientBodyTimeout() const{return _clientBodyTimeout;}

const int&	ServerConfig::getSendTimeout() const{return _sendTimeout;}

const int&	ServerConfig::getKeepAliveTimeout() const{return _keepAliveTimeout;}

/* ************************* member funcs / methods ************************* */

void	ServerConfig::listenDirective(const t_directive& tk)
{
	if (tk.args.size() > 2)
		throw ServerConfigInsufArgsException();

	for (size_t j = 0; j < tk.args.size(); j++)
	{
		if (tk.args[j] == "default_server")
			_defaultServer = true;

		size_t	pos = tk.args[j].find(":");
		if (pos != std::string::npos)//x.x.x.x
		{
			if (tk.args[j] == "localhost")
				_host = "127.0.0.1";
			else
				_host = tk.args[j].substr(0, pos);
			_port = atoi(tk.args[j].substr(pos + 1).c_str());
		}
		else
			_port = atoi(tk.args[j].c_str());
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

	nd.src = tk.args.back();

	for (size_t j = 0; j < tk.args.size() - 1; j++)
	{
		int	nCode;

		nCode = atoi(tk.args[j].c_str());
		if (nCode < 300 || nCode > 599)
			throw ServerConfigErrorCodeOutLimitsException();
		nd.codes.push_back(nCode);
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
		_clientMaxBodySize = value * 1000000;
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

	//Units are wrong. Ask to Arcadio whitch unit he uses.
	if (unit == "")
		target = value;
	else if (unit == "ms")
		target = value * 1000;
	else if (unit == "s")
		target = value * 100;
	else if (unit == "m")
		target = value * 10;
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

