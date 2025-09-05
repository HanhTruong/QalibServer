/********************************************************************

FILENAME:	LinearCalibration.cpp
_____________________________________________________________________

MODULE DESCRIPTION:
	Functions to process a calibration with a linear algorithm.

_____________________________________________________________________

GLOBAL FUNCTIONS:

	Name			Description

_____________________________________________________________________

COMPILER:	Visual C++ 6.0		INVOCATION: Build->LinearCalibration.cpp

_____________________________________________________________________

$HISTORY: $

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Linear calibration processing.

********************************************************************/
#include "stdafx.h"
#include <VDstd.h> 
#include "dboInsertLinearFitParameters.H"
#include "DbAccess.h"
#include "Loging.h"
#include "Support.h"
#include "globals.h"
#include "Utils.h"
#include "DbUtils.h"
#include "CalibrationCalculations.h"
#include "LinearCalibration.h"
#include "LinearFitParameters.h"
#include "Rotor.h"
#include "AssignedValues.h"
#include "FitParameters.h"

void LinearRate( dVector BasFuncs, double x, unsigned size);

/*****************************************************************
HRESULT LinearFitCalibration(LONG calibrationID,
							 CString algorithm,
							 dVector fitParameters,		// A
							 dMatrix Covariance,		// Covar
							 iVector AStatus,			// Status
							 unsigned numberFitPars,	// number of fit parameters.
							 dVector assignedValues,	// X
						     dVector analyzerData,		// Y
						     dVector Weights,			// InvVar
						     ui sizex,					// sizex
							 double *chisquared)

Data-fitting to models y=f(x) linear in the parameters

INPUT
	calibrationID - Identifier for current calibration.
	algorithm - Algorithm of current calibration.
	fitParameters - Fit parameters. Calculated by OptiVec's function.
	Covariance - Variance between the calibration data sets.
	AStatus - Status of fit parameters.
	numberFitPars - Number of fit parameters to calculate.
	assignedValues - Assigned values data set.
	analyzerData - Analyzer data set. To calibrate.
	Weights - Weights for linear fit calculations.
	sizex - Size of calibration data vectors.
	*chisquared - Chi-squared statistic, from OptiVec's function.

OUTPUT
	fitParameters - Fit parameters. Calculated by OptiVec's function.
	Covariance - Variance between the calibration data sets.
	*chisquared - Chi-squared statistic, from OptiVec's function.

LOCAL
	NONE.

FUNCTIONS
	NumberOfFixedParameters - Test number of fit parameters that are fixed.
	VD_linfitwW - Calculate linear fit.
	LinearChiSquared - Chi-squared statistic, if all parameters are fixed.
	LinearRate - Linear function model used by OptiVec's VD_linfitwW function.

*****************************************************************/
HRESULT LinearFitCalibration(LONG calibrationID,
							 CString algorithm,
							 dVector fitParameters,		// A
							 dMatrix Covariance,		// Covar
							 iVector AStatus,			// Status
							 unsigned numberFitPars,	// number of fit parameters.
							 dVector assignedValues,	// X
						     dVector analyzerData,		// Y
						     dVector W,					// InvVar
						     ui size,					// sizex
							 double *chisquared,
							 VARIANT *error_msg)
{
	HRESULT hr;
	char	buf[512];

	/*
	#include <MFstd.h>
	void VF_linfitwW( fVector A, fMatrix Covar, iVector AStatus, unsigned npars, fVector X, fVector Y, fVector InvVar, ui sizex,
	void (*funcs)(fVector BasFuncs, float x, unsigned nfuncs));

	The input data X, Y (and InvVar) are used to evaluate the parameters 
	ai of a general linear function,
	y = a0f0(x) + a1f1(x) + a2f2(x)...
	The parameters ai are returned in the vector A. 

	Arguments: 
	A				vector of size npars; returns the coefficients 

	Covar			matrix of dimensions [npars, npars];
					returns the covariances of the coefficients 

	AStatus			vector of size npars; decides which parameters 
					are treated as free or as fixed 

	npars			total number of parameters 

	X, Y, InvVar	vectors of size sizex, holding the input data 
	
	funcs			user-defined model function 
 
	Your model function may actually contain more parameters 
	than you wish to treat as adjustable. 
	This is why you have to provide the vector AStatus, 
	which contains the necessary information about which parameters are 
	to be held fixed at their input values (AStatus[i] = 0) 
	and which are free (AStatus[i] = 1). Any fixed parameters must be 
	initialized in A prior to calling VF_linfit. npars denotes the total 
	number of parameters in A (not only the free parameters!). 

	The model function funcs must calculate the individual fi(x) for 
	any argument x and store the fi(x) in a vector BasFuncs of size npars. 
	In C/C++, it has to be defined as
	void MyFunc( fVector BasFuncs, float x, unsigned nfuncs) 
	{ 
	  BasFuncs[0] = f0( x ); 
	  BasFuncs[1] = f1( x ); 
	  . . . 
	}
	and shall be passed to VF_linfit by calling
	VF_linfit( A, AStatus, npars, X, Y, sizex, MyFunc );
	*/

/*
CString temp="++++ BEGIN LinearFitCalibration ++++";
DisplayCString(&temp);
DisplayVector(assignedValues,size,"Assigned Values");
DisplayVector(analyzerData,size,"Data");
DisplayVector(W,size,"Weights");
*/

	if (NumberOfFixedParameters(AStatus, numberFitPars) < numberFitPars)
	{
		VD_linfitwW(fitParameters, 
					Covariance, 
					AStatus, 
					numberFitPars, 
					assignedValues, 
					analyzerData, 
					W, 
					size, 
					LinearRate);

		// < ----- UPDATE FIT PARAMETERS REPORTING ----- >
		hr = UpdateLinearFitParameters(calibrationID,error_msg);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"LinearFitCalibration\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"UpdateLinearFitParameters\"", buf);
			return hr;
		}
		// </ ----- UPDATE FIT PARAMETERS REPORTING ----- >
	}
	else
	{
		*chisquared = LinearChiSquared(fitParameters,
									   assignedValues, 
									   analyzerData, 
									   W, 
									   size);

		Covariance[0][0] = 0;
		Covariance[1][1] = 0;
	}

