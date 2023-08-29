//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file SimpleAerodynamicDisturbances.h
*  \brief Simple atmospheric disturbance force function models.
*  \author $Author: nilspace $
*  \version $Revision: 1.3 $
*  \date    $Date: 2003/06/12 23:06:21 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/* 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_SIMPLE_ATMOSPHERE_DISTURBANCES_H__
#define __OSESSAME_SIMPLE_ATMOSPHERE_DISTURBANCES_H__
#include "matrix/Matrix.h"
#include "Time.h"
#include "OrbitState.h"
#include "Attitude.h"
#include "Environment.h"
#include <vector.h>
using namespace std;
using namespace O_SESSAME;

/*! \brief Simple atmospheric force function .
* @ingroup EnvironmentForceFunctions
*
* 
* @param _currentTime current simulation time
* @param _currentOrbitState current orbit state, including representation and reference frame
* @param _currentAttitudeState current attitude state, including rotation and reference frame
* @param _parameterList EnvFuncParamaterType parameter list for external variables = [\f$\BC, \rho\f$], Ballistic Coefficient, atmospheric density
* @return This force functions returns a 3-element vector of forces (x,y,z) due to gravity using a 
*	two-body simplified force model.
*/
inline Vector SimpleAerodynamicDragForce(const ssfTime &_currentTime, const OrbitState &_currentOrbitState, const AttitudeState &_currentAttitudeState, const EnvFuncParamaterType &_parameterList)
{
    static Vector Forces(3);
    double BC = *(reinterpret_cast<double*>(_parameterList[0]));
    static Vector Vrel(3); Vrel(_) = _currentOrbitState.GetState()(_(VectorIndexBase+3,VectorIndexBase+5));
    double Vrel_mag = norm2(Vrel);
    double rho = *(reinterpret_cast<double*>(_parameterList[1]));
    Forces = -1/2 * rho / BC * pow(Vrel_mag,2) * Vrel / Vrel_mag; 
    return Forces;
}
#endif
// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: GravityFunctions.h,v $
*
******************************************************************************/
