//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include <iostream>
#include "CServer_Manager.h"
#include "CChat_Server.h"

int main()
{
    CChat_Server chat;
    chat.start(NULL);
    return 0;
}