//temp="++++ END LinearFitCalibration ++++";
//DisplayCString(&temp);

	return S_OK;
}

/********************************************************************
FUNCTION:
	LinearRate - Rate calculations for linear model.
_____________________________________________________________________
DESCRIPTION:

 The model function funcs must calculate the individual 
 fi(x) for any argument x and store the fi(x) in a vector 
 BasFuncs of size npars.
 void LinearRate( dVector BasFuncs, float x, unsigned nfuncs) 
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	None.
____________________________________________________________________
PARAMETERS:
	BasFuncs - Model calculation functions.
	x - Model function variable.
	size - Number of functions.
____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Model function for calibration.

********************************************************************/
void LinearRate( dVector BasFuncs, double x, unsigned size) 
{ 
	// BasFuncs[0] = f0( x ); 
	// BasFuncs[1] = f1( x ); 

	BasFuncs[0] = x;
	BasFuncs[1] = 1;
}

/********************************************************************
FUNCTION:
	LinearCorrelationCoefficient
_____________________________________________________________________
DESCRIPTION:
	Calculate correlation coefficient for linear model calibration.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	ccoe - Correlation coefficient.
____________________________________________________________________
PARAMETERS:
	X - assigned values.
	Y - rates. Analyzer data.
____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Correlation coefficient for linear algorithm.

********************************************************************/
double LinearCorrelationCoefficient(dVector X,
									dVector Y,
									ui n)
{
	double ccoe;

	/*
	The linear correlation coefficient ("Pearson's r") takes on 
	values between -1.0 and +1.0. 
	The mean values of both distributions must be known. 
	They are passed to VD_corrcoeff as the parameters Xmean and Ymean. 

	Return value linear correlation coefficient r 

	Example C/C++ r = VD_corrcoeff( X, Y, n, VD_mean( X, n ), VD_mean( Y, n ) ); 
	double VD_corrcoeff( dVector X, dVector Y, ui size, double Xmean, double Ymean );
	*/

	ccoe = VD_corrcoeff( X, Y, n, VD_mean( X, n ), VD_mean( Y, n ) ); 

	return ccoe;
}

