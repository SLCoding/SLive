//
//  CThread.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CThread__
#define __SLive__CThread__

#include <pthread.h>
#include <list>

class CThread
{
private:
    pthread_t *thread;
protected:
    
public:
    CThread();
    ~CThread();
    pthread_t* start(void*, void * (*start_routine)(void *));
    int join(void **thread_return);
    int cancel();
    int cancel(pthread_t *thread);
    pthread_t* getThreadHandle() { return thread; }
};

#endif /* defined(__SLive__CThread__) */
