//
//  CSLiveDB.cpp
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CSLiveDB.h"
#include <stdlib.h>
#include <stdio.h>






cUser::cUser(CSLiveDB db)
{
    this->db = db;
}

cUser::cUser(CSLiveDB db, long id, string name, string pwhash, string email)
{
    this->db = db;
    this->id = id;
    this->name = name;
    this->pwhash = pwhash;
    this->email = email;
}
/*

cUser::cUser(CSLiveDB db, long id, string name, string pwhash, string email, list<cConference> conf_list, list<cUser> bdy_list)
{
    this->db = db;
    this->db.dbconn.connect();
    this->id = id;
    this->name = name;
    this->pwhash = pwhash;
    this->email = email;
    //this->conf_list = conf_list;
    //this->bdy_list = bdy_list;
}
*/

cUser::~cUser()
{
    this->db.dbconn.close();
       
}


long cUser::get_id()
{
    return this->id;
}
string cUser::get_name()
{
    return this->name;
}
string cUser::get_email()
{
    return this->email;
}
string cUser::get_server()
{
    stringstream query;
    query<<"SELECT server FROM user WHERE user_id = "<<this->id<<";";
    
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    map<string, string> result = this->db.dbconn.fetch_assoc();
    return result["server"].c_str();
}
list<cConference> cUser::get_confList()
{
    CDatabase_Connection db;
    db.setDB("SLive2");
    db.setHost("10.12.45.253");
    db.setPassword("SLive2");
    db.setUsername("SLive2");
    db.connect();
    stringstream query;
    query << "SELECT * FROM conf_user WHERE user_id = " << this->id << ";";
    db.query(query.str(), query.str().length());
    
    if(db.errnum() != 0)
        throw db.error();
    
    list<cConference> conf_list;
    map<string, string> result;
    int rows = db.affected_rows();
    for(int i = 0; i < db.affected_rows(); i++)
    {
        result = db.fetch_assoc();
        conf_list.push_front(this->db.get_Conf(result["conf_id"]));
    }
    db.close();
    return conf_list;
}
list<cUser> cUser::get_bdyList()
{
    CDatabase_Connection db;
    db.setDB("SLive2");
    db.setHost("10.12.45.253");
    db.setPassword("SLive2");
    db.setUsername("SLive2");
    db.connect();
    
    stringstream query;
    query<<"SELECT * FROM buddy WHERE user_id = "<<this->get_id()<<";";
    
    //this->db.dbconn.query(query.str(), query.str().length());
    db.query(query.str(), query.str().length());
    /*if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    */
    list<cUser> bdy_list;
    /*
    for(int i = 0; i < this->db.dbconn.affected_rows(); i++)
    {
        map<string, string> result = this->db.dbconn.fetch_assoc();
        bdy_list.push_front(this->db.get_User(atoi((result["bdy_id"]).c_str())));
    }
*/
    for(int i = 0; i < db.affected_rows(); i++)
    {
        map<string, string> result = db.fetch_assoc();
        bdy_list.push_front(this->db.get_User(atoi((result["bdy_id"]).c_str())));
    }
    db.close();
    return bdy_list;
}



user_status cUser::get_status()
{
    stringstream query;
    query<<"SELECT status FROM user WHERE user_id = "<<this->id<<";";
    
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    map<string, string> result = this->db.dbconn.fetch_assoc();
    return (user_status)atoi(result["status"].c_str());
    
}





bool cUser::set_id(long id)
{
    stringstream query;
    query<<"UPDATE user SET user_id = " << id << " WHERE user_id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    this->id = id;
    return true;
}
bool cUser::set_name(string name)
{
    stringstream query;
    
    query<<"UPDATE user SET name = '" << name << "' WHERE user_id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    this->name = name;
    
    return true;
}
bool cUser::set_email(string email)
{
    stringstream query;
    
    query<<"UPDATE user SET email = '" << email << "' WHERE user_id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    this->email = email;
    
    return true;
}
bool cUser::set_server(string server)
{
    stringstream query;
    
    query<<"UPDATE user SET server = '" << server << "' WHERE user_id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    this->server = server;
    
    return true;
}


bool cUser::set_status(user_status status)
{
    stringstream query;
    
    query<<"UPDATE user SET status = '" << (int)status << "' WHERE user_id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    return true;
}




bool cUser::logout()
{
    this->set_server("");
    this->set_status(OFFLINE);
    return true;
    
}







