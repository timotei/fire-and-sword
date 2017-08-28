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
#ifndef GAMECLIENT_HPP_INCLUDED
#define GAMECLIENT_HPP_INCLUDED

#include "SDL_net.h"
#include "ServerUtils.hpp"
#include "GameObject.hpp"
#include "Character.hpp"

#include "miniui/UiTypes.h"
#include "mmolib/TCPMessage.hpp"

#include "boost/shared_ptr.hpp"
#include <string>

class GameClient;
class GameWorld;

/**
 * A shared_ptr to a GameClient class
 */
typedef boost::shared_ptr<GameClient> ClientPtr;

/**
 * This class represents a server side's view client
 */
class GameClient{

public:

	/**
	 * Creates a new Game Client with the specified details
	 *
	 * @param world The world where this client belogns to
	 * @param ip The IPAddress of the client
	 * @param socket The socket associated with the client
	 */
    GameClient( GameWorld& world, IPaddress* ip, TCPsocket& socket );
    ~GameClient();

	/**
	 * Processes this client and returns the process result
	 */
	TCPMessagePtr processClient();

	/**
	 * Gets this client's IPAddress
	 */
	const IPaddress* getIP() const { return _ip; };

	/**
	 * Gets this client's TCPSocket
	 */
	const TCPsocket& getSocket() const { return _socket; }
	
	/**
	 * Sends the specified TCP message to this client
	 *
	 * @param msg The message to send
	 */
	int sendTcpMessage( const TCPMessage& msg ) const;

	/**
	 * Tries to login this client
	 *
	 * @param username The username used to login
	 * @param password The password for the username
	 *
	 * @return a TCPMessage::message_type enumeration
	 */
	TCPMessage::message_type login( const std::string& username, const std::string& password );

	/**
	 * Selects a character as the active one
	 *
	 * @param char_name the name of the character to select
	 */
	TCPMessage::message_type selectChar( const std::string& char_name );

	/**
	 * Saves current's client state to the database
	 *
	 */
	void save();

	/**
	 * Returns the identifier of this GameClient
	 */
	int getId() const { return _id; }

	/** 
	 * Returns true if this client is Logged in. That is,
	 * it has selected a character successfully
	 */
	bool isLoggedIn() const { return _isLoggedIn; }

	/**
	 * Returns the username of this GameClient
	 */
	const std::string& getUsername() const { return _username; }

	/**
	 * Returns the current active Character
	 */
	const CharacterPtr& getCurrentCharacter() const { return _currentChar; }

private:
	static const char* QUERY_ACCOUNT_DETAILS_BY_USERNAME;
	static const char* QUERY_CHARACTER_DETAILS_BY_ACCNAME;
	static const char* QUERY_CHARACTER_LIST_BY_ACCID;

	DISALLOW_COPY_AND_ASSIGN(GameClient);

	IPaddress* _ip;
	TCPsocket _socket;

	bool _isLoggedIn;
	std::string _username;
	int _id;

	GameWorld* _world;
	CharacterPtr _currentChar;
};
#endif