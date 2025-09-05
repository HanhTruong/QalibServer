#include "stdafx.h"
#include <math.h>
#include <MDstd.h>
#include "Loging.h"
#include "globals.h"
#include "DbAccess.h"
#include "Support.h"
#include "ExponentialCalculations.h"
#include "CalibrationCalculations.h"
#include "QuadraticCalculations.h"
#include "LinearCalibration.h"
#include "ExponentialDerivatives.h"
#include "AssignedValues.h"
#include "FitParameters.h"

HRESULT ExponentialCalculations(LONG calibrationID,
								CString algorithm,
								double *chisquared,
								VARIANT *newFitParameters,
								VARIANT *error_msg)
{
	HRESULT hr=S_OK;
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
		sprintf(buf,"Function \"ExponentialCalculations\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetFitParsLimits\" function",buf);
		return hr;
	}

	// Set fit parameters status.
	// ParStatus, which contains the necessary information about which parameters 
	// are to be held fixed at their input values (ParStatus[i] = 0) 
	// and which are free (ParStatus[i] = 1).
	// Default status. All 1's.
	FitParameterStatus[Slope] = FitParameterStatus[Intercept] = 1;

	// Is this calculation for updated fit parameters?
	if (newFitParameters->vt == VT_NULL || newFitParameters->vt == VT_EMPTY)
	{
		// Initialize fit parameters.
		InitializeExponentialFitParameters(FitParameters);
	}
	else
	{
		// Parse new fit parameter array.
		hr = AssignFitParameters(calibrationID, newFitParameters, error_msg);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"SigmoidalCalculations\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"AssignFitParameters\"",buf);
			return hr;
		}
	}

	hr = ExponentialLinearFitWithWeight(chisquared);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ExponentialCalculations\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"ExponentialLinearFitWithWeight\"",	buf);
		return hr;
	}

	return S_OK;
}

/****************************************************************
HRESULT ExponentialLinearFitWithWeight(double *chisquared)

	Calculate nonlinear fit of current data set.
	 Initialize fit parameters.
	 Set fit parameters status.
	 Get upper and lower fit parameter limits from the database.
	 Set nonlinear fit options.
	 Calculate non linear fit of data.

-----------------------------------------------------------------
 INPUT
	chisquared - Best of fit method.

-----------------------------------------------------------------
 OUTPUT
	Result of nonlinear fit function, include:
	*chisquared - Return from nonlinear function.
	FitParameters - Parameters calculated by nonlinear function.
	Covar - Covariance results, from nonlinear function.

-----------------------------------------------------------------
 LOCAL

-----------------------------------------------------------------
 GLOBALS
	FitParameters - Parameters to be calculated by nonlinear function.
	Covar - Covariance matrix.
	FitParameterStatus - Parameter status.
	npars - Number of parameters to calculate.
	Weights - Weights vector.
	sizex - Total number of data points.
	Rate - Function called by nonlinear function.
	DerivModel - Model used by nonlinear function.
	numberofsamples - Number of samples in current calibartion.
	Weights - Vector to contain calculated weights.
	XExp - Vector to contain assigned values.
	PoolSizes - Vector to contain lengths of each sample.
	UpperLimits - Upper limits for FitParameters.
	LowerLimits - Lower limits for FitParameters.
	Opt - Options structure for nonlinear function.

----------------------------------------------------------------
 FUNCTIONS
	InitializeFitParameters
	GetFitParsLimits
	VD_getNonlinfitOptions
	VD_nonlinfitwW

----------------------------------------------------------------
 RETURN														
	S_OK.														

----------------------------------------------------------------
 NOTE:
	The Covar matrix contains the estimated variance-covariance
	of the fit parameters.
	The standard errors of the regression coefficients 
	are the square roots of the diagonal entries in the Covar matrix.

  From:
	Applied regression analysis
	Third Edition
	Norman R. Draper
	Harry Smith
	Page 129.

***************************************************************/
HRESULT ExponentialLinearFitWithWeight(double *chisquared)
{
	double temp;
	// Set nonlinear fit options.
    VD_getNonlinfitOptions( &Opt );

	Opt.UpperLimits = UpperLimits;
	Opt.LowerLimits = LowerLimits;
    VD_setNonlinfitOptions( &Opt );

	if (NumberOfFixedParameters(FitParameterStatus, numberOfFitParameters) < numberOfFitParameters)
	{
		VD_linfitwW(FitParameters, 
					Covar,
					FitParameterStatus, 
					numberOfFitParameters, 
					XExp,					// assigned values
					YExp,					// analyzer data
					Weights, 
					sizex, 
					ExponentialRate);

		// < ------- UPDATE FIT PARAMETERS REPORTING ------- >
		FitParameters[Slope] = 1.0 / FitParameters[Slope];
		FitParameters[Intercept] = exp(-FitParameters[Slope]*FitParameters[Intercept]);

		temp = FitParameters[Slope];
		FitParameters[Slope] = FitParameters[Intercept];
		FitParameters[Intercept] = temp;
		// </ ------- UPDATE FIT PARAMETERS REPORTING ------- >
	}
	else
	{
		Covar[Slope][Slope] = 0;
		Covar[Intercept][Intercept] = 0;
	}

	*chisquared = ExponentialChiSquared(FitParameters, 
									    XExp,			// assigned values
									    YExp,			// analyzer data
									    Weights, 
									    sizex);

	return S_OK;
}

