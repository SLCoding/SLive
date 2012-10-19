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
    
    //int ret1 = chat.start(NULL, CChat_Server_run);
    //int ret2 = music.start(NULL, CMusik_Server_run);
    //int ret3 = client.start(NULL, CClient_run);
    int ret4 = logger.start(NULL, CLogger_run);
    
    //gcout << "ret: " << ret1 << endl;
    
    
    

    

    
    /* hole die Messagetypnummer aus dem ersten Parameter */
    CQueue queue(8300);
    
    queue.send_msg("testnachricht", 3);
    
<<<<<<< HEAD
    int ret1 = chat.start(NULL, CChat_Server_run);
    
        //int ret2 = music.start(NULL, CMusik_Server_run);
        //int ret3 = client.start(NULL, CClient_run);
    
    cout << "ret: " << ret1 << endl;
    chat.join(ret1, NULL);
        //music.join(ret2, NULL);
        //client.join(ret3, NULL);
=======
    
   // chat.join(ret1, NULL);
    //music.join(ret2, NULL);
    //client.join(ret3, NULL);
    logger.join(ret4, NULL);
>>>>>>> master
    return 0;
}
