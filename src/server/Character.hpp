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
#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <string>

#include "GameObject.hpp"

#include "miniui/UiTypes.h"
#include "mmolib/MMOTypes.hpp"
#include "mmolib/MMOUtils.hpp"

//#include "GameClient.hpp"
class GameClient;

class Character;
typedef boost::shared_ptr<Character> CharacterPtr;

/**
 * This class represents an active playing character
 * which is tighten to a GameClient
 */
class Character : public GameObject
{

public:
	/**
	 * Creates a new character on which has as parent the
	 * specified GameClient
	 */
	Character( GameClient& client, CharacterClass class_, int id, const std::string& name = std::string() );
	~Character();

	/**
	 * Saves this character to the database
	 *
	 * @return 0 for success, -1 for error
	 */
	int save();

	/**
	 * Returns this Character's class
	 */
	const CharacterClass& getClass() const { return _charClass; }

	/**
	 * Returns this Character's level
	 */
	int& level() { return _level; }
	
	/**
	 * Returns this Character's name
	 */
	const std::string& getName() const { return _name; }

	/**
	 * Returns this Character's id
	 */
	int getId() const { return _id; }

private:
	static const char* QUERY_SAVE_CHARACTER;
	DISALLOW_COPY_AND_ASSIGN(Character);

	GameClient& _client;

	int _id;
	std::string _name;
	int _level;

	CharacterClass _charClass;
};
#endif