//
//  CConference.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CConference.h"




CConference::CConference(CDatabase_Connection p_db_conn)
{
    client_queue = CQueue(8301);
    client_queue.set_type(1);
    this->db_conn = p_db_conn;
    
}

CConference::CConference(CDatabase_Connection p_db_conn, int p_id)
{
    client_queue = CQueue(8301);
    client_queue.set_type(1);
    this->db_conn = p_db_conn;
    this->id = p_id;
    
    //laden

}

CConference::CConference(CDatabase_Connection p_db_conn, int p_id, list<int> p_client_list)
{
    client_queue = CQueue(8301);
    client_queue.set_type(1);
    this->db_conn = p_db_conn;
    
    this->id = p_id;
    this->client_list = p_client_list;

}

CConference CConference::load(int p_id)
{
    
    
}



bool CConference::add_client(int p_id)
{
    this->client_list.push_back(p_id);
    return true;
}

bool CConference::remove_client(int p_id)
{
    this->client_list.remove(p_id);
    return true;
}

bool CConference::send_msg(string message)
{
    
    list<int>::iterator I;
    
    for(I=this->client_list.begin(); I != this->client_list.end(); ++I)
    {
        client_queue.send_msg(message, true, *I);
    }
    
    return true;
    
}
