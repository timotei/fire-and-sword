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
#ifndef __C_BUTTON_H__
#define __C_BUTTON_H__

#include <list>

#include "CUiManager.h"
#include "CEventListener.h"
#include "CWidget.h"
#include "CTheme.h"
#include "CRenderQueue.h"

namespace miniui
{
	class CWidget;

	enum BUTTON_STATE
	{
		STATE_NORMAL,
		STATE_HOVER,
		STATE_CLICKED,

		_STATE_SIZE
	};

	class CButton : public CWidget
	{
	friend class CUiManager;

	private:
		std::string caption;
		std::vector< CImage* > images;
		CImage* textImage;
		void repaint( );
		CRect textPosition;
		std::list< CEventListener* > events;

		BUTTON_STATE state;
		bool isMouseHovering;

		// EVENTS
		int onMouseEnter();
		int onMouseLeave();
		int onMouseMove();
		int onMouseDown();
		int onMouseUp();
		int onMouseClick();

	public:
		CButton( CWidget* parent );
		~CButton();

		void setCaption( std::string caption ){ this->caption = caption; repaint(); }
		std::string getCaption(){ return this->caption; repaint(); }

		void addEventListener( CEventListener* eventListener );
		void removeEventListener( CEventListener* eventListener );

		BUTTON_STATE getState();
		void render();

		bool handleEvent( SDL_Event& evt );
	};
}

#endif