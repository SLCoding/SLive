//
//  CQueue.h
//  SLive
//
//  Created by Marcus Schütte on 17.10.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SLive_CQueue_h
#define SLive_CQueue_h

#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>


#define MSGSIZE 128    //determines the size of the parts of the message

using namespace std;


//a single part of the message
struct msg_part
{
    long type;
    long cont_num;
    char text[MSGSIZE];
    
};

//container holding the whole message
struct t_msg
{
    long type;
    string text;
};

class CQueue
{
private:
    int queue_id;
    int queue_ref;
    int queue_cont_ref;
    
public:
    
    CQueue();
    CQueue(int queue_id);
    
    int get_queue_id();
    
    bool send_msg(string msg, long type = 1);   //send message    
    t_msg receive_msg(long type = 0);           //get message (with block)
    t_msg get_msg(long type = 0);               //get message (no block)
    bool destroy();                             //Destroy Queue
};
#endif
