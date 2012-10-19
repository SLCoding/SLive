//
//  CChat_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CChat_Server.h"

void* CChat_Server_run(void* param)
{
    string rec;
    CSocket sock;
    sock.bind(8376);
    sock.listen();
    cout << "Server gestartet...warte auf Client..." << endl;
    CSocket client_socket = sock.accept();
    client_socket.setBuffer(8192);
    cout << "client connected" << endl << endl;
    while(1)
    {
            //cout << "aktueller socket: " << client_socket.getSocket() << endl;
        client_socket << rec;
            // cout << "vom client empfangen: " << rec;
            // cout << "sende nachricht..." << rec << endl;
            // cout.flush();
        rec = string("nachricht erhalten: ") + rec + string("\n");
        client_socket >> rec;
    }
    return (void*)0;
}