/********************************************************
HRESULT InsertLinearFitParameters(LONG calibrationID,
								  dVector FitPars,
								  double ccoe,
								  double slopeError,
								  double interceptError,
								  double chi_squared,
								  double bartlett)

Add calibration results, calculated with a linear algorithm, to the database.

---------------------------------------------------------
INPUT
	calibrationID - Identifier of current calibration.
	FitPars - Vector containing calculated fit parameters.
	size - Number of fit parameters.
	ccoe - Correlation coefficient.
	SlopeError - Error on Slope.
	InterceptError - Error on Intercept.
	chisquared - Chi squared statistic test.
	bartlett - Homogeneity test.
	
---------------------------------------------------------
LOCAL
	hr - Return code.
	db - Database access object.
	session - Session access.

---------------------------------------------------------
OUTPUT
	Updated sigmoidal fit parameters.

---------------------------------------------------------
RETURN
	hr - Status code.
********************************************************/
HRESULT InsertLinearFitParameters(LONG calibrationID,
								  dVector FitPars,
								  double ccoe,
								  double slopeError,
								  double interceptError,
								  double chi_squared,
								  double bartlett)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to insert linear fit parameters into the database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboInsertLinearFitParametersAccessor> > cmdInsertLinearFitParameters;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "InsertLinearFitParameters" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","InsertLinearFitParameters");
		return hr;
	}

	hr = cmdInsertLinearFitParameters.Create (session, _T("{ ? = CALL dbo.InsertLinearFitParameters;1 (?,?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for stored procedure \"InsertLinearFitParameters\"","InsertLinearFitParameters");
		return hr;
	}

	// Set data to add to the database.
	cmdInsertLinearFitParameters.m_calibrationID = calibrationID;
	cmdInsertLinearFitParameters.m_CorrelationCoefficient = ccoe;
	cmdInsertLinearFitParameters.m_Slope = FitPars[0];
	cmdInsertLinearFitParameters.m_Intercept = FitPars[1];
	cmdInsertLinearFitParameters.m_SlopeStdError = slopeError;
	cmdInsertLinearFitParameters.m_InterceptStdError = interceptError;
	cmdInsertLinearFitParameters.m_ChiSquared = chi_squared;
	cmdInsertLinearFitParameters.m_BartlettTest = bartlett;

	hr = cmdInsertLinearFitParameters.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"InsertLinearFitParameters\"","InsertLinearFitParameters");
		return hr;
	}

	cmdInsertLinearFitParameters.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/********************************************************************
FUNCTION:
	LinearCalculateAssignedValues
_____________________________________________________________________
DESCRIPTION:
	Calculate assigned values with:
	y[Assigned] = a * x[AnalyzerData] + b. Used to calculate a,b.

	y[CalculatedAssigned] = a * x[AnalyzerData] + b. 
	Used to calculate calculated assigned values.
	a - Slope, b - Intercept.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	x - Intermediate value.
____________________________________________________________________
PARAMETERS:
	fitParameters - Current fit parameters.
	calibratorData - Analyzer data.
	calcAssignedValues - Return calculated assigned values.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Calculate assigned values.

********************************************************************/
void LinearCalculateAssignedValues(CArray<double,double> *fitParameters,
								   CArray<double,double> *calibratorData,
								   CArray<double,double> *calcAssignedValues)
{
    double		x;

	calcAssignedValues->RemoveAll();

	for (int k=0; k<calibratorData->GetSize(); k++)
	{
		x = fitParameters->GetAt(0) * calibratorData->GetAt(k) + fitParameters->GetAt(1);

		calcAssignedValues->Add(x);
	}
}

