//
//  CThread.cpp
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#include "CThread.h"

CThread::CThread()
{
    ret = 0;
}

int CThread::start(void *param)
{
    return ret = pthread_create( &this->thread, NULL, CThread_run, (void*) param);
}

int CThread::join(void **thread_return)
{
    return pthread_join( this->thread, thread_return );
}

void CThread::exit(void *ret)
{
    pthread_exit(ret);
}



