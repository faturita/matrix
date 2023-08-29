#include "matbse.h"
//
//******************************************************************************
//                    MATGPH.CPP
//******************************************************************************
//
//        BINDINGS TO THE UC GRAHPICS CLASSES
//
//********************************************************************************
//
//            Chris Anderson  (C) UCLA
//
//            Tue Sep 26 14:40:00 1995
//
//********************************************************************************
//

void  CAMmatrixBase::plot() const
{
//
//  Need Conversion Check
//
	long dimension;
	long M,N;

	const CAMmatrixBase* InputPtr;
	CAMmatrixBase A;

	double* AdataPtr; double* BdataPtr;

	long j; int autoFlag;
	double x_min, x_max, y_min, y_max;

	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	InputPtr = &A;
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	InputPtr =  this;
	}

	switch (dimension)
	{
	case 1  :
	CAMgraphics::plot(AdataPtr,M);

	break;

	case 2  :
	autoFlag = CAMgraphics::getAutoScaleFlag();
	x_min = 1;
	x_max = M;
	y_min = InputPtr->min();
	y_max = InputPtr->max();
	CAMgraphics::axis(x_min,x_max,y_min,y_max);
	for(j = 1; j <= N; j++)
	{
	BdataPtr = AdataPtr + (j-1)*M;
	CAMgraphics::plot(BdataPtr,M);
	}
	CAMgraphics::axis(autoFlag);

	break;
	default :
	CAMmatrixBase::plotDimensionError(A.Structure);
	}

}

void  CAMmatrixBase::plot(int p_arg) const
{
	char point_type_save = CAMgraphics::getPointType();
	int  plot_style_save = CAMgraphics::getPlotStyle();
    if(p_arg < 32) 
	{ CAMgraphics::setPlotStyle(p_arg);}
    else
	{ CAMgraphics::setPointType(char(p_arg));
	  CAMgraphics::setPlotStyle(CAMgraphics::POINTS);
	}

	this->plot();

	CAMgraphics::setPointType(point_type_save);
	CAMgraphics::setPlotStyle(plot_style_save);
}
void  CAMmatrixBase::plot(int p_style, int p_arg) const
{
	char point_type_save = CAMgraphics::getPointType();
	int  plot_style_save = CAMgraphics::getPlotStyle();
	CAMgraphics::setPointType(char(p_arg));
	CAMgraphics::setPlotStyle(p_style);

	this->plot();

	CAMgraphics::setPointType(point_type_save);
	CAMgraphics::setPlotStyle(plot_style_save);
}
void  CAMmatrixBase::plot(const CAMmatrixBase& Ordinates) const
{
//
//  Need Conversion Check
//
	long dimension;
	long M,N;

	CAMmatrixBase A;
	const CAMmatrixBase* InputPtr;
	double* AdataPtr; double* BdataPtr;

	long j; int autoFlag;
	double x_min, x_max, y_min, y_max;

	CAMmatrixBase O;
    double* OdataPtr;
	long    Odimension;
	long    Ocount;
//
	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	InputPtr = &A;
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	InputPtr = this;
	}

	if(Ordinates.Structure.isSubset() == 1)
	{
	O.initializeMinDuplicate(Ordinates);
	Odimension  = O.getDimension();
	OdataPtr    = (double*)O.getDataPointer();
	Ocount      = O[1].getIndexCount();
	}
	else
	{
	Odimension  = Ordinates.getDimension();
	OdataPtr    = (double*)Ordinates.getDataPointer();
	Ocount      = Ordinates[1].getIndexCount();
	}

	if(Odimension != 1)
	{CAMmatrixBase::ordinateError(Ordinates.Structure);}

	if(M != Ocount)
	{CAMmatrixBase::ordinateError(Ordinates.Structure);}

	switch (dimension)
	{
	case 1  :
	CAMgraphics::plot(OdataPtr,AdataPtr,M);

	break;

	case 2  :

	autoFlag = CAMgraphics::getAutoScaleFlag();
	x_min = *OdataPtr;
	x_max = *(OdataPtr +(M-1));
	y_min = InputPtr->min();
	y_max = InputPtr->max();
	CAMgraphics::axis(x_min,x_max,y_min,y_max);
	for(j = 1; j <= N; j++)
	{
	BdataPtr = AdataPtr + (j-1)*M;
	CAMgraphics::plot(OdataPtr, BdataPtr,M);
	}
	CAMgraphics::axis(autoFlag);

	break;
	default :
	CAMmatrixBase::plotDimensionError(A.Structure);
	}

}

