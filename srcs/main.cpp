/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:16:19 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/15 11:21:55 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

int main()
{
	HttpRequest	request;
	HttpResponse	response;

	request.hello();
	response.hello();

	return (0);
}
