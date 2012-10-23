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
#include <errno.h>

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
    
    long default_type;
    long send_type;
    long rec_type;
public:
    
    CQueue();
    CQueue(int queue_id);
    
    int get_queue_id();
    
    bool set_type(long type);
    bool set_send_type(long type = 0);      //Type of 0 will use the default type to send
    bool set_rec_type(long type = 0);       //Type of 0 will use the default type to receive
    
    bool send_msg(string msg, long type = 1) const;   //send message    
    t_msg receive_msg(long type = 0) const;           //get message (with block)
    t_msg get_msg(long type = 0) const;               //get message (no block)
    bool destroy();                             //Destroy Queue
    
    const CQueue& operator << (const string& s) const;     // send the string
    const CQueue& operator >> (string& s); 
    
    const CQueue& operator << (const char* s) const;     // send the string
    const CQueue& operator >> (t_msg &s); 
};
#endif