/****************************************************************
HRESULT LinearRatesFromAssignedValues
			(LONG calibrationID, 
			dVector linearParameters, 
			VARIANT *rates,
			VARIANT *error_msg)

	Calculate rates from assigned values.
	Get assigned values.
	Set up vector spaces.
	Calculate rates.
	Put rates in return array.

	y[assigned] = a[slope] * x[analyzer_data] + b[intercept]

	x[rates_from_assigned_values] = (y[assigned] - b[intercept]) / a[slope] 

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
HRESULT LinearRatesFromAssignedValues(LONG calibrationID, 
									  dVector linearParameters, 
									  VARIANT *rates,
									  VARIANT *error_msg)
{
	HRESULT		hr;
	BSTR		bstr = NULL;
	long		numberOfRates;
	long		i=0;
	double		x;
	CArray<double,double> assignedValues;
	CArray<double,double> l_rates;
	CStringArray		  a_sampleIdList;
	char				  buf[128];
	CString				  tmp;

	//*** define the safearray ***
    SAFEARRAY FAR* psa_Rates = NULL;	// assigned rates.

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function","LinearRatesFromAssignedValues");
		return hr;
	}

	hr = RemoveCONTROLData(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"RemoveCONTROLData\" function","LinearRatesFromAssignedValues");
		return hr;
	}

CString temp="++++ BEFORE CALL TO GetSampleAssignedValues ++++";
DisplayCString(&temp);

	hr = GetSampleAssignedValues(calibrationID, &a_sampleIdList, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetSampleAssignedValues\" function","LinearRatesFromAssignedValues");
		return hr;
	}

temp="++++ AFTER CALL TO GetSampleAssignedValues ++++";
DisplayCString(&temp);

	// < ----- CALCULATE RATES FROM ASSIGNED VALUES ----- >
	for (int k=0; k<assignedValues.GetSize(); k++)
	{
		x = (assignedValues.GetAt(k) - linearParameters[1]) / linearParameters[0];
		l_rates.Add(x);
	}
	// </ ----- CALCULATE RATES FROM ASSIGNED VALUES ----- >

	numberOfRates = l_rates.GetSize();

	// Set size of array to pass to client.
	sabound[0].cElements = 2 * numberOfRates;
	sabound[0].lLbound = 0;

	//*** create the safearray to return ***
	psa_Rates = SafeArrayCreate(VT_BSTR, 1, sabound);

	i=0;
	for (k=0; k<(ui)numberOfRates; k++)
	{
		bstr = a_sampleIdList.GetAt(k).AllocSysString();
		hr = SafeArrayPutElement(psa_Rates, &i, bstr);

		i++;

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

/****************************************************************
HRESULT EvaluateLinearFitParameters(VARIANT *newFitPars
									VARIANT *error_msg) 

	Test limits for new linear fit parameters.

-----------------------------------------------------------------
 INPUT														
	newFitPars - List of fit parameters. Id, value pairs.
	error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT														
	Evaluated fit parameters.
	
-----------------------------------------------------------------
 LOCAL
	hr			- Return code.
	bstr		- BSTR to move the strings in and out.
	i			- Variant array's traversal index.
	lLCID		- Language code, to convert from string to double.
	no_error	- Loop control. Did an error occur?
	curValue	- Current fit parameter converted value.
	temp		- New fit parameter label.
	local_error - Error message.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT EvaluateLinearFitParameters(VARIANT *newFitPars,
									VARIANT *error_msg) 
{
	HRESULT		hr;
	DWORD		lLCID;
	bool		no_error = true;
	char		buf[512];

	// Find a Language code, used by conversion functions. i.e. VarR8FromStr.
	lLCID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

	//*** define the safearray ***
    SAFEARRAY FAR* psa_newFitPars = NULL;

	// set new fit parameter values VARIANT type to SAFEARRAY type.
	psa_newFitPars = V_ARRAY(newFitPars);

	//*** define a BSTR to move the strings in and out ***
	double	curValue,limit;
	BSTR	bstr = NULL;

	long i = 0;
	CString temp;
	CString local_error;

	// Get fit parameter's identifier.
	while (((hr = SafeArrayGetElement(psa_newFitPars, &i, &bstr)) == S_OK) && no_error)
	{
		// Get current fit parameter name.
		temp = bstr;

		i++;

		// Get new fit parameter value.
		hr = SafeArrayGetElement(psa_newFitPars, &i, &bstr);
		// Convert string data to double values.
		hr = VarR8FromStr(bstr,lLCID,LOCALE_NOUSEROVERRIDE,&curValue);

		if (temp.CompareNoCase("SLOPE") == 0)
		{
			FitParameters[0] = curValue;
			
			// Set status of fit parameter "Slope" to fixed.
			FitParameterStatus[0] = 0;

			if (curValue > UpperLimits[0])
			{
				limit = UpperLimits[0];
				local_error = "high";
				no_error = false;
			}
			else if (curValue < LowerLimits[0])
			{
				limit = LowerLimits[0];
				local_error = "low";
				no_error = false;
			}
		}
		else if (temp.CompareNoCase("INTERCEPT") == 0)
		{
			FitParameters[1] = curValue;

			// Set status of fit parameter "Intercept" to fixed.
			FitParameterStatus[1] = 0;

			if (curValue > UpperLimits[1])
			{
				local_error = "high";
				limit = UpperLimits[1];
				no_error = false;
			}
			else if (curValue < LowerLimits[1])
			{
				local_error = "low";
				limit = LowerLimits[1];
				no_error = false;
			}
		}

		i++;
	}

	if (!no_error)
	{
		sprintf(buf,"Parameter \"%s = %f\", is \"%s\", limit is \"%f\"!",temp,curValue,local_error,limit);
		UserErrorMessage("3",buf, error_msg);

		return 0x80040E21;
	}

	return S_OK;
}
/********************************************************************
FUNCTION:
	LinearChiSquared
_____________________________________________________________________
DESCRIPTION:
	Calculate Chi-squared statistic for calibration with linear algorithm.

	#include <VFstd.h> 
	float VD_chi2( fVector X, fVector Y, fVector InvVar, ui size );

	chi^2 = sum( 1/si^2 * (Xi - Yi)^2 ). 
	The chi^2 (chi-square) merit function is most often used to judge the 
	quality of a fit. 
	One vector (either X or Y) represents experimental values that are 
	obtained with individual standard deviations sigmai, 
	the other contains the values predicted on the basis of a theory or a model. 
	If the experimental data are "normally" distributed, and if s (sigma) 
	is the correct measure for the quality of these data, then chi^2 is a measure 
	for the quality of the fit. Rather than the standard deviations themselves, 
	the routine needs the inverse of their squares 
	(i.e., the inverse of the variances) to be passed as the vector InvVar.

_____________________________________________________________________
GLOBAL VARIABLES:
	LinearParams - Current fit parameters.
____________________________________________________________________
LOCAL VARIABLES:
	fitParameters - Fit parameters in MFC format.
	calibratorData - Assigned values in MFC format.
	calcAssignedValues - Calculated assigned values in MFC format.
	vCalculatedAssignedValues - Calculated assigned values in OptiVec format.

____________________________________________________________________
PARAMETERS:
	X - Assigned values.
	Y - Calculated assigned values.
	W - Weights.
	s - size of vectors, X,Y and W.
____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Chi-squred statistic for linear calibration.

********************************************************************/
double LinearChiSquared(dVector LinearParams, 
						dVector X, 
						dVector Y, 
						dVector W, 
						ui s )
{
	CArray<double,double> fitParameters;
	CArray<double,double> calibratorData;
	CArray<double,double> calcAssignedValues;
	dVector vCalculatedAssignedValues;

	vCalculatedAssignedValues = VD_vector( s );

	fitParameters.Add(LinearParams[Slope]);
	fitParameters.Add(LinearParams[Intercept]);

	for (int i=0; i<s; i++)
	{
		calibratorData.Add(Y[i]);
	}

	LinearCalculateAssignedValues(&fitParameters,
								  &calibratorData,
								  &calcAssignedValues);

	for (i=0; i<s; i++)
	{
		vCalculatedAssignedValues[i] = calcAssignedValues.GetAt(i);
	}

	// X - Assigned values.
	// Y - Calculated assigned values.
	// W - Weights.
	// s - size of vectors, X,Y and W.
	return VD_chi2( X, vCalculatedAssignedValues, W, s );
}

