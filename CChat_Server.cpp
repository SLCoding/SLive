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

CChat_Server::CChat_Server() /*: CServer()*/
{
    this->thread_server_communication_incoming = new CThread;
    this->thread_server_communication_outgoing = new CThread;

    this->thread_server_communication_incoming->start(reinterpret_cast<void*>(this), server_communication_incoming);
    this->thread_server_communication_outgoing->start(NULL, server_communication_outgoing);
}

void CChat_Server::start_message_dispatcher()
{
    message_dispatcher_obj = this->start(reinterpret_cast<void*>(this), message_dispatcher);
}

void* accept_new_Clients(void* param)
{
    CChat_Server *server = reinterpret_cast<CChat_Server*>(param);
    CQueue queue(8300);
    queue.set_type(3);
    int client_id = 8300;
    CSocket sock;
        //queue << "Socket erstellt...";
    sock.bind(8376);
        // queue << "Port auf Adresse gebunden...";
    sock.listen();
    while(true)
    {
        queue << "warte auf Client-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);
        queue << "client connected";
        
        Client_processing client_obj;
        client_obj.client = new CClient(++client_id, client_socket); // Create a new client

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
                                //CQueue queue(8301);
                                //queue.set_type(myself->getID());
                                //queue << "ENDE!!!";
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
                    // rufe id ab, setze client id manuell nach
                        // myself->setdID(...);
                        // speichere in db anmeldung auf diesem server!
                        //  myself_struct->thread_messagequeue = new CThread; // create a new thread object for the new client
                        //myself_struct->thread_messagequeue->start((void*)myself, client_messagequeue_processing);// start a seperate thread for listening on the msg
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
                //CQueue queue(8301);
                //queue.set_type(myself->getID());
                //queue << "ENDE!!!";
            myself->setLoginStatus(false);
            pthread_exit((void*)0);
        }
    }
    pthread_exit((void*)0);
}
/*
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
        if(message == "ENDE!!!") // string sollte nochmal bedacht werden
        {
            queue_log << "Beende thread...";
            pthread_exit((void*)0);
        }
        queue_log << "Empfange Nachricht fuer: " + std::to_string( myself->getID() ) + " " + message;
        myself->getSocket() << message;
    }
}
*/
// verarbeitet die liste von zu sendenden nachrichten und leitet die nachrichten in die entsprechenden messagequeues weiter
void* message_dispatcher(void* param)
{
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(param);
    CQueue messages(8302);
    CQueue server2server(8303);
    server2server.set_type(10);
    messages.set_type(5);
        //CQueue server2client(8301);
    CQueue logger(8300);
    logger.set_type(3);
    string message;
    istringstream ss;
    bool found = false;
    bool not_search = false;
    while(true)
    {
        messages >> message;
        std::istringstream s(message);
        string id_sender;
        string id_recipient;
        string message;
        s >> id_sender;
        s >> id_recipient;
        s >> message;

        list<Client_processing>::const_iterator iterator;
        for (iterator = chat->clients.begin(); iterator != chat->clients.end(); ++iterator)
        {
            if( iterator->client->getID() == atoi(id_recipient.c_str()))
            {
                if(iterator->client->getLoginStatus() == false)
                {
                        // todo:
                        // überprüfe anmeldung wo anders, wenn nicht
                        // speichere nachricht in datenbank ab für spätere übertragung
                    chat->logout(iterator->client);
                    not_search = true; // wenn nicht woanders angemeldet
                    logger << "Nicht suchen...";
                    break;
                }
                logger << "Sende Nachricht an " + id_recipient + " von sender " + id_sender + " " + message;
                   
                iterator->client->getSocket() << message;
                found = true;
            }
        }

        if((found == false) && (not_search == false))
        {
                // frage datenbank nach standort von user
            // bekomme server ip adresse
            logger << "Client nicht gefunden, kontaktiere Server...leite Nachricht weiter...";
            server2server << id_recipient + " " + id_sender + " " + message + " " + "127.0.0.1"; // + serverip;
        }
        found = false;
        not_search = false;
    }
    pthread_exit((void*)0);
}

    // persistent
void* server_communication_outgoing(void* param)
{
    CQueue log(8300);
    CQueue msg(8303);
    log.set_type(3);
    msg.set_type(10);
    list<CSocket> socks;
    string message2send;
    string message, recipient, sender, ip;
    bool found = false;
    try
    {
        while(true)
        {
            log << "server wartet auf ausgehende nachricht...";
            msg >> message2send;
            log << "server_communication_outgoing hat message empfangen!!";
            std::istringstream ss(message2send);
            ss >> recipient >> sender >> message >> ip;

            list<CSocket>::const_iterator iterator;
            for (iterator = socks.begin(); iterator != socks.end(); ++iterator)
            {
                log << "gespeicherte IPs: " + iterator->getIP();
                if(iterator->getIP() == ip)
                {
                    log << "socket gefunden!! ";
                    iterator->send(recipient + " " + sender + " " + message);
                    found = true;
                }
            }

            if(found == false)
            {
                // no ip adress found, open a new connection
                CSocket newsock;
                log << "baue verbindung zu server auf...";
                newsock.connect(ip, 8377);
                socks.push_back(newsock);
                newsock << recipient + " " + sender + " " + message;
            }
            found = false;
            recipient = sender = message = "";
        }
    }
    catch(string e)
    {
        log.set_type(2);
        log << e;
        if(e == "An error occured while sending the message")
        {
                // fehlerbehandlung implementieren!!!
        }
    }
    pthread_exit((void*)0);
}

    // persistent
void* server_communication_incoming(void* param)
{
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(param);
    CQueue queue(8300);
    queue.set_type(3);
    CSocket sock;
    sock.bind(8377);
    sock.listen();
    while(true)
    {
        queue << "warte auf Server-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);
        queue << "server connected";
        CThread *thread = new CThread;
        thread->start(reinterpret_cast<void*>(&client_socket), messageForClient);
        chat->incoming_messages.push_back(thread);
    }
    pthread_exit((void*)0);
}

void* messageForClient(void* param)
{
    CSocket *sock = reinterpret_cast<CSocket*>(param);
    CQueue log(8300);
    string message2recipe;
    string message, recipient, sender, ip;
    CQueue client(8301);
    log.set_type(3);
    try
    {
        while(true)
        {
            log << "warte auf eingehende message";
            *sock >> message2recipe;
            log << "message eingegangen";
            std::istringstream ss(message2recipe);

            ss >> recipient >> sender >> message;

            client.set_type( atoi(recipient.c_str()) );
            log << "message " + message + " beim client " + recipient + " angekommen!";
            client << message;
        }
    }
    catch(string e)
    {
        log.set_type(2);
        log << e;
        if(e == "Client terminate connection!")
        {
            sock->closeSocket();
            pthread_exit((void*)0);
        }
    }
    pthread_exit((void*)0);
}

void CChat_Server::logout(CClient *client)
{
    CQueue log(8300);
    log.set_type(3);
    list<Client_processing>::const_iterator iterator;
    for (iterator = this->clients.begin(); iterator != this->clients.end(); ++iterator)
    {
        if( iterator->client->getID() == client->getID())
        {
            log << "Client " + std::to_string( client->getID() ) + " hat sich ausgeloggt";
            this->clients.erase(iterator);
        }
    }

}