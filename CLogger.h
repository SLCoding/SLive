//
//  CLogger.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CLogger__
#define __SLive__CLogger__

#include <iostream>
#include "CThread.h"

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void* CLogger_run(void *param);

class CLogger: public CThread
{
private:
    
protected:
    
public:
    CLogger(){;}
    friend void* CLogger_run(void *param);
    
};

#endif /* defined(__SLive__CLogger__) */