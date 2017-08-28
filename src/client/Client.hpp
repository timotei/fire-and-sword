/*
   Copyright (C) 2011 by Timotei Dolean <timotei21@gmail.com>
   Copyright (C) 2011 by Alexandru Pana <r.whitesoul@gmail.com>

   Part of the Fire and Sword Project http://code.google.com/p/fire-and-sword/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#include <map>
#include <string>

#include "mmolib/MMOTypes.hpp"
#include "miniui/CWidget.h"

#include "ClientUtils.hpp"
#include "Player.hpp"

typedef std::map< std::string, PlayerPtr > PlayersMap;

/**
 * Represents a client side's view client
 */
class Client : public miniui::CEventListener {

public:
	Client();
	~Client();

	/**
	 * Returns a singleton instance of the client
	 */
	static Client* getInstance();

	/**
	 * Runs the game loop
	 */
	void runGame();

	/**
	 * Triggered on mouse click
	 */
	void onMouseClick( miniui::CWidget* sender );

private:
	static Client* _instance;

	TCPMessagePtr checkServerResponse();
	int connectToServer();
	void doLogin();
	void inputThread();
	void sendMoveRequest( const int& x, const int& y );
	void sendQuit();
	void drawMap( SDL_Surface* surface );
	int setPlayerDetails( const char* data, const PlayerPtr& player );

	// general
	Config _config;
	bool _running;

	// map
	int _mapWidth;
	int _mapHeight;
	std::string _mapData;
	miniui::CImage _imgTrunk;
	miniui::CImage _imgBackground;

	PlayersMap _players;
	PlayerPtr _currentPlayer;

	// graphics part
	SDL_Surface* _screen;

	// net part
	bool _isConnectedToServer;
	TCPsocket _serverSocket;
	SDLNet_SocketSet _socketSet;
	IPaddress* _serverIp;
};