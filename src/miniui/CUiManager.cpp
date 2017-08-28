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
#include "CUiManager.h"

#define MU_VERSION		0
#define MU_REVISION		1

namespace miniui
{
	CUiManager* CUiManager::instance = 0;

	int CUiManager::getVersion()  { return MU_VERSION;	}
	int CUiManager::getRevision() { return MU_REVISION; }

	bool CUiManager::useTheme( std::string filename )
	{
		if ( theme != NULL )
			delete theme;

		theme = new CTheme();
		return theme->load( filename );
	}

	void CUiManager::useCursor( bool use )
	{
		usingCursor = use;
		SDL_ShowCursor( !use );
	}

	void CUiManager::setScreenSize( int w, int h )
	{
		if ( surface != NULL )
			delete surface;

		surface = new CImage( w, h );
	}

	void CUiManager::injectInput( SDL_Event evt )
	{
		// read the mouse position
		if ( evt.type == SDL_MOUSEMOTION )
		{
			// send the event
			mousePosition.x = evt.motion.x;
			mousePosition.y = evt.motion.y;
		}
	}

	void CUiManager::update( int deltaTime )
	{
		mouseHovering = ((int)SDL_GetAppState() & SDL_APPMOUSEFOCUS) > 0;

		// draw the cursor if needed
		if ( usingCursor && mouseHovering )
			RENDERQUEUE->addImage( (*theme)[TP_CURSOR], mousePosition );

	}

	void CUiManager::update(int deltaTime, SDL_Surface *screen)
	{
		update( deltaTime );
		RENDERQUEUE->render( screen );
	}
}