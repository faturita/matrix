//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file AttitudeNumericPropagator.h
*  \brief Interface to the Attitude Numeric Propagator class.
*  \author $Author: nilspace $
*  \version $Revision: 1.5 $
*  \date    $Date: 2003/05/13 18:54:04 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/* \todo Finish documentation
*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SSF_ATTITUDE_NUMERIC_PROPAGATOR_H__
#define __SSF_ATTITUDE_NUMERIC_PROPAGATOR_H__

#include "Matrix.h"
#include "Rotation.h"
#include "Integrator.h"
#include "Propagator.h"

namespace O_SESSAME {

/*! \brief Propagation of attitude using a numeric integrator.
* \ingroup PropagatorToolkit
*
* \detail 
* \example testDynamics.cpp
*/
class AttitudeNumericPropagator : public NumericPropagator
{
public:
    /*! Default Constructor */
    AttitudeNumericPropagator();
    /*! Default Deconstructor */
    virtual ~AttitudeNumericPropagator();
    
    /*! Propagates the dynamics forward through time
        * @param _propTime vector of ssfTime values specifying beginning, ending times of propagation as well as time step [initialTime, timeStep, finalTime] (s)
        * @param _orbitInitConditions initial conditions of the orbit integration.
        * @param _attInitConditions initial conditions of the attitude integration.
        */
    virtual void Propagate(const vector<ssfTime> &_propTime, const Vector &_orbitInitConditions, const Vector &_attInitConditions);
    
    void SetAttitudeIntegrator(Integrator* _pAttitudeIntegrator);

    Integrator* GetOrbitIntegrator() const;
    Integrator* GetAttitudeIntegrator() const;

protected:
    void SetOrbitIntegrator(Integrator* _pOrbitIntegrator);

    /*! \brief Propagate the attitude through a specified time
        * @param _propTime Vector of values specifying beginning, ending times of propagation as well as time step [initialTime, timeStep, finalTime] (s)
        * @param _initConditions Vector of initial conditions of the attitude propagation. 
        * @return Matrix of integrated times and states as returned by the attitude dynamics equation
        */
    virtual Matrix PropagateAttitude(const vector<ssfTime> &_propTime, const Vector &_initConditions);
    
private:
    /*! \brief Matrix of calculated integration mesh points for the orbit dynamics */
    OrbitHistory m_OrbitStateMeshPoints;
    /*! \brief Matrix of calculated integration mesh points for the attitude dynamics */
    Matrix m_AttitudeStateMeshPoints;
    
    /*! \brief pointer to the integrator used by the numeric propagator for attitude integration. */
    Integrator* m_pAttitudeIntegrator;
};
} // close namespace O_SESSAME

#endif
/*****************************************************************************
*	$Log: NumericPropagator.h,v $
*	Revision 1.5  2003/05/13 18:54:04  nilspace
*	Fixed comments & various propagate functions.
*	
*	Revision 1.4  2003/04/27 22:04:33  nilspace
*	Created the namespace O_SESSAME.
*	
*	Revision 1.3  2003/04/25 13:43:52  nilspace
*	Updated to actually work. Still needs verification.
*	
*	Revision 1.2  2003/04/23 16:30:58  nilspace
*	Various bugfixes & uploading of all changed code for new programmers.
*	
*	Revision 1.1  2003/04/08 22:49:56  nilspace
*	Initial Submission.
*
******************************************************************************/
