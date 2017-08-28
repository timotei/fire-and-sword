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
#ifndef _C_RENDER_QUEUE_H_
#define _C_RENDER_QUEUE_H_

#define RENDERQUEUE miniui::CRenderQueue::getInstance()

#include <queue>

#include "SDL.h"

#include "CImage.h"
#include "CRect.h"

namespace miniui
{
	class CRenderQueue
	{
	private:
		static CRenderQueue *instance;
		CRenderQueue() { };
		std::queue< std::pair< CImage*, CRect > > imageQueue;

	public:
		static CRenderQueue* getInstance();
		void addImage( CImage*, int x, int y );
		void addImage( CImage*, CRect dstRect = CRect() );

		void render( CImage * );
		void render( SDL_Surface* );
	};
}

#endif