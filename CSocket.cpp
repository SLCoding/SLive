//
//  CNetwork.cpp
//  SLive
//
//  Created by Markus Scholl on 17.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CSocket.h"

CSocket::CSocket()
{
    ;
}

CSocket::CSocket(string host, int port)
{
    ;
}

int CSocket::createSocket(int port)
{
    this->port = port;
    // Create the socket - communication is via TCP/IP
    socket_handle = socket( AF_INET, SOCK_STREAM, 0 );
    if (socket_handle < 0)
    {
        throw "Can't create a socket!";
    }
    return 0;
}

int CSocket::bindServer()
{
    memset( &host, 0, sizeof (host));
    // IPv4-Adresse
    host.sin_family = AF_INET;
    // Jede IP-Adresse ist gültig
    host.sin_addr.s_addr = htonl( INADDR_ANY );
    host.sin_port = htons( port );
    auto test = bind( socket_handle, (struct sockaddr*)&host, sizeof(host));
    cout << sizeof(test);
   /* if ( bind( socket_handle, (struct sockaddr*)&host, sizeof(host)) < 0)
    {
        throw "Can't bind address to socket!";
    }*/
    return 0;
}

int CSocket::connectToTarget(string dnsname)
{
    host_info = gethostbyname(dnsname.c_str());
    if(host_info == NULL)
    {
        addr = inet_addr( host_info->h_addr_list[0] );
    }
    else
    {
        addr = inet_addr( dnsname.c_str() ); // in dnsname isn't a real dnsname it is a direct ip-address
    }
    memset( &host, 0, sizeof (host));
    memcpy( (char *)&host.sin_addr, &addr, sizeof(addr));
    host.sin_family = AF_INET;
    host.sin_port = htons(port);
    
    // Baue die Verbindung zum Server auf.
    if (connect( socket_handle, (struct sockaddr*)&host, sizeof(host) ) < 0)
    {
        throw "Cant connect to target!";
    }
    return 0;
}

int CSocket::closeSocket()
{
    return close(socket_handle);
}
