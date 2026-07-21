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
#include <fcntl.h>
#include <unistd.h> //sleep, close
#include <arpa/inet.h> //address.sin_addr.s_addr = inet_addr(server.host.c_str());

static const int LISTEN_BACKLOG = 0;


ServerManager::ServerManager(const Config& config) : _config(config) {}

ServerManager::~ServerManager()
{
	for (size_t i = 0; i < _listenSockets.size(); i++)
		close(_listenSockets[i]);
}

void	ServerManager::init()
{
	createSockets();
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
		
		// Hacer el socket no bloqueante
		if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl(F_SETFL) failed");
		
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

void	ServerManager::printSockets() const
{
	for (size_t i = 0; i < _listenSockets.size(); i++)
		std::cout << "Listening socket: " << _listenSockets[i] << std::endl;
}


void	ServerManager::run()
{
	if (_listenSockets.empty())
		throw std::runtime_error("No sockets to poll");
	
	while (true) { }
}






