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
#include "Player.hpp"

#include <iostream>

#include "mmolib/MMOUtils.hpp"
#include "miniui/CRenderQueue.h"

Player::Player( CharacterClass class_ )
: _class( class_ )
, _boundingBox( 0, 0, MAP_OBJECT_WIDTH, MAP_OBJECT_HEIGHT )
, _sprite( "client_data\\unknown.png" )
, _name()
, _level( 0 )
, _nameSprite( NULL )
{
	loadClassSprite();
	setName( "Unknown" , true );
}

Player::~Player()
{
	if ( _nameSprite != NULL )
		delete _nameSprite;
}

void Player::draw()
{
	RENDERQUEUE->addImage( &_sprite, _boundingBox );
	RENDERQUEUE->addImage( _nameSprite, miniui::CRect( 
		_boundingBox.x,
		_boundingBox.y + _boundingBox.h,
		_nameSprite->width(),
		_nameSprite->height()) );
}

void Player::setClass( CharacterClass newClass )
{
	_class = newClass;
	loadClassSprite();
}

void Player::loadClassSprite()
{
	if ( _class == CLASS_WIZARD ) // wizard
		_sprite = miniui::CImage( "client_data\\fire.png" );
	else if ( _class == CLASS_WARRIOR )
		_sprite = miniui::CImage( "client_data\\sword.png" );
}