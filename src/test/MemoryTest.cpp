//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file testInterface.cpp
*  \brief Uses a text-based user interface to step through running a simulation.
*  \author $Author: nilspace $
*  \version $Revision: 1.3 $
*  \date    $Date: 2003/06/05 20:31:54 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*!  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "Matrix.h"
#include "Rotation.h"
#include "Attitude.h"
#include "Orbit.h"
#include "CombinedNumericPropagator.h"
#include "RungeKuttaFehlbergIntegrator.h"
#include "orbitmodels/TwoBodyDynamics.h"
#include "CentralBody/EarthCentralBody.h"
#include "OrbitState.h"
#include "AttitudeState.h"
#include "orbitstaterep/PositionVelocity.h"
#include "orbitframes/OrbitFrameIJK.h"
#include "Plot.h"
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace O_SESSAME;

// Global Variables
static Orbit* myOrbit = NULL;
static Attitude* myAttitude = NULL;
static Environment* myEnvironment = NULL;
static CombinedNumericPropagator* myPropagator = NULL;
static RungeKuttaFehlbergIntegrator* orbitIntegrator = NULL; 
static RungeKuttaFehlbergIntegrator* attitudeIntegrator = NULL; 
static vector<ssfTime> propTimes;

void SetupPropagator();
void SetupEnvironment();
void SetupOrbit();
void SetupAttitude();

void ChangePropagator();
void ChangeEnvironment();
void ChangeOrbit();
void ChangeOrbitIntegrator();
void ChangeAttitude();
void ChangeAttitudeIntegrator();

void Propagate(); 
void Plot();

void myOrbitStateConvFunc(const Matrix &_meshPoint, OrbitState &_convertedOrbitState);
void myAttitudeStateConvFunc(const Matrix &_meshPoint, AttitudeState &_convertedAttitudeState);

void DisplayOrbit()
{
    if(myOrbit->IsIntegrateable())
        cout << "Orbit State (km, km/s): " << ~myOrbit->GetStateObject().GetStateRepresentation()->GetPositionVelocity() << std::endl;
    else
        cout << "Orbit State: [ uninitialized ]" << std::endl;
    return;
}

void DisplayAttitude()
{
    if(myAttitude->IsIntegrateable())
        cout << "Attitude State: (-, rad/s)" << ~myAttitude->GetStateObject().GetState() << std::endl;
    else
        cout << "Attitude State: [ uninitialized ]" << std::endl;
    return;
}
void DisplayPropagator()
{
    if(myPropagator)
        cout << "Propagate times: [" << propTimes[0] << " -> " << propTimes[1] <<  "] (s)" << std::endl;
    else
        cout << "Propagate State: [ uninitialized ]" << std::endl;
    return;
}

int main()
{
    SetupOrbit();
    SetupAttitude();
    SetupPropagator();
    myOrbit->SetPropagator(myPropagator);
    myAttitude->SetPropagator(myPropagator);

    SetupEnvironment();
    myOrbit->SetEnvironment(myEnvironment);
    myAttitude->SetEnvironment(myEnvironment);
   
    ChangeOrbit();
    ChangeEnvironment();
    ChangePropagator();
    Propagate();
    // Store the output to file
    std::ofstream ofile;

    if(myOrbit)
        if (myOrbit->IsIntegrateable())
        {
            ofile.open("OrbitHistory.dat");
            ofile << std::setprecision(15) << myOrbit->GetHistoryObject().GetHistory();
            ofile.close();
        }
    
    if(myAttitude)
        if (myAttitude->IsIntegrateable())
        {
            ofile.open("AttitudeHistory.dat");
            ofile << std::setprecision(15) << myAttitude->GetHistoryObject().GetHistory();
            ofile.close();
        }
    return 0;
                            
}

void Plot()
{
    if(myOrbit)
        if (myOrbit->IsIntegrateable())
        {
            Matrix orbitHistory = myOrbit->GetHistoryObject().GetHistory();
            Matrix orbitPlotting = orbitHistory(_,_(MatrixIndexBase+1,MatrixIndexBase+3));
            Plot3D(orbitPlotting);
        }
    if(myAttitude)
        if (myAttitude->IsIntegrateable())
        {
            Matrix attitudeHistory = myAttitude->GetHistoryObject().GetHistory();
            Matrix attitudePlotting = attitudeHistory(_,_(MatrixIndexBase,MatrixIndexBase+4));
            Plot2D(attitudePlotting);
        }

}

