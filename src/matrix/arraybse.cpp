#include "arraybse.h"
#include "vecbse.h"
#include "matbse.h"
#include "bnengine.h"

//
//***************************************************************
//                    ARRAYBSE.CPP
//***************************************************************
//
//
//*****************************************************************
//
//            Chris Anderson
//
//            Mon Sep 11 12:49:20 1995
//
//*****************************************************************
//
//
//*****************************************************************
//                    CONSTRUCTORS
//*****************************************************************
//
//
//*****************************************************************
//               
//*****************************************************************
//
CAMarrayBase::CAMarrayBase()
: Structure()
{
    DataP                   = 0;
    typeValue               = 0;
    referenceFlag           = 0;
    arrayBaseReferenceCount = 0;
}

CAMarrayBase::CAMarrayBase(int d_type)
: Structure()
{
    DataP                   = 0;
    typeValue               = d_type;
    referenceFlag           = 0;
    arrayBaseReferenceCount = 0;
}

CAMarrayBase::CAMarrayBase(const CAMarrayBase& A)
:Structure(A.Structure)
{
    if(A.DataP == 0)
    {DataP = 0;}
    else
    {
     if(A.referenceFlag == 1)
     {DataP = A.DataP; }
     else if(A.DataP->getTemporaryFlag() == 1)
     {
      DataP = new CAMdataHandler(*(A.DataP));
      DataP ->setReferenceCount(1);
     }
     else
     {
      A.Structure.initializeMinStructure(Structure);
      DataP = new CAMdataHandler(Structure.getFullDataCount(),A.DataP->getDataType());
      DataP->setReferenceCount(1);
      //
      //  copy based on type
      //
      CAMbinaryEngine::doubleAequalToB(Structure, *DataP, A.Structure, *A.DataP);
      //
      //
      //
     }
    }
    typeValue               = A.typeValue;
    referenceFlag           = 0;
    arrayBaseReferenceCount = 0;
}

CAMarrayBase::CAMarrayBase(int d_type, const CAMrange& R1, const CAMrange& R2 ,
                        const CAMrange& R3 , const CAMrange&R4 , 
                        const CAMrange& R5 , const CAMrange&R6 ,
                        const CAMrange& R7)
:Structure(R1,R2,R3,R4,R5,R6,R7)
{
     DataP = new CAMdataHandler(Structure.getFullDataCount(),d_type);
     DataP->setReferenceCount(1);
     typeValue               = d_type;
     referenceFlag           = 0;
     arrayBaseReferenceCount = 0;
}

