//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CServer_Manager.h"
#include "CChat_Server.h"
#include "CMusic_Server.h"
#include "CClient.h"
#include "CServer.h"
#include "CSocket.h"
#include "CDatabase_Connection.h"
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "CQueue.h"

using namespace std;

int main()
{
    CLogger logger;
<<<<<<< HEAD
    CQueue log(8300);
    log.set_type(3);
    logger.start(NULL, CLogger_run);
    CServer_Manager manager;

    logger.join(NULL);
    
    return 0;
=======
    
    int ret4 = logger.start(NULL, CLogger_run);
    //CServer_Manager manager;
    
    CQueue log(8300);
    getchar();
    log.set_type(2);
    log<<"Test";
    
    /* hole die Messagetypnummer aus dem ersten Parameter */
    logger.join(ret4, NULL);
    

        return 0;
>>>>>>> dev-japortie
}
