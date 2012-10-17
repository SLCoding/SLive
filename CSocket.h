//
//  CNetwork.h
//  SLive
//
//  Created by Markus Scholl on 17.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CSocket__
#define __SLive__CSocket__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

class CSocket
{
private:
    int socket_handle;
    struct sockaddr_in host;
    struct hostent *host_info;
    int port;
    string hostname;
    string ip;
    unsigned long addr;
    
protected:
    
public:
    CSocket();
    CSocket(string host, int port);
    int createSocket(int port);
    int connectToTarget(string host);
    int send(string info);
    string recv();
    int closeSocket();
    
    int bindServer();
};

#endif /* defined(__SLive__CSocket__) */
