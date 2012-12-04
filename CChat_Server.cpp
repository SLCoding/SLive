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
#include "CSLiveDB.h"

CChat_Server::CChat_Server() /*: CServer()*/
{
    this->database = new CSLiveDB("", "", "", "", 0);

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

    CSocket sock;

    int client_id = 8300;

    queue.set_type(3);

    sock.bind(8376);
    sock.listen();

    while(true)
    {
        queue << "warte auf Client-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);
        queue << "client connected";

        Client_processing client_obj;
        client_obj.client = new CClient(++client_id, client_socket); // Create a new client

        client_obj.db = server->database;

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
    cUser *user;
    CClient *myself = myself_struct->client;

    CQueue queue_log(8300);
    CQueue client_queue(8302);

    bool logout = false;

    string message;
    string command;
    string parameter = "";

    client_queue.set_type(5);
    queue_log.set_type(3);

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

                            user->set_server("");

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

                    bool login = myself_struct->db->checkUsername(username);
                        //bool pw = myself_struct->db->

                    if(login == true)
                    {
                        user = &myself_struct->db->get_User(username);
                        myself->setID(user.get_id());
                        user.set_server( (myself->getSocket()).getLocalIP() );
                        
                        myself_struct->thread_messagequeue = new CThread; // create a new thread object for the new client
                        myself_struct->thread_messagequeue->start((void*)myself, client_messagequeue_processing);// start a seperate thread for listening on the msg
                        
                        myself->getSocket() << "/login 1 " + std::to_string( myself->getID() ) + "\n";
                        myself->setLoginStatus(true);
                    }
                    else
                        myself->getSocket() << "/login 0 0\n";
                }
                if(command == "/usr_register")
                {
                    string username, pw;
                    std::istringstream s(parameter);
                    bool name_in_use = true;

                    s >> username;
                    s >> pw;

                    name_in_use = myself_struct->db->checkUsername(username);
                    if(name_in_use)
                    {
                        myself->getSocket() << "/usr_register name_already_in_use\n";
                    }
                    else
                    {
                        user = &myself_struct->db->create_User(username); // noch zu aktualisieren
                        myself->getSocket() << "/usr_register 1\n";
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
                if(command == "/conf_create")
                {
                    if(myself->getLoginStatus())
                    {

                    }
                }
                if(command == "/conf_send")
                {
                    if(myself->getLoginStatus())
                    {
                        string ausgabe = "Client " + std::to_string( myself->getID() ) + " sendet " + parameter;
                        queue_log << ausgabe;
                        client_queue << std::to_string( myself->getID() ) + " " + parameter;
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
        if(message == "ENDE!!!") // string sollte nochmal bedacht werden
        {
            queue_log << "Beende thread...";
            pthread_exit((void*)0);
        }
        queue_log << "Empfange Nachricht fuer: " + std::to_string( myself->getID() ) + " " + message;
        myself->getSocket() << message;
    }
}

    // verarbeitet die liste von zu sendenden nachrichten und leitet die nachrichten in die entsprechenden sockets weiter oder an einen zielserver
void* message_dispatcher(void* param)
{
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(param);

    CQueue messages(8302);
    CQueue server2server(8303);
    CQueue logger(8300);

    string message;
    istringstream ss;

    bool found = false;
    bool search = true;
    bool found_db = true;

    server2server.set_type(10);
    messages.set_type(5);
    logger.set_type(3);

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

        found = false;
        found_db = true;
        search = true;

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

                    found_db = true; // oder false, db aufruf abhängig
                    chat->logout(iterator->client);
                    search = false; // wenn nicht woanders angemeldet
                    logger << "Nicht suchen...";
                    break;
                }
                else
                {
                        //map<string, string> bdy_name = chat->database->getUserById(id_sender);

                    logger << "Sende Nachricht an " + id_recipient + " von sender " + id_sender + " " + message;

                    iterator->client->getSocket() << "/conf_send " << id_recipient << " " << id_sender << " " << message << "\n";
                    found = true;
                }
            }
        }

        if(found == true)
            continue;

        if((found == false) && (search == true) && (found_db == true))
        {
                // frage datenbank nach standort von user
                // bekomme server ip adresse

            logger << "Client nicht gefunden, kontaktiere Server...leite Nachricht weiter...";
            server2server << id_recipient + " " + id_sender + " " + message + " " + "127.0.0.1"; // + serverip;
        }
    }
    pthread_exit((void*)0);
}

    // persistent
void* server_communication_outgoing(void* param)
{
    CQueue log(8300);
    CQueue msg(8303);

    list<CSocket> socks;

    string message2send;
    string message;
    string recipient;
    string sender;
    string ip;

    bool found = false;

    log.set_type(3);
    msg.set_type(10);

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

    CSocket sock;

    queue.set_type(3);

    sock.bind(8377);
    sock.listen();

    while(true)
    {
        queue << "warte auf Server-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);

        server incoming_server;

        queue << "server connected";
        CThread *thread = new CThread;
        incoming_server.thread = thread;
        incoming_server.sock = &client_socket;
        incoming_server.chat = reinterpret_cast<void*>( chat );
            // chat->incoming_messages.push_back(thread);
        chat->server_list.push_back(incoming_server);
        thread->start(reinterpret_cast<void*>(&incoming_server), messageForClient);
    }
    pthread_exit((void*)0);
}

void* messageForClient(void* param)
{
    server *incoming_server = reinterpret_cast<server*>(param);
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(incoming_server->chat);

    CQueue log(8300);
    CQueue client(8301);

    string message2recipe;
    string message;
    string recipient;
    string sender;
    string ip;

    bool found = false;

    log.set_type(3);

    try
    {
        while(true)
        {
            log << "warte auf eingehende message";
            message2recipe = incoming_server->sock->recv();
            log << "message eingegangen";
            std::istringstream ss(message2recipe);

            ss >> recipient >> sender >> message;
            list<Client_processing>::const_iterator iterator;
            for (iterator = chat->clients.begin() ; iterator != chat->clients.end(); ++iterator)
            {
                if(iterator->client->getID() == atoi( recipient.c_str()) )
                {
                    log << "client gefunden!! ";
                    log << "message " + message + " beim client " + recipient + " angekommen!";
                    iterator->client->getSocket() << recipient + " " + sender + " " + message;
                    log << "message gesendet";
                    found = true;
                }
            }
                //client.set_type( atoi(recipient.c_str()) );

                //client << message;
        }
    }
    catch(string e)
    {
        log.set_type(1);
            // log << e;
        if(e == "Client terminate connection!")
        {
            log << "Server terminate connection!";

            list<server>::const_iterator iterator;
            for (iterator = chat->server_list.begin() ; iterator != chat->server_list.end(); ++iterator)
            {
                    // if the iterator object and the server object equal
                if((memcmp( reinterpret_cast<void*>( &iterator ),reinterpret_cast<void*>( incoming_server ), sizeof(server))) == 0)
                {
                    incoming_server->sock->closeSocket();
                        // delete server object in the list
                    chat->server_list.erase(iterator);
                        // kill thread
                    pthread_exit((void*)0);
                }
            }    
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