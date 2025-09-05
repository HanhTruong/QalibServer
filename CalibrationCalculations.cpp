/*******************************************************************
FILENAME:	CalibrationCalculations.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for calibration processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	RatesFromAssignedValues
	RemoveCONTROLData
	InitializeGlobals
	CalculateWeights
	BartlettsTest
	NumberOfFreeParameters
	NumberOfFixedParameters
	DerivModel
	CalibrationCalculations
	QalibNonlinearFitWithWeight
	QalibWeightMethod

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	CalibrationCalculations.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <VIstd.h>
#include <math.h>
#include "Utils.h"
#include "Support.h"
#include "Loging.h"
#include "globals.h"
#include "SigmoidalCalculations.h"
#include "LinearCalibration.h"
#include "AnalyteTypeClass.h"
#include "BusinessRulesSupport.h"
#include "derivatives.h"
#include "CalibrationCalculations.h"
#include "QuadraticCalculations.h"
#include "LinearFitParameters.h"
#include "Rotor.h"
#include "FixedSlope.h"
#include "ExponentialCalculations.h"

/****************************************************************
HRESULT RatesFromAssignedValues(LONG calibrationID, 
								CString algorithm, 
								VARIANT *rates)

	Calculate rates from assigned values.
	Get assigned values.
	Set up vector spaces.
	Calculate rates. Depending on algorithm.
	Put rates in return array.
-----------------------------------------------------------------
 INPUT														
	calibrationID - ID of calibration to calculate rates.
	algorithm - Rates calculations depend on algorithm.
	*Rates - Structure to return rates in.

-----------------------------------------------------------------
 OUTPUT														
	Rates calculated from assigned values.
	
-----------------------------------------------------------------
 LOCAL
	hr - Return code.

-----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT RatesFromAssignedValues(LONG calibrationID,
								CString algorithm,
								dVector fitParameters,
								VARIANT *rates,
								VARIANT *error_msg)
{
	HRESULT		hr;

CString temp="-+-+ BEGIN RatesFromAssignedValues +-+-";
DisplayCString(&temp);

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = SigmoidalRatesFromAssignedValues(calibrationID, rates, error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from function \"SigmoidalRatesFromAssignedValues\"","RatesFromAssignedValues");
			return hr;
		}
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
temp="++++ BEFORE CALL TO LinearRatesFromAssignedValues ++++";
DisplayCString(&temp);

		hr = LinearRatesFromAssignedValues(calibrationID,
										   fitParameters,
										   rates,
										   error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from function \"LinearRatesFromAssignedValues\"","RatesFromAssignedValues");
			return hr;
		}

temp="++++ AFTER CALL TO LinearRatesFromAssignedValues ++++";
DisplayCString(&temp);
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		hr = QuadraticRatesFromAssignedValues(calibrationID,fitParameters,rates,error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from function \"QuadraticRatesFromAssignedValues\"","RatesFromAssignedValues");
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = ExponentialRatesFromAssignedValues(calibrationID,fitParameters,rates,error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from function \"QuadraticRatesFromAssignedValues\"","RatesFromAssignedValues");
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
	}

temp="-+-+ END RatesFromAssignedValues +-+-";
DisplayCString(&temp);

	return S_OK;
}

/****************************************************************
HRESULT RemoveCONTROLData(LONG calibrationID, 
						  CStringArray *sampleIDsList)

	Removes the names of sample type control.

-----------------------------------------------------------------
 INPUT														
	calibrationID - ID of calibration to calculate rates.
	sampleIDsList - Current sample list.

-----------------------------------------------------------------
 OUTPUT														
	Calibrator sample ids list.
	
-----------------------------------------------------------------
 LOCAL
	hr - Function status code.
	l_sampleIDs - Local sample ids.
	l_sampleTypesList - Local sample type list.

-----------------------------------------------------------------
 RETURN
	hr - Function status code.

****************************************************************/
HRESULT RemoveCONTROLData(LONG calibrationID, 
						  CStringArray *sampleIDsList)
{
	HRESULT hr;
	CStringArray l_sampleIDs;
	CStringArray l_sampleTypesList;

	// Get current sample's types.
	hr = GetSamplesAnalyteTypes(sampleIDsList, &l_sampleTypesList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr, 
				"Return from function \"GetSamplesAnalyteTypes\"",
				"RemoveCONTROLData");
		return hr;
	}

	// Remove CONTROL type samples.
	for (int i=0; i<l_sampleTypesList.GetSize(); i++)
	{
		if (l_sampleTypesList.GetAt(i).CompareNoCase("CONTROL") !=0 )
		{
			l_sampleIDs.Add(sampleIDsList->GetAt(i));
		}
	}

	// Clear current sample id list.
	sampleIDsList->RemoveAll();
	// Update current sample id list.
	sampleIDsList->Copy(l_sampleIDs);

	return S_OK;
}

