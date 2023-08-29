//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file MatrixConversionForm.h
*  \brief Interface to the MatrixConversionForm class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_MATRIX_CONVERSIONFORM_H__
#define __OSESSAME_MATRIX_CONVERSIONFORM_H__
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
class MatrixConversionForm : public ConversionForm 
{
public:
    MatrixConversionForm();
    virtual ~MatrixConversionForm();
    
    // ConversionForm MUTATORS
    virtual void FromHeader(const string& _body);
    virtual void FromBody(const string& _body);
    virtual void FromFooter(const string& _body);
    virtual void SetMatrix(const Matrix& _exportMatrix);
    virtual void InitializeData(int _sizeObject);
    
    // ConversionForm INSPECTORS
    virtual string GetHeader() const;
    virtual string GetBody() const;
    virtual string GetFooter() const;
    Matrix GetMatrix() const;
    
    Matrix* GetDataPtr() const;
    int GetDataSize() const;
    // ConversionForm OPERATORS

protected:

private:
    Matrix m_exportMatrix;
};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/