/*******************************************************************

FILENAME: Support.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Function to support calibration processing.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------

COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	Support.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>

#include "DbAccess.h"

#include "Loging.h"
#include "Support.h"
#include "globals.h"
#include "HelperFunctions.h"
#include "derivatives.h"

#include "ErrorProcessing.h"
#include "SigmoidalCalculations.h"
#include "QuadraticCalculations.h"
#include "CalibrationCalculations.h"
#include "ExponentialCalculations.h"
#include "LinearCalibration.h"

#include "Utils.h"
#include "QalibConstants.h"
#include "AssignedValues.h"

#include "dboGetWeightMethod.H"
#include "dboGetGoodnessOfFitTests.H"
#include "dboGetAssignedFitParameters.H"
#include "dboGetCalibrationAlgorithm.H"
#include "dboInsertSigmoidalFitParameters.H"
#include "dboGetPlotSampleSizes.H"
#include "dboGetListOfSamplesToPlot.H"
#include "dboGetPlotData.H"
#include "dboGetAssignedValuesToPlot.H"
#include "dboInsertRecoveredValuesFromRates.H"
#include "dboGetSampleNames.H"
#include "dboGetPartNumber.H"
#include "dboGetSampleSizes.H"
#include "dboGetPartNumberFromChemistryName.H"
#include "dboGetChemistryNameFromPartNumber.H"
#include "dboGetDiluent.H"
#include "dboGetSampleIdsWithType.H"
#include "dboGetChemistryName.H"
#include "dboGetListOfRequiredSamples.H"
#include "dboGetSampleStatus.H"

//QalibConstants currentConstants;
extern QalibConstants currentConstants;

/****************************************************************
HRESULT CorrelationCoefficient(dVector X, 
							   dVector Y, 
							   ui sizeYX,
							   CString algorithm,
							   double *ccoe,
							   VARIANT *error_msg)

-----------------------------------------------------------------
 INPUT
	Y		- Y data values vector.
	X		- X data values vector.
	sizeYX	- Size of input data arrays.
	algorithm - Algorithm of current chemistry.
	*ccoe - Return calculated correlation coefficient.
	*error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT
	None.

-----------------------------------------------------------------
 LOCAL
	hr - Return code.
	buf - Error message.
-----------------------------------------------------------------
 RETURN
	HRESULT code.
****************************************************************/
HRESULT CorrelationCoefficient(dVector X, 
							   dVector Y, 
							   ui sizeYX,
							   CString algorithm,
							   double *ccoe,
							   VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	char	buf[512];

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		*ccoe = SigmoidalCorrelationCoefficient(X, Y, sizeYX);
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		*ccoe = LinearCorrelationCoefficient(X,	Y, sizeYX);
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		*ccoe = QuadraticCorrelationCoefficient(X, Y, sizeYX);
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		*ccoe = ExponentialCorrelationCoefficient(X, Y, sizeYX);
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
		hr = 0x80040E21;
		sprintf(buf,"Function \"CorrelationCoefficient\", file %s, line %d", __FILE__, __LINE__);
		// 0x80040E21  -2147217887  Errors occurred
		LogErrorMessage(hr, "Unknown algorithm", buf);
	}

	return hr;
}

/****************************************************************
HRESULT GetWeightMethodDB(CString calibrationDate, 
						  CString &wMethod)

 Get weight method, for current chemistry, from the database.	
--------------------------------------------------------------
 INPUT
	calibrationDate - Calibration date.
	&wMethod - Method to calculate weights.

--------------------------------------------------------------
 OUTPUT
	current chemistry's weight method.

--------------------------------------------------------------
 LOCAL
	hr - System return code.
	db - Database structure.
	session - Session required by database access.
	cmdGetWeightMethod - Command to call stored procedure.
	cmdinit - Command properties.
	wMethod - Return found weight method in this variable.

--------------------------------------------------------------
 RETURN														
	status code hr.												
****************************************************************/
HRESULT GetWeightMethodDB(LONG calibrationID, 
						  CString &wMethod)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;

	// Define command to get Fit Parameter Limits from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetWeightMethodAccessor> > cmdGetWeightMethod;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetWeightMethodDB" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetWeightMethodDB");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetWeightMethod.Create(session, _T("{ ? = CALL dbo.WeightMethod;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to stored procedure \"WeightMethod\"","GetWeightMethodDB");
		return hr;
	}

	// Set database searching criteria.
	cmdGetWeightMethod.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetWeightMethod.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"WeightMethod\"","GetWeightMethodDB");
		return hr;
	}

	cmdGetWeightMethod.MoveFirst();
	wMethod = (LPCTSTR)cmdGetWeightMethod.m_colWeightMethod;

	cmdGetWeightMethod.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/****************************************************************
HRESULT GoodnessTests(CString calibrationDate,
					  CStringList &TestList)

 Get list of goodness of fit tests for current calibration.

--------------------------------------------------------------
 INPUT
	calibrationDate - Calibration date.
	&TestList - Parameter to return test list in.

--------------------------------------------------------------
 OUTPUT														
	current chemistry's goodness test list.

--------------------------------------------------------------
 LOCAL
	hr - System return code.
	db - Database structure.
	session - Session required by database access.
	cmdGetGoodnessOfFitTest - Command to call stored procedure.
	cmdinit - Command properties.
	TestList - Return found test list in this variable.

--------------------------------------------------------------
 RETURN
	status code hr.

****************************************************************/
HRESULT GoodnessTests(LONG calibrationID,
					  CStringList &TestList)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;

	// Define command to get Goodness Of Fit Test list from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetGoodnessOfFitTestsAccessor> >cmdGetGoodnessOfFitTest;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GoodnessTests" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\"","GoodnessTests");

		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetGoodnessOfFitTest.Create(session, _T("{ ? = CALL dbo.GetGoodnessOfFitTests;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetGoodnessOfFitTests\" stored procedure","GoodnessTests");

		return hr;
	}

	// Set database searching criteria.
	cmdGetGoodnessOfFitTest.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetGoodnessOfFitTest.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetGoodnessOfFitTests\" stored procedure","GoodnessTests");

		return hr;
	}

	while (cmdGetGoodnessOfFitTest.MoveNext() == S_OK)
	{
		TestList.AddTail( (LPCTSTR)cmdGetGoodnessOfFitTest.m_colTest );
	}

	cmdGetGoodnessOfFitTest.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/*************************************************************
LoadVariantToVector(VARIANT *varStruct, 
				    dVector vecStruct)

	Copies a VARIANT type to an OptiVec vector.
--------------------------------------------------------------
 INPUT
	*varStruct - VARIANT to load to vector.
	vecStruct - Vector to contain data from VARIANT.

--------------------------------------------------------------
 OUTPUT
	vecStruct vector filled with VARIANT varStruct values.

--------------------------------------------------------------
 LOCAL
	psa_varStruct	- SAFEARRAY to contain VARIANT data.
	i				- Index to traverse SAFEARRAY.
	hr				- Status code.

--------------------------------------------------------------
 RETURN
	None.
*************************************************************/
void LoadVariantToVector(VARIANT *varStruct, 
						 dVector vecStruct)
{
	SAFEARRAY FAR*	psa_varStruct;		// sample plot list.
	HRESULT			hr=S_OK;
	long			i=0;

	// set psa_plotList safearray pointer to the current plot list.
	psa_varStruct = V_ARRAY(varStruct);

	// Verify that data to plot is of correct type.
	if ( varStruct->vt == 8197 )	// if ( plotData->vt == VT_R8 )
	{
		double data_value;

		// Get current data value. Put it in vector.
		while ((hr = SafeArrayGetElement(psa_varStruct, &i, &data_value)) == S_OK)
		{
			vecStruct[i] = data_value;
			i++;
		}
	}
	else
	{
		LogErrorMessage(hr,"Failed to copy variant to a vector.","LoadVariantToVector");
		return;
	}
}

