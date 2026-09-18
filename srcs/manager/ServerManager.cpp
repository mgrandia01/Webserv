/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:50:07 by arcmarti          #+#    #+#             */
/*   Updated: 2026/08/24 11:21:55 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "http/HttpStatus.hpp"

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
#include <csignal>
#include <cerrno>

#include "CGI.hpp"


static const int LISTEN_BACKLOG = 128;
static const int POLL_TIMEOUT = 1000;

#define CGI_TIMEOUT 10 // TO DO recofer fel configgggggggggggggggggggg TEMP

volatile sig_atomic_t ServerManager::_running = 1;


ServerManager::ServerManager(const Config& config) : _config(config) {}

ServerManager::~ServerManager()
{
	
	//cgis

	for (std::map<int, CGI*>::iterator it = _cgis.begin();
         it != _cgis.end(); ++it)
    {
        delete it->second;
    }

    _cgis.clear();
    _cgiFds.clear();

	

	// close sockets only if init() has been succesfully
	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd != -1)
			close(_pollFds[i].fd);
	}

	_listenSockets.clear();
	_listenConfigs.clear();
	_pollFds.clear();
}

void	ServerManager::init()
{
	if (signal(SIGINT, ServerManager::signalHandler) == SIG_ERR)
		throw std::runtime_error("Failed to register SIGINT handler.");

	if (signal(SIGTERM, ServerManager::signalHandler) == SIG_ERR)
		throw std::runtime_error("Failed to register SIGTERM handler.");

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		throw std::runtime_error("Failed to register SIGPIPE handler.");


	createSockets();
	initPollFds();
	//std::cout << "Server listening..." << std::endl;
}

void	ServerManager::createSockets()
{
	const std::vector<ServerConfig>& servers = _config.getServers();

	for (size_t i = 0; i < servers.size(); i++)
	{
		int socketFd = -1;
		try
		{
			// socket creation	
			socketFd = socket(AF_INET, SOCK_STREAM, 0);
			if (socketFd == -1)
				throw std::runtime_error("socket() failed");

			// socket reuse on bindind process
			int opt = 1;
			if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		 	{
		 		
		 		throw std::runtime_error("setsockopt() failed");
		 	}
		
			// non-blocking socket
			if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
			{
				
				throw std::runtime_error("fcntl(F_SETFL) failed");
			}
			
			bindSocket(socketFd, servers[i]);
			listenSocket(socketFd);
			_listenSockets.push_back(socketFd);
			_listenConfigs.push_back(&servers[i]);

		} catch(std::exception& e)
		{
			if (socketFd != -1)
				close(socketFd);
			for (size_t i = 0; i < _listenSockets.size(); ++i)
			{
            	if (_listenSockets[i] != -1)
                	close(_listenSockets[i]);
            }

            throw std::runtime_error(std::string("Initial configuration failed: ") + e.what() + ". Webserver aborted.");
        }
		
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
		
		throw std::runtime_error("invalid host");
	}

	std::cout << server.getPort() << "..."  << server.getHost() << "..." << socketFd << std::endl;
	if (bind(socketFd, (sockaddr *)&address, sizeof(address)) == -1)
	{
		
		throw std::runtime_error("bind() failed");
	}
	
}

void	ServerManager::listenSocket(int socketFd)
{
	if (listen(socketFd, LISTEN_BACKLOG) == -1)
	{
		
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
	if (_pollFds.empty())
		throw std::runtime_error("No sockets to poll");
	
	while (_running) // running while not SIGINT or SIGTERM received
	{
		// POLL_TIMEOUT, maximum timoeut until at least one fd has an event
		int	ret = poll(&_pollFds[0], _pollFds.size(), POLL_TIMEOUT);
		
		if (ret == -1)
		{
			if (errno == EINTR) // SIGINT exits the program, not poll fails
				continue;
			else
				throw std::runtime_error("poll() failed");
		}
		
		if (ret == 0)
		{
			// check all possible timeouts before executing main loop
			checkTimeouts();
			continue ;
		}

		std::cout << "Activity detected! Ret del poll= " << ret << std::endl;
		
		for (int i = 0; i < (int)_pollFds.size(); i++)
		{
			int fd = _pollFds[i].fd;

			if (_cgiFds.find(fd) != _cgiFds.end())
			{
        		if (handleCgiEvent(i))
        		{
            		i--;
            		continue;
            	}
            	continue;
            }
            
			if (_pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) // closed socket (all information could no be fully transmited) or error
			{
				int fd = _pollFds[i].fd;

				bool isListeningSocket = std::find(_listenSockets.begin(), _listenSockets.end(), fd) != _listenSockets.end();
				
				if (isListeningSocket)
				{
					std::cout << "Socket error/hangup on SERVER listening socket with fd: " << _pollFds[i].fd << std::endl;

					close(fd);
					for (size_t j = 0; j < _listenSockets.size(); ++j)
    				{
        				if (_listenSockets[j] == fd)
        				{
            				_listenSockets.erase(_listenSockets.begin() + j);
            				_listenConfigs.erase(_listenConfigs.begin() + j);
            				break ;
            			}
            		}
            		_pollFds.erase(_pollFds.begin() + i);
            		i--;


				    if (_listenSockets.empty())
				    {
				        std::cout << "No listening sockets available" << std::endl;
				        _running = 0;
				        break ;
				    }
				    continue ;
				}
				else
				{
					std::cout << "Socker error/hangup on CLIENT data socket with fd: " << _pollFds[i].fd << std::endl;

					close(fd);
					_pollFds.erase(_pollFds.begin() + i);
					_clients.erase(fd);
					i--;
					continue ;	
				}
				
			} 
			else if (_pollFds[i].revents & POLLIN) // event POLLIN happened because there are data to be read
			{
				std::cout << "POLLIN on fd: " << _pollFds[i].fd << std::endl;
				
				int fd = _pollFds[i].fd;
				
				// type 1: LISTENING sockets : POLLIN means there are new connections waiting to be accepted
				if (std::find(_listenSockets.begin(), _listenSockets.end(), fd) != _listenSockets.end())
				{
					std::cout << "Client requesting connection on SERVER listener socket fd: " << _pollFds[i].fd << std::endl;
					acceptClient(fd);
				}
				else // type 2: data sockets : event POLLIN indicates sockets have received data and are ready to be read
				{
					std::cout << "Client receiving data on CLIENT socket fd: " << _pollFds[i].fd << std::endl;
					if (readClient(i))
					{
						i--;
						continue ;
					}
				}
			} else if (_pollFds[i].revents & POLLOUT)  // temporal debug
			{
				if (sendResponse(i))
				{
					i--;
					continue ;
				}
			}
			
		}
	}
}


void ServerManager::checkTimeouts()
{
    time_t now = time(NULL);

    // CGI timeouts


	for (std::map<int, CGI*>::iterator it = _cgis.begin(); it != _cgis.end(); )
	{
	    CGI* cgi = it->second;
		
		std::map<int, Client>::iterator clientIt = _clients.find(cgi->getClientFd());
		Client& client = clientIt->second;
		const ServerConfig* server = client.getServerConfig();

	    if (now - cgi->getStartTime() > server->getCgiTimeout())
		{
		    timeoutCgi(cgi);
		    delete cgi;
		    _cgis.erase(it++);
		}
	    else
	    {
	        ++it;
	    }
	}


    std::map<int, Client>::iterator it = _clients.begin();

    // only check timeouts if there are Clients connected
    // si virtual servers, coger el default server en el primer acceso
    // y luego los valores ya seran los correctos
    // esto es indep de los timeouts globales que se usan al inicio de Config
    while (it != _clients.end())
    {
        Client& client = it->second;
        const ServerConfig* server = client.getServerConfig();

        if (server)
        {
            int timeout = 0;

            switch (client.getTimeoutState())
			{
			    case WAITING_REQUEST:
			        timeout = server->getClientHeaderTimeout(); //TO DO en el server cambiar el nombre a Request en vez de Header
			        break;

			    case SENDING_RESPONSE:
			        timeout = server->getSendTimeout();
			        break;

			    case KEEP_ALIVE:
			        timeout = server->getKeepAliveTimeout();
			        break;

			    default:
			    	++it;
			    	continue ;

			}

			if ((timeout > 0) && (now - client.getLastActivity() > timeout))
            {
                int fd = client.getFd();

                if (client.getTimeoutState() == WAITING_REQUEST)
                {
    				std::cout << "CLIENT data timeout WAITING REQUEST on fd: " << fd << std::endl;
                
                 	// TO DO este es un ejemplo de integracion de errores
                 	// queda pendiente ampliar a todos los errores en el siguiente pullrequest
                 	Response response(Response::createError(REQUEST_TIMEOUT, *server));
                 	//Response response("HTTP/1.1 408 Request Timeout\r\n Content-Length: 19\r\n\r\n");
                 	client.setResponse(response);
                	client.setKeepAlive(false);


                	client.setLastActivity();

	                for (size_t i = 0; i < _pollFds.size(); i++)
	                {
	                	if (_pollFds[i].fd == fd)
	                    {
	                       _pollFds[i].events = POLLOUT;
	            			break ;
	                    }
	                }
                	++it;
                }
                else //SENDING RESPONS OR KEEP ALIVE
                {
               		
					std::cout << "CLIENT data timeout SENDING RESPONSE or KEEP ALVE on fd: " << fd << std::endl;

			        std::map<int, Client>::iterator current = it;
			        ++it;

			        close(fd);
			        _clients.erase(current);

			        for (size_t i = 0; i < _pollFds.size(); ++i)
			        {
			            if (_pollFds[i].fd == fd)
			            {
			                _pollFds.erase(_pollFds.begin() + i);
			                break;
			            }
			        }
                }
              
            }
            else
                ++it;
        }
        else
            ++it;
    }
}


void	ServerManager::acceptClient(int socketFd)
{
	sockaddr_in		clientAddress;
	struct pollfd	p_client;
	
	
	socklen_t clientLen = sizeof(clientAddress);
	int clientFd = accept(socketFd, (sockaddr *)&clientAddress, &clientLen);

	if (clientFd == -1)
	{
		std::cout << "accept() failed on SEVER listener fd: " << socketFd << std::endl;

		return  ;
	}

	// new socket needs to be non-blocking
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
    	std::cout << "fcntl(F_SETFL) failed for client fd: " << clientFd << std::endl;
    	close(clientFd);

    	return ;
	}
	
	const ServerConfig* serverConfig = getServerConfigFromSocket(socketFd);
	if (serverConfig)
	{
		p_client.fd = clientFd;
		p_client.events = POLLIN;
		p_client.revents = 0;
		_pollFds.push_back(p_client);
		Client client(clientFd);
		client.setServerConfig(serverConfig);
				
		std::cout << "Assigned server: ";
		if (serverConfig->getServerName().empty())
		    std::cout << "(empty)";
		else
		    std::cout << serverConfig->getServerName()[0];
		std::cout << std::endl;
		
		client.setTimeoutState(WAITING_REQUEST);
		_clients.insert(std::make_pair(clientFd, client));
	}
	else
	{
		std::cout << "ERROR: no ServerConfig assigned" << std::endl;
		close(clientFd);
		return ;
	}
	
	
	std::cout << "New CLIENT socket data connected with fd = " << clientFd << std::endl;
	
	
}


bool ServerManager::readClient(int indexPoll)
{
    int clientFd = _pollFds[indexPoll].fd;

    std::map<int, Client>::iterator it = _clients.find(clientFd);

    if (it == _clients.end())
        return false;

    Client& client = it->second;

    if (!client.receive())
    {
        std::cout << "CLIENT data disconnected fd = " << clientFd << std::endl;

        close(clientFd);
        _clients.erase(clientFd);
        _pollFds.erase(_pollFds.begin() + indexPoll);

        return true;
    }

    if (client.hasParserError())
    {
        //Response response("HTTP/1.1 400 Bad Request\r\n Content-Length: 0\r\n\r\n");
        Response response(Response::createError(client.getParser().getErrorCode(), *client.getServerConfig()));
        client.setResponse(response);
    }
    else if (client.isRequestComplete())
    {
        // Parser of request is done because request is Complete ???????

        const HttpRequest& request = client.getParser().getRequest();
        const ServerConfig* server = client.getServerConfig();

        client.setKeepAlive(request.isKeepAlive); // TO DO habra que sacarlo de httpRequest quee habra parseado el header

        if (!server)
        {
            Response response("HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
            client.setResponse(response);
        }
        else
        {
            
	    	Response response = _requestHandler.handle(request, *server);
	    	
	    	//TEMP lineas de pruebas temporal para test CGI
	    	// simulo POST enviando body
	    	CGI* cgia = new CGI(clientFd, "Hola desde CGI\n");
			// o simulo GET enviando nada
			//CGI* cgia = new CGI(clientFd, "");
			response.setCgi(cgia);
			// TEMP fin de zona temporal para test CGI

	    	client.setResponse(response);

            CGI* cgi = client.getResponse().getCgi();
            if (cgi)
            {
               	_pollFds[indexPoll].events = 0;
            	registerCgi(cgi);
            }

        }
    }

    if (client.hasResponse() && client.getResponse().getCgi() == NULL)
    {
    	_pollFds[indexPoll].events = POLLOUT;
        std::cout << "Setting POLLOUT for fd " << clientFd << std::endl;	
    }
    
        
    

    return false;
}



bool ServerManager::sendResponse(int index)
{
    int fd = _pollFds[index].fd;

    std::map<int, Client>::iterator it = _clients.find(fd);
	
	if (it == _clients.end())
    	return false;
	
	Client& client = it->second;

    const std::string& data = client.getResponse().getStream();


    //temporal, no hay que asumir que el send envia todo
    //send(fd, data.c_str(), data.size(), 0);

    ssize_t bytesSent = send(fd, data.c_str() + client.getBytesSent(), data.size() - client.getBytesSent(), 0);

    std::cout << "-------- Data.c_str = " << data.c_str() << std::endl;
    std::cout << "-------- Data.size = " << data.size() << std::endl;
    if (bytesSent == -1)
    {
    	std::cout << "CLIENT data send() failed on fd: " << fd << std::endl;
    	close(fd);
    	_clients.erase(fd);
    	_pollFds.erase(_pollFds.begin() + index);
    	
    	return true;
	} else if (bytesSent == 0)
	{
		std::cout << "CLIENT data send() returned 0 on fd: " << fd << std::endl;
		close(fd);
		_clients.erase(fd);
		_pollFds.erase(_pollFds.begin() + index);
		
		return true;
	} else // bytesSent >0
	{
			client.addBytesSent(bytesSent);
			client.setLastActivity();
			client.setTimeoutState(SENDING_RESPONSE);
	}

	if (client.getBytesSent() >= data.size())
	{
    	// respuesta completamente enviada
    	if (client.getKeepAlive())
    	{
	        client.getParser().reset();
        	client.clearResponse();
        	client.setTimeoutState(KEEP_ALIVE);
        	client.setLastActivity();
    	    _pollFds[index].events = POLLIN;
    	    std::cout << "CLIENT has sent full data on fd: " << fd << " and fd continues opened" << std::endl;
    	    return false;
	    }
    	else
    	{
	        close(fd);
    	    _clients.erase(fd);
        	_pollFds.erase(_pollFds.begin() + index);
        	std::cout << "CLIENT has sent full data on fd: " << fd << " and fd is closed" << std::endl;
        	return true;
    	}
	}
	else
	{
		// todavía queda respuesta por enviar
    	_pollFds[index].events = POLLOUT;
    	return false;
	}
    
	//return false;
    
}


const ServerConfig* ServerManager::getServerConfigFromSocket(int fd) const
{
    for (size_t i = 0; i < _listenSockets.size(); i++)
    {
        if (_listenSockets[i] == fd)
            return _listenConfigs[i];
    }

    return NULL;
}

void	ServerManager::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		std::cout << "\nSIGINT received. Initiating server shutdown..." << '\n';
        _running = 0;
	}
}


//CGIs
void ServerManager::registerCgi(CGI* cgi)
{
    if (!cgi)
        return;

    int stdinFd = cgi->getStdinFd();
    int stdoutFd = cgi->getStdoutFd();
    int clientFd = cgi->getClientFd();

    // ServerManager registra el CGI del Response mientras esta activo
    _cgis[clientFd] = cgi;

    struct pollfd pollFd;

    // CGI stdin: ServerManager -> pipe -> CGI
    pollFd.fd = stdinFd;
    pollFd.events = POLLOUT;
    pollFd.revents = 0;
    
    _pollFds.push_back(pollFd);
    _cgiFds[stdinFd] = cgi;

    // CGI stdout:
    // CGI -> pipe -> ServerManager
    
    pollFd.fd = stdoutFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;
    
    _pollFds.push_back(pollFd);
     _cgiFds[stdoutFd] = cgi;

    std::cout << "CGI registered:" << " clientFd=" << clientFd << " stdinFd=" << stdinFd << " stdoutFd=" << stdoutFd << std::endl;
}


bool ServerManager::handleCgiEvent(int indexPoll)
{
    int fd = _pollFds[indexPoll].fd;

    std::map<int, CGI*>::iterator it = _cgiFds.find(fd);

    if (it == _cgiFds.end())
        return false;

    CGI* cgi = it->second;
    short revents = _pollFds[indexPoll].revents;

    // CGI STDIN ServerManager -> pipe -> CGI
	 
	if (fd == cgi->getStdinFd())
	{
		// Si el otro extremo ha cerrado el pipe, ya no podemos escribir. Eliminamos este FD.
		
		if (revents & (POLLHUP | POLLERR | POLLNVAL))
		{
			removeCgiFd(fd, cgi);
			return true;
		}

		if (revents & POLLOUT)
		{
			const std::string& input = cgi->getInput();
			size_t offset = cgi->getBytesWritten();

			if (offset < input.size())
			{
				ssize_t bytes = write(cgi->getStdinFd(), input.c_str() + offset, input.size() - offset);

				if (bytes > 0)
				{
					cgi->addBytesWritten(static_cast<size_t>(bytes));

					std::cout << "Wrote " << bytes << " bytes to CGI fd " << fd << std::endl;
				}
				else if (bytes == -1)
				{
					// NO permitido comprobar errno
					std::cout << "write() failed on CGI fd " << fd << std::endl;

					removeCgiFd(fd, cgi);
					return true;
				}
			}

			// Hemos enviado todo el body. Cerramos stdin para enviar EOF al CGI.
			
			if (cgi->getBytesWritten() == input.size())
			{
				std::cout << "CGI input completely sent on fd " << fd << std::endl;

				removeCgiFd(fd, cgi);
				return true;
			}
		}

		return false;
	}

	// CGI STDOUT CGI -> pipe -> ServerManager
	 
	if (fd == cgi->getStdoutFd())
	{
		// POLLHUP puede aparecer junto con datos todavía pendientes en el pipe.
		// Por eso intentamos leer tanto con POLLIN como con POLLHUP.
		 
		if (revents & (POLLIN | POLLHUP))
		{
			char buffer[4096];

			ssize_t bytes = read(fd, buffer, sizeof(buffer));

			if (bytes > 0)
			{
				cgi->feed(buffer, bytes);

				std::cout << "Read " << bytes << " bytes from CGI fd " << fd << std::endl;

				return false;
			}

			if (bytes == 0)
			{
				// EOF no hay mas salida real del CGI.
				std::cout << "CGI EOF on fd " << fd << std::endl;

				
				removeCgiFd(fd, cgi);
				cgi->collectProcess();
				finishCgi(cgi);
				//asumimos que la respoisne esta preparada... veerificar esto

				return true;
			}

			// read() ha fallado, pero no podemos comprobar errono usamos errno
			
			std::cout << "read() failed on CGI fd " << fd << std::endl;
			removeCgiFd(fd, cgi);
			return true;
		}

		if (revents & (POLLERR | POLLNVAL))
		{
			std::cout << "Error on CGI stdout fd " << fd << std::endl;

			removeCgiFd(fd, cgi);
			return true;
		}
	}

    /*if (_pollFds[indexPoll].revents & POLLOUT)
    {
        // WRITE to CGI in case of POST (body write)
        const std::string& input = cgi->getInput();
        size_t offset = cgi->getBytesWritten();
        if (offset < input.size())
    	{
        	ssize_t bytes = write(cgi->getStdinFd(), input.c_str() + offset, input.size() - offset);
        	if (bytes > 0)
        	{
        		cgi->addBytesWritten(static_cast<size_t>bytes);
        		std::cout << "Wrote " << bytes << " bytes to CGI fd " << fd  << std::endl;
        	}
        	else if (bytes == -1)
        	{
            	if (errno != EAGAIN && errno != EWOULDBLOCK) // no permitido en 42
            		std::cout << "write() failed on CGI fd " << fd << std::endl;
            	return true;
            }
        }
        if (cgi->getBytesWritten() == input.size())
        {
    		 std::cout << "CGI input completely sent on fd " << fd << std::endl;
    		
    		int stdinFd = cgi->getStdinFd();
    		close(stdinFd); // cloe pipe write
			_cgiFds.erase(stdinFd);
    		_pollFds.erase(_pollFds.begin() + indexPoll);

    		return true;
    	}


    }

    if (_pollFds[indexPoll].revents & POLLIN)
    {
        // READ from CGI

        char buffer[4096];
        ssize_t bytes = read(fd, buffer, sizeof(buffer));
        if (bytes > 0)
        {
			cgi->feed(buffer, bytes);
			std::cout << "Read " << bytes << " bytes from CGI fd " << fd << std::endl;
			return false;
        }
		else if (bytes == 0)
		{
			cgi->setStdoutClosed(true);
			_cgiFds.erase(fd);
            close(fd);
            _pollFds.erase(_pollFds.begin() + indexPoll);
             if (cgi->isFinished())
    			 finishCgi(cgi);
    		
			//consultar el output del cgi
			//waitpid ???? podria bloquear
			//cosnturir response final (cidado pq ya tendria que venir rellenada)
			std::cout << "CGI EOF on fd " << fd << std::endl;
			return true;
		}
		else
		{
    		if (errno != EAGAIN && errno != EWOULDBLOCK) //  NO permitido segun subject en 42!!!!
    		{
        		// error
        		return true;
    		}
		}

    }*/



    return false;
}

