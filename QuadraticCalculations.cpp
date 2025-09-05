/*******************************************************************

FILENAME:	QuadraticCalculations.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for quadratic calculations.

--------------------------------------------------------------------
FUNCTIONS called from this module:

	QuadraticCalculations
	QuadraticChiSquared
	QuadraticCalculateAssignedValues
	EvaluateQuadraticFitParameters
	InsertQuadraticFitParameters
	QuadraticRatesFromAssignedValues
	QuadraticCorrelationCoefficient

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	QuadraticCalculations.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/

#include "stdafx.h"
#include <math.h>
#include <MDstd.h>
#include "Loging.h"
#include "globals.h"
#include "QuadraticCalculations.h"
#include "dboInsertQuadraticFitParameters.H"
#include "DbAccess.h"
#include "Support.h"
#include "CalibrationCalculations.h"
#include "AssignedValues.h"

/****************************************************************
HRESULT QuadraticCalculations(LONG calibrationID,
							  CString algorithm,
							  double *chisquared,
							  VARIANT *newFitParameters,
							  VARIANT *error_msg)

	Linear fit calculations.

-----------------------------------------------------------------
 INPUT														
	X - Assigned values.
	Y - Calibration data.
	sizeXY - Size of X and Y vectors.
	FitParams - Fit parameters.
	W - Weights.
	s - Size of vectors.
calibrationID - Identifier for current calibration.
algorithm - Algorithm for current calibration.
chisquared - Chi-squared statistic.
newFitParameters - New fit parameter values.
error_msg - Error messages.

-----------------------------------------------------------------
 OUTPUT
	Chi-squared statistic.
	
-----------------------------------------------------------------
 LOCAL

  	fitParameters - Fit parameter values, double format.
	calibratorData - Calibration data, double format.
	calcAssignedValues - Calculated assigned values, double format.
	vCalculatedAssignedValues - Vector calculated assigned values.
	i - Index for calibration data.

----------------------------------------------------------------
 RETURN														
	hr - Results code.

****************************************************************/
HRESULT QuadraticCalculations(LONG calibrationID,
							  CString algorithm,
							  double *chisquared,
							  VARIANT *newFitParameters,
							  VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	char	buf[512];

	// Is this calculation for updated fit parameters?
	if (newFitParameters->vt != VT_EMPTY)
	{
		// Parse new fit parameter array.
		hr = EvaluateQuadraticFitParameters(newFitParameters, error_msg);
		if (FAILED(hr))
		{
			if (newFitParameters->vt == VT_EMPTY)
			{
				sprintf(buf,"Function \"QuadraticCalculations\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr,"Return from function \"EvaluateQuadraticFitParameters\"",buf);
			}
			return hr;
		}
	}
	else
	{
		/*
		FUNCTION DEFINITION:
		void VF_polyfitwW( fVector A, fMatrix Covar, unsigned deg, 
		fVector X, fVector Y, fVector InvVar, ui sizex );

		YExp - rates. XExp - assigned values.
		Find fit with assigned values in the Y and rates in X.
		X and Y vectors as related to definition above.
		Calibration requires that the Rates be in X vector and Assigned be in Y vector.
		VD_polyfitwW(FitParameters,Covar,numberOfFitParameters,YExp,XExp,Weights,sizex); 
		*/

		// YExp - rates. XExp - assigned values.
		// Find fit with assigned values in the Y and rates in X.
		VD_polyfitwW(FitParameters,Covar,numberOfFitParameters,YExp,XExp,Weights,sizex); 
	}

	// Must be after FitParameters have been calculated.
	*chisquared = QuadraticChiSquared(FitParameters,XExp,YExp,Weights,sizex);

	return hr;
}