/*************************************************************
HRESULT VariantLength(VARIANT *varToSize, 
					  ui *varLength)

 	Calculate length of a VARIANT structure.
--------------------------------------------------------------
 INPUT
	*varToSize - VARIANT structure to find length of.
	*varLength - Return variant length.

--------------------------------------------------------------
 OUTPUT
	None.

--------------------------------------------------------------
 LOCAL
	psa_varToSize - Pointer to convert VARIANT to SAFEARRAY.
					VARIANTs can't be queried for metadata.
	hr - Return code from quering functions.
	cElements - Calculated length of structure.
	lLBound - Lower bound of SAFEARRAY struct.
	lUBound - Upper bound of SAFEARRAY struct.

--------------------------------------------------------------
 RETURN
	Length of VARIANT structure.

*************************************************************/
HRESULT VariantLength(VARIANT *varToSize, 
					  ui *varLength)
{
	SAFEARRAY FAR*	psa_varToSize;
	HRESULT			hr;
	LONG			lLBound, lUBound;

	// set psa_plotList safearray pointer to the current plot list.
	psa_varToSize = V_ARRAY(varToSize);

	// Get array lower bound.
	hr = SafeArrayGetLBound(psa_varToSize, 1, &lLBound);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"SafeArrayGetLBound\" function","VariantLength");
		
		return hr;
	}
	else
	{
		// Get array upper bound.
		hr = SafeArrayGetUBound(psa_varToSize, 1, &lUBound);
		if(FAILED(hr))
		{
			LogInterfaceErrorMessage(hr,"Return from \"SafeArrayGetUBound\" function","VariantLength");

			return hr;
		}
		else
		{
			// Number of elements in array.
			*varLength = lUBound-lLBound+1;
		}
	}

	return S_OK;
}

