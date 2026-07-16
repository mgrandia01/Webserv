/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:16:19 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 11:49:07 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <iostream>
#include <string>

int main()
{
  RequestParser parser;

std::string req =
    "GET /index.html HTTP/1.1\r\n"
    "Host: localhost\r\n"
    "\r\n";

parser.feed(req.c_str(), req.size());

if (parser.isComplete())
{
    Request r = parser.getRequest();

    std::cout << r.method << std::endl;
    std::cout << r.target << std::endl;
    std::cout << r.version << std::endl;
    std::cout << r.headers["Host"] << std::endl;
}

	return (0);
}

//TODO Arcadio solo tiene que hacer "parse.feed(buffer, bytes)"
