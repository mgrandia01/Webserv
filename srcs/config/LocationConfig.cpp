/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:45:44 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/19 21:02:55 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include "LocationConfig.hpp"

LocationConfig::LocationConfig(){}

LocationConfig::LocationConfig(const t_directive& tk)
{
	_allowMethods[GET] = false;
	_allowMethods[DELETE] = false;
	_allowMethods[POST] = false;
	_autoindex = false;
	_return.isEnabled = false;

	uriDirective(tk);
	
	std::vector<t_directive>	childrenStruct = tk.children;
	std::map<std::string, locationDirFunc>	childFuncMap;

	childFuncMap["allow_methods"] = &LocationConfig::allowMethodsDirective;
	childFuncMap["root"] = &LocationConfig::rootDirective;
	childFuncMap["index"] = &LocationConfig::indexDirective;
	childFuncMap["autoindex"] = &LocationConfig::autoindexDirective;
	childFuncMap["upload_store"] = &LocationConfig::uploadStoreDirective;
	childFuncMap["cgi"] = &LocationConfig::cgiDirective;
	childFuncMap["return"] = &LocationConfig::returnDirective;

	for (std::vector<t_directive>::iterator it = childrenStruct.begin();
			it != childrenStruct.end(); it++)
	{
		std::map<std::string, locationDirFunc>::iterator func;

		func = childFuncMap.find(it->name);
		if (func == childFuncMap.end())
		{
			std::cout << it->name << std::endl; 
			throw LocationConfigMissedDirectiveException();
		}
		(this->*(func->second))(*it);
	}
}


/*LocationConfig::LocationConfig(const LocationConfig& other)
{
 	*this = other;
}*/

/*LocationConfig&	LocationConfig::operator=(const LocationConfig& rhs)
{
	if (this != &rhs)
	{
		//varibales
	}

	return (*this);
}*/

LocationConfig::~LocationConfig(){}

/* ******************************** operators ******************************* */

std::ostream&	operator<<(std::ostream &out, const LocationConfig& location)
{
	out << "\tLocation: " << location.getUri() << std::endl;

	out << "\t\tAllow methods: ";
	if (location.getMethodGet())
		out << "GET ";
	if (location.getMethodPost())
		out << "POST ";
	if (location.getMethodDelete())
		out << "DELETE";
	out << std::endl;

	out << "\t\tRoot: " << location.getRoot() <<std::endl;

	std::vector<std::string>	index = location.getIndex();
	out << "\t\tIndex: ";
	for (size_t k = 0; k < index.size(); k++)
		out << index[k] << " ";
	out << std::endl;

	if (location.getAutoindex())
		out << "\t\tAutoindex: true" << std::endl;
	else
		out << "\t\tAutoindex: false" << std::endl;

	out << "\t\tUpload store: " << location.getUploadStore() << std::endl;

	std::map<std::string, std::string>	cgi = location.getCgi();
	for (std::map<std::string, std::string>::iterator mit = cgi.begin();
			mit != cgi.end(); mit++)
		out << "\t\tCGI: " << mit->first << " " << mit->second << std::endl;

	t_return	tmpReturn = location.getReturn();
	if (tmpReturn.isEnabled)
		out << "\t\treturn: " << tmpReturn.code << " " << tmpReturn.target << std::endl;
	return (out);
}

/* ******************************** get & set ******************************* */

void	LocationConfig::setRoot(const std::string& serverRoot)
{
	if (_root.empty())
		_root = serverRoot;
}

void	LocationConfig::setIndex(const std::vector<std::string>& serverIndex)
{
	if (_index.empty())
		_index = serverIndex;
}

const std::string&	LocationConfig::getUri() const {return (_uri);}

const bool&	LocationConfig::getMethodGet() const {return (_allowMethods[GET]);}

const bool&	LocationConfig::getMethodPost() const {return (_allowMethods[POST]);}

