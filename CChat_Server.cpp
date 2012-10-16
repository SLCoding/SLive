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
    for(int i = 0; i < 100; i++)
        std::cout << i << std::endl;
}