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

void* CClient_run(void *param);

class CClient : public CServer
{
private:
    
protected:
    
public:
    CClient() {;}
    friend void* CClient_run(void *param);
};

#endif /* defined(__SLive__CClient__) */
