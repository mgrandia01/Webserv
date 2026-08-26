/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserExceptions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:54:34 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/25 20:47:00 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_EXCEPTIONS_HPP
# define PARSER_EXCEPTIONS_HPP

# include <iostream>
# include <map>
# include <vector>
# include "ParserUtils.hpp"

class	EmptyStringExc : public std::runtime_error
{
	public:
		EmptyStringExc(std::string from, const std::string& directive)
			: std::runtime_error
			("From " + from + ": directive \'" + directive + "\' have a empty argument."){}
};

class SlashExc : public std::runtime_error
{
	public:
		SlashExc(std::string from, const std::string& directive)
			: std::runtime_error
			("From " + from + ": directive \'" + directive + "\' must start with '/' or './'."){}
};

class	DupExc : public std::runtime_error
{
	public:
		DupExc(std::string from, std::string& directive)
			: std::runtime_error("From " + from + ": directive \'"+ directive + "\' is duplicated."){}
};

class	NumberExc : public std::runtime_error
{
	public:
		NumberExc(std::string from, const std::string& directive, const std::string& tk)
			: std::runtime_error("From " + from + 
								": parameter \'" + tk +
								"\' of directive \'" + directive + 
								"\' must be a non-negative numeric value."){}
};

class	UnknowDirectiveExc : public std::runtime_error
{
	public:
		UnknowDirectiveExc(std::string from, const std::string& directive)
			: std::runtime_error("From " + from +  ": unknown directive \'"+ directive + "\'."){}
};

class	ArgsExc : public std::runtime_error
{
	public:
		ArgsExc(std::string from, const std::string& directive, std::string sign, int expected, const std::vector<std::string>& args)
			: std::runtime_error("From " + from + ": directive \'"+ directive +
								"\' expects " + sign +
								" " + intToString(expected) +
								" argument(s), but " + intToString(args.size()) +
								" were provided: " + vectorToString(args)){}
};

class WrongChildrenExc : public std::runtime_error
{
	public:
		WrongChildrenExc(const t_directive& directive, int lvl)
			: std::runtime_error("Some children do not belong to this directive.\n" + directiveToString(directive, lvl)){}

};

class RegexExc : public std::runtime_error
{
	public:
		RegexExc(std::string from, const std::string& directive)
			: std::runtime_error("From " + from + ": regex is not supported for directive \'" + directive + "\'."){}
};

class CodeOutLimitsExc : public std::runtime_error
{
	public:
		CodeOutLimitsExc(std::string from, const std::string& directive, const std::string& tk, size_t min, size_t max)
			: std::runtime_error("From " + from +
                                ": parameter \'" + tk +
                                "\' of directive \'" + directive +
								" must be between " + intToString(min) + 
								" and " + intToString(max)){}
};

class InvalidUnitException : public std::runtime_error
{
	public:
		InvalidUnitException(const std::string& directive, std::string& arg, std::string& unit)
			: std::runtime_error("Directive " + directive + " in arg \'" + arg + "\' contains an invalid \'" + unit + "\' unit suffix."){}

};

#endif
