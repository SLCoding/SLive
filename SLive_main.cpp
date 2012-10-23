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
#include <iostream>

#include <string>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "CQueue.h"

using namespace std;

int main()
{
    CChat_Server chat;
    CMusic_Server music;
    CClient client;
    CLogger logger;
    
    int ret4 = logger.start(NULL, CLogger_run);
    
    /* hole die Messagetypnummer aus dem ersten Parameter */
    try
    {
     CQueue queue(257);  
        
        queue.set_type(3);
        
        queue<<"TEST";
        
        int ret1 = chat.start(NULL, CChat_Server_run);
        
        // cout << "ret: " << ret1 << endl;
        chat.join(ret1, NULL);
        logger.join(ret4, NULL);
        

    }
    catch(string s)
    {
        cout<<s;
    }
        return 0;
}
