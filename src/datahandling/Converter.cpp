//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file Converter.cpp
*  \brief Implementation of the Exporter class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "Converter.h"
namespace O_SESSAME {

Converter::Converter(const string& _newFilename, string _newExtension, string _newFileLocation) : m_Filename(_newFilename), m_Extension(_newExtension), m_FileLocation(_newFileLocation)  
{
}
Converter::~Converter()
{
}
    
void Converter::SetFilename(const string& _newFilename)
{ m_Filename = _newFilename;}

void Converter::SetExtension(const string& _newExtension)
{ m_Extension = _newExtension;}

void Converter::SetFileLocation(const string& _newFileLocation)
{ m_FileLocation = _newFileLocation;}
    
string Converter::GetFilename()
{return m_Filename;}

string Converter::GetExtension()
{return m_Extension;}

string Converter::GetFileLocation()
{return m_FileLocation;}
 
} // close namespace O_SESSAME

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.cpp,v $
*
******************************************************************************/
