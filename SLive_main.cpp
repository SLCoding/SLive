//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CServer_Manager.h"
#include "CChat_Server.h"
#include <iostream>
using namespace std;

int main()
{
    CChat_Server chat;
    
    int ret1 = chat.start(NULL, CChat_Server_run);

    cout << "ret: " << ret1 << endl;
    chat.join(NULL);
    return 0;
}
