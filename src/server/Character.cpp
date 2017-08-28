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
#include "Character.hpp"

#include "sqlite3.h"

#include "Server.hpp"

const char* Character::QUERY_SAVE_CHARACTER = 
	"UPDATE characters SET level=?, position_x=?, position_y=? WHERE id_character=?;";

Character::Character( GameClient& client, CharacterClass class_, int id, const std::string& name )
: GameObject( CHARACTER)
, _charClass( class_ )
, _client( client )
, _level( 0 )
, _id( id )
, _name( name )
{
}

Character::~Character()
{
}

int Character::save()
{
	sqlite3_stmt* query = Database::getInstance()->prepare( QUERY_SAVE_CHARACTER );

	sqlite3_bind_int( query, 1, _level );
	sqlite3_bind_int( query, 2, _position.x );
	sqlite3_bind_int( query, 3, _position.y );
	sqlite3_bind_int( query, 4, _id );
	int res = sqlite3_step( query );
	if ( res != SQLITE_DONE ){
		std::cout << "Error on query: " << Database::getInstance()->getErrorMessage() << "\n";
		return -1;
	}
	return 0;
}
