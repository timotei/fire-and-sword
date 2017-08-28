/*
   Copyright (C) 2011 by Timotei Dolean <timotei21@gmail.com>

   Part of the Fire and Sword Project http://code.google.com/p/fire-and-sword/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef MMOTYPES_HPP_INCLUDED
#define MMOTYPES_HPP_INCLUDED

#include <vector>

enum CharacterClass : int {
	CLASS_NONE		= 0,
	CLASS_WIZARD	= 1,
	CLASS_WARRIOR	= 2
};

template< typename T > 
struct Matrix 
{ 
	Matrix( unsigned m, unsigned n ) 
		: _m ( m )
		, _contents( m * n ) {} 
	
	T& operator () ( unsigned i, unsigned j ) { return _contents[ i + _m * j ]; }
	T* operator () () const { return &_contents[0]; }

	const std::vector< T >& contents() const { return _contents; }

private: 
	unsigned _m; 
	std::vector< T > _contents; 
};

#endif