/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBodyValidation.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 11:46:22 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/24 12:21:11 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "HttpStatus.hpp"

bool RequestParser::validateBodySize() const
{

//TODO if (request.contentLength > serverConfig.clientMaxBodySize) TODO mas adelante
	if (_contentLength > _clientMaxBodySize)
		return false;
	return true;
}