void Propagate()
{

    cout << "PropTime = " << propTimes[0].GetSeconds() << " s -> " << propTimes[1].GetSeconds() << " s" << std::endl;
    Vector orbInitState(6);
    Vector attInitState(7);
    if(myOrbit->IsIntegrateable())
    {
        orbInitState = myOrbit->GetStateObject().GetState();
        cout << "Orbit State: " << ~orbInitState << std::endl;
    }
    if(myAttitude->IsIntegrateable())
    {
        attInitState = myAttitude->GetStateObject().GetState();
        cout << "Attitude State: " << ~attInitState << std::endl;
    }
    // Integrate over the desired time interval
    tick();
    myPropagator->Propagate(propTimes, orbInitState, attInitState);
    ssfSeconds calcTime = tock();
    cout << "finished propagating in " << calcTime << " seconds." << std::endl;
}
// *************************
// ****** ENVIRONMENT ****** 
// *************************
// Force function that will be called each timestep
Vector GravityForceFunction(const ssfTime &_currentTime, const OrbitState  &_currentOrbitState, const AttitudeState &_currentAttitudeState, const EnvFuncParamaterType &_parameterList)
{
//    Vector state = _currentOrbitState.GetState();
    static Vector Forces(3);
//    Vector Position(3); Position(_) = state(_(VectorIndexBase,VectorIndexBase+2));
    static Vector Position(3); Position(_) = _currentOrbitState.GetState()(_(VectorIndexBase,VectorIndexBase+2));
    Forces = -398600.441 / pow(norm2(Position),3) * Position;
    return Forces;
}

Vector DragForceFunction(const ssfTime &_currentTime, const OrbitState &_currentOrbitState, const AttitudeState &_currentAttitudeState, const EnvFuncParamaterType &_parameterList)
{
    static Vector Forces(3);
    double BC = *(reinterpret_cast<double*>(_parameterList[0]));
    static Vector Vrel(3); Vrel(_) = _currentOrbitState.GetState()(_(VectorIndexBase+3,VectorIndexBase+5));
    double Vrel_mag = norm2(Vrel);
    double rho = *(reinterpret_cast<double*>(_parameterList[1]));
    Forces = -1/2 * rho / BC * pow(Vrel_mag,2) * Vrel / Vrel_mag; 
    return Forces;
}

void SetupEnvironment()
{
    // ENVIRONMENT TESTING
    myEnvironment = new Environment;
    EarthCentralBody *pCBEarth = new EarthCentralBody;
    myEnvironment->SetCentralBody(pCBEarth);

    return;
}
void ChangeEnvironment()
{
    // Add Gravity force function
    cout << "Filling Parameters" << std::endl;
    EnvFunction TwoBodyGravity(&GravityForceFunction);
    myEnvironment->AddForceFunction(TwoBodyGravity);
    
   
    return;
}

// *************************
// ****** PROPAGATOR ******* 
// ************************* 
void SetupPropagator()
{
    myPropagator = new CombinedNumericPropagator;
    orbitIntegrator = new RungeKuttaFehlbergIntegrator;
    attitudeIntegrator = new RungeKuttaFehlbergIntegrator;
        
    myPropagator->SetOrbitIntegrator(orbitIntegrator);
    myPropagator->SetAttitudeIntegrator(attitudeIntegrator);
        
    // Integration Times
    double intTime = 1;
    ssfTime begin(0);
    ssfTime end(begin + intTime);
    propTimes.push_back(begin);
    propTimes.push_back(end);
    return;
}

void ChangePropagator()
{
    DisplayPropagator();
    
    // Create & setup the integator
        // Setup an integrator and any special parameters
    int stepSize = 1;

    // Integration Times
    double intTime = 86400;

    double tolerance;
    orbitIntegrator->SetStepSizes(1, 200);
    tolerance = 0.000001;
    orbitIntegrator->SetTolerance(tolerance);
    attitudeIntegrator->SetStepSizes(0.01, 5);
    attitudeIntegrator->SetTolerance(tolerance);
    ssfTime begin(0);
    ssfTime end(begin + intTime);
    propTimes[0] = begin;
    propTimes[1] = end;
}
// *************************
// ********* ORBIT ********* 
// ************************* 
void myOrbitStateConvFunc(const Matrix &_meshPoint, OrbitState &_convertedOrbitState)
{
    static Vector tempVector(_meshPoint[MatrixColsIndex].getIndexBound() - 1);
    tempVector(_) = ~_meshPoint(_,_(MatrixIndexBase+1, _meshPoint[MatrixColsIndex].getIndexBound()));
    _convertedOrbitState.SetState(tempVector);
    return;
}

