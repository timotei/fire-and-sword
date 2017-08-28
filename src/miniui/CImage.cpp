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
#include "CImage.h"
#include "assert.h"

namespace miniui
{

	CImage::CImage()
	{
		initImage();
	}

	CImage::CImage( SDL_Surface* sdl_surface )
	{
		initImage();
		surface = sdl_surface;
	}

	CImage::CImage( std::string filename )
	{
		initImage();
		surface = IMG_Load( filename.c_str() );

		if ( isNull() )
			return;

		surface->flags -= SDL_SRCALPHA;
	}

	CImage::CImage( int width, int height )
	{
		initImage();
		surface = SDL_CreateRGBSurface
		(
			SDL_SWSURFACE | SDL_SRCALPHA, 
			width, 
			height, 
			32, 
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF, 
			0xFF000000 
		);
		//surface = SDL_DisplayFormatAlpha( surface );
		surface->flags -= SDL_SRCALPHA;
	}

	CImage::CImage( const CImage& img ) 
	{
		initImage();

		// copy the surface;
		surface = SDL_CreateRGBSurface
		(
			SDL_SWSURFACE | SDL_SRCALPHA, 
			img.surface->w, 
			img.surface->h, 
			32, 
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF, 
			0xFF000000 
		);
		SDL_BlitSurface( img.surface, 0, surface, 0 );
	}

	CImage& CImage::operator =( const CImage& img)
	{
		if ( this == &img )
			return *this;

		if ( surface != NULL )
			SDL_FreeSurface( surface );
		initImage();

		// copy the surface;
		surface = SDL_CreateRGBSurface
			(
			SDL_SWSURFACE | SDL_SRCALPHA, 
			img.surface->w, 
			img.surface->h, 
			32, 
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF, 
			0xFF000000 
			);
		SDL_BlitSurface( img.surface, 0, surface, 0 );

		return *this;
	}

	void CImage::initImage()
	{
		isDirty = false;
		surface = NULL;
	}

	CImage::~CImage()
	{
		SDL_FreeSurface( surface );
		surface = NULL;
	}

	int CImage::width()
	{
		return isNull() ? 0 : surface->w;
	}

	int CImage::height()
	{
		return isNull() ? 0 : surface->h;
	}

	CImage* CImage::copy( CRect area )
	{
		CImage* subImage = new CImage( area.w, area.h );

		SDL_Rect dRect; // destination rect

		// destination position
		dRect.x = 0;
		dRect.y = 0;

		SDL_Rect* src = area.getSDLRect();

		SDL_BlitSurface( surface, src, subImage->surface, &dRect );

		SDL_free( src );
		return subImage;
	}

	CImage* CImage::copy( int x, int y, int w, int h )
	{
		return copy( CRect( x, y, w, h ) );
	}

	void CImage::blit( SDL_Surface* srcSurface, CRect dstRect, CRect srcRect )
	{
		if ( isNull() )
			return;

		if ( !isDirty )
		{
			SDL_SetAlpha( surface, SDL_SRCALPHA, 0xFF );
			isDirty = true;
		}
		SDL_SetAlpha( srcSurface, 0, 0xFF );

		SDL_Rect* src = srcRect.getSDLRect();
		SDL_Rect* dst = dstRect.getSDLRect();

		SDL_BlitSurface( srcSurface, src, surface, dst );

		SDL_free( src );
		SDL_free( dst );
	}

	void CImage::blit( CImage& srcImage, CRect dstRect, CRect srcRect )
	{
		blit( srcImage.surface, dstRect, srcRect );
	}

	void CImage::blitTo( SDL_Surface* dstSurface, CRect dstRect, CRect srcRect )
	{
		if ( isNull() )
			return;

		SDL_Rect* src = srcRect.getSDLRect();
		SDL_Rect* dst = dstRect.getSDLRect();
		this->surface->flags |= SDL_SRCALPHA;

		SDL_BlitSurface( surface, src, dstSurface, dst );

		SDL_free( src );
		SDL_free( dst );
	}

	void CImage::clear()
	{
		if ( !isDirty )
			return;

		SDL_FillRect( surface, 0, 0x00000000 );
		isDirty = false;
	}

	bool CImage::convert( SDL_Surface* other )
	{
		// try to convert the surface
		SDL_Surface* newSurface = SDL_ConvertSurface( surface, other->format, other->flags );

		if ( newSurface )
		{
			SDL_Surface* oldSurface = surface;
			surface = newSurface;
			SDL_FreeSurface( oldSurface );
			return 1;
		}

		SDL_FreeSurface( newSurface );
		return 0;
	}

	bool CImage::convert( const CImage& other )
	{
		return convert( other.surface );
	}

	CImage* CImage::fill(int color)
	{
		SDL_FillRect( surface, NULL, color );
		return this;
	}

	CImage* CImage::fill(CImage* other, FILL_DIRECTION direction )
	{
		if ( direction && FILL_HORIZONTAL == 0 && direction && FILL_VERTICAL == 0 )
			return this;

		int fillX = 0;
		int fillY = 0;

		int stepX = ( direction & FILL_HORIZONTAL ? other->width()  : this->width() );
		int stepY = ( direction & FILL_VERTICAL   ? other->height() : this->height() );


		while ( fillX < width() )
		{
			fillY = 0;
			while ( fillY <= height() )
			{
				this->blit( *other, CRect(fillX, fillY) );
				fillY += stepY;
			}
			fillX += stepX;
		}

		return this;
	}

	bool CImage::isNull()
	{
		return surface == 0;
	}

	CRect CImage::getBoundingRect()
	{
		return CRect( 0, 0, width(), height() );
	}
}