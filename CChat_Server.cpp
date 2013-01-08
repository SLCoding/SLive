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
#include <stdlib.h>
#include "CSLiveDB.h"
#include <time.h>

/*******************************************************************************
 * Initialise Server, initialise db-connection, starting several threads
 ******************************************************************************/
CChat_Server::CChat_Server()
{
    CQueue logger(8300);
    logger.set_type(2);
    try
    {
        this->thread_server_communication_incoming = new CThread;
        this->thread_server_communication_outgoing = new CThread;
        //this->thread_logout = new CThread;

        this->thread_server_communication_incoming->start(reinterpret_cast<void*>(this), server_communication_incoming);
        this->thread_server_communication_outgoing->start(NULL, server_communication_outgoing);
        // this->thread_logout->start(reinterpret_cast<void*>(this), logout);

        message_dispatcher_obj = this->start(reinterpret_cast<void*>(this), message_dispatcher);
        this->start(reinterpret_cast<void*>(this), accept_new_Clients);
        
        this->database = new CSLiveDB("SLive2", "SLive2", "SLive2", "127.0.0.1", "127.0.0.1", 3306, 3306);
    }
    catch(string e)
    {
        logger << e;
        logger << "Server shutdown...";
        logger.destroy();
        this->~CChat_Server();
    }
    catch(std::exception e)
    {
        logger << e.what();
        logger << "Server shutdown...";
        logger.destroy();
        this->~CChat_Server();
    }
}

/*******************************************************************************
 * Clean up all allocated ressources and kill open threads
 ******************************************************************************/
CChat_Server::~CChat_Server()
{
    try
    {
        list<Client_processing>::iterator iterator1;
        for (iterator1 = clients.begin(); iterator1 != clients.end(); ++iterator1)
        {
            delete iterator1->client;
            delete iterator1->db;
            delete iterator1->thread_processing;
            clients.erase(iterator1);
        }

        list<server>::iterator iterator2;
        for (iterator2 = server_list.begin(); iterator2 != server_list.end(); ++iterator2)
        {
            delete iterator2->sock;
            delete iterator2->thread;
            server_list.erase(iterator2);
        }

        //thread_server_communication_incoming->cancel();
        thread_server_communication_outgoing->cancel();
        this->cancel();
        delete message_dispatcher_obj;
        // delete thread_server_communication_incoming;
        delete thread_server_communication_outgoing;
        //delete thread_logout;
        delete database;
    }
    catch(std::exception e)
    {
        cout << "FATAL ERROR: Destructor crashed...wayne, the programm stops anyway ;-)";
    }
}

/*******************************************************************************
 * Waiting for an inquiry of a client and start a seperate thread
 ******************************************************************************/
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
        queue << "Warte auf Client-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);
        queue << "Neuer Client verbunden!";

        Client_processing client_obj;
        client_obj.client = new CClient(++client_id, client_socket); // Create a new client
        
        client_obj.db = server->database;

        client_obj.thread_processing = new CThread;
        client_obj.thread_processing->start((void*)&client_obj, client_processing);// start a seperate thread for processing incoming messages

        //queue << "Thread gestartet!";
        server->clients.push_back(client_obj); // add the client to the list
    }
    pthread_exit((void*)0);
}

/*******************************************************************************
 * Each client is running in this thread, receive over network a command,
 * execute it and send the answer back
 ******************************************************************************/
