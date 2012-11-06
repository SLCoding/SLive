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
    list<Thread> threads;
protected:
    
public:
    CThread();
    int start(void*, void * (*start_routine)(void *));
    int join(int id, void **thread_return);
    // void exit(void *ret);
};

#endif /* defined(__SLive__CThread__) */
