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
    //this->db.dbconn.close();
    
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
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    
    
    
    
    map<string, string> result = db.fetch_assoc();
    
    db.close();
    
    return result["server"].c_str();
}
list<cConference> cUser::get_confList()
{
    stringstream query;
    query<<"SELECT * FROM conf_user WHERE user_id = "<<this->id<<";";
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    list<cConference> conf_list;
    map<string, string> result;
    
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
    stringstream query;
    query<<"SELECT * FROM buddy WHERE user_id = "<<this->get_id()<<";";
    
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
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
    
    CDatabase_Connection db;
    try 
    {
        db.setDB(this->db.DB);
        db.setHost(this->db.Host);
        db.setPassword(this->db.password);
        db.setPort(this->db.port);
        db.setUsername(this->db.user);
        //db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            // db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.setDB(this->db.DB);
            db.setHost(this->db.Host2);
            db.setPassword(this->db.password);
            db.setPort(this->db.port2);
            db.setUsername(this->db.user);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    
    map<string, string> result = db.fetch_assoc();
    db.close();
    return (user_status)atoi(result["status"].c_str());
    
}





bool cUser::set_id(long id)
{
    
    stringstream query;
    query<<"UPDATE user SET user_id = " << id << " WHERE user_id = " << this->id << ";";
    
    
    
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
    this->id = id;
    return true;
}
bool cUser::set_name(string name)
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, name, name.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"UPDATE user SET name = '" << secure_param << "' WHERE user_id = " << this->id << ";";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"UPDATE user SET email = '" << name << "' WHERE user_id = " << this->id << ";";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    this->name = name;
    db.close();
    return true;
}
bool cUser::set_email(string email)
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, email, email.length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"UPDATE user SET email = '" << secure_param << "' WHERE user_id = " << this->id << ";";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"UPDATE user SET email = '" << email << "' WHERE user_id = " << this->id << ";";
    }
    
    try
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    this->email = email;
    db.close();
    return true;
}
bool cUser::set_server(string server)
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    db.setDB(this->db.DB);
    db.setHost(this->db.Host);
    db.setPassword(this->db.password);
    db.setPort(this->db.port);
    db.setUsername(this->db.user);
    try
    {
        db.real_escape_string(secure_param, server, server.length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"UPDATE user SET server = '" << secure_param << "' WHERE user_id = " << this->id << ";";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"UPDATE user SET server = '" << server << "' WHERE user_id = " << this->id << ";";
    }
    
    try 
    {
        // db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db.setDB(this->db.DB);
            db.setHost(this->db.Host2);
            db.setPassword(this->db.password);
            db.setPort(this->db.port2);
            db.setUsername(this->db.user);
            // db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    this->server = server;
    db.close();
    return true;
}


bool cUser::set_status(user_status status)
{
    stringstream query;
    
    query<<"UPDATE user SET status = '" << (int)status << "' WHERE user_id = " << this->id << ";";
    CDatabase_Connection db;
    db.setDB(this->db.DB);
    db.setHost(this->db.Host);
    db.setPassword(this->db.password);
    db.setPort(this->db.port);
    db.setUsername(this->db.user);
    try 
    {
        // db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db.setDB(this->db.DB);
            db.setHost(this->db.Host2);
            db.setPassword(this->db.password);
            db.setPort(this->db.port2);
            db.setUsername(this->db.user);
            // db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
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
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, conf_id, conf_id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"INSERT INTO conf_user(conf_id, user_id) VALUES('" << secure_param << "', " << this->id << ");";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"INSERT INTO conf_user(conf_id, user_id) VALUES('" << conf_id << "', " << this->id << ");";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
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
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
    return true;
}
bool cUser::add_bdy(cUser bdy)
{
    return add_bdy(bdy.get_id());
    
}


bool cUser::del_conf(string conf_id)
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, conf_id, conf_id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< secure_param <<"'* AND user_id = "<< this->id <<";";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< conf_id <<"'* AND user_id = "<< this->id <<";";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
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
    
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    
    db.close();
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
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    db.close();
    
    query<<"DELETE FROM buddy WHERE user_id = "<< this->id <<";";
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    db.close();
    
    query<<"DELETE FROM user WHERE user_id = "<< this->id <<";";
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    db.close();
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
    //this->db.dbconn.close();
    
}


string cConference::get_id()
{
    return this->id;
}
list<cUser> cConference::get_usrList()
{
    stringstream query;
    //string secure_param;
    CDatabase_Connection db;
    try
    {
        //db.real_escape_string(secure_param, this->id, this->id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"SELECT * FROM conf_user WHERE conf_id LIKE '"<<this->id<<"';";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"SELECT * FROM conf_user WHERE conf_id LIKE '"<<this->id<<"';";
    }
    
    try
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
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
    string secure_param;
    string secure_param2;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, id, id.length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        db.real_escape_string(secure_param2, this->id, this->id.length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"UPDATE conf_user SET conf_id = '" << secure_param << "' WHERE conf_id LIKE '" << secure_param2 << "';";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"UPDATE conf_user SET conf_id = '" << id << "' WHERE conf_id LIKE '" << this->id << "';";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    db.close();
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
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
    return true;
}
bool cConference::add_usr(cUser usr)
{
    return this->add_usr(usr.get_id());
}

bool cConference::del_usr(long usr_id)
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, this->id, this->id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< secure_param <<"'* AND user_id = "<< usr_id <<";";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< this->id <<"'* AND user_id = "<< usr_id <<";";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
    
    return true;
    
}
bool cConference::del_usr(cUser usr)
{
    return this->del_usr(usr.get_id());
}

bool cConference::del_conf()
{
    stringstream query;
    string secure_param;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, this->id, this->id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< secure_param <<"';";    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"DELETE FROM conf_user WHERE conf_id LIKE '"<< this->id <<"';";
    }
    
    try 
    {
        db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host, this->db.port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->db.user, this->db.password, this->db.DB, this->db.Host2, this->db.port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    db.close();
    return true;
}






CSLiveDB::CSLiveDB()
{
    
}

CSLiveDB::CSLiveDB(string user, string password, string DB, string Host, string Host2, int port, int port2)
{
    /*
     this->dbconn = CDatabase_Connection(user, password, DB, Host, port);
     dbconn.connect();
     if(dbconn.errnum() != 0)
     throw dbconn.error();*/
    
    this->user = user;
    this->password = password;
    this->DB = DB;
    this->Host = Host;
    this->Host2 = Host2;
    this->port = port;
    this->port2 = port2;
}




CSLiveDB::~CSLiveDB()
{
    //this->dbconn.close();
    
}


/*cConference CSLiveDB::create_conf()
 {
 
 return cConference(*this);
 }*/
cConference CSLiveDB::create_conf(string name)
{
    stringstream query;
    query<<"SELECT uuid() AS id;";
    
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    map<string, string> result = db.fetch_assoc();
    cout<<"Generated UUID: "<<result["id"]<<endl;
    query.str("");
    
    string secure_param;
    try
    {
        db.real_escape_string(secure_param, name, name.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"INSERT INTO conference (conf_id, name) VALUES('"<<result["id"]<<"', '"<< secure_param <<"');";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"INSERT INTO conference (conf_id, name) VALUES('"<<result["id"]<<"', '"<< name <<"');";
    }
    
    try 
    {
        db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    db.close();
    return this->get_Conf(result["id"]);
    
}
cConference CSLiveDB::create_conf(list<cUser> usr_list, string name)
{
    CDatabase_Connection db(this->user, this->password, this->DB, this->Host, this->port);
    db.connect();
    
    cConference conf = this->create_conf(name);
    
    list<cUser>::iterator it;
    
    for(it = usr_list.begin(); it != usr_list.end(); it++)
    {
        conf.add_usr(*it);
    }
    
    db.close();
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
    //string secure_param;
    stringstream query;
    CDatabase_Connection db;
    try
    {
        //db.real_escape_string(secure_param, id, id.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"SELECT * FROM conf_user WHERE conf_id LIKE '" << id <<"';";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"SELECT * FROM conf_user WHERE conf_id LIKE '" << id <<"';";
    }
    
    try 
    {
        db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    cConference conf = cConference(*this, id);
    
    db.close();
    return conf;
}



bool CSLiveDB::checkUsername(string name)
{
    string secure_param;
    stringstream query;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, name, name.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query<<"SELECT * FROM user WHERE name LIKE '" << secure_param <<"';";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query<<"SELECT * FROM user WHERE name LIKE '" << name <<"';";
    }
    
    try 
    {
        db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    if(db.affected_rows() > 0)
    {
        db.close();
        return false;
    }
    else
    {
        db.close();
        return true;
    }
}

cUser CSLiveDB::create_User(string name, string pw)
{
    string backup = name;
    string secure_param;
    stringstream query;
    CDatabase_Connection db;
    try
    {
        db.real_escape_string(secure_param, name, name.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        name = backup;
    }
    
    if(this->checkUsername(name))
    {
        query<<"INSERT INTO user(name, pwhash, email) VALUES ('"<< name << "', '"<< md5(pw) << "', '');";
        try 
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        
        catch (dbError err1)
        {
            try
            {
                db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
                db.connect();
                db.query(query.str(), query.str().length());
                if(db.errnum() != 0)
                {
                    dbError err;
                    err.errnum = db.errnum();
                    err.errstr = db.error();
                    throw err;
                }
            }
            catch (dbError err2)
            {
                cout<<"Unhandled DB Exception on Both Databases"<<endl;
                cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
                cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
            }
            
        }
        
        
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
            throw db.error();
        
        db.close();
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
    CDatabase_Connection db;
    try 
    {
        db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    map<string, string> result = db.fetch_assoc();
    
    cUser usr = cUser(*this, atoi(result["user_id"].c_str()), result["name"] , result["pwhash"], result["email"]);
    db.close();
    return usr;
    
}
cUser CSLiveDB::get_User(string name)
{
    string secure_param;
    stringstream query;
    CDatabase_Connection db;
    db.setDB(this->DB);
    db.setHost(this->Host);
    db.setPassword(this->password);
    db.setPort(this->port);
    db.setUsername(this->user);
    try
    {
        db.real_escape_string(secure_param, name, name.length()); // every specialcharacter is escaped
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
        query << "SELECT * FROM user WHERE name LIKE '" << secure_param << "';";
    }
    catch(dbError err1)
    {
        cout<<"Unhandled Exception on mysql_real_escape_string"<<endl;
        cout<<"--> "<<err1.errnum<<" - "<<err1.errstr<<endl;
        query << "SELECT * FROM user WHERE name LIKE '" << name << "';";
    }
    
    try 
    {
        //db = CDatabase_Connection(this->user, this->password, this->DB, this->Host, this->port);
        db.connect();
        db.query(query.str(), query.str().length());
        if(db.errnum() != 0)
        {
            dbError err;
            err.errnum = db.errnum();
            err.errstr = db.error();
            throw err;
        }
    }
    
    catch (dbError err1)
    {
        try
        {
            //db.close();
            //db = CDatabase_Connection(this->user, this->password, this->DB, this->Host2, this->port2);
            db.setDB(this->DB);
            db.setHost(this->Host2);
            db.setPassword(this->password);
            db.setPort(this->port2);
            db.setUsername(this->user);
            db.connect();
            db.query(query.str(), query.str().length());
            if(db.errnum() != 0)
            {
                dbError err;
                err.errnum = db.errnum();
                err.errstr = db.error();
                throw err;
            }
        }
        catch (dbError err2)
        {
            cout<<"Unhandled DB Exception on Both Databases"<<endl;
            cout<<"DB1: "<<err1.errnum<<" - "<<err1.errstr<<endl;
            cout<<"DB2: "<<err2.errnum<<" - "<<err2.errstr<<endl;                
        }
        
    }
    
    map<string, string> result = db.fetch_assoc();
    
    cUser usr = cUser(*this, atoi(result["user_id"].c_str()), result["name"] , result["pwhash"], result["email"]);
    db.close();
    return usr;
}


cUser CSLiveDB::login(string name, string pw, string server)
{
    cUser usr = this->get_User(name);
    //if(usr.get_status() != OFFLINE)
    //{
    //   throw "Already logged in!";
    //}
    //else
    // {
    return this->login(this->get_User(name).get_id(), pw, server);        
    //}
    
}

cUser CSLiveDB::login(long id, string pw, string server)
{
    cUser usr = this->get_User(id);
    // if(usr.get_status() != OFFLINE)
    //{
    //   throw "Already logged in!";
    //}
    //else
    //{
    if(usr.pwhash == md5(pw))
    {
        usr.set_server(server);
        usr.set_status(ONLINE);
    }
    else
    {
        throw string("wrong username or password");
    }
    
    return usr;
    
    //    }
}


