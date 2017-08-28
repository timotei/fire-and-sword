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
#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "boost/shared_ptr.hpp"

#include "mmolib/MMOTypes.hpp"
#include "mmolib/MMOUtils.hpp"

#include "miniui/CRect.h"
#include "miniui/CUiManager.h"
#include "miniui/CImage.h"

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_image.h"

#include "ClientUtils.hpp"

class Player;

typedef boost::shared_ptr<Player> PlayerPtr;

/**
 * This is a client-side representation of a Player's entity
 * It is mainly used just for display purposes
 */
class Player{

public:
	/**
	 * Creates a new character with the specified class
	 */
	Player( CharacterClass class_ = CLASS_NONE );
	~Player();

	/**
	 * Draws this player
	 */
	void draw();

	/**
	 * Sets a new class for the current player
	 * which will set also a new sprite based on the class
	 */
	void setClass( CharacterClass newClass );

	const CharacterClass& getClass () { return _class; }

	/**
	 * Returns the name of this Player's.
	 */
	const std::string& getName() { return _name; }

	/**
	 * Sets player's name to the new one
	 * @param newName The new Name to set
	 */
	void setName( const std::string& newName, bool main_player ) 
	{
		if ( newName != _name ) {
			_name = newName;
			
			if ( _nameSprite != NULL )
				delete _nameSprite;

			_nameSprite = MINIUI->getTheme()->getTextRenderer()->render( newName, main_player ? 1 : 0 );
		}
	}

	/**
	 * Returns the level of this Player's.
	 */
	int& level() { return _level; }
	
	/**
	 * Returns the bounding box of this Player's.
	 */
	miniui::CRect& boundingBox() { return _boundingBox; }

private:
	DISALLOW_COPY_AND_ASSIGN(Player);
	void loadClassSprite();

	miniui::CImage _sprite;
	miniui::CRect _boundingBox;

	miniui::CImage* _nameSprite;

	CharacterClass _class;
	std::string _name;
	int _level;
};

#endif