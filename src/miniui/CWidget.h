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
#ifndef _C_WIDGET_H_
#define _C_WIDGET_H_

#include <list>

#include "SDL_events.h"
#include "CRect.h"
#include "CTheme.h"

namespace miniui
{
	class CWidget
	{

	private:
		CRect geometry;
		bool hasFocus;

	public:
		CWidget *parent;
		std::list< CWidget* > children;

		CWidget( CWidget* parent )
		{
			this->parent = parent;
		}

		CWidget( CWidget* parent, CRect geometry)
		{
			this->parent = parent;
			this->geometry = geometry;
		}

		virtual void render( ) = 0;
		virtual void repaint( ) = 0;

		void setGeometry( CRect rect ) { geometry = rect; }
		CRect getGeometry( ) { return geometry; }

		bool isFocused() { return hasFocus; };

		bool handleEvent( SDL_Event evt );
	};
}

#endif