/***************************************************************
HRESULT PutCalculatedAssignedValuesInDb(LONG calibrationID,
										CString	sampleType,
										dVector	assignedCoef,
										double	ccoe,
										dVector	vec_AssignedValues,
										dVector	vec_calcAssignedValues,
										ui		sizeallplotData)

 	Put current calculated assigned values in database.			
----------------------------------------------------------------
 INPUT
	calibrationDate - Date of current calibration.
	sampleType - Type of current sample.
	assignedCoef - Calculated fit parameters and errors.
	ccoe - Correlation coefficient.
	vec_AssignedValues - Vector with assigned value.
	vec_calcAssignedValues - Vector with calculated assigned values.
	sizeallplotData - Number of plot values.

----------------------------------------------------------------
 OUTPUT
	Database with current calculated assigned values.

----------------------------------------------------------------
 LOCAL														
	hr - Return code.
	db - Database access object.
	session - Session object.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											

***************************************************************/
HRESULT PutCalculatedAssignedValuesInDb(LONG calibrationID,
										CString	sampleType,
										dVector	assignedCoef,
										double	ccoe,
										dVector	vec_AssignedValues,
										dVector	vec_calcAssignedValues,
										ui		sizeallplotData,
										VARIANT	*error_msg)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	CString		tmpAssigned, tmpCalcAssigned;
	char		buff[80];
	char		error_buf[512];

	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboInsertRecoveredValuesFromRatesAccessor> > cmdRecoveredFromRates;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_INSERT);

	// Convert vector doubles to strings.
	for (int i=0; i<sizeallplotData; i++)
	{
		// Convert assigned value from double to string.
		sprintf(buff, "%.15f",vec_AssignedValues[i]);
		tmpAssigned += buff;
		tmpAssigned += ',';

		// Convert calculated assigned value from double to string.
		sprintf(buff, "%.15f",vec_calcAssignedValues[i]);
		tmpCalcAssigned += buff;
		tmpCalcAssigned += ',';
	}

	tmpAssigned.Delete(tmpAssigned.GetLength() -1, 1);
	tmpCalcAssigned.Delete(tmpCalcAssigned.GetLength() -1, 1);

	// Open database.
	hr = OpenDatabase( &db, &session, "PutCalculatedAssignedValuesInDb" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","PutCalculatedAssignedValuesInDb");
		return hr;
	}

	hr = cmdRecoveredFromRates.Create (session, _T("{ ? = CALL dbo.InsertRecoveredValuesFromRates;1 (?,?,?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create stored procedure \"InsertRecoveredValuesFromRates\" command","PutCalculatedAssignedValuesInDb");
		return hr;
	}

	// Send current sample's calculated assigned values to db.
	// Set first stored procedure parameter with chemistry name.
 	cmdRecoveredFromRates.m_calibrationID = calibrationID;

	// Set second stored procedure parameter with sample type.
 	_tcsncpy(cmdRecoveredFromRates.m_SampleType, sampleType,
 		sizeof(cmdRecoveredFromRates.m_SampleType) /
 		sizeof(cmdRecoveredFromRates.m_SampleType[0]));

	// Set third stored procedure parameter with current corelation coefficient.
	cmdRecoveredFromRates.m_ccoe = ccoe * ccoe;

	// Set fourth stored procedure parameter with current slope.
	cmdRecoveredFromRates.m_Slope = assignedCoef[0];

	// Set fifth stored procedure parameter with current slope standard error.
	cmdRecoveredFromRates.m_SlopeStdError = assignedCoef[1];

	// Set sixth stored procedure parameter with current intercept.
	cmdRecoveredFromRates.m_Intercept = assignedCoef[2];

	// Set seventh stored procedure parameter with current intercept standard error.
	cmdRecoveredFromRates.m_InterceptStdError = assignedCoef[3];

	// Set eighth  stored procedure parameter with current assigned values.
 	_tcsncpy(cmdRecoveredFromRates.m_Assigned, tmpAssigned,
 		sizeof(cmdRecoveredFromRates.m_Assigned) /
 		sizeof(cmdRecoveredFromRates.m_Assigned[0]));

	// Set nineth  stored procedure parameter with current recovered values.
 	_tcsncpy(cmdRecoveredFromRates.m_Recovered, tmpCalcAssigned,
 		sizeof(cmdRecoveredFromRates.m_Recovered) /
 		sizeof(cmdRecoveredFromRates.m_Recovered[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdRecoveredFromRates.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(error_buf, "Function \"PutCalculatedAssignedValuesInDb\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call stored procedure \"InsertRecoveredValuesFromRates\"",error_buf);

		return hr;
	}

	cmdRecoveredFromRates.Close();

	// Close the database.
	db.Close();

	// Test return code from stored procedure.
	if (cmdRecoveredFromRates.m_RETURNVALUE != 0)
	{
		hr = DataBaseError(cmdRecoveredFromRates.m_RETURNVALUE,"PutCalculatedAssignedValuesInDb",error_msg);
		return hr;
	}

	return S_OK;
}

/***************************************************
HRESULT GetGraphParameters(BSTR *calibrationDate,
						BSTR calculatedAssignedType,
						double *ccoe,
						double *slope,
						double *intercept)

Get correlation, slope and intercept parameters for
given sample and type.

----------------------------------------------------
INPUT
	calibrationDate - Calibration date.
	calculatedAssignedType - Current sample type.
	*ccoe - Return correlation coefficient.
	*slope - Return slope.
	*intercept - Return intercept.

----------------------------------------------------
OUTPUT
	*ccoe, *slope, *intercept - With current values.
	
----------------------------------------------------
RETURN
	Status code, hr.

***************************************************/
HRESULT GetGraphParameters(LONG calibrationID,
						BSTR calculatedAssignedType,
						double *ccoe,
						double *slope,
						double *intercept)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;

	// Define command to get Assigned Fit Parameters from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetAssignedFitParametersAccessor> >cmdGetGraphParms;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetGraphParameters" );
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"OpenDatabase\" functon","GetGraphParameters");

		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetGraphParms.Create(session, _T("{ ? = CALL dbo.GetAssignedFitParameters;1 (?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access \"GetAssignedFitParameters\" stored procedure","GetGraphParameters");
		return hr;
	}

	// Set database searching criteria.
	cmdGetGraphParms.m_calibrationID = calibrationID;

	// Set database searching criteria.
	_tcsncpy(cmdGetGraphParms.m_SampleType, (CString)calculatedAssignedType,
		sizeof(cmdGetGraphParms.m_SampleType) /
		sizeof(cmdGetGraphParms.m_SampleType[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetGraphParms.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetAssignedFitParameters\" stored procedure","GetGraphParameters");
		return hr;
	}

	cmdGetGraphParms.MoveFirst();
	
	*ccoe = cmdGetGraphParms.m_colccoe;
	*slope = cmdGetGraphParms.m_colSlope;
	*intercept = cmdGetGraphParms.m_colIntercept;

	cmdGetGraphParms.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/*******************************************************
HRESULT AssignFitParameters(LONG calibrationID, 
							VARIANT *newFitPars,
							VARIANT *error_msg) 

	Set FitParameters global vector with new values.

--------------------------------------------------------
INPUT
	calibrationDate - Calibration date.
	*newFitPars - List of new values for fit parameters.
	error_msg - User response message.

--------------------------------------------------------
LOCAL
	hr - Result code.
	lLCID - Find a Language code, used by conversion 
			functions. i.e. VarR8FromStr.
	no_error - Error test.
    psa_newFitPars - SAFEARRAY for new fit parameters.
	curValue - Convert string data to double values.
	bstr - String value fraom SAFEARRAY.
	i - SAFEARRAY index.
	temp - Compare current SAFEARRAY label.
	local_error - Error of current label.

--------------------------------------------------------
OUTPUT
	FitParameters global vector set with new values.

--------------------------------------------------------
RETURN
	calibDate - Current calibration date.

*******************************************************/
HRESULT AssignFitParameters(LONG calibrationID, 
							VARIANT *newFitPars,
							VARIANT *error_msg) 
{
	HRESULT		hr;
	DWORD		lLCID;
	bool		no_error = true;
	char		buf[512];
	double		limit;

	// Find a Language code, used by conversion functions. i.e. VarR8FromStr.
	lLCID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

	//*** define the safearray ***
    SAFEARRAY FAR* psa_newFitPars = NULL;

	// set new fit parameter values VARIANT type to SAFEARRAY type.
	psa_newFitPars = V_ARRAY(newFitPars);

	//*** define a BSTR to move the strings in and out ***
	double	curValue;
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

		if (temp.CompareNoCase("TOP") == 0)
		{
			FitParameters[Top] = curValue;
			
			// Set status of fit parameter "Top" to fixed.
			FitParameterStatus[Top] = 0;

			if (curValue > UpperLimits[Top])
			{
				local_error = "HIGH";
				no_error = false;
				limit = UpperLimits[Top];
			}
			else if (curValue < LowerLimits[Top])
			{
				local_error = "LOW";
				no_error = false;
				limit = LowerLimits[Top];
			}
		}
		else if (temp.CompareNoCase("BOTTOM") == 0)
		{
			FitParameters[Bottom] = curValue;

			// Set status of fit parameter "Bottom" to fixed.
			FitParameterStatus[Bottom] = 0;
				
			if (curValue > UpperLimits[Bottom])
			{
				local_error = "HIGH";
				no_error = false;
				limit = UpperLimits[Bottom];
			}
			else if (curValue < LowerLimits[Bottom])
			{
				local_error = "LOW";
				no_error = false;
				limit = LowerLimits[Bottom];
			}
		}	
		else if (temp.CompareNoCase("SLOPE") == 0)
		{
			FitParameters[Slope] = curValue;
			
			// Set status of fit parameter "Slope" to fixed.
			FitParameterStatus[Slope] = 0;

			if (curValue > UpperLimits[Slope])
			{
				local_error = "HIGH";
				no_error = false;
				limit = UpperLimits[Slope];
			}
			else if (curValue < LowerLimits[Slope])
			{
				local_error = "LOW";
				no_error = false;
				limit = LowerLimits[Slope];
			}
		}
		else if (temp.CompareNoCase("INTERCEPT") == 0)
		{
			FitParameters[Intercept] = curValue;

			// Set status of fit parameter "Intercept" to fixed.
			FitParameterStatus[Intercept] = 0;

			if (curValue > UpperLimits[Intercept])
			{
				local_error = "HIGH";
				no_error = false;
				limit = UpperLimits[Intercept];
			}
			else if (curValue < LowerLimits[Intercept])
			{
				local_error = "LOW";
				no_error = false;
				limit = LowerLimits[Intercept];
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

/********************************************************
HRESULT FindCurrentAlgorithmType(BSTR calibrationDate, 
								 CString *algorithmType)

Find algorithm used for current calibration.

---------------------------------------------------------
INPUT
	calibrationDate - Date of current calibration.
	algorithmType - Type of current algorithm.

---------------------------------------------------------
OUTPUT
	algorithmType - With current calibration's algorithm.

---------------------------------------------------------
LOCAL
	hr - Return code.
	db - Database access object.
	session - Session access.

---------------------------------------------------------

RETURN
	hr - Status code.
********************************************************/
HRESULT FindCurrentAlgorithmType(LONG calibrationID, 
								 CString *algorithmType)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;

	// Define command to find algorithm used for the current calibration.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetCalibrationAlgorithmAccessor> > cmdFindCurrentAlgType;

	// Open database.
	hr = OpenDatabase( &db, &session, "FindCurrentAlgorithmType" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","FindCurrentAlgorithmType");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdFindCurrentAlgType.Create(session, _T("{ ? = CALL dbo.GetCalibrationAlgorithm;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to call \"GetCalibrationAlgorithm\"","FindCurrentAlgorithmType");
		return hr;
	}

	// Set database searching criteria.
	cmdFindCurrentAlgType.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdFindCurrentAlgType.Open();
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetCalibrationAlgorithm\"","FindCurrentAlgorithmType");
		return hr;
	}

	*algorithmType = (LPCTSTR)cmdFindCurrentAlgType.m_Algorithm;

	cmdFindCurrentAlgType.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/********************************************************
HRESULT InsertSigmoidalFitParameters(LONG calibrationID,
									 dVector FitPars,
									 ui size,
									 double ccoe,
									 double chisquared,
									 double bartlett,
									 double TopError,
									 double BottomError,
									 double SlopeError)

Add calibration results, calculated with a sigmoidal algorithm, to the database.

---------------------------------------------------------
INPUT
	calibrationID - Identifier of current calibration.
	FitPars - Vector containing calculated fit parameters.
	size - Number of fit parameters.
	ccoe - Correlation coefficient.
	chisquared - Chi squared statistic test.
	bartlett - Homogeneity test.
	TopError - Error on Top fit parameter.
	BottomError - Error on Bottom fit parameter.
	SlopeError - Error on Slope.
	InterceptError - Error on Intercept.
	
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
HRESULT InsertSigmoidalFitParameters(LONG calibrationID,
									 dVector FitPars,
									 ui size,
									 double ccoe,
									 double chisquared,
									 double bartlett,
									 double TopError,
									 double BottomError,
									 double SlopeError,
									 double InterceptError)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to insert sigmoidal fit parameters into the database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboInsertSigmoidalFitParametersAccessor> > objInsertSigmoidalFitParms;

	// Open database.
	hr = OpenDatabase( &db, &session, "InsertSigmoidalFitParameters" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","InsertSigmoidalFitParameters");
		return hr;
	}

	hr = objInsertSigmoidalFitParms.Create (session, _T("{ ? = CALL dbo.InsertSigmoidalFitParameters;1 (?,?,?,?,?,?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for stored procedure \"InsertSigmoidalFitParameters\"","InsertSigmoidalFitParameters");
		return hr;
	}

	// Set data to add to the database.
	objInsertSigmoidalFitParms.m_calibrationID = calibrationID;

	objInsertSigmoidalFitParms.m_ChiSquared = chisquared;
	objInsertSigmoidalFitParms.m_CorrelationCoefficient = ccoe;
	objInsertSigmoidalFitParms.m_Bottom = FitPars[Bottom];
	objInsertSigmoidalFitParms.m_Intercept = FitPars[Intercept];
	objInsertSigmoidalFitParms.m_Slope = FitPars[Slope];
	objInsertSigmoidalFitParms.m_Top = FitPars[Top];
	objInsertSigmoidalFitParms.m_BartlettTest = bartlett;
	objInsertSigmoidalFitParms.m_TopStdError = TopError;
	objInsertSigmoidalFitParms.m_BottomStdError = BottomError;
	objInsertSigmoidalFitParms.m_SlopeStdError = SlopeError;
	objInsertSigmoidalFitParms.m_InterceptStdError = InterceptError;

	hr = objInsertSigmoidalFitParms.Open();
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr, 
			"Call stored procedure \"InsertSigmoidalFitParameters\"",
			"InsertSigmoidalFitParameters");
		return hr;
	}

	objInsertSigmoidalFitParms.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/***************************************************
HRESULT GetPlotSampleSizes(BSTR				calibrationDate, 
						   CStringArray		*plotList, 
						   CArray<int,int>	*totalNumberOfValues,
						   CArray<int,int>	*noOutliersNumberOfValues)

  Get lengths of samples in current plot.

---------------------------------------------------------
INPUT
	calibrationDate - Date of current calibration.
	*plotList - List of samples in current plot.
	*totalNumberOfValues - Number of values.
	*noOutliersNumberOfValues - Number of values, without
		outliers.
	
---------------------------------------------------------
LOCAL
	hr - Return code.
	db - Database access object.
	session - Session access.
	seps[] - Separators for value extraction.
	*sampleid - String values.
	l_plotList - Local plot list string.
	bstr - SAFEARRAY string extraction.

---------------------------------------------------------
OUTPUT
	*var_plotSizes - Filled with sizes of current plot samples.

---------------------------------------------------------
RETURN
	hr - Status code.
***************************************************/
HRESULT GetPlotSampleSizes(LONG				calibrationID, 
						   CStringArray		*plotList, 
						   CArray<int,int>	*totalNumberOfValues,
						   CArray<int,int>	*noOutliersNumberOfValues)
{
	HRESULT			hr;
	char			seps[] = ",\n";
	char			*sampleid;
	CDataSource		db;
	CSession		session;
	CString			l_plotList;
	BSTR			bstr=NULL;

	CCommand<CAccessor<CdboGetPlotSampleSizesAccessor> > cmdPlotSampleSizes;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	for (int i=0; i<plotList->GetSize(); i++)
	{
		// Fill list of sample ids.
		l_plotList += plotList->GetAt(i);
		l_plotList += ',';
	}

	// Adjust temporary strings.
	// Remove extra id delimeter.
	l_plotList.Delete(l_plotList.GetLength() -1, 1);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetPlotSampleSizes" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetPlotSampleSizes");
		return hr;
	}

	hr = cmdPlotSampleSizes.Create (session, _T("{ ? = CALL dbo.GetPlotSampleSizes;1 (?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for stored procedure \"GetPlotSampleSizes\"","GetPlotSampleSizes");
		return hr;
	}

	// Set database searching criteria.
	cmdPlotSampleSizes.m_calibrationID = calibrationID;

	// Set database searching criteria.
	_tcsncpy(cmdPlotSampleSizes.m_SampleIdList, l_plotList,
		sizeof(cmdPlotSampleSizes.m_SampleIdList) /
		sizeof(cmdPlotSampleSizes.m_SampleIdList[0]));

	// Execute current command.
	// Call stored procedure, "GetPlotSampleSizes".
	hr = cmdPlotSampleSizes.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetPlotSampleSizes\"","GetPlotSampleSizes");
		return hr;
	}

	// Close access command.
	cmdPlotSampleSizes.Close();
	// Close the database.
	db.Close();

	// Set access to first row of record set.
	// Get list of sample sizes, total number of values.
	sampleid = strtok( cmdPlotSampleSizes.m_SampleSizesList, seps );
	while( sampleid != NULL )
	{
		totalNumberOfValues->Add(atoi(sampleid));
		//Get next sample size.
		sampleid = strtok( NULL, seps );
	}

	// Get list of sample sizes, number of values, without outliers.
	sampleid = strtok( cmdPlotSampleSizes.m_SampleNoOutlierSizesList, seps );
	while( sampleid != NULL )
	{
		noOutliersNumberOfValues->Add(atoi(sampleid));
		//Get next sample size.
		sampleid = strtok( NULL, seps );
	}

	return S_OK;
}

