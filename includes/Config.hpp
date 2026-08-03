/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/03 20:49:26 by mcuenca-         ###   ########.fr       */
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

		class ConfigBlockException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Brace is open.");}
		};

		class ConfigAnyServerException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("\'Server\' not found.");}
		};

		class ConfigOpenBraceException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Expected '{' after 'server'.");}
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

		size_t	jumpHeader(std::vector<std::string>& lines);
		void	tokenizer(std::string& str, std::vector<std::string>& tokens);
		void	tokenizerStruct(std::vector<t_directive>& tokensStruct,
					std::vector<std::string>& tokens,
					size_t& start, size_t& end);
		size_t	findStart(std::vector<std::string>& tokens, size_t size, size_t & n);
		size_t	findEnd(std::vector<std::string>& tokens, size_t size, size_t& n);
};

#endif
