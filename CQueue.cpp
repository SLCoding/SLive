//
//  CQueue.cpp
//  SLive
//
//  Created by Marcus Schütte on 17.10.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "CQueue.h"
#include <iostream>


CQueue::CQueue()
{
    
}

CQueue::CQueue(int queue_id)
{
    //create 2 queues
    //first for sending messages
    //second for sending following parts of a message
    //
    //this is required for maintaining order of the messages
    queue_ref = msgget(queue_id, IPC_CREAT | 0666);
    queue_cont_ref = msgget(1111, IPC_CREAT | 0666);
    
}

bool CQueue::send_msg(string msg, long type)
{
    int length = msg.length();
    int struct_size = sizeof(msg_part) - sizeof(long);
    
    //calculate random type number for following parts
    //
    //required for identifying message
    srand(time(NULL));
    int cont_type = rand() %8000 +1;
    
    //are multiple parts required?
    msg_part message;
    message.type = type;
    /*if(length>=MSGSIZE)
        message.cont_num = cont_type;
    else
        message.cont_num = 0;
    */
    //send all parts of the message
    for(int pos = 0;pos < length; pos+=MSGSIZE-1)
    {
        const char *message_cstr = msg.substr(pos, 130).c_str();
        //if its not the last part, send identifikationnumber for following parts
        //else 0
        if(strlen(message_cstr)>=MSGSIZE)
            message.cont_num = cont_type;
        else 
            message.cont_num = 0;
        
        strcpy(message.text, msg.substr(pos, MSGSIZE-1).c_str());
        
        if(pos == 0)
            msgsnd(queue_ref, &message, struct_size, 0);
        else
        {
            message.type = cont_type;
            msgsnd(queue_cont_ref, &message, struct_size, 0);
        }
    }
    
    
    return true;
    
}


t_msg CQueue::receive_msg(long type)
{
    string msg_str;
    
    msg_part message;
    t_msg result;
    
    
    int struct_size = sizeof(msg_part) - sizeof(long);
    
    msgrcv(queue_ref, &message, struct_size, type, 0);
    result.type = message.type;
    
    msg_str = message.text;
    
    while(message.cont_num != 0)
    {
        msgrcv(queue_cont_ref, &message, struct_size, message.cont_num, 0);
        msg_str += message.text;
    }
    
    result.text = msg_str;
    
    return result;
    
}


t_msg CQueue::get_msg(long type)
{
    string msg_str;
    
    msg_part message;
    t_msg result;
    
    
    int struct_size = sizeof(msg_part) - sizeof(long);
    
    if(msgrcv(queue_ref, &message, struct_size, type, IPC_NOWAIT) != -1)
    {
        result.type = message.type;
    
        msg_str = message.text;
    
        while(message.cont_num != 0)
        {
            msgrcv(queue_cont_ref, &message, struct_size, message.cont_num, 0);
            msg_str += message.text;
        }
    
        result.text = msg_str;
    
        return result;
    }
    
    result.type=0;
    return result;
}


bool CQueue::destroy()
{
    return msgctl(queue_ref, IPC_RMID, NULL) + 1;
    
    
}