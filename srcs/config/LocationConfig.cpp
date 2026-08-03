/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:45:44 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/03 21:11:32 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "LocationConfig.hpp"

LocationConfig::LocationConfig(){}

LocationConfig::LocationConfig(const t_directive& tk)
{
	std::cout << "PARAM constructor of the called." << tk.name << std::endl;
	//std::map<std::string, directiveFunc>	tkFuncMap;
}

LocationConfig::~LocationConfig(){}

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

/* ************************* member funcs / methods ************************* */

void	LocationConfig::uriDirective(const t_directive& tk)
{
	/*if (tk.args.size() < 1)
		throw LocationConfigInsufArgsException();//CREATE A EXCEPTION */

	_uriPattern = tk.args[0];
}

void	LocationConfig::uploadStoreDirective(const t_directive& tk)
{
	/*if (tk.args.size() < 1)
		throw LocationConfigInsufArgsException();//CREATE A EXCEPTION */

	_uploadStore = tk.args[0];
}
