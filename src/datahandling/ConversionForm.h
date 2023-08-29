//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file ConversionForm.h
*  \brief Interface to the ConversionForm class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_CONVERSIONFORM_H__
#define __OSESSAME_CONVERSIONFORM_H__
#include "Matrix.h"
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
class ConversionForm 
{
public:
    virtual ~ConversionForm() {};
    
    // ConversionForm MUTATORS
    virtual void FromHeader(const string& _body) = 0;
    virtual void FromBody(const string& _body) = 0;
    virtual void FromFooter(const string& _body) = 0;
    virtual void InitializeData(int _sizeObject) = 0;

    // ConversionForm INSPECTORS
    virtual string GetHeader() const = 0;
    virtual string GetBody() const = 0;
    virtual string GetFooter() const = 0;
    
    virtual void* GetDataPtr() const;
    virtual int GetDataSize() const = 0;
    // ConversionForm OPERATORS

protected:

private:

};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/