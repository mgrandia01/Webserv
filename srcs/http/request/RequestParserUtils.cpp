/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:05:50 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 11:10:29 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RequestParser.hpp"

//TODO utils
std::string RequestParser::trimWhitespace(const std::string &str)
{
	size_t start = 0;
	size_t end = str.length();

	while (start < end && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	return str.substr(start, end - start);
}

//TODO utils i posar al hpp
std::string RequestParser::toLower(const std::string &str)
{
    std::string result = str;

    for (size_t i = 0; i < result.size(); ++i)
        result[i] = std::tolower(result[i]);

    return result;
}


