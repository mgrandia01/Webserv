/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParserBody.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:18:28 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 15:19:16 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

void RequestParser::parseBody()
{
  //TODO:

	_request.body = _stream;
	_stream.clear();

	_state = COMPLETE;
}


