/*******************************************************************

FILENAME:	BestFitMethods.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for best fit methods processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	BestFitMethods

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BestFitMethods.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <math.h>

#include "Utils.h"
#include "Loging.h"
#include "Support.h"
#include "globals.h"
#include "NewFitParameters.h"
#include "LinearCalibration.h"
#include "QuadraticCalculations.h"
#include "Controls.h"
#include "AssignedValues.h"

/*************************************************************
STDMETHODIMP QalibServerMain::BestFitMethods(
				LONG calibrationID,
				BSTR algorithmType, 
				double chisquared,
				VARIANT *error_msg)

- Best fit method calculations.

INPUT
	calibrationID - ID of calibration to find best fit methods.
	algorithmType - Regression algorithm. LINEAR, SIGMOIDAL, etc.
	chisquared - Chi-squared best fit test.
	error_msg - Error reporting.

OUTPUT														
	Best fit methods results in the database.

LOCAL
	pos - Index to traverse sample list.
	plotList - List of sample to plot.
	plotData - Data to plot.
	var_AssignedValues - Assigned values in a VARIANT.
	vec_AssignedValues - Vector of assigned values.
	curTest - Current test.
	searchFor - Test to search for.
	var_calcAssignedValues - Calculated assigned values, VARIANT format.
	vec_calcAssignedValues - Vector with calculated assigned values.
	bstr - String to hold temporary values.
	chiSquaredFlag - Flag to decide if current chemistry requires chi squared test.
	T - Bartllet's test.
	ccoe - Correlation coefficient.
	algorithmType - Which calculations to perform.
	sizeallplotData - Find number of plot points.
	sampleType - Type of sample to plot.
	plotSampleSizes - Size of samples to plot.

GLOBALS														
	LowerLimits - Lower limits for FitParameters.						
	Opt - Options structure for nonlinear function.

FUNCTIONS													
	VariantLength - Calculate the size of a VARIANT
	LoadVariantToVector - Load plot data to vector.
	GetAssignedValues - Get assigned values for current plot data.
	LoadVariantToVector - Load assigned values to vector.
	CalculateAssignedValues - Calculate assigned values
	VD_linregress - Linear regression
	VD_corrcoeff - Correlation coefficient of sample to plot.
	PutCalculatedAssignedValuesInDb - Put calculated assigned values in database.
	CorrelationCoefficient - Correlation coefficient.
	BartlettsTest - Bartlett's fit test.
*************************************************************/
HRESULT BestFitMethods(LONG calibrationID,
					   BSTR algorithmType,
					   double chisquared,
					   VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT					hr;
	POSITION				pos;
	CStringArray			plotList;
	CArray<double,double>	plotData;
	CArray<double,double>	assignedValues;
	CArray<double,double>	calcAssignedValues;
	CArray<double,double>	fitParameters;
	dVector					vec_AssignedValues,
							vec_calcAssignedValues;
	ui						sizeallplotData;
	CString					curTest,
							sampleType,
							tmpAlgorithmType;
	BSTR					bstr=NULL;
	CArray<int,int>			plotSampleSizes;
	char					buf[256];
	int						numberOfPlotSamples;
	CString					chemistry;
	CStringArray			businessrules;
	CArray<double,double>	ruleValues;
	CStringList				testList;

	// Initilize fitness test values.
	// To distinguish between needed and not needed tests.
	bool	chiSquaredFlag = false;
	double	T = -1.0;
	ccoe = -1.0;
	tmpAlgorithmType = algorithmType;

	// Get goodness of fit tests list from db.
	hr = GoodnessTests( calibrationID, testList );
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Can't find list of goodness tests", error_msg);
		sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"GoodnessTests\"", buf);
		return hr;
	}

	// List of tests for current chemistry.
	pos = testList.GetHeadPosition();

	// Linear regression for current plot data.
	// assignedCoef must have 5 cells.
	// Required by VD_linregress, see below.
	assignedCoef = VD_vector( 5 );

	// Traverse test list.
	// Perform tests indicated in list, testList.
	while (pos != NULL)
	{
		curTest = testList.GetNext(pos);

		if ((curTest.CompareNoCase("PLOT_CALIBRATORS") == 0) ||
			(curTest.CompareNoCase("PLOT_CONTROLS") == 0))
		{
			if (curTest.CompareNoCase("PLOT_CALIBRATORS") == 0)
			{
				sampleType = "CALIBRATOR";
			}
			else if (curTest.CompareNoCase("PLOT_CONTROLS") == 0)
			{
				sampleType = "CONTROL";

				// Test control business rules.
				numberOfPlotSamples = TestCurrentControls(calibrationID);
				if (numberOfPlotSamples < MIN_SAMPLES_TO_PLOT)
				{
					sampleType = "NONE";
				}
			}

			// Test current type. Calibration may or may not have
			// all the types contained in the db.
			if (TestCurrentType(calibrationID, sampleType))
			{
				// Clear sample sizes array.
				plotSampleSizes.RemoveAll();
				plotList.RemoveAll();
				plotData.RemoveAll();
				assignedValues.RemoveAll();
				plotSampleSizes.RemoveAll();
				calcAssignedValues.RemoveAll();

				hr = GetPlotData(calibrationID, sampleType, 
								 &plotList, &plotData, &plotSampleSizes);
				if (FAILED(hr))
				{
					sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr, "Return from \"GetPlotData\" function",	buf);
					return S_OK;
				}

				numberOfPlotSamples = plotList.GetSize();

				// Get assigned values for current plot data.
				// 1-to-1 with sample id. i.e. each pool to plot, has 1 assigned value.
				hr = GetAssignedValuesToPlot(calibrationID, 
											 &plotList, 
											 &assignedValues, 
											 &plotSampleSizes,
											 error_msg);
				if (FAILED(hr))
				{
					sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"GetAssignedValuesToPlot\" function",buf);
					return hr;
				}

				// < ----- SET FIT PARAMETERS ----- >
				// Copy fit parameters from a vector format to CArray format.
				SetFitParameters(&fitParameters,
								 FitParameters,
								 (CString)algorithmType);
				// </ ----- SET FIT PARAMETERS ----- >

				// Calculate assigned values
				hr = CalculateAssignedValues(&fitParameters,
											 &plotData, 
											 &assignedValues, 
											 &calcAssignedValues,
											 (CString)algorithmType);
				if (FAILED(hr))
				{
					sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"CalculateAssignedValues\" function",buf);
					return hr;
				}

				sizeallplotData = assignedValues.GetSize();

				// Allocate memory for current assigned values.
				vec_AssignedValues	= VD_vector(sizeallplotData);

				// Allocate memory for current calculated assigned values.
				vec_calcAssignedValues = VD_vector(sizeallplotData);

				// Load assigned values to vector.
				LoadCArrayToVector(&assignedValues, vec_AssignedValues);

				// Load calculated assigned values to vector.
				LoadCArrayToVector(&calcAssignedValues, vec_calcAssignedValues);

				/*
				void VF_linregress( fVector Param, fVector X, fVector Y, ui size );
				The X-Y data are fitted to a straight line y = ax + b; 
				the parameters a and b are determined in such a way that the quantity 
				h2 = sum( (Y[i] - y(x=X[i]))^2 ) is minimized.
				Uncertainties da and db are also determined.
				On output, Param is filled with the 5 elements {a, da, b, db, h2}
				in the order indicated.
				*/
				VD_linregress( assignedCoef, 
							   vec_AssignedValues,
							   vec_calcAssignedValues,
							   sizeallplotData );

				// Correlation coefficient of samples to plot.
				ccoe = VD_corrcoeff(vec_AssignedValues, vec_calcAssignedValues, sizeallplotData, 
									VD_mean( vec_AssignedValues, sizeallplotData ), 
									VD_mean( vec_calcAssignedValues, sizeallplotData ) );

				// Put calculated assigned values in database.
				hr = PutCalculatedAssignedValuesInDb(calibrationID,
													 sampleType,
													 assignedCoef,
													 ccoe,
													 vec_AssignedValues,
													 vec_calcAssignedValues,
													 sizeallplotData,
													 error_msg);
				if (FAILED(hr))
				{
					sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"PutCalculatedAssignedValuesInDb\" function",buf);
					return hr;
				}

				// De-allocate vector memory.
				V_free(vec_AssignedValues);
				V_free(vec_calcAssignedValues);
			}
		}
		else if(curTest.CompareNoCase("CORRELATION") == 0)
		{
			// Correlation coefficient.
			hr = CorrelationCoefficient(XExp,
										YExp, 
										sizex,
										(CString)algorithmType,
										&ccoe,
										error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with correlation coefficient calculations.", error_msg);
					sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"CorrelationCoefficient\" function",buf);
				}
				return hr;
			}
		}
		else if(curTest.CompareNoCase("HOMOGENEITY") == 0)
		{
			// Bartlett's fit test.
			// YExp			- current experimental data.
			// sizex		- number of current experimental values.
			// PoolSizes	- size of each pool.
			// poolsizelen	- number of pools.
			// Find number of pools.

			T = BartlettsTest(YExp, sizex, PoolSizes, numberofsamples);
		}
		else if(curTest.CompareNoCase("CHI_SQUARED") == 0)
		{
			// Set chi-squared flag. i.e. Keep calculated chi-squared.
			chiSquaredFlag = true;
		}
		else
		{
			sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
			// 0x80040E21  -2147217887  Errors occurred
			LogErrorMessage(0x80040E21,"Unknown test",buf);
			return hr;
		}
	}

	// Chi-squared is given by default.
	// Need test if current chemistry does not use chi-squared value.
	if (!chiSquaredFlag)
	{
		chisquared = -2.0;
	}

	CString	tempAlg = algorithmType;

	// Add current results to database. Depend on algorithm type.
	if (tempAlg.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = InsertSigmoidalFitParameters(calibrationID, FitParameters, numberOfFitParameters, ccoe, chisquared, T,
				sqrt(Covar[Top][Top]), sqrt(Covar[Bottom][Bottom]),
				sqrt(Covar[Slope][Slope]),sqrt(Covar[Intercept][Intercept]));

		if (FAILED(hr))
		{
			sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"InsertSigmoidalFitParameters\"",buf);
			return hr;
		}
	}
	else if ((tempAlg.CompareNoCase("LINEAR") == 0) ||
			 (tempAlg.CompareNoCase("FIXEDSLOPE") == 0))
	{
		hr = InsertLinearFitParameters(calibrationID, 
									   FitParameters, 
									   ccoe, 
									   sqrt(fabs(Covar[Slope][Slope])),
									   sqrt(fabs(Covar[Intercept][Intercept])),
									   chisquared,
									   T);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"InsertLinearFitParameters\"",buf);
			return hr;
		}
	}
	else if (tempAlg.CompareNoCase("QUADRATIC") == 0)
	{
		hr = InsertQuadraticFitParameters(calibrationID,
										  FitParameters,
										  ccoe,
										  sqrt(fabs(Covar[Curvature][Curvature])),
										  sqrt(fabs(Covar[Anchor][Anchor])),
										  sqrt(fabs(Covar[Slope][Slope])),
										  chisquared,
										  T);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"InsertQuadraticFitParameters\"",buf);
			return hr;
		}
	}
	else if (tempAlg.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = InsertLinearFitParameters(calibrationID, 
									   FitParameters, 
									   ccoe, 
									   sqrt(fabs(Covar[Slope][Slope])),
									   sqrt(fabs(Covar[Intercept][Intercept])),
									   chisquared,
									   T);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"InsertLinearFitParameters\"",buf);
			return hr;
		}
	}
	else if (tempAlg.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
		// 0x80040E21  -2147217887  Errors occurred
		hr = 0x80040E21;
		sprintf(buf,"Function \"BestFitMethods\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Unknown algorithm", buf);
		return hr;
	}

	V_free(assignedCoef);

	return S_OK;
}