//
//*****************************************************************
//                    DESTRUCTOR
//*****************************************************************
//
CAMarrayBase::~CAMarrayBase()
{
    if(DataP != 0)
    {
    DataP->decrementReferenceCount();
    if(DataP->getReferenceCount() < 0)
    {
    std::cerr << " Error : Reference Count < 0" << std::endl;
    }
    if(DataP->getReferenceCount() == 0) delete DataP;
    }
}
//
//*****************************************************************
//                    ASSIGNMENT
//*****************************************************************
//
void CAMarrayBase::operator =(double value)
{
//
//  NEED CONVERSION CHECK
//
    if(DataP == 0)
    {
    Structure.initialize(_(1,1));
    DataP = new CAMdataHandler(1,CAMType::typeDouble);
    DataP->setReferenceCount(1);
    referenceFlag  = 0;
    arrayBaseReferenceCount = 0;
    }
    else
    {
    CAMstructureBase AStructure(_(1,1));
    if(Structure.isConformingTo(AStructure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,AStructure);}
    }

    CAMbinaryEngine::doubleAequalToAlpha(Structure, *DataP, value);
}
void  CAMarrayBase::operator =( const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    if(DataP == 0)
    {
    A.Structure.initializeMinStructure(Structure);
    DataP = new CAMdataHandler(Structure.getFullDataCount(),typeValue);
    DataP->setReferenceCount(1);
    typeValue               = CAMType::typeDouble;
    referenceFlag           = 0;
    arrayBaseReferenceCount = 0;
    }
    else
    {
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
    }

    CAMbinaryEngine::doubleAequalToB(Structure, *DataP, A.Structure, *A.DataP);
}
std::ostream& operator <<(std::ostream& out_stream, const CAMarrayBase& A)
{
    const long* beginPtr  = A.Structure.indexBegin.getDataPointer();
    const long* endPtr    = A.Structure.indexEnd.getDataPointer();
    const long* stridePtr = A.Structure.indexStride.getDataPointer();
    long i1, i2, i3, i4, i5, i6, i7;
    long i;

    int  saveWidth=out_stream.width();
    double OutValue;
//
//  NEED CONVERSION CHECK
//
    switch(A.Structure.dataDimension)
    {
    case 1 :
    for(i1 = *beginPtr; i1 <= *endPtr; i1+= *stridePtr)
    {
     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//    out_stream << *((double*)A.getDataPointer(i1)) << " ";
    }
    break;

    case 2 :
//
//  Use orientation of first quadrant of a cartesian grid.
//
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {
     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//   out_stream << *((double*)A.getDataPointer(i1,i2)) << " ";
    }
    out_stream << std::endl;
    }

    break;

    case 3 :
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2,i3));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3)) << " ";
    }
    out_stream << std::endl;
    }
    out_stream << std::endl << std::endl;
    }

    break;

    case 4 :

    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {
     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2,i3,i4));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4)) << " ";
    }
    out_stream << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }

    break;

    case 5 :

    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2,i3,i4,i5));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5)) << " ";
    }
    out_stream << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }

    case 6 :

    for(i6 = *(beginPtr + 5); i6 <= *(endPtr + 5); i6+= *(stridePtr + 5))
    {
    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {
     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//   out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6)) << " ";
    }
    out_stream << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    break;


    case 7 :

    for(i7 = *(beginPtr + 6); i7 <= *(endPtr + 6); i7+= *(stridePtr + 6))
    {
    for(i6 = *(beginPtr + 5); i6 <= *(endPtr + 5); i6+= *(stridePtr + 5))
    {
    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     out_stream.width(saveWidth);
     OutValue =*((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6,i7));
     if(OutValue < 0 )
     out_stream << OutValue <<" ";
     else
     out_stream << " " << OutValue <<" ";

//     out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6,i7)) << " ";
    }
    out_stream << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    out_stream << std::endl << std::endl;
    }
    break;
    }
    return(out_stream);

}
std::istream& operator >>(std::istream& in_stream, CAMarrayBase& A)
{
    const long* beginPtr  = A.Structure.indexBegin.getDataPointer();
    const long* endPtr    = A.Structure.indexEnd.getDataPointer();
    const long* stridePtr = A.Structure.indexStride.getDataPointer();
    long i1, i2, i3, i4, i5, i6, i7;
    long i;

    if( A.Structure.getFullDataCount() == 0)
    {CAMarrayBase::nullOperandError(">>");}
//
//  NEED CONVERSION CHECK
//
    switch(A.Structure.dataDimension)
    {
    case 1 :
    for(i1 = *beginPtr; i1 <= *endPtr; i1+= *stridePtr)
    {
     if( !( in_stream >> *((double*)A.getDataPointer(i1))))
     CAMarrayBase::inputSizeError();

//    out_stream << *((double*)A.getDataPointer(i1)) << " ";
    }
    break;

    case 2 :
//
//  Use orientation of first quadrant of a cartesian grid.
//
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {
     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2))))
     CAMarrayBase::inputSizeError();

//   out_stream << *((double*)A.getDataPointer(i1,i2)) << " ";
    }}

    break;

    case 3 :
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2,i3))))
     CAMarrayBase::inputSizeError();

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3)) << " ";
    }}}

    break;

    case 4 :

    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {
     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2,i3,i4))))
     CAMarrayBase::inputSizeError();

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4)) << " ";
    }}}}

    break;

    case 5 :

    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2,i3,i4,i5))))
     CAMarrayBase::inputSizeError();

//    out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5)) << " ";
    }}}}}

    case 6 :

    for(i6 = *(beginPtr + 5); i6 <= *(endPtr + 5); i6+= *(stridePtr + 5))
    {
    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6))))
     CAMarrayBase::inputSizeError();

