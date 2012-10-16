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

class CThread
{
private:
    pthread_t thread;
    int ret;
protected:
    
public:
    CThread();
    int start(void*);
    friend void* CThread_run(void*);
    int join(void **thread_return);
    void exit(void *ret);
    
};

void* CThread_run(void*)
{
    ;
}

#endif /* defined(__SLive__CThread__) */
