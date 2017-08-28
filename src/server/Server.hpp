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
#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <list>
#include <boost/shared_ptr.hpp>
#include "SDL_net.h"

#include "mmolib/Config.hpp"
#include "mmolib/Database.hpp"

#include "GameClient.hpp"
#include "GameWorld.hpp"

#define SERVER_MAX_CLIENTS      100

/** 
 * This class represents the server. 
 * It runs and manages all connections and logic
 * between different clients.
 */
class Server {

public:
    /**
     * Returns this server's instance
     */
    static Server* getInstance();

    /**
     * Runs the server loop. It returns 0 if the server
     * has finished successfully, or error code if there
     * was an error
     * 
     * @return  server exit value. 0 means ok, while 
     *          other value means error
     */
    int runServer();

    /**
     * Returns true if the server has finished initializing and 
     * is waiting and/or processing clients or false otherwise
     * 
     * @return The running state
     */
    bool isRunning() const { return _isRunning; }

    ~Server();
private:
	
	void checkClientsPending();
	void configure();

    static Server* _instance;
    Server();
	DISALLOW_COPY_AND_ASSIGN(Server);

    bool _isRunning;

    SDLNet_SocketSet _socketSet;
	TCPsocket _serverSocket;
    Config _configFile;

	GameWorld _world;
};
#endif 