/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:25:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/21 17:59:46 by mcuenca-         ###   ########.fr       */
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
		class ConfigLenExtensionException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Length cannot be less than the extension length.");}
		};

		class ConfigExtensionException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Extension does not match the expected extension.");}
		};

		class ConfigEmptyFileException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("File is empty.");}
		};

		class ConfigBlockException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Mismatched braces.");}
		};

		class ConfigAnyServerException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("\'Server\' not found.");}
		};

		class ConfigBraceServerException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Expected '{' after 'server'.");}
		};

		class ConfigMissedCharException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Invalid character between server blocks.");}
		};
		
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
		};

		class ConfigSemiColonException : public std::runtime_error
		{
			public:
				ConfigSemiColonException(std::string& directive)
									: std::runtime_error(
									"Previous directive \"" + directive +
									"\" does not have a ';' before closing the block."){}
		};

		class ConfigVirtualServerException : public std::runtime_error
		{
			public:
				ConfigVirtualServerException(std::string& ip, int port)
									: std::runtime_error(
									"Duplicate listening address: \"" + ip +
									":" + intToString(port) +
									"\".\nMultiple servers are using the same IP:port, possibly because the default values were applied."){}
		};


	private:

		Config();
		//Config(const Config& src);
		//Config& operator=(const Config& rhs);
		
		std::vector<ServerConfig> _servers;

		void		checkExtension(const char* file);
		size_t		jumpHeader(std::vector<std::string>& lines);
		void		tokenizer(std::string& str, std::vector<std::string>& tokens, size_t j);
		void		tokenizerStruct(std::vector<t_directive>& tokensStruct,
						std::vector<std::string>& tokens,
						size_t& start, size_t& end);
		void		parserDirective(std::vector<t_directive>& tkStruct,
						std::vector<std::string>& tokens, size_t& j);
		size_t		findStart(std::vector<std::string>& tokens, size_t size, size_t & n);
		size_t		findEnd(std::vector<std::string>& tokens, size_t size, size_t& n);
		bool		isSeparator(char c);
		void		checkVirtualServers();
		static std::string	marker(std::string& line, size_t cPos);	
		static std::string	markerQuote(std::string& line, size_t cPos);	
		static std::string	markerMisplaceQuote(std::string& line, char c,size_t cPos);
};

std::ostream& operator<<(std::ostream &out, const Config& config);

#endif