//   out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6)) << " ";
    }}}}}}
    break;


    case 7 :

    for(i7 = *(beginPtr + 6); i7 <= *(endPtr + 6); i7+= *(stridePtr + 6))
    {
    for(i6 = *(beginPtr + 5); i6 <= *(endPtr + 5); i6+= *(stridePtr + 5))
    {
    for(i5 = *(beginPtr + 4); i5 <= *(endPtr + 4); i5+= *(stridePtr + 4))
    {
    for(i4 = *(beginPtr + 3); i4 <= *(endPtr + 3); i4+= *(stridePtr + 3))
    {
    for(i3 = *(beginPtr + 2); i3 <= *(endPtr + 2); i3+= *(stridePtr + 2))
    {
    for(i = ((*(endPtr +1) - *(beginPtr +1))/(*(stridePtr + 1))) + 1;
        i >= 1; i--)
    {
    i2 = *(beginPtr +1) + (i-1)*(*(stridePtr + 1));

    for(i1 = *beginPtr; i1 <= *endPtr; i1 += *stridePtr)
    {

     if( !( in_stream >> *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6,i7))))
     CAMarrayBase::inputSizeError();

//     out_stream << *((double*)A.getDataPointer(i1,i2,i3,i4,i5,i6,i7)) << " ";
    }}}}}}}
    break;
    }
    return(in_stream);

}

//
//*****************************************************************
//             INITIALIZATION MEMBER_FUNCTIONS
//*****************************************************************
//

void  CAMarrayBase::initialize()
{
    Structure.initialize();
    if(DataP != 0)
     {
      DataP->decrementReferenceCount();
      if(DataP->getReferenceCount() == 0) delete DataP;
     }
    DataP = 0;
    typeValue      = 0;
    referenceFlag  = 0;
    arrayBaseReferenceCount = 0;
}

void  CAMarrayBase::initialize(int d_type)
{
    Structure.initialize();
    if(DataP != 0)
     {
      DataP->decrementReferenceCount();
      if(DataP->getReferenceCount() == 0) delete DataP;
     }
    DataP = 0;
    typeValue      = d_type;
    referenceFlag  = 0;
    arrayBaseReferenceCount = 0;
}

void  CAMarrayBase::initialize(const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    if(DataP != 0)
    {
    DataP->decrementReferenceCount();
    if(DataP->getReferenceCount() == 0) delete DataP;
    }

    if(A.DataP != 0)
    {
    A.Structure.initializeMinStructure(Structure);

    DataP = new CAMdataHandler(Structure.getFullDataCount(),A.DataP->getDataType());
    DataP->setReferenceCount(1);
    CAMbinaryEngine::doubleAequalToB(Structure, *DataP, A.Structure, *A.DataP);
    }
    else
    {
    DataP = 0;
    Structure.initialize();
    }

    typeValue      = A.typeValue;
    referenceFlag  = 0;
    arrayBaseReferenceCount = 0;
}

void  CAMarrayBase::initialize(int d_type, const CAMrange& R1, const CAMrange& R2 ,
                        const CAMrange& R3 , const CAMrange&R4 ,
                        const CAMrange& R5 , const CAMrange&R6 ,
                        const CAMrange& R7 )
{
     Structure.initialize(R1,R2,R3,R4,R5,R6,R7);

     if(DataP != 0)
     {
     DataP->decrementReferenceCount();
     if(DataP->getReferenceCount() == 0) delete DataP;
     }

     DataP = new CAMdataHandler(Structure.getFullDataCount(),d_type);
     DataP->setReferenceCount(1);
     typeValue               = d_type;
     referenceFlag           = 0;
     arrayBaseReferenceCount = 0;
}

//
//***************************************************************
//                    ARRAYOPS.CPP
//***************************************************************
//
//
//*****************************************************************
//
void  CAMarrayBase::initializeReturnArgument(const CAMarrayBase& A)
{
    A.Structure.initializeMinStructure(Structure);
    DataP = new CAMdataHandler(Structure.getFullDataCount(),A.DataP->getDataType());
    DataP->setReferenceCount(1);
}
void  CAMarrayBase::initializeReturnArgument(const CAMstructureBase& S, int dataT)
{
    S.initializeMinStructure(Structure);
    DataP = new CAMdataHandler(Structure.getFullDataCount(), dataT);
    DataP->setReferenceCount(1);
}
void  CAMarrayBase::initializeMinDuplicate(const CAMarrayBase& A)
{

    if(A.DataP == 0) return;

    long i;
    long icount    = 0;
    long dimension = 0;

    for(i = 1; i <= A.Structure.getDimension(); i++)
    if(A.Structure[i].getIndexCount() != 1) dimension++;


    if(dimension == 0) dimension = 1;
    Structure.initialize(dimension);


    for(i = 1; i <= A.Structure.getDimension(); i++)
    {
      if(A.Structure[i].getIndexCount() != 1)
      {
      Structure.indexBegin[icount]     = 1;
      Structure.indexBeginBase[icount] = 1;
      Structure.indexEndBase[icount]   = A.Structure[i].getIndexCount();
      Structure.indexEnd[icount]       = A.Structure[i].getIndexCount();
      Structure.indexStride[icount]    = 1;
      icount++;
      }
    }

    if(icount == 0)
    {
    Structure.indexBegin[icount]     = 1;
    Structure.indexBeginBase[icount] = 1;
    Structure.indexEndBase[icount]   = 1;
    Structure.indexEnd[icount]       = 1;
    Structure.indexStride[icount]    = 1;
    }
    DataP = new CAMdataHandler(Structure.getFullDataCount(), A.typeValue);
    DataP->setReferenceCount(1);
//
//  copy over data
//
    *this = A;
}
//
//*****************************************************************
//                    Operators
//*****************************************************************
//
CAMarrayBase CAMarrayBase::operator-() const
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S;
    S.initializeReturnArgument(*this);

    CAMbinaryEngine::doubleAequalToMinusB(S.Structure, *S.DataP, Structure, *DataP);
    S.setTemporaryFlag();
    return S;
}

