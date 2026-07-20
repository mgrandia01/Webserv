#include "RequestParser.hpp"

#include <iostream>
#include <string>


void test(const std::string &req)
{
	RequestParser parser;
	//TODO existe la funcion parser.reset(), para poder reutilizar el objeto
	parser.feed(req.c_str(), req.size());
	
	if (parser.hasError())
	{
		std::cout << "ERROR " << parser.getErrorCode() << std::endl;
		return;
	}

	if (parser.isComplete())
	{
		Request r = parser.getRequest();

		std::cout << r.method << std::endl;
		std::cout << r.target << std::endl;
		std::cout << r.version << std::endl;
	}
	else
		std::cout << "INCOMPLETE\n";
}
/*
int main()
{
	std::cout << "----- Test 1 -----" << std::endl;
	test(
		"GET /%20 HTTP/1.1\r\n"
		"\r\n");

	std::cout << "\n----- Test 2 -----" << std::endl;
	test(
		"GET /index.html HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"\r\n");

	std::cout << "\n----- Test 3 -----" << std::endl;
	test(
		"GET /\r\n"
		"\r\n");

  return 0;
}*/

int main()
{
    std::cout << "----- Test 1: GET válido -----" << std::endl;
    test(
        "GET / HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 2: Target con fichero -----" << std::endl;
    test(
        "GET /index.html HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 3: POST válido -----" << std::endl;
    test(
        "POST /upload HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 4: DELETE válido -----" << std::endl;
    test(
        "DELETE /file.txt HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 5: Método no soportado (501) -----" << std::endl;
    test(
        "PUT / HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 6: HTTP/1.0 -----" << std::endl;
    test(
        "GET / HTTP/1.0\r\n"
        "\r\n");

    std::cout << "\n----- Test 7: HTTP/2 (505) -----" << std::endl;
    test(
        "GET / HTTP/2\r\n"
        "\r\n");

    std::cout << "\n----- Test 8: Falta versión (400) -----" << std::endl;
    test(
        "GET /\r\n"
        "\r\n");

    std::cout << "\n----- Test 9: Falta target (400) -----" << std::endl;
    test(
        "GET HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 10: Espacio al principio (400) -----" << std::endl;
    test(
        " GET / HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 11: Espacio al final (400) -----" << std::endl;
    test(
        "GET / HTTP/1.1 \r\n"
        "\r\n");

    std::cout << "\n----- Test 12: Doble espacio método-target (400) -----" << std::endl;
    test(
        "GET  / HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 13: Doble espacio target-version (400) -----" << std::endl;
    test(
        "GET /  HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 14: Campo extra (400) -----" << std::endl;
    test(
        "GET / HTTP/1.1 EXTRA\r\n"
        "\r\n");

    std::cout << "\n----- Test 15: Target sin '/' (400) -----" << std::endl;
    test(
        "GET index.html HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 16: Percent-encoding válido -----" << std::endl;
    test(
        "GET /hola%20mundo HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 17: Percent-encoding inválido (%ZZ) -----" << std::endl;
    test(
        "GET /hola%ZZ HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 18: Percent-encoding incompleto (%) -----" << std::endl;
    test(
        "GET /hola% HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 19: Percent-encoding incompleto (%A) -----" << std::endl;
    test(
        "GET /hola%A HTTP/1.1\r\n"
        "\r\n");

    std::cout << "\n----- Test 20: Header Host -----" << std::endl;
    test(
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n");

    return 0;
}


//TODO Arcadio tiene que hacer

/*while (true)
{
	bytes = recv(...);

	parser.feed(buffer, bytes);

	if (parser.isComplete())
		break;

	if (parser.hasError())
		break;

	// Esperar otro recv()
}*/
