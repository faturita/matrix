#include <ostream>
#include <iomanip.h>
#include "Rotation.h"
#include "Matrix.h"
#include "Attitude.h"
#include "LinearInterpolator.h"
#include "RungeKuttaIntegrator.h"
using namespace O_SESSAME;
 
 // Torque function that will be called each timestep
Vector NullFunctor(const ssfTime& _pSSFTime, const OrbitState& _pOrbitState, const AttitudeState& _pAttitudeState)
{
    return Vector(3);
}

static Vector EugeneDynamics(const ssfTime &_time, const Vector& _integratingState, Orbit *_Orbit, Attitude *_Attitude, const Matrix &_parameters, const Functor &_forceFunctorPtr)
{
    //function statedot=test1_rhs(t,state)
    static Vector h = _integratingState(_(1,3)); 	//h=state(1:3);
    static double T = _integratingState(4);		//T=state(4);

    Matrix I(3,3);				//I=[100 0 0;0 200 0;0 0 150];
        I(1,1) = 100;
        I(2,2) = 200;
        I(3,3) = 150;
    static Vector w = I.inverse() * h;			//w=inv(I)*h;
    Vector ge(3);				//ge=[0 0 0]';
    
    static Vector hdot = skew(w) * h + ge;		//hdot=-cr(w)*h+ge;
    static Vector wdot = I.inverse() * hdot;		//wdot=inv(I)*hdot;
    static Matrix temp = (~ge * wdot);			//Tdot=ge'*wdot;
    double Tdot = temp(1,1);
    
    static Vector statedot(hdot.getIndexBound() + 1);
        statedot(_(1, hdot.getIndexBound())) = hdot(_);
        statedot(hdot.getIndexBound()+1) = Tdot;
        
    return statedot;				//statedot=[hdot; Tdot];

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
    Tmoment = (bMOI.inverse() * (Tmoment - skew(wIn) * (bMOI * wIn)));
    
    stateDot(_(VectorIndexBase,VectorIndexBase + 3)) = qtemp(_,MatrixIndexBase);
    stateDot(_(VectorIndexBase + 4,VectorIndexBase + 6)) = Tmoment(_,MatrixIndexBase);

    return stateDot;
}
int main()
{

//Rotations
//1
Quaternion q(0.5,0.5,0.5,0.5);

//2

cout << q << q.GetMRP() << std::endl;

//3
DirectionCosineMatrix dcm(10,-30,150, 321);

//4
Rotation qrot(q);
Rotation dcmrot(dcm);

//5
Rotation rot2;
rot2 = qrot + dcmrot;
cout << rot2.GetDCM() << std::endl;

//utilities
//ssfSeconds sec(10);
ssfTime time;
time.SetEpoch(Now());
time.Set(time.GetEpoch()+50);

tm date = time.GetDateTime();
cout << date.tm_mday << "  " << date.tm_mon << "  "  << date.tm_year << std::endl << std::endl << std::endl;


//cout << std::setprecision(15) << time.GetJulianDate() << std::endl;//http://aa.usno.navy.mil/data/docs/JulianDate.html
//cout << time.GetSeconds() << std::endl;

//tm date = time.GetDateTime();
//cout << date.tm_hour << "  " << date.tm_mday << "  "  << date.tm_year << std::endl;

//interpolation

Vector timevec(10);
Matrix datavec(10,1);
for (int J = 1; J < 11;  J = J+1){
    timevec(J) = J/10.0;
    datavec(J,1) = sin(J/10.0);}

LinearInterpolator interp(timevec,datavec);
Vector chk = interp.Evaluate(0.25);
cout <<  timevec << std::endl << datavec << std::endl << chk << std::endl;



//Attitude
Quaternion q2(0,0,0,1);
Rotation rot(q2);
AttitudeState att(rot);


    // Setup an integrator and any special parameters
    RungeKuttaIntegrator myIntegrator; 
    myIntegrator.SetNumSteps(1000);

    // Create the initial attitude state
    AttitudeState myAttitudeState;
    myAttitudeState.SetRotation(Rotation(Quaternion(0,0,0,1)));
    Vector initAngVelVector(3);
        initAngVelVector(1) = 0.1;
    myAttitudeState.SetAngularVelocity(initAngVelVector);
    
    // Create the matrix of parameters needed for the RHS - MOI
    Matrix Parameters = eye(3);
 
    // Integration times
    vector<ssfTime> integrationTimes;
    ssfTime begin(0);
    ssfTime end(begin + 20);
    integrationTimes.push_back(begin);
    integrationTimes.push_back(end);
    cout << "PropTime = " << begin.GetSeconds() << " s -> " << end.GetSeconds() << " s" << std::endl;
    cout << "Attitude State: " << ~myAttitudeState.GetState() << std::endl;

    SpecificFunctor AttitudeForcesFunctor(&NullFunctor);
    
    Matrix I(3,3);				//I=[100 0 0;0 200 0;0 0 150];
        I(1,1) = 100;
        I(2,2) = 200;
        I(3,3) = 150;
    Vector h(3);
        h(1) = 10;
        h(2) = 20;
        h(3) = 30;

    Vector w = I.inverse() * h;
    Matrix Tmat = 0.5 * (~w * (I * w));
    Vector initEugeneState(4);
	initEugeneState(_(1,3)) = h(_(1,3));
        initEugeneState(4) = Tmat(1,1);
    // Integrate over the desired time interval
    tick();
    Matrix history = myIntegrator.Integrate(
                            integrationTimes, 		// seconds
                            &AttituteDynamics, 
                            myAttitudeState.GetState(),
                            NULL,
                            NULL,
                            Parameters,
                            AttitudeForcesFunctor
                        );
    cout << tock();
return 0;
}