bool cUser::add_conf(string conf_id)
{
    stringstream query;
    
    query<<"INSERT INTO conf_user(conf_id, user_id) VALUES('" << conf_id << "', " << this->id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    return true;
} 


bool cUser::add_conf(cConference conf)
{
    return this->add_conf(conf.get_id());
    
}

bool cUser::add_bdy(long bdy_id)
{
    stringstream query;
    
    query<<"INSERT INTO buddy(user_id, bdy_id) VALUES(" << this->id << ", " << bdy_id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    return true;
}
bool cUser::add_bdy(cUser bdy)
{
    return add_bdy(bdy.get_id());
    
}


bool cUser::del_conf(string conf_id)
{    
    stringstream query;
    
    query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< conf_id <<"'* AND user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    return true;
    
}
bool cUser::del_conf(cConference conf)
{
    return this->del_conf(conf.get_id());
}

bool cUser::del_bdy(long bdy_id)
{
    stringstream query;
    
    query<<"DELETE FROM buddy WHERE user_id = "<< this->id <<"' AND bdy_id = "<< bdy_id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    return true;
}
bool cUser::del_bdy(cUser bdy)
{
    return this->del_bdy(bdy.get_id());
}

bool cUser::del_user()
{
    stringstream query;
    query<<"DELETE FROM conf_user WHERE user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    query<<"DELETE FROM buddy WHERE user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());    
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    query<<"DELETE FROM user WHERE user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    return true;
}
















cConference::cConference(CSLiveDB db)
{
    this->db = db;
}
cConference::cConference(CSLiveDB db, string id)
{
    this->db  = db;
    this->id = id;
}

cConference::~cConference()
{
    this->db.dbconn.close();
    
}


string cConference::get_id()
{
    return this->id;
}
list<cUser> cConference::get_usrList()
{
    
    CDatabase_Connection db;
    db.setDB("SLive2");
    db.setHost("10.12.45.253");
    db.setPassword("SLive2");
    db.setUsername("SLive2");
    db.connect();
    
    stringstream query;
    
    query<<"SELECT * FROM conf_user WHERE conf_id LIKE '"<<this->id<<"';";
    
    db.query(query.str(), query.str().length());
    /*
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();*/
    
    if(db.errnum() != 0)
        throw db.error();
     
    list<cUser> usr_list;
    /*
    for(int i = 0; i < this->db.dbconn.affected_rows(); i++)
    {
        map<string, string> result = this->db.dbconn.fetch_assoc();
        usr_list.push_front(this->db.get_User(atoi(result["user_id"].c_str())));
    }*/
    
    for(int i = 0; i < db.affected_rows(); i++)
    {
        map<string, string> result = db.fetch_assoc();
        usr_list.push_front(this->db.get_User(atoi(result["user_id"].c_str())));
    }
    db.close();
    
    return usr_list;
}
  
bool cConference::set_id(string id)
{
    stringstream query;
    
    query<<"UPDATE conf_user SET conf_id = '" << id << "' WHERE conf_id LIKE '" << this->id << "';";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    this->id = id;
    return true;

}
/*bool cConference::set_usrList(list<cUser>)
{
    
}*/
    
    
bool cConference::add_usr(long usr_id)
{
    stringstream query;
    
    query<<"INSERT INTO conf_user(conf_id, user_id) VALUES('" << this->id << "', " << usr_id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    
    return true;
}
bool cConference::add_usr(cUser usr)
{
    return this->add_usr(usr.get_id());
}
    
bool cConference::del_usr(long usr_id)
{
    stringstream query;
    
    query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< this->id <<"'* AND user_id = "<< usr_id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    
    return true;

}
bool cConference::del_usr(cUser usr)
{
    return this->del_usr(usr.get_id());
}
  
bool cConference::del_conf()
{
    stringstream query;
    
    query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< this->id <<"';";
    this->db.dbconn.query(query.str(), query.str().length());
    if(db.dbconn.errnum() != 0)
        throw db.dbconn.error();
    return true;
}
    





CSLiveDB::CSLiveDB()
{
    
}

CSLiveDB::CSLiveDB(string user, string password, string DB, string Host, int port)
{
    this->dbconn = CDatabase_Connection(user, password, DB, Host, port);
    dbconn.connect();
    if(dbconn.errnum() != 0)
        throw dbconn.error();
}




CSLiveDB::~CSLiveDB()
{
    this->dbconn.close();
    
}


