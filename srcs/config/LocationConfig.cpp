/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:45:44 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/04 20:25:18 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "structs.hpp"
#include "LocationConfig.hpp"

LocationConfig::LocationConfig(){}

LocationConfig::LocationConfig(const t_directive& tk)
{
	_allowMethods[GET] = false;
	_allowMethods[DELETE] = false;
	_allowMethods[POST] = false;
	_autoindex = false;

	uriDirective(tk);
	
	std::vector<t_directive>	childrenStruct = tk.children;
	std::map<std::string, locationDirFunc>	childFuncMap;

	childFuncMap["allow_methods"] = &LocationConfig::allowMethodsDirective;
	childFuncMap["root"] = &LocationConfig::rootDirective;//How heritate server root?
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

	std::cout << getUri() << std::endl;
	std::cout << getMethodGet() << std::endl;
	std::cout << getMethodPost() << std::endl;
	std::cout << getMethodDelete() << std::endl;
	std::cout << getRoot() << std::endl;
	//INDEX
	std::cout << getAutoindex() << std::endl;
	std::cout << getUploadStore() << std::endl;
	//CGI
	//RETURN
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

/* ******************************** get & set ******************************* */

const std::string&	LocationConfig::getUri(){return (_uri);}

const bool&	LocationConfig::getMethodGet(){return (_allowMethods[GET]);}

const bool&	LocationConfig::getMethodPost(){return (_allowMethods[POST]);}

const bool&	LocationConfig::getMethodDelete(){return (_allowMethods[DELETE]);}

const std::string&	LocationConfig::getRoot(){return (_root);}

const bool&	LocationConfig::getAutoindex(){return (_autoindex);}

const std::string&	LocationConfig::getUploadStore(){return (_uploadStore);}

/* ************************* member funcs / methods ************************* */

void	LocationConfig::uriDirective(const t_directive& tk)
{
	if (tk.args.size() < 1)
		throw LocationConfigInsufArgsException();

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

void	LocationConfig::indexDirective(const t_directive& child)//YET
{
	std::cout << "\t" << child.name << std::endl;//Server _index is vector, Location have to?
}

void	LocationConfig::rootDirective(const t_directive& child)//How heritate server root?
{
	if (child.args.size() < 1)
		throw LocationConfigInsufArgsException(); 

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
		throw LocationCofigAutoindexException();
}

void	LocationConfig::uploadStoreDirective(const t_directive& child)
{
	if (child.args.size() < 1)
		throw LocationConfigInsufArgsException();
	
	_uploadStore = child.args[0];
}

void	LocationConfig::cgiDirective(const t_directive& child)//YET
{
	std::cout << "\t" << child.name << std::endl;//
}

void	LocationConfig::returnDirective(const t_directive& child)//YET
{
	std::cout << "\t" << child.name << std::endl;//
}
