//
//######################################################################
//            CAMrange Sample Program  #1
//
// Chris Andersion (C) UCLA 1997                                8/18/97
//######################################################################
//
/*
   This program demonstrates the use of CAMrange constructors and member
   functions. 
*/
#include "access.h"
#include <ostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   cout << "CAMrange Object _(1,3) : " << _(1,3)  << std::endl;
   cout << std::endl;

   CAMrange R(-1,1);

   cout << "CAMrange Object R   : " << R   << std::endl;
   cout << "CAMrange Object R+2 : " << R+2 << std::endl;
   cout << "CAMrange Object R-2 : " << R-2 << std::endl;
   cout << "CAMrange Object R++ : " << R++ << std::endl;
   cout << "CAMrange Object R-- : " << R-- << std::endl;
   cout << "R.getCount()        = " << R.getCount() << std::endl;
   cout << "R.length()          = " << R.length()   << std::endl;

   cout << std::endl;

   R.initialize(-2,2,2);
   cout << "R.initialize(-2,2,2) : " << R  << std::endl;
   cout << "CAMrange Object R    : " << R   << std::endl;
   cout << "CAMrange Object R-2  : " << R-2 << std::endl;
   cout << "CAMrange Object R++  : " << R++ << std::endl;
   cout << "CAMrange Object R--  : " << R-- << std::endl;
   cout << "R.getCount()         = " << R.getCount() << std::endl;
   cout << "R.length()           = " << R.length()   << std::endl;

   cout << " Program End : Hit Any Key to Terminate " << std::endl;
   getchar();
   return 0;
}
  
