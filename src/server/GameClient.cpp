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
#include <iostream>
#include <cstring>

#include "mmolib/MMOUtils.hpp"
#include "mmolib/TCPMessage.hpp"

#include "GameClient.hpp"
#include "Server.hpp"

const char* GameClient::QUERY_ACCOUNT_DETAILS_BY_USERNAME = 
	"SELECT * FROM accounts WHERE name=?";

const char* GameClient::QUERY_CHARACTER_DETAILS_BY_ACCNAME = 
	"SELECT * FROM characters WHERE id_account=? AND name=?";

const char* GameClient::QUERY_CHARACTER_LIST_BY_ACCID = 
	"SELECT name FROM characters WHERE id_account=?";

GameClient::GameClient( GameWorld& world, IPaddress* ip, TCPsocket& socket )
: _ip(ip)
, _socket( socket )
, _currentChar()
, _isLoggedIn( false )
, _id( 0 )
, _world( &world )
{
}

GameClient::~GameClient()
{
	SDLNet_TCP_Close(_socket);
	_socket = NULL;
}

TCPMessagePtr GameClient::processClient()
{
	TCPMessagePtr msg(new TCPMessage);

	if (SDLNet_SocketReady(_socket)){
		msg = TCPMessage::receiveFromSocket(_socket);
		std::cout << "Got message[" << static_cast<int>( msg->type ) << "]: "<< msg->data << "\n";
		
		if ( msg->type == TCPMessage::MSG_LOGIN ){
			size_t tmpPos = msg->data.find_first_of(';');
			if (tmpPos == std::string::npos){
				std::cout << "Invalid LOGIN message!!!\n";
				return TCPMessagePtr(new TCPMessage(TCPMessage::MSG_DISCONNECT, ""));
			}

			// check login
			std::string username(msg->data.substr(0, tmpPos));
			std::string password(msg->data.substr(tmpPos + 1));

			msg->type = login( username, password );
		} else if ( msg->type == TCPMessage::MSG_LIST_CHARS )
		{
			std::string chars;
			sqlite3_stmt* query = Database::getInstance()->prepare( QUERY_CHARACTER_LIST_BY_ACCID );
			sqlite3_bind_int( query, 1, _id );
			
			while(sqlite3_step( query ) == SQLITE_ROW) {
				chars += reinterpret_cast<const char*>( sqlite3_column_text( query, 0 ) );

				if (chars.empty() == false)
					chars += ';';
			}

			TCPMessage charsMsg( TCPMessage::MSG_LIST_CHARS, chars );
			charsMsg.sendToSocket(_socket);

		} else if ( msg->type == TCPMessage::MSG_SELECT_CHAR ) {
			selectChar(msg->data);

		} else if ( msg->type == TCPMessage::MSG_MOVE_REQ ) {
			const int* x = reinterpret_cast<const int*>(msg->data.c_str());
			const int* y = reinterpret_cast<const int*>(msg->data.c_str() + INT_SIZE);
			std::cout << "move request: " << *x << " " << *y << "\n";

			int coords[] = { *x, *y };
			if ( _world->canMoveTo( *x, *y, *this, true ) == false ){
				// reset to current coords
				coords[0] = getCurrentCharacter()->position().x;
				coords[1] = getCurrentCharacter()->position().y;

				// send correction message
				TCPMessagePtr msg = TCPMessage::createMessage( TCPMessage::MSG_MOVE_COORD, coords, 2 );
				msg->sendToSocket(_socket);
			} else {
				// save the coords
				getCurrentCharacter()->position().x = coords[0];
				getCurrentCharacter()->position().y = coords[1];

				TCPMessagePtr playerMsg = TCPMessage::createMessage( TCPMessage::MSG_PLAYER_MOVE, coords, 2,
					getCurrentCharacter()->getName());
				_world->broadcastMessage( playerMsg, this );
			}
		}
	}

	return msg;
}

int GameClient::sendTcpMessage(const TCPMessage& msg) const
{
	return msg.sendToSocket(_socket);
}

void GameClient::save()
{
	if ( _currentChar != NULL ){
		if ( _currentChar->save() == -1)
			std::cerr << "Error saving character '" << _currentChar->getName() << "' to database.\n";
	}
}

TCPMessage::message_type GameClient::login(const std::string& username, const std::string& password)
{
	std::cout << "Trying to log in with: [" << username << "] - [" << password << "]\n";
	if (username.empty() == false && password.empty() == false){
		sqlite3_stmt* query = Database::getInstance()->prepare(QUERY_ACCOUNT_DETAILS_BY_USERNAME);
		sqlite3_bind_text(query, 1, username.c_str(), username.size(), NULL);
		sqlite3_step(query);

		const unsigned char* res_password = sqlite3_column_text(query, 2);

		if (res_password != NULL){ // account ok
			if (strcmp( reinterpret_cast<const char*>( res_password ), password.c_str() ) == 0){ // pass ok
				TCPMessage ok_msg(TCPMessage::MSG_LOGINOK, "");
				ok_msg.sendToSocket(_socket);

				// save current data
				_username = username;
				_id = sqlite3_column_int(query, 0);


				return TCPMessage::MSG_LOGINOK;
			}

			TCPMessage passfail_msg(TCPMessage::MSG_LOGINFAIL_PASS,  "");
			passfail_msg.sendToSocket(_socket);
			return TCPMessage::MSG_LOGINFAIL_PASS;
		}
	}

	TCPMessage userfail_msg(TCPMessage::MSG_LOGINFAIL_USR, "");
	userfail_msg.sendToSocket(_socket);
	return TCPMessage::MSG_LOGINFAIL_USR;
}

TCPMessage::message_type GameClient::selectChar(const std::string& charName)
{
	// retrieve char details
	sqlite3_stmt* query = Database::getInstance()->prepare( QUERY_CHARACTER_DETAILS_BY_ACCNAME );
	sqlite3_bind_int( query, 1, _id );
	sqlite3_bind_text( query, 2, charName.c_str(), charName.size(), NULL );
	sqlite3_step(query);

	// now grab the details
	_currentChar = CharacterPtr( new Character( 
		*this, 
		static_cast<CharacterClass> ( sqlite3_column_int( query, 3 ) ),
		sqlite3_column_int( query, 0 ),
		charName ) );

	_currentChar->level() = sqlite3_column_int( query, 4 );
	_currentChar->position().x = sqlite3_column_int( query, 5 );
	_currentChar->position().y = sqlite3_column_int( query, 6 );

	_isLoggedIn = true;

	std::string tmpStr;
	tmpStr += static_cast<char> ( sqlite3_column_int( query, 3 ) );
	tmpStr += charName;

	int data[] = { _currentChar->position().x, _currentChar->position().y, _currentChar->level() };
	// send the details to the client
	TCPMessagePtr detailsMsg = TCPMessage::createMessage( TCPMessage::MSG_CHAR_DETAILS, data, 3, tmpStr );
	detailsMsg->sendToSocket(_socket);

	return TCPMessage::MSG_SELECT_CHAR;
}