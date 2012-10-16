//
//  CMusic_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CMusic_Server.h"

void *CMusik_Server_run(void *param)
{
    while(1)
    {
        int i = 0;
        for(i = 0; i < 100; i++)
             printf("CMusic_Server_run: %d\n", i);
        i=0;
    }
}