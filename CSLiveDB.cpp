//
//  CSLiveDB.cpp
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CSLiveDB.h"

CSLiveDB::CSLiveDB(string user, string password, string DB, string Host, int port)
{
    this->dbconn = CDatabase_Connection(user, password, DB, Host, port);
    dbconn.connect();
}



//"Search in Database" - methods
bool CSLiveDB::checkUsername(string name)
{
    string query = "SELECT * FROM user WHERE name LIKE '" + name + "';";
    dbconn.query(query, query.length());
    if(dbconn.affected_rows() > 0)
        return false;
    else 
        return true;
    
}

map<string, string> CSLiveDB::getUserById(string id)
{
    string query = "SELECT * FROM user WHERE id = '" + id + "';";
    dbconn.query(query, query.length());
    map<string, string> result;
    result = dbconn.fetch_assoc();
    
    return result;
}
map<string, string> CSLiveDB::getUserByName(string name)
{
    string query = "SELECT * FROM user WHERE name LIKE '" + name + "';";
    dbconn.query(query, query.length());
    map<string, string> result;
    result = dbconn.fetch_assoc();
    
    return result;
}

list<map<string, string> > CSLiveDB::getBdylist(string id)
{
    string query = "SELECT * FROM buddy WHERE id = " + id + ";";
    dbconn.query(query, query.length());
    
}
map<string, string> CSLiveDB::getBdyByName(string id, string name)
{


}

map<string, string> CSLiveDB::getConferenceById(string id)
{

}
list<map<string, string> > CSLiveDB::getConferenceByUser(string id)
{
    
    
}

list<map<string, string> > CSLiveDB::getLogByUser(string id)
{
    
}
map<string, string> CSLiveDB::getLogById(string id)
{
    
}


//"create entry" - methods

map <string, string> CSLiveDB::createUser(string name, string email, string password)
{
    
}
bool CSLiveDB::createLog(string conf_id, string time, string log)
{
    
}
bool CSLiveDB::createConference()
{
    
}

bool CSLiveDB::add_Bdy(string usr_id, string bdy_id)
{
    
}
bool CSLiveDB::add_usr(string conf_id, string usr_id)
{
    

}
//"remove entry" - methods
bool CSLiveDB::removeUser(string usr_id)
{
    
}
bool CSLiveDB::removeBdy(string usr_id, string bdy_id)
{
    
}
bool CSLiveDB::removeUserConf(string conf_id, string usr_id)
{
    
}