void  CAMmatrixBase::plot(const CAMmatrixBase& Ordinates, int p_arg) const
{
	char point_type_save = CAMgraphics::getPointType();
	int  plot_style_save = CAMgraphics::getPlotStyle();
    if(p_arg < 32) 
	{ CAMgraphics::setPlotStyle(p_arg);}
    else
	{ CAMgraphics::setPointType(char(p_arg));
	  CAMgraphics::setPlotStyle(CAMgraphics::POINTS);
	}

	this->plot(Ordinates);

	CAMgraphics::setPointType(point_type_save);
	CAMgraphics::setPlotStyle(plot_style_save);
}

void  CAMmatrixBase::plot(const CAMmatrixBase& Ordinates, int p_arg, int p_style) const
{
	char point_type_save = CAMgraphics::getPointType();
	int  plot_style_save = CAMgraphics::getPlotStyle();
	CAMgraphics::setPointType(char(p_arg));
	CAMgraphics::setPlotStyle(p_style);

	this->plot(Ordinates);

	CAMgraphics::setPointType(point_type_save);
	CAMgraphics::setPlotStyle(plot_style_save);
}

void  CAMmatrixBase::contour() const
{
//
//  Need Conversion Check
//
	CAMmatrixBase A;
	double* AdataPtr;

	long dimension;
	long M,N;
	int  Fflag;

	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	}

	if(dimension == 2)
	{
	Fflag = CAMgraphics::getFortranArrayFlag();
	CAMgraphics::fortranArrayFlagOn();
	CAMgraphics::contour(AdataPtr,M,N);
	if(Fflag == 0) CAMgraphics::fortranArrayFlagOff();
	}
	else
	{CAMmatrixBase::plotDimensionError(A.Structure);}

}

void  CAMmatrixBase::contour(int nContour)  const
{
	CAMgraphics::setContourLevel(nContour);
	this->contour();
}

void  CAMmatrixBase::contour(long nContour)  const
{
	CAMgraphics::setContourLevel(nContour);
	this->contour();
}
void  CAMmatrixBase::contour(double increment) const
{
	CAMgraphics::setContourLevel(increment);
	this->contour();
}

void  CAMmatrixBase::contour(double low_value, double high_value) const
{
	CAMgraphics::setContourLevel(low_value, high_value);
	this->contour();
}

void  CAMmatrixBase::contour(long nContour, double low_value, double high_value) const
{
	CAMgraphics::setContourLevel(nContour, low_value, high_value);
	this->contour();
}

void  CAMmatrixBase::contour(int nContour, double low_value, double high_value) const
{
	CAMgraphics::setContourLevel(nContour, low_value, high_value);
	this->contour();
}

void  CAMmatrixBase::contour(double increment, double low_value, double high_value) const
{
	CAMgraphics::setContourLevel(increment, low_value, high_value);
	this->contour();
}

void  CAMmatrixBase::contour(const CAMmatrixBase& contourValues) const
{
//
//  Need Conversion Check
//
	CAMmatrixBase A;
	double* AdataPtr;

	long dimension;
	long M,N;
	int  Fflag;

	CAMmatrixBase CV;
	double* CVdataPtr;
	long    CVdimension;
	long    CVcount;

	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	}

	if(contourValues.Structure.isSubset() == 1)
	{
	CV.initializeMinDuplicate(contourValues);
	CVdimension  = CV.getDimension();
	CVdataPtr    = (double*)CV.getDataPointer();
	CVcount      = CV[1].getIndexCount();
	}
	else
	{
	CVdimension  = contourValues.getDimension();
	CVdataPtr    = (double*)contourValues.getDataPointer();
	CVcount      = contourValues[1].getIndexCount();
	}


	if(CVdimension != 1)
	{CAMmatrixBase::ordinateError(contourValues.Structure);}

	if(dimension == 2)
	{
	Fflag = CAMgraphics::getFortranArrayFlag();
	CAMgraphics::fortranArrayFlagOn();
	CAMgraphics::contour(AdataPtr,M,N,CVdataPtr,CVcount);
	if(Fflag == 0) CAMgraphics::fortranArrayFlagOff();
	}
	else
	{CAMmatrixBase::plotDimensionError(A.Structure);}

}