/****************************************************************
HRESULT BuildListOfSamplesToPlot
					(LONG calibrationID,
					 BSTR sampleType,
					 CStringArray *plotList)

	Find a list of id's for current samples to plot.

--------------------------------------------------------------
 INPUT												
	calibrationID - Calibration id.
	*sampleType - Type of samples to find.
	*plotList - Name list for current pools to plot.

--------------------------------------------------------------
 OUTPUT
	Current sample id list.

--------------------------------------------------------------
 LOCAL														
	hr		- Error code.								
	i		- Index to traverse current plot list.		
	bstr	- BSTR to move the strings in and out.
	db		- Database access object.
	session	- Session access.
	temp	- Convert from TCHAR to CString.

--------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT BuildListOfSamplesToPlot(LONG calibrationID,
								 CString sampleType,
								 CStringArray *plotList)
{
	HRESULT		hr;
	long		i=0;
	CDataSource db;
	CSession	session;
	char		buf[256];

	//*** define a BSTR to move the strings in and out ***
    BSTR bstr = NULL;

	// Define command to get list of samples to plot from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetListOfSamplesToPlotAccessor> > cmdGetListOfSamplesToPlot;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "BuildListOfSamplesToPlot" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"BuildListOfSamplesToPlot\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetListOfSamplesToPlot.Create(session, _T("{ ? = CALL dbo.GetListOfSamplesToPlot;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		sprintf(buf,"Function \"BuildListOfSamplesToPlot\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"GetListOfSamplesToPlot\" stored procedure",buf);
		return hr;
	}

	// Set database searching criteria.
	cmdGetListOfSamplesToPlot.m_calibrationID = calibrationID;

	_tcsncpy(cmdGetListOfSamplesToPlot.m_type, sampleType,
		sizeof(cmdGetListOfSamplesToPlot.m_type) /
		sizeof(cmdGetListOfSamplesToPlot.m_type[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetListOfSamplesToPlot.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		sprintf(buf,"Function \"BuildListOfSamplesToPlot\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call \"GetListOfSamplesToPlot\" stored procedure",buf);
		return hr;
	}

	i=0;
	CString temp;
	while (cmdGetListOfSamplesToPlot.MoveNext() == S_OK)
	{
		temp = cmdGetListOfSamplesToPlot.m_colSampleID;

		plotList->Add(temp);

		i++;
	}

	cmdGetListOfSamplesToPlot.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/****************************************************************
HRESULT GetSampleDataToPlot(LONG calibrationID,
							CStringArray *plotList,
							CArray<double,double>  *plotData)
	Get data to plot.

--------------------------------------------------------------
 INPUT														
	*plotList - List of pools to plot.
	NOTE: Assume plotList contains samples to plot after
			outlier removal.

	*plotData - Return data for current pool list.
	calibrationID - Calibration id.

--------------------------------------------------------------
 OUTPUT
	Data of current sample list.

--------------------------------------------------------------
 LOCAL
	hr		- Error code.								
	db		- Database access object.
	session	- Session access.

--------------------------------------------------------------
 RETURN														
	hr - Status code.											

***************************************************************/
HRESULT GetSampleDataToPlot(LONG calibrationID,
							CStringArray *plotList,
							CArray<double,double>  *plotData)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to get sample data to plot from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetPlotDataAccessor> > cmdGetSampleDataToPlot;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSampleDataToPlot" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetSampleDataToPlot");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetSampleDataToPlot.Create(session, _T("{ ? = CALL dbo.GetPlotData;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for \"GetPlotData\" stored procedure","GetSampleDataToPlot");
		return hr;
	}

	// Set database searching criteria.
	cmdGetSampleDataToPlot.m_calibrationID = calibrationID;

	// Get current data to plot. Without outliers.
	for (int i=0; i<plotList->GetSize(); i++)
	{
		_tcsncpy(cmdGetSampleDataToPlot.m_sampleID, plotList->GetAt(i),
			sizeof(cmdGetSampleDataToPlot.m_sampleID) /
			sizeof(cmdGetSampleDataToPlot.m_sampleID[0]));

		// Execute current command.
		// Call stored procedure.
		hr = cmdGetSampleDataToPlot.Open(&cmdinit,NULL,true);
		if (FAILED(hr))
		{
			// Close the database.
			db.Close();

			LogInterfaceErrorMessage(hr,"Call \"GetPlotData\" stored procedure","GetSampleDataToPlot");
			return hr;
		}

		while (cmdGetSampleDataToPlot.MoveNext() == S_OK)
		{
			plotData->Add(cmdGetSampleDataToPlot.m_colValue);
		}

		cmdGetSampleDataToPlot.Close();
	}

	// Close the database.
	db.Close();

	return S_OK;
}

