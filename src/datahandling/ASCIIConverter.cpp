//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file ASCIIConverter.cpp
*  \brief Implementation of the ASCII Converter class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "ASCIIConverter.h"
#include <iomanip>
#include <fstream>
using namespace std;

namespace O_SESSAME {
ASCIIConverter::ASCIIConverter(const string& _newFilename, string _newExtension, string _newFileLocation) : Converter(_newFilename, _newExtension, _newFileLocation)
{
}

void ASCIIConverter::Export(const ConversionForm& _exportConvForm)
{
    /*! \todo figure out how to store & load matrix without storing the size in the text file. */
    std::ofstream ofile;
    ofile.open((GetFileLocation() + GetFilename() + GetExtension()).data(), ofstream::out);
    ofile << _exportConvForm.GetBody() << std::endl;
    ofile.close();
}
Matrix ASCIIConverter::Import(const ConversionForm& _importConvForm)
{
    int length;
    char * buffer;
    ifstream ifile;
    ifile.open((GetFileLocation() + GetFilename() + GetExtension()).data(), ifstream::in);

 // get length of file:
    ifile.seekg (0, ios::end);
    length = ifile.tellg();
    ifile.seekg (0, ios::beg);

    // allocate memory:
    buffer = new char [length];

    // read data as a block:
    ifile.read (buffer,length);
    ifile.close();

    string fileString(buffer);

    _importConvForm.FromBody(fileString);
}
} // close namespace O_SESSAME

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.cpp,v $
*
******************************************************************************/
