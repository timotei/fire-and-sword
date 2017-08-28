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
#ifndef _C_THEME_H_
#define _C_THEME_H_

#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

#include "CImage.h"
#include "CTextRender.h"

enum THEME_PARTS
{
	TP_CURSOR,

	// Button parts
	TP_BUTTON_NORMAL_LEFT,
	TP_BUTTON_NORMAL_MIDDLE,
	TP_BUTTON_NORMAL_RIGHT,

	TP_BUTTON_CLICKED_LEFT,
	TP_BUTTON_CLICKED_MIDDLE,
	TP_BUTTON_CLICKED_RIGHT,

	_TP_SIZE
};

namespace miniui
{
	class CTheme
	{
	private:
		std::vector< CImage* > parts;
		CImage* themeImage;
		CTextRender* textRenderer;

	public:

		bool load( std::string filename );

		CImage* operator[]( THEME_PARTS part );
		CImage* getPart( THEME_PARTS part );
		CTextRender* getTextRenderer() { return textRenderer; };
	};
}

#endif