/****************************************************************
double QuadraticChiSquared(dVector FitParams,
						   dVector X,
						   dVector Y,
						   dVector W,
						   ui s )

	Calculate Chi-squared statistic for calibrations with quadratic algorithm.

-----------------------------------------------------------------
 INPUT														
	X - Assigned values.
	Y - Calibration data.
	sizeXY - Size of X and Y vectors.
	FitParams - Fit parameters.
	W - Weights.
	s - Size of vectors.

-----------------------------------------------------------------
 OUTPUT
	Chi-squared statistic.
	
-----------------------------------------------------------------
 LOCAL

  	fitParameters - Fit parameter values, double format.
	calibratorData - Calibration data, double format.
	calcAssignedValues - Calculated assigned values, double format.
	vCalculatedAssignedValues - Calculated assigned values, vector.
	i - Index for calibration data.

----------------------------------------------------------------
 RETURN														
	Correlation coefficient.

****************************************************************/
double QuadraticChiSquared(dVector FitParams,
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

	fitParameters.Add(FitParams[0]);
	fitParameters.Add(FitParams[1]);
	fitParameters.Add(FitParams[2]);

	for (int i=0; i<s; i++)
	{
		calibratorData.Add(Y[i]);
	}

	QuadraticCalculateAssignedValues(&fitParameters,
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

/****************************************************************
void QuadraticCalculateAssignedValues(CArray<double,double> *fitParameters,
									  CArray<double,double> *calibratorData,
									  CArray<double,double> *calcAssignedValues)

	Calculate assigned values using the rates, 
		i.e. data from analyzer.

	To plot with the assigned values. 
	How close are the calculated assigned values 
	to the given assigned values.

	y = a1*x^2 + a2*x + a3
			OR
	y = a3 + a2*x + a1*x^2

	y - Assigned values
	x - Rates

	Given: rates
	 Find: Assigned values

	a3 = fitParameters[0]	Constant value.
	a2 = fitParameters[1]	x multiplier.
	a1 = fitParameters[2]	x^2 multiplier.

-----------------------------------------------------------------
 INPUT
	fitParameters - Fit parameters.
	calibratorData - Calibration data, from analyzer.
	calcAssignedValues - Calculated assigned values.

-----------------------------------------------------------------
 OUTPUT
	Calculated assigned values.

-----------------------------------------------------------------
 LOCAL
	k - Calibration data index.
	y - Calculation results.
	
----------------------------------------------------------------
 RETURN														
	None.

****************************************************************/
void QuadraticCalculateAssignedValues(CArray<double,double> *fitParameters,
									  CArray<double,double> *calibratorData,
									  CArray<double,double> *calcAssignedValues)
{
    double	y;

	calcAssignedValues->RemoveAll();

	for (int k=0; k<calibratorData->GetSize(); k++)
	{
		y = fitParameters->GetAt(2)*calibratorData->GetAt(k)*calibratorData->GetAt(k) +
			fitParameters->GetAt(1)*calibratorData->GetAt(k)+
			fitParameters->GetAt(0);

		calcAssignedValues->Add(y);
	}
}


/****************************************************************
HRESULT EvaluateQuadraticFitParameters(VARIANT *newFitPars,
									   VARIANT *error_msg) 

	Test new fit parameters against values in the database.

-----------------------------------------------------------------
 INPUT
	*newFitPars - New fit parameter values.
	*error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT
	FitParameters global variable with new fit parameter values.

-----------------------------------------------------------------
 LOCAL
	hr - Result code.
	lLCID - Conversion language code.
	no_error - Current fit parameter status.
	buf[512] - Error message.
	curValue - Current double value for fit parameter.
	limit - Current limit.
	bstr - Binary string from VARIANT.
	i - VARIANT variable index.
	temp - Converted new fit parameter VARIANT value.
	local_error - Test status string.

----------------------------------------------------------------
 RETURN														
	hr - Error code.

****************************************************************/
HRESULT EvaluateQuadraticFitParameters(VARIANT *newFitPars,
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
			Covar[Slope][Slope] = 0.0;

			// Set status of fit parameter "Slope" to fixed.
//			FitParameterStatus[0] = 0;

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
		else if (temp.CompareNoCase("ANCHOR") == 0)
		{
			FitParameters[1] = curValue;
			Covar[Anchor][Anchor] = 0.0;

			// Set status of fit parameter "Anchor" to fixed.
//			FitParameterStatus[1] = 0;

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
		else if (temp.CompareNoCase("CURVATURE") == 0)
		{
			FitParameters[2] = curValue;
			Covar[Curvature][Curvature] = 0.0;

			// Set status of fit parameter "Curvature" to fixed.
//			FitParameterStatus[2] = 0;

			if (curValue > UpperLimits[2])
			{
				local_error = "high";
				limit = UpperLimits[2];
				no_error = false;
			}
			else if (curValue < LowerLimits[2])
			{
				local_error = "low";
				limit = LowerLimits[2];
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

/****************************************************************
HRESULT InsertQuadraticFitParameters(LONG calibrationID,
									 dVector FitPars,
									 double ccoe,
									 double slopeError,
									 double anchorError,
									 double curvatureError,
									 double chi_squared,
									 double bartlett)

	Put quadratic fit parameters in the database.

-----------------------------------------------------------------
 INPUT														
	calibrationID - Identifier for current calibration.
	FitPars - Calculated fitparameters.
	ccoe - Correlation coefficient.
	slopeError - Error in slope.
	anchorError - Error in anchor.
	curvatureError - Error in curvature.
	chi_squared - Chi-squared statistic.
	bartlett - Homogeneity value.

-----------------------------------------------------------------
 OUTPUT
	Quadratic fit parameters added to the database.

-----------------------------------------------------------------
 LOCAL
	hr - Result code.
	db - Database access.
	session - Database session structure.

----------------------------------------------------------------
 RETURN														
	hr - Error code.

****************************************************************/
HRESULT InsertQuadraticFitParameters(LONG calibrationID,
									 dVector FitPars,
									 double ccoe,
									 double slopeError,
									 double anchorError,
									 double curvatureError,
									 double chi_squared,
									 double bartlett)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to insert linear fit parameters into the database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboInsertQuadraticFitParametersAccessor> > cmdInsertQuadraticFitParameters;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "InsertQuadraticFitParameters" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","InsertQuadraticFitParameters");
		return hr;
	}

	hr = cmdInsertQuadraticFitParameters.Create (session, _T("{ ? = CALL dbo.InsertQuadraticFitParameters;1 (?,?,?,?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for stored procedure \"InsertQuadraticFitParameters\"","InsertLinearFitParameters");
		return hr;
	}

	// Set data to add to the database.
	cmdInsertQuadraticFitParameters.m_calibrationID = calibrationID;
	cmdInsertQuadraticFitParameters.m_CorrelationCoefficient = ccoe;
//	cmdInsertQuadraticFitParameters.m_Slope = FitPars[Slope];
	cmdInsertQuadraticFitParameters.m_Slope = FitPars[Curvature];
	cmdInsertQuadraticFitParameters.m_Anchor = FitPars[Anchor];
//	cmdInsertQuadraticFitParameters.m_Curvature = FitPars[Curvature];
	cmdInsertQuadraticFitParameters.m_Curvature = FitPars[Slope];
	cmdInsertQuadraticFitParameters.m_SlopeStdError = slopeError;
	cmdInsertQuadraticFitParameters.m_AnchorStdError = anchorError;
	cmdInsertQuadraticFitParameters.m_CurvatureStdError = curvatureError;
	cmdInsertQuadraticFitParameters.m_ChiSquared = chi_squared;
	cmdInsertQuadraticFitParameters.m_BartlettTest = bartlett;

	hr = cmdInsertQuadraticFitParameters.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"InsertQuadraticFitParameters\"","InsertLinearFitParameters");
		return hr;
	}

	cmdInsertQuadraticFitParameters.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/****************************************************************
HRESULT QuadraticRatesFromAssignedValues(LONG calibrationID,
										 dVector fitParameters,
										 VARIANT *rates,
										 VARIANT *error_msg)

	Calculate rates from assigned values.
	Get assigned values.
	Calculate rates.
	Put rates in return array.

	y = assigned values
	x = rates
	y = a1*x^2 + a2*x + a3
			OR
	y = a1 + a2*x + a3*x^2

	Given: Assigned values
	Solve: rates

	Solve for x with quadratic formula:
		y = a1*x^2 + a2*x + a3
	y - y = a1*x^2 + a2*x + a3 - y
		0 = a1*x^2 + a2*x + a3 - y
		c = a3 - y
		0 = a1*x^2 + a2*x + c

	x = -a2 +/- sqrt(a2^2 - 4*a1*a3)
		----------------------------
					2*a1

	If 4*a1*a3 > a2^2 both roots are complex.

	Use:
	q = -1 * (a2 +  a2 sqrt(a2^2 - 4*a1*a3))
		--		   ---
		 2		   |a2|

	roots:
		q	   a3
		-- and --
		a1      q

	a3 = fitParameters[0]	Constant
	a2 = fitParameters[1]	x multiplier
	a1 = fitParameters[2]	x^2 multiplier

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
HRESULT QuadraticRatesFromAssignedValues(LONG calibrationID,
										 dVector fitParameters,
										 VARIANT *rates,
										 VARIANT *error_msg)
{
	HRESULT					hr;
	BSTR					bstr = NULL;
	long					numberOfRates;
	long					i=0;
	double					q,four_x_a1a3,a2_squared,a3,a2_div_absa2;
	CArray<double,double>	assignedValues;
	CArray<double,double>	l_rates;
	CArray<int,int>			sampleSizesNoOutliers;
	CStringArray			a_sampleIdList;
	char					buf[128];
	CString					tmp;

	//*** define the safearray ***
    SAFEARRAY FAR* psa_Rates = NULL;	// assigned rates.

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function","QuadraticRatesFromAssignedValues");
		return hr;
	}

	hr = RemoveCONTROLData(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"RemoveCONTROLData\" function","QuadraticRatesFromAssignedValues");
		return hr;
	}

	hr = GetSampleAssignedValues(calibrationID, &a_sampleIdList, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetSampleAssignedValues\" function","QuadraticRatesFromAssignedValues");
		return hr;
	}

	// < ----- CALCULATE RATES FROM ASSIGNED VALUES ----- >
	for (int k=0; k<a_sampleIdList.GetSize(); k++)
	{
		// a3 = a3 - y
		a3 = fitParameters[0] - assignedValues.GetAt(k);

		// 4*a*c = 4*a1*a3
		four_x_a1a3 = 4*fitParameters[2] * a3;

		a2_div_absa2 = fitParameters[1]/fabs(fitParameters[1]);
		a2_squared = fitParameters[1] * fitParameters[1];

		// 4*a*c > b^2 then the roots are imaginary.
		// None real numbers here don't make sence.
		if (four_x_a1a3 < a2_squared)
		{
			q = -0.5 * (fitParameters[1] + a2_div_absa2 * sqrt(a2_squared - four_x_a1a3));
			l_rates.Add(a3/q);
		}
	}
	// </ ----- CALCULATE RATES FROM ASSIGNED VALUES ----- >

	numberOfRates = l_rates.GetSize();

	// Set size of array to pass to client.
	sabound[0].cElements = 2 * numberOfRates;
	sabound[0].lLbound = 0;

	//*** create the safearray to return ***
	psa_Rates = SafeArrayCreate(VT_BSTR, 1, sabound);

	// < ----- PUT CALCULATED RATES IN VARIANT TO RETURN ----- >
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
	// </ ----- PUT CALCULATED RATES IN VARIANT TO RETURN ----- >

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
double QuadraticCorrelationCoefficient(dVector X, 
									   dVector Y,
									   ui sizeYX)

	Calculate correlation coefficient for calibrations with quadratic algorithm.

	R^2 = Sxy^2 /(Sxx * Syy)

	Sxy = Sum((X[i]-mean(X))*(Y[i]-mean(Y)))
	Sxx = Sum((X[i]-mean(X))^2)
	Sxy = Sum((Y[i]-mean(Y))^2)

	From: Applied Regression Analysis, Third Edition
	Norman R. Draper and Harry Smith.
	Pages 25 and 33.

-----------------------------------------------------------------
 INPUT														
	X - Assigned values.
	Y - Calibration data.
	sizeXY - Size of X and Y vectors.

-----------------------------------------------------------------
 OUTPUT
	
-----------------------------------------------------------------
 LOCAL
	meanX - Average of the X, assigned values., vector.
	meanY - Average of the Y, calibration values., vector.
	Sxx - Corrected sum of squares of the X's.
	Syy - Corrected sum of squares of the Y's.
	Sxy - Corrected sum of products of X and Y.
	SintermediateY - Intermediate results for Syy.
	SintermediateX - Intermediate results for Sxx.
	SintermediateXY - Intermediate results for XY.

----------------------------------------------------------------
 RETURN														
	Correlation coefficient.

****************************************************************/
double QuadraticCorrelationCoefficient(dVector X, 
									   dVector Y,
									   ui sizeYX)
{
	double	meanX;
	double	meanY;
	double	Sxx;
	double	Syy;
	double	Sxy;
	dVector	SintermediateY;
	dVector	SintermediateX;
	dVector	SintermediateXY;

	meanX = VD_mean(X,sizeYX);
	meanY = VD_mean(Y,sizeYX);

	SintermediateY = VD_vector(sizeYX);
	SintermediateX = VD_vector(sizeYX);
	SintermediateXY = VD_vector(sizeYX);

	// Initialize a vector with zero.
	VD_equ0( SintermediateY, sizeYX );
	// Subtract average from each experimental data point.
	// SintermediateY = Y[i] - mean(Y)
	VD_subC( SintermediateY, Y, sizeYX, meanY);

	// Initialize a vector with zero.
	VD_equ0( SintermediateX, sizeYX );
	// Subtract average from each experimental data point.
	// SintermediateX = X[i] - mean(X)
	VD_subC( SintermediateX, X, sizeYX, meanX);

	// Multiply corresponding vector elements
	// normal versions: Zi = Xi * Yi 
	VD_mulV( SintermediateXY, SintermediateX, SintermediateY, sizeYX); 

	// Sxy = Sum((X[i] - mean(X))(Y[i] - mean(Y)))
	Sxy = VD_sum( SintermediateXY, sizeYX );

	// SintermediateXY = ((X[i] - mean(X))^2
	VD_square( SintermediateXY, SintermediateX, sizeYX );

	// Sxx = Sum(((X[i] - mean(X))^2)
	Sxx = VD_sum(SintermediateXY, sizeYX);

	// SintermediateXY = ((Y[i] - mean(Y))^2
	VD_square( SintermediateXY, SintermediateY, sizeYX );

	// Syy = Sum(((Y[i] - mean(Y))^2)
	Syy = VD_sum(SintermediateXY, sizeYX);

	V_free(SintermediateY);
	V_free(SintermediateX);
	V_free(SintermediateXY);

	// R^2 =  (Sxy)^2
	//       ----------
	//       (Sxx)(Syy)

	return ((Sxy * Sxy) / (Sxx * Syy));
}