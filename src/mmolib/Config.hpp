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
#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <map>
#include <string>
#include <fstream>
#include <iostream>

/**
 This is a class that represents a simple string-base 
 key-value configuration file:

  key=value
 
 Each value is considered to be till the end of the line.
 */
class Config{

	typedef std::map<std::string, std::string> string_map;
public:
    /**
     * Creates a new config reader on the specified filename.
     * The file's contents are not yet read. 
     *
     * @see load() for reading the contents in memory
     * @param filename The filename of the config
     */
    Config(const std::string& filename);

	/**
	 * Creates a new config reader based on another
	 *
	 * @param cfg The Config to copy from
	 */
	Config(const Config& cfg);
	Config& Config::operator=(const Config& cfg);
    ~Config();

    /**
     * Saves the current config in the file.
     * 
     * @return Returns 0 if the save was successful, 1 otherwise
     */
    int save() const;

    /**
     * Loads the contents of the current config file in memory.
     * All unsaved changes are lost.
     * 
     * @return Returns 0 if the load was successful, 1 otherwise
     */
    int load();

    /**
     * Dumps the current contents to stdout
     */
    void dump() const;

    /**
     * Dumps the current contents to the specified output stream
     * 
     * @param out The stream to write to
     */
    void dump(std::ostream& out) const;

    /**
     * Returns the value for the specified key. If the key
     * doesn't exist in the config it will return an empty string
     * 
     * @param key The key name
     * @param def The default string if the key does not exist
     */
    const std::string get( const std::string& key, const std::string& def = std::string()) ;

    /**
     * Returns the value for the specified key as an integer. If the key
     * doesn't exist in the config it will return default value
     * 
     * @param key The key name
     * @param def The default integer value if the does not exist
     */
    const int get_int( const std::string& key, const int def = -1 );

    /**
     * Sets a value for the specified key
     * 
     * @param key The key name
     * @param value The string value to set on the specified key
     */
    void set(const std::string& key, const std::string& value);

    /**
     * Sets a value for the specified key
     * 
     * @param key The key name
     * @param value The integer value to set on the specified key
     */
    void set(const std::string& key, const int value);

private:
    std::string _filename;
    string_map _config_map;

};
#endif