/*****************************************
void InitializeGlobals(CString algorithm)

  Initialize globals, depending on algorithm.

------------------------------------------
INPUT
	algorithm - Current algorithm.

------------------------------------------
OUTPUT
	Initialized globals.

------------------------------------------
RETURN
	none.
*****************************************/
void InitializeGlobals(CString algorithm)
{

	if (algorithm.CompareNoCase("LINEAR") == 0)
	{
		numberOfFitParameters = 2;
		FitParameterStatus = VI_vector( numberOfFitParameters );
		FitParameterStatus[0] = FitParameterStatus[1] = 1;
	}
	else if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		numberOfFitParameters = 4;	// Quadratic has 3 fit parameters. a1,a2,a3.

		FitParameterStatus = VI_vector( numberOfFitParameters );

		FitParameterStatus[0] = FitParameterStatus[1] = FitParameterStatus[2] = FitParameterStatus[3] = 1;
	}
	else if (algorithm.CompareNoCase("FIXEDSLOPE") == 0)
	{
		numberOfFitParameters = 2;
		FitParameterStatus = VI_vector( numberOfFitParameters );
		FitParameterStatus[0] = 0;	// FIXEDSLOPE. Keep slope fit parameter constant.
		FitParameterStatus[1] = 1;
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		numberOfFitParameters = 3;	// Quadratic has 3 fit parameters. a1,a2,a3.
									// numberOfFitParameters = deg + 1

		FitParameterStatus = VI_vector( numberOfFitParameters );

		// Fit parameter status not currently used with quadratic algorithm.
		FitParameterStatus[0] = FitParameterStatus[1] = FitParameterStatus[2] = 1;
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		numberOfFitParameters = 2;
		FitParameterStatus = VI_vector( numberOfFitParameters );
		FitParameterStatus[0] = FitParameterStatus[1] = 1;
	}

	FitParameters = VD_vector( numberOfFitParameters );
	Covar = MD_matrix( numberOfFitParameters, numberOfFitParameters);
	UpperLimits = VD_vector( numberOfFitParameters );
	LowerLimits = VD_vector( numberOfFitParameters );
}

/*****************************************
void CalculateWeights(dVector W, ui size)

  Sets weights to default values, all 1's.

------------------------------------------
INPUT
	size - Number of calibration values.

------------------------------------------
OUTPUT
	Sets objects results vector with 1's.

------------------------------------------
RETURN
	none.
*****************************************/
void CalculateWeights(dVector W, ui size)
{
	// Default weight values are 1's for all weights.
	// Initialize a vector with 1.0.
	VD_equ1( W, size );
}

/**************************************************************
void CalculateWeights(dVector W,
					  dVector Rate, 
				      ui size, 
					  uVector poolsizes, 
					  ui poolsizelen)

Calculates the weights for the current set of calibration data.
	Standard deviation.

--------------------------------------------------------------
INPUT
	W			- Vector to return calculated weights.
	Rate		- Current calibration data.
	size		- Number of calibration values.
	poolsizes	- Size of each pool in calibration data.
	poolsizelen - Number of samples in calibration data.

--------------------------------------------------------------
OUTPUT
	Sets objects results vector with calculated weights.

--------------------------------------------------------------
RETURN
	none.
**************************************************************/
void CalculateWeights(dVector W,
					  dVector Rate,
					  ui size,
					  liVector poolsizes,
					  ui poolsizelen)
{
	dVector tCal;
	double	variance;
	double	samplemean;

	ui		ri=0;
	ui		rs=0;

	for (ui i=0; i<poolsizelen; i++)
	{
		tCal = VD_vector( poolsizes[i] );

		for (long k=0; k<poolsizes[i]; k++)
		{
			tCal[k] = Rate[ri++];
		}

		// mean = (1 / size) * sum( X[i] )
		// var = (1 / (size-1)) * sum( (X[i] - mean)^2 )
		samplemean = VD_meanvar( &variance, tCal, poolsizes[i] );

		// Set weights vector.
		for (k=(long)rs; k<(long)ri; k++)
		{
			W[k] = 1.0/variance;	// InvVar - inverse of the variance.
		}

		rs = ri;

		V_free(tCal);
	}
}

