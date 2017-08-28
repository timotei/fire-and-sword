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
#include "mmolib\Config.hpp"
#include "mmolib\MMOUtils.hpp"
#include "mmolib\MMOTypes.hpp"
#include "mmolib\TCPMessage.hpp"

#include "SDL.h"
#include "SDL_Net.h"
#include <ostream>
#include <algorithm>
#include <iostream>
#include "boost\thread.hpp"
#include "boost\bind.hpp"

#include "miniui\CTextRender.h"
#include "miniui\CUiManager.h"
#include "miniui\CButton.h"
#include "miniui\CImage.h"
#include "miniui\UiTypes.h"
#include "miniui\CRect.h"
#include "miniui\CRenderQueue.h"

#include "Client.hpp"

Client* Client::_instance = NULL;

Client::Client()
: _running(true)
, _config("client.cfg")
, _serverIp(NULL)
, _isConnectedToServer(false)
, _screen(NULL)
, _imgTrunk( "client_data\\tree_trunk.png" )
, _imgBackground( "client_data\\backgroundBig.png" )
, _mapHeight(0)
, _mapWidth(0)
{
	// init graphics and network
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::cout << "Error initializing SDL: " << SDL_GetError() << "\n";
		return;
	}

	if (SDLNet_Init() < 0){
		std::cout << "Error initializing SDL Net: " << SDLNet_GetError() << "\n";
		return;
	}

	std::cout << "Mini Ui version: " << MINIUI->getVersion() << "." << MINIUI->getRevision() << "\n";
}

Client::~Client()
{
	SDLNet_TCP_DelSocket(_socketSet, _serverSocket);
	SDLNet_TCP_Close(_serverSocket);
	SDLNet_FreeSocketSet(_socketSet);

	delete _serverIp;

	_serverSocket = NULL;
	_socketSet = NULL;
	// cleanup
	SDLNet_Quit();
	SDL_Quit();
}

#define SERVER_

void Client::runGame()
{
	MINIUI->setScreenSize( 800, 800 );
	MINIUI->useTheme( "client_data\\theme_medieval.png" );
	MINIUI->useCursor( true );

	_config.load();

	_currentPlayer = PlayerPtr(new Player);

#ifdef SERVER_
	doLogin();
#endif

	// start the input thread
	boost::shared_ptr<boost::thread> thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Client::inputThread, this)));
	if (thread == NULL){
		std::cout << "Error creating thread!.\n";
		return;
	}

	_screen = SDL_SetVideoMode( 821, 645, 32, SDL_DOUBLEBUF );
	SDL_WM_SetCaption( "Fire & Sword :: Client", 0 );

	// add the quit button
	miniui::CButton quitButton( 0 );
	quitButton.setGeometry( miniui::CRect( 700, 500) );
	quitButton.setCaption( "QUIT" );
	quitButton.addEventListener( this );

	while ( _running )
	{
		SDL_Event evt;
		while ( SDL_PollEvent( &evt ) )
		{
			MINIUI->injectInput( evt );
			quitButton.handleEvent( evt );			

			if ( evt.type == SDL_QUIT ){
				_running = false;
				sendQuit();
			}
			else if (evt.type == SDL_KEYDOWN){
				bool moved = false;
				switch(evt.key.keysym.sym){
					case SDLK_LEFT:
						_currentPlayer->boundingBox().x -= MAP_OBJECT_WIDTH;
						moved = true;
						break;
					case SDLK_RIGHT:
						_currentPlayer->boundingBox().x += MAP_OBJECT_WIDTH;
						moved = true;
						break;
					case SDLK_DOWN:
						_currentPlayer->boundingBox().y += MAP_OBJECT_HEIGHT;
						moved = true;
						break;
					case SDLK_UP:
						_currentPlayer->boundingBox().y -= MAP_OBJECT_HEIGHT;
						moved = true;
						break;
					case SDLK_ESCAPE:
						_running = false;
						sendQuit();
						break;
				}

				if (moved)
					sendMoveRequest(_currentPlayer->boundingBox().x, _currentPlayer->boundingBox().y);
			}
		}

		checkServerResponse();
		
		SDL_FillRect( _screen, &_screen->clip_rect, 0x9e8579 );

		// draw background
		RENDERQUEUE->addImage( &_imgBackground );

		// draw interface
		quitButton.render();

		// draw map
		drawMap( _screen );
		// draw players
		for ( PlayersMap::const_iterator itor = _players.begin(); itor != _players.end(); ++ itor ) {
			(*itor).second->draw();
		}
		_currentPlayer->draw();
		MINIUI->update( 0, _screen );

		SDL_Flip( _screen );
		SDL_Delay(10);
	}
}

void Client::onMouseClick( miniui::CWidget* sender )
{
	miniui::CButton* butSource = dynamic_cast<miniui::CButton*> ( sender);
	if ( butSource != NULL ){
		if ( butSource->getCaption() == "QUIT" )
			_running = false;
	}
}

