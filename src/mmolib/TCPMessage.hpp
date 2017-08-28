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
#ifndef TCPMESSAGE_HPP_INCLUDED
#define TCPMESSAGE_HPP_INCLUDED

#include "SDL_net.h"
#include <string>
#include "boost/shared_ptr.hpp"

class TCPMessage;

typedef boost::shared_ptr<TCPMessage> TCPMessagePtr;

/**
 * Message sent between client and server
 * The format of the tcp message is the following:
 *
 * |Message Type - 1 byte | Message content length - 4 bytes | Message content - max 1019 bytes|
 * 
 */
class TCPMessage{
public:	
	/**
	 * The type of a TCPMessage
	 * @see TCPMessage
	 */
	enum message_type : char{
		MSG_DEFAULT			= 0,
		MSG_ERROR			= 1,
		MSG_QUIT			= 2,
		MSG_DISCONNECT		= 3,
		MSG_PLAYER_JOINED	= 4,
		MSG_PLAYER_LEFT		= 5,

		MSG_CHAT			= 10,

		MSG_LOGIN			= 30,
		MSG_LOGINOK			= 31,
		MSG_LOGINFAIL_USR	= 32,
		MSG_LOGINFAIL_PASS	= 33,

		MSG_LIST_CHARS		= 35,
		MSG_SELECT_CHAR		= 36,
		MSG_CHAR_DETAILS	= 37,

		MSG_LIST_PLAYERS	= 40,
		MSG_PLAYER_MOVE		= 41,

		MSG_MOVE_REQ		= 60,
		MSG_MOVE_COORD		= 61,
		MSG_MAP				= 62,

		MSG_EOF				= 100
	};

	/**
	 * Creates a new TCP message
	 *
	 * @param msg_type The type of the message
	 * @param content The text content to send
	 */
	explicit TCPMessage(message_type msg_type = MSG_DEFAULT, const std::string& content = "");

	/**
	 * Creates a TCPMessage copy
	 *
	 * @param msg the message to copy from
	 */
	TCPMessage(const TCPMessage& msg);

	inline TCPMessage& TCPMessage::operator=(const TCPMessage& other);

	/**
	 * The message type
	 */
	message_type type;
	
	/**
	 * The message's contents
	 */
	std::string data;

	/**
	 * Sends the current message to the specified
	 * socket
	 *
	 * @param socket The socket to send the message to
	 * @return 0 on success, -1 on error
	 */
	int sendToSocket(const TCPsocket& socket) const;

	/**
	 * Constructs a received message on that socket and
	 * returns it. This is a blocking method.
	 *
	 * @param socket The socket to receive the message from
	 * @param The received message. If the message
	 * has the type MSG_QUIT that means the remote
	 * socket closed the connection
	 */
	static TCPMessagePtr receiveFromSocket( const TCPsocket& socket );

	/**
	 * Creates a new TCPMessage which consists of the specified integers
	 *
	 * @param type The type of the message
	 * @param data The integers array
	 * @param size The number of integers
	 */
	static TCPMessagePtr createMessage( message_type type, const int data[], int size );

	/**
	 * Creates a new TCPMessage which consists of an array of integer and a string
	 *
	 * @param type The type of the message
	 * @param data1 The integers array
	 * @param size The number of integers
	 * @param data2 The string
	 */
	static TCPMessagePtr createMessage( message_type type, const int data[], int size, const std::string& data2 );
};
#endif