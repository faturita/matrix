//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file NumericPropagator.cpp
*  \brief Implementation of the Numeric Propagator class.
*  \author $Author: nilspace $
*  \version $Revision: 1.7 $
*  \date    $Date: 2003/05/13 18:54:04 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/* 
*
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "NumericPropagator.h"
namespace O_SESSAME {

NumericPropagator::NumericPropagator()
{

}

NumericPropagator::~NumericPropagator()
{
    /*! \todo figure out how to delete m_pIntegrator only when it is not being used */
    if(m_pOrbitIntegrator)
        delete m_pOrbitIntegrator;
    if(m_pAttitudeIntegrator)
        delete m_pAttitudeIntegrator;
}

void NumericPropagator::Propagate(const vector<ssfTime> &_propTime, const Vector &_orbitInitConditions, const Vector &_attInitConditions)
{
    /** \todo Determine what prop times exist and what times are req'd to recalc */
    
    // Propagate the orbit & attitude, store in the matrix of integration meshpoints
    m_OrbitStateMeshPoints = PropagateOrbit(_propTime, _orbitInitConditions);
    m_AttitudeStateMeshPoints = PropagateAttitude(_propTime, _attInitConditions);
    
    // Create temporary orbit & attitude that have the same types (copy of) the current state objects
        // the actual state vectors will be replaced, but we need to make sure to have the same representation & frame types
    OrbitState tempOrbit = m_pOrbitObject->GetStateObject();
    AttitudeState tempAttitude = m_pAttitudeObject->GetStateObject();
    vector<ssfTime> timeVector;
    vector<OrbitState> orbitVector;		/*! \todo determine if having vector<State> in Propagate is polymorphic */
    vector<AttitudeState> attitudeVector;
    
    /** \todo Append the requested time steps in the appropriate component history */
    // Convert the meshpoints to stored objects and append them to the appropriate history.
    for (int ii = MatrixIndexBase; ii < m_OrbitStateMeshPoints[MatrixRowsIndex].getIndexBound() + MatrixIndexBase; ++ii)
    {
        m_OrbitStateConversionFunction(m_OrbitStateMeshPoints(ii,_(MatrixIndexBase+1,m_OrbitStateMeshPoints[MatrixColsIndex].getIndexBound())), tempOrbit);
        m_AttitudeStateConversionFunction(m_AttitudeStateMeshPoints(ii,_(MatrixIndexBase+1,m_AttitudeStateMeshPoints[MatrixColsIndex].getIndexBound())), tempAttitude);
        timeVector.push_back(ssfTime(m_OrbitStateMeshPoints(ii,MatrixIndexBase)));
        orbitVector.push_back(tempOrbit);
        attitudeVector.push_back(tempAttitude);
    }
    
    // Store the new histories to the orbit & attitude objects
    if(m_pOrbitObject)
        m_pOrbitObject->GetHistory().AppendHistory(timeVector, orbitVector);
    if(m_pAttitudeObject)
        m_pAttitudeObject->GetHistory().AppendHistory(timeVector, attitudeVector);
        
    // Set the stored Attitude & Orbit
    /*! \todo Determine if the updated attitude & orbit should be set in Propagator */
    m_pOrbitObject->SetStateObject(tempOrbit);
    m_pAttitudeObject->SetStateObject(tempAttitude);
    
    return;
}

Matrix NumericPropagator::PropagateOrbit(const vector<ssfTime> &_propTime, const Vector &_initConditions)
{
    OrbitState tempOrbit = m_pOrbitObject->GetStateObject();
    AttitudeState tempAttitude = m_pAttitudeObject->GetStateObject();
    if(m_pOrbitObject)
    {
	return m_pOrbitIntegrator->Integrate(   
                                    _propTime, 
                                    m_pOrbitObject->GetDynamicsEq(),
                                    _initConditions, 
                                    *m_pOrbitObject,
                                    *m_pAttitudeObject,
                                    m_pOrbitObject->GetParameters(),
                                    m_pOrbitObject->GetEnvironmentForcesFunctor() 
                                );
    }
    return Matrix(0);
}

Matrix NumericPropagator::PropagateAttitude(const vector<ssfTime> &_propTime, const Vector &_initConditions)
{
    OrbitState tempOrbit = m_pOrbitObject->GetStateObject();
    AttitudeState tempAttitude = m_pAttitudeObject->GetStateObject();
    if(m_pAttitudeObject)
    {
        return m_pAttitudeIntegrator->Integrate(   
                                    _propTime, 
                                    m_pAttitudeObject->GetDynamicsEq(),
                                    _initConditions, 
                                    *m_pOrbitObject,
                                    *m_pAttitudeObject,
                                    m_pAttitudeObject->GetParameters(),
                                    m_pAttitudeObject->GetEnvironmentForcesFunctor() 
                                );
    }
    return Matrix(0);
}

void NumericPropagator::SetOrbitIntegrator(Integrator* _pOrbitIntegrator)
{
    m_pOrbitIntegrator = _pOrbitIntegrator;
    return;
}
void NumericPropagator::SetAttitudeIntegrator(Integrator* _pAttitudeIntegrator)
{
    m_pAttitudeIntegrator = _pAttitudeIntegrator;
    return;
}    
Integrator* NumericPropagator::GetOrbitIntegrator() const
{
    return m_pOrbitIntegrator;
}
Integrator* NumericPropagator::GetAttitudeIntegrator() const
{
    return m_pAttitudeIntegrator;
}
} // close namespace O_SESSAME

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: NumericPropagator.cpp,v $
*	Revision 1.7  2003/05/13 18:54:04  nilspace
*	Fixed comments & various propagate functions.
*	
*	Revision 1.6  2003/04/30 14:45:35  nilspace
*	Set the OrbitStateObject() and AttitudeStateObject() in the Propagate() function.
*	
*	Revision 1.5  2003/04/29 20:56:03  nilspace
*	Updated to work.
*	
*	Revision 1.4  2003/04/27 22:04:33  nilspace
*	Created the namespace O_SESSAME.
*	
*	Revision 1.3  2003/04/25 13:43:52  nilspace
*	Updated to actually work. Still needs verification.
*	
*	Revision 1.1  2003/04/08 22:49:56  nilspace
*	Initial Submission.
*
******************************************************************************/
