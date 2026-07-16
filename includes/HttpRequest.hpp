/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:07:13 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/16 11:50:54 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <cstddef>
#include <string>

#include "Request.hpp"

class RequestParser
{
	public:
		RequestParser();

		void feed(const char *buffer, size_t bytes);

		bool isComplete() const;
		bool hasError() const;

		Request getRequest() const;
		void reset();

	private:

		void parseHeaders();
		void parseBody();

		bool parseRequestLine(const std::string &line);
		bool parseHeaderLine(const std::string &line);

	private:

		enum State
		{
			PARSING_HEADERS,
			PARSING_BODY,
			COMPLETE,
			ERROR
		};

		State _state;

		bool _requestLineParsed;

		std::string _stream;

		Request _request;

		size_t _contentLength;

		bool _chunked;
};
#endif