/*****************************************
Calculate assigned values using the rates, 
	i.e. data from analyzer.
To plot with the assigned values. 
How close are the calculated assigned values 
to the given assigned values.

	y = a*e^b*x reduce this equation to:
	Rate = m.ln(Conc) + b same as:

	y = m.ln(x) + b

	x - Assigned values
	y - Rates

	Given: rates
	 Find: Assigned values

	a = fitParameters[0]	slope.
	b = fitParameters[1]	intercept.
	
	Fit parameters interpreted as in linear case.

******************************************/
void ExponentialCalculateAssignedValues(CArray<double,double> *fitParameters,
										CArray<double,double> *calibratorData,
										CArray<double,double> *calcAssignedValues)
{
    double	y;

	calcAssignedValues->RemoveAll();

	for (int k=0; k<calibratorData->GetSize(); k++)
	{
		y = fitParameters->GetAt(Slope) * exp(fitParameters->GetAt(Intercept)*calibratorData->GetAt(k));

		calcAssignedValues->Add(y);
	}
}

/****************************************************************
HRESULT ExponentialRatesFromAssignedValues(LONG calibrationID,
										   dVector fitParameters,
										   VARIANT *rates,
										   VARIANT *error_msg)

	Calculate rates from assigned values.
	Get assigned values.
	Calculate rates.
	Put rates in return array.

	y = m.ln(x) + b

	y = assigned values
	x = rates

	Given: Assigned values, y.
	Solve: rates, x.

	Solve for x:

	y - b 
	----- = ln(Conc)
	  m

	Conc = exp[(y - b)/m]

	Where:
		m = FitParameter[Slope]
		b = FitParameter[Intercept]

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
	X				- Allocate memory for sample assigned values.
	Y				- Allocate memory for rates of assigned values.
	assignedValues	- Current assigned values.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT ExponentialRatesFromAssignedValues(LONG calibrationID,
										   dVector fitParameters,
										   VARIANT *rates,
										   VARIANT *error_msg)
{
	HRESULT					hr;
	BSTR					bstr = NULL;
	long					numberOfRates;
	long					i=0;
	double					x;
	CArray<double,double>	assignedValues;
	CArray<double,double>	l_rates;
	CArray<int,int>			sampleSizesNoOutliers;
	CStringArray			a_sampleIdList;
	char					buf[128];
	CString					tmp;

	l_rates.RemoveAll();

	//*** define the safearray ***
    SAFEARRAY FAR* psa_Rates = NULL;	// assigned rates.

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function","ExponentialRatesFromAssignedValues");
		return hr;
	}

	hr = RemoveCONTROLData(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"RemoveCONTROLData\" function","ExponentialRatesFromAssignedValues");
		return hr;
	}

	hr = GetSampleAssignedValues(calibrationID, &a_sampleIdList, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetSampleAssignedValues\" function","ExponentialRatesFromAssignedValues");
		return hr;
	}

	for (int k=0; k<a_sampleIdList.GetSize(); k++)
	{
		//	x = (1/a)*[ln(y) - ln(b)]
		//	y-assigned value, 
		//	x-rate
		//	a-slope
		//	b-intercept
		x = (1.0/FitParameters[Intercept])*log(assignedValues.GetAt(k)/FitParameters[Slope]);
		l_rates.Add(x);
	}

	numberOfRates = l_rates.GetSize();

	// Set size of array to pass to client.
	sabound[0].cElements = 2 * numberOfRates;
	sabound[0].lLbound = 0;

	//*** create the safearray to return ***
	psa_Rates = SafeArrayCreate(VT_BSTR, 1, sabound);

	i=0;
	for (k=0; k<(ui)numberOfRates; k++)
	{
		// Put sample id in return array.
		bstr = a_sampleIdList.GetAt(k).AllocSysString();
		hr = SafeArrayPutElement(psa_Rates, &i, bstr);

		i++;

		// Put rate in return array.
		sprintf(buf, "%.15f", l_rates.GetAt(k));
		tmp = buf;
		bstr = tmp.AllocSysString();
		hr = SafeArrayPutElement(psa_Rates, &i, bstr);

		i++;
	}

	// Set the return array's type, array of VT_R8.
	// VT_R8, 8-byte real. Type of data to return in plotData.
	//*** set up the safearray to export it as an array of reals.  
	V_VT(rates) = VT_ARRAY | VT_BSTR;

	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
	V_ARRAY(rates) = psa_Rates;

	return S_OK;
}

/*
double ExponentialCorrelationCoefficient(dVector X, 
										 dVector Y,
										 ui sizeYX)
SAME AS LINEAR
*/
double ExponentialCorrelationCoefficient(dVector X, 
										 dVector Y,
										 ui sizeYX)
{
	return (LinearCorrelationCoefficient(X, Y, sizeYX));
}

double ExponentialChiSquared(dVector LinearParams, 
							 dVector X, 
							 dVector Y, 
							 dVector W, 
							 ui s )
{
	return(LinearChiSquared(LinearParams, X, Y, W, s));
}

/*************************************************************

 The model function funcs must calculate the individual 
 f[i](x) for any argument x and store the f[i](x) in a vector 
 BasFuncs of size npars.

*************************************************************/
void ExponentialRate( dVector BasFuncs, double x, unsigned size) 
{
	// BasFuncs[0] = f0( x ); 
	// BasFuncs[1] = f1( x ); 

	BasFuncs[0] = log(x);
	BasFuncs[1] = 1;
}

void InitializeExponentialFitParameters(dVector FitPars)
{
	// Initilize slope and intercept to 0.0.
	FitPars[Slope] = 0.0;

	FitPars[Intercept] = 0.0;
}