void ChangeOrbit()
{
    DisplayOrbit();
    
    // Create & initialize the orbit
    OrbitState myOrbitState;
    myOrbitState.SetStateRepresentation(new PositionVelocity);
    myOrbitState.SetOrbitFrame(new OrbitFrameIJK);
    Vector initPV(6);

        initPV(VectorIndexBase+0) = -2216.054359; // km
        initPV(VectorIndexBase+1) = -2837.094126; // km
        initPV(VectorIndexBase+2) = -6235.382907; // km
        initPV(VectorIndexBase+3) = 6.938058; // km
        initPV(VectorIndexBase+4) = -5.419317; // km
        initPV(VectorIndexBase+5) = 0; // km

    myOrbitState.SetState(initPV);
    myOrbit->SetStateObject(myOrbitState);
    
//    cout << "Choose the dynamics equation: " << flush;
    myOrbit->SetDynamicsEq(&TwoBodyDynamics);
    myOrbit->SetStateConversion(&myOrbitStateConvFunc);

    // Setup Parameters (ie mass, etc)
    return;
}
void SetupOrbit()
{
    myOrbit = new Orbit;
    return;
}


// *************************
// ******* ATTITUDE ******** 
// ************************* 
void myAttitudeStateConvFunc(const Matrix &_meshPoint, AttitudeState &_convertedAttitudeState)
{
    static Vector tempQ(4); tempQ(_) = ~_meshPoint(_,_(2, 5));
    static Vector tempVector(3); tempVector(_) = ~_meshPoint(1, _(6, 8));
    _convertedAttitudeState.SetState(Rotation(Quaternion(tempQ)), tempVector);
    return;
}
static Vector AttituteDynamics(const ssfTime &_time, const Vector& _integratingState, Orbit *_Orbit, Attitude *_Attitude, const Matrix &_parameters, const Functor &_forceFunctorPtr)
{
    static Vector stateDot(7);
    static Matrix bMOI(3,3); 
    static Matrix qtemp(4,3); 
    static Matrix Tmoment(3,1);
    static Vector qIn(4); 
    static Vector wIn(3);
    qIn = _integratingState(_(VectorIndexBase,VectorIndexBase + 3));
    wIn = _integratingState(_(VectorIndexBase + 4,VectorIndexBase + 6));
    qIn /= norm2(qIn);

//    _attitudeState->SetState(Rotation(Quaternion(qIn)), wIn);  // Need to speed up in some way
    qtemp(_(VectorIndexBase+0,VectorIndexBase+2),_(VectorIndexBase+0,VectorIndexBase+2)) = skew(qIn(_(VectorIndexBase+0,VectorIndexBase+2))) + qIn(VectorIndexBase+3) * eye(3);
    qtemp(VectorIndexBase+3, _(VectorIndexBase+0,VectorIndexBase+2)) = -(~qIn(_(VectorIndexBase+0,VectorIndexBase+2)));
    qtemp(_,MatrixIndexBase) = 0.5 * qtemp * wIn;

    bMOI = _parameters(_(MatrixIndexBase+0,MatrixIndexBase+2),_(MatrixIndexBase+0,MatrixIndexBase+2));
    Tmoment(_,_) = (_forceFunctorPtr.Call(_time, _Orbit->GetStateObject(), _Attitude->GetStateObject()))(_);
    Tmoment = (bMOI.inverse() * (Tmoment - skew(wIn) * (bMOI * wIn)));
    
    stateDot(_(VectorIndexBase,VectorIndexBase + 3)) = qtemp(_,MatrixIndexBase);
    stateDot(_(VectorIndexBase + 4,VectorIndexBase + 6)) = Tmoment(_,MatrixIndexBase);

    return stateDot;
}

void ChangeAttitude()
{
    DisplayAttitude();
    double q1,q2,q3,q4;
    cout << "Enter Attitude quaternion: " << flush;
    cin >> q1 >> q2 >> q3 >> q4;
    
    AttitudeState myAttitudeState;
    myAttitudeState.SetRotation(Rotation(Quaternion(q1,q2,q3,q4)));

    cout << "Enter Attitude angular velocity (rad/s): " << flush;
    cin >> q1 >> q2 >> q3;
    Vector initAngVelVector(3);
        initAngVelVector(1) = q1;
        initAngVelVector(2) = q2;
        initAngVelVector(3) = q3;
        
    myAttitudeState.SetAngularVelocity(initAngVelVector);

    myAttitude->SetStateObject(myAttitudeState);
    myAttitude->SetDynamicsEq(&AttituteDynamics);
    myAttitude->SetStateConversion(&myAttitudeStateConvFunc);
    myAttitude->SetParameters(eye(3));
    
    return;
}
void SetupAttitude()
{
    myAttitude = new Attitude;
    
    return;
    
}

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: testInterface.cpp,v $
*	Revision 1.3  2003/06/05 20:31:54  nilspace
*	Changed integrator to Runge-Kutta Fehlberg.
*	
*	Revision 1.2  2003/05/29 21:12:21  nilspace
*	Asks if the user wants to include drag.
*	
*	Revision 1.1  2003/05/26 21:23:21  nilspace
*	Initial submission.
*	
*
******************************************************************************/

                        
