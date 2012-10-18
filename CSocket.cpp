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
    socket_handle = 0;
    setBuffer();
}

CSocket::CSocket(int socket)
{
    socket_handle = socket;
    setBuffer();
}

CSocket::~CSocket()
{
    closeSocket();
}

bool CSocket::createSocket()
{
    // Create the socket - communication is via TCP/IP
    socket_handle = socket( AF_INET, SOCK_STREAM, 0 );
    if (socket_handle < 0)
    {
        throw "Can't create a socket!";
    }
    return true;
}

bool CSocket::bind(int port)
{
    memset( &host, 0, sizeof (host));
    // IPv4-Adresse
    host.sin_family = AF_INET;
    // Jede IP-Adresse ist gültig
    host.sin_addr.s_addr = htonl( INADDR_ANY );
    host.sin_port = htons( port );
    if ( ::bind( socket_handle, (struct sockaddr*)&host, sizeof(host)) < 0)
    {
        throw "Can't bind address to socket!";
    }
    return true;
}

bool CSocket::connect(string address, int port)
{
    struct hostent *host_info;
    unsigned long addr;
    
    /* Erzeuge die Socketadresse des Servers.
     * Sie besteht aus Typ, IP-Adresse und Portnummer. */
    memset( &host, 0, sizeof (host));
    if ((addr = inet_addr( address.c_str() )) != INADDR_NONE)
    {
        /* argv[1] ist eine numerische IP-Adresse. */
        memcpy( (char *)&host.sin_addr, &addr, sizeof(addr));
    }
    else
    {
        /* Für den Fall der Fälle: Wandle den
         * Servernamen bspw. "localhost" in eine IP-Adresse um. */
        host_info = gethostbyname(address.c_str());
        if (NULL == host_info)
            throw "Unknown Server";
        /* Server-IP-Adresse */
        memcpy( (char *)&host.sin_addr, host_info->h_addr, host_info->h_length );
    }
    host.sin_family = AF_INET;
    host.sin_port = htons(port);
    
    // Baue die Verbindung zum Server auf.
    if ( ::connect( socket_handle, (struct sockaddr*)&host, sizeof(host) ) < 0)
    {
        throw "Cant connect to target!";
    }
    return true;
}

bool CSocket::closeSocket()
{
    return close(socket_handle);
}

bool CSocket::listen(int queue_size)
{
    if( ::listen( socket_handle, queue_size ) == -1 )
    {
        throw "Unkown error: CSocket::listen";
    }
    return true;
}

CSocket CSocket::accept()
{
    struct sockaddr_in client;
    int sock2;
    socklen_t len;
    
    len = sizeof( client );
    sock2 = ::accept( socket_handle, (struct sockaddr*)&client, &len);
    if (sock2 < 0)
    {
        throw "Can't accept connectioninquire";
    }
    return CSocket(sock2);
}

bool CSocket::send(string message)
{
    char *char_message = new char[message.length()];
    strncpy(char_message, message.c_str(), message.length());
    int transmitted_data = 0;
    int send_return = 0;
    while(transmitted_data != message.length())
    {
        send_return = ::send(socket_handle, char_message, message.length() - transmitted_data, 0);
        if(send_return == -1)
        {
            delete [] char_message;
            throw "An error occured while sending the message";
        }
        transmitted_data = transmitted_data + send_return;
    }
    delete [] char_message;
    return true;
}

string CSocket::recv()
{
    char char_buffer[buffer];
    int recv_size;
    
    if((recv_size = ::recv(socket_handle, char_buffer, buffer,0)) < 0)
        throw "An error occured while receiving the message";
    char_buffer[recv_size] = '\0';
    string message = char_buffer;
    return message;
}

void CSocket::setBuffer(int buffer_size)
{
    if(buffer_size < 1024)
        buffer_size = 1024;
    else if(buffer_size > 16384)
        buffer_size = 16384;
    buffer = buffer_size;
}

int CSocket::getBuffer()
{
    return buffer;
}

