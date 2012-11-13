//
//  CServer_Manager.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CServer_Manager__
#define __SLive__CServer_Manager__

#include <iostream>
#include <list>
#include "CSocket.h"
#include "CDatabase_Connection.h"
#include "CServer.h"
#include "CChat_Server.h"
#include "CLogger.h"

void* CServer_Manager_run(void* param);

class CServer_Manager : public CThread
{
private:
    CChat_Server *chat;
    CServer *music;
    CDatabase_Connection *dbconn;
    int chat_thread_id;
    int music_thread_id;
    int db_thread_id;
    list<CSocket> client_sockets;
protected:
    
public:
    CServer_Manager();
    
};

#endif /* defined(__SLive__CServer_Manager__) */
