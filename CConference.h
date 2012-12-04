//
//  CConference.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CConference__
#define __SLive__CConference__

#include <iostream>
#include <list>
#include "CQueue.h"
#include "CDatabase_Connection.h"

using namespace std;

class CConference
{
private:
    int id;
    CDatabase_Connection db_conn;
        // CQueue client_queue;
    CQueue client_queue;
    list<int> client_list;
    
protected:
    
public:
    CConference(CDatabase_Connection p_db_conn);
    CConference(CDatabase_Connection p_db_conn, int p_id);
    CConference(CDatabase_Connection p_db_conn, int p_id, list<int> p_client_list);
    static CConference load(int p_id);
    
    bool add_client(int p_id);
    bool remove_client(int p_id);
    bool send_msg(string message);
};

#endif /* defined(__SLive__CConference__) */
