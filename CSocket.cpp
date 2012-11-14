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
    socket_handle = -1;
    setBuffer();
    createSocket();
}

CSocket::CSocket(int socket)
{
    socket_handle = socket;
    setBuffer();
}

CSocket::~CSocket()
{
   ;
}

bool CSocket::is_valid() const
{
    if(socket_handle == -1)
        return false;
    return true;
}

bool CSocket::createSocket()
{
    // Create the socket - communication is via TCP/IP
    int i = 1;
    socket_handle = socket( AF_INET, SOCK_STREAM, 0 );
    setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
    if (socket_handle < 0)
    {
        throw string("Can't create a socket!");
    }
    return true;
}

bool CSocket::bind(int port)
{
    if( !is_valid() )
        return false;
    memset( &host, 0, sizeof (host));
    // IPv4-adress
    host.sin_family = AF_INET;
    // any ip-address is ok
    host.sin_addr.s_addr = htonl( INADDR_ANY );
    host.sin_port = htons( port );
    if ( ::bind( socket_handle, (struct sockaddr*)&host, sizeof(host)) < 0)
    {
        throw string("Can't bind address to socket!");
    }
    return true;
}

bool CSocket::connect(string address, int port)
{
    if( !is_valid() )
        return false;
    struct hostent *host_info;
    unsigned long addr;
    
    // Create the socket address of the server
    // It consists of type, IP-address and port number.
    memset( &host, 0, sizeof (host)); // clear memory of structure
    if ((addr = inet_addr( address.c_str() )) != INADDR_NONE)
    {
        ip = address;
        // address is a numeric ip-address
        memcpy( (char *)&host.sin_addr, &addr, sizeof(addr));
    }
    else
    {
        // if address is not a numeric ip-address change servername into ip-address
        host_info = gethostbyname(address.c_str());
        if (NULL == host_info)
            throw string("Unknown Server");
        // save server-ip-address in structure
        
        memcpy( (char *)&host.sin_addr, host_info->h_addr, host_info->h_length );
        ip = host_info->h_addr;
    }
    host.sin_family = AF_INET;      // use IPv4
    host.sin_port = htons(port);    // change port number in host-to-network-syntax
    
    // connect to server
    if ( ::connect( socket_handle, (struct sockaddr*)&host, sizeof(host) ) < 0)
    {
        throw string("Cant connect to target!");
    }
    return true;
}

bool CSocket::closeSocket()
{
    if( !is_valid() )
        return false;
    return ::close(socket_handle);
}

bool CSocket::listen(int queue_size)
{
    if( !is_valid() )
        return false;
    // the function is listening for a new incoming client and add it to the queue, maximum number of clients = queue_size
    if( ::listen( socket_handle, queue_size ) == -1 )
    {
        throw string("Unkown error: CSocket::listen");
    }
    return true;
}

CSocket CSocket::accept()
{
    if( !is_valid() )
        return false;
    struct sockaddr_in client;
    int sock2;
    socklen_t len;
    
    len = sizeof( client );
    // accept a specific client connectinginquire and returns a socket for the communication 
    sock2 = ::accept( socket_handle, (struct sockaddr*)&client, &len);
    if (sock2 < 0)
    {
        throw string("Can't accept connectioninquire");
    }
    return CSocket(sock2);
}

bool CSocket::send(string message) const
{
    if( !is_valid() )
        return false;
    char *char_message = new char[message.length()];
    strncpy(char_message, message.c_str(), message.length());
        // long transmitted_data = 0;
    long send_return = 0;
    // send the data, if something goes wrong and some data could not be send, send the remainder until all of data are transmitted
        // while(transmitted_data != message.length())
        //  {
        send_return = ::write(socket_handle, char_message, message.length()/* - transmitted_data*/);
        if(send_return == -1)
        {
            delete [] char_message;
            throw string("An error occured while sending the message");
        }
        //  transmitted_data = transmitted_data + send_return;
        //    }
    delete [] char_message;
    return true;
}

string CSocket::recv()
{
    if( !is_valid() )
        throw "Socket not ready!";
    char char_buffer[buffer];
    long recv_size;
    recv_size = ::read(socket_handle, char_buffer, buffer);
    if(recv_size == 0)
        throw string("Client terminate connection!");
    else if(recv_size < 0)
        throw string("An error occured while receiving the message");
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

const int CSocket::getBuffer() const
{
    return buffer;
}

const string CSocket::getIP() const
{
    return this->ip;
}

const CSocket& CSocket::operator<<(const string& s) const
{
    this->send(s);
    return *this;
}

const CSocket& CSocket::operator<<(const char* s) const
{
    this->send(s);
    return *this;
}

const CSocket& CSocket::operator>>(string &s)
{
    s = this->recv();
    return *this;
}

const int CSocket::getSocket() const
{
    return socket_handle;
}
