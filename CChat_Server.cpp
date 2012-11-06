//
//  CChat_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CChat_Server.h"
#include "CQueue.h"

void* accept_new_Clients(void* param)
{
    CChat_Server *server = reinterpret_cast<CChat_Server*>(param);
    CQueue queue(8300);
    queue.set_type(3);
    int client_id = 8300;
    string rec;
    CSocket sock;
    queue << "Socket erstellt...";
    sock.bind(8376);
    queue << "Port auf Adresse gebunden...";
    sock.listen();
    while(true)
    {
        queue << "warte auf Client-Anfrage...";
            //client_socket;
        CSocket client_socket = sock.accept();
            // client_socket << "Hallo Test\n";
            //queue << "Client-Anfrage angenommen";
        client_socket.setBuffer(8192);
            //client_socket >> rec;
            //queue << rec;
        // queue << "Setze Lese-Buffer auf 8192";
        queue << "client connected";
        CClient *client = new CClient(++client_id, &client_socket); // Create a new client
        server->clients.push_back(client); // add the client to the list
        server->client_thread.start(reinterpret_cast<void*>(client), client_processing);    // start a seperate thread for the new client
        queue << "Thread gestartet!";
    }
    return NULL;
}

void* client_processing(void* param)
{
    CThread thread;
    CClient *myself = reinterpret_cast<CClient*>(param);
        // CSocket sock = (myself->getSocket())->getSocket();
    CSocket *sock = myself->getSocket();
        // CQueue queue(myself->getID());
    CQueue queue_log(8300);
        // queue.set_type(3);
    queue_log.set_type(3);
    bool logout = false;
    string message;
    while(!logout)
    {
        queue_log << "Warte auf Nachricht";
        (*sock) >> message;
        queue_log << "Nachricht empfangen";
        if(message == "/usr_logout true")
        {
            logout = true;
        queue_log << "Client hat sich ausgeloggt!";
        }
        queue_log << message;
            //thread.start(reinterpret_cast<void*>(&message), processing_message);
        (*sock) << "Hallo\n";
    }
    return NULL;
}

void* processing_message(void* param)
{
    string *message = reinterpret_cast<string*>(param);
    CQueue queue(8300);
    queue.set_type(3);
    queue << *message;

    return NULL;
}