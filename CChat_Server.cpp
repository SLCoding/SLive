//
//  CChat_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CChat_Server.h"
#include "CQueue.h"
#include <sstream>

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
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);
        queue << "client connected";
        CClient *client = new CClient(++client_id, client_socket); // Create a new client
        server->clients.push_back(client); // add the client to the list
        server->client_thread.start((void*)client, client_processing);    // start a seperate thread for the new client
        queue << "Thread gestartet!";
    }
    return NULL;
}

void* client_processing(void* param)
{
    CClient *myself = (CClient*)param;
    CQueue queue_log(8300);
    queue_log.set_type(3);
    bool logout = false;
    string message;
    string command;
    try
    {
        while(!logout)
        {
            queue_log << "Warte auf Nachricht";
            myself->getSocket() >> message;
            std::istringstream s(message);
            do
            {
                string sub;
                s >> sub;
                if(sub.substr(0,1) == "/")
                {
                    command = sub;
                    s >> sub;
                    while(!((sub.substr(0,1)) == "/"))
                        command += sub;
                }
                cout << "Command: " << command << endl;
                if(message == "/usr_logout true\n")
                {
                    logout = true;
                    break;
                    queue_log << "Client hat sich ausgeloggt!";
                }
                queue_log << message;
                myself->getSocket() << message;
            }
            while (s);
        }
    }
    catch(string e)
    {
        queue_log.set_type(2);
        queue_log << e;
    }
    return NULL;
}
