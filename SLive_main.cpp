//
//  SLive_main.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CLogger.h"
#include "CQueue.h"
#include "CDatabase_Connection.h"
#include "CChat_Server.h"

int main()
{
    CLogger logger;
    CQueue log(8300);
    CChat_Server *chat;

    log.set_type(3);
    pthread_t thread;
    pthread_create( &thread , NULL, CLogger_run, NULL);
    chat = new CChat_Server;
    //pthread_join(thread, NULL);
    while(true)
        sleep(10000);
    //  logger.start(NULL, CLogger_run);
  /*  while(true)
    {
        try
        {
            pthread_join(thread, NULL);
        }
        catch(string e)
        {
            log.set_type(1);
            log << e;
            log << "Starte Logger-Thread neu...";
            log.set_type(3);
            pthread_cancel(thread);
            pthread_create( &thread , NULL, CLogger_run, NULL);
        }
    }*/
            //logger.join(NULL);

    delete chat;
    
    return 0;
}
