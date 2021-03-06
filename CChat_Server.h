    //
    //  CChat_Server.h
    //  SLive
    //
    //  Created by Markus Scholl on 10.10.12.
    //  Copyright (c) 2012 CLMM. All rights reserved.
    //

#ifndef __SLive__CChat_Server__
#define __SLive__CChat_Server__

#include <iostream>
#include "CSocket.h"
#include "CConference.h"
#include "CThread.h"
#include "CClient.h"
#include "CSLiveDB.h"

void* accept_new_Clients(void* param);
void* client_processing(void* param);
void* client_messagequeue_processing(void* param);
void* message_dispatcher(void* param);
void* server_communication_outgoing(void* param);
void* server_communication_incoming(void* param);
void* messageForClient(void* param);
void* logout(void* param);
void* executeCommand(void* param);

struct Client_processing
{
    CClient *client;
    CThread *thread_processing;
        //CThread *thread_messagequeue;
    CSLiveDB *db;
};

struct server
{
    CThread *thread;
    CSocket *sock;
    void *chat;
};

struct Command
{
    Client_processing *client;
    string message;
};

class CChat_Server : public CThread/*: public CServer*/
{
private:
    list<Client_processing> clients;
    list<server> server_list;
        //list<CThread*> incoming_messages;
    pthread_t message_dispatcher_obj;
    CThread *thread_server_communication_outgoing;
    CThread *thread_server_communication_incoming;
    CThread *thread_logout;
    CSLiveDB *database;

protected:

public:
    CChat_Server();
    virtual ~CChat_Server();
    
    friend void* accept_new_Clients(void* param);
    friend void* client_processing(void* param);
    friend void* client_messagequeue_processing(void* param);
    friend void* message_dispatcher(void* param);
    friend void* server_communication_outgoing(void* param);
    friend void* server_communication_incoming(void* param);
    friend void* messageForClient(void* param);
    friend void* logout(void *param);
    friend void* executeCommand(void* param);
};


#endif /* defined(__SLive__CChat_Server__) */