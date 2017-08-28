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
#ifndef _C_IMAGE_H_
#define _C_IMAGE_H_

#include "string"
#include "iostream"

#include "SDL.h"
#include "SDL_image.h"

#include "CRect.h"

namespace miniui
{
	enum FILL_DIRECTION
	{
		FILL_HORIZONTAL = 1,
		FILL_VERTICAL
	};

	class CImage
	{
	private:
		bool isDirty;

	public:
		SDL_Surface* surface;

		CImage();
		CImage( std::string filename );
		CImage( int width, int height );
		CImage( const CImage& img );
		CImage( SDL_Surface* surface );

		CImage& CImage::operator=( const CImage& img);
		void initImage();

		~CImage();

		int width();
		int height();
		CRect getBoundingRect();

		bool isNull();

		CImage* copy( int x, int y, int w, int h );
		CImage* copy( CRect area );

		void blit( SDL_Surface* srcSurface, CRect dstRect = CRect(), CRect srcRect = CRect() );
		void blit( CImage& srcImage,        CRect dstRect = CRect(), CRect srcRect = CRect() );
		//void blit( CImage* srcImage,        CRect dstRect = CRect(), CRect srcRect = CRect() );

		void blitTo( SDL_Surface* dstSurface, CRect dstRect = CRect(), CRect srcRect = CRect() );

		void clear();
		bool convert( SDL_Surface* other );
		bool convert(const CImage& other );

		CImage* fill( int color );
		CImage* fill( CImage* other, FILL_DIRECTION direction );

		int color( int r, int g, int b );
		int color( int r, int g, int b, int a );
	};
}

#endif