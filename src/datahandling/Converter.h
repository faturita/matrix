//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file Converter.h
*  \brief Interface to the Converter class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_CONVERTER_H__
#define __OSESSAME_CONVERTER_H__
#include "Matrix.h"
#include "ConversionForm.h"
#include <string>  	
#include <stdlib.h>  
#include <sstream>

using namespace std; 
using namespace O_SESSAME; 

namespace O_SESSAME {

/*! \brief Class interface for importing and exporting data to various external formats.
* \ingroup DataHandling
*
*
*/
class Converter 
{
public:
    virtual ~Converter();
    
    // CONVERTER MUTATORS
    virtual void SetFilename(const string& _newFilename);
    virtual void SetExtension(const string& _newExtension);
    virtual void SetFileLocation(const string& _newFileLocation);
    
    // CONVERTER INSPECTORS
    virtual string GetFilename();
    virtual string GetExtension();
    virtual string GetFileLocation();

    // OPERATORS
    virtual void Export(const ConversionForm& _exportConvForm) = 0;
    virtual Matrix Import(const ConversionForm& _importConvForm) = 0;

protected:
    Converter(const string& _newFilename, string _newExtension, string _newFileLocation);

private:
    string m_Filename;
    string m_Extension;
    string m_FileLocation;
};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/