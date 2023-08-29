//******************************************************************************
//                    MVAEXIT.CPP
//******************************************************************************
//
//
//********************************************************************************
//
//            Chris Anderson (C) UCLA 1997
//            7/31/97
//
//
//********************************************************************************
//
#include "mvaexit.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#ifndef __BCPLUSPLUS__
void CAMmvaExit()
{
    std::cerr << " Fatal Error : Program Stopped " << std::endl;
    exit(1);
};
void CAMmvaExit(char* ErrorMessage)
{
    std::cerr << ErrorMessage << std::endl << std::endl << std::endl;
    std::cerr << " Fatal Error " << std::endl;
    exit(1);
};
#else
void CAMmvaExit()
{
    std::cerr << std::endl << std::endl;
    std::cerr << "Hit return to Exit " << std::endl << std::endl;
    getchar();
    throw CAMmvaException("Error In CAM mvametric entity Classes");
};
void CAMmvaExit(char* ErrorMessage)
{
     throw CAMmvaException(ErrorMessage);
};
CAMmvaException::CAMmvaException()
{
    errorMessage = new char[1];
    errorMessage[0] = '\0';
}
CAMmvaException::CAMmvaException(char* Emessage)
{
    errorMessage = new char[strlen(Emessage) + 1];
    strcpy(errorMessage, Emessage);
}
CAMmvaException::CAMmvaException(const CAMmvaException& C)
{
    errorMessage = new char[strlen(C.errorMessage) + 1];
    strcpy(errorMessage, C.errorMessage);
}
CAMmvaException::~CAMmvaException()
{
    delete [] errorMessage;
}
#endif

  