/*cConference CSLiveDB::create_conf()
{
    
    return cConference(*this);
}*/
cConference CSLiveDB::create_conf(string name)
{
    stringstream query;
    query<<"SELECT uuid() AS id;";
    
    this->dbconn.query(query.str(), query.str().length());
    
    if(dbconn.errnum() != 0)
        throw dbconn.error();
    
    map<string, string> result = this->dbconn.fetch_assoc();
    
    query.str("");
    query<<"INSERT INTO conference (conf_id, name) VALUES('"<<result["id"]<<"', '"<< name <<"');";
    this->dbconn.query(query.str(), query.str().length());
    
    if(dbconn.errnum() != 0)
        throw dbconn.error();
    
    return this->get_Conf(result["id"]);
    
}
cConference CSLiveDB::create_conf(list<cUser> usr_list, string name)
{
    cConference conf = this->create_conf("");
    
    list<cUser>::iterator it;
    
    for(it = usr_list.begin(); it != usr_list.end(); it++)
    {
        conf.add_usr(*it);
    }
    
    return conf;
}
/*
cConference CSLiveDB::create_conf(string name, list<cUser> usr_list)
{
    cConference conf = cConference(*this);
    conf.set_id(id);
    
    for(list<cUser>::iterator i = usr_list.begin(); i != usr_list.end(); i++)
    {
        stringstream query;
        
        query<<"INSERT INTO conf_user(conf_id, user_id) VALUES('"<<id<<"', "<<i->get_id()<<");";
        this->dbconn.query(query.str(), query.str().length());
        
        
    }
    
    
    return conf;
}
*/
cConference CSLiveDB::get_Conf(string id)
{
    stringstream query;
    
    query<<"SELECT * FROM conf_user WHERE conf_id LIKE '" << id <<"';";
    this->dbconn.query(query.str(), query.str().length());
    if(dbconn.errnum() != 0)
        throw dbconn.error();
    cConference conf = cConference(*this, id);
    
    return conf;
}



bool CSLiveDB::checkUsername(string name)
{
    stringstream query;
    
    query<<"SELECT * FROM user WHERE name LIKE '" << name <<"';";
    this->dbconn.query(query.str(), query.str().length());
    if(dbconn.errnum() != 0)
        throw dbconn.error();
    if(this->dbconn.affected_rows() > 0)
        return false;
    else 
        return true;

}

cUser CSLiveDB::create_User(string name, string pw)
{
    if(this->checkUsername(name))
    {
        stringstream query;
        query<<"INSERT INTO user(name, pwhash, email) VALUES ('"<< name << "', '"<< md5(pw) << "', '');";
        
        
        string query_str = query.str();
        
        
        this->dbconn.query(query.str(), query.str().length());
        if(dbconn.errnum() != 0)
            throw dbconn.error();
        
        return this->get_User(name);
        
    }
    else
        throw "Username already in use";
}
cUser CSLiveDB::create_User(string name, string pw, string email)
{
    string name_test = name;
    cUser usr = this->create_User(name, pw);
    usr.set_email(email);
    return usr;
}

cUser CSLiveDB::get_User(long id)
{
    stringstream query;
    query << "SELECT * FROM user WHERE user_id = " << id << ";";
    this->dbconn.query(query.str(), query.str().length());
    
    map<string, string> result = this->dbconn.fetch_assoc();
    
    cUser usr = cUser(*this, atoi(result["user_id"].c_str()), result["name"] , result["pwhash"], result["email"]);
    
    return usr;
    
}
cUser CSLiveDB::get_User(string name)
{
    stringstream query;
    query << "SELECT * FROM user WHERE name LIKE '" << name << "';";
    this->dbconn.query(query.str(), query.str().length());
    
    map<string, string> result = this->dbconn.fetch_assoc();
    
    cUser usr = cUser(*this, atoi(result["user_id"].c_str()), result["name"] , result["pwhash"], result["email"]);
    
    return usr;
}


cUser CSLiveDB::login(string name, string pw, string server)
{
    return this->login(this->get_User(name).get_id(), pw, server);
}

cUser CSLiveDB::login(long id, string pw, string server)
{
    cUser usr = this->get_User(id);
    if(usr.pwhash == md5(pw))
    {
        usr.set_server(server);
        usr.set_status(ONLINE);
    }
    else {
        throw string("wrong username or password");
    }
    
    return usr;
}


