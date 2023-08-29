//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file BinaryConverter.h
*  \brief Interface to the Binary Converter class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __O_SESSAME_BINARY_CONVERTER_H__
#define __O_SESSAME_BINARY_CONVERTER_H__
#include "Converter.h"
#include "Matrix.h"
#include "ConversionForm.h"

namespace O_SESSAME {

/*! \brief Class for importing and exporting data to ASCII.
* \ingroup DataHandling
*
*
*/
class BinaryConverter : public Converter 
{
public:
    BinaryConverter(const string& _newFilename, string _newExtension ="", string _newFileLocation = "");

    // BinaryConverter OPERATORS
    virtual void Export(const ConversionForm& _exportConvForm);
    virtual Matrix Import(const ConversionForm& _importConvForm);
private:

};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/