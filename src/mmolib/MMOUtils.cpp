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
#include "MMOUtils.hpp"
#include <sstream>

const std::string to_string_ip(const Uint32& addr)
{
	std::ostringstream out;
	const Uint32 tmpAddr = SDLNet_Read32(&addr);
	out << (tmpAddr >> 24) << '.'
		<< ((tmpAddr >> 16) & 0xff) << '.'
		<< ((tmpAddr >> 8) & 0xff) << '.'
		<< (tmpAddr & 0xff);
	return out.str();
}


std::vector< std::string > split_string(std::string const &val, char c)
{
	std::vector< std::string > res;

	std::string::const_iterator i1 = val.begin();
	std::string::const_iterator i2 = val.begin();

	while (i2 != val.end()) {
		if (*i2 == c) {
			std::string newVal( i1, i2 );
			if ( newVal.empty() == false )
				res.push_back( newVal );
			++i2;
			i1 = i2;
		} else {
			++i2;
		}
	}

	std::string newVal( i1, i2 );
	if ( newVal.empty() == false )
		res.push_back( newVal );

	return res;
}
