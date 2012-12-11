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
		mysql_init(&my);
		LoadDefaults();
		this->connected = false;
		this->initialised = false;
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'Default-Constructor'!" << endl;
	}
}

// Parameter-Konstruktor
CDatabase_Connection::CDatabase_Connection(string user, string password, string DB, string Host, int Port)
{
    try
    {
        mysql_init(&my);
        LoadDefaults();
        setUsername(user);
        setPassword(password);
        setDB(DB);
        setHost(Host);
        setPort(Port);

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
		//if(this->initialised == true)
        //		mysql_free_result(mysql_res);
        //this->close();
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'Destructor'!" << endl;
	}
}

CDatabase_Connection:: CDatabase_Connection(const CDatabase_Connection& rhs)
{
    try
	{
		mysql_init(&this->my);
        //LoadDefaults();
        this->setUsername(rhs.username);
        this->setPassword(rhs.password);
        this->setDB(rhs.db);
        this->setHost(rhs.host);
        this->setPort(rhs.port);
        this->setSocket(rhs.socket_name);
        

		this->connected = false;
		this->initialised = false;

        if(rhs.connected)
            this->connect();

        this->mysql_res = NULL;
        this->field = NULL;
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'Copy-Constructor'!" << endl;
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

void CDatabase_Connection::setPort(int port_num)
{
	port = port_num;
}

void CDatabase_Connection::setSocket(string socket)
{
	socket_name = socket;
}

void CDatabase_Connection::setFlag(int flag_num)
{
	flag = flag_num;
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

int CDatabase_Connection::GetPort()
{
	return port;
}

string CDatabase_Connection::GetSocket()
{
	if( socket_name != "" )
		return socket_name;
	else
		return "";
}

int CDatabase_Connection::GetFlag()
{
	return flag;
}

bool CDatabase_Connection::connect()
{
	try
	{
        //cout << "Baue Verbindung zur Datenbank auf ...\n\n";
        mysql_init(&this->my);
		mysql_real_connect (&my, host.c_str(), username.c_str(), password.c_str(), db.c_str(), port, socket_name.c_str(), flag);
        if(this->errnum() == 0)
            this->connected = true;
        else
            return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CDatabase_Connection::close()
{
	try
	{
        //cout << "Verbindung wird beendet....\n\n";
		if((this->connected)&&(this->initialised))
		{
			mysql_free_result(mysql_res);
			mysql_close (&my);
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

bool CDatabase_Connection::query(string sql_query, unsigned long size)
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
        
        //cout << "ausgabe: " << this->my.net.last_error << endl;
        mysql_real_query(&my, sql_query.c_str(), strlen(sql_query.c_str()));

        mysql_res = mysql_store_result(&my);

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
        if(mysql_res == NULL)
            throw "Query failed!";
		mysql_field_seek(mysql_res, 0); //!
		if(mysql_affected_rows(&my) == 0)
			throw "Nothing found! Is your request right??";
		if((row = mysql_fetch_row (mysql_res)) != NULL)
		{
			mysql_field_seek (mysql_res, 0);

			for (i = 0; i < mysql_num_fields (mysql_res); i++)
			{
				field = mysql_fetch_field (mysql_res);
				if (row[i] == NULL)
                    daten[field->name] = "";
                else
                    daten[field->name] = row[i];
			}
		}
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
	port = 3306;
	socket_name = "";
	flag = 0;
}

bool CDatabase_Connection::UseDB(string db_name)
{
	try
	{
		mysql_select_db(&my, db_name.c_str());
	}
	catch(...)
	{
		return false;
	}
	db = db_name;
	return true;
}

string CDatabase_Connection::error()
{
	if (mysql_errno(&my) != 0)
	{
		return mysql_error(&my);
	}
	return "no errors occured";
}

int CDatabase_Connection::errnum()
{
	return mysql_errno(&my);
}

my_ulonglong CDatabase_Connection::affected_rows()
{
	return mysql_affected_rows(&my);
}

my_bool CDatabase_Connection::autocommit(bool mode)
{
	return mysql_autocommit(&my, mode);
}

string CDatabase_Connection::character_set_name()
{
	return mysql_character_set_name(&my);
}

my_bool CDatabase_Connection::commit()
{
	return mysql_commit(&my);
}

void CDatabase_Connection::debug(string debug)
{
	mysql_debug(debug.c_str());
}

int CDatabase_Connection::dump_debug_info()
{
	return mysql_dump_debug_info(&my);
}

unsigned long CDatabase_Connection::real_escape_string(string to, const string from, unsigned long length)
{
	unsigned long result = 0;
	try
	{
		char* zu = new char[to.length()];

		strncpy(zu, to.c_str(), strlen(to.c_str()) );
		result = mysql_real_escape_string(&my, zu, from.c_str(), length);

		delete [] zu;
	}
	catch(...)
	{
		cerr << "An unexpected error occured in function 'real_escape_string'!" << endl;
		return -1;
	}
	return result;
}

unsigned int CDatabase_Connection::field_count()
{
	return mysql_field_count(&my);
}

void CDatabase_Connection::free_result()
{
	mysql_free_result(this->mysql_res);
}

string CDatabase_Connection::get_client_info()
{
	return mysql_get_client_info();
}

unsigned long CDatabase_Connection::get_client_version()
{
	return mysql_get_client_version();
}

string CDatabase_Connection::get_host_info()
{
	return mysql_get_host_info(&my);
}

unsigned long CDatabase_Connection::get_server_version()
{
	return mysql_get_server_version(&my);
}

unsigned int CDatabase_Connection::get_proto_info()
{
	return mysql_get_proto_info(&my);
}

string CDatabase_Connection::get_server_info()
{
	return mysql_get_server_info(&my);
}

string CDatabase_Connection::info()
{
	return mysql_info(&my);
}

my_ulonglong CDatabase_Connection::insert_id()
{
	return mysql_insert_id(&my);
}

int CDatabase_Connection::kill(unsigned long pid)
{
	return mysql_kill(&my, pid);
}

unsigned int CDatabase_Connection::num_fields()
{
	return mysql_num_fields(this->mysql_res);
}

my_ulonglong CDatabase_Connection::num_rows()
{
	return mysql_num_rows(this->mysql_res);
}

int CDatabase_Connection::ping()
{
	return mysql_ping(&my);
}

int CDatabase_Connection::refresh(unsigned int options)
{
	return mysql_refresh(&my, options);
}

int CDatabase_Connection::shutdown(enum mysql_enum_shutdown_level shutdown_level)
{
	return mysql_shutdown(&my, shutdown_level);
}

string CDatabase_Connection::stat()
{
	return mysql_stat(&my);
}

unsigned long CDatabase_Connection::thread_id()
{
	return mysql_thread_id(&my);
}

unsigned int CDatabase_Connection::warning_count()
{
	return mysql_warning_count(&my);
}