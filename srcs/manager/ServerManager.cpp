/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:50:07 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/18 09:50:09 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h> //sleep, close
#include <arpa/inet.h> //address.sin_addr.s_addr = inet_addr(server.host.c_str());
#include <algorithm>

static const int LISTEN_BACKLOG = 128;


ServerManager::ServerManager(const Config& config) : _config(config) {}

ServerManager::~ServerManager()
{
	for (size_t i = 0; i < _pollFds.size(); i++)
		close(_pollFds[i].fd);
}

void	ServerManager::init()
{
	createSockets();
	initPollFds();
	std::cout << "Server listening..." << std::endl;
}

void	ServerManager::createSockets()
{
	const std::vector<ServerConfig>& servers = _config.getServers();

	for (size_t i = 0; i < servers.size(); i++)
	{
		int	socketFd = socket(AF_INET, SOCK_STREAM, 0);
		
		if (socketFd == -1)
			throw std::runtime_error("socket() failed");
			
		int opt = 1;
		if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	 	{
	 		close(socketFd);
	 		throw std::runtime_error("setsockopt() failed");
	 	}
	
		if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
		{
			close(socketFd);
			throw std::runtime_error("fcntl(F_SETFL) failed");
		}
		
		bindSocket(socketFd, servers[i]);
		listenSocket(socketFd);
		_listenSockets.push_back(socketFd);
	}
	
}

void	ServerManager::bindSocket(int socketFd, const ServerConfig& server)
{
	
	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(server.getPort());
	address.sin_addr.s_addr = inet_addr(server.getHost().c_str());
	
	if (address.sin_addr.s_addr == INADDR_NONE)
	{
		close(socketFd);
		throw std::runtime_error("invalid host");
	}

	if (bind(socketFd, (sockaddr *)&address, sizeof(address)) == -1)
	{
		close(socketFd);
		throw std::runtime_error("bind() failed");
	}
	
}

void	ServerManager::listenSocket(int socketFd)
{
	if (listen(socketFd, LISTEN_BACKLOG) == -1)
	{
		close(socketFd);
		throw std::runtime_error("listen() failed");
	}
}


void	ServerManager::initPollFds()
{
	for (size_t i = 0; i < _listenSockets.size(); i++)
	{
		struct pollfd	pollFd;

		pollFd.fd = _listenSockets[i];
		pollFd.events = POLLIN;
		pollFd.revents = 0;
		
		_pollFds.push_back(pollFd);
	}
}


void	ServerManager::printSockets() const
{
	for (size_t i = 0; i < _listenSockets.size(); i++)
		std::cout << "Listening socket: " << _listenSockets[i] << std::endl;
}


void	ServerManager::run()
{
	if (_listenSockets.empty())
		throw std::runtime_error("No sockets to poll");
	
	while (true)
	{
		int	ret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ret == -1)
			throw std::runtime_error("poll() failed");
		
		std::cout << "Activity detected!" << std::endl;
		
		for (int i = 0; i < (int)_pollFds.size(); i++)
		{
			std::cout << "fd: " << _pollFds[i].fd << " revents: " << _pollFds[i].revents << std::endl;
			if (_pollFds[i].revents & POLLIN)
			{
				std::cout << "POLLIN on fd: " << _pollFds[i].fd << std::endl;
				
				// only listening general sockets can accept new connections
				if (std::find(_listenSockets.begin(), _listenSockets.end(), _pollFds[i].fd) != _listenSockets.end())
				{
					std::cout << "Client activity on listening socket fd: " << _pollFds[i].fd << std::endl;
					acceptClient(_pollFds[i].fd);
				}
				else // listening specific sockets receive data
				{
					std::cout << "Client activity on fd: " << _pollFds[i].fd << std::endl;
					if (readClient(i))
						i--;
				
					
				}
			}
			else if (_pollFds[i].revents & POLLHUP) // closing socket all information transmited
			{
				std::cout << "POLLHUP on fd: " << _pollFds[i].fd << std::endl;
				close(_pollFds[i].fd);
				_pollFds.erase(_pollFds.begin() + i);
				i--;
				continue ;	
			}
			else if (_pollFds[i].revents & POLLNVAL)
			{
				std::cout << "POLLNVAL on fd: " << _pollFds[i].fd << std::endl;
				close(_pollFds[i].fd);
				_pollFds.erase(_pollFds.begin() + i);
				i--;
				continue;
			}
			
			// temporal debug
			if (_pollFds[i].revents & POLLOUT)
			{
			sendResponse(i);
			i--;
			continue ;
			}
			
		}
	}
}

bool	ServerManager::isListenSocket(int fd) const
{
	for (size_t i = 0; i < _listenSockets.size(); i++)
	{
		if (_listenSockets[i] == fd)
			return true;
	}

	return false;
}

void	ServerManager::acceptClient(int socketFd)
{
	sockaddr_in	clientAddress;
	struct pollfd	client;
	
	socklen_t clientLen = sizeof(clientAddress);
	int clientFd = accept(socketFd, (sockaddr *)&clientAddress, &clientLen);

	if (clientFd == -1)
		throw std::runtime_error("accept() failed");
		
	//accept crea un nuevo socker que tienen que ser no bloquante
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(clientFd);
    		throw std::runtime_error("fcntl(F_SETFL) failed");
    	}
    	
	client.fd = clientFd;
	client.events = POLLIN;
	client.revents = 0;
	
	_pollFds.push_back(client);

	std::cout << "New client connected. fd = " << clientFd << std::endl;

}


bool	ServerManager::readClient(int indexPoll)
{
	int clientFd = _pollFds[indexPoll].fd;
	
	char buffer[4096];

	int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

	if (bytes == -1)
	{
		close(clientFd);
		_pollFds.erase(_pollFds.begin() + indexPoll);
		return false;
	}
	if (bytes == 0)
	{
		std::cout << "Client disconnected fd = " << clientFd << std::endl;
		close(clientFd);
		_pollFds.erase(_pollFds.begin() + indexPoll);
		return true;
	}

	buffer[bytes] = '\0';

	std::cout << "Request received:" << std::endl;
	std::cout << buffer << std::endl;
	
	_pollFds[indexPoll].events = POLLOUT;// temporal

	
	return false;
}


//temporal debug
void ServerManager::sendResponse(int index)
{
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "<b>Hello</b>";

    int fd = _pollFds[index].fd;

    send(fd, response.c_str(), response.size(), 0);
    close(fd);
    _pollFds.erase(_pollFds.begin() + index);

    _pollFds[index].events = POLLIN;
}






