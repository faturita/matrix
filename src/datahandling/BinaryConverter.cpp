//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file BinaryConverter.cpp
*  \brief Implementation of the Binary Converter class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "BinaryConverter.h"
#include <iomanip>
#include <fstream>
using namespace std;

namespace O_SESSAME {
BinaryConverter::BinaryConverter(const string& _newFilename, string _newExtension, string _newFileLocation) : Converter(_newFilename, _newExtension, _newFileLocation)
{
}

void BinaryConverter::Export(const ConversionForm& _exportConvForm)
{
    std::ofstream ofile;
    ofile.open((GetFileLocation() + GetFilename() + ".dat").data(), ios::binary);
    ofile.write(reinterpret_cast<char*>(_exportConvForm.GetDataPtr()),  _exportConvForm.GetDataSize());
    ofile.close();
}
Matrix BinaryConverter::Import(const ConversionForm& _importConvForm)
{
    int sizeObject;
    ifstream ifile;
    ifile.open((GetFileLocation() + GetFilename() + ".dat").data(), ios::binary);
  
    // get length of file:
    ifile.seekg (0, ios::end);
    sizeObject = ifile.tellg();
    ifile.seekg (0, ios::beg);
    _importConvForm.InitializeData(sizeObject);
    ifile.read(reinterpret_cast<char*>(_importConvForm.GetDataPtr()),  sizeObject);
    ifile.close();
}
} // close namespace O_SESSAME

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.cpp,v $
*
******************************************************************************/
