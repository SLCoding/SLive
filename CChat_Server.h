//
//  CChat_Server.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CChat_Server__
#define __SLive__CChat_Server__

#include <iostream>
#include "CServer.h"
#include "CSocket.h"
#include "CConference.h"
#include "CThread.h"
#include "CClient.h"

void* accept_new_Clients(void* param);
void* client_processing(void* param);
void* processing_message(void* param);

class CChat_Server : public CServer
{
private:
    CThread client_thread;
    list<CClient*> clients;
protected:
    
public:
    CChat_Server() {;}
    void test();
    friend void* accept_new_Clients(void* param);
    friend void* client_processing(void* param);
    friend void* processing_message(void* param);
};

#endif /* defined(__SLive__CChat_Server__) */
