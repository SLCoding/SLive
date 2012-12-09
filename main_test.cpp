//
//  main_test.cpp
//  SLive
//
//  Created by Marcus Schütte on 08.12.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CSLiveDB.h"


int main()
{
    CSLiveDB db_int = CSLiveDB("SLive", "SLive", "SLive", "192.168.0.19");
    //db_int.create_User("Japortie5", "testpw", "schuette.marcus@googlemail.com");
    
    
    db_int.login("Japortie5", "testpw", "127.0.0.1");
    return 0;
}
