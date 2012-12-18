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
    try
    {
        CLogger logger;
        CQueue log(8300);
        CChat_Server *chat;

        log.set_type(3);
        pthread_t thread;
        pthread_create( &thread , NULL, CLogger_run, NULL);
        chat = new CChat_Server;
        pthread_join(thread, NULL);
        //  logger.start(NULL, CLogger_run);
        //logger.join(NULL);

        delete chat;
    }
    catch(exception e)
    {
        cout << "ERROR: " << e.what() << endl;
        return -1;
    }
    catch(...)
    {
        cout << "ERROR: main()";
        return -1;
    }
    return 0;
}
