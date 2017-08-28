/*
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
#include "CTheme.h"

namespace miniui
{
	bool CTheme::load(std::string filename)
	{
		// load the theme image
		themeImage = new CImage( filename );

		// allocate the space
		for ( int i = 0; i < _TP_SIZE; i++ )
			parts.push_back( 0 );

		parts.at( TP_CURSOR ) = themeImage->copy( CRect( 0, 0, 24, 24 ) );

		parts.at( TP_BUTTON_NORMAL_LEFT ) =		themeImage->copy( CRect(  0, 24, 32, 22 ) );
		parts.at( TP_BUTTON_NORMAL_MIDDLE ) =	themeImage->copy( CRect( 32, 24, 12, 22 ) );
		parts.at( TP_BUTTON_NORMAL_RIGHT ) =	themeImage->copy( CRect( 44, 24, 32, 22 ) );

		parts.at( TP_BUTTON_CLICKED_LEFT ) =	themeImage->copy( CRect(  0, 46, 32, 22 ) );
		parts.at( TP_BUTTON_CLICKED_MIDDLE ) =	themeImage->copy( CRect( 32, 46, 12, 22 ) );
		parts.at( TP_BUTTON_CLICKED_RIGHT ) =	themeImage->copy( CRect( 44, 46, 32, 22 ) );

		textRenderer = new CTextRender( "client_data\\font.png" );
		return 1;
	}

	CImage* CTheme::getPart( THEME_PARTS part )
	{
		return parts.at(part);
	}

	CImage* CTheme::operator[]( THEME_PARTS part )
	{
		return getPart( part );
	}

}