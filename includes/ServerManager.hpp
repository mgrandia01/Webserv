/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:47:52 by arcmarti          #+#    #+#             */
/*   Updated: 2026/09/30 16:37:16 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include <csignal>
#include "Config.hpp"
#include "http/RequestParser.hpp"
#include "Client.hpp"
#include "http/HttpHandler.hpp"
#include "Response.hpp"

#include "CgiExecve.hpp"


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
	
	static volatile sig_atomic_t	_running;
	
	void	createSockets();
	void	bindSocket(int socketFd, const ServerConfig& server);
	void	listenSocket(int socketFd);
	void	initPollFds();
	
	void	acceptClient(int socketFd);
	bool	readClient(int indexPoll);
	bool	sendResponse(int index);

	void	checkTimeouts();

	static void signalHandler(int signal);

	

	// CGI activos, indexados por client fd. ServerManager es el owner de los CGI.
    std::map<int, CgiExecve*>                 _cgis;
    // Permite encontrar el CGI a partir de cualquiera de sus dos FDs registrados en poll().
    std::map<int, CgiExecve*>                 _cgiFds;
	void registerCgi(CgiExecve* cgi);
	void removeCgiFd(int fd, CgiExecve* cgi);
	//bool isCgiFd(int fd) const;
	bool handleCgiEvent(int indexPoll);
	void finishCgi(CgiExecve* cgi);
	void timeoutCgi(CgiExecve* cgi);

	
};

#endif