CAMarrayBase CAMarrayBase::operator+(const CAMarrayBase& A) const
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S;
    S.initializeReturnArgument(A.Structure,A.DataP->getDataType());

    CAMbinaryEngine::doubleCequalAplusB(Structure, *DataP, A.Structure, *A.DataP,
    S.Structure, *S.DataP);

    S.setTemporaryFlag();
    return S;
}
CAMarrayBase CAMarrayBase::operator-(const CAMarrayBase& A) const
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S;
    S.initializeReturnArgument(A.Structure,A.DataP->getDataType());

    CAMbinaryEngine::doubleCequalAminusB(Structure, *DataP, A.Structure, *A.DataP,
    S.Structure, *S.DataP);

    S.setTemporaryFlag();
    return S;
}
CAMarrayBase CAMarrayBase::operator*(const CAMarrayBase& A) const
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S;
    S.initializeReturnArgument(A);

    CAMbinaryEngine::doubleCequalAtimesB(Structure, *DataP, A.Structure, *A.DataP,
    S.Structure, *S.DataP);

    S.setTemporaryFlag();
    return S;
}
CAMarrayBase CAMarrayBase::operator/(const CAMarrayBase& A) const
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S;
    S.initializeReturnArgument(A);

    CAMbinaryEngine::doubleCequalAdivideB(Structure, *DataP, A.Structure, *A.DataP,
    S.Structure, *S.DataP);

    S.setTemporaryFlag();
    return S;
}
//
//*****************************************************************
//                    op = Operators
//*****************************************************************
//

void CAMarrayBase::operator+=(const CAMarrayBase& A)
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//

    CAMbinaryEngine::doubleAplusEqualB(Structure, *DataP, A.Structure, *A.DataP);
}
void CAMarrayBase::operator-=(const CAMarrayBase& A)
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//

    CAMbinaryEngine::doubleAminusEqualB(Structure, *DataP, A.Structure, *A.DataP);
}
void CAMarrayBase::operator*=(const CAMarrayBase& A)
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//

    CAMbinaryEngine::doubleAtimesEqualB(Structure, *DataP, A.Structure, *A.DataP);
}

void CAMarrayBase::operator/=(const CAMarrayBase& A)
{
    if(Structure.isConformingTo(A.Structure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,A.Structure);}
//
//  NEED CONVERSION CHECK
//

    CAMbinaryEngine::doubleAdivideEqualB(Structure, *DataP, A.Structure, *A.DataP);
}
//
//*****************************************************************
//                    Operations with Scalers
//*****************************************************************
//
CAMarrayBase  CAMarrayBase::operator +(const double value) const
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase AStructure(_(1,1));
    if(Structure.isConformingTo(AStructure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,AStructure);}

    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAplusEqualAlpha(S.Structure, *(S.DataP), value);

    S.setTemporaryFlag();
    return S;
}
CAMarrayBase  operator +(const double value, const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase  BStructure(_(1,1));
    if(A.Structure.isConformingTo(BStructure) != 1)
    {CAMarrayBase::nonConformingMessage(A.Structure,BStructure);}

    CAMarrayBase S(A);
    CAMbinaryEngine::doubleAplusEqualAlpha(S.Structure, *(S.DataP), value);

    S.setTemporaryFlag();
    return S;
}

