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
#ifndef _C_POINT_H_
#define _C_POINT_H_

namespace miniui
{
	class CPoint
	{
	public:
		int x;
		int y;

		CPoint::CPoint() : x(0), y(0) { };
		CPoint::CPoint( int _x, int _y ) : x(_x), y(_y)	{ };	
	};

}

#endif