/*
 *  testTime.cpp
 *  
 *
 *  Created by Andrew Turner on Fri May 16 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include <iomanip>
#include "Time.h"
using namespace O_SESSAME;

int main()
{
    cout << std::setprecision(25) << Now() << std::endl;
    timeval tv; gettimeofday(&tv, NULL); cout <<  (tv.tv_sec + (tv.tv_usec)/1000000.) << std::endl;
    cout << tv.tv_sec << " : " << tv.tv_usec << std::endl;
    
    ssfTime J2000(2000,1,1,0,0,0);
    
    cout << "GST @ 2000: " << J2000.GetGreenwichSiderealTime() << std::endl;
    
}