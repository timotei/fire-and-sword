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
#include "CTextRender.h"

namespace miniui
{

	CTextRender::CTextRender( std::string bitmapFont )
	{
		font = new CImage( bitmapFont );
		fontLetters = std::string ( FONT_GLYPHS ); 
		fontSize = font->surface->h / 2;
	}

	CImage* CTextRender::render( std::string text, int outline )
	{
		int width = fontSize * text.length();
		int height = fontSize;

		CImage* textSurface = new CImage( width, height );
		textSurface->convert( *font );

		// blit the letters
		for ( size_t i = 0; i < text.length(); i++ )
		{
			char c = text.at(i);

			if ( 'a' <= c && c <= 'z' ) c -= 32;

			int glyphIndex = fontLetters.find(c);
			if ( glyphIndex == -1 )
				glyphIndex =  60;

			CRect sRect;
			CRect dRect;

			// source rect
			sRect.w = sRect.h = fontSize;
			sRect.x = glyphIndex * fontSize;
			sRect.y = (1 - outline ) * fontSize;

			// destination position
			dRect.x = i * fontSize;
			dRect.y = 0;

			textSurface->blit( *font, dRect, sRect );
		}

		return textSurface;
	}

	CImage* CTextRender::getFont()
	{
		return font;
	}

	CTextRender::~CTextRender()
	{
		
	}
}