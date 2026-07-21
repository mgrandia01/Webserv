/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mgrandia <mgrandia@student.42barcelon	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/07/16 13:35:01 by mgrandia		  #+#	#+#			 */
/*   Updated: 2026/07/17 15:04:04 by mgrandia         ###   ########.fr       */
/*																			*/
/* ************************************************************************** */


#include <iostream>
#include <exception>
#include <string>
#include "Config.hpp"
#include "ServerManager.hpp"
#include "RequestParser.hpp"

void test(const std::string &req)
{
	RequestParser parser;
	//TODO existe la funcion parser.reset(), para poder reutilizar el objeto
	parser.feed(req.c_str(), req.size());
	
	if (parser.hasError())
	{
		std::cout << "ERROR " << parser.getErrorCode() << std::endl;
		return;
	}

	if (parser.isComplete())
	{
		Request r = parser.getRequest();

		std::cout << r.method << std::endl;
		std::cout << r.target << std::endl;
		std::cout << r.version << std::endl;
	}
	else
		std::cout << "INCOMPLETE\n";
}


int main(int argc, char **argv)
{
    
    /*
    std::cout << "----- Test 1 -----" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"\r\n");

	std::cout << "\n----- Test 2 -----" << std::endl;
	test(
		"GET /index.html HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"\r\n");

	std::cout << "\n----- Test 3 -----" << std::endl;
	test(
		"GET /\r\n"
		"\r\n");
    
    */


	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv config.conf\n";
		return (1);
	}
	
	try
	{
		Config config(argv[1]);
		
		const std::vector<ServerConfig>& servers = config.getServers();
		
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::cout << "Server " << i << std::endl;
			std::cout << "Host : " << servers[i].getHost() << std::endl;
			std::cout << "Port : " << servers[i].getPort() << std::endl;
			std::cout << "Name : " << servers[i].getServerName() << std::endl;
			std::cout << "Root : " << servers[i].getRoot() << std::endl;
			std::cout << std::endl;
		}
		
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





//TODO Arcadio tiene que hacer

/*while (true)
{
	bytes = recv(...);

	parser.feed(buffer, bytes);

	if (parser.isComplete())
		break;

	if (parser.hasError())
		break;

	// Esperar otro recv()
}*/
