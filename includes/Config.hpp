/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/27 19:04:38 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "ServerConfig.hpp"

/*class LocationConfig {

public:
	LocationConfig();
	~LocationConfig();
	LocationConfig(const LocationConfig& other);
	LocationConfig& operator=(const LocationConfig& rhs);
	

private:


	
	std::string	_att1;
	int		_att2;

};*/

class Config {

	public:
		Config(const char* file);
		~Config();
		
		const std::vector<ServerConfig>& getServers() const;

		class ConfigServerBlockException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Brace is open.");}
		};

		/*class ConfigException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Configuration file error");}
		};*/

	private:

		Config();
		Config(const Config& src);
		Config& operator=(const Config& rhs);
		
		std::vector<ServerConfig> _servers;
};

#endif
