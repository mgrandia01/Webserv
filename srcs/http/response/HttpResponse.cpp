/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:17:15 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/15 11:21:40 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <iostream>

HttpResponse::HttpResponse() {}

void HttpResponse::hello() const
{
	std::cout << "Hello from HttpResponse!" << std::endl;
}
