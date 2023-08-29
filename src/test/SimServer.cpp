//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file SimServer.cpp
*  \brief Creates a simulation server that allows external programs to communicate with and control
*	an Open-SESSAME application.
*  \author $Author: nilspace $
*  \version $Revision: 1.2 $
*  \date    $Date: 2003/05/29 21:12:21 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*!  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

// Netxx Includes
#include <Netxx/NetBuf.h>
#include <Netxx/Address.h>
#include <Netxx/Peer.h>
#include <Netxx/Stream.h>
#include <Netxx/StreamServer.h>
#include <Netxx/Timeout.h>
#include <Netxx/Types.h>

// O_SESSAME Includes
#include "Matrix.h"
#include "Rotation.h"
#include "Attitude.h"
#include "Orbit.h"
#include "CombinedNumericPropagator.h"
#include "RungeKuttaIntegrator.h"
#include "orbitmodels/TwoBodyDynamics.h"
#include "EarthCentralBody.h"
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
static RungeKuttaIntegrator* orbitIntegrator = NULL; 
static RungeKuttaIntegrator* attitudeIntegrator = NULL; 
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
void Serve(); 
void Plot();

void myOrbitStateConvFunc(const Matrix &_meshPoint, OrbitState &_convertedOrbitState);
void myAttitudeStateConvFunc(const Matrix &_meshPoint, AttitudeState &_convertedAttitudeState);

void Serve()
{
    try {
	Netxx::Address address;
	Netxx::Timeout timeout(10);
        address.add_all_addresses(7000);

	Netxx::StreamServer server(address, timeout);

	char buffer[1024];

	for (;;) {
	    Netxx::Peer client = server.accept_connection();

	    if (!client) {
		std::cout << ": timeout waiting for connection" << std::std::endl;
		continue;
	    }

	    Netxx::Stream client_stream(client.get_socketfd(), timeout);
	    Netxx::signed_size_type bytes_read;
            Netxx::Netbuf<1024> sbuf(client_stream);
            std::istd::ostream stream(&sbuf);
            std::string string_buffer;
        
	    std::cout << ": connection from " << client << std::std::endl;
	    while ((bytes_read = client_stream.read(buffer, sizeof(buffer))))
            {
                Propagate();
                stream << myOrbit->GetStateObject().GetStateRepresentation()->GetPositionVelocity() << std::std::endl;
            }
	    std::cout << ": client disconnected from server" << std::std::endl;
	}
    } catch (std::exception &e) {
	std::std::cerr << ": " << e.what() << std::std::endl;
	return 1;
    }
}

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
int DisplayMenu()
{
    DisplayOrbit();
    DisplayAttitude();
    DisplayPropagator();
    cout << std::endl;
    cout << "[1] Setup Orbit" << std::endl;
    cout << "[2] Setup Orbit Integrator" << std::endl;
    cout << "[3] Setup Attitude" << std::endl;
    cout << "[4] Setup Attitude Integrator" << std::endl;
    cout << "[5] Setup Environment" << std::endl;
    cout << "[6] Setup Propagator" << std::endl;
    cout << "[7] Serve" << std::endl;
    cout << "[8] Plot" << std::endl;
    cout << "[0] Exit" << std::endl;
    cout << std::endl << "Selection: " << flush;
    
    int selection;
    cin >> selection;
    return selection;
}

void Select()
{
    switch(DisplayMenu())
    {
        case 1: // ChangeOrbit
            ChangeOrbit();
            break;
        case 2:
            ChangePropagator();
            //ChangeOrbitIntegrator();
            break;
        case 3: 
            ChangeAttitude();
            break;
        case 4:
            ChangePropagator();
            //ChangeAttitudeIntegrator();
            break;
        case 5:
            ChangeEnvironment();
            break;
        case 6:
            ChangePropagator();
            break;
        case 7:
            Serve();
            break;
        case 8:
            ::Plot();
            break;
        case 0:
            return;
            break;
        default:
            cout << "Incorrect Selection." << std::endl;
            break;
    }
    Select();

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
   
   Select(); 
    // Store the output to file
    std::ofstream ofile;

    if(myOrbit)
        if (myOrbit->IsIntegrateable())
        {
            ofile.open("OrbitHistory.dat");
            ofile << std::setprecision(15) << myOrbit->GetHistory().GetHistory();
            ofile.close();
        }
    
    if(myAttitude)
        if (myAttitude->IsIntegrateable())
        {
            ofile.open("AttitudeHistory.dat");
            ofile << std::setprecision(15) << myAttitude->GetHistory().GetHistory();
            ofile.close();
        }
    return 0;
                            
}

