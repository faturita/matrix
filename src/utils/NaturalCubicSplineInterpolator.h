//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file NaturalCubicSplineInterpolator.h
*  \brief Interface to the Natural Cubic Spline Interpolator.
*  \author $Author: nilspace $
*  \version $Revision: 1.4 $
*  \date    $Date: 2003/04/27 22:04:34 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SSF_NATURAL_CUBIC_SPLINE_INTERPOLATOR_H__
#define __SSF_NATURAL_CUBIC_SPLINE_INTERPOLATOR_H__

#include <Matrix.h>
#include "Interpolator.h"

namespace O_SESSAME {
/*! \brief Interpolates between a given set of data points to create a smooth function.
*
* \ingroup InterpolationLibrary
* \detail Works for either a single function, or Vector of functions (such as interpolating a state)
*/
class NaturalCubicSplineInterpolator : public Interpolator
{
public:
    /*! \brief Creates an empty natural cubic spline interpolation. 
        *
        */
    NaturalCubicSplineInterpolator();
    
    /*! \brief Creates a natural cubic spline interpolation. 
        *
        */
    NaturalCubicSplineInterpolator(const Vector& _inputPoints, const Matrix& _outputPoints);
    
    /*! \brief Creates an interpolation from the vector of time points and matrix of corresponding data points. 
        *
        * @param _timePoints Vector of time (seconds) points of the data values.
        * @param _dataPoints Matrix of data points at each time step in the _timePoints vector. 
        *	[x1(t1) x2(t1) x3(t1) ... ]
        *	[x1(t2) x2(t2) x3(t2) ... ]
        */
    virtual void Interpolate(const Vector& _timePoints, const Matrix& _dataPoints);
        
    /*! \brief Evaluate interpolation curve at a specified time.
        *
        * \detail Output = m_Slope * _inputPoint + m_Offset
        * @param _inputPoint Input point (time) at which to evaluate the vector of interpolations.
        * @return Vector of output values from the evaluated interpolation.
        */
    virtual Vector Evaluate(const double& _inputPoint);
    
    /*! \brief Returns the number of data points required for interpolation.
        *
        * \detail the number of data points is the number of X-values (time) required to interpolate.
        * @return the number of data points, centered about the evaluation time, req'd to interpolate.
        */
    virtual int GetNumberDataPoints()	{return m_NumDataPoints;};

    /*! \brief Return a pointer to a new instance of a natural cubic spline interpolator type.
        *
        * \detail This is used to request memory for a new instance of a NaturalCubicSplineInterpolator. It is necessary 
        *	when attempting to get a pointer from the abstract data type Interpolator 
        *	and the actual representation type isn't known.
        * @return a pointer to a new allocation of memory for the NaturalCubicSplineInterpolator object.
        */
    virtual NaturalCubicSplineInterpolator* NewPointer();
    
    /*! \brief Return a pointer to a copy of the natural cubic spline interpolator instance.
        *
        * \detail This is used to request memory for a copy of this instance of NaturalCubicSplineInterpolator. It is necessary 
        *	when attempting to get a pointer from the abstract data type Interpolator 
        *	and the actual representation type isn't known.
        * @return a pointer to a copy of the NaturalCubicSplineInterpolator object.
        */
    virtual NaturalCubicSplineInterpolator* Clone();
    
protected:
    /*! \brief Computes the parameters of the natural cubic spline interpolation given the data points.
        * 
        */
    void BuildNaturalCubicSplineInterpolation(const double& _x1, const double& _y1, const double& _x2, const double& _y2, double& _Slope, double& _Offset);
    
    /*! \brief Doesn't do anything for linear interpolator since there are always only 2 data points. */
    virtual void SetNumberDataPoints(const int& _numberDataPoints) {};    

private:
    /** Number of data points used for interpolation, should always be 2 for linear interpolation */
    int m_NumDataPoints;
    /** Number of elements in the state vector. Used to determine how many linear interpolations there are per data set. */
    int m_NumElements;

    Matrix m_Acoeff;
    Matrix m_Bcoeff;
    Matrix m_Ccoeff;
    Matrix m_Dcoeff;
    
    Vector m_tempOutput; // this is faster, but larger could be a problem for big histories
    
};

#endif 
} // close namespace O_SESSAME


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: Integrator.h,v $
*
******************************************************************************/
