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
#ifndef _C_UI_MANAGER_H_
#define _C_UI_MANAGER_H_

#define MINIUI miniui::CUiManager::getInstance()

#include <string>
#include <iostream>

#include "CTheme.h"
#include "CImage.h"
#include "CPoint.h"
#include "CTextRender.h"
#include "CRenderQueue.h"

// Widgets
#include "CWidget.h"
#include "CButton.h"

namespace miniui
{
	class CUiManager
	{
	private:
		CTheme* theme;
		bool usingCursor;
		CImage* surface;

		CRect mousePosition;
		bool mouseHovering;

		static CUiManager* instance;
		CUiManager()
			: theme( NULL )
			, usingCursor( false )
			, surface ( NULL )
			, mouseHovering ( false )
		{ };

		~CUiManager()
		{
			if ( theme != NULL ) 
				delete theme;
			if ( surface != NULL )
				delete surface;
		}

	public:

		static CUiManager* getInstance()
		{
			if ( ! instance ) 
				instance = new CUiManager();
			return instance;
		}

		int getVersion();
		int getRevision();
		//bool useTheme( CTheme theme );
		bool useTheme( std::string filename );
		CTheme* getTheme() { return theme; };

		void setScreenSize( int w, int h );
		void useCursor( bool use );

		void injectInput( SDL_Event evt );

		void update( int deltaTime );
		void update( int deltaTime, SDL_Surface* screen );

	};
}

#endif