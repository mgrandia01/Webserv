/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSerializer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 15:25:46 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/10 14:43:11 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERIALIZER_HPP
#define HTTPSERIALIZER_HPP

#include <string>
#include "Response.hpp"

class HttpSerializer
{
	public:
		static std::string serialize(const Response& response);
};

#endif