CAMarrayBase  CAMarrayBase::operator -(const double value) const
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase AStructure(_(1,1));
    if(Structure.isConformingTo(AStructure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,AStructure);}

    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAminusEqualAlpha(S.Structure, *(S.DataP), value);

    S.setTemporaryFlag();
    return S;
}

CAMarrayBase  operator -(const double value, const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase  BStructure(_(1,1));
    if(A.Structure.isConformingTo(BStructure) != 1)
    {CAMarrayBase::nonConformingMessage(A.Structure,BStructure);}

    CAMarrayBase S(A);
    CAMbinaryEngine::doubleAminusEqualAlpha(S.Structure, *(S.DataP), value);

    S.setTemporaryFlag();
    return S;
}

void  CAMarrayBase::operator +=(const double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase AStructure(_(1,1));
    if(Structure.isConformingTo(AStructure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,AStructure);}

    CAMbinaryEngine::doubleAplusEqualAlpha(Structure, *(DataP), value);
}

void  CAMarrayBase::operator -=(const double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMstructureBase AStructure(_(1,1));
    if(Structure.isConformingTo(AStructure) != 1)
    {CAMarrayBase::nonConformingMessage(Structure,AStructure);}

    CAMbinaryEngine::doubleAminusEqualAlpha(Structure, *(DataP), value);
}

CAMarrayBase  CAMarrayBase::operator*(double value) const
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAtimesEqualAlpha(S.Structure, *S.DataP, value);
    S.setTemporaryFlag();
    return S;
}

CAMarrayBase  operator *(double value, const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(A);
    CAMbinaryEngine::doubleAtimesEqualAlpha(S.Structure, *S.DataP, value);
    S.setTemporaryFlag();
    return S;
}

CAMarrayBase  CAMarrayBase::operator /(double value) const
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAdivideEqualAlpha(S.Structure, *S.DataP, value);
    S.setTemporaryFlag();
    return S;
}

CAMarrayBase  operator /(double value, const CAMarrayBase& A)
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(A);
    CAMbinaryEngine::doubleAdivideEqualAlpha(S.Structure, *S.DataP, value);
    S.setTemporaryFlag();
    return S;
}

void  CAMarrayBase::operator *=(double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMbinaryEngine::doubleAtimesEqualAlpha(Structure, *DataP, value);
}

void  CAMarrayBase::operator /=(double value)
{
    CAMbinaryEngine::doubleAdivideEqualAlpha(Structure, *DataP, value);
}



void  CAMarrayBase::setToValue(double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMbinaryEngine::doubleAequalToAlpha(Structure, *DataP, value);
}

CAMarrayBase  CAMarrayBase::plusValue(double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAplusEqualAlpha(S.Structure, *S.DataP, value);
     S.setTemporaryFlag();
    return S;
}

CAMarrayBase  CAMarrayBase::minusValue(double value)
{
//
//  NEED CONVERSION CHECK
//
    CAMarrayBase S(*this);
    CAMbinaryEngine::doubleAminusEqualAlpha(S.Structure, *S.DataP, value);
    S.setTemporaryFlag();
    return S;
}

//
//*****************************************************************
//            Temporary and Reference Utilities
//*****************************************************************
//
void  CAMarrayBase::exchangeContentsWith(CAMarrayBase& B)
{
//
//  This routine exchanges the contents of *this with B.
//  The routine is typically used with *this input as
//  a null object and B a temporary object (i.e. the result
//  of an operator or function which returns B with the
//  temporary flag set). In such a case, the contents of
//  B are captured by *this --- resulting in a transfer of the
//  results without invoking a data copy.
//
//
    CAMstructureBase S_temp(Structure);
    CAMdataHandler*  DataP_temp        = DataP;
    int  typeValue_temp                = typeValue;
    int  referenceFlag_temp            = referenceFlag;
    long  arrayBaseReferenceCount_temp = arrayBaseReferenceCount;

    Structure.initialize(B.Structure);
    DataP                   = B.DataP;
    typeValue               = B.typeValue;
    referenceFlag           = B.referenceFlag;
    arrayBaseReferenceCount = B.arrayBaseReferenceCount;
//
    B.Structure.initialize(S_temp);
    B.DataP                   = DataP_temp;
    B.typeValue               = typeValue_temp;
    B.referenceFlag           = referenceFlag_temp;
    B.arrayBaseReferenceCount = arrayBaseReferenceCount_temp;
}