void  CAMmatrixBase::surface() const
{
//
//  Need Conversion Check
//
	CAMmatrixBase A;
	double* AdataPtr;

	long dimension;
	long M,N;
	int  Fflag;


	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	}

	if(dimension == 2)
	{
	Fflag = CAMgraphics::getFortranArrayFlag();
	CAMgraphics::fortranArrayFlagOn();
	CAMgraphics::surface(AdataPtr,M,N);
	if(Fflag == 0) CAMgraphics::fortranArrayFlagOff();
	}
	else
	{CAMmatrixBase::plotDimensionError(A.Structure);}
}

void  CAMmatrixBase::surface(const CAMmatrixBase& x, const CAMmatrixBase& y) const
{
//
//  Need Conversion Check
//
	CAMmatrixBase A;
	double* AdataPtr;

	long dimension;
	long M,N;
	int  Fflag;

	CAMmatrixBase XC;
	double* XCdataPtr;
	long    XCdimension;
	long    XCcount;

	CAMmatrixBase YC;
	double* YCdataPtr;
	long    YCdimension;
	long    YCcount;

	if(Structure.isSubset() == 1)
	{
	A.initializeMinDuplicate(*this);
	dimension   = A.getDimension();
	AdataPtr    = (double*)A.getDataPointer();
	if(dimension >= 1) M = A[1].getIndexCount();
	if(dimension == 2) N = A[2].getIndexCount();
	}
	else
	{
	dimension = getDimension();
	AdataPtr  = (double*)getDataPointer();
	if(dimension >= 1) M = Structure[1].getIndexCount();
	if(dimension == 2) N = Structure[2].getIndexCount();
	}

	if(x.Structure.isSubset() == 1)
	{
	XC.initializeMinDuplicate(x);
	XCdimension  = x.getDimension();
	XCdataPtr    = (double*)x.getDataPointer();
	XCcount      = x[1].getIndexCount();
	}
	else
	{
	XCdimension  = x.getDimension();
	XCdataPtr    = (double*)x.getDataPointer();
	XCcount      = x[1].getIndexCount();
	}

	if(y.Structure.isSubset() == 1)
	{
	YC.initializeMinDuplicate(y);
	YCdimension  = y.getDimension();
	YCdataPtr    = (double*)y.getDataPointer();
	YCcount      = y[1].getIndexCount();
	}
	else
	{
	YCdimension  = y.getDimension();
	YCdataPtr    = (double*)y.getDataPointer();
	YCcount      = y[1].getIndexCount();
	}

	if(XCdimension != 1)
	{CAMmatrixBase::ordinateError(x.Structure);}

	if(YCdimension != 1)
	{CAMmatrixBase::ordinateError(y.Structure);}

	if(M != XCcount)
	{CAMmatrixBase::ordinateError(x.Structure);}

	if(N != YCcount)
	{CAMmatrixBase::ordinateError(y.Structure);}


	if(dimension == 2)
	{
	Fflag = CAMgraphics::getFortranArrayFlag();
	CAMgraphics::fortranArrayFlagOn();
	CAMgraphics::surface(AdataPtr,M, N, XCdataPtr, YCdataPtr);
	if(Fflag == 0) CAMgraphics::fortranArrayFlagOff();
	}
	else
	{CAMmatrixBase::plotDimensionError(A.Structure);}

}

void CAMmatrixBase::ordinateError(const CAMstructureBase & A) 
{
	long i;
    std::cerr << std::endl;
	std::cerr << "    Ordinates in Plot Command Incorrectly Specified " << std::endl << std::endl;
	std::cerr << " Error in Number of Ordinates or Ordinate Array Dimension  " << std::endl;
	std::cerr << " Ordinates Size : 	";
	std::cerr << A[1].getIndexCount();
	for(i = 2; i <= A.dataDimension; i++)
	std::cerr <<  " x " << A[i].getIndexCount() ;
	std::cerr << std::endl << std::endl;
	CAMmvaExit();
}
void CAMmatrixBase::plotDimensionError(const CAMstructureBase& A)
{
	long i;
    std::cerr << std::endl;
	std::cerr << "Error :  Object Dimensions not Compatable with Plot Operation " << std::endl << std::endl;
	std::cerr << "    Operand Dimensions :   ";
	std::cerr << A[1].getIndexCount();
	for(i = 2; i <= A.dataDimension; i++)
	std::cerr <<  " x " << A[i].getIndexCount() ;
	std::cerr << std::endl << std::endl;
	CAMmvaExit();
}
//
//********************************************************************************
//                     CPP File End 
//********************************************************************************
//
