/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:25:50 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 10:32:40 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
	public:
		Response();
		Response(std::string data);
		~Response();
		Response& operator=(const Response& rhs);
		Response(const Response& other);

		const std::string& getStream() const;
		
	private:
		
		
		
		std::string _stream;
	
};

#endif
