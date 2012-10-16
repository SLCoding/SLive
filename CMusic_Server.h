//
//  CMusic_Server.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CMusic_Server__
#define __SLive__CMusic_Server__

#include <iostream>
#include "CServer.h"

void *CMusik_Server_run(void *param);

class CMusic_Server : public CServer
{
private:
    
protected:
    
public:
    CMusic_Server() {;}
    friend void *CMusik_Server_run(void *param);
};

#endif /* defined(__SLive__CMusic_Server__) */
