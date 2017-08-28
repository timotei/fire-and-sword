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
#include "CRenderQueue.h"

namespace miniui
{
	CRenderQueue* CRenderQueue::instance = 0;

	CRenderQueue* CRenderQueue::getInstance()
	{
		if ( instance == 0 )
			instance = new CRenderQueue();
		return instance;
	}

	void CRenderQueue::addImage( CImage* img, CRect dstRect )
	{
		imageQueue.push( std::make_pair( img, dstRect ) );
	}

	void CRenderQueue::addImage( CImage* img, int x, int y )
	{
		addImage( img, CRect( x, y ) );
	}

	void CRenderQueue::render( SDL_Surface* surface )
	{
		while( !imageQueue.empty() )
		{
			std::pair< CImage*, CRect > first = imageQueue.front();
			first.first->blitTo( surface, first.second );
			imageQueue.pop();
		}
	}

	void CRenderQueue::render( CImage* image )
	{
		render( image->surface );
	}
}