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
#ifndef _C_RECT_
#define _C_RECT_

#include <memory>
#include "SDL.h"
#include "CPoint.h"

namespace miniui
{
	class CRect;
	typedef std::auto_ptr<CRect> CRectPtr;
	typedef std::auto_ptr<SDL_Rect> SDLRectPtr;

	class CRect
	{
	public:

		int x, y, w, h;

		CRect();
		CRect( int _x, int _y );
		CRect( int _x, int _y, int _w, int _h );
		CRect( CPoint pos );

		SDL_Rect* getSDLRect();
		CRectPtr getPoint();

		CRect operator+( CRect& other );
		CRect operator-( CRect& other );

		bool contains( CRect& other );
	};
}

#endif