/***************************************************
void Sample::CalculateWeights()
Sets weights to historical values from the database.

INPUT

OUTPUT

RETURN

***************************************************/
void CalculateWeights()
{
	cout << "Nothing to calculate \"Historicaly!\"" << endl;
	cout << "\"Historicaly!\" means from db." << endl;
}

/***************************************************
double BartlettsTest(dVector	Rate,
					 ui			size,
					 uVector	poolsizes,
					 ui			poolsizelen)

	Bartlett's test is used to test if k samples have 
equal variances.
	Equal variances across samples is called
"homogeneity" of variances.

-----------------------------------------------------
INPUT
	Rate		- current experimental data.
	size		- number of current experimental values.
	poolsizes	- size of each pool.
	poolsizelen - number of pools.

-----------------------------------------------------
OUTPUT

-----------------------------------------------------
LOCAL
	tCal - Intermediate values vector.
	groupvariances - Group variances.
	pooledVariance - Sample variances.
	cVariance - Intermediate value.
	samplemean - Mean of sample.
	T - Homogeneity value.
	ri - Rate vector index.

-----------------------------------------------------
RETURN
	Homogeneity value.

***************************************************/
double BartlettsTest(dVector	Rate,
					 ui			size,
					 liVector	poolsizes,
					 ui			poolsizelen)
{
	dVector tCal;
	dVector groupvariances;
	double	pooledVariance;
	double	cVariance;
	double	samplemean;
	double	T;
	ui		ri=0;

	// Array to hold group variances.
	groupvariances = VD_vector( poolsizelen );

	// Find group variances.
	for (ui i=0; i<poolsizelen; i++)
	{
		tCal = VD_vector( poolsizes[i] );

		for (long k=0; k<poolsizes[i]; k++)
		{
			tCal[k] = Rate[ri++];
		}

		// mean = (1 / size) * sum( X[i] )
		// var = (1 / (size-1)) * sum( (X[i] - mean)^2 ) 
		// float VF_meanvar( float *Var, fVector X, ui size );
		samplemean = VD_meanvar( &cVariance, tCal, poolsizes[i] );

		// Save current group's variance.
		groupvariances[i] = cVariance;

		V_free(tCal);
	}

	pooledVariance = 0;
	// Find pooled variance
	for (i=0; i<poolsizelen; i++)
	{
		pooledVariance = pooledVariance + ((poolsizes[i] - 1) * groupvariances[i]);
	}
	pooledVariance = pooledVariance / (size - poolsizelen);

	// Numerator terms.
	double numeratorTerm1 = (size - poolsizelen) * log(pooledVariance);

	double numeratorTerm2 = 0;
	for (i=0; i<poolsizelen; i++)
	{
		numeratorTerm2 = numeratorTerm2 + (poolsizes[i] - 1) * log(groupvariances[i]);
	}

	double numerator = numeratorTerm1 - numeratorTerm2;

	double denomenatorTerm1 = 0;
	for (i=0; i<poolsizelen; i++)
	{
		denomenatorTerm1 = denomenatorTerm1 + 1.0 / (poolsizes[i] - 1);
	}

	double denomenator = 1 + (1 / (3 * (poolsizelen - 1))) * (denomenatorTerm1 - ( 1.0 / (size - poolsizelen)));

	T = numerator / denomenator;

	return T;
}

/*******************************************************
int NumberOfFreeParameters()

  Count the number of fit parameters with "free" status.
  "free" status, fit parameter will be changed.

  status = 0, hold fixed at their input.
  status = 1, free, allow changes.

--------------------------------------------------------
INPUT
	None.

--------------------------------------------------------
OUTPUT
	None.

--------------------------------------------------------
LOCAL
Count - Status counter.

--------------------------------------------------------
RETURN
	Number of "free" status.
*******************************************************/
int NumberOfFreeParameters()
{
	int Count = 0;

	for (int i=0; i<numberOfFitParameters; i++)
	{
		if (FitParameterStatus[i] == 1)
		{
			Count++;
		}
	}

	return Count;
}

/*******************************************************
int NumberOfFixedParameters(iVector statusVector, int statusSize)

  Count the number of fit parameters with "fixed" status.
  "fixed" status, fit parameter will not change.

  status = 0, hold fixed at their input.
  status = 1, free, allow changes.

--------------------------------------------------------
INPUT
	None.

--------------------------------------------------------
OUTPUT
	None.

--------------------------------------------------------
LOCAL
Count - Status counter.

--------------------------------------------------------
RETURN
	Number of "fixed" status.
*******************************************************/
int NumberOfFixedParameters(iVector statusVector, int statusSize)
{
	int Count = 0;

	for (int i=0; i<statusSize; i++)
	{
		if (statusVector[i] == 0)
		{
			Count++;
		}
	}

	return Count;
}


