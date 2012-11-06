//
//  CClient.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CClient__
#define __SLive__CClient__

#include <iostream>
#include "CServer.h"
#include "CSocket.h"

    // void* CClient_run(void *param);

class CClient 
{
private:
    CSocket *sock;
    int id;
    
protected:
    
public:
    CClient(int, CSocket*);
    void setSocket(CSocket*);
    void setID(int);

    CSocket* getSocket();
    int getID();
};

#endif /* defined(__SLive__CClient__) */