void* client_processing(void* param)
{
    bool login = false;
    Client_processing *myself_struct = (Client_processing*)param;
    cUser user;
    CClient *myself = myself_struct->client;

    CQueue queue_log(8300);
    CQueue client_queue(8302);

    bool logout = false;

    string message;
    string command;
    string parameter = "";
    string buffer;
    stringstream sonderzeichen;
    
    client_queue.set_type(1);
    queue_log.set_type(3);
    
    while(true)
    {
        try
        {
            while(!logout)
            {
                myself->getSocket() >> message;
                cout << message;
               
                if(message.length() < 2)
                    continue;

                if(message.length() > 1024)
                {
                    myself->getSocket() << "Message too largely!";
                    continue;
                }
                
                do
                {
                    buffer = message.substr((message).find_first_of("/"), message.find_first_of("\n"));
                    std::istringstream ss(buffer);
                    // filtert den eigentlichen befehl
                    ss >> command;
                    // löscht den aktuellen datensatz aus dem gesamtstring
                    message.replace(0, buffer.length()+1, "");
                    // parsed die parameter aus dem datensatz
                    parameter = buffer.substr(buffer.find_first_of(" ") + 1, buffer.find_first_of("\n"));

                    std::istringstream s(parameter);

                    if(parameter.substr(0, 1) == "/")
                        parameter = "";

                    queue_log.set_type(3);
                    //queue_log << "BEFEHL: " + command;
                    //queue_log << "PARAM: " + parameter;
                    //queue_log << "ISTRINGSTREAM: " + s.str();
                    //execute parsed message

                    if(command == "/usr_logout")
                    {
                        if(login)
                        {
                            if(parameter == "true")
                            {
                                try
                                {
                                    logout = true;
                                    stringstream buffer;
                                    buffer << "Client " << myself->getID() << " hat sich ausgeloggt!";
                                    queue_log << buffer.str();
                                    myself->getSocket().closeSocket();
                                    user.logout();

                                    // Send a kill-message to his brother thread
                                    //CQueue queue(8301);
                                    //queue.set_type(myself->getID());
                                    //queue << "ENDE!!!";
                                    myself->setLoginStatus(false);
                                    myself->setLogoutStatus(true);
                                    //user->set_server("");
                                    
                                    pthread_exit((void*)0);
                                }
                                catch(string e)
                                {
                                    queue_log << e;
                                }
                                catch(exception e)
                                {
                                    queue_log << e.what();
                                }
                                catch(...)
                                {
                                    queue_log << "Error beim ausloggen...";
                                }
                            }
                         }
                    }
                    if(command == "/usr_login")
                    {
                        if(!login)
                        {
                            string username, pw;
                            s >> username >> pw;
                            queue_log << "LOGIN";
                            queue_log << "Username " + username;
                            queue_log << "pw " + pw;
                            try
                            {
                                user = myself_struct->db->login(username, pw, (myself->getSocket()).getLocalIP() );

                                queue_log << "User " + username + " eingeloggt";
                                myself->setID((int)user.get_id());

                                //myself_struct->thread_messagequeue = new CThread; // create a new thread object for the new client
                                //myself_struct->thread_messagequeue->start((void*)myself, client_messagequeue_processing);// start a seperate thread for listening on the msg
                                stringstream buffer;
                                buffer << "/usr_login 1 " << myself->getID() << "\n";
                                myself->getSocket() << buffer.str();
                                login = true;
                            }
                            catch(string e)
                            {
                                queue_log << "Loginversuch fehlgeschlagen...";
                                //if(e == "wrong username or password")
                                try
                                {
                                    myself->getSocket() << "/usr_login 0 0\n";
                                }
                                catch(string e)
                                {
                                    queue_log << e;
                                }
                            }
                            catch(...)
                            {
                                try
                                {
                                    myself->getSocket() << "/usr_login 0 0\n";
                                }
                                catch(string e)
                                {
                                    queue_log << e;
                                }
                            }
                        }
                    }
                    if(command == "/usr_register")
                    {
                        string username, pw, email;
                        bool name_available = true;

                        s >> username >> pw >> email;
                        queue_log << "User-Registrierung";
                        queue_log << "Username " + username << "Passwort " + pw << "Email " + email;

                        try
                        {
                            name_available = myself_struct->db->checkUsername(username);
                            if(!name_available)
                            {
                                queue_log << "Registrierung fehlgeschlagen...Username in use!";
                                myself->getSocket() << "/usr_register name_already_in_use\n";
                            }
                            else
                            {
                                queue_log << "Registrierung erfolgreich!";
                                user = myself_struct->db->create_User(username, pw, email);
                                myself->getSocket() << "/usr_register 1\n";
                            }
                        }
                        catch(string e)
                        {
                            queue_log << e;
                        }
                    }
                    if(command == "/bdy_info")
                    {
                        if(login)
                        {
                            try
                            {
                                string user_id;
                                string answer = "/bdy_info";
                                s >> user_id;
                                answer += " " + user_id + " " + myself_struct->db->get_User(atoi(user_id.c_str())).get_name() + "\n";
                                myself->getSocket() << answer;
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_bdy_info")
                    {
                        if(login)
                        {
                            string conf_id;
                            string user_id;
                            string answer = "/conf_bdy_info";
                            try
                            {
                                s >> conf_id >> user_id;
                                answer += " " + conf_id + " " + user_id + " " + myself_struct->db->get_User(atoi(user_id.c_str())).get_name() + "\n";
                                //queue_log << answer;
                            
                                myself->getSocket() << answer;
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/bdy_add")
                    {
                        if(login)
                        {
                            queue_log << "bdy_add";
                            bool bodyadd = false;
                            string userid;
                            try
                            {
                                s >> userid;
                                bodyadd = user.add_bdy( atoi( userid.c_str() ) );
                            
                                if(bodyadd)
                                {
                                    queue_log << "hinzufuegen erfolgreich!";
                                    myself->getSocket() << "/bdy_add 1\n";
                                }
                                else
                                {
                                    queue_log << "hinzufuegen fehlgeschlagen...";
                                    myself->getSocket() << "/bdy_add 0\n";
                                }
                            }
                            catch(string e)
                            {
                                queue_log << e;
                                continue;
                            }
                        }
                    }
                    if(command == "/bdy_remove")
                    {
                        if(login)
                        {
                            queue_log << "bdy_remove";
                            bool bodyadd = false;
                            string userid;
                            try
                            {
                                s >> userid;
                                bodyadd = user.del_bdy( atoi( userid.c_str() ) );
                                if(bodyadd)
                                {
                                    queue_log << "entfernen erfolgreich";
                                    myself->getSocket() << "/bdy_remove 1\n";
                                }
                                else
                                {
                                    queue_log << "entfernen fehlgeschlagen!";
                                    myself->getSocket() << "/bdy_remove 0\n";
                                }
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/bdy_list")
                    {
                        if(login)
                        {
                            try
                            {
                                list<cUser> userliste;
                                userliste = user.get_bdyList();
                                stringstream answer;
                                answer << command << " ";
                                list<cUser>::iterator iterator;
                                for (iterator = userliste.begin(); iterator != userliste.end(); ++iterator)
                                {
                                    answer << iterator->get_id() << " ";
                                }
                                answer << "\n";
                                //queue_log << answer.str();

                                myself->getSocket() << answer.str();
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/bdy_get_status")
                    {
                        if(login)
                        {
                            string userid;
                            stringstream buffer;
                            try
                            {
                                s >> userid;
                                cUser temp = myself_struct->db->get_User(atoi(userid.c_str()));
                                user_status status = temp.get_status();
                                buffer << command << " " << userid << " " << status << "\n";
                                //queue_log << buffer.str();
                            
                                myself->getSocket() << buffer.str();
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_create")
                    {
                        if(login)
                        {
                            list <cUser> userlist;
                            string user;
                            stringstream userliste;
                            userliste << command << " ";
                            try
                            {
                                while(s)
                                {
                                    s >> user;
                                    if(user != "")
                                    {
                                        userliste << user;
                                        userlist.push_back(myself_struct->db->get_User(atoi(user.c_str())));
                                    }
                                    user = "";
                                }
                                queue_log << userliste.str();
                                myself_struct->db->create_conf(userlist);
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_send")
                    {
                        if(login)
                        {
                            string conf_id;
                            string nachricht;
                            string buffer_message;
                            try
                            {
                                s >> conf_id;
                                //cout << "/conf_send...." << endl;
                                while(s.good())
                                {
                                    s >> buffer_message;
                                    nachricht += buffer_message + " ";
                                    buffer_message = "";
                                }

                                cConference temp = myself_struct->db->get_Conf(conf_id);
                                stringstream ausgabe;
                                //ausgabe << "Client " << myself->getID() << " sendet " << nachricht;
                                //queue_log << ausgabe.str();
                                cout << ausgabe.str() << endl;
                                stringstream log;
                                stringstream buffer;
                                list <cUser> userliste = temp.get_usrList();
                                list<cUser>::iterator iterator;
                                for (iterator = userliste.begin(); iterator != userliste.end(); ++iterator)
                                {
                                    if(iterator->get_id() != user.get_id())
                                    {
                                        if(iterator->get_status() != OFFLINE)
                                        {
                                            buffer << conf_id << " " << myself->getID() << " " << iterator->get_id() << " " << nachricht;
                                            client_queue << buffer.str();
                                            buffer.str("");
                                        }
                                    }
                                }
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                            catch(exception e)
                            {
                                queue_log << e.what();
                            }
                            catch(...)
                            {
                                queue_log << "UNEXPECTED ERROR: /conf_send";
                            }
                        }
                    }
                    if(command == "/conf_add")
                    {
                        if(login)
                        {
                            try
                            {
                                string conf_id;
                                string user_id;
                                s >> conf_id;
                                s >> user_id;
                                myself_struct->db->get_Conf(conf_id).add_usr(atoi(user_id.c_str())); // holt cConference objekt aus der datenbank und fügt user hinzu
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_remove")
                    {
                        if(login)
                        {
                            try
                            {
                                string conf_id;
                                string user_id;
                                s >> conf_id;
                                s >> user_id;
                                myself_struct->db->get_Conf(conf_id).del_usr(atoi(user_id.c_str())); // holt cConference objekt aus der datenbank und entfernt user
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_list")
                    {
                        if(login)
                        {
                            try
                            {
                                list<cConference> conf_list = user.get_confList();
                                stringstream answer;
                                answer << command << " ";
                                list<cConference>::iterator iterator;
                                for (iterator = conf_list.begin(); iterator != conf_list.end(); ++iterator)
                                {
                                    answer << iterator->get_id() << " ";
                                }
                                answer << "\n";
                                //queue_log << answer.str();

                                myself->getSocket() << answer.str();
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                    if(command == "/conf_get_user")
                    {
                        if(login)
                        {
                            string conf_id;
                            stringstream answer;
                            try
                            {
                                answer << command << " ";
                                s >> conf_id;
                                answer << conf_id;
                                cConference conference = myself_struct->db->get_Conf(conf_id);
                                list <cUser> userliste = conference.get_usrList();
                                list<cUser>::iterator iterator;
                                for (iterator = userliste.begin(); iterator != userliste.end(); ++iterator)
                                {
                                    answer << " " << iterator->get_id();
                                }
                                answer << "\n";
                                myself->getSocket() << answer.str();
                            }
                            catch(string e)
                            {
                                queue_log << e;
                            }
                        }
                    }
                }
                while(message.length() > 0);
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
                user.logout();
                myself->setLoginStatus(false);
                myself->setLogoutStatus(true);
                pthread_exit((void*)0);
            }
            else
            {
                queue_log << "restart endlosschleife...";
                continue;
            }
        }
        catch(std::exception e)
        {
            queue_log.set_type(1);
            queue_log << e.what();
            queue_log.set_type(3);
            continue;
        }
        catch(...)
        {
            queue_log << "UNEXPECTED ERROR: continue...";
            continue;
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

/*******************************************************************************
 * This thread receive messages via a queue from other threads.
 * If a client is connected local, the message will send to this client.
 * If not the message will deliver to server_communication_outgoing.
 ******************************************************************************/
void* message_dispatcher(void* param)
{
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(param);


    CQueue logger(8300);
    CQueue messages(8302);
    CQueue server2server(8303);

    string message;
    istringstream ss;
    string nachricht;

    logger.set_type(3);
    messages.set_type(1);
    server2server.set_type(1);

    while(true)
    {
        messages >> message;
        //logger << "MESSAGEDISPATCHER: " + message;
        //cout << "MESSAGEDISPATCHER: " + message << endl;
        std::istringstream s(message);
        string id_sender;
        string conf_id;
        string id_recipient;
        string message;
        string buffer;

        try
        {            
            s >> conf_id >> id_sender >> id_recipient;
            while(s.good())
            {
                s >> buffer;
                nachricht += buffer + " ";
                buffer = "";
            }
            cUser sender = chat->database->get_User(atoi(id_sender.c_str())); // hole daten für sender aus datenbank
            cUser recipient = chat->database->get_User(atoi(id_recipient.c_str())); // hole daten für empfänger aus datenbank
            list<Client_processing>::iterator iterator;
            for (iterator = chat->clients.begin(); iterator != chat->clients.end(); ++iterator)
            {
                if(sender.get_server() == recipient.get_server()) //user ist lokal angemeldet
                {
                    //logger << "User " + iterator->client->getID() + " ist lokal angemeldet!";
                    //cout << "User " << iterator->client->getID() << " ist lokal angemeldet!" << endl;
                    if( iterator->client->getID() == atoi(id_recipient.c_str()))
                    {
                        //logger << "Sende Nachricht an " + id_recipient + " von sender " + id_sender + " " + nachricht;
                        try
                        {
                            (iterator->client->getSocket()).send("/conf_send " + conf_id + " " +  sender.get_name() + " " + nachricht + "\n");
                            break;
                        }
                        catch(string e)
                        {
                            logger << e;
                            break;
                        }
                    }
                }
                else if((sender.get_server() != recipient.get_server()) && (recipient.get_server() != ""))    // user ist auf entferntem rechner angemeldet
                {
                    logger << "Client nicht lokal angemeldet, kontaktiere Server...leite Nachricht weiter...";
                    server2server << conf_id + " " + id_recipient + " " + id_sender + " " + recipient.get_server() + " " + nachricht;
                    break;
                }
                else
                {
                    logger << "Keinen Server gefunden...";// TODO nicht angemeldet, daten in db hinterlegen
                }
            }
        }
        catch(string e)
        {
            logger.set_type(1);
            logger << e;
            logger.set_type(3);
        }
        message = nachricht = buffer = "";
    }
    pthread_exit((void*)0);
}

/*******************************************************************************
 * Reveive a message from message_dispatcher and lookup the destination ip.
 * Is a connection established use it or connect to the server and send the
 * message.
 ******************************************************************************/
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
    string conf_id;
    string nachricht;

    bool found = false;

    log.set_type(3);
    msg.set_type(1);

    try
    {
        while(true)
        {
            log << "server wartet auf ausgehende nachricht...";
            msg >> message2send;
            log << "server_communication_outgoing hat message empfangen!!";
            std::istringstream ss(message2send);
            ss >> conf_id >> recipient >> sender >> ip;
            while(ss.good())
            {
                ss >> message;
                nachricht += message + " ";
                message = "";
            }

            list<CSocket>::iterator iterator;
            for (iterator = socks.begin(); iterator != socks.end(); ++iterator)
            {
                log << "gespeicherte IPs: " + iterator->getIP();
                if(iterator->getIP() == ip)
                {
                    log << "socket gefunden!! ";
                    iterator->send(conf_id + " " + recipient + " " + sender + " " + nachricht);
                    found = true;
                }
            }

            if(found == false)
            {
                CSocket newsock;
                try
                {
                    // no ip adress found, open a new connection
                    log << "Baue eine neue Verbindung zu server " + ip + " auf...";
                    newsock.connect(ip, 8377);
                    newsock.send(conf_id + " " + recipient + " " + sender + " " + nachricht);
                }
                catch(string e)
                {
                    log.set_type(1);
                    log << e;
                    log.set_type(3);
                }
                socks.push_back(newsock);
            }
            found = false;
            recipient = sender = message = message2send = "";
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

/*******************************************************************************
 * A server can connect to this server via this thread. For every server
 * a new thread is created.
 ******************************************************************************/

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
        queue << "Warte auf Server-Anfrage...";
        CSocket client_socket = sock.accept();
        client_socket.setBuffer(8192);

        server incoming_server;

        queue << "Neuer Server verbunden!";
        CThread *thread = new CThread;
        incoming_server.thread = thread;
        incoming_server.sock = &client_socket;
        incoming_server.chat = reinterpret_cast<void*>( chat );
        chat->server_list.push_back(incoming_server);
        thread->start(reinterpret_cast<void*>(&incoming_server), messageForClient);
    }
    pthread_exit((void*)0);
}

/*******************************************************************************
 * Every server runs in this thread. The thread receive a message and search
 * in a local list for the recipient and send the message
 ******************************************************************************/

void* messageForClient(void* param)
{
    server *incoming_server = reinterpret_cast<server*>(param);
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(incoming_server->chat);

    CQueue log(8300);

    string message2recipe;
    string message;
    string id_recipient;
    string id_sender;
    string conf_id;
    string zeit;
    string nachricht;

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

            ss >> conf_id >> id_recipient >> id_sender;
            while(ss.good())
            {
                ss >> message;
                nachricht += message + " ";
                message = "";
            }
            
            cUser sender = chat->database->get_User(atoi(id_sender.c_str())); // hole daten für sender aus datenbank

            list<Client_processing>::iterator iterator;
            for (iterator = chat->clients.begin() ; iterator != chat->clients.end(); ++iterator)
            {
                if(iterator->client->getID() == atoi( id_recipient.c_str()) )
                {
                    log << "client gefunden!! ";
                    log << "message " + message + " beim client " + id_recipient + " angekommen!";
                    iterator->client->getSocket() << "/conf_send " << conf_id << " " << zeit << " " <<  sender.get_name() << " " << nachricht << "\n";
                    log << "message gesendet";
                    found = true;
                }
            }
        }
    }
    catch(string e)
    {
        log.set_type(1);
            // log << e;
        if(e == "Client terminate connection!")
        {
            log << "Server terminate connection!";

            list<server>::iterator iterator;
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

/*******************************************************************************
 * Every 60 seconds this thread iterate through the local client list and
 * delete all objects that not used.
 ******************************************************************************/
void* logout(void* param)
{
    CChat_Server *chat = reinterpret_cast<CChat_Server*>(param);
    CQueue log(8300);
    stringstream buffer;
    log.set_type(3);
    list<Client_processing>::iterator iterator;
    sleep(60*1); // sleep for 10 Minuten
    while(true)
    {
        try
        {
            log << "Clean up client-list...";
            for (iterator = chat->clients.begin(); iterator != chat->clients.end(); ++iterator)
            {
                //if( iterator->db->get_User(iterator->client->getID()).get_server() == "")   // client ist nicht irgendwo angemeldet
                if( iterator->client->getLogoutStatus() == true)
                {
                    buffer << "Client " << iterator->client->getID() << " hat sich ausgeloggt";
                    log << buffer.str();
                    chat->clients.erase(iterator);
                    buffer.str("");
                    break;
                }
                sleep(1); // zur last und netzwerkreduzierung warte nach jedem client eine sekunde
                buffer.clear();
            }
        }
        catch(exception e)
        {
            log.set_type(2);
            log << e.what();
            log.set_type(3);
            continue;
        }
        sleep(60*1); // sleep for 10 Minuten
    }
    pthread_exit((void*)0);
}