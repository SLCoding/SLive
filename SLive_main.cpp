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
using namespace std;

int main()
{
    CChat_Server chat;
    CMusic_Server music;
    CClient client;
    
    int ret1 = chat.start(NULL, CChat_Server_run);
    
    CSocket sock;
    sock.createSocket();
    sock.bind(8376);
    sock.listen();
    CSocket client_socket = sock.accept();
    
    string rec = client_socket.recv();
    cout << "vom client empfangen " << rec << endl;
    client_socket.send("nachricht erhalten:" + rec);
    
    getchar();
    
    int ret2 = music.start(NULL, CMusik_Server_run);
    int ret3 = client.start(NULL, CClient_run);
    
    cout << "ret: " << ret1 << endl;
    chat.join(ret1, NULL);
    music.join(ret2, NULL);
    client.join(ret3, NULL);
    return 0;
}
