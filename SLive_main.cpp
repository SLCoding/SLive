//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CServer_Manager.h"
#include "CLogger.h"
#include "CQueue.h"

using namespace std;

int main()
{
    CLogger logger;
    CQueue log(8300);
    log.set_type(3);
    logger.start(NULL, CLogger_run);
    CServer_Manager manager;

    logger.join(NULL);
    
    return 0;
}
