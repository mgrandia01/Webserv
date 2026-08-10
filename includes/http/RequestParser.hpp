/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:07:13 by mgrandia          #+#    #+#             */
/*   Updated: 2026/08/04 13:35:59 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstddef>
#include <string>
#include <cctype>
#include <iostream>

#include "HttpRequest.hpp"
#include "http/HttpStatus.hpp"

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

		// valorar si cambiar a
		const HttpRequest& getRequest() const;

		HttpRequest getRequest() const;
		HttpStatus getErrorCode() const;
		void reset();

	private:

		// RequestParserHeaders.cpp
		void parseHeaders();
		bool parseHeaderLine(const std::string &line);

		// RequestParserHeadersValidation.cpp
		bool isValidHeaderName(const std::string &key);
		bool validateHeaders();
		bool validateFramingHeaders();
		bool validateTransferEncoding();
		bool validateContentLength();
		bool validateHost();
		bool isValidContentLength(const std::string &value);


		// RequestParserStartLine.cpp
		void parseRequestTarget(const std::string &target);
		bool parseRequestLine(const std::string &line);

		// RequestParserStartLineValidation.cpp
		bool validateRequestLineStructure(const std::string &line);
		bool validateMethod(const std::string &method);
		bool validateVersion(const std::string &version);
		bool validateTarget(const std::string &target);
		
		// RequestParserBody.cpp
		void parseContentLengthBody();
		void parseChunkedBody();
		bool hasBody() const;
		void parseBody();

		// RequestParserBodyValidation.cpp
		bool validateBodySize() const;
		bool isValidHexSize (const std::string &hexSize);
		size_t hexToDecimal(const std::string &hexSize);

		// RequestParserUtils.cpp
		std::string trimWhitespace(const std::string &str);
		std::string toLower(const std::string &str);
	private:
		size_t _clientMaxBodySize;//TODO parche para tener valor del .config

		enum State
		{
			PARSING_HEADERS,
			PARSING_BODY,
			COMPLETE,
			ERROR
		};

		State _state;

		HttpStatus _errorCode;

		bool _requestLineParsed;

		std::string _stream;//buffer recibido

		HttpRequest _request;

		size_t _contentLength;

		bool _chunked;
};
#endif
