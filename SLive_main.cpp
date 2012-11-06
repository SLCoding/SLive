//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CServer_Manager.h"
#include "CChat_Server.h"
#include "CMusic_Server.h"
#include "CClient.h"
#include "CServer.h"
#include "CSocket.h"
#include "CDatabase_Connection.h"
#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "CQueue.h"

using namespace std;

int main()
{
    /*
    CDatabase_Connection db;
    map<string, string> result;
    db.setUsername("root");
    db.setPassword("");
    db.setDB("stundenplan");
    db.setHost("127.0.0.1");
    if(db.connect())
    {
        cout << "db connection OK" << endl;
        cout << "serverinfo: " << db.get_server_info() << endl;
        cout << "serverversion: " << db.get_server_version()<< endl;
        cout << "clientinfo: " << db.get_client_info() << endl;
        cout << "clientversion: " << db.get_client_version() << endl;
        cout << "host info: " << db.get_host_info() << endl;
        cout << "proto info: " << db.get_proto_info() << endl;
        cout << "thread id: " << db.thread_id() << endl;
        cout << "character_set_name: " << db.character_set_name() << endl;
        cout << "debug_info: " << db.dump_debug_info() << endl;
        cout << "stat: " << db.stat() << endl;
    }
    else
    {
        cout << "fehler: " << db.error() << endl; 
    }
    string sql = "SELECT * FROM stundenplan;";
    if(db.query(sql, (int)sql.length()))
    {
        cout << "query ok" << endl;
    }
    else
    {
        cout << "query not ok!" << endl;
    }

    cout << "affected rows: " << db.affected_rows() << endl;
    try
    {
        int i = 0;
        while(i < db.affected_rows())
        {
            result = db.fetch_assoc() ;
            cout << "Raum: " << result["Raum"] << endl;
            i++;
        }
    }
    catch(char *e)
    {
        cout << e << endl;
    }
    catch(string e)
    {
        cout << e << endl;
    }
     */
    
    CChat_Server chat;
    CMusic_Server music;
    CLogger logger;
    
    int ret4 = logger.start(NULL, CLogger_run);
    CServer_Manager manager;
    /* hole die Messagetypnummer aus dem ersten Parameter */
    
    logger.join(ret4, NULL);
    
    return 0;
}