const bool&	LocationConfig::getMethodDelete() const {return (_allowMethods[DELETE]);}

const std::string&	LocationConfig::getRoot() const {return (_root);}

const std::vector<std::string>&	LocationConfig::getIndex() const {return _index;}

const bool&	LocationConfig::getAutoindex() const {return (_autoindex);}

const std::string&	LocationConfig::getUploadStore() const {return (_uploadStore);}

const std::map<std::string, std::string>	LocationConfig::getCgi() const {return (_cgi);}

const bool&	LocationConfig::getIsEnabledReturn() const {return (_return.isEnabled);}

const t_return&	LocationConfig::getReturn() const {return (_return);}

/* ************************* member funcs / methods ************************* */

void	LocationConfig::uriDirective(const t_directive& tk)
{
	if (tk.args.size() < 1)
		throw LocationConfigInsufArgsException();
	else if (tk.args[0] == "~" || tk.args[0] == "~*" || tk.args[0] == "~/")
		throw LocationConfigRegex();

	_uri = tk.args[0];
}

void	LocationConfig::allowMethodsDirective(const t_directive& child)
{
	std::map<std::string, e_methods>	methodsMap;

	methodsMap["GET"] = GET;
	methodsMap["POST"] = POST;
	methodsMap["DELETE"] = DELETE;
	
	for (size_t j = 0; j < child.args.size(); j++)//Este itera los elementos del vector en orden ascendente
	{
		//Este busca en el map que combina Xvar-Yvar, busca 'j' en Xvar.
		//Y devuelve en que posicion lo encuentr.
		//Ejem.: "GET" = 0, "POST" = 1 y "DELETE" = 2 dentro del mapa,
		//no tiene  que seguir el orden del enum (podria ser , "DELETE" = 0, GET" = 1" y "POST" = 2
		//pero si que de combinen adecuamente
		//Me dice que el 'j' es el match del mapa numero N
		std::map<std::string, e_methods>::iterator	method = methodsMap.find(child.args[j]);

		if (method == methodsMap.end())
			throw LocationConfigInvalidMethodException();
		if (_allowMethods[method->second] == true)
			throw LocationConfigDuplicatedMethodException();
		_allowMethods[method->second] = true;
	}
}

void	LocationConfig::indexDirective(const t_directive& child)
{
	_index = child.args;
}

void	LocationConfig::rootDirective(const t_directive& child)
{
	if (child.args.size() < 1)
		throw LocationConfigInsufArgsException(); 
	else if (child.args[0][0] != '/')
		throw LocationConfigSlashException(child.name);

	_root = child.args[0];
}

void	LocationConfig::autoindexDirective(const t_directive& child)
{
	if (child.args.size() < 1)
		throw LocationConfigInsufArgsException();

	if (child.args[0] == "on" || child.args[0] == "ON")
		_autoindex = true;
	else if (child.args[0] == "off" || child.args[0] == "OFF")
		_autoindex = false;
	else
		throw LocationConfigAutoindexException();
}

void	LocationConfig::uploadStoreDirective(const t_directive& child)
{
	if (child.args.size() < 1)
		throw LocationConfigInsufArgsException();
	else if (child.args[0][0] != '/')
		throw LocationConfigSlashException(child.name);
	
	_uploadStore = child.args[0];
}

void	LocationConfig::cgiDirective(const t_directive& child)
{
	if (child.args.size() != 2)
		throw LocationConfigInsufArgsException();

	_cgi[child.args[0]] = child.args[1];
}

void	LocationConfig::returnDirective(const t_directive& child)
{
	if (child.args.size() != 2)
		throw LocationConfigInsufArgsException();

	_return.isEnabled = true;

	char		*end;
	std::string	tmp = child.args[0];
	long		value = std::strtol(tmp.c_str(), &end, 10);

	if (*end != '\0')
		throw LocationConfigUnisgnedNumberException();
	_return.code = value;

	_return.target = child.args[1];
}
