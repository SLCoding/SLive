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

CChat_Server::CChat_Server() : CServer()
{
    ;
}

void CChat_Server::start_message_dispatcher()
{
    message_dispatcher_obj = this->start(reinterpret_cast<void*>(NULL), message_dispatcher);
}

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
        client_obj.thread_messagequeue->start((void*)client_obj.client, client_messagequeue_processing);// start a seperate thread for listening on the msg

        client_obj.thread_processing = new CThread;
        client_obj.thread_processing->start((void*)&client_obj, client_processing);// start a seperate thread for processing incoming messages

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
    client_queue.set_type(5);
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

                // execute parsed message
                if(command == "/usr_logout")
                {
                    if(myself->getLoginStatus())
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
                            myself->setLoginStatus(false);
                            pthread_exit((void*)0);
                        }
                    }
                }
                if(command == "/usr_login")
                {
                    string username, pw;
                    std::istringstream s(parameter);
                    s >> username;
                    s >> pw;

                        //datenbankaufruf um username und pw zu überprüfen
                    myself->getSocket() << "true " + std::to_string( myself->getID() ) + "\n";
                    myself->setLoginStatus(true);
                }
                if(command == "/usr_register")
                {
                    
                }
                if(command == "/send")
                {
                    if(myself->getLoginStatus())
                    {
                        string ausgabe = "Client " + std::to_string( myself->getID() ) + " sendet " + parameter;
                        queue_log << ausgabe;
                        client_queue << std::to_string( myself->getID() ) + " " + parameter;
                    }
                }
                if(command == "/bdy_search")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
               /* if(command == "/bdy_send")
                {

                }*/
                if(command == "/bdy_info")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/bdy_add")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/bdy_remove")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/bdy_list")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/bdy_get_status")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_send")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_create")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_add")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_remove")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_list")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_get_user")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_getProtocol")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/srv_serverdown")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/srv_getList")
                {
                    if(myself->getLoginStatus())
                    {

                    }
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
        if(e == "Client terminate connection!")
        {
            myself->getSocket().closeSocket();
            CQueue queue(8301);
            queue.set_type(myself->getID());
            queue << "ENDE!!!";
            myself->setLoginStatus(false);
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
    queue_log << "id:" + std::to_string(myself->getID());

    string message;
    while(true)
    {
        queue >> message;
        if(message == "ENDE!!!")
        {
            queue_log << "Beende thread...";
            pthread_exit((void*)0);
        }
        queue_log << "Empfange Nachricht fuer: " + std::to_string( myself->getID() ) + " " + message;
        myself->getSocket() << message;
    }
}

// verarbeitet die liste von zu sendenden nachrichten und leitet die nachrichten in die entsprechenden messagequeues weiter
void* message_dispatcher(void* param)
{
    CQueue messages(8302);
    messages.set_type(5);
    CQueue server2client(8301);
    CQueue logger(8300);
    logger.set_type(3);
    string message;
    istringstream ss;
    while(true)
    {
        messages >> message;
        std::istringstream s(message);
        string id_sender;
        string id_empfanger;
        string nachricht;
        s >> id_sender;
        s >> id_empfanger;
        s >> nachricht;

        logger << "Verarbeite Nachricht fuer: " + id_empfanger + " von sender " + id_sender + " " + nachricht;
        server2client.set_type( atoi(id_empfanger.c_str()) );
        server2client << nachricht;
    }
    pthread_exit((void*)0);
}
