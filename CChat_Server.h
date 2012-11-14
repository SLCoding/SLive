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
#include "CServer.h"
#include "CSocket.h"
#include "CConference.h"
#include "CThread.h"
#include "CClient.h"

void* accept_new_Clients(void* param);
void* client_processing(void* param);
    //void* client_messagequeue_processing(void* param);
void* message_dispatcher(void* param);
void* server_communication_outgoing(void* param);
void* server_communication_incoming(void* param);
void* messageForClient(void* param);

struct Client_processing
{
    CClient *client;
    CThread *thread_processing;
        //CThread *thread_messagequeue;
};

class CChat_Server : public CThread/*: public CServer*/
{
private:
    list<Client_processing> clients;
    list<CThread*> incoming_messages;
    pthread_t *message_dispatcher_obj;
    CThread *thread_server_communication_outgoing;
    CThread *thread_server_communication_incoming;
    
protected:
    
public:
    CChat_Server();
    friend void* accept_new_Clients(void* param);
    friend void* client_processing(void* param);
        //friend void* client_messagequeue_processing(void* param);
    friend void* message_dispatcher(void* param);
    friend void* server_communication_outgoing(void* param);
    friend void* server_communication_incoming(void* param);
    friend void* messageForClient(void* param);
    void start_message_dispatcher();
    void logout(CClient *client);
};

#endif /* defined(__SLive__CChat_Server__) */
