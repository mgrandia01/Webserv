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
#include "Config.hpp"


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

	const Config&		_config;
	std::vector<int>	_listenSockets;

	void	createSockets();
	void	bindSocket(int socketFd, const ServerConfig& server);
	void	listenSocket(int socketFd);
	

		
};

#endif
