//
//  CChat_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CChat_Server.h"
#include "CQueue.h"
#include <pthread.h>
#include <sstream>
#include <stdio.h>

void* accept_new_Clients(void* param)
{
    CChat_Server *server = reinterpret_cast<CChat_Server*>(param);
    CQueue queue(8300);
    queue.set_type(3);
    int client_id = 8300;
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
        
        Client_processing client_obj;
        client_obj.client = new CClient(++client_id, client_socket); // Create a new client
        
        client_obj.thread_messagequeue = new CThread; // create a new thread object for the new client
        client_obj.thread_id_messagequeue = client_obj.thread_messagequeue->start((void*)client_obj.client, client_messagequeue_processing);// start a seperate thread for listening on the msg

        client_obj.thread_processing = new CThread;
        client_obj.thread_id_processing = client_obj.thread_processing->start((void*)&client_obj, client_processing);// start a seperate thread for processing incoming messages

        queue << "Thread gestartet!";
        server->clients.push_back(client_obj); // add the client to the list
    }
    pthread_exit((void*)0);
}

void* client_processing(void* param)
{
    Client_processing *myself_struct = (Client_processing*)param;
    CClient *myself = myself_struct->client;
    CQueue queue_log(8300);
    CQueue client_queue(8302);
    client_queue.set_type(myself->getID());
    queue_log.set_type(3);
    bool logout = false;
    string message;
    string command;
    string parameter = "";
    try
    {
        while(!logout)
        {
            
            myself->getSocket() >> message;
            // parse message 
            std::istringstream s(message);
            do
            {
                string sub;
                s >> sub;
                if(sub.substr(0,1) == "/")
                {
                    command = sub;
                    s >> sub;
                    while( ((sub.substr(0,1)) != "/") && (s))
                    {
                        parameter += sub + " ";
                        s >> sub;
                    }
                }
                
                if(command == "/usr_logout")
                {
                    if(parameter == "true")
                    {
                        logout = true;
                        queue_log << "Client " + std::to_string( myself->getID() ) + " hat sich ausgeloggt!";
                        myself->getSocket().closeSocket();
                        // Send a kill-message to his brother thread
                        CQueue queue(8301);
                        queue.set_type(myself->getID());
                        queue << "ENDE!!!";
                        pthread_exit((void*)0);
                    }
                }
                if(command == "/usr_login")
                {

                }
                if(command == "/send")
                {
                    string ausgabe = "Client " + std::to_string( myself->getID() ) + " sendet " + parameter;
                    queue_log << ausgabe;
                }
                command = "";
                parameter = "";
            }
            while (s);
        }
    }
    catch(string e)
    {
        queue_log.set_type(2);
        queue_log << e;
        queue_log << "Client " + std::to_string( myself->getID() ) + " brach Verbindung ab";
        if(e == "Client terminate connection!")
        {
                //queue_log << "Beende Threads...";
            myself->getSocket().closeSocket();
                //myself_struct->thread_messagequeue->cancel(myself_struct->thread_id_messagequeue);

            CQueue queue(8301);
            queue.set_type(myself->getID());
            queue << "ENDE!!!";
            myself_struct->thread_messagequeue->join(NULL);
            pthread_exit((void*)0);
        }
    }
    pthread_exit((void*)0);
}

void* client_messagequeue_processing(void* param)
{
    CClient *myself = (CClient*)param;
    CQueue queue_log(8300);
    CQueue queue(8301);
    queue.set_type(myself->getID());
    queue_log.set_type(3);
    string message;
    while(true)
    {
        queue >> message;
        if(message == "ENDE!!!")
        {
            queue_log << "Beende thread...";
            pthread_exit((void*)0);
        }
        myself->getSocket() << message;
    }
}

void* message_dispatcher(void* param)
{
    CQueue messages(8302);
    CQueue server2client(8301);
    CQueue logger(8300);

    
    pthread_exit((void*)0);
}
