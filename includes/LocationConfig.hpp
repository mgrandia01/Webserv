/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:44:51 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/03 21:07:07 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

# include <iostream>
# include <map>
# include <vector>
# include "structs.hpp"

class LocationConfig
{

	public:
		LocationConfig(const t_directive& tk);
		~LocationConfig();
		

	private:
		typedef void (LocationConfig::*locationFunc)(const t_directive&);

		LocationConfig();
		//LocationConfig(const LocationConfig& other);
		//LocationConfig& operator=(const LocationConfig& rhs);

		std::string							_uriPattern;
		bool								_allowMethods[3];
		std::string							_index;//Server _index is vector, Location have to? 
		bool								_autoindex;
		std::string							_uploadStore;
		std::map<std::string, std::string>	_cgi;
		std::vector<t_return>				_return;

		void	uriDirective(const t_directive& tk);
		void	uploadStoreDirective(const t_directive& tk);
};

#endif
