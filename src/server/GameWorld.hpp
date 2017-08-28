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
#ifndef GAME_WORLD_HPP_INCLUDED
#define GAME_WORLD_HPP_INCLUDED

#include <list>
#include <vector>
#include "boost/shared_ptr.hpp"

#include "mmolib/MMOUtils.hpp"
#include "mmolib/MMOTypes.hpp"

#include "GameClient.hpp"

/**
 * A list of client_ptrs
 */
typedef std::list<ClientPtr> ClientList;

/**
 * Represents a Game World which has a map and objects
 */
class GameWorld {

public:
	GameWorld();
	~GameWorld();

	/**
	 * Generates a new world data
	 */
	void generate();

	/**
	 * Processes each child client and checks for events
	 *
	 * @return Returns a vector of disconnected clients
	 */
	std::vector<ClientPtr> processClients();

	/**
	 * Adds a new client to the world
	 */
	void addClient( ClientPtr& client );

	/**
	 * Removes an existing client from the world
	 */
	void removeClient( ClientPtr& client );

	/**
	 * Returns a non-modifyable list of current child clients
	 */
	const ClientList& getClients() const { return _clients; }

	/**
	 * Sends the specified message to all current connected clients, 
	 * except the one specified (if any)
	 *
	 * @param msg The message pointer to send
	 * @param except Send the message to all clients except this one
	 */
	void broadcastMessage(const TCPMessagePtr& msg, const GameClient* except = NULL) const;

	/**
	 * Sends the specified message to all current connected clients, 
	 * except the one specified (if any)
	 *
	 * @param msg The message to send
	 * @param except Send the message to all clients except this one
	 */
	void broadcastMessage(const TCPMessage& msg, const GameClient* except = NULL) const;

	/**
	 * Returns true or false if the specified client can move to the 
	 * specified position. Optionally it does the move if possible
	 *
	 * @param x The x map coordinate
	 * @param y The y map coordinate
	 * @param client The client to check the move for
	 * @param doMove If true and the client can move, it will move it.
	 */
	bool canMoveTo( int x, int y, const GameClient& client, bool doMove = false );
private:
	DISALLOW_COPY_AND_ASSIGN(GameWorld);

	Matrix<char> _mapData;

	ClientList _clients;
};
#endif