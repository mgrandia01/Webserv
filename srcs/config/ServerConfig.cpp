/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 20:15:23 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/07/29 21:20:04 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include <map>
#include "Config.hpp"
#include "ServerConfig.hpp"

/* ***************************** constr & destr ***************************** */

ServerConfig::ServerConfig(){}

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

void	tokenizerStruct(std::vector<t_directive>& tokensStruct, std::vector<std::string>& tokens)
{
	size_t	j = 0;
	size_t	len = tokens.size();

	while (j < len)
	{
		parserDirective(tokensStruct, tokens, j);
		j++;
	}
}

//PASAR DE LECTURA A TOKEN
void	tokenizer(std::string str, std::vector<std::string>& tokens)
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
}

ServerConfig::ServerConfig(const std::vector<std::string>& lines, size_t start, size_t end)
{
	std::vector<std::string>	tokens;

	
	for (size_t j = start + 1; j < end; j++)
		tokenizer(lines[j], tokens);
	//PRINT BASIC TOKEN
	/*for (size_t j = 0; j < tokens.size(); j++)
		std::cout << tokens[j] << std::endl;*/
	
	std::vector<t_directive>	tokensStruct;

	tokenizerStruct(tokensStruct, tokens);
	//PRINT SRTUCT TOKEN
	/*for (size_t k = 0; k < tokensStruct.size(); k++)
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
	}*/

	std::map<std::string, directiveFunc>	tkFuncMap;

	//tkFuncMap["listen"] = &ServerConfig::listenDirective;
	tkFuncMap["server_name"] = &ServerConfig::serverNameDirective;
	tkFuncMap["error_page"] = &ServerConfig::errorPageDirective;
	tkFuncMap["client_max_size_body"] = &ServerConfig::clientMaxBodySizeDirective;
	//			^^^^							^^^
	//tkFuncMap["root"] = &ServerConfig::rootDirective;
	//tkFuncMap["index"] = &ServerConfig::indexDirective;

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

const std::vector<std::string>&	ServerConfig::getServerName() const {return (_serverName);}

const std::vector<t_errorPage>&	ServerConfig::getErrorPage() const {return (_errorPage);}

const size_t&	ServerConfig::getClientMaxBodySize() const{return (_clientMaxBodySize);}

const int&	ServerConfig::getPort() const{return _port;}

/* ************************* member funcs / methods ************************* */

void	ServerConfig::listenDirective(const t_directive& tk)
{
	std::cout << "Listen function: It's me, \"" << tk.name << "\".\n";
	/*std::vector<t_directive>	tokens;
	
	tokenizer(lines, tokens, start, end);

	for (size_t k = 0; k < tokens.size(); k++)
	{
		std::cout << k << " " << tokens[k].name << std::endl;
		for (size_t l = 0; l < tokens[k].args.size(); l++)
			std::cout << k << " " << l << " " << tokens[k].args[l] << std::endl;
	}*/
	
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
	
	int	tmp = atoi(tk.args[0].c_str());

	if (tmp < 1)
		throw ServerConfigBodySizeException();

	_clientMaxBodySize = tmp;
}

void	ServerConfig::rootDirective(const t_directive& tk)
{
	std::cout << "Root function: It's me, \"" << tk.name << "\".\n";
}

void	ServerConfig::indexDirective(const t_directive& tk)
{
	std::cout << "Index function: It's me, \"" << tk.name << "\".\n";
}
