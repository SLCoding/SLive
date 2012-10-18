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

void* CChat_Server_run(void* param);

class CChat_Server : public CServer
{
private:
    
protected:
    
public:
    CChat_Server() {;}
    friend void* CChat_Server_run(void* param);
};

#endif /* defined(__SLive__CChat_Server__) */
