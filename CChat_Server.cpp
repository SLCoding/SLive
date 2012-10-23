//
//  CChat_Server.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CChat_Server.h"
#include "CQueue.h"

void* CChat_Server_run(void* param)
{
    CQueue queue(8300);
    queue.set_type(3);
    
    string rec;
    CSocket sock;
    queue << "Socket erstellt...";
    sock.bind(8376);
    queue << "Port auf Adresse gebunden...";
    sock.listen();
    queue << "warte auf Client-Anfrage...";
    CSocket client_socket = sock.accept();
    queue << "Client-Anfrage angenommen";
    client_socket.setBuffer(8192);
    queue << "Setze Lese-Buffer auf 8192";
    cout << "client connected" << endl << endl;
    try
    {
    while(1)
    {
            //cout << "aktueller socket: " << client_socket.getSocket() << endl;
        queue << "Warte auf Nachricht vom Client...";
        client_socket >> rec;
        queue << "Nachricht empfangen";
        queue << rec;
            // cout << "vom client empfangen: " << rec;
            // cout << "sende nachricht..." << rec << endl;
            // cout.flush();
        queue << "Sende Antwortnachricht...";
        client_socket << string("nachricht erhalten: ") + rec + string("\n");
        queue << "Antwort gesendet!";
    }
    }
    catch(string e)
    {
        queue.set_type(1);
        queue << e;
            //queue.send_msg("Der Client hat die Verbindung beendet!", 1);
    }
    return (void*)0;
}