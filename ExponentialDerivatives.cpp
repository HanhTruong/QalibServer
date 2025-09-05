#include "stdafx.h"
#include <math.h>
#include <MDstd.h>
#include "derivatives.h"
#include "globals.h"

/********************************************************************
void ExponentialDerivatives( dVector dYdAi, dVector X, ui size, unsigned iPar ) 
	Calculate the derivatives of the regression.
	If fit parameter index, iPar, is unknown.
	A default derivative function will be called.
---------------------------------------------------------------------
 INPUT
	dYdAi - Derivative results vector.
	X	  - Vector containing values to be deferentiated.
	size  - Size of derivative arrays.
	iPar  - Index of current fit parameter to be deferentiated.
---------------------------------------------------------------------
 OUTPUT
	dYdAi - Will contain the derivatives of the current data set.
---------------------------------------------------------------------
 LOCAL
	i	  - index to traverse output vector.
********************************************************************/
void ExponentialDerivatives( dVector dYdAi, dVector X, ui size, unsigned iPar ) 
{ 
	ui i;
	
	switch( iPar ) 
	{ 
	case 0:		// Slope
		for (i=0; i<size; i++)
		{
			//	Intercept FitParameters[1]
			dYdAi[i] = ddslope( X[i], FitParameters[Intercept]);
		}
	break;
	case 1:		// Intercept
		for (i=0; i<size; i++)
		{
			//		Slope FitParameters[0]
			//	Intercept FitParameters[1]
			dYdAi[i] = ddintercept( X[i], FitParameters[Intercept], FitParameters[Slope]);
		}
	break;
	default:	// Unknown
		VD_nonlinfit_autoDeriv( dYdAi, X, size, iPar); 
	} 
}

