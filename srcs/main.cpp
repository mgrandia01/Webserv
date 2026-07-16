/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 13:35:01 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 14:55:25 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

#include <iostream>
#include <string>


void test(const std::string &req)
{
	RequestParser parser;

	parser.feed(req.c_str(), req.size());

	if (parser.hasError())
	{
		std::cout << "ERROR\n";
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

int main()
{
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

  return 0;
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
