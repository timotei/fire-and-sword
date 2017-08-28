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
#ifndef MMOUTILS_HPP_INCLUDED
#define MMOUTILS_HPP_INCLUDED

#include <string>
#include <vector>
#include "SDL_net.h"

#define foreach         BOOST_FOREACH

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

/**
 * The message content max length
 */
static const int MSG_CONTENT_LEN = 1024;

/**
 * The default port where the server runs
 */
static const int SERVER_DEFAULT_PORT = 1600;

static const int MAP_MAX_WIDTH = 20;
static const int MAP_MAX_HEIGHT = 20;

static const int MAP_OBJECT_WIDTH = 32;
static const int MAP_OBJECT_HEIGHT = 32;

/**
 * Converts the specified IPv4 address to a string representation
 * 
 * @param addr The 32bit integer IPv4 to convert
 * 
 * @return A string representation in the form of IPv4 format: %d.%d.%d.%d
 */
const std::string to_string_ip(const Uint32& addr);

/**
 * Splits the specified string using the specified character
 *
 * @param val The string to split
 * @param c The character to split by
 * @return A string vector with the tokens
 */
std::vector< std::string > split_string (std::string const &val, char c = ';');

static const int INT_SIZE = sizeof(int);
static const int INT_DOUBLE_SIZE = 2 * sizeof(int);
static const int INT_TRIPLE_SIZE = 3 * sizeof(int);
static const int SIZE_T_SIZE = sizeof(size_t);
#endif