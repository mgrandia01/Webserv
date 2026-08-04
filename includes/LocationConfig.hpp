/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 14:44:51 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/04 20:31:45 by mcuenca-         ###   ########.fr       */
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

		//GETTERS
		const std::string&	getUri();
		const bool&			getMethodGet();
		const bool&			getMethodPost();
		const bool&			getMethodDelete();
		const std::string&	getRoot();
		const bool&			getAutoindex();
		const std::string&	getUploadStore();
		
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

		class LocationCofigAutoindexException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{return ("Autoindex must be \"on\" or \"off\"");}
		};

	private:
		typedef void (LocationConfig::*locationDirFunc)(const t_directive&);

		LocationConfig();
		//LocationConfig(const LocationConfig& other);
		//LocationConfig& operator=(const LocationConfig& rhs);

		std::string							_uri;
		bool								_allowMethods[3];
		std::string							_root;//How heritate server root?
		std::string							_index;//YET//Server _index is vector, Location have to? 
		bool								_autoindex;
		std::string							_uploadStore;
		std::map<std::string, std::string>	_cgi;//YET
		std::vector<t_return>				_return;//YET

		void	uriDirective(const t_directive& tk);
		void	rootDirective(const t_directive& child);
		void	allowMethodsDirective(const t_directive& child);
		void	indexDirective(const t_directive& child);
		void	autoindexDirective(const t_directive& child);
		void	uploadStoreDirective(const t_directive& child);
		void	cgiDirective(const t_directive& child);
		void	returnDirective(const t_directive& child);
};

#endif
