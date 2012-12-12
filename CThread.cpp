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
    ;
}

CThread::~CThread()
{
    delete thread;
}

pthread_t CThread::start(void *param, void * (*start_routine)(void *))
{
    try
    {
        pthread_create( &thread , NULL, start_routine, (void*) param);

        return thread;
    }
    catch(std::exception e)
    {
        throw e.what();
    }
}

int CThread::join(void **thread_return)
{
    try
    {

        return pthread_join(thread, thread_return);
    }
    catch(std::exception e)
    {
        throw e.what();
    }

    return -1;
}

int CThread::cancel(pthread_t thread)
{
    try
    {
        return pthread_cancel(thread);
    }
    catch(std::exception e)
    {
        throw e.what();
    }
    
    return -1;
}

int CThread::cancel()
{
    try
    {
        return pthread_cancel(thread);
    }
    catch(std::exception e)
    {
        throw e.what();
    }

    return -1;
}

