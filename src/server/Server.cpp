/*
   Copyright (C) 2011 by Timotei Dolean <timotei21@gmail.com>

   Part of the Fire and Sword Project http://code.google.com/p/fire-and-sword/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#include "miniui\CUiManager.h"
#include "mmolib\Config.hpp"
#include "mmolib\Database.hpp"
#include "Server.hpp"
#include "ServerUtils.hpp"

#include <vector>
#include "SDL_Timer.h"

Server* Server::_instance = NULL;

Server::Server()
    : _configFile("server.cfg")
    , _isRunning(false)
{
	std::cout << "Starting Fire & Sword Server...\n";
    std::cout << "Initializing SDLNet... ";

    // initialize SDL
    if (SDLNet_Init() < 0){
		std::cerr << "Error initializing SDL" << SDLNet_GetError() << "\n";
        return;
    }
	std::cout << "Done\n";

	// test the UI
	std::cout << "Mini Ui version: " << MINIUI->getVersion() << " / " << MINIUI->getRevision() << "\n";

	_socketSet = SDLNet_AllocSocketSet(SERVER_MAX_CLIENTS);

	if (_socketSet == NULL){
		std::cerr << "Error creating socket set " << SDLNet_GetError() << "\n";
		return;
	}
}

int Server::runServer()
{
	if (_isRunning == true)
		return 0;

	_isRunning = true;

    // read config file
    std::cout << "Reading config file... ";
    if (_configFile.load() != 0)
        return 1;
    std::cout << "Done\n";

	// open database
	std::cout << "Opening database... ";
	if (Database::getInstance()->open("server_data\\main.db") != 0)
		return 1;
	std::cout << "Done\n";

    IPaddress* server_ip = new IPaddress;

    // load values from config
    const int port = _configFile.get_int("port", 1600);

    // resolve host
    std::cout << "Resolving host on port " << port << " ...";
    if (SDLNet_ResolveHost(server_ip, NULL, port) < 0){
        std::cerr << "Could not resolve host: " << SDLNet_GetError() << "\n";
		delete server_ip;
        return -1;
    }
    std::cout << "Done\n";

	// open socket
    _serverSocket = SDLNet_TCP_Open(server_ip);
    if (_serverSocket == NULL){
        std::cerr << "Error trying to open socket: " << SDLNet_GetError() << "\n";
		delete server_ip;
        return -1;
    }

	// add the server itself to socket set
	SDLNet_TCP_AddSocket(_socketSet, _serverSocket);

	configure();

	int socks_ready_cnt = -1;
    std::cout << "Server is up and running...\n";

    while(_isRunning){

		socks_ready_cnt = SDLNet_CheckSockets(_socketSet, 0);

		if (socks_ready_cnt == -1){
			std::cerr << "Error on checking sockets: " << SDLNet_GetError() << "\n";
			return -1;
		}

		// no activity
		if (socks_ready_cnt == 0){
			SDL_Delay(1);
			continue;
		}

		checkClientsPending();
		
		std::vector<ClientPtr> toRemove = _world.processClients();

		if (toRemove.empty() == false){
			for(std::vector<ClientPtr>::const_iterator itor = toRemove.begin()
				; itor != toRemove.end()
				; ++itor){

					// disconnect client from socket set
					SDLNet_TCP_DelSocket(_socketSet, (*itor)->getSocket());
			}

			toRemove.clear();
		}

        SDL_Delay(1);
    }

	delete server_ip;
	// close the socket
	SDLNet_TCP_Close(_serverSocket);
	SDLNet_TCP_DelSocket(_socketSet, _serverSocket);
	_serverSocket = NULL;
	return 0;
}

Server::~Server()
{
	// close the socket
	SDLNet_TCP_Close(_serverSocket);
	SDLNet_TCP_DelSocket(_socketSet, _serverSocket);

	_serverSocket = NULL;
	SDLNet_FreeSocketSet(_socketSet);
}

void Server::checkClientsPending()
{
	// we can check for new clients
	if (SDLNet_SocketReady(_serverSocket)){

		// wait for a new client connection
		TCPsocket client_socket = SDLNet_TCP_Accept(_serverSocket);

		// got client
		if (client_socket != NULL){

			IPaddress* remote_ip = SDLNet_TCP_GetPeerAddress(client_socket);

			// add client
			ClientPtr client( new GameClient( _world, remote_ip, client_socket ) );
			_world.addClient(client);

			SDLNet_TCP_AddSocket(_socketSet, client_socket);

			if (remote_ip != NULL)
				std::cout << "Got client : "
				<< to_string_ip(remote_ip->host)
				<< ":" 
				<< SDLNet_Read16(&remote_ip->port) << "\n";
		}
	}
}

void Server::configure()
{
	// generate the world map
	_world.generate();
}

Server* Server::getInstance()
{
    if (_instance == NULL)
        _instance = new Server;
    return _instance;
}

int main(int , char * [])
{
    // load the server
    Server::getInstance();
    int result = Server::getInstance()->runServer();

    std::cout << "Server closed\n";
	system("pause");
    return result;
}