/****************************************************************
HRESULT GetSampleIdsFromType(LONG calibrationID,
							 CString type,
							 CArray<CString,CString> *sampleIdsList)

	Get sample ids from a calibration,"calibrationID", 
	with type, "type".

--------------------------------------------------------------
 INPUT														
	type - Current type.
	sampleIdsList - List of samples returned.
	calibrationID - Calibration id.

--------------------------------------------------------------
 OUTPUT
	Current sample list.

--------------------------------------------------------------
 LOCAL
	hr		- Error code.								
	db		- Database access object.
	session	- Session access.

--------------------------------------------------------------
 RETURN														
	hr - Error code.											
***************************************************************/
HRESULT GetSampleIdsFromType(LONG calibrationID,
							 CString type,
							 CStringArray *sampleIdsList)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to get sample id list.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetSampleIdsWithTypeAccessor> > cmdGetSampleIds;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSampleIdsFromType" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr, 
			"Return from \"OpenDatabase\" function",
			"GetSampleIdsFromType");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetSampleIds.Create(session, _T("{ ? = CALL dbo.GetSampleIdsWithType;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr, 
			"Create command for \"GetSampleIdsWithType\" stored procedure",
			"GetSampleIdsFromType");
		return hr;
	}

	// Set database searching criteria.
	cmdGetSampleIds.m_calibrationID = calibrationID;

	_tcsncpy(cmdGetSampleIds.m_type, type,
		sizeof(cmdGetSampleIds.m_type) /
		sizeof(cmdGetSampleIds.m_type[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetSampleIds.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call \"GetSampleIdsWithType\" stored procedure","GetSampleIdsFromType");
		return hr;
	}

	while (cmdGetSampleIds.MoveNext() == S_OK)
	{
		sampleIdsList->Add(cmdGetSampleIds.m_colSampleID);
	}

	cmdGetSampleIds.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/****************************************************************
HRESULT GetAssignedValuesToPlot(LONG calibrationID,
								CStringArray *plotList, 
								CArray<double,double> *varAssignedValues, 
								CArray<int,int> *plotSampleSizes,
								VARIANT *error_msg)

 	Get plot sample's assigned values.							

-----------------------------------------------------------------
 INPUT
	*plotList - List of pools to plot.
	*varAssignedValues - Return array with assigned values.
	*plotSampleSizes - Sizes of samples to plot.

-----------------------------------------------------------------
 OUTPUT
	Assigned values for curent samples.

-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	i				- Variant array's traversal index.
	curValue		- Assigned value, from assigned values list.
	nAssignedValues - Number of assigned values.
	lUBound			- Lower bound of plot list.
	lLBound			- Upper bound of plot list.
  bstr			- BSTR to move the strings in and out.
-----------------------------------------------------------------
 RETURN
	hr - Error code.

****************************************************************/
HRESULT GetAssignedValuesToPlot(LONG calibrationID,
								CStringArray *plotList, 
								CArray<double,double> *varAssignedValues, 
								CArray<int,int> *plotSampleSizes,
								VARIANT *error_msg)
{
	HRESULT		hr;
	CString		chemistryName;
	char		buf[256];
	CString		lPlotList;
	CArray<double,double> l_AssignedValues;

	// < ----- GET CHEMISTRY NAME ----- >
	hr = GetChemistryNameGivenCalibrationID(calibrationID,&chemistryName);
	if (FAILED(hr))
 	{
		sprintf(buf,"Function \"GetAssignedValuesToPlot\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" stored procedure",buf);
 		return hr;
 	}
	// < ----- GET CHEMISTRY NAME ----- >

	// < ----- CONVERT PLOT SAMPLE LIST FROM CStringArray to CString ----- >
	hr = FromCStringArrayToCString(plotList, &lPlotList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetAssignedValuesToPlot\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// </ ----- CONVERT PLOT SAMPLE LIST FROM CStringArray to CString ----- >

	// < ----- GET SAMPLE ASSIGNED VALUES ----- >
	hr = GetAssignedValues(chemistryName, lPlotList, varAssignedValues, error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetAssignedValuesToPlot\" function",buf);
		return hr;
	}
	// </ ------ GET SAMPLE ASSIGNED VALUES ----- >

	// Expand assigned values to accomodate all analyzer data.
	// To calculate assigned values.
	for (int i=0; i<plotList->GetSize(); i++)
	{
		for (int k=0; k<plotSampleSizes->GetAt(i); k++)
		{
			l_AssignedValues.Add(varAssignedValues->GetAt(i));
		}
	}

	varAssignedValues->RemoveAll();
	varAssignedValues->Copy(l_AssignedValues);

	return S_OK;
}

/*****************************************************************
HRESULT GetPlotData(LONG calibrationID, 
 					BSTR sampleType, 
 					VARIANT *plotList, 
 					VARIANT *plotData,
					CArray<int,int> *noOutliersNumberOfValues)

Get sample data to plot.

-----------------------------------------------------------------
INPUT
	calibrationID - Identifier of calibration to get plot data for.
	sampleType - Type of sample to get plot data.
	plotList - List of samples to plot.
	plotData - Data of samples to plot.
	*noOutliersNumberOfValues - Number of values to plot.
								Does not include outliers.

-----------------------------------------------------------------
OUTPUT
	Plot list and plot data.

-----------------------------------------------------------------
LOCAL
	hr - Return code.
	totalNumberOfValues - Number of values. Including outliers.
	local_plotList - List to find other sample attributes.

-----------------------------------------------------------------
RETURN
	Return S_OK.

****************************************************************/
HRESULT GetPlotData(LONG calibrationID, 
 					CString sampleType, 
 					CStringArray *plotList, 
 					CArray<double,double> *plotData,
					CArray<int,int> *noOutliersNumberOfValues)
{
	HRESULT			hr;
	CArray<int,int> totalNumberOfValues;
	CArray<int,int> l_noOutliersNumberOfValues;
	CStringArray	l_plotList;

	// Build list of current samples to plot
	hr = BuildListOfSamplesToPlot(calibrationID,sampleType,plotList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"BuildListOfSamplesToPlot\" function","GetPlotData");
		return hr;
	}

	// Get lenghts of current samples to plot.
	hr = GetPlotSampleSizes(calibrationID, 
							plotList, 
							&totalNumberOfValues, 
							noOutliersNumberOfValues);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetPlotSampleSizes\" function","GetPlotData");
		return hr;
	}

	// Test for percentage of outliers.
	for (int i=0; i<totalNumberOfValues.GetSize(); i++)
	{
		if ((((float)noOutliersNumberOfValues->GetAt(i)/(float)totalNumberOfValues.GetAt(i)) * 100.0) >= 67.0)
		{
			l_noOutliersNumberOfValues.Add(noOutliersNumberOfValues->GetAt(i));
			l_plotList.Add(plotList->GetAt(i));
		}
	}

	// Get sample data to plot.
	hr = GetSampleDataToPlot(calibrationID, &l_plotList, plotData);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetSampleDataToPlot\" function","GetPlotData");
		return hr;
	}

	plotList->RemoveAll();
	noOutliersNumberOfValues->RemoveAll();

	plotList->Copy(l_plotList);
	noOutliersNumberOfValues->Copy(l_noOutliersNumberOfValues);

	return S_OK;
}

