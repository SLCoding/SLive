//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include <iostream>
using namespace std;

#include "CServer_Manager.h"
#include "CChat_Server.h"

void* cchat_server_run(void* param)
{
    cout << "Test" << endl;
}

int main()
{
    CChat_Server chat;
    
    int ret = chat.start(NULL, cchat_server_run);
    cout << "ret: " << ret << endl;
    return 0;
}
