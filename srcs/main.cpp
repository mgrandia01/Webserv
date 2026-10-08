/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:51:44 by mgrandia          #+#    #+#             */
/*   Updated: 2026/10/08 11:50:16 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <map>
#include <string>

#include "Config.hpp"
#include "ParserUtils.hpp"
#include "Response.hpp"
#include "ServerManager.hpp"


#include "http/RequestParser.hpp"
#include "http/HttpSerializer.hpp"
#include "CgiExecve.hpp"

#include <cstring>

//1. el main has de restaurarlo
//2. Cgi meterla en un include
//3. Que Cgi funcione con HTTP Request
//4. Que Cgi std::string path, std::string query sean const
//
//EL listen se sobreescribe, ojo ahi

int main(int argc, char **argv)
{
	if ((argc > 2))
	{
		std::cerr << "Usage: ./webserv [config.conf]\n";
		return (1);
	}
	
	try
	{
		const char *fileName;

		if (argc == 1)
		{
			std::cout << "Loading default file configuration" << std::endl;
			fileName = "config/default.conf";
		}
		else
		{
			std::cout << "Loading " << argv[1] << " configuration file..." << std::endl;
			fileName = argv[1];
		}
		
		Config 		config(fileName);

		ServerManager manager(config);
		manager.init();
		manager.printSockets();
		manager.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