/********************************************************************
 void DerivModel( dVector dYdAi, dVector X, ui size, unsigned iPar )
	Calculate the derivatives of the regression.
	If fit parameter index, iPar, is unknown.
	A default derivative function will be called.
---------------------------------------------------------------------
 INPUT
	dYdAi - Derivative results vector.
	X	  - Vector containing values to be deferentiated.
	size  - Size of derevitive arrays.
	iPar  - Index of current fit parameter to be deferentiated.
---------------------------------------------------------------------
 OUTPUT
	dYdAi - Will contain the derivatives of the current data set.
---------------------------------------------------------------------
 LOCAL
	i	  - index to traverse output vector.
********************************************************************/
void DerivModel( dVector dYdAi, dVector X, ui size, unsigned iPar ) 
{ 
	ui i;
	
//double curChi = VD_nonlinfit_getChi2();
//LogFile << "DerivModel curChi = " << curChi << endl;

	switch( iPar ) 
	{ 
	case 0:		// Slope
		for (i=0; i<size; i++)
		{
			//		Slope FitParameters[0]
			//	Intercept FitParameters[1]
			//		  Top FitParameters[2]
			//	   Bottom FitParameters[3]
			dYdAi[i] = ddslope( X[i], FitParameters[Intercept], FitParameters[Slope], FitParameters[Bottom], FitParameters[Top]);
		}
	break;
	case 1:		// Intercept
		for (i=0; i<size; i++)
		{
			//		Slope FitParameters[0]
			//	Intercept FitParameters[1]
			//		  Top FitParameters[2]
			//	   Bottom FitParameters[3]
			dYdAi[i] = ddintercept( X[i], FitParameters[Intercept], FitParameters[Slope], FitParameters[Bottom], FitParameters[Top]);
		}
	break;
	case 2:		// Top
		for (i=0; i<size; i++)
		{
			//		Slope FitParameters[0]
			//	Intercept FitParameters[1]
			dYdAi[i] = ddtop( X[i], FitParameters[Intercept], FitParameters[Slope]);
		}
	break; 
	case 3:		// Bottom
		for ( i=0; i<size; i++)
		{
			//		Slope FitParameters[0]
			//	Intercept FitParameters[1]
			dYdAi[i] = ddbottom( X[i], FitParameters[Intercept], FitParameters[Slope]);
		}
	break;
	default:	// Unknown
		VD_nonlinfit_autoDeriv( dYdAi, X, size, iPar); 
	} 
}

/********************************************************************
HRESULT CalibrationCalculations(LONG calibrationID,
								CString algorithm,
								double *chisquared,
								VARIANT *newFitParameters,
								VARIANT *error_msg)

	Carry out calibration calculatins.
	Fairly abstract. i.e. All calibration methods go through here.

---------------------------------------------------------------------
 INPUT
	calibrationID - Identifier of current calibration.
	algorithm - Algorithm for current chemistry.
	chisquared - Return chi-squared statistic.
	newFitParameters - Calculate fit parameters.
	error_msg - Error reporting.

---------------------------------------------------------------------
 OUTPUT
	Calibration calculations carried out and results put in db.

---------------------------------------------------------------------
 LOCAL
	hr - Results code.
	buf[512] - Temporary buffer.
	chemistry - Chemistry name for current calibration.

********************************************************************/
HRESULT CalibrationCalculations(LONG calibrationID,
								CString algorithm,
								double *chisquared,
								VARIANT *newFitParameters,
								VARIANT *error_msg)
{
	HRESULT hr;
	char	buf[512];

	// < ----- SIGMOIDAL CALCULATIONS ----- >
	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = SigmoidalCalculations(calibrationID,
								   algorithm,
								   chisquared,
								   newFitParameters,
								   error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"CalibrationCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,	"Return from function \"SigmoidalCalculations\"", buf);
			}
			return hr;
		}
	}
	// </ ----- SIGMOIDAL CALCULATIONS ----- >
	// < ----- LINEAR CALCULATIONS ----- >
	else if (algorithm.CompareNoCase("LINEAR") == 0)
	{
		hr = LinearCalculations(calibrationID,
								algorithm,
								chisquared,
								newFitParameters,
								error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"CalibrationCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,	"Return from function \"LinearCalculations\"", buf);
			}

			return hr;
		}
	}
	// </ ----- LINEAR CALCULATIONS ----- >
	// < ----- FIXED SLOPE CALCULATIONS ----- >
	else if (algorithm.CompareNoCase("FIXEDSLOPE") == 0)
	{
		hr = FixedSlopeCalculations(calibrationID,
									algorithm,
									chisquared,
									newFitParameters,
									error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"CalibrationCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,	"Return from function \"FixedSlopeCalculations\"", buf);
			}
			return hr;
		}
	}
	// </ ----- FIXED SLOPE CALCULATIONS ----- >
	// < ----- QUADRATIC CALCULATIONS ----- >
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		hr = QuadraticCalculations(calibrationID,
								   algorithm,
								   chisquared,
								   newFitParameters,
								   error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"CalibrationCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,	"Return from function \"QuadraticCalculations\"", buf);
			}
			return hr;
		}
	}
	// </ ----- QUADRATIC CALCULATIONS ----- >
	// < ----- EXPONENTIAL CALCULATIONS ----- >
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = ExponentialCalculations(calibrationID,
									 algorithm,
									 chisquared,
									 newFitParameters,
									 error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"CalibrationCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,	"Return from function \"ExponentialCalculations\"", buf);
			}

			return hr;
		}
	}
	// </ ----- EXPONENTIAL CALCULATIONS ----- >

	return hr;
}

