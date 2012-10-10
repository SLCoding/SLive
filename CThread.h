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

class CThread
{
private:
    
protected:
    
public:
    void start(); // parameter not finished
    virtual void* run() = 0; // parameter not finished
};

#endif /* defined(__SLive__CThread__) */
