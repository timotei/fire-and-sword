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
#ifndef _UI_TYPES_H_
#define _UI_TYPES_H_

#include "SDL_video.h"

namespace miniui
{
	struct vec2
	{
		int x, y;
		vec2( int _x = 0, int _y = 0 ) { x = _x; y = _y; };
	};
}

#endif