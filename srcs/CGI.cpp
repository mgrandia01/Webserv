/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 18:28:51 by arcmarti          #+#    #+#             */
/*   Updated: 2026/09/16 18:28:56 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdexcept>
#include <iostream>


CGI::CGI(int clientFd, const std::string& input)
    : _clientFd(clientFd),
      _stdinFd(-1),
      _stdoutFd(-1),
      _pid(-1),
      _input(input),
      _bytesWritten(0),
      _startTime(time(NULL))
{
    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe) == -1)
        throw std::runtime_error("pipe() failed");

    std::cout << "CIG -> inputPipe:  read=" << inputPipe[0] << " write=" << inputPipe[1] << std::endl;

    if (pipe(outputPipe) == -1)
    {
        close(inputPipe[0]);
        close(inputPipe[1]);
        throw std::runtime_error("pipe() failed");
    }

    std::cout << "CGI -> outputPipe: read=" << outputPipe[0] << " write=" << outputPipe[1] << std::endl;

    _pid = fork();

    if (_pid == -1)
    {
        close(inputPipe[0]);
        close(inputPipe[1]);
        close(outputPipe[0]);
        close(outputPipe[1]);
        throw std::runtime_error("fork() failed");
    }

    if (_pid == 0)
    {
        // =========================
        // HIJO = CGI (/bin/cat)
        // =========================

        // stdin del hijo <- pipe de entrada
        dup2(inputPipe[0], STDIN_FILENO);

        // stdout del hijo -> pipe de salida
        dup2(outputPipe[1], STDOUT_FILENO);

        // Ya no necesitamos estos FDs.
        close(inputPipe[0]);
        close(inputPipe[1]);
        close(outputPipe[0]);
        close(outputPipe[1]);

        // CGI temporal de prueba.
        execl("/bin/cat", "cat", (char *)NULL);
        //execl("/bin/sleep", "sleep", "15", (char *)NULL);

        // Si execl falla.
        _exit(1);
    }

    // =========================
    // PADRE = ServerManager
    // =========================

    // El padre escribe en inputPipe[1].
    close(inputPipe[0]);

    // El padre lee de outputPipe[0].
    close(outputPipe[1]);

    _stdinFd = inputPipe[1];
    _stdoutFd = outputPipe[0];

    std::cout << "CGI -> padre conserva:" << " stdinFd=" << _stdinFd << " stdoutFd=" << _stdoutFd << " pid=" << _pid
          << std::endl;
}


CGI::~CGI()
{
    if (_stdinFd != -1)
        close(_stdinFd);

    if (_stdoutFd != -1)
        close(_stdoutFd);

    // Si por algún motivo el proceso todavía sigue vivo,
    // esperamos a recogerlo.
    if (_pid > 0)
        waitpid(_pid, NULL, 0);
}


int CGI::getStdinFd() const
{
    return _stdinFd;
}


int CGI::getStdoutFd() const
{
    return _stdoutFd;
}


int CGI::getClientFd() const
{
    return _clientFd;
}


pid_t CGI::getPid() const
{
    return _pid;
}


const std::string& CGI::getInput() const
{
    return _input;
}


size_t CGI::getBytesWritten() const
{
    return _bytesWritten;
}


void CGI::addBytesWritten(size_t bytes)
{
    _bytesWritten += bytes;
}


void CGI::feed(const char* buffer, size_t size)
{
    // De momento no necesitamos almacenar el output
    // para probar el networking.
    
    //_output.append(buffer, size);
    
    (void)buffer;
    (void)size;
    
    std::cout << "CGI-> Feed: " << size << " bytes" << std::endl;
    std::cout.write(buffer, size);
    std::cout << std::endl;
}


void CGI::collectProcess()
{
    if (_pid > 0)
    {
        int status;
        pid_t result = waitpid(_pid, &status, WNOHANG);

        if (result == _pid)
            _pid = -1;
    }
}

time_t CGI::getStartTime() const
{
    return _startTime;
}

void CGI::closeStdinFd()
{
    if (_stdinFd != -1)
    {
        close(_stdinFd);
        _stdinFd = -1;
    }
}

void CGI::closeStdoutFd()
{
    if (_stdoutFd != -1)
    {
        close(_stdoutFd);
        _stdoutFd = -1;
    }
}