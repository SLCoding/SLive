//
//  CThread.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CThread__
#define __SLive__CThread__

#include <iostream>
#include <pthread.h>
#include <list>

struct Thread
{
    pthread_t *thread;
    int thread_handle;
};

using namespace std;

class CThread
{
private:
        //list<Thread> threads;
        //Thread threads;
    pthread_t *thread;
protected:
    
public:
    CThread();
    pthread_t* start(void*, void * (*start_routine)(void *));
    int join(void **thread_return);
    int cancel(pthread_t *thread);
        // pthread_t getThread(pthread_t *thread);
    // void exit(void *ret);
};

#endif /* defined(__SLive__CThread__) */