void  CAMarrayBase::initializeReferenceDuplicate(const CAMarrayBase& B)
{
//
//  This routine initializes *this with the contents of B and
//  and sets the *this reference flag.  This results in a
//  class instance whose data is that associated with B.
//
//  Typically this routine is used in a container class which
//  contains arrays --- and one wants to have sub-array access
//  for the contained class which is based upon the sub-array
//  access of the array class. 
//
    if(DataP != 0)
    {
    DataP->decrementReferenceCount();
     if(DataP->getReferenceCount() == 0) delete DataP;
    }

    Structure.initialize(B.Structure);
    DataP                   = B.DataP;
    DataP->incrementReferenceCount();  //One Reference for local copy
    DataP->incrementReferenceCount();  //One Reference for compiler copy
    typeValue               = B.typeValue;
    referenceFlag           = 1;
    arrayBaseReferenceCount = 0;
}
//
//*****************************************************************
//                    CONVERSIONS
//*****************************************************************
//
CAMmatrixBase  CAMarrayBase::asMatrix() const
{
    CAMmatrixBase S;
    if(DataP == 0) return S;

    CAMrange R1; CAMrange R2;
    long i;
    long dimension = 0;

    for(i = 1; i <= getDimension(); i++)
    {
    if(Structure[i].getIndexCount() != 1)
    {
    dimension++;
    if(dimension == 1)
    R1.initialize(Structure[i].getIndexBase(),Structure[i].getIndexBound());
    if(dimension == 2)
    R2.initialize(Structure[i].getIndexBase(),Structure[i].getIndexBound());
    }
    }
    switch (dimension)
    {
    case 0 :
    R1.initialize(Structure[1].getIndexBase(),Structure[1].getIndexBound());
    R2.initialize(Structure[1].getIndexBaseBase(),Structure[1].getIndexBaseBase());
    break;

    case 1 :
    R2.initialize(Structure[1].getIndexBaseBase(),Structure[1].getIndexBaseBase());
    break;

    case 2 :
    break;
    default :
    CAMarrayBase::objectConversionError(Structure);
    }
//
//
    S.initialize(CAMType::typeDouble, R1,R2);
    CAMbinaryEngine::doubleAequalToB(S.Structure, *(S.DataP), Structure, *DataP);
//
//
    S.setTemporaryFlag();
    return S;
}
CAMvectorBase  CAMarrayBase::asVector() const
{
    CAMvectorBase S;
    if(DataP == 0) return S;

    CAMrange R1;
    long i;
    long dimension = 0;

    for(i = 1; i <= getDimension(); i++)
    {
    if(Structure[i].getIndexCount() != 1)
    {
    dimension++;
    if(dimension == 1)
    R1.initialize(Structure[i].getIndexBase(),Structure[i].getIndexBound());
    }
    }
    switch (dimension)
    {
    case 0 :
    R1.initialize(Structure[1].getIndexBase(),Structure[1].getIndexBound());
    break;

    case 1 :
    break;
    default :
    CAMarrayBase::objectConversionError(Structure);

    }
//
//
    S.initialize(CAMType::typeDouble, R1);
    CAMbinaryEngine::doubleAequalToB(S.Structure, *(S.DataP), Structure, *DataP);
//
//
    S.setTemporaryFlag();
    return S;
}
//
//*****************************************************************
//                    DIMENSION SELECTION
//*****************************************************************
//
CAMstructureBase&  CAMarrayBase::operator[](long i)
{
    if((i < 0)||(i > Structure.dataDimension))
    {CAMstructureBase::illegalDimension(i, Structure.dataDimension);}

    Structure.exchangeReferenceIndex(i);

    return  Structure;
}

const CAMstructureBase&  CAMarrayBase::operator[](long i) const
{
    if((i < 0)||(i > Structure.dataDimension))
    {CAMstructureBase::illegalDimension(i, Structure.dataDimension);}

    Structure.exchangeReferenceIndex(i);

    return  Structure;
}
//
//********************************************************************************
//                    Array Helper Functions
//********************************************************************************
//
void*  CAMarrayBase::getDataPointer(long i1, long i2, long i3, long i4,
long i5, long i6, long i7) const
{
    const long* beginPtr  = Structure.indexBeginBase.getDataPointer();
    const long* endPtr    = Structure.indexEndBase.getDataPointer();
    long  i[7];

    i[0] = i1; i[1] = i2; i[2] = i3; i[3] = i4;
    i[4] = i5; i[5] = i6; i[6] = i7;
//
//  NEED CONVERSION CHECK
//
    double* dataPtr =  (double*)(DataP->dataPointer);

    long offset;
    offset  = (i[Structure.dataDimension - 1]
            - *(beginPtr + (Structure.dataDimension - 1)));
    for(long j = Structure.dataDimension - 2; j >= 0; j--)
    {
    offset = (i[j] - *(beginPtr + j)) +
    offset*((*(endPtr + j) - *(beginPtr + j)) + 1);
    }

    return (void*)(dataPtr + offset);
}
//
//********************************************************************************
//                    Reference Counting Functions
//********************************************************************************
//

