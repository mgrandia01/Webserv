/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:29:23 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/13 18:08:54 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "utils.hpp"

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
		tokenizer(lines[j], tokens, j);

	//TOKEN STRUCT && VECTOR<SERVERS>	
	const	size_t	size = tokens.size();
	for (size_t n = 0; n < size; n++)
	{
		size_t	start = findStart(tokens, size, n) + 2;
		n++;
		size_t	end = findEnd(tokens, size, n) - 1;
		
		std::vector<t_directive>	tokensStruct;
		tokenizerStruct(tokensStruct, tokens, start, end);
	
///
	for (size_t k = 0; k < tokensStruct.size(); k++)
	{
		std::cout << k << "\t" << tokensStruct[k].name << std::endl;
		//std::cout << std::setw(10) << tokensStruct[k].args[l] << std::endl;
		for (size_t l = 0; l < tokensStruct[k].args.size(); l++)
			std::cout << k << " " << l << "\t\t" << tokensStruct[k].args[l] << std::endl;
		for (size_t l = 0; l < tokensStruct[k].children.size(); l++)
		{
			std::cout << k << " " << l << "\t\t>" << tokensStruct[k].children[l].name << std::endl;
			for (size_t m = 0; m < tokensStruct[k].children[l].args.size(); m++)
				std::cout << k << " " << l << " " << m << "\t\t\t" << tokensStruct[k].children[l].args[m] << std::endl;
		}
	}
//

		ServerConfig	tmp(tokensStruct);
		_servers.push_back(tmp);
	}
}

//Config::Config(const Config& src){}

//Config::operator=(const Config& rhs){}

Config::~Config(){}

/* ******************************** operators ******************************* */

std::ostream& operator<<(std::ostream &out, const Config& config)
{
	std::vector<ServerConfig>	tmp = config.getServers();

	std::cout << "CONFIG FILE" << std::endl;
	for (std::vector<ServerConfig>::const_iterator it = tmp.begin();
			it != tmp.end(); it++)
		out << *it;

	return (out);
}

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
std::string	Config::markerMisplaceQuote(std::string& line, char c, size_t cPos)
{
	std::string	mark;

	for (size_t i = 0; i < cPos; i++)
	{
		if (line[i] == '\t')
			mark += '\t';
		else
			mark += ' ';
	}
	mark += '^';
	for (size_t i = cPos + 1; i < static_cast<size_t>(line.size()) - 1; i++)
	{
		if (line[i] == '\t')
			mark += '\t';
		else if (line[i] == c)
		{
			mark += '^';
			break ;
		}
		else
			mark += '_';
	}
	return (mark);
}

std::string	Config::markerQuote(std::string& line, size_t cPos)
{
	std::string	mark;

	for (size_t i = 0; i < cPos; i++)
	{
		if (line[i] == '\t')
			mark += '\t';
		else
			mark += ' ';
	}
	mark += '^';
	for (size_t i = cPos; i < static_cast<size_t>(line.size()) - 1; i++)
	{
		if (line[i] == '\t')
			mark += '\t';
		else
			mark += '_';
	}
	return (mark);
}

std::string	Config::marker(std::string& line, size_t cPos)
{
	std::string	mark;

	for (size_t i = 0; i < cPos && i < static_cast<size_t>(line.size()); i++)
	{
		if (line[i] == '\t')
			mark += '\t';
		else
			mark += ' ';
	}
	mark += '^';
	return (mark);
}

bool	Config::isSeparator(char c)
{
	return (std::isspace(static_cast<unsigned char>(c))
		|| c == '{'
		|| c == '}'
		|| c == ';'
		|| c == '#');
		//|| c == '\''
		//|| c == '\"');
}

/*bool	Config::isValidChar(char c)
{
	return (std::isalnum(static_cast<unsigned char>(c))
        || c == '_'
		|| c == '-'
       	|| c == '.'
       	|| c == ':'
		|| c == '/');
}*/

void	Config::tokenizer(std::string& str, std::vector<std::string>& tokens, size_t j)
{
	char		quote;
	size_t		i = 0;
	size_t		size = str.size();
	size_t 		start = 0;
	size_t		len = 0;


//caso 1: "example.com" valido
//caso 2: "example .com" valido
//caso 3: example".com" no valido, siempre comienza con comillas
//caso 4: "example\n.com" no valido, no aceptamos saltos de linea, comilla no cerrada
//caso 5: "example.com //y// example.com" no valido, comillas sin cerrar
	while (i < size)
	{
		
		while (std::isspace(static_cast<unsigned char>(str[i])))
			i++;
		
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i];
			start = i;
			len = 0;
			i++;
			while (i < size && str[i] != quote)
			{
				i++;
				len++;
			}
			if (i >= size)
				throw ConfigUnclosedQuoteException(quote, start, j, str);
			else if(i < size && !isSeparator(str[i + 1]))
				throw ConfigInvalidQuotePlacementException(quote, start, j, str);
			tokens.push_back(str.substr(start, len + 2));
			i++;
		}

		if (str[i] == '#')
			return ;
		if (str[i] == '{' || str[i] == '}' || str[i] == ';')
		{
			tokens.push_back(std::string(1, str[i]));
			i++;
		}

		start = i;
		len = 0;
		while (i < size && !isSeparator(str[i]))
		{
			if (str[i] == '\'' || str[i] == '\"')
				throw ConfigInvalidQuotePlacementException(str[i], i, j, str);
			i++;
			len++;
		}
		if (len > 0)
			tokens.push_back(str.substr(start, len));
		else
			i++;
		//if (i < size && !isValidChar(str[i]) && !isSeparator(str[i]))
		//	throw  ConfigGraphException(str[i], i, j, str);
	}
}

/*
void	Config::tokenizer(std::string& str, std::vector<std::string>& tokens)
{
	size_t		i = 0;
	size_t		size = str.size();
	size_t 		start = 0;
	size_t		len = 0;
	std::string	tmp;

	
	while (i < size)
	{
		while (str[i] && isspace(str[i]))
			i++;
		start = i;
		len = 0;
		while (i < size && str[i] != '{' && str[i] != '}' && str[i] != ';' && !isspace(str[i]))
		{
			i++;
			len++;
		}
		if (len > 0)
		{
			tmp = str.substr(start, len);
			tokens.push_back(tmp);
		}
		if (i < size && (str[i] == '{' || str[i] == '}' || str[i] == ';'))
			tokens.push_back(std::string(1, str[i]));
		//Careful wrong chars
		i++;
	}
}*/

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