/*****************************************************************
HRESULT GetCalibrationIDAssignedValues(
						LONG calibrationID,
						CArray<double,double> *assignedValues,
						VARIANT *error_msg)

Get assigned values for calibration that has identifier equal to
	given identifier "calibrationID".

-----------------------------------------------------------------
INPUT
	calibrationID - Id of calibration to get plot data for.
	assignedValues - Assigned values found.

-----------------------------------------------------------------
OUTPUT
	Assigned values for current calibration.

-----------------------------------------------------------------
LOCAL
	hr - Return system code.
	db - Database access object.
	session - Session object to access db.

-----------------------------------------------------------------
RETURN
	System code S_OK
*****************************************************************/
HRESULT GetCalibrationIDAssignedValues(
						LONG calibrationID,
						CArray<double,double> *assignedValues,
						VARIANT *error_msg)
{
	HRESULT			hr;
	CString			lSampleIdList;
	CStringArray	a_sampleIdList;
	CString			chemistry;

	// < ----- GET CHEMISTRY NAME ----- >
	hr = GetChemistryNameGivenCalibrationID(calibrationID, &chemistry);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" function","GetCalibrationIDAssignedValues, Support.cpp");
		return hr;
	}
	// </ ----- GET CHEMISTRY NAME ----- >

	// < ----- GET SAMPLE NAMES ----- >
	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIdList);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function","GetCalibrationIDAssignedValues, Support.cpp");
		return hr;
	}
	// </ ----- GET SAMPLE NAMES ----- >

	hr = FromCStringArrayToCString(&a_sampleIdList,&lSampleIdList);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetAssignedValues\" function","FromCStringArrayToCString, Support.cpp");
		return hr;
	}

	// < ----- GET ASSIGNED VALUES ----- >
	hr = GetAssignedValues(chemistry, lSampleIdList, assignedValues, error_msg);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetAssignedValues\" function","GetCalibrationIDAssignedValues, Support.cpp");
		return hr;
	}
	// </ ----- GET ASSIGNED VALUES ----- >

	return S_OK;
}