/****************************************************************
QalibNonlinearFitWithWeight
	(double *chisquared)

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
HRESULT QalibNonlinearFitWithWeight(double *chisquared)
{
	SetMatrix(Covar, numberOfFitParameters, 0.0);

	// Set nonlinear fit options.
    VD_getNonlinfitOptions( &Opt );

	Opt.UpperLimits = UpperLimits;
	Opt.LowerLimits = LowerLimits;
    VD_setNonlinfitOptions( &Opt );

	if (NumberOfFixedParameters(FitParameterStatus, numberOfFitParameters) < numberOfFitParameters)
	{
		// Calculate non linear fit of data.
		*chisquared = VD_nonlinfitwW(FitParameters, 
									 Covar, 
									 FitParameterStatus, 
									 numberOfFitParameters, 
									 XExp, 
									 YExp, 
									 Weights, 
									 sizex, 
									 Rate, 
									 DerivModel);
	}
	else
	{
		*chisquared = VD_chi2( XExp, YExp, Weights, sizex );
		Covar[Top][Top] = 0;
		Covar[Bottom][Bottom] = 0;
		Covar[Slope][Slope] = 0;
		Covar[Intercept][Intercept] = 0;
	}

	return S_OK;
}

/*************************************************************
	QalibWeightMethod(BSTR wMethod)
 	Calculates weights based on given method.

--------------------------------------------------------------
 INPUT
	wMethod - Current weight method.

--------------------------------------------------------------
 OUTPUT
	Current weights.

--------------------------------------------------------------
 LOCAL
	strwMethod - Convert weight method from BSTR to CString.

--------------------------------------------------------------
 GLOBALS														
	numberofsamples - Number of samples in current calibartion.	
	sizex - Number of data points in all samples.				
	Weights - Vector to contain calculated weights.				
	XExp - Vector to contain assigned values.					
	PoolSizes - Vector to contain lengths of each sample.		

--------------------------------------------------------------
 FUNCTIONS
	CalculateWeights

--------------------------------------------------------------
 RETURN
	S_OK - Error code.
*************************************************************/
HRESULT QalibWeightMethod(BSTR wMethod)
{
	HRESULT hr = S_OK;

	// Convert current method label, from BSTR to CString.
	CString strwMethod = wMethod;

	// Calculate weights.
	if (strwMethod.CompareNoCase("RATE") == 0)
	{
		// Weights - Calculated weight values.
		// YExp - Data used in calculating weights.
		CalculateWeights(Weights, YExp, sizex, PoolSizes, numberofsamples);
	}
	else if (strwMethod.CompareNoCase("HISTORICAL") == 0)
	{
		CalculateWeights();
	}
	else if (strwMethod.CompareNoCase("NONE") == 0)
	{
		// Default weights calculations. Set all weights to 1.
		CalculateWeights(Weights, sizex);
	}
	else
	{
		LogErrorMessage(hr,"Unknown weight method!","QalibWeightMethod");
		return hr;
	}

	return S_OK;
}
