#include "RequestParser.hpp"

#include <iostream>
#include <string>


#include <iostream>
#include <map>

#include "RequestParser.hpp"

void test(const std::string &req)
{
	RequestParser parser;

	parser.feed(req.c_str(), req.size());

	if (parser.hasError())
	{
		std::cout << "ERROR " << parser.getErrorCode() << std::endl;
		return;
	}

	if (parser.isComplete())
	{
		Request r = parser.getRequest();

		std::cout << "Method : " << r.method << std::endl;
		std::cout << "Target : " << r.target << std::endl;
		std::cout << "Version: " << r.version << std::endl;

		std::cout << "Headers:" << std::endl;

		for (std::map<std::string, std::string>::const_iterator it = r.headers.begin();
			it != r.headers.end(); ++it)
		{
			std::cout << "  [" << it->first << "] = [" << it->second << "]" << std::endl;
		}
	}
	else
	{
		std::cout << "INCOMPLETE" << std::endl;
	}
}

int main()
{
	std::cout << "==============================" << std::endl;
	std::cout << "Test 1 - No headers" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 2 - Host header" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 3 - Multiple headers" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET /index.html HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: keep-alive\r\n"
		"User-Agent: curl/8.0\r\n"
		"Accept: */*\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 4 - Optional whitespace" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"Host:      localhost      \r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 5 - Empty header value" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"X-Test:\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 6 - Spaces only after colon" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"X-Test:        \r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 7 - Missing colon" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"Host localhost\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 8 - Empty header name" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		": localhost\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 9 - Colon inside value" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"Host: localhost:8080\r\n"
		"\r\n");

	std::cout << "\n==============================" << std::endl;
	std::cout << "Test 10 - Duplicate header" << std::endl;
	std::cout << "==============================" << std::endl;
	test(
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Host: google.com\r\n"
		"\r\n");

	return 0;
}