HRESULT GetCalibrationIDSamples(LONG calibrationID,
								CStringArray *sampleIdList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get sample names.
	CCommand<CAccessor<CdboGetSampleNamesAccessor> > cmdGetSampleNames;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationIDSamples" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetCalibrationIDSamples");
		return hr;
	}

	// < ----- GET SAMPLE NAMES ----- >
	// Create command to execute stored procedure "GetSampleNames".
	hr = cmdGetSampleNames.Create(session, _T("{ ? = CALL dbo.GetSampleNames;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetSampleNames\" stored procedure","GetCalibrationIDSamples");
		return hr;
	}

	// Set database search criterium.
	cmdGetSampleNames.m_calibrationID = calibrationID;

	// Get sample names.
	hr = cmdGetSampleNames.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetAssignedValues\" stored procedure","GetCalibrationIDSamples");
		return hr;
	}

	sampleIdList->SetSize(0);

	// Fill sample id list.
	while (cmdGetSampleNames.MoveNext() == S_OK)
	{
		sampleIdList->Add(cmdGetSampleNames.m_colSampleID);
	}

	cmdGetSampleNames.Close();
	// </ ----- GET SAMPLE NAMES ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT GetCalibrationIDSampleSizes(LONG calibrationID,
									CString restriction,
									CArray<int,int> *sampleSizesList)
{
	HRESULT			hr;
	CString			lSampleIdList;
	CStringArray	sampleIdList;
	char			buf[512];

	// < ----- GET SAMPLE NAMES ----- >
	hr = GetCalibrationIDSamples(calibrationID, &sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationIDSampleSizes\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ ----- GET SAMPLE NAMES ----- >

	hr = FromCStringArrayToCString(&sampleIdList,&lSampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationIDSampleSizes\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// < ----- GET NUMBER OF VALUES IN SAMPLEs ----- >
	hr = GetSizesOfAListOfSamples(calibrationID,&sampleIdList,restriction,sampleSizesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationIDSampleSizes\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ ----- GET NUMBER OF VALUES IN SAMPLEs ----- >

	return S_OK;
}

HRESULT GetSizesOfAListOfSamples(LONG calibrationID,
								 CStringArray *sampleIdList,
								 CString outliers,
								 CArray<int,int> *sampleSizesList)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	CString		l_sampleIdList;
	char		*token;
	char		seps[] = ",\n";
	char		buf[512];

	CCommand<CAccessor<CdboGetSampleSizesAccessor> > cmdGetGetSampleSizes;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// < ----- STORED PROCEDURE REQUIRE A FLAT STRING ----- >
	hr = FromCStringArrayToCString(sampleIdList,&l_sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetSizesOfAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// </ ----- STORED PROCEDURE REQUIRE A FLAT STRING ----- >

	// < ----- GET SAMPLE SIZES ----- >
	// Open database.
	hr = OpenDatabase( &db, &session, "GetSizesOfAListOfSamples" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetSizesOfAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return hr;
	}

	// Create command to execute stored procedure "GetSampleSizes".
	hr = cmdGetGetSampleSizes.Create(session, _T("{ ? = CALL dbo.GetSampleSizes;1 (?,?,?,?) }"));
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetSizesOfAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"GetSampleSizes\" stored procedure",buf);
		return hr;
	}

	// Set database search criterium.
	cmdGetGetSampleSizes.m_CalibrationID = calibrationID;

	// Set database searching criteria.
 	_tcsncpy(cmdGetGetSampleSizes.m_SampleIdList, l_sampleIdList,
 		sizeof(cmdGetGetSampleSizes.m_SampleIdList) /
 		sizeof(cmdGetGetSampleSizes.m_SampleIdList[0]));

	// Set database searching criteria.
 	_tcsncpy(cmdGetGetSampleSizes.m_excludestatus, outliers,
 		sizeof(cmdGetGetSampleSizes.m_excludestatus) /
 		sizeof(cmdGetGetSampleSizes.m_excludestatus[0]));

	// Get sample sizes.
	hr = cmdGetGetSampleSizes.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetSampleSizes\" stored procedure","GetSizesOfAListOfSamples");
		return hr;
	}

	// Convert sample sizes.
	token = strtok( cmdGetGetSampleSizes.m_SampleSizesList, seps );
	while( token != NULL )
	{
		sampleSizesList->Add(atoi(token));
		// Get next sample size.
		token = strtok( NULL, seps );
	}

	cmdGetGetSampleSizes.Close();
	// </ ----- GET SAMPLE SIZES ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT GetCalibrationIDDiluent(LONG calibrationID,
								CString *diluent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get diluent.
	CCommand<CAccessor<CdboGetDiluentAccessor> > cmdGetDiluent;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationIDDiluent" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetCalibrationIDDiluent");
		return hr;
	}

	// < ----- GET DILUENT ----- >
	// Create command to execute stored procedure "GetDiluent".
	hr = cmdGetDiluent.Create(session, _T("{ ? = CALL dbo.GetDiluent;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetDiluent\" stored procedure","GetCalibrationIDDiluent");
		return hr;
	}

	// Set database search criterium.
	cmdGetDiluent.m_calibrationID = calibrationID;

	// Get sample names.
	hr = cmdGetDiluent.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call \"GetDiluent\" stored procedure","GetCalibrationIDDiluent");
		return hr;
	}

	cmdGetDiluent.MoveFirst();

	*diluent = cmdGetDiluent.m_colDiluent;

	cmdGetDiluent.Close();
	// </ ----- GET DILUENT ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT GetChemistryNameGivenCalibrationID(LONG calibrationID,
										   CString *ChemistryName)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get chemistry name.
	CCommand<CAccessor<CdboGetChemistryNameAccessor> > cmdGetChemistryName;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetChemistryNameGivenCalibrationID" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetChemistryNameGivenCalibrationID");
		return hr;
	}

	// < ----- GET CHEMISTRY NAME ----- >
	// Create command to execute stored procedure "GetChemistryName".
	hr = cmdGetChemistryName.Create(session, _T("{ ? = CALL dbo.GetChemistryName;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetChemistryName\" stored procedure","GetChemistryNameGivenCalibrationID");
		return hr;
	}

	// Set database search criterium.
	cmdGetChemistryName.m_calibrationID = calibrationID;

	// Get sample names.
	hr = cmdGetChemistryName.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetChemistryName\" stored procedure","GetChemistryNameGivenCalibrationID");
		return hr;
	}

	cmdGetChemistryName.MoveFirst();

	*ChemistryName = cmdGetChemistryName.m_colChemistryID;

	cmdGetChemistryName.Close();
	// </ ----- GET CHEMISTRY NAME ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT BuildListOfRequiredSamples(LONG calibrationID,
								   CString chemistry,
								   CString sampleType,
								   CString calibrationSpecies,
								   CStringArray *requiredSamplesList,
								   VARIANT *error_msg)
{
	HRESULT hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get sample names.
	CCommand<CAccessor<CdboGetListOfRequiredSamplesAccessor> > cmdGetRequiredSamples;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationIDSamples" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","BuildListOfRequiredSamples");
		return hr;
	}

	// < ----- GET REQUIRED SAMPLE NAMES ----- >
	// Create command to execute stored procedure "GetSampleNames".
	hr = cmdGetRequiredSamples.Create(session, _T("{ ? = CALL dbo.GetListOfRequiredSamples;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for \"GetListOfRequiredSamples\" stored procedure","BuildListOfRequiredSamples");
		return hr;
	}

	// Set database searching criteria.
 	_tcsncpy(cmdGetRequiredSamples.m_chemistry, chemistry,
 		sizeof(cmdGetRequiredSamples.m_chemistry) /
 		sizeof(cmdGetRequiredSamples.m_chemistry[0]));

 	_tcsncpy(cmdGetRequiredSamples.m_species, calibrationSpecies,
 		sizeof(cmdGetRequiredSamples.m_species) /
 		sizeof(cmdGetRequiredSamples.m_species[0]));

 	_tcsncpy(cmdGetRequiredSamples.m_type, sampleType,
 		sizeof(cmdGetRequiredSamples.m_type) /
 		sizeof(cmdGetRequiredSamples.m_type[0]));

	// Get sample names.
	hr = cmdGetRequiredSamples.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call \"GetListOfRequiredSamples\" stored procedure","GetCalibrationIDSamples");
		return hr;
	}

	// Fill sample id list.
	while (cmdGetRequiredSamples.MoveNext() == S_OK)
	{
		requiredSamplesList->Add(cmdGetRequiredSamples.m_colSampleID);
	}

	cmdGetRequiredSamples.Close();
	// </ ----- GET REQUIRED SAMPLE NAMES ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT ListOfSamplesToUseInCalibration(LONG calibrationID,
										CStringArray *samplesList,
										VARIANT *error_msg)
{
	HRESULT					hr;
	char					buf[512];

	// < ----- GET SAMPLE NAMES ----- >
	hr = GetCalibrationIDSamples(calibrationID, samplesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ListOfSamplesToUseInCalibration\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ ----- GET SAMPLE NAMES ----- >

	// < ----- Remove CONTROL data from calibration data. ----- >
	// Controls are not part of the calibration calculations.
	hr = RemoveCONTROLData(calibrationID, samplesList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"RemoveCONTROLData\"","ListOfSamplesToUseInCalibration");
		return hr;
	}
	// </ ----- Remove CONTROL data from calibration data. ----- >

	// Update sample ids list, depending on data status.
	// Remove outliers.
	hr = RemoveSamplesWithTooManyOutliers(calibrationID, samplesList);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"RemoveSamplesWithTooManyOutliers\"","ListOfSamplesToUseInCalibration");
		return hr;
	}

	return S_OK;
}