void Client::drawMap( SDL_Surface* surface )
{
	int line = 0, col = 0, index, max_index = _mapHeight * _mapWidth;
	miniui::CRect dest = _imgTrunk.getBoundingRect();

	for ( index = 0; index < max_index; ++ index ) {
		if (_mapData[index] == 'T'){
			RENDERQUEUE->addImage( &_imgTrunk, dest);
			//_imgTrunk.blitTo( surface , dest );
		}
		
		col = ( col + 1 ) % _mapHeight;
		if ( col == 0 )
			++ line;

		dest.x = col * 32;
		dest.y = line * 32;
	}
}

void Client::sendMoveRequest(const int& x, const int& y)
{
	int coords[] = { x, y };
	TCPMessagePtr msg = TCPMessage::createMessage(TCPMessage::MSG_MOVE_REQ, coords, 2);
	msg->sendToSocket(_serverSocket);
}

void Client::sendQuit()
{
	TCPMessage quitMsg(TCPMessage::MSG_QUIT, "");
	quitMsg.sendToSocket(_serverSocket);
}

int Client::connectToServer()
{
	const std::string ip = _config.get("ip");
	const int port = _config.get_int("port", SERVER_DEFAULT_PORT);

	_serverIp = new IPaddress;

	if (SDLNet_ResolveHost(_serverIp, ip.c_str(), port) < 0){
		std::cout << "Error resolving remote host: " << SDLNet_GetError() << "\n";
		return -1;
	}

	std::cout << "Connected to: " << to_string_ip( _serverIp->host ) << "\n";

	if ((_serverSocket = SDLNet_TCP_Open(_serverIp)) == NULL){
		std::cout << "Error opening the socket to remote host: " << SDLNet_GetError() << "\n";
		return -1;
	}

	_socketSet = SDLNet_AllocSocketSet(1);
	if (_socketSet == NULL){
		std::cout << "Error creating socket set: " << SDLNet_GetError() << "\n";
		return -1;
	}

	SDLNet_TCP_AddSocket(_socketSet, _serverSocket);
	_isConnectedToServer = true;
	return 0;
}

void Client::inputThread()
{
	std::string message;
	char buffer[1000];

	while(_running == true){

		std::cout << "> ";
		std::cin.getline(buffer, 999);
		message = std::string(buffer);

		std::string buffer;
		// trim the message to maximum size
		if (message.size() > MSG_CONTENT_LEN){
			message = message.substr(0, MSG_CONTENT_LEN);
		}

		if (message.empty() == false){
			TCPMessage msg_to_send;

			if (message.at(0) == '/' && message.size() > 1){ // command
				// trim the slash
				size_t space_pos = message.find_first_of(' ');
				std::string command;
				if (space_pos == std::string::npos)
					command = message.substr(1, message.size());
				else
					command = message.substr(1, space_pos - 1);

				if (command == "quit"){
					_running = false;
					sendQuit();
				}
				else if (command == "move"){
					if (space_pos != std::string::npos){
						std::cout << command << " - " << message.substr(space_pos+1);
					}
				}

			}
			else if (message.at(0) != '.') { // message to other clients
				msg_to_send.type = TCPMessage::MSG_CHAT;
				msg_to_send.data = message;
			}


			if (_isConnectedToServer == false){
				std::cout << "Not connected to server!\n";
				continue;
			}

			// send the message if ok
			if (msg_to_send.type != TCPMessage::MSG_DEFAULT){
				msg_to_send.sendToSocket(_serverSocket);
			}
		}
		else std::cout << "\n";
	}
}

int Client::setPlayerDetails( const char* data, const PlayerPtr& player )
{
	const int* x = reinterpret_cast<const int*>( data );
	const int* y = reinterpret_cast<const int*>( data + INT_SIZE);
	const int* level = reinterpret_cast<const int*>( data + INT_DOUBLE_SIZE );
	CharacterClass charClass = static_cast<CharacterClass>(* (data + INT_TRIPLE_SIZE ));
	std::string name( data + INT_TRIPLE_SIZE + 1 );

	player->setName( name, player == _currentPlayer );
	player->setClass(charClass);
	player->level() = *level;
	player->boundingBox().x = *x;
	player->boundingBox().y = *y;

	std::cout << "set details: name {" << name << "} class: {" 
		<< charClass << "} level: {" 
		<< *level << "} x: {" << *x << "} y: {" << *y << "}\n";

	return ( INT_TRIPLE_SIZE + 1 + name.size() );
}

