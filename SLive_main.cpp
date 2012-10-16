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
#include <iostream>
using namespace std;

int main()
{
    CChat_Server chat;
    CMusic_Server music;
    CClient client;
    
    int ret1 = chat.start(NULL, CChat_Server_run);
    int ret2 = music.start(NULL, CMusik_Server_run);
    int ret3 = client.start(NULL, CClient_run);
    
    cout << "ret: " << ret1 << endl;
    chat.join(NULL);
    music.join(NULL);
    client.join(NULL);
    return 0;
}
