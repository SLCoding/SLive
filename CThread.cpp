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

pthread_t* CThread::start(void *param, void * (*start_routine)(void *))
{
    try
    {
            //  Thread thread_object;
            //thread_object.thread = new pthread_t;
            // thread_object.thread_handle = pthread_create( thread_object.thread , NULL, start_routine, (void*) param);
                    //  threads.push_back(thread_object);
        thread = new pthread_t;
        pthread_create( thread , NULL, start_routine, (void*) param);

        return thread;
    }
    catch(exception e)
    {
        throw e.what();
    }
}

int CThread::join(void **thread_return)
{
        //list<Thread>::const_iterator iterator;
        //for (iterator = threads.begin(); iterator != threads.end(); ++iterator)
        //{
        //if( (*iterator).thread_handle == id)
        //{
            try
            {
                    //return pthread_join( *((*iterator).thread), thread_return );
                return pthread_join(*thread, thread_return);
            }
            catch(exception e)
            {
                throw e.what();
            }
        //}
        //}
    return -1;
}

int CThread::cancel(pthread_t *thread)
{
        // list<Thread>::const_iterator iterator;
        //for (iterator = threads.begin(); iterator != threads.end(); ++iterator)
        //{
        //if( (*iterator).thread_handle == id)
        //{
            try
            {
                return pthread_cancel(*thread);
            }
            catch(exception e)
            {
                throw e.what();
            }
        // }
        //  }
    return -1;
}

int CThread::cancel()
{
        // list<Thread>::const_iterator iterator;
        //for (iterator = threads.begin(); iterator != threads.end(); ++iterator)
        //{
        //if( (*iterator).thread_handle == id)
        //{
    try
    {
        return pthread_cancel(*thread);
    }
    catch(exception e)
    {
        throw e.what();
    }
        // }
        //  }
    return -1;
}

/*
void CThread::exit(void *ret)
{
    list<Thread>::const_iterator iterator;
    for (iterator = threads.begin(); iterator != threads.end(); ++iterator)
    {
        if( (*iterator).thread_handle == id)
        {
            return pthread_join( *((*iterator).thread), thread_return );
        }
    }
    return -1;
}
*/