void  CAMarrayBase::incrementReferenceCount()
{
    if(arrayBaseReferenceCount == 0) CAMarrayBase::referenceCountError();
    arrayBaseReferenceCount++;
}

void  CAMarrayBase::referenceCountError()
{
std::cerr << " Cannot Use Reference Counting on Objects New\'d by the Compiler " << std::endl;
CAMmvaExit();
}
//
//*****************************************************************
//                    MEMBER_FUNCTIONS
//*****************************************************************
//

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1)
{
    if(S.dataDimension != 1)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << " --- Dimension Used = "<< 1 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2)
{
    if(S.dataDimension != 2)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 2 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2, long i3)
{
    if(S.dataDimension != 3)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 3 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
     if((i3 < *(indexBeginP +2))||(i3 > *(indexEndP+2)))
     {
     CAMarrayBase::indexErrorMessage(3, *(indexBeginP+2), *(indexEndP+2), i3);
     }
}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2, long i3, long i4)
{
    if(S.dataDimension != 4)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 4 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
     if((i3 < *(indexBeginP +2))||(i3 > *(indexEndP+2)))
     {
     CAMarrayBase::indexErrorMessage(3, *(indexBeginP+2), *(indexEndP+2), i3);
     }
     if((i4 < *(indexBeginP +3))||(i4 > *(indexEndP+3)))
     {
     CAMarrayBase::indexErrorMessage(4, *(indexBeginP+3), *(indexEndP+3), i4);
     }

}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2, long i3, long i4, long i5)
{
    if(S.dataDimension != 5)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 5 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
     if((i3 < *(indexBeginP +2))||(i3 > *(indexEndP+2)))
     {
     CAMarrayBase::indexErrorMessage(3, *(indexBeginP+2), *(indexEndP+2), i3);
     }
     if((i4 < *(indexBeginP +3))||(i4 > *(indexEndP+3)))
     {
     CAMarrayBase::indexErrorMessage(4, *(indexBeginP+3), *(indexEndP+3), i4);
     }
     if((i5 < *(indexBeginP +4))||(i5 > *(indexEndP+4)))
     {
     CAMarrayBase::indexErrorMessage(5, *(indexBeginP+4), *(indexEndP+4), i5);
     }
}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2, long i3, long i4, long i5, long i6)
{
    if(S.dataDimension != 6)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 6 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
     if((i3 < *(indexBeginP +2))||(i3 > *(indexEndP+2)))
     {
     CAMarrayBase::indexErrorMessage(3, *(indexBeginP+2), *(indexEndP+2), i3);
     }
     if((i4 < *(indexBeginP +3))||(i4 > *(indexEndP+3)))
     {
     CAMarrayBase::indexErrorMessage(4, *(indexBeginP+3), *(indexEndP+3), i4);
     }
     if((i5 < *(indexBeginP +4))||(i5 > *(indexEndP+4)))
     {
     CAMarrayBase::indexErrorMessage(5, *(indexBeginP+4), *(indexEndP+4), i5);
     }
     if((i6 < *(indexBeginP +5))||(i6 > *(indexEndP+5)))
     {
     CAMarrayBase::indexErrorMessage(6, *(indexBeginP+5), *(indexEndP+5), i6);
     }
}

