//
//  CNetwork.h
//  SLive
//
//  Created by Markus Scholl on 17.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//
// The class CSocket allows you to use the C-API for network communication in an easier way

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

// Following functions throws an exeption if an error occure and return a string with the error information
//
// createSocket(), send(), recv(), closeSocket(), connect(), bind(), listen(), accept()

class CSocket
{
    private:
        int socket_handle;
        struct sockaddr_in host; // store address-information about target computer
        int buffer;
        bool is_valid() const;
        bool createSocket();                        // Create a specific socket
        string ip;
        string local_ip;
    
    public:
        CSocket();
        CSocket(int socket);
        virtual ~CSocket();
    
        // Client/Server
        
        bool send(string message) const;                        // send a message to target computer
        string recv();                                          // receive message
        bool closeSocket();                                     // close the socket and terminate communication
        void setBuffer(int buffer_size = 1024);                 // Set the maximum buffer for receiving data / Minimum buffer size is 1024 / Maximum buffer size is 16384
        const int getBuffer() const;
        const int getSocket() const;
        const CSocket& operator << (const string& s) const;     // send the string
        const CSocket& operator << (const char* s) const;     // send the string
        const CSocket& operator >> (string& s);                 // receive data and store them in the string
        const string getIP() const; // return the ip adress of the target
        const string getLocalIP() const;
    
        // Client-functions
        bool connect(string address, int port);     // connect to the target computer
    
        // Server-functions
        bool bind(int port);                        // bind the port and allow any clients to connect
        bool listen(int queue_size = 10);           // listen for a new incoming client - queue_size describe the maximum number of connectinginquire
        CSocket accept();                           // Accept a specific connectinginquire from a client and returns a new Socket-object
};

#endif /* defined(__SLive__CSocket__) */
