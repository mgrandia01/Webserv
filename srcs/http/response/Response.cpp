/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:57:48 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/20 11:24:27 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <string>

Response::Response(){}

Response::Response(std::string stream) : _stream(stream){}

Response::~Response(){}

Response::Response(const Response& other)
{

	*this = other;
}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
        _stream = other._stream;
    }
    return *this;
}

const std::string& Response::getStream() const
{
	return _stream;
}