void  CAMarrayBase::indexCheck(const CAMstructureBase &S, long i1, long i2, long i3, long i4, long i5, long i6, long i7)
{
    if(S.dataDimension != 7)
    {
     std::cerr << " Error : Incorrect Array Index Dimension " << std::endl;
     std::cerr << " Object Dimension = " << S.dataDimension
          << "--- Dimension Used = "<< 7 << std::endl;
     CAMmvaExit();
     }
     const long* indexBeginP = S.indexBegin.getDataPointer();
     const long* indexEndP = S.indexEnd.getDataPointer();
     if((i1 < *indexBeginP)||(i1 > *indexEndP))
     {
     CAMarrayBase::indexErrorMessage(1, *(indexBeginP), *(indexEndP), i1);
     }
     if((i2 < *(indexBeginP +1))||(i2 > *(indexEndP+1)))
     {
     CAMarrayBase::indexErrorMessage(2, *(indexBeginP+1), *(indexEndP+1), i2);
     }
     if((i3 < *(indexBeginP +2))||(i3 > *(indexEndP+2)))
     {
     CAMarrayBase::indexErrorMessage(3, *(indexBeginP+2), *(indexEndP+2), i3);
     }
     if((i4 < *(indexBeginP +3))||(i4 > *(indexEndP+3)))
     {
     CAMarrayBase::indexErrorMessage(4, *(indexBeginP+3), *(indexEndP+3), i4);
     }
     if((i5 < *(indexBeginP +4))||(i5 > *(indexEndP+4)))
     {
     CAMarrayBase::indexErrorMessage(5, *(indexBeginP+4), *(indexEndP+4), i5);
     }
     if((i6 < *(indexBeginP +5))||(i6 > *(indexEndP+5)))
     {
     CAMarrayBase::indexErrorMessage(6, *(indexBeginP+5), *(indexEndP+5), i6);
     }
     if((i7 < *(indexBeginP +6))||(i7 > *(indexEndP+6)))
     {
     CAMarrayBase::indexErrorMessage(7, *(indexBeginP+6), *(indexEndP+6), i7);
     }
}
void  CAMarrayBase::indexErrorMessage(long indexDimension, long base, long bound, long index)
{
     std::cerr << " Error :  Index " << indexDimension << " Out of Range " << std::endl;

     std::cerr << " Current Index Range  :(" << base << ", " << bound << ")"  << std::endl;
     std::cerr << " Index Used          : " << index << std::endl;
     CAMmvaExit();
}
void CAMarrayBase::nonConformingMessage(const CAMstructureBase& A,  const CAMstructureBase& B)
{
    long i;
    std::cerr << std::endl;
    std::cerr << "Error :  Object Dimensions not Compatable with Operation " << std::endl << std::endl;
    std::cerr << "Left  Operand Dimensions :   ";
    std::cerr << A[1].getIndexCount();
    for(i = 2; i <= A.dataDimension; i++)
    std::cerr <<  " x " << A[i].getIndexCount() ;
    std::cerr << std::endl << std::endl;
    std::cerr << "Right Operand Dimensions :   ";
    std::cerr << B[1].getIndexCount();
    for(i = 2; i <= B.dataDimension; i++)
    std::cerr <<  " x " << B[i].getIndexCount();
    std::cerr << std::endl << std::endl;
    CAMmvaExit();
}
void CAMarrayBase::doubleConversionError(const CAMstructureBase& A)
{
    long i;
    std::cerr << std::endl;
    std::cerr << "Dimensions not Compatable with Conversion to Double " << std::endl << std::endl;
    std::cerr << "Operand Dimensions :   ";
    std::cerr << A[1].getIndexCount();
    for(i = 2; i <= A.dataDimension; i++)
    std::cerr <<  " x " << A[i].getIndexCount() ;
    std::cerr << std::endl << std::endl;
    CAMmvaExit();
}

void CAMarrayBase::objectConversionError(const CAMstructureBase& A)
{
    long i;
    std::cerr << std::endl;
    std::cerr << " Dimensions not Compatable with Conversion Operator " << std::endl << std::endl;
    std::cerr << "Operand Dimensions :   ";
    std::cerr << A[1].getIndexCount();
    for(i = 2; i <= A.dataDimension; i++)
    std::cerr <<  " x " << A[i].getIndexCount() ;
    std::cerr << std::endl << std::endl;
    CAMmvaExit();
}
void CAMarrayBase::nullOperandError()
{
    std::cerr << std::endl;
    std::cerr << " Error : Null Object Used As Operand  " << std::endl;
    CAMmvaExit();
}
void CAMarrayBase::nullOperandError(char* Operation)
{
    std::cerr << std::endl;
    std::cerr << " Error : Null Object Used As Operand With "<< Operation << std::endl;
    CAMmvaExit();
}
void CAMarrayBase::inputSizeError()
{
    std::cerr << std::endl;
    std::cerr << " Input error : Insufficient # of data elements in input stream " << std::endl;
    CAMmvaExit();
}
//
//*****************************************************************
//                     CPP File End
//*****************************************************************
//












  
