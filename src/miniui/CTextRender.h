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
#ifndef _C_TEXT_RENDER_H_
#define _C_TEXT_RENDER_H_

#include "iostream"
#include "string"

#include "SDL.h"
#include "SDL_image.h"

#include "CImage.h"
#include "CRect.h"

#define FONT_GLYPHS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?;,.:<>()[]{}+-*/=#$%^&"


namespace miniui
{
	class CTextRender
	{
	private:
		CImage* font;
		//SDL_Surface* font;
		int fontSize;
		std::string fontLetters;

	public:
		CTextRender( std::string bitmapFont );

		CImage* render( std::string text, int outline = 1 );

		CImage* getFont();

		~CTextRender();

	};
}

#endif