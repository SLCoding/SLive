//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CLogger.h"
#include "CQueue.h"
#include "CDatabase_Connection.h"
#include "CChat_Server.h"

int main()
{
    CLogger logger;
    CQueue log(8300);
    CChat_Server *chat;

    log.set_type(3);
    logger.start(NULL, CLogger_run);

    chat = new CChat_Server;

    logger.join(NULL);

    delete chat;
    
    return 0;
}
