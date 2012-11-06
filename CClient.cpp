//
//  CClient.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CClient.h"
/*
void* CClient_run(void *param)
{
    while(1)
    {
        int i = 0;
        for(i = 0; i < 100; i++)
             printf("CClient: %d\n", i);
        i = 0;
    }
}
*/

CClient::CClient(int id, CSocket sock)
{
    this->sock = sock;
    this->id = id;
}

void CClient::setSocket(CSocket sock)
{
    this->sock = sock;
}

void CClient::setID(int id)
{
    this->id = id;
}

CSocket CClient::getSocket()
{
    return this->sock;
}

int CClient::getID()
{
    return this->id;
}


