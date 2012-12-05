//
//  CClient.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CClient.h"

CClient::CClient(int id, CSocket sock)
{
    this->sock = sock;
    this->id = id;
    this->login = false;
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


