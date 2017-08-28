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
#include "CButton.h"

#ifndef max
#define max( a, b ) ((a > b)?(a):(b))
#endif 

namespace miniui
{
	CButton::CButton( CWidget* parent )
		: CWidget( parent )
	{
		caption = "";
		state = STATE_NORMAL;

	}

	CButton::~CButton()
	{
	}

	void CButton::repaint(  )
	{
		CTheme* theme = MINIUI->getTheme();

		if ( images.size() > 0 )
			images.clear();

		for ( int i = 0 ; i < _STATE_SIZE; i++ )
			images.push_back( 0 );

		CImage* text = theme->getTextRenderer()->render(caption);

		int w = 64;
		int h = 22;
		int m = max( text->width() - 30, 0 );
		int tx = ( ( w + m ) - text->width() ) / 2;
		int ty = ( h - text->height() ) / 2;
		textPosition = CRect( tx, ty );

		// STATE_NORMAL
		CImage* sNormal = new CImage( w + m, h );
		sNormal->blit( *(theme->getPart(TP_BUTTON_NORMAL_LEFT)), CRect(0, 0) );
		sNormal->blit( *(CImage(m, h).fill( theme->getPart(TP_BUTTON_NORMAL_MIDDLE), FILL_HORIZONTAL )), CRect( w / 2, 0 ) );
		sNormal->blit( *(theme->getPart(TP_BUTTON_NORMAL_RIGHT)), CRect( w / 2 + m, 0 ) );

		// STATE_CLICKED
		CImage* sClicked = new CImage( w + m, h );
		sClicked->blit( *(theme->getPart(TP_BUTTON_CLICKED_LEFT)), CRect(0, 0) );
		sClicked->blit( *(CImage(m, h).fill( theme->getPart(TP_BUTTON_CLICKED_MIDDLE), FILL_HORIZONTAL )), CRect( w / 2, 0 ) );
		sClicked->blit( *(theme->getPart(TP_BUTTON_CLICKED_RIGHT)), CRect( w / 2 + m, 0) );

		// STATE_HOVERED
		CImage* sHovered = 0;

		images[STATE_NORMAL] = sNormal;
		images[STATE_CLICKED] = sClicked;
		images[STATE_HOVER] = sHovered;

		setGeometry( CRect( getGeometry().x, getGeometry().y, w + m, h ) );
		textImage = theme->getTextRenderer()->render(caption);
	}

	void CButton::render()
	{
		RENDERQUEUE->addImage( images[state], getGeometry() );
		if ( state == STATE_NORMAL )
			RENDERQUEUE->addImage( textImage, textPosition + getGeometry() );
		else
			RENDERQUEUE->addImage( textImage, textPosition + getGeometry() + CRect( -1, 2 ) );
	}

	// EVENTS

	void CButton::addEventListener( CEventListener* eventListener )
	{
		events.push_back( eventListener );
	}

	void CButton::removeEventListener( CEventListener* eventListener )
	{
		events.remove( eventListener );
	}

	bool CButton::handleEvent( SDL_Event& evt )
	{
		bool handled = CWidget::handleEvent( evt );

		if ( handled )
			return true;

		CRect mousePos;

		if ( evt.type == SDL_MOUSEMOTION || evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP )
			mousePos = CRect( evt.motion.x, evt.motion.y );

		if ( evt.type == SDL_MOUSEMOTION )
		{
			// on mouse enter
			if ( ! isMouseHovering && getGeometry().contains(mousePos) )
				return onMouseEnter();

			// on mouse move
			if ( isMouseHovering && getGeometry().contains(mousePos) )
				return onMouseMove();

			// on mouse leave
			if ( isMouseHovering && !getGeometry().contains(mousePos) )
				return onMouseLeave();
		}

		if ( evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT && getGeometry().contains(mousePos) )
			return onMouseDown();

		if ( evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT && getGeometry().contains(mousePos) )
			return onMouseUp();
	}

	int CButton::onMouseEnter()
	{
		isMouseHovering = true;
		//state = STATE_HOVER;
		// fire the event

		return 1;
	}

	int CButton::onMouseMove()
	{
		// fire the event

		return 1;
	}

	int CButton::onMouseLeave()
	{
		isMouseHovering = false;
		state = STATE_NORMAL;
		// fire the event

		return 0;
	}

	int CButton::onMouseDown()
	{
		state = STATE_CLICKED;
		// fire the event
 
		return 1;
	}

	int CButton::onMouseUp()
	{
		if ( state == STATE_CLICKED )
		{
			state = STATE_NORMAL;
			onMouseClick();
		}
		// fire the event

		return 1;
	}

	int CButton::onMouseClick()
	{
		// fire the event
		for ( std::list< CEventListener* >::iterator it = events.begin(); it != events.end(); ++it )
			(*it)->onMouseClick(  this  );
		return 1;
	}
}