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
    
    int ret = chat.start(NULL, test);
    cout << "ret: " << ret << endl;
    return 0;
}
