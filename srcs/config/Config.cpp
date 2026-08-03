/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:29:23 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/03 21:01:25 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "structs.hpp"

/* ***************************** constr & destr ***************************** */

Config::Config(){}

Config::Config(const char* file)
{
	std::ifstream				fd(file);
	std::string					buff;
	std::vector<std::string>	lines;

	//READ
	if (!fd.is_open())
		throw std::runtime_error(strerror(errno));
	while (getline(fd, buff))
		lines.push_back(buff);

	//TOKENS
	std::vector<std::string>	tokens;

	size_t	header = jumpHeader(lines);
	for (size_t j = header; j < lines.size(); j++)
		tokenizer(lines[j], tokens);

	//TOKEN STRUCT && VECTOR<SERVERS>	
	const	size_t	size = tokens.size();
	for (size_t n = 0; n < size; n++)
	{
		size_t	start = findStart(tokens, size, n) + 2;
		n++;
		size_t	end = findEnd(tokens, size, n) - 1;
		
		std::vector<t_directive>	tokensStruct;
		tokenizerStruct(tokensStruct, tokens, start, end);

		ServerConfig	tmp(tokensStruct);
		_servers.push_back(tmp);
	}
}

//Config::Config(const Config& src){}

//Config::operator=(const Config& rhs){}

Config::~Config(){}

/* ******************************** get & set ******************************* */

const std::vector<ServerConfig>& Config::getServers() const{return (_servers);}

/* ************************* member funcs / methods ************************* */
void	parserDirective(std::vector<t_directive>& tkStruct,
				std::vector<std::string>& tokens,
				size_t& j)
{
	t_directive	nd;

	nd.name = tokens[j];
	j++;
	while (tokens[j] != ";" && tokens[j] != "{")
	{
		nd.args.push_back(tokens[j]);
		j++;
	}
	if (tokens[j] == ";")
		nd.isBlock = false;
	else if (tokens[j] == "{")
	{
		nd.isBlock = true;
		j++;
		while (tokens[j] != "}")
		{
			parserDirective(nd.children, tokens, j);
			j++;
		}
	}	
	tkStruct.push_back(nd);
}

void	Config::tokenizerStruct(std::vector<t_directive>& tokensStruct,
								std::vector<std::string>& tokens,
								size_t& start, size_t& end)
{
	size_t	j = start;

	while (j < end)
	{
		parserDirective(tokensStruct, tokens, j);
		j++;
	}
}

//PASAR DE LECTURA A TOKEN
void	Config::tokenizer(std::string& str, std::vector<std::string>& tokens)
{
	size_t 		start = 0;
	size_t		len = 0;
	std::string	tmp;

	for (int i = 0; str[i]; i++)
	{
		while (str[i] && isspace(str[i]))
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
		/*Careful rong chars*/
	}
}

size_t  Config::findEnd(std::vector<std::string>& tokens, size_t size, size_t& n)
{
	int		braces = 0;

	while (n < size)
	{
		if (tokens[n] == "{")
			braces++;
		else if (tokens[n] == "}")
			braces--;
		if (braces == 0)
			return (n);
		n++;
	}
	throw ConfigBlockException();
}
size_t	Config::findStart(std::vector<std::string>& tokens, size_t size, size_t& n)
{
	while (n < size && tokens[n] != "server")
		n++;
	if (n == size)
		throw ConfigAnyServerException();
	else if (n + 1 >= size || tokens[n + 1] != "{")
		throw ConfigOpenBraceException();
	return (n);
}

size_t	Config::jumpHeader(std::vector<std::string>& lines)
{
	int	j = 0;

	while (lines[j].find ("server"))
		j++;
	return (j);
}

/*int	find_closing_line(const std::vector<std::string>& lines, size_t& start)
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
}*/
