//
//  CLogger.cpp
//  SLive
//
//  Created by Marcus Schütte on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CLogger.h"
#include "CQueue.h"

using namespace std;

void* CLogger_run(void *param)
{
    //-3 -> All Logs
    //-2 -> Warnings and Errors
    //-1 -> Only Errors
    long logging_level = -3;
    
    cout<<"Logging enabled!"<<std::endl;
    
    CQueue msg_queue(8300);
    msg_queue.set_type(logging_level);
    
    while(true)
    {
        t_msg message;
        msg_queue >> message;
        
        switch(message.type)
        {
            case 3: cout<<"INFO: "; break;
            case 2: cout<<"WARNING: "; break;
            case 1: cout<<"ERROR: "; break;
        
        }
        cout<<"\""<<message.text<<"\""<<endl;
        
    }
    
}

