/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 18:29:13 by arcmarti          #+#    #+#             */
/*   Updated: 2026/09/16 18:29:15 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>

class CGI
{
public:
    CGI(int clientFd, const std::string& input);
    ~CGI();

    // Identificación / FDs
    int getStdinFd() const;
    int getStdoutFd() const;
    int getClientFd() const;
    pid_t getPid() const;

    // Entrada hacia el CGI para enviar el body del POST	
    // Si es un GET hacer input.empty() cerrando el stdin sin escribir
    const std::string& getInput() const;
    size_t getBytesWritten() const;
    void addBytesWritten(size_t bytes);
    

    // Salida del CGI para recibir el output
    void feed(const char* buffer, size_t size);
    

    // Estado del CGI
    // el CGI ya no tiene más salida disponible
    void collectProcess();
    
    
    time_t getStartTime() const;
    void closeStdinFd();
    void closeStdoutFd();

private:
    CGI();
    CGI(const CGI& other);
    CGI& operator=(const CGI& other);

    int         _clientFd;
    int         _stdinFd;
    int         _stdoutFd;
    pid_t       _pid;

    std::string _input;
    size_t      _bytesWritten;

    time_t _startTime;

    // acumila la salida para al final copiarla a la salida del Response
    std::string _output;

};

#endif

