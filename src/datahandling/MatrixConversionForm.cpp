//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file MatrixConversionForm.cpp
*  \brief Implementation of the MatrixConversionForm class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "MatrixConversionForm.h"
#include <iomanip>
namespace O_SESSAME {
MatrixConversionForm::MatrixConversionForm()
{
}

MatrixConversionForm::~MatrixConversionForm()
{
}
void MatrixConversionForm::SetMatrix(const Matrix& _exportMatrix)
{
    m_exportMatrix = _exportMatrix;
}

Matrix MatrixConversionForm::GetMatrix() const
{
    return m_exportMatrix;
}

void MatrixConversionForm::InitializeData(int _sizeObject)
{

}

Matrix* MatrixConversionForm::GetDataPtr() const
{
    return &m_exportMatrix;
}
int MatrixConversionForm::GetDataSize() const
{
    return sizeof(m_exportMatrix);
}
string MatrixConversionForm::GetHeader() const
{
    string matrixHeader;
    matrixHeader = typeid(m_exportMatrix).name();
    return matrixHeader;
}

string MatrixConversionForm::GetBody() const
{
    string matrixString;
    stringstream matrixStream;
    matrixStream << std::setprecision(15) << m_exportMatrix;
    matrixString = matrixStream.str();

    return matrixString;
}

string MatrixConversionForm::GetFooter() const
{
    string matrixString;
    return matrixString;
}
 
void MatrixConversionForm::FromHeader(const string& _body)
{

}

void MatrixConversionForm::FromBody(const string& _body)
{
    char matrixValues[] = {'+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
    int rowCount = 0;
    int columnCount = 0;
    int finder = 0;
    stringstream fileStream;
    string bodyLine;
    fileStream << _body;
    getline(fileStream,bodyLine,'\n');
    while((finder = bodyLine.find_first_of(matrixValues, finder+1))  != string::npos)
    {
        ++columnCount;
        finder = bodyLine.find(' ', finder+1);
    }    

    while(!fileStream.eof())
    {
        getline(fileStream,bodyLine,'\n');
        ++rowCount;
    }
    // Check to see if there are numbers in the last line or if it is an extra linefeed
    if(((finder = bodyLine.find_first_of(matrixValues, finder+1))  == string::npos) && rowCount != 0)
        --rowCount;

    fileStream.clear();
    fileStream << _body;
    m_exportMatrix.initialize(rowCount, columnCount);
    for(int row = MatrixIndexBase; row <= rowCount; ++row)
        for(int col = MatrixIndexBase; col <= columnCount; ++col)
            if(!fileStream.eof())
                fileStream >> m_exportMatrix(row,col);
}

void MatrixConversionForm::FromFooter(const string& _body)
{
    
//    ifile.open((GetFileLocation() + GetFilename() + GetExtension()).data(), ifstream::in);
//    filebuf fb;
//    fb.open ((GetFileLocation() + GetFilename() + GetExtension()).data(),ios::in);
//    istd::ostream ios(&fb);
  
//    ios >> m_exportMatrix;
//    fb.close();
    return;
}
} // close namespace O_SESSAME

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.cpp,v $
*
******************************************************************************/
