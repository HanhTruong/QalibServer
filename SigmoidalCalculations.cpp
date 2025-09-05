#include "stdafx.h"
#include <math.h>
#include <MDstd.h>
#include "globals.h"
#include "SigmoidalCalculations.h"
#include "Loging.h"
#include "Support.h"
#include "Utils.h"
#include "CalibrationCalculations.h"
#include "FitParameters.h"
#include "AssignedValues.h"
#include "Constants.h"

/****************************************************************
	double CorrelationCoefficient(	dVector X,					
									dVector Y,					
									ui sizeYX)					

 Calculates the correlation coefficient using the formula:	
 R^2 = 1 - ((sum (Y[i] - yHat[i])^2)/sum(Y[i] - mean(Y[i]))^2)

-----------------------------------------------------------------
 INPUT
	Y		- Y data values vector.
	X		- X data values vector.
	sizeYX	- Size of input data arrays.

-----------------------------------------------------------------
 OUTPUT
	None.

-----------------------------------------------------------------
 LOCAL
	x			 - Intermediate results.
	ccoe - Correlation coefficient value, to return to caller.
	meany		 - Average of input data y values.
	yHat		 - Intermediate calculation vector.
	Numerator	 - Intermediate value.
	Denominator  - Intermediate value.
	IValues		 - Vector to hold intermediate values.

-----------------------------------------------------------------
 RETURN
	ccoe - Calculated correlation coefficient.

****************************************************************/
double SigmoidalCorrelationCoefficient(dVector X, 
									   dVector Y,
									   ui sizeYX)
{
	double ccoe;
	double meany;

	dVector yHat;
	double  Numerator;		// Value for last calculation's numerator.
	double  Denominator;	// Value for last calculation's denominator.
	dVector IValues;		// Vector to hold intermediate values.

    IValues = VD_vector( sizeYX );
    yHat	= VD_vector( sizeYX );

	// Average of y values. Experimental data.
	meany = VD_sum( Y, sizeYX );
	meany = meany / sizeYX;

	// Calculate yHat
	Rate( yHat, X, sizeYX );

	// IValues[i] = Y[i] - yHat[i] 
	VD_subV( IValues, Y, yHat, sizeYX );

	// normal versions: Y[i] = X[i]^2 
	// IValues[i] = (IValues[i])^2
	int retsquare = VD_square( IValues, IValues, sizeYX );

	// float VF_sum( fVector X, ui size );
	Numerator = VD_sum( IValues, sizeYX );

	// (Re-) initialize a vector with zero.
	VD_equ0( IValues, sizeYX );

	// Subtract average from each experimental data point.
	VD_subC( IValues, Y, sizeYX, meany);

	// normal versions: Y[i] = X[i]^2 
	int sqcode = VD_square( IValues, IValues, sizeYX );

	// Sum up all the elements of a vector.
	Denominator = VD_sum( IValues, sizeYX );

	V_free( yHat );
	V_free( IValues );

	ccoe = 1 - ( Numerator / Denominator );

	return ccoe;
}

/*************************************************************
 void Rate( dVector Y, dVector X, ui size )
 Rate function has to fill a whole result vector,
 using your first guess of FitPars.
 The coefficients are explicitly used in the model function.
 You must initialize FitPars with something, even if you have
 no idea about the result.
 FitPars must be global, so that the model function can
 access the parameters.

--------------------------------------------------------------
 INPUT
	Y	 - Rate values, calculated, results vector.
	X	 - Values used to calculate rate.
	size - Size of input arrays.

--------------------------------------------------------------
 OUTPUT
	Y	 - Contains the calculated rates.

--------------------------------------------------------------
 LOCAL
	x	 - Intermediate results.
*************************************************************/
void Rate( dVector Y, dVector X, ui size )
{
    double x;

    for( ui i=0; i<size; i++ )
    {
		if (fabs(FitParameters[Slope]) > SMALL_VALUE)
		{
			x = exp((-X[i] + FitParameters[Intercept]) / FitParameters[Slope]);
			Y[i] = (x * FitParameters[Bottom] + FitParameters[Top]) / (x + 1);
		}
	}
}

