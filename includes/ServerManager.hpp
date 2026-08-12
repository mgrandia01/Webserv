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
#include "http/RequestParser.hpp"
#include "Client.hpp"
#include "http/HttpHandler.hpp"
#include "Response.hpp"


class ServerManager {

public:

	ServerManager(const Config& config);
	~ServerManager();

	void	init();
	void	printSockets() const;
	void	run();
	



	const ServerConfig* getServerConfigFromSocket(int fd) const;

	

private:

	ServerManager();
	ServerManager(const ServerManager& other);
	ServerManager& operator=(const ServerManager& rhs);

	const Config&				_config;
	
	std::vector<int>					_listenSockets;
	std::vector<const ServerConfig*> 	_listenConfigs;
	std::vector<struct pollfd>			_pollFds;

	


	HttpHandler					_requestHandler;
	std::map<int, Client> _clients;
	
	
	void	createSockets();
	void	bindSocket(int socketFd, const ServerConfig& server);
	void	listenSocket(int socketFd);
	void	initPollFds();
	
	void	acceptClient(int socketFd);
	bool	readClient(int indexPoll);
	bool	sendResponse(int index);

	void	checkTimeouts();
	
};

#endif
