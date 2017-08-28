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
#ifndef GAME_OBJECT_HPP_INCLUDED
#define GAME_OBJECT_HPP_INCLUDED

#include "SDL_timer.h"

#include "miniui/UiTypes.h"

#include "boost/shared_ptr.hpp"

enum GameObjectType : char {
	WALKABLE	= ' ',
	TRUNK		= 'T',

	CHARACTER	= 'C',
	ENEMY		= 'E'
};

/**
 * An interactable object in the GameWorld
 * This is the base class for all objects that are in the
 * GameWorld
 */
class GameObject{
	
public:
	/**
	 * Returns this object's type
	 */
	GameObjectType getType() const { return _objType; }
	virtual ~GameObject() { }

	/**
	 * Returns this object's position
	 */
	miniui::vec2& position() { return _position; }

protected:
	/**
	 * Creates a new Game Object with the type WALKABLE.
	 * For other types the specific specialized class should
	 * be instantiated
	 *
	 * @param type The type of the game object
	 */
	explicit GameObject( GameObjectType type = WALKABLE ) : _objType( type ) {}

	GameObject( const GameObject& other ) : _objType( other._objType ) { }
	GameObject& GameObject::operator=( const GameObject& ) { }

	GameObjectType _objType;

	miniui::vec2 _position;
};
#endif