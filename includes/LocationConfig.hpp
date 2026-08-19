/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:44:51 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/19 21:03:44 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

# include <iostream>
# include <map>
# include <vector>
# include "utils.hpp"

class LocationConfig
{

	public:
		LocationConfig(const t_directive& tk);
		~LocationConfig();

		//SETTERS
		void	setRoot(const std::string& serverRoot);
		void	setIndex(const std::vector<std::string>& serverIndex);

		//GETTERS
		const std::string&							getUri() const;
		const bool&									getMethodGet() const;
		const bool&									getMethodPost() const;
		const bool&									getMethodDelete() const;
		const std::string&							getRoot() const;
		const std::vector<std::string>&				getIndex() const;
		const bool&									getAutoindex() const;
		const std::string&							getUploadStore() const;
		const std::map<std::string, std::string>	getCgi() const;
		const bool& 								getIsEnabledReturn() const;
		const t_return& 							getReturn() const;
		
		//EXCEPTIONS
		class LocationConfigMissedDirectiveException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Unknown location children directive.");}
		};

		class LocationConfigInsufArgsException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Insufficient arguments.");}
		};

		class LocationConfigInvalidMethodException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Invalid HTTP method.");}
		};
		
		class LocationConfigDuplicatedMethodException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Duplicated HTTP method.");}
		};

		class LocationConfigAutoindexException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Autoindex must be \"on\" or \"off\"");}
		};

		class LocationConfigUnisgnedNumberException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Location numeric values must be non-negative.");}
		};

		class LocationConfigRegex : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Regex is not supported for directive 'location'");}
		};

		class LocationConfigSlashException : public std::runtime_error
		{
			public:
				LocationConfigSlashException(const std::string& directive)
												: std::runtime_error(
												"Directive \'" + directive +
												"\' must start with \'/\'."){}
		};




	private:
		typedef void (LocationConfig::*locationDirFunc)(const t_directive&);

		LocationConfig();
		//LocationConfig(const LocationConfig& other);
		//LocationConfig& operator=(const LocationConfig& rhs);

		std::string							_uri;
		bool								_allowMethods[3];
		std::string							_root;
		std::vector<std::string>			_index; 
		bool								_autoindex;
		std::string							_uploadStore;
		std::map<std::string, std::string>	_cgi;
		t_return							_return;

		void	uriDirective(const t_directive& tk);
		void	allowMethodsDirective(const t_directive& child);
		void	rootDirective(const t_directive& child);
		void	indexDirective(const t_directive& child);
		void	autoindexDirective(const t_directive& child);
		void	uploadStoreDirective(const t_directive& child);
		void	cgiDirective(const t_directive& child);
		void	returnDirective(const t_directive& child);
};

std::ostream&	operator<<(std::ostream &out, const LocationConfig& location);

#endif
