/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:15:23 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/07/27 21:12:38 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include "Config.hpp"
#include "ServerConfig.hpp"

/* ***************************** constr & destr ***************************** */

ServerConfig::ServerConfig(){}

void	tokenizer(const std::vector<std::string>& lines,
					std::vector<t_directive>& tokens,
					size_t start, size_t end)
{
	t_directive	nd;
	t_directive	child;
	bool		keyword = false;

	for (size_t j = start + 1; j < end; j++)
	{
		std::string	str = lines[j];

		for (size_t i = 0; str[i]; i++)
		{
			std::string	tmp;
			size_t		begin;
			size_t		len;

			while (str[i] && isspace(str[i]))
				i++;
			begin = i;
			len = 0;
			while (str[i] && str[i] != '{' && str[i] != '}' && str[i] != ';' && !isspace(str[i]))
			{
				i++;
				len++;
			}
			if (len > 0)
			{
				tmp = str.substr(begin, len);
				std::cout << tmp << std::endl;
				if (keyword == false)
				{
					keyword = true;
					nd.name = tmp; 
				}
				else
					nd.args.push_back(tmp);
			}


			/*if (str[i] && str[i] == '{')
				nd.isBlock = true;	
			else if (str[i] == '}')
			{
				keyword = false;
				tokens.push_back(nd);
			}*/
			if(str[i] == ';')
			{
				keyword = false;
				nd.isBlock = false;
				tokens.push_back(nd);
				nd = t_directive();
			}
		}
	}
}

/*void	tokenizer(std::string str, std::vector<std::string>& tokens)
{
	size_t 		start = 0;
	size_t		len = 0;
	std::string	tmp;

	for (int i = 0; str[i]; i++)
	{
		while (isspace(str[i]))
			i++;
		start = i;
		len = 0;
		while (str[i] && str[i] != '{' && str[i] != '}' && str[i] != ';' && !isspace(str[i]))
		{
			i++;
			len++;
		}
		if (len > 0)
		{
			tmp = str.substr(start, len);
			tokens.push_back(tmp);
		}
		if (str[i] && (str[i] == '{' || str[i] == '}' || str[i] == ';'))
			tokens.push_back(std::string(1, str[i]));
	}
}*/

ServerConfig::ServerConfig(const std::vector<std::string>& lines, size_t start, size_t end)
{
	/*std::vector<std::string>	tokens;
	
	for (size_t j = start + 1; j < end; j++)
		tokenizer(lines[j], tokens);*/
	
	std::vector<t_directive>	tokens;
	
	tokenizer(lines, tokens, start, end);

	for (size_t k = 0; k < tokens.size(); k++)
	{
		std::cout << k << " " << tokens[k].name << std::endl;
		for (size_t l = 0; l < tokens[k].args.size(); l++)
			std::cout << k << " " << l << " " << tokens[k].args[l] << std::endl;
	}
	
	/*for (int i = start + 1; i < end; i++)
	{
		//listen_directive(lines[i]);

		int	listenPos = lines[i].find("listen ");
		int	listenLen = 7;
		int	semicolonPos = lines[i].find(";"); 

		if (semicolonPos == -1)
			throw ServerConfigNotSemicolonException();
		if (semicolonPos < listenPos)
			throw ServerConfigSemicolonPosException();


		int	colonPos = lines[i].find(":");
		int	portPos = listenPos + listenLen;

		if (colonPos != -1)
		{
			int	hostPos = listenPos + listenLen;
			int	hostLen = colonPos- hostPos;

			_host = lines[i].substr(hostPos, hostLen);
			portPos = colonPos + 1;
		}
		else
			_host = "";

		int			portLen = semicolonPos - portPos;
		std::string tmp = lines[i].substr(portPos, portLen);

		_port = atoi(tmp.c_str());

		std::cout << _host  << "\n" << _port << std::endl;

	}*/
}

ServerConfig::~ServerConfig(){}

/* ******************************** get & set ******************************* */

const std::string&	ServerConfig::getHost() const{return _host;}

int			ServerConfig::getPort() const{return _port;}


/*ServerConfig::ServerConfig(const std::string& host, int port, const std::string& serverName, const std::string& root) : _host(host), _port(port), _serverName(serverName), _root(root) {};
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
}*/
