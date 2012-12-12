//
//  CSLiveDB.h
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//


//logging
//---offline nachrichten




#ifndef SLive_CSLiveDB_h
#define SLive_CSLiveDB_h

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <sstream>
#include <stdlib.h>
#include "CDatabase_Connection.h"
#include "md5.h"
using namespace std;


class cConference;
class cUser;

struct dbError
{
    int errnum;
    string errstr;
};


class CSLiveDB
{
    friend class cUser;
    friend class cConference;
    
private:
    //CDatabase_Connection dbconn;
    
    
protected:
    
    
public:
    CSLiveDB();
    CSLiveDB(string user, string password, string DB, string Host = "127.0.0.1", string Host2 = "127.0.0.1", int port = 3306, int port2 = 3306);
    ~CSLiveDB();
    
    
    string user;
    string password;
    string DB;
    string Host;
    string Host2;
    int port;
    int port2;
    
    
    cConference create_conf(string name);
    cConference create_conf(list<cUser> usr_list, string name = "");
    //cConference create_conf(string name, list<cUser> usr_list);
    
    cConference get_Conf(string id);
    
    
    
    bool checkUsername(string name);
    
    cUser create_User(string name, string pw);
    cUser create_User(string name, string pw, string email);
    
    cUser get_User(long id);
    cUser get_User(string name);
    list<cUser> search_User(string name);
    
    cUser login(string name, string pw, string server);
    cUser login(long id, string pw, string server);
};



enum user_status {OFFLINE, BUSY, AWAY, ONLINE};

class cUser
{
    friend class CSLiveDB;
private:
    cUser(CSLiveDB db);
    cUser(CSLiveDB db, long id, string name, string pwhash, string email);
    cUser(CSLiveDB db, long id, string name, string pwhash, string email, list<cConference> conf_list, list<cUser> bdy_list);
    
    
    CSLiveDB db;
    
    long id;
    string name;
    string pwhash;
    string email;
    string server;
    //list<cConference> conf_list;
    //list<cUser> bdy_list;    
    
public:
    cUser() {;}
    ~cUser();
    
    long get_id();
    string get_name();
    string get_email();
    string get_server();
    user_status get_status();
    list<cConference> get_confList();
    list<cUser> get_bdyList();
    
    bool set_id(long id);
    bool set_name(string name);
    bool set_email(string email);
    bool set_server(string server);
    bool set_status(user_status status);
    
    bool logout();
    
    bool add_conf(string conf_id);
    bool add_conf(cConference conf);
    
    bool add_bdy(long bdy_id);
    bool add_bdy(cUser bdy);
    
    
    bool del_conf(string conf_id);
    bool del_conf(cConference conf);
    
    bool del_bdy(long bdy_id);
    bool del_bdy(cUser bdy);
    
    bool del_user();
    
};



class cConference
{
    friend class CSLiveDB;
private:
    cConference(CSLiveDB db);
    cConference(CSLiveDB db, string id);
    cConference(CSLiveDB db, string id, list<cUser> usr_list);
    cConference(CSLiveDB db, list<cUser> usr_list);
    
    CSLiveDB db;
    
    string id;
    //list<cUser> usr_list;
    
    
    
public:
    ~cConference();
    
    string get_id();
    list<cUser> get_usrList();
    
    bool set_id(string id);
    //bool set_usrList(list<cUser>);
    
    
    bool add_usr(long usr_id);
    bool add_usr(cUser usr);
    
    bool del_usr(long usr_id);
    bool del_usr(cUser usr);
    
    bool del_conf();
    
};



#endif
