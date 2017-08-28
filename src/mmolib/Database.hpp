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
#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include "MMOUtils.hpp"

#include <string>
#include <sqlite3.h>
#include <boost/shared_ptr.hpp>

/**
 * This class represents a wrapper over a
 * sqlite3 database connection.
 *
 * The connection is closed & freed
 * automatically when this object goes out of scope.
 */
class Database{
public:

	static Database* getInstance() 
	{ 
		if ( _instance == NULL )
			_instance = new Database;
		return _instance;
	}

	/**
	 * Opens a new database connection on the 
	 * specified path. The connection is closed 
	 * automatically when this object is destroyed
	 * 
	 * @param db_path The path to the database file
	 * @return 0 if the database was successfully opened
	 * or -1 if there was an error
	 */
	int open( const std::string& db_path) ;

	/**
	 * Executes the specified query and returns an
	 * instance to a sqlite3_stmt
	 *
	 * @param query_string The query string to be executed
	 * @return a sqlite3 statement or NULL if there was an error
	 */
	sqlite3_stmt* query( const std::string& query_string );

	/**
	 * Prepares the specified query and returns an
	 * instance to a sqlite3_stmt
	 *
	 * @param query_string The query string to be prepared
	 * @return a sqlite3 statement or NULL if there was an error
	 */
	sqlite3_stmt* prepare( const std::string& query_string );

	/**
	 * Returns the last statement executed 
	 *
	 * @return The last statement executed or NULL if none was
	 */
	sqlite3_stmt* getLastStmt() const { return _statement; }

	/**
	 * Returns the last error message from the database
	 */
	const char* getErrorMessage() const { return sqlite3_errmsg( _database ); }

	/**
	 * Returns the latest operation result
	 */
	int getLastResult() const { return _result; }

	/**
	 * Returns the current database connection opened by
	 * the 'open' call
	 *
	 * @return The current database connection or NULL if 
	 * no database was opened yet
	 */
	const sqlite3* getConnection() const { return _database; }
private:
	Database();
	~Database();
	DISALLOW_COPY_AND_ASSIGN(Database);

	static Database* _instance;

	sqlite3* _database;
	sqlite3_stmt* _statement;
	int _result;

};

#endif