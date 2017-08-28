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
#include "Config.hpp"

Config::Config(const std::string& filename)
: _filename(filename)
{
}

Config::Config(const Config& cfg)
: _filename(cfg._filename)
, _config_map(cfg._config_map)
{

}

Config& Config::operator =(const Config& cfg)
{
	if (this != &cfg){
		_filename = std::string(cfg._filename);
		_config_map = string_map(cfg._config_map);
	}
	return *this;
}

Config::~Config()
{
}

int Config::load()
{
    _config_map.clear();

    char buf[1025];
	std::ifstream fin(_filename.c_str());

    if (fin == false){
        std::cerr << "Could not open the config file: " << _filename << "\n";
        return 1;
    }
    
    while(fin.eof() == false){
        fin.getline(buf, 1024);

        std::string line(buf);
        size_t index = line.find('=');

        // split by =
        if (index != std::string::npos){
            _config_map[line.substr(0, index)] = line.substr(index + 1);
        }
    }

    fin.close();
    return 0;
}

int Config::save() const
{
	std::ofstream fout(_filename.c_str());

    if (fout == false){
        std::cerr << "There was an error trying to save the config file.\n";
        return -1;
    }

    dump(fout);
    fout.close();
    return 0;
}

const std::string Config::get(const std::string& key, const std::string& def)
{
    // didn't found anything
    if (_config_map.find(key) == _config_map.end())
        return def;

	return _config_map[key];
}

const int Config::get_int(const std::string& key, const int def)
{
    // didn't found anything
    if (_config_map.find(key) == _config_map.end())
        return def;

	return atoi(_config_map[key].c_str());
}

void Config::set(const std::string& key, const std::string& value)
{
    _config_map[key] = value;
}

void Config::set(const std::string& key, const int value)
{
    _config_map[key] = value;
}

void Config::dump(std::ostream& out) const
{
    for(string_map::const_iterator it = _config_map.begin(); it != _config_map.end(); ++it){
        out << it->first << "=" << it->second << "\n";
    }

    out << "\n";
}

void Config::dump() const
{
	dump(std::cout);
}
