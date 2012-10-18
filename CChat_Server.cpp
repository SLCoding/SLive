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
    sleep(4);
    CSocket sock;
    sock.createSocket();
    sock.connect("localhost", 8376);
    sock.send("Hallo ich bin Client 1");
    string rec = sock.recv();
    cout << "vom server empfangen " << rec << endl;
}