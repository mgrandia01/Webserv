/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:16:51 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/15 11:21:27 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest() {}

void HttpRequest::hello() const
{
	std::cout << "Hello from HttpRequest!" << std::endl;
}
