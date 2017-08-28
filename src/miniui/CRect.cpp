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
#include "CRect.h"

namespace miniui
{

	CRect::CRect()
	{
		x = y = w = h = 0;
	}

	CRect::CRect( int _x, int _y )
	{
		x = _x; y = _y; w = h = 0;
	}

	CRect::CRect( int _x, int _y, int _w, int _h )
	{
		x = _x; y = _y;
		w = _w; h = _h;
	}

	CRect::CRect(miniui::CPoint pos)
	{
		x = pos.x;
		y = pos.y;
	}

	SDL_Rect* CRect::getSDLRect()
	{
		if ( x == 0 && y == 0 && w == 0 && h == 0 )
			return NULL;

		SDL_Rect* sdlRect = new SDL_Rect();

		sdlRect->x = x;
		sdlRect->y = y;
		sdlRect->w = w;
		sdlRect->h = h;

		return sdlRect;
	}

	CRectPtr CRect::getPoint()
	{
		return CRectPtr(new CRect( x, y ) );
	}

	CRect CRect::operator+( CRect& other )
	{
		return CRect( x + other.x, y + other.y, w + other.w, h + other.h );
	}

	CRect CRect::operator-( CRect& other )
	{
		return CRect( x - other.x, y - other.y, w - other.w, h - other.h );
	}

	bool CRect::contains( CRect& other )
	{
		return x <= other.x && y <= other.y && ( x + w ) >= ( other.x + other.w ) && ( y + h ) >= ( other.y + other.h );
	}
}