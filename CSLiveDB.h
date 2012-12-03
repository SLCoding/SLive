//
//  CSLiveDB.h
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef SLive_CSLiveDB_h
#define SLive_CSLiveDB_h

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <sstream>
#include "CDatabase_Connection.h"
using namespace std;





class cConference;
class cUser;




class CSLiveDB
{
    friend class cUser;
    friend class cConference;
    
private:
    CDatabase_Connection dbconn;
    
    
protected:
    
    
public:
    CSLiveDB();
    CSLiveDB(string user, string password, string DB, string Host = "127.0.0.1", int port = 3306);
    
    
    
    cConference create_conf();
    cConference create_conf(string id);
    cConference create_conf(string id, list<cUser> usr_list);
    
    cConference get_Conf(string id);
    
    
    
    bool checkUsername(string name);
    
    cUser create_User(string name);
    cUser create_User(string name, string email);
    cUser create_User(int id, string name);
    cUser create_User(int id, string name, string email);
    
    cUser get_User(int id);
    cUser get_User(string name);
};




class cUser
{
    friend class CSLiveDB;
private:
    cUser(CSLiveDB db);
    cUser(CSLiveDB db, int id, string name, string pwhash, string email);
    cUser(CSLiveDB db, int id, string name, string pwhash, string email, list<cConference> conf_list, list<cUser> bdy_list);
    
    
    CSLiveDB db;
    
    int id;
    string name;
    string pwhash;
    string email;
    string server;
    list<cConference> conf_list;
    list<cUser> bdy_list;    
    
public:
    
    
    
    int get_id();
    string get_name();
    string get_email();
    string get_server();
    list<cConference> get_confList();
    list<cUser> get_bdyList();
    
    bool set_id(int id);
    bool set_name(string name);
    bool set_email(string email);
    bool set_server(string server);
    
    
    bool add_conf(string conf_id);
    bool add_conf(cConference conf);
    
    bool add_bdy(int bdy_id);
    bool add_bdy(cUser bdy);
    
    
    bool del_conf(string conf_id);
    bool del_conf(cConference conf);
    
    bool del_bdy(int bdy_id);
    bool del_bdy(cUser bdy);
    
    bool del_user();
    
};



class cConference
{
    friend class CSLiveDB;
private:
    cConference(CSLiveDB db);
    cConference(CSLiveDB db, string id, list<cUser> usr_list);
    
    
    CSLiveDB db;
    
    string id;
    list<cUser> usr_list;
    
    
    
public:
    string get_id();
    list<cUser> get_usrList();
    
    bool set_id(string id);
    //bool set_usrList(list<cUser>);
    
    
    bool add_usr(int usr_id);
    bool add_usr(cUser usr);
    
    bool del_usr(int usr_id);
    bool del_usr(cUser usr);
    
    bool del_conf();
    
};



#endif
