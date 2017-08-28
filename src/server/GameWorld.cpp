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
#include "GameWorld.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Character.hpp"

GameWorld::GameWorld()
: _clients(0)
, _mapData(MAP_MAX_HEIGHT, MAP_MAX_WIDTH)
{
}

GameWorld::~GameWorld()
{

}

void GameWorld::generate()
{
	std::cout << "Generated map: \n";

	//TODO: make a real generation algorithm.
	std::ifstream fin("server_data/map.txt");

	for( int line = 0; line < MAP_MAX_HEIGHT; ++line ){
		for (int col = 0; col < MAP_MAX_WIDTH; ++ col){
			do{
				fin.get(_mapData(col, line));
			}while(fin.eof() == false && (_mapData(col ,line) == '\r' || _mapData(col, line) == '\n'));

			std::cout << _mapData(col, line);
		}

		std::cout << "\n";
	}

	fin.close();
}

std::vector<ClientPtr> GameWorld::processClients()
{
	std::vector<ClientPtr> toRemove(0);
	// process clients
	for(ClientList::const_iterator itor = _clients.begin(); itor != _clients.end(); ){
		ClientPtr client = *itor;
		TCPMessagePtr result = client->processClient();

		if (result->type == TCPMessage::MSG_QUIT || 
			result->type == TCPMessage::MSG_DISCONNECT) {
				std::cout << "Client " << client->getId() << " quit "
					<< ( result->type == TCPMessage::MSG_DISCONNECT ? "abnormally" : "" )
					<< "\n";

				toRemove.push_back(client);

				// broadcast to clients if they saw it before
				if (client->isLoggedIn()){
					broadcastMessage( TCPMessage( TCPMessage::MSG_PLAYER_LEFT,
						client->getCurrentCharacter()->getName() ), client.get() );
				}

				// save the account data
				client->save();

				// remove from list
				itor = _clients.erase(itor);
		}
		else{
			// send message to other clients
			if (result->type == TCPMessage::MSG_CHAT){
				broadcastMessage(result);
			} else if (result->type == TCPMessage::MSG_SELECT_CHAR){
				// character selected

				{ // send the map to the player
					int mapSizes[] = { MAP_MAX_WIDTH, MAP_MAX_HEIGHT };
					TCPMessagePtr msg = TCPMessage::createMessage(TCPMessage::MSG_MAP, 
						mapSizes, 2, std::string(_mapData.contents().begin(), _mapData.contents().end()));

					msg->sendToSocket(client->getSocket());
				}
				
				if ( _clients.size() > 1 )
				{ // send the existing players list (except us)
					std::string playerStr;
					playerStr += ( _clients.size() - 1 );

					for ( ClientList::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
						ClientPtr other = *it;
						if ( other != client && other->isLoggedIn() ) {

							int data[] = 
							{ 
								other->getCurrentCharacter()->position().x, 
								other->getCurrentCharacter()->position().y,
								other->getCurrentCharacter()->level()
							};

							std::string tmpStr;
							tmpStr += other->getCurrentCharacter()->getClass();
							tmpStr += other->getCurrentCharacter()->getName();

							playerStr += TCPMessage::createMessage(TCPMessage::MSG_LIST_PLAYERS, 
								data, 3, tmpStr)->data;
							playerStr += '\0';
						}
					}

					TCPMessage playersMsg( TCPMessage::MSG_LIST_PLAYERS, playerStr );
					playersMsg.sendToSocket( client->getSocket() );
				}

				{ // announce the players about the new player					
					miniui::vec2 position = client->getCurrentCharacter()->position();
					std::string tmpStr;
					tmpStr += client->getCurrentCharacter()->getClass();
					tmpStr += client->getCurrentCharacter()->getName();

					int data[] = { position.x, position.y, client->getCurrentCharacter()->level() };
					TCPMessagePtr joinMsg = TCPMessage::createMessage(TCPMessage::MSG_PLAYER_JOINED, data, 3, tmpStr);
					broadcastMessage(joinMsg, client.get());
				}
			}

			++itor;
		}
	}
	return toRemove;
}

void GameWorld::addClient(ClientPtr& client)
{
	_clients.push_back(client);
}

void GameWorld::broadcastMessage(const TCPMessage& msg, const GameClient* except) const
{
	for(ClientList::const_iterator msg_itor = _clients.begin()
		; msg_itor != _clients.end()
		; ++ msg_itor){
			if ( (*msg_itor).get() != except)
				(*msg_itor)->sendTcpMessage(msg);
	}
}

void GameWorld::broadcastMessage(const TCPMessagePtr& msg, const GameClient* except) const
{
	broadcastMessage(*msg, except);
}

bool GameWorld::canMoveTo( int x, int y, const GameClient& client, bool doMove )
{
	int x_map = x / MAP_OBJECT_WIDTH;
	int y_map = y / MAP_OBJECT_HEIGHT;
	std::cout <<abs( client.getCurrentCharacter()->position().x - x )<< "-"
		<<abs( client.getCurrentCharacter()->position().y - y )<< "-"
		<<_mapData( x_map, y_map ) << "\n";

	bool canMove =  
		( 
			abs( client.getCurrentCharacter()->position().x - x ) <= MAP_OBJECT_WIDTH &&
			abs( client.getCurrentCharacter()->position().y - y ) <= MAP_OBJECT_HEIGHT &&
			_mapData( x_map, y_map ) == WALKABLE
		);

	// save new position
	if ( canMove == true && doMove == true ) {
		_mapData( client.getCurrentCharacter()->position().x / MAP_OBJECT_WIDTH , 
			client.getCurrentCharacter()->position().y / MAP_OBJECT_HEIGHT ) = WALKABLE;
		_mapData( x_map , y_map ) = CHARACTER;
	}

	return canMove;
}