/*************************************************************
void SigmoidalCalculateAssignedValues(
				CArray<double,double> *calibratorData,
				CArray<double,double> *assignedValues,
				CArray<double,double> *calcAssignedValues)

 	Calculate assigned values for current calibrators data.

--------------------------------------------------------------
 INPUT
	calibratorData	- Vector to contain calibrator data.
	assignedValues - Assigned values list.
	calcAssignedValues	- Return calculated assigned values.

--------------------------------------------------------------
 OUTPUT
	None.

--------------------------------------------------------------
 LOCAL
	x - Intermediate calculations.
	i - Vector traversal index.
	y - Can't take log of negative number.
	calibDatum - Current value to use in calculation.
	lLCID - Language code, for conversion.
	hr - Test/return code.
	l_calcAssignedValues - Store calculated assigned values.

--------------------------------------------------------------
 RETURN
	None.
*************************************************************/
void SigmoidalCalculateAssignedValues(
				CArray<double,double> *fitParameters,
				CArray<double,double> *calibratorData,
				CArray<double,double> *assignedValues,
				CArray<double,double> *calcAssignedValues)
{
    double	x,y,z;
	int		s;
	CArray<double,double> l_calibratorData;
	CArray<double,double> l_assignedValues;
	CArray<double,double> l_calcAssignedValues;

	calcAssignedValues->RemoveAll();
	
	s = calibratorData->GetSize();

	// < ----- CALCULATE ASSIGNED VALUES ----->
	for (int k=0; k<s; k++)
	{
		y = -((calibratorData->GetAt(k) - fitParameters->GetAt(Top)) / (calibratorData->GetAt(k) - fitParameters->GetAt(Bottom)));

		if ( y > 0)
		{
			x = log(y);
			z = -fitParameters->GetAt(Slope) * x + fitParameters->GetAt(Intercept);

			l_calibratorData.Add(calibratorData->GetAt(k));
			l_assignedValues.Add(assignedValues->GetAt(k));
			l_calcAssignedValues.Add(z);
		}
	}
	// </ ----- CALCULATE ASSIGNED VALUES ----->

	calibratorData->RemoveAll();
	assignedValues->RemoveAll();
	calcAssignedValues->RemoveAll();

	calibratorData->Copy(l_calibratorData);
	assignedValues->Copy(l_assignedValues);
	calcAssignedValues->Copy(l_calcAssignedValues);
}

