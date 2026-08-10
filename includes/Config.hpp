/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/06 18:00:24 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "ServerConfig.hpp"

class Config {

	public:
		//CONSTRUCTORS
		Config(const char* file);
		~Config();

		//GETTERS
		const std::vector<ServerConfig>& getServers() const;
		//EXCEPTIONS
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

std::ostream& operator<<(std::ostream &out, const Config& config);

#endif