void Plot()
{
    if(myOrbit)
        if (myOrbit->IsIntegrateable())
        {
            Matrix orbitHistory = myOrbit->GetHistory().GetHistory();
            Matrix orbitPlotting = orbitHistory(_,_(MatrixIndexBase+1,MatrixIndexBase+3));
            Plot3D(orbitPlotting);
        }
    if(myAttitude)
        if (myAttitude->IsIntegrateable())
        {
            Matrix attitudeHistory = myAttitude->GetHistory().GetHistory();
            Matrix attitudePlotting = attitudeHistory(_,_(MatrixIndexBase,MatrixIndexBase+4));
            Plot2D(attitudePlotting);
        }

}

void Propagate()
{

    cout << "PropTime = " << propTimes[0].GetSeconds() << " s -> " << propTimes[1].GetSeconds() << " s" << std::endl;
    cout << "Orbit State: " << ~myOrbit->GetStateObject().GetState() << std::endl;
    cout << "Attitude State: " << ~myAttitude->GetStateObject().GetState() << std::endl;

    // Integrate over the desired time interval
    tick();
    myPropagator->Propagate(propTimes, myOrbit->GetStateObject().GetState(), myAttitude->GetStateObject().GetState());
    ssfSeconds calcTime = tock();
    cout << "finished propagating in " << calcTime << " seconds." << std::endl;
}
// *************************
// ****** ENVIRONMENT ****** 
// *************************
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
    
    cout << "Add Drag? (y or n): " << flush;
    char answer;
    cin >> answer;
    if(answer == 'y')
    {
        // Add Drag Force Function
        EnvFunction DragForce(&SimpleAerodynamicDragForce);
        double *BC = new double(200);
        DragForce.AddParameter(reinterpret_cast<void*>(BC), 1);
        double *rho = new double(1.13 * pow(static_cast<double>(10), static_cast<double>(-12))); // kg/m^3
        DragForce.AddParameter(reinterpret_cast<void*>(rho), 2);
        myEnvironment->AddForceFunction(DragForce);
    }
    cout << "Finished Setting up Earth Environment" << std::endl;
       
    // Adds a controller
    EnvFunction Controller(&SimpleController);
    Vector* Gains = new Vector(3);
    cout << "Input Control gains: "<< flush;
    cin >> (*Gains)(1);
    cin >> (*Gains)(2);
    cin >> (*Gains)(3);
    
    Controller.AddParameter(reinterpret_cast<void*>(Gains),1);
    myEnvironment->AddTorqueFunction(Controller);    
    return;
}

// *************************
// ****** PROPAGATOR ******* 
// ************************* 
void SetupPropagator()
{
    myPropagator = new CombinedNumericPropagator;
    orbitIntegrator = new RungeKuttaIntegrator;
    attitudeIntegrator = new RungeKuttaIntegrator;
        
    myPropagator->SetOrbitIntegrator(orbitIntegrator);
    myPropagator->SetAttitudeIntegrator(attitudeIntegrator);
    
    myPropagator->SetOrbitStateConversion(&myOrbitStateConvFunc);
    myPropagator->SetAttitudeStateConversion(&myAttitudeStateConvFunc);
    
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
    double intTime = 1;
    cout << "Integration duration (s): " << flush;
    cin >> intTime;
      
    cout << "Orbit Integration Stepsize (s): "<< flush;
    cin >> stepSize;
    orbitIntegrator->SetNumSteps(floor(intTime/stepSize));

    cout << "Attitude Integration Stepsize(s): "<< flush;
    cin >> stepSize;
    attitudeIntegrator->SetNumSteps(floor(intTime/stepSize));
    
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
    //-5776.6 -157 3496.9
    // -2.595 -5.651 -4.513
        cout << "Enter the position vector (km): " << flush;
        cin >> initPV(VectorIndexBase+0); // km
        cin >> initPV(VectorIndexBase+1); // km
        cin >> initPV(VectorIndexBase+2); // km
        cout << "Enter the velocity vector (km): " << flush;
        cin >> initPV(VectorIndexBase+3); // km
        cin >> initPV(VectorIndexBase+4); // km
        cin >> initPV(VectorIndexBase+5); // km
    myOrbitState.SetState(initPV);
    myOrbit->SetStateObject(myOrbitState);
    
//    cout << "Choose the dynamics equation: " << flush;
    myOrbit->SetDynamicsEq(&TwoBodyDynamics);
    
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
*	Revision 1.2  2003/05/29 21:12:21  nilspace
*	Asks if the user wants to include drag.
*	
*	Revision 1.1  2003/05/26 21:23:21  nilspace
*	Initial submission.
*	
*
******************************************************************************/

                        
