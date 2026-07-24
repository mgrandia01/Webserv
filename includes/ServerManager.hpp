/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:47:52 by arcmarti          #+#    #+#             */
/*   Updated: 2026/07/18 09:47:54 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include "Config.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include "Client.hpp"



class ServerManager {

public:

	ServerManager(const Config& config);
	~ServerManager();

	void	init();
	void	run();
	void	printSockets() const;

private:

	ServerManager();
	ServerManager(const ServerManager& other);
	ServerManager& operator=(const ServerManager& rhs);

	const Config&				_config;
	std::vector<int>			_listenSockets;
	std::vector<struct pollfd>	_pollFds;

	void	createSockets();
	void	bindSocket(int socketFd, const ServerConfig& server);
	void	listenSocket(int socketFd);
	void	acceptClient(int socketFd);
	
	
	void	initPollFds();
	bool	isListenSocket(int fd) const;
	bool	readClient(int indexPoll);

	std::map<int, Client> _clients;
		
	//temporal debug
	//void sendResponse(int indexPoll);

	void sendResponse(int index);
	
};

#endif