HRESULT RemoveSamplesWithTooManyOutliers(LONG calibrationID, CStringArray *samplesList)
{
	HRESULT			hr=S_OK;
	CDataSource		db;
	CSession		session;
	CStringArray	l_samplesList;
	CArray<int,int>	sample_status;
	char			*token;
	char			seps[] = ",\n";

	CCommand<CAccessor<CdboGetSampleStatusAccessor> > cmdGetSampleStatus;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	// Open database.
	hr = OpenDatabase(&db,&session,"RemoveSamplesWithTooManyOutliers");
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","RemoveSamplesWithTooManyOutliers");
		return hr;
	}

	// Create command to execute stored procedure "TestSampleStatus".
	hr = cmdGetSampleStatus.Create(session, _T("{ ? = CALL dbo.GetSampleStatus;1 (?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for \"GetSampleStatus\" stored procedure","RemoveSamplesWithTooManyOutliers");
		return hr;
	}

	// Set database searching criteria.
	cmdGetSampleStatus.m_calibrationID = calibrationID;

	for (int i=0; i<samplesList->GetSize(); i++)
	{
		sample_status.RemoveAll();

 		_tcsncpy(cmdGetSampleStatus.m_SampleId, samplesList->GetAt(i),
 			sizeof(cmdGetSampleStatus.m_SampleId) /
 			sizeof(cmdGetSampleStatus.m_SampleId[0]));

		// Get sample status.
		hr = cmdGetSampleStatus.Open(&cmdinit,NULL,true);
		if (FAILED(hr))
		{
			// Close the database.
			db.Close();

			LogInterfaceErrorMessage(hr,"Call \"GetSampleStatus\" stored procedure","RemoveSamplesWithTooManyOutliers");
			return hr;
		}
		
		// Convert value status.
		token = strtok( cmdGetSampleStatus.m_SampleStatus, seps );
		while( token != NULL )
		{
			sample_status.Add(atoi(token));
			// Get next value status.
			token = strtok( NULL, seps );
		}

		if (TestSampleStatus(&sample_status,cmdGetSampleStatus.m_NumberOfSets))
		{
			l_samplesList.Add(samplesList->GetAt(i));
		}

		cmdGetSampleStatus.Close();
	}

	// Close the database.
	db.Close();

	samplesList->RemoveAll();
	samplesList->Copy(l_samplesList);

	return hr;
}

bool TestSampleStatus(CArray<int,int> *status, int numberOfSets)
{
	bool ret_code = true;
	int	number_of_passing_status;
	int	number_of_failing_status;
	int	set_size, k;
	bool valid_status;

	set_size = status->GetSize() / numberOfSets;

	number_of_passing_status = 0;
	number_of_failing_status = 0;

	if ((status->GetSize() % numberOfSets) == 0)
	{
		for (int i=0; i<set_size; i++)
		{
			k = 0;
			valid_status = true;
			while(k<numberOfSets)
			{
				if ((status->GetAt(k*set_size+i) != 0) && 
					(status->GetAt(k*set_size+i) != 2))
				{
					valid_status = false;
				}

				k++;
			}

			if (valid_status)
			{
				number_of_passing_status++;
			}
			else
			{
				number_of_failing_status++;
			}
		}

		// One third of the total value comes from the following documents:
		//
		//			Title:Calibration of T4 Method
		//		 Revision: A
		// Effective Date: 01/02/01
		//			  DCO: 42106
		//
		//			Title:Calibration of Creatinine Method
		//		 Revision: D
		// Effective Date: 8/8/03
		//			  DCO: 43138
		if ((float)number_of_failing_status > ((float)set_size / 3.0))
		{
			ret_code = false;
		}
		else
		{
			ret_code = true;
		}
	}
	else
	{
		ret_code = false;
	}

	return ret_code;
}
