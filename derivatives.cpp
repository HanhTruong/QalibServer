/*******************************************************************

FILENAME:	derivatives.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Derivatives functions called with DerivModel in OptiVec's 
	VD_nonlinfitwW functions.

chisquared = VD_nonlinfitwW(FitParameters, Covar, FitParameterStatus, npars, XExp, 
							YExp, Weights, sizex, Rate, DerivModel );

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	derivatives.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"

#include "derivatives.h"

/******************************************************
double ddslope(double AV, 
			   double intercept, 
			   double slope, 
			   double bottom, 
			   double top)

	Derivative of each data point in relation to slope.

-------------------------------------------------------
INPUT
	AV		  - Current data point.
	intercept - Intercept value.
	slope	  - Slope value.
	bottom	  - Bottom value. Low limit.
	top		  - Top value. High limit.

-------------------------------------------------------
OUTPUT
	derivative of current data point.

-------------------------------------------------------
LOCAL
	retval - Derivative.
	x - Intermediary value.
******************************************************/
double ddslope(double AV, 
			   double intercept, 
			   double slope, 
			   double bottom, 
			   double top)
{
	double retval;
	double x;
  
	x = exp(-(AV - intercept) / slope);

	retval = (AV - intercept) * x * ((bottom - top) / (pow((x + 1),2) * pow(slope,2)));

	return retval;
}

/**********************************************************
double ddintercept(double AV, double intercept, 
				   double slope, double bottom, double top)
	Derivative of each data point in relation to intercept.

-----------------------------------------------------------
INPUT

	AV		  - Current data point.
	intercept - Intercept value.
	slope	  - Slope value.
	bottom	  - Bottom value. Low limit.
	top		  - Top value. High limit.

-----------------------------------------------------------
OUTPUT
	Derivative of current data point.

-----------------------------------------------------------
LOCAL
	retval - Derivative.
	x - Intermediary value.
**********************************************************/
double ddintercept(double AV, 
				   double intercept, 
				   double slope, 
				   double bottom, 
				   double top)
{
	double retval;
	double x;

	x = exp(-(AV - intercept) / slope);

	retval = x * ((bottom - top) / (pow((x + 1),2) * slope));

	return retval;
}

/*********************************************************
double ddbottom(double AV, double intercept, double slope)
	Derivative of each data point in relation to bottom.

----------------------------------------------------------
INPUT
	AV		  - Current data point.
	intercept - Intercept value.
	slope	  - Slope value.

----------------------------------------------------------
OUTPUT
	Derivative of data point.

----------------------------------------------------------
LOCAL
	retval - Derivative.
	x - Intermediary value.

----------------------------------------------------------
RETURN
	Derivative.
*********************************************************/
double ddbottom(double AV, 
				double intercept, 
				double slope)
{
	double retval;
	double x;
  
	x = exp(-(AV - intercept) / slope);

	retval = x / (x + 1);

	return retval;
}

/******************************************************
double ddtop(double AV, double intercept, double slope)

	Derivative of each data point in relation to top.

-------------------------------------------------------
INPUT
	AV		  - Current data point.
	intercept - Intercept value.
	slope	  - Slope value.

-------------------------------------------------------
OUTPUT
	Derivative of data point.

-------------------------------------------------------
LOCAL
	retval - Derivative.
	x - Intermediary value.

-------------------------------------------------------
RETURN
	Derivative.

******************************************************/
double ddtop(double AV, 
			 double intercept, 
			 double slope)
{
	double retval;
	double x;
  
	x = exp(-(AV - intercept) / slope);

	retval = 1.0 / (x + 1);

	return retval;
}

/******************************************************
double ddslope(double AV, 
			   double intercept)

	Derivative of each data point in relation to slope.

-------------------------------------------------------
INPUT
	AV		  - Current data point.
	intercept - Intercept value.

-------------------------------------------------------
OUTPUT
	derivative of current data point.

-------------------------------------------------------
LOCAL
	none.
******************************************************/
double ddslope(double AV, 
			   double intercept)
{
	return (exp(intercept * AV));
}

/**********************************************************
double ddintercept(double AV,double intercept,double slope)
	Derivative of each data point in relation to intercept.

-----------------------------------------------------------
INPUT
	AV		  - Current data point.
	intercept - Intercept value.
	slope	  - Slope value.

-----------------------------------------------------------
OUTPUT
	Derivative of current data point.

-----------------------------------------------------------
LOCAL
	None.

**********************************************************/
double ddintercept(double AV,
				   double intercept,
				   double slope)
{
	return (AV * exp(slope * intercept));
}
