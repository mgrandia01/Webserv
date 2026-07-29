/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerGet.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:57:57 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/29 09:56:46 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpHandler.hpp"
#include "http/HttpStatus.hpp"

bool HttpHandler::readFile(int fd, std::string& body)
{
	char buffer[1024];
	while (true)
	{
		ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
		if (bytesRead < 0)
			return (false);

		if (bytesRead == 0)
			break;
		response.body.append(buffer, bytesRead);
	
	}
	return (true);
}