void ServerManager::finishCgi(CGI* cgi)
{
    if (!cgi)
        return;

    int clientFd = cgi->getClientFd();

	std::map<int, Client>::iterator it = _clients.find(clientFd);

	if (it == _clients.end())
        return;
    
    Client& client = it->second;
    Response& response = client.getResponse();

    //response.body = cgi->getOutput(); No se puede anyadir directamente al body pq hay headers
    //response.processCGIOutput(); metodo especial para procesar el output del CGI y reconstruir el Response
	// Aquí el CGI ya ha terminado y su output está completo asi que puede acabarse de rellenar el Response

	// El Response deja de referenciar al CGI
	response.clearCgi();

    
    // El Response ya debe estar rellenado aquí con el resultado del CGI

    
    client.setTimeoutState(SENDING_RESPONSE);
    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (_pollFds[i].fd == clientFd)
        {
            _pollFds[i].events = POLLOUT;
            break;
        }
    }


    _cgis.erase(clientFd);

    // El CGI lo borrara el ServerManager
    delete cgi;

 }
/*
void ServerManager::removeCgiFd(int fd)
{
	std::cout << "removeCgiFd(" << fd << ")" << std::endl; //temp

	_cgiFds.erase(fd);

	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == fd)
		{
			std::cout << "REMOVING FROM POLL fd=" << fd << std::endl;
			close(fd);
			_pollFds.erase(_pollFds.begin() + i);
			return;
		}
	}
}

*/
void ServerManager::removeCgiFd(int fd, CGI* cgi)
{
    
    std::map<int, CGI*>::iterator it = _cgiFds.find(fd);

    if (it == _cgiFds.end())
    	return;
    

    // FD is reused and is not the initial one belonging to CGI.
    if (it->second != cgi)
    	return;
    
    _cgiFds.erase(it);

    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (_pollFds[i].fd == fd)
        {
            if (fd == cgi->getStdinFd())
            	cgi->closeStdinFd();
        	else if (fd == cgi->getStdoutFd())
            	cgi->closeStdoutFd();
            _pollFds.erase(_pollFds.begin() + i);
            return;
        }
    }
}


void ServerManager::timeoutCgi(CGI* cgi)
{
    if (!cgi)
        return;

    std::cout << "CGI timeout: clientFd=" << cgi->getClientFd() << std::endl;

    int clientFd = cgi->getClientFd();

     // Eliminar el CGI
    if (cgi->getPid() > 0)
        kill(cgi->getPid(), SIGKILL);

    removeCgiFd(cgi->getStdinFd(), cgi);
    removeCgiFd(cgi->getStdoutFd(), cgi);

    cgi->collectProcess();

    std::map<int, Client>::iterator it = _clients.find(clientFd);

    if (it == _clients.end())
        return;

    Client& client = it->second;
    const ServerConfig* server = client.getServerConfig();

    if (!server)
        return;

    // 504 Gateway Timeout
    Response response(Response::createError(GATEWAY_TIMEOUT, *server));
    client.setResponse(response);
    client.setKeepAlive(false);

    for (size_t i = 0; i < _pollFds.size(); ++i)
	{
	    if (_pollFds[i].fd == clientFd)
	    {
	        _pollFds[i].events = POLLOUT;
	        break;
	    }
	}

	Response& clientResponse = client.getResponse();
    clientResponse.clearCgi();

   
}