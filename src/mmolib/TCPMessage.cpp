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
#include "TCPMessage.hpp"
#include "assert.h"
#include "MMOUtils.hpp"

#include <iostream>
#include <vector>

TCPMessage::TCPMessage( message_type msg_type, const std::string& content )
: type(msg_type)
, data(content)
{
}

TCPMessage::TCPMessage( const TCPMessage& msg )
: type(msg.type)
, data(msg.data)
{
}

TCPMessage& TCPMessage::operator =( const TCPMessage& other )
{
	if (this != &other){

		data = std::string(other.data);
		type = message_type(other.type);
	}

	return *this;
}

int TCPMessage::sendToSocket( const TCPsocket& socket ) const
{
	if (type == MSG_DEFAULT){
		std::cerr << "Invalid message to send!\n";
		return -1;
	}

	if (socket == NULL){
		std::cerr << "Invalid socket\n";
		return -1;
	}

	size_t size = data.size();

	assert( size < 1019 );
	if (size > 1019){
		std::cout << "Message too big to be sent!\n";
		return -1;
	}

	char* size_ptr = reinterpret_cast<char*>(&size);

	std::vector<char> buffer(sizeof(unsigned char) + size + SIZE_T_SIZE);

	buffer[0] = type;
	std::copy(size_ptr, size_ptr + SIZE_T_SIZE, &(buffer[1]));
	if (size > 0)
		std::copy(data.begin(), data.end(), &(buffer[SIZE_T_SIZE + 1]));

	int res = SDLNet_TCP_Send(socket, &buffer[0], buffer.size());
	if (res < 0){
		std::cerr << "There was an error sending the data: " << SDLNet_GetError() << "\n";
	}

	return res;
}

TCPMessagePtr TCPMessage::receiveFromSocket(const TCPsocket& socket)
{
	char msg_type;
	size_t size;
	int recv_cnt = 0;

	recv_cnt = SDLNet_TCP_Recv(socket, reinterpret_cast<char*>(&msg_type), 1);
	if (recv_cnt <= 0)
		return TCPMessagePtr(new TCPMessage(TCPMessage::MSG_DISCONNECT, ""));

	recv_cnt = SDLNet_TCP_Recv(socket, reinterpret_cast<char*>(&size), SIZE_T_SIZE);
	if (recv_cnt == 4){
		std::vector<char> buffer(size);

		if (size > 0){
			recv_cnt = SDLNet_TCP_Recv(socket, &(buffer[0]), size);
			if (recv_cnt == (int)size){
				return TCPMessagePtr( new TCPMessage( static_cast<TCPMessage::message_type>( msg_type ), 
						std::string(buffer.begin(), buffer.end())));
			}
		}
		else
			return TCPMessagePtr(new TCPMessage( static_cast<TCPMessage::message_type>( msg_type ), ""));
	}

	std::cerr << "Error on receiving message. Expected " << size << " but got " << recv_cnt << "\n";
	return TCPMessagePtr(new TCPMessage(TCPMessage::MSG_ERROR, ""));
}

TCPMessagePtr TCPMessage::createMessage( message_type type, const int data[], int size )
{
	TCPMessagePtr res(new TCPMessage( static_cast<TCPMessage::message_type>( type )));
	
	std::vector< char > buf( size * INT_SIZE );
	for ( int i = 0; i < size; ++i ){
		const char* data_ptr = reinterpret_cast<const char*>( data + i );
		std::copy( data_ptr, data_ptr + INT_SIZE, &buf[INT_SIZE * i] );
	}
	res->data = std::string( buf.begin(), buf.end() );

	return res;
}

TCPMessagePtr TCPMessage::createMessage( message_type type, const int data[], int size, const std::string& data2 )
{
	TCPMessagePtr res = createMessage( type, data, size );

	res->data += data2;

	return res;
}