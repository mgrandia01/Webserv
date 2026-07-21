/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:07:13 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/21 13:45:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstddef>
#include <string>

#include "Request.hpp"

class RequestParser
{
	public:
		// Ortodox Canonical Form
		RequestParser();
		RequestParser(const RequestParser &other);
		RequestParser &operator=(const RequestParser &other);
		~RequestParser();


		void feed(const char *buffer, size_t bytes);

		bool isComplete() const;
		bool hasError() const;

		Request getRequest() const;
		int getErrorCode() const;
		void reset();

	private:

		void parseHeaders();
		void parseBody();

		bool validateRequestLineStructure(const std::string &line);
		bool validateMethod(const std::string &method);
		bool validateVersion(const std::string &version);
		bool validateTarget(const std::string &target);
		std::string trimWhitespace(const std::string &str);
		bool parseHeaderLine(const std::string &line);
		bool parseRequestLine(const std::string &line);
		bool validateHeaders();
	private:

		enum State
		{
			PARSING_HEADERS,
			PARSING_BODY,
			COMPLETE,
			ERROR
		};

		State _state;

		int _errorCode;

		bool _requestLineParsed;

		std::string _stream;

		Request _request;

		size_t _contentLength;

		bool _chunked;
};
#endif