/********************************************************************
FUNCTION:
	LinearCalibration
_____________________________________________________________________
DESCRIPTION:
	Process calibration with a linear algorithm.
_____________________________________________________________________
GLOBAL VARIABLES:
	FitParameters - Calculated fit parameters
	Covar - Covariance matrix
	FitParameterStatus - Status of fit parameters.
	numberOfFitParameters - Number of fit parameters.
	XExp - assigned values
	YExp - analyzer data.
	Weights - For weighted calculations.
	sizex - Number of calibration values.
							  
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	buf - Error buffer.
____________________________________________________________________
PARAMETERS:
	calibrationID - Calibration identifier.
	newFitPars - Current fit parameters.
	algorithm - Calibration algorithm.
	chisquared - Chi squared statistic.
	error_msg - Error reporting.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Process linear calibrations.

********************************************************************/
HRESULT LinearCalibration(LONG		calibrationID,
						  VARIANT	*newFitPars,
						  CString	algorithm,
						  double	*chisquared,
						  VARIANT	*error_msg)
{
	HRESULT		hr;
	char		buf[512];

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
		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"GetFitParsLimits\"",buf);
				
		return hr;
	}

	hr = LinearFitCalibration(calibrationID,
							  algorithm,
							  FitParameters,		// A
							  Covar,				// Covariance
							  FitParameterStatus,	// Status
							  numberOfFitParameters,
							  XExp,					// X - assigned values.
							  YExp,					// Y - analyzer data.
							  Weights,				// InvVar
							  sizex,				// sizex
							  chisquared,
							  error_msg);

	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,	"Return from function \"LinearCalibration\"", buf);
		return hr;
	}

	*chisquared = LinearChiSquared(FitParameters, XExp, YExp, Weights, sizex );

	return S_OK;
}

