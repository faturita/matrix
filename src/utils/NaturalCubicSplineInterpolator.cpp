//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file NaturalCubicSplineInterpolator.cpp
*  \brief Implementation of the Natural Cubic Spline Interpolator.
*  \author $Author: nilspace $
*  \version $Revision: 1.1 $
*  \date    $Date: 2003/05/13 18:42:08 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "NaturalCubicSplineInterpolator.h"

namespace O_SESSAME {
NaturalCubicSplineInterpolator::NaturalCubicSplineInterpolator() : m_NumDataPoints(2), m_NumElements(0), m_Acoeff(m_NumElements), m_Bcoeff(m_NumElements), m_Ccoeff(m_NumElements), m_Dcoeff(m_NumElements), m_tempOutput(m_NumElements)
{

}

NaturalCubicSplineInterpolator::NaturalCubicSplineInterpolator(const Vector& _timePoints, const Matrix& _dataPoints) : m_NumDataPoints(2), m_NumElements(_dataPoints[MatrixRowsIndex].getIndexBound()), m_Acoeff(m_NumElements), m_Bcoeff(m_NumElements), m_Ccoeff(m_NumElements), m_Dcoeff(m_NumElements), m_tempOutput(m_NumElements)
{
    Interpolate(_timePoints, _dataPoints);
}

NaturalCubicSplineInterpolator::~NaturalCubicSplineInterpolator()
{
}

void NaturalCubicSplineInterpolator::Interpolate(const Vector& _timePoints, const Matrix& _dataPoints)
{
    m_NumElements = _dataPoints[MatrixColsIndex].getIndexBound();
    if(m_Acoeff.getIndexBound() < m_NumElements)
        m_Acoeff.initialize(m_NumElements);
    if(m_Bcoeff.getIndexBound() < m_NumElements)
        m_Bcoeff.initialize(m_NumElements);
    if(m_Ccoeff.getIndexBound() < m_NumElements)
        m_Ccoeff.initialize(m_NumElements);
    if(m_Dcoeff.getIndexBound() < m_NumElements)
        m_Dcoeff.initialize(m_NumElements);    
            
    for(int ii = MatrixIndexBase; ii <= m_NumElements; ++ii)
    {
        BuildNaturalCubicSplineInterpolation(_timePoints(VectorIndexBase), _dataPoints(VectorIndexBase,ii), 
                        _timePoints(VectorIndexBase+1), _dataPoints(VectorIndexBase+1,ii), 
                        m_Slope(ii), m_Offset(ii));
    }
    SetValid(TRUE);
}

Vector NaturalCubicSplineInterpolator::Evaluate(const double& _timeEvalPoint)
{
    if(m_tempOutput.getIndexBound() < m_NumElements)
        m_tempOutput.initialize(m_NumElements);
        
    for(int ii = VectorIndexBase; ii <= m_NumElements; ++ii)
    {
        //m_tempOutput(ii) = ;
    }
    return m_tempOutput;
}

void NaturalCubicSplineInterpolator::BuildNaturalCubicSplineInterpolation(const double& _x1, const double& _y1, const double& _x2, const double& _y2, double& _Slope, double& _Offset)
{

}

NaturalCubicSplineInterpolator* NaturalCubicSplineInterpolator::NewPointer()
{
    return new NaturalCubicSplineInterpolator();
}

NaturalCubicSplineInterpolator* NaturalCubicSplineInterpolator::Clone()
{
    return new NaturalCubicSplineInterpolator(*this);
}

} // close namespace O_SESSAME


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: NaturalCubicSplineInterpolator.cpp,v $
*
******************************************************************************/