/****************************************************************
HRESULT SigmoidalRatesFromAssignedValues
	(LONG calibrationID, VARIANT *rates)

	Calculate rates from assigned values.
	Get assigned values.
	Set up vector spaces.
	Calculate rates.
	Put rates in return array.
-----------------------------------------------------------------
 INPUT														
	calibrationID - ID of calibration to calculate rates.
	algorithm - Rates calculations depend on algorithm.
	*Rates - Structure to return rates in.

-----------------------------------------------------------------
 OUTPUT														
	Assigned value rates for curent samples.
	
-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	bstr			- BSTR to move the strings in and out.
	numberOfSamples - Number of samples in current list.
	i				- Variant array's traversal index.
	X	  - Allocate memory for sample assigned values.
	Y	  - Allocate memory for rates of assigned values.
	assignedValues - Current assigned values.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT SigmoidalRatesFromAssignedValues(
			LONG calibrationID, 
			VARIANT *rates, 
			VARIANT *error_msg)
{
	HRESULT		hr;
	BSTR		bstr = NULL;
	long		numberOfSamples;
	long		i=0;
	char		buf[128];
	CArray<double,double> assignedValues;

	CArray<double,double> l_rates;
	CStringArray		  a_sampleIdList;
	CString				  tmp;

	//*** define the safearray ***
    SAFEARRAY FAR* psa_Rates = NULL;	// assigned rates.

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function","SigmoidalRatesFromAssignedValues");
		return hr;
	}

	hr = RemoveCONTROLData(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"RemoveCONTROLData\" function","SigmoidalRatesFromAssignedValues");
		return hr;
	}

	hr = GetSampleAssignedValues(calibrationID, &a_sampleIdList, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetSampleAssignedValues\" function","SigmoidalRatesFromAssignedValues");
		return hr;
	}

	numberOfSamples = assignedValues.GetSize();

	dVector X, Y;
	// Allocate memory for sample assigned values.
	X = VD_vector( numberOfSamples );
	// Allocate memory for rates of assigned values.
	Y = VD_vector( numberOfSamples );

	i=0;
	// Put assigned values in vector X to send to Rate function.
	for (ui k=0; k<(ui)assignedValues.GetSize(); k++)
	{
		X[k] = assignedValues.GetAt(k);
	}

	// Calculate rates for assigned values.
	Rate(Y, X, numberOfSamples);

	// Set size of array to pass to client.
	sabound[0].cElements = 2 * numberOfSamples;
	sabound[0].lLbound = 0;

	//*** create the safearray to return ***
	psa_Rates = SafeArrayCreate(VT_BSTR, 1, sabound);

	i=0;
	for (k=0; k<(ui)numberOfSamples; k++)
	{
		bstr = a_sampleIdList.GetAt(k).AllocSysString();
		hr = SafeArrayPutElement(psa_Rates, &i, bstr);

		i++;

		sprintf(buf, "%.15f", Y[k]);
		tmp = buf;
		bstr = tmp.AllocSysString();
		hr = SafeArrayPutElement(psa_Rates, &i, bstr);

		i++;
	}

	// Set the return array's type, array of VT_BSTR.
	// VT_R8, 8-byte real. Type of data to return in plotData.
	//*** set up the safearray to export it as an array of reals.  
	V_VT(rates) = VT_ARRAY | VT_BSTR;

	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
	V_ARRAY(rates) = psa_Rates;

	return S_OK;
}

HRESULT SigmoidalCalculations(LONG calibrationID,
							  CString algorithm,
							  double *chisquared,
							  VARIANT *newFitParameters,
							  VARIANT *error_msg)
{
	HRESULT	hr;
	char	buf[512];

	// Get upper and lower fit parameter limits from the database.
	hr = GetFitParsLimits(calibrationID,
						  LowerLimits,
						  UpperLimits,
						  algorithm,
						  numberOfFitParameters,
						  "",
						  error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SigmoidalCalculations\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetFitParsLimits\" function",buf);
		return hr;
	}

	// Set fit parameters status.
	// ParStatus, which contains the necessary information about which parameters 
	// are to be held fixed at their input values (ParStatus[i] = 0) 
	// and which are free (ParStatus[i] = 1).
	// Default status. All 1's.
	FitParameterStatus[0] = FitParameterStatus[1] = FitParameterStatus[2] = FitParameterStatus[3] = 1;

	// Is this calculation for updated fit parameters?
	if (newFitParameters->vt == VT_NULL || newFitParameters->vt == VT_EMPTY)
	{
		// Initialize fit parameters.
		InitializeFitParameters(LowerLimits, UpperLimits, FitParameters, YExp, sizex);
	}
	else
	{
		// Parse new fit parameter array.
		hr = AssignFitParameters(calibrationID, newFitParameters, error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"SigmoidalCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,"Return from function \"AssignFitParameters\"",buf);
			}
			return hr;
		}
	}

	// Process nonlinear fit of data.
	hr = QalibNonlinearFitWithWeight(chisquared);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SigmoidalCalculations\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"QalibNonlinearFitWithWeight\"",	buf);
		return hr;
	}

	return S_OK;
}
