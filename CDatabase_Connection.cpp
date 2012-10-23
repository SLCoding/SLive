//
//  CDatabase_Connection.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CDatabase_Connection.h"

    // Konstruktor
CDatabase_Connection::CDatabase_Connection()
{
	try
	{
		LoadDefaults();
		this->connected = false;
		this->initialised = false;
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'Default-Constructor'!" << endl;
	}
}

    // Destruktor
CDatabase_Connection::~CDatabase_Connection()
{
	try
	{
		if(this->initialised == true)
        {
            PQclear(res);
            PQfinish(conn);
        }
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'Destructor'!" << endl;
	}
}

    // Speichernde Elementfunktionen
void CDatabase_Connection::setUsername(string name)
{
	username = name;
}

void CDatabase_Connection::setPassword(string passwd)
{
	password = passwd;
}

void CDatabase_Connection::setHost(string host_addr)
{
	host = host_addr;
}

void CDatabase_Connection::setDB(string db_name)
{
	db = db_name;
}

    // Wertzur¸ckgebende Elementfunktion
string CDatabase_Connection::GetUsername()
{
	if( username != "" )
		return username;
	else
		return "";
}

string CDatabase_Connection::GetPassword()
{
	if( password != "" )
		return password;
	else
		return "";
}

string CDatabase_Connection::GetHost()
{
	if( host != "" )
		return host;
	else
		return "";
}

string CDatabase_Connection::GetDB()
{
	if( db != "" )
		return db;
	else
		return "";
}

bool CDatabase_Connection::connect()
{
	try
	{
            //cout << "Baue Verbindung zur Datenbank auf ...\n\n";
            //mysql_real_connect (my, host.c_str(), username.c_str(), password.c_str(), db.c_str(), port, socket_name.c_str(), flag);
	}
	catch(...)
	{
		return false;
	}
	this->connected = true;
	return true;
}

bool CDatabase_Connection::close()
{
	try
	{
            //cout << "Verbindung wird beendet....\n\n";
		if((this->connected)&&(this->initialised))
		{
                //mysql_free_result(mysql_res);
                //mysql_close (my);
			this->connected = false;
			this->initialised = false;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CDatabase_Connection::query(string sql_query, unsigned int size)
{
	if(connected == false)
		return false;
	if(sql_query.length() != size)
	{
            //cout << "Ungueltige Laenge.\n";
		return false;
	}
    
	if(size > 1000)
	{
            //cout << "SQL Befehl zu lang.\n";
		return false;
	}
	try
	{
            //mysql_real_query(my, sql_query.c_str(),	strlen(sql_query.c_str()));
        
            //mysql_res = mysql_store_result(my);
	}
	catch(...)
	{
		return false;
	}
	this->initialised = true;
	return true;
}

map<string, string> CDatabase_Connection::fetch_assoc()
{
	map<string, string> daten;
	daten["error"] = "false";
	try
	{
		if(this->initialised == false)
			throw "No query found!";
        /*
		mysql_field_seek(mysql_res, 0); //!
		if(mysql_affected_rows(this->my) == 0)
			throw "Nothing found! Is your request right??";
		if((row = mysql_fetch_row (mysql_res)) != NULL)
		{
			mysql_field_seek (mysql_res, 0);
            
			for (i = 0; i < mysql_num_fields (mysql_res); i++)
			{
				field = mysql_fetch_field (mysql_res);
				if (row[i] == NULL)
                    daten[field->name] = "";
                
				daten[field->name] = row[i];
			}
		}*/
	}
	catch(char* error)
	{
		cerr << error << endl;
		daten["error"] = "true";
	}
	catch(...)
	{
		daten["error"] = "true";
	}
    
	return daten;
}

void CDatabase_Connection::LoadDefaults()
{
	username = "";
	password = "";
	host = "localhost";
	db = "";
}

bool CDatabase_Connection::UseDB(string db_name)
{
	try
	{
            //mysql_select_db(my, db_name.c_str());
	}
	catch(...)
	{
		return false;
	}
	db = db_name;
	return true;
}