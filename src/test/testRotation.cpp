//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file testRotation.cpp
*  \brief Test program testing & verifying the rotation library.
*  \author $Author: nilspace $
*  \version $Revision: 1.4 $
*  \date    $Date: 2003/08/24 20:59:13 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <iomanip.h>
#include "Rotation.h"
#include "Matrix.h"
#include "ASCIIConverter.h"
#include "BinaryConverter.h"
#include "MatrixConversionForm.h"

using namespace O_SESSAME;

int main()
{
    Quaternion qInit(0,0,0,1);
    Rotation rotInit(qInit);
    cout << "q - R(q) = " << qInit - rotInit.GetQuaternion() << std::endl;
    cout << "R(q)->DCM = " << rotInit.GetDCM() << std::endl;
    Rotation rot2(Quaternion(0.5,0.5,0.5,0.5));
    cout << "R(q(0.5,0.5,0.5,0.5)) = " <<  rot2.GetDCM() << std::endl;

    Rotation Rot321(Deg2Rad(10),Deg2Rad(0.1),Deg2Rad(-5),321);
    cout << "R(10, 0.1, -5, 321) (degs) = " << Rot321.GetDCM() << std::endl;
    cout << "R(10, 0.1, -5, 321) (degs) = " << Rot321.GetDCM().GetQuaternion().GetDCM() << std::endl;


    cout << "R1(-2)" << std::setprecision(15) << R1(Deg2Rad(-2)) << std::endl;
    cout << "R2(-5)" << std::setprecision(15) << R2(Deg2Rad(-5)) << std::endl;
    cout << "R3(-10)" << std::setprecision(15) << R3(Deg2Rad(10)) << std::endl; 
    DirectionCosineMatrix DCM1(Deg2Rad(10),Deg2Rad(0.1),Deg2Rad(-5),321);
    cout << "DCM(10, 0.1, -5, 321) (degs) = " << std::endl << std::setprecision(15) << DCM1 << std::endl;
    Vector xAxis(3);
    xAxis(1) = 1;
    cout << ~xAxis << " -> " << ~(rot2 * xAxis) << std::endl;
    cout << ~xAxis << " -> " << ~(rotInit * xAxis) << std::endl;
    
//    ModifiedRodriguezParameters mrp1(Quaternion(0,0.2, -0.3, .2));
    ModifiedRodriguezParameters mrp1(0,0.2, -0.3);
    cout << "mrp1(0,0.2, -0.3): " << ~mrp1;
    cout << "sigmaSq = " << (~mrp1 * mrp1);
    cout << "mrp1->Quat: " << ~mrp1.GetQuaternion();
//    Rotation rot3(mrp1);
    Rotation rot3(mrp1.GetQuaternion());
    cout << "Rot3->Quat->MRP: " << ~rot3.GetQuaternion().GetMRP();
    cout << "Rot3->MRP: " << ~rot3.GetMRP() << std::endl;
    cout << "Rot3->Quat: " << ~rot3.GetQuaternion();
    cout << "Rot3.DCM->Quaternion: " << ~rot3.GetDCM().GetQuaternion() << std::endl;
    cout << "mrp1->DCM: " << std::endl << mrp1.GetDCM() << std::endl;
    cout << "mrp->Quat->DCM: " << std::endl << mrp1.GetQuaternion().GetDCM() << std::endl;
    cout << "Rot3.DCM: " << std::endl <<  rot3.GetDCM() << std::endl;
    
    // Text save & restore
    MatrixConversionForm convForm;
    ASCIIConverter rotOutput("rotationSave.txt");
    convForm.SetMatrix(rot3.GetDCM());
    rotOutput.Export(convForm);
    
    ASCIIConverter rotInput("rotationSave.txt");
    rotInput.Import(convForm);
    cout << "Imported Matrix: " <<  convForm.GetMatrix();
    Rotation rotImport(convForm.GetMatrix());
    cout << "Rot3_Import MRP: " << ~rotImport.GetMRP();

    // Binary save & restore
    MatrixConversionForm binConvForm;
    convForm.SetMatrix(rot3.GetDCM());
    BinaryConverter rotOutputBIN("rotationSave");
    convForm.SetMatrix(rot3.GetDCM());
    rotOutputBIN.Export(convForm);
    
    BinaryConverter rotInputBIN("rotationSave");
    rotInputBIN.Import(binConvForm);
    cout << "Imported Matrix: " <<  binConvForm.GetMatrix();
    Rotation rotImportBin(binConvForm.GetMatrix());
    cout << "Rot3_Import Binary MRP: " << ~rotImportBin.GetMRP();
    return 0;
}

// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: testRotation.cpp,v $
*	Revision 1.4  2003/08/24 20:59:13  nilspace
*	Updated.
*	
*	Revision 1.3  2003/04/27 22:04:34  nilspace
*	Created the namespace O_SESSAME.
*	
*	Revision 1.2  2003/04/23 16:30:59  nilspace
*	Various bugfixes & uploading of all changed code for new programmers.
*	
*	Revision 1.1  2003/03/25 02:26:00  nilspace
*	initial submission of test files.
*
******************************************************************************/
