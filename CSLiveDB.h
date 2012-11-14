//
//  CSLiveDB.h
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SLive_CSLiveDB_h
#define SLive_CSLiveDB_h

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "CDatabase_Connection.h"
using namespace std;

class CSLiveDB
{
private:
    CDatabase_Connection dbconn;
    
    
protected:
    
    
public:
    CSLiveDB(string user, string password, string DB, string Host = "127.0.0.1", int port = 3306);
    
    
    
    //"Search in Database" - methods
    bool checkUsername(string name);
    
    map<string, string> getUserById(string id);
    map<string, string> getUserByName(string name);
    
    
    list<map<string, string> > getBdylist(string id);
    map<string, string> getBdyByName(string id, string name);
    
    map<string, string> getConferenceById(string id);
    list<map<string, string> > getConferenceByUser(string id);
    
    list<map<string, string> > getLogByUser(string id);
    map<string, string> getLogById(string id);
    
    
    //"create entry" - methods
    
    map<string, string> createUser(string name, string email, string password);
    bool createLog(string conf_id, string time, string log);
    bool createConference();
    
    bool add_Bdy(string usr_id, string bdy_id);
    bool add_usr(string conf_id, string usr_id);
    
 
    //"remove entry" - methods
    bool removeUser(string usr_id);
    bool removeBdy(string usr_id, string bdy_id);
    bool removeUserConf(string conf_id, string usr_id);
    
    
};

#endif
