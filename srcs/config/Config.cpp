/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:29:23 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/27 19:04:10 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "ServerConfig.hpp"

/* ***************************** constr & destr ***************************** */

Config::Config(){}

int	find_closing_line(const std::vector<std::string>& lines, size_t& start)
{
	int		braces = 0;

	for (size_t j = start; j < lines.size(); j++)
	{
		for (size_t i = 0; lines[j][i]; i++)
		{
			if (lines[j][i] == '{')
				braces++;
			if (lines[j][i] == '}')
				braces--;
		}
		if (braces == 0)
			return (j);
	}
	return (-1);
}

Config::Config(const char* file)
{
	std::ifstream				fd(file);
	std::string					buff;
	std::vector<std::string>	lines;

	if (!fd.is_open())
		throw std::runtime_error(strerror(errno));
	while (getline(fd, buff))
		lines.push_back(buff);

	//check_"server"_word_func()
		//check_spaces
	for (size_t i = 0; i < lines.size(); i++)
	{
		if (lines[i] == "server {")
		{
			int				end = find_closing_line(lines, i);	

			if (end < 0)
				throw ConfigServerBlockException();
			
			ServerConfig	tmp(lines, i, end);

			_servers.push_back(tmp);
		}
	}
}

//Config::Config(const Config& src){}

//Config::operator=(const Config& rhs){}

Config::~Config(){}

/* ******************************** get & set ******************************* */

const std::vector<ServerConfig>& Config::getServers() const
{
    return (_servers);
}

/*LocationConfig::LocationConfig(){}

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

	return (*this);
}*/
