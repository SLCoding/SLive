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
    string rec;
    CSocket sock;
    queue.send_msg("Socket erstellt...", 3);
    sock.bind(8376);
    queue.send_msg("Port auf Adresse gebunden...", 3);
    sock.listen();
    queue.send_msg("warte auf Client-Anfrage...", 3);
    CSocket client_socket = sock.accept();
    queue.send_msg("Client-Anfrage angenommen", 3);
    client_socket.setBuffer(8192);
    queue.send_msg("Setze Lese-Buffer auf 8192", 3);
    cout << "client connected" << endl << endl;
    try
    {
    while(1)
    {
            //cout << "aktueller socket: " << client_socket.getSocket() << endl;
        queue.send_msg("Warte auf Nachricht vom Client...", 3);
        client_socket >> rec;
        queue.send_msg("Nachricht empfangen", 3);
        queue.send_msg(rec, 3);
            // cout << "vom client empfangen: " << rec;
            // cout << "sende nachricht..." << rec << endl;
            // cout.flush();
        rec = string("nachricht erhalten: ") + rec + string("\n");
        queue.send_msg("Sende Antwortnachricht...", 3);
        client_socket << rec;
        queue.send_msg("Antwort gesendet!", 3);
    }
    }
    catch(string e)
    {
        queue.send_msg( e, 1);
            //queue.send_msg("Der Client hat die Verbindung beendet!", 1);
    }
    return (void*)0;
}