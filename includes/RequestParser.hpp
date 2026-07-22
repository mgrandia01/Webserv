/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:07:13 by mgrandia          #+#    #+#             */
/*   Updated: 2026/07/22 11:45:58 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstddef>
#include <string>
#include <cctype>

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

		// RequestParserHeaders.cpp
		void parseHeaders();
		bool parseHeaderLine(const std::string &line);

		// RequestParserBody.cpp
		void parseBody();

		// RequestParserStartLine.cpp
		bool parseRequestLine(const std::string &line);

		// RequestParserHeadersValidation.cpp
		bool validateHeaders();
		bool validateFramingHeaders();
		bool validateTransferEncoding();
		bool validateContentLength();
		bool validateHost();
		bool isValidContentLength(const std::string &value);

		// RequestParserStartLineValidation.cpp
		bool validateRequestLineStructure(const std::string &line);
		bool validateMethod(const std::string &method);
		bool validateVersion(const std::string &version);
		bool validateTarget(const std::string &target);

		// RequestParserUtils.cpp
		std::string trimWhitespace(const std::string &str);
		std::string toLower(const std::string &str);
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
