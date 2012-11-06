//
//  CServer_Manager.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CServer_Manager.h"
#include "CChat_Server.h"
#include "CMusic_Server.h"
#include "CDatabase_Connection.h"

CServer_Manager::CServer_Manager()
{
    chat = new CChat_Server;
    music = new CMusic_Server;
    dbconn = new CDatabase_Connection;

    int chatid = chat->start(reinterpret_cast<void*>(chat), accept_new_Clients);
    chat->join(chatid, NULL);
}