/*******************************************************************

FILENAME:	derivatives.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declarations for derivative functions.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	derivative.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include <math.h>
#include <iostream.h>

double ddslope(double AV, double intercept, double slope, double bottom, double top);
double ddintercept(double AV, double intercept, double slope, double bottom, double top);
double ddbottom(double AV, double intercept, double slope);
double ddtop(double AV, double intercept, double slope);
double ddslope(double AV, double intercept);
double ddintercept(double AV, double intercept, double slope);
