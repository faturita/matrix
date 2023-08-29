//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file testDynamics.cpp
*  \brief Test program for trying out rigid body dynamics.
*  \author $Author: simpliciter $
*  \version $Revision: 1.3 $
*  \date    $Date: 2003/06/17 19:53:18 $
//////////////////////////////////////////////////////////////////////////////////////////////////
*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include "Matrix.h"
#include "Rotation.h"
#include "ASCIIConverter.h"
#include "BinaryConverter.h"
#include "MatrixConversionForm.h"
#include "MathUtils.h"
#include "math.h"

using namespace O_SESSAME;


/*
 * Devuelve un numero al azar entre iMin e iMax.
 */
int getProb(int iMin, int iMax)
{
    return (iMin+(int)(iMax*1.0*rand()/(RAND_MAX+1.0)));
}

int main()
{
    Matrix MOI = eye(3);

	
	cout << MOI << std::endl;

	Matrix I(3,3);
	
	
	Matrix M(3,3);
	
	M(2,2)=7.23;
	M(1,1)=-3;
	M(3,3)=-2.3;	
	
	Vector pos(3);
	pos(1) = 2.3;
	pos(2) = 1.4;
	pos(3) = -2.3;
	
	cout << "Initialization Finished." << std::endl;
		
	cout << M.inverse() << std::endl;
	cout << M.inverse() * pos << std::endl;
	
	DirectionCosineMatrix dcm1(Deg2Rad(30),Deg2Rad(-10),Deg2Rad(5), 123);
	DirectionCosineMatrix dcm2(Deg2Rad(20),Deg2Rad(9),Deg2Rad(3), 123);
	
	Rotation rot( dcm1 );
	
	cout << "Outcome Rotation:" << std::endl << dcm1*dcm2 <<  std::endl;
	
	
	Matrix M2(20,20);
	
	for(int i=1;i<=20;i++)
		for (int j=1;j<=20;j++)
			M2(i,j) = pow((-1),(i+j))*1*1.0*rand()/(RAND_MAX+1.0);
			
			
	cout << "Matrix:" << std::endl << M2 << std::endl;
	
	cout << "Inverting...." << std::endl;

	cout << M2.inverse() << std::endl;	
	
	
    return 0;
}

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: testMatrix.cpp,v $
*	Revision 1.3  2003/06/17 19:53:18  simpliciter
*	Added namespace call.
*	
*	Revision 1.2  2003/04/23 16:30:59  nilspace
*	Various bugfixes & uploading of all changed code for new programmers.
*	
*	Revision 1.1  2003/03/25 02:26:00  nilspace
*	initial submission of test files.
*
******************************************************************************/