/********************************************************************
FUNCTION:
	LinearCalculations
_____________________________________________________________________
DESCRIPTION:
	Calculations for linear algorithms.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - return status code.
	chemistry - Chemistry name.
	mode - Calibration mode.
	buf - Error buffer.
____________________________________________________________________
PARAMETERS:
	calibrationID - Calibration identifier.
	algorithm - Calibration algorithm.
	chisquared - Chi-squared statistic.
	newFitParameters - New values for fit parameters.
	error_msg - Error reporting.
____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	07/21/04	Alves		Calculations for linear calibrations.

********************************************************************/
HRESULT LinearCalculations(LONG calibrationID,
						   CString algorithm,
						   double *chisquared,
						   VARIANT *newFitParameters,
						   VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	CString chemistry;
	CString mode;
	char	buf[512];

	// < ----- IF FIT PARAMETERS HAVE CHANGED ----- >
	// Is this calculation for updated fit parameters?
	if (newFitParameters->vt != VT_EMPTY)
	{
		// Parse new fit parameter array.
		hr = EvaluateLinearFitParameters(newFitParameters, error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,"Return from function \"EvaluateLinearFitParameters\"",buf);
			}

			return hr;
		}
	}
	// </ ----- IF FIT PARAMETERS HAVE CHANGED ----- >

	// < ----- LINEAR CALIBRATION CALCULATIONS ----- >
	hr = LinearCalibration(calibrationID,
						   newFitParameters,
						   algorithm,
						   chisquared,
						   error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,	"Return from function \"LinearCalibration\"", buf);
		return hr;
	}
	// </ ----- LINEAR CALIBRATION CALCULATIONS ----- >

	return hr;
}
