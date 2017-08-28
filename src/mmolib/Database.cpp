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
#include "Database.hpp"

#include <iostream>

Database* Database::_instance = NULL;

Database::Database()
: _database(NULL)
, _statement(NULL)
, _result(NULL)
{

}

Database::~Database()
{
	if (_database != NULL){
		sqlite3_close(_database);
		_database = NULL;
	}

	if (_statement != NULL){
		sqlite3_finalize(_statement);
		_statement = NULL;
	}
}

int Database::open(const std::string& db_path)
{
	_result = sqlite3_open(db_path.c_str(), &_database);
	if (_result != SQLITE_OK){
		std::cout << "Error opening database '" << db_path << "' : " << sqlite3_errmsg(_database) << "\n";
		sqlite3_close(_database);
		return -1;
	}

	return 0;
}

sqlite3_stmt* Database::prepare(const std::string& query_string)
{
	if (_statement != NULL){
		sqlite3_finalize(_statement);
		_statement = NULL;
	}

	_result = sqlite3_prepare(_database, query_string.c_str(), query_string.size(), &_statement, NULL);
	if (_result != SQLITE_OK){
		std::cout << "Error on prepare statement (" << query_string << ") : " << sqlite3_errmsg(_database) << "\n";
		_statement = NULL;
	}

	return _statement;
}

sqlite3_stmt* Database::query(const std::string& query_string)
{
	prepare(query_string);
	_result = sqlite3_step(_statement);
	return _statement;
}