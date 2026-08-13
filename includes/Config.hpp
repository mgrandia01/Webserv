/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/13 17:56:20 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

# include <string>
# include <vector>
# include <exception>
# include "ServerConfig.hpp"
# include "utils.hpp"

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
		
		/*class ConfigGraphException : public std::runtime_error
		{
			public:
				ConfigGraphException(char c, size_t cPos, size_t lineNumber, std::string& line)
									: std::runtime_error(
									"Invalid character \'" + std::string(1, c) +
									"\' position " + intToString(cPos) +
									" on line " + intToString (lineNumber) +
									".\n" + line +
									"\n" + marker(line, cPos)){}
		};*/

		class ConfigUnclosedQuoteException : public std::runtime_error
		{
			public:
				ConfigUnclosedQuoteException(char c, size_t cPos, size_t lineNumber, std::string& line)
									: std::runtime_error(
									"Unclosed quote: expected " + std::string(1, c) +
									" before end of line " + intToString (lineNumber) +
									".\n" + line +
									"\n" + markerQuote(line, cPos)){}
		};

		class ConfigInvalidQuotePlacementException : public std::runtime_error
		{
			public:
				ConfigInvalidQuotePlacementException(char c, size_t cPos, size_t lineNumber, std::string& line)
									: std::runtime_error(
									"Quoted value on line " + intToString (lineNumber) +
									" must be a separate token.\n" + line +
									"\n" + markerMisplaceQuote(line, c, cPos)){}
									//quoted value must be a separate token
		};


	private:

		Config();
		Config(const Config& src);
		Config& operator=(const Config& rhs);
		
		std::vector<ServerConfig> _servers;

		size_t		jumpHeader(std::vector<std::string>& lines);
		void		tokenizer(std::string& str, std::vector<std::string>& tokens, size_t j);
		void		tokenizerStruct(std::vector<t_directive>& tokensStruct,
						std::vector<std::string>& tokens,
						size_t& start, size_t& end);
		size_t		findStart(std::vector<std::string>& tokens, size_t size, size_t & n);
		size_t		findEnd(std::vector<std::string>& tokens, size_t size, size_t& n);
		//bool		isValidChar(char c);
		bool		isSeparator(char c);
		static std::string	marker(std::string& line, size_t cPos);	
		static std::string	markerQuote(std::string& line, size_t cPos);	
		static std::string	markerMisplaceQuote(std::string& line, char c,size_t cPos);	
};

std::ostream& operator<<(std::ostream &out, const Config& config);

#endif
