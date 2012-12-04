//
//  CSLiveDB.cpp
//  SLive
//
//  Created by Marcus Schütte on 14.11.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CSLiveDB.h"

CSLiveDB::CSLiveDB(string user, string password, string DB, string Host, int port)
{
    this->dbconn = CDatabase_Connection(user, password, DB, Host, port);
    dbconn.connect();
}

cUser::cUser(CSLiveDB db)
{
    this->db = db;
}

cUser::cUser(CSLiveDB db, int id, string name, string pwhash, string email)
{
    this->db = db;
    this->id = id;
    this->name = name;
    this->pwhash = pwhash;
    this->email = email;
    this->conf_list = conf_list;
    this->bdy_list = bdy_list;
}


cUser::cUser(CSLiveDB db, int id, string name, string pwhash, string email, list<cConference> conf_list, list<cUser> bdy_list)
{
    this->db = db;
    this->id = id;
    this->name = name;
    this->pwhash = pwhash;
    this->email = email;
    this->conf_list = conf_list;
    this->bdy_list = bdy_list;
}


int cUser::get_id()
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
    return this->server;
}
list<cConference> cUser::get_confList()
{
    return this->conf_list;
}
list<cUser> cUser::get_bdyList()
{
    return this->bdy_list;
}

bool cUser::set_id(int id)
{
    stringstream query;
    
    query<<"UPDATE user SET id = " << id << " WHERE id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->id = id;
    return true;
}
bool cUser::set_name(string name)
{
    stringstream query;
    
    query<<"UPDATE user SET name = '" << name << "' WHERE id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->name = name;
    
    return true;
}
bool cUser::set_email(string email)
{
    stringstream query;
    
    query<<"UPDATE user SET email = '" << email << "' WHERE id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->email = email;
    
    return true;
}
bool cUser::set_server(string server)
{
    stringstream query;
    
    query<<"UPDATE user SET server = '" << server << "' WHERE id = " << this->id << ";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    
    this->server = server;
    
    return true;
}


bool cUser::add_conf(string conf_id)
{
    stringstream query;
    
    query<<"INSERT INTO conference(id, user_id) VALUES('" << conf_id << "', " << this->id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->conf_list.push_front(this->db.get_Conf(conf_id));
    
    return true;
}
bool cUser::add_conf(cConference conf)
{
    return this->add_conf(conf.get_id());
    
}

bool cUser::add_bdy(int bdy_id)
{
    stringstream query;
    
    query<<"INSERT INTO buddy(user_id, bdy_id) VALUES(" << this->id << ", " << bdy_id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->bdy_list.push_front(this->db.get_User(bdy_id));
    
    return true;
}
bool cUser::add_bdy(cUser bdy)
{
    return add_bdy(bdy.get_id());
    
}


bool cUser::del_conf(string conf_id)
{    
    stringstream query;
    
    query<<"DELETE FROM conference WHERE id LIKE '"<< conf_id <<"'* AND user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->conf_list.remove(this->db.get_Conf(conf_id));
    
    return true;
    
}
bool cUser::del_conf(cConference conf)
{
    return this->del_conf(conf.get_id());
}

bool cUser::del_bdy(int bdy_id)
{
    stringstream query;
    
    query<<"DELETE FROM buddy WHERE user_id = "<< this->id <<"'* AND bdy_id = "<< bdy_id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->bdy_list.remove(this->db.get_User(bdy_id));
    
    return true;
}
bool cUser::del_bdy(cUser bdy)
{
    return this->del_bdy(bdy.get_id());
}

bool cUser::del_user()
{
    stringstream query;
    query<<"DELETE FROM conference WHERE user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    query<<"DELETE FROM buddy WHERE user_id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());    
    
    query<<"DELETE FROM user WHERE id = "<< this->id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    return true;
}
















cConference::cConference(CSLiveDB db)
{
    this->db = db;
}
cConference::cConference(CSLiveDB db, string id, list<cUser> usr_list)
{
    this->db  = db;
    this->id = id;
    this->usr_list = usr_list;
}
    

string cConference::get_id()
{
    return this->id;
}
list<cUser> cConference::get_usrList()
{
    return this->usr_list;
}
  
bool cConference::set_id(string id)
{
    stringstream query;
    
    query<<"UPDATE conference SET id = '" << id << "' WHERE id LIKE '" << this->id << "';";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->id = id;
    return true;

}
/*bool cConference::set_usrList(list<cUser>)
{
    
}*/
    
    
bool cConference::add_usr(int usr_id)
{
    stringstream query;
    
    query<<"INSERT INTO conference(id, user_id) VALUES('" << this->id << "', " << usr_id << ");";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->usr_list.push_front(this->db.get_User(usr_id));
    
    return true;
}
bool cConference::add_usr(cUser usr)
{
    return this->add_usr(usr.get_id());
}
    
bool cConference::del_usr(int usr_id)
{
    stringstream query;
    
    query<<"DELETE FROM conference WHERE id LIKE '"<< this->id <<"'* AND user_id = "<< usr_id <<";";
    this->db.dbconn.query(query.str(), query.str().length());
    
    this->usr_list.remove(this->db.get_User(usr_id));
    
    return true;

}
bool cConference::del_usr(cUser usr)
{
    return this->del_usr(usr.get_id());
}
  
bool cConference::del_conf()
{
    stringstream query;
    
    query<<"DELETE FROM conference WHERE id LIKE '"<< this->id <<"';";
    this->db.dbconn.query(query.str(), query.str().length());
    
    return true;
}
    











































>>>>>>> dev-japortie
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

<<<<<<< HEAD
map<string, string> CSLiveDB::getUserById(string id)
=======
sUser CSLiveDB::getUserById(string id)
>>>>>>> dev-japortie
{
    string query = "SELECT * FROM user WHERE id = '" + id + "';";
    dbconn.query(query, query.length());
    map<string, string> result;
    result = dbconn.fetch_assoc();
    
<<<<<<< HEAD
=======
    sUser usr;
    usr.id = result["id"];
    
    
>>>>>>> dev-japortie
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
    
<<<<<<< HEAD
}
map<string, string> CSLiveDB::getBdyByName(string id, string name)
{


}

map<string, string> CSLiveDB::getConferenceById(string id)
{

}
list<map<string, string> > CSLiveDB::getConferenceByUser(string id)
{
    
    
=======
    list<map<string, string > > result;
    for(int i=0; i<dbconn.affected_rows(); i++)
    {
        result.push_front(dbconn.fetch_assoc());
    }
    
    return result;
}
/*map<string, string> CSLiveDB::getBdyByName(string id, string name)
{
    string query = "SELECT * FROM user WHERE name LIKE '" + name + "';";
    dbconn.query(query, query.length());
    map<string, string> result;
    result = dbconn.fetch_assoc();
    
    return result;
    
}*/

list<string> CSLiveDB::getConferenceById(int id)
{
    stringstream ss;
    ss << id;
    string query = "SELECT * FROM conference WHERE id=" + ss.str() + ";";
    dbconn.query(query, query.length());
    list<string> result;
    for(int i=0; i<dbconn.affected_rows(); i++)
    {
        result.push_front(dbconn.fetch_assoc().first());
    }
    
    return result;
}

list<string> CSLiveDB::getConferenceByUser(string id)
{
    stringstream ss;
    ss << id;
    string query = "SELECT * FROM conference WHERE id=" + ss.str() + ";";
    dbconn.query(query, query.length());
    list<map<string, string > > result;
    for(int i=0; i<dbconn.affected_rows(); i++)
    {
        result.push_front(dbconn.fetch_assoc());
    }
    
    return result;
>>>>>>> dev-japortie
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