void Client::doLogin()
{
	if (_isConnectedToServer == false){
		if (connectToServer() == -1){
			std::cout << "Could not connect to server :(\n";
			return;
		}
	}

	// wait till logged
	bool loggedIn = false;
	bool charSelected = false;
	do{
		std::string username, password;
		std::cout << "Username: ";
		std::cin >> username;

		std::cout << "Password: ";
		std::cin >> password;

		// send query to server
		TCPMessage login_msg(TCPMessage::MSG_LOGIN, username + ";" + password);
		login_msg.sendToSocket(_serverSocket);

		TCPMessagePtr res;

		std::cout << "Waiting for authorization...\n";
		while(true){

			res = checkServerResponse();
			if (res){
				if (res->type == TCPMessage::MSG_LOGINOK){
					loggedIn = true;
					std::cout << "Logged in successfully. Please select a character:\n";

					// request chars list
					TCPMessage charsListMsg(TCPMessage::MSG_LIST_CHARS, "");
					charsListMsg.sendToSocket(_serverSocket);
				}
				else if ( res->type == TCPMessage::MSG_LOGINFAIL_USR )
				{
					std::cout << "Invalid username.\n";
					break;
				}
				else if ( res->type == TCPMessage::MSG_LOGINFAIL_PASS )
				{
					std::cout << "Invalid password.\n";
					break;
				}
				else if ( res->type == TCPMessage::MSG_LIST_CHARS )
				{
					// output chars list
					std::vector< std::string > charList = split_string(res->data);
					
					for ( std::vector< std::string >::const_iterator char_itor = charList.begin()
						; char_itor != charList.end()
						; ++char_itor ){
							std::cout << *char_itor << "\n";
					}

					std::cout << "> ";
					std::string charName;

					while(true) {
						std::cin >> charName;

						if (std::find(charList.begin(), charList.end(), charName) != charList.end())
							break; // correct char name
						std::cout << "Incorrect character name!. Try again\n>";
					}

					TCPMessage selectCharMsg(TCPMessage::MSG_SELECT_CHAR, charName);
					selectCharMsg.sendToSocket(_serverSocket);
				}
				else if ( res->type == TCPMessage::MSG_CHAR_DETAILS )
				{
					charSelected = true;
					setPlayerDetails( res->data.c_str(), _currentPlayer );
					std::string title =  "Fire & Sword :: Client - ";
					title += _currentPlayer->getName();

					SDL_WM_SetCaption( title.c_str(), 0 );

					break;
				}
			}

			SDL_Delay(1);
		}
	}while( loggedIn == false || charSelected == false );
}


TCPMessagePtr Client::checkServerResponse()
{
	if (_isConnectedToServer == false)
		return TCPMessagePtr();

	// check for server activity.
	if (SDLNet_CheckSockets(_socketSet, 0) > 0 &&
		SDLNet_SocketReady(_serverSocket)){

		TCPMessagePtr msg = TCPMessage::receiveFromSocket(_serverSocket);

		if ( msg->type == TCPMessage::MSG_CHAT ){
			std::cout << "< ";
			// print the message to this client
			std::cout << msg->data << "\n";
		} else if ( msg->type == TCPMessage::MSG_DISCONNECT ){
			// server got down
			std::cout << "No response from server. Quitting...\n";
			_running = false;
		} else if ( msg->type == TCPMessage::MSG_MOVE_COORD ) {
			// enforce new coordinates
			const int* x = reinterpret_cast<const int*>(msg->data.c_str());
			const int* y = reinterpret_cast<const int*>(msg->data.c_str() + INT_SIZE);

			_currentPlayer->boundingBox().x = *x;
			_currentPlayer->boundingBox().y = *y;
		} else if ( msg->type == TCPMessage::MSG_MAP ) {
			// load the map
			const int* width = reinterpret_cast<const int*>(msg->data.c_str());
			const int* height = reinterpret_cast<const int*>(msg->data.c_str() + INT_SIZE);
			_mapData = msg->data.substr(INT_DOUBLE_SIZE);

			_mapHeight = *height;
			_mapWidth = *width;
		} else if ( msg->type == TCPMessage::MSG_LIST_PLAYERS ) {

			int playersCount = static_cast<int>( msg->data[0] );
			// size of player details message
			int offset = 1;

			for( int i = 0; i < playersCount; ++i, ++offset ){
				PlayerPtr newPlayer(new Player());
				offset += setPlayerDetails( msg->data.c_str() + offset, newPlayer );

				_players[newPlayer->getName()] = newPlayer;
			}

		} else if ( msg->type == TCPMessage::MSG_PLAYER_JOINED ) {
			PlayerPtr player(new Player);
			setPlayerDetails( msg->data.c_str(), player );
			
			_players[player->getName()] = player;

			std::cout << "Player '" << player->getName() << "' joined.\n";
		} else if ( msg->type == TCPMessage::MSG_PLAYER_LEFT ) {
			_players.erase(msg->data);

			std::cout << "Player '" << msg->data << "' left.\n";
		} else if ( msg->type == TCPMessage::MSG_PLAYER_MOVE ) {
			const int* x = reinterpret_cast<const int*>(msg->data.c_str());
			const int* y = reinterpret_cast<const int*>(msg->data.c_str() + INT_SIZE);
			std::string name( msg->data.substr( INT_DOUBLE_SIZE ) );

			PlayerPtr player = _players[name];
			player->boundingBox().x = *x;
			player->boundingBox().y = *y;
		}

		return msg;
	}	

	return TCPMessagePtr(); // no response
}

Client* Client::getInstance()
{
	if (_instance == NULL)
		_instance = new Client;
	return _instance;
}

int main(int, char* [])
{
	// run the game
	Client::getInstance()->runGame();
    return 0;
}
