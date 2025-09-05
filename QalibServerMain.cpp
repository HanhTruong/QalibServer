/*******************************************************************

FILENAME:	QalibServerMain.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Main module for calibration processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:

STDMETHODIMP QalibServerMain::ReceiveClientData(
				BSTR	chemistry, 
				INT		outlierMethod, 
				BSTR	mode, 
				BSTR	user, 
				VARIANT *inputData, 
				VARIANT *serialNumber, 
				VARIANT *barCode, 
				VARIANT *sampleSpecies, 
				VARIANT *statusData, 
				LONG	*calibrationID, 
				VARIANT *error_msg)

STDMETHODIMP QalibServerMain::GetUpdatedFitParameters(
				LONG calibrationID, 
				VARIANT *newFitParameters, 
				VARIANT *error_msg)
STDMETHODIMP QalibServerMain::SendCalibrationResults(
				LONG	calibrationID, 
				BSTR	*chemistry, 
				BSTR	*waveLength, 
				BSTR	*species, 
				BSTR	*diluent, 
				BSTR	*mold, 
				BSTR	*rotor, 
				BSTR	*cuvette, 
				BSTR	*mode, 
				BSTR	*user, 
				BSTR	*date, 
				VARIANT *description, 
				VARIANT *originalFitParameters, 
				VARIANT *newFitParameters, 
				VARIANT *minLimits, 
				VARIANT *maxLimits, 
				VARIANT *originalDisposition, 
				VARIANT *newDisposition, 
				VARIANT *calibrationData, 
				VARIANT *statusData, 
				VARIANT *rates, 
				VARIANT *plotData, 
				BSTR	*comment, 
				VARIANT *error_msg)
STDMETHODIMP QalibServerMain::SendAssignedValues(
				BSTR chemistry, 
				const VARIANT *sampleIDs, 
				VARIANT *curAssignedValues, 
				VARIANT *error_msg)
STDMETHODIMP QalibServerMain::SendEditableFitParameters(
				LONG calibrationID, 
				VARIANT *editableFitParameters, 
				VARIANT *error_msg)
STDMETHODIMP QalibServerMain::SetComment(
				LONG calibrationID, 
				BSTR *comment, 
				VARIANT *error_msg)
STDMETHODIMP QalibServerMain::SendUserRights(
				BSTR user, 
				BSTR password, 
				VARIANT *modeList, 
				VARIANT *chemistryList, 
				VARIANT *error_msg)
--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	QalibServerMain.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
// QalibServerMain.cpp : Implementation of CQalibServerApp and DLL registration.

#include "stdafx.h"

#include <math.h>
#include <iomanip>
#include <fstream.h>
#include <VIstd.h>
#include "ClientData.h"

#include "QalibConstants.h"

#include "Results.h"

#include "Loging.h"
#include "Support.h"
#include "HelperFunctions.h"
#include "globals.h"
#include "Utils.h"

#include "LinearCalibration.h"

#include "NewFitParameters.h"

#include "QalibServer.h"
#include "QalibServerMain.h"

#include "Outliers.h"

#include "ErrorProcessing.h"
#include "BusinessRulesProcess.h"
#include "BusinessRulesEnforce.h"
#include "FixedSlope.h"

#include "dboGetOutlierLimits.h"
#include "dboSetComment.H"
#include "dboGetAssignedValues.H"
#include "dboGetCalibrationAlgorithm.H"
#include "dboUserAllowedAccess.H"
#include "CalibrationCalculations.h"
#include "DbAccess.h"
#include "DbUtils.h"
#include "BarCodes.h"
#include "Rotor.h"
#include "LinearFitParameters.h"
#include "BestFitMethods.h"
#include "AssignedValues.h"

// BEGIN externs in global.h
double		ccoe;
ui			numberofsamples;
liVector	PoolSizes;
dVector		XExp, YExp, Weights;
dVector		UpperLimits;	// impose upper limits on parameters
dVector		LowerLimits;	// impose lower limits on parameters
dMatrix		Covar;			// covariance matrix.
VD_NONLINFITOPTIONS Opt;
ui			sizex;
dVector		assignedCoef;
dVector		FitParameters;
iVector		FitParameterStatus;
ui			numberOfFitParameters;
// END externs in global.h

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP QalibServerMain::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IQalibServerMain,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/*****************************************************************
STDMETHODIMP QalibServerMain::ReceiveClientData(
				BSTR	chemistry, 
				INT		outlierMethod, 
				BSTR	mode, 
				BSTR	user, 
				VARIANT *inputData, 
				VARIANT *serialNumber, 
				VARIANT *barCode, 
				VARIANT *sampleSpecies, 
				VARIANT *statusData, 
				LONG	*calibrationID, 
				VARIANT *error_msg)

- Receives current calibration data from client and stores it 
  in a local data structure, CalibrationData.
- Set current calibration date.
- Tags outliers, according to given method.
- Stores calibration data in database.
- Performs calibration calculations.
- Stores results in database.

INPUT
	chemistry			- Current chemistry to calibrate. 
	outlierMethod		- Outlier removal method.
	mode				- Calibration operation choice.
	user				- User identifier. 
	inputData			- Calibration data.
	serialNumber		- List of analyzer serial numbers.
	barCode				- Bar codes.
	sampleSpecies		- Species of current calibration.
	statusData			- Status of current values.
	diluent				- Diluent used in rotor.
	mold				- Mold number used for current rotors used in calibration.
	rotor				- Rotors used to get current calibration data.
	cuvette				- Cuvette of chemistry being calibrated.
	calibrationID		- Calibration identifier. 
	error_msg			- Error reporting buffer.

OUTPUT
	Database updated with current calibration data and results.

	calibrationID - Identifier for current calibration.
	error_msg	  - Error message.

LOCAL
	hr				 - Handle returned from safearray object.
	buf[256]		 - Message buffer.
	newFitParameters - Current fit parameters.
	algorithm		 - Current algorithm.

RETURN
	Function status.

*****************************************************************/
STDMETHODIMP QalibServerMain::ReceiveClientData(
				BSTR	chemistry, 
				INT		outlierMethod, 
				BSTR	mode, 
				BSTR	user, 
				VARIANT *inputData, 
				VARIANT *serialNumber, 
				VARIANT *barCode, 
				VARIANT *sampleSpecies, 
				VARIANT *statusData, 
				LONG	*calibrationID, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT					hr;
	char					buf[256];
	VARIANT					newFitParameters;
	CString					algorithm;
	CString					calibrationSpecies;
	CStringArray			rules;
	CArray<double,double>	values;

CString temp="???????????? BEGIN ReceiveClientData ????????????";
DisplayCString(&temp);

	// < ----- PUT DATA FOR CURRENT CALIBRATION IN THE DATABASE ----- >
	hr = ProcessClientData(chemistry,outlierMethod,
						   mode,user,
						   &calibrationSpecies,
						   inputData,serialNumber, 
						   barCode,sampleSpecies,
						   statusData,calibrationID,
						   error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3","Error: Processing client data.",error_msg);
		}

		return hr;
	}
	// </ ----- PUT DATA FOR CURRENT CALIBRATION IN THE DATABASE ----- >

	// Clear user's error message. Set vt field to VT_EMPTY.
	VariantInit(error_msg);

	// < ----- APPLY BUSINESS RULES TO CURRENT CALIBRATION ----- >
	// Test current part number's, chemistry, business rules.
	hr = ProcessBusinessRules(*calibrationID,
							  (CString)chemistry,
							  (CString)mode,
							  calibrationSpecies,
							  serialNumber, 
							  error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3", "Problem processing business rules", error_msg);

			sprintf(buf,"Function \"ReceiveClientData\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"ProcessBusinessRules\"", buf);
		}

		return hr;
	}
	// </ ----- APPLY BUSINESS RULES TO CURRENT CALIBRATION ----- >

	// < ----- FIND CURRENT ALGORITHM FOR CURRENT CHEMISTRY ----- >
	// Results depend on what algorithm the calibration used.
	hr = FindCurrentAlgorithmType(*calibrationID, &algorithm);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ReceiveClientData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"FindCurrentAlgorithmType\"", buf);
		return hr;
	}
	// </ ----- FIND CURRENT ALGORITHM FOR CURRENT CHEMISTRY ----- >

	// < ----- TEST CALIBRATION DATA FOR OUTLIERS ----- >
	// Check outliers.
	// Outliers processing depend on Chemistry algorithm.
	hr = Outliers(*calibrationID,(CString)mode,algorithm,outlierMethod,error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"ReceiveClientData\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"QalibOutliers\"", buf);
		}

		return hr;
	}
	// </ ----- TEST CALIBRATION DATA FOR OUTLIERS ----- >

	// < ----- TEST NUMBER OF OUTLIERS ----- >
	hr = TestOutlierResults(*calibrationID,(CString)mode,error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"ReceiveClientData\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"TestOutlierResults\"", buf);
		}

		return hr;
	}
	// </ ----- TEST NUMBER OF OUTLIERS ----- >

	// < ----- INITIALIZE GLOBALS ----- >
	// Initialize global variables.
	InitializeGlobals(algorithm);

	// Set vt field to VT_EMPTY
	VariantInit(&newFitParameters);
	// </ ----- INITIALIZE GLOBALS ----- >

	// < ----- APPLY CALCULATION TO CURRENT DATA SETs ----- >
	// Calibration calculations
	hr = GetUpdatedFitParameters(*calibrationID, &newFitParameters, error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3", "Problem with fit parameters", error_msg);

			sprintf(buf,"Function \"ReceiveClientData\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"GetUpdatedFitParameters\"",	buf);
		}

		return hr;
	}
	// </ ----- APPLY CALCULATION TO CURRENT DATA SETs ----- >

	OKMessage(error_msg);

temp="???????????? END ReceiveClientData ????????????";
DisplayCString(&temp);

	return S_OK;
}

/*****************************************************************
GetUpdatedFitParameters(BSTR *calibrationDate,
						VARIANT *newFitParameters, 
						VARIANT *error_msg)

	- Main function to carry out calibration calculations.
	- Call functions to process each method of calculations.

INPUT
	*calibrationDate - Date of current calibration.
	*newFitParameters - New fit parameters to use in re-calculations.
	*error_msg - Error buffer.

OUTPUT
	Calibration calculations.

LOCAL
	hr - Result code for system functions.
	wMethod - Weight method for current calibration.
	chisquared - Chi-squared fit test.

FUNCTIONS
	ErrorMessage - Report error if there is a failure.
	GoodnessTests - Get goodness of fit tests list from db.
	UpdateCalibrationData - Update calibration data, to run calibration calculations
							for updated fit parameters.
	GetWeightMethodDB - Get current chemitry's weight method.
	QalibWeightMethod - Calculate weights, according to requested weight method.
	QalibNonlinearFitWithWeight - Process nonlinear fit of data.
	BestFitMethods - Best fit methods.
*****************************************************************/
STDMETHODIMP QalibServerMain::GetUpdatedFitParameters(
				LONG calibrationID, 
				VARIANT *newFitParameters, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT					hr;
	double					chisquared = -2.0;
	CString					wMethod;
	CString					algorithmType;
	CString					chemistry;
	char					buf[256];
	CPtrArray				datasets;
	CPtrArray				statussets;
	CStringArray			sampleIds;
	CArray<int,int>			sampleSizes;
	CArray<double,double>	assignedValues;
	CStringArray			rulesToEnforce;
	CArray<double,double>	ruleValues;
	CString					mode;

	// < ----- GET CHEMISTRY NAME ----- >
	hr = GetChemistryNameGivenCalibrationID(calibrationID,&chemistry);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" function","ChemistryDependencies");
		return hr;
	}
	// </ ----- GET CHEMISTRY NAME ----- >

	// Find algorithm of calibration with calibrationID.
	hr = FindCurrentAlgorithmType(calibrationID, &algorithmType);
	if (FAILED(hr))
	{
		sprintf(buf,"Chemistry \"%s\" does not use algorithm \"%s\".",chemistry,algorithmType);
		UserErrorMessage("3","Can't find current algorithm type", error_msg);

		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"FindCurrentAlgorithmType\"",buf);

		return hr;
	}

	// < ----- PREPARE DATA FOR CALIBRATION CALCULATIONS ----- >
	hr = UpdateCalibrationData(calibrationID, 
							   &datasets,
							   &statussets,
							   &sampleIds,
							   &sampleSizes,
							   error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"UpdateCalibrationData\"",buf);
		}
		return hr;
	}
	// </ ----- PREPARE DATA FOR CALIBRATION CALCULATIONS ----- >

	// ------------------------------------------------------------
	// ***** Does the data set still qualify for calibration, *****
	// ***** after all the outlier removal?					  *****
	// ------------------------------------------------------------
	
	// < ----- CALIBRATION MODE ----- >
	hr = CalibrationMode(calibrationID, &mode, error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"CalibrationMode\"", buf);
		}
		return hr;
	}
	// < ----- CALIBRATION MODE ----- >

	// Verify that the number of calibrators is still valid,
	// to continue the calibration.
	rulesToEnforce.Add("NUMBER_OF_CALIBRATORS");
	// Number of samples is the number of "calibrators".
	ruleValues.Add(sampleIds.GetSize());

	// < ----- TEST RULES FOR CURRENT CHEMISTRY ----- >
	hr = TestBusinessRules(chemistry,
						   mode,	
						   &rulesToEnforce,
						   &ruleValues,
						   error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from function \"TestBusinessRules\"", buf);
		}
		return hr;
	}
	// </ ----- TEST RULES FOR CURRENT CHEMISTRY ----- >

	// ------------------------------------------------------------
	// ***** Does the data set still qualify for calibration, *****
	// ***** after all the outlier removal?					  *****
	// ------------------------------------------------------------

sprintf(buf,"Calibration ID = %ld\n",calibrationID);
CString temp=buf;
DisplayCString(&temp);
DisplayCStringArray(&sampleIds,"sampleIds BEFORE GetSampleAssignedValues");

	// < ----- SAMPLE ASSIGNED VALUE ----- >
	// Valid assigned values, after outlier procedure 
	// has been applied to the sample list.
	hr = GetSampleAssignedValues(calibrationID,&sampleIds,&assignedValues,error_msg);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetSampleAssignedValues\" function","GetUpdatedFitParameters");
		return hr;
	}
	// </ ----- SAMPLE ASSIGNED VALUE ----- >

//DisplayCArray(&assignedValues,"assignedValues AFTER GetSampleAssignedValues");

	// < ----- GET CURRENT CHEMISTRY's WEIGHT METHOD ----- >
	hr = GetWeightMethodDB( calibrationID, wMethod );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"GetWeightMethodDB\"",buf);

		return hr;
	}
	// < ----- GET CURRENT CHEMISTRY's WEIGHT METHOD ----- >		

	// < ----- CHEMISTRY DEPENDENT PRE-PROCESSING ----- >
	// Chemistries may require that the data sets be related in some expression.
	hr = ChemistryDependencies(calibrationID,&datasets,error_msg);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"ChemistryDependencies\" function","GetUpdatedFitParameters");
		return hr;
	}
	// </ ----- CHEMISTRY DEPENDENT PRE-PROCESSING ----- >
	
	for (int i=0; i<datasets.GetSize(); i++)
	{
		// Put data into structures expected by the OptiVec library.
		hr = SetupDataForOptivecFunctions((CArray<double,double> *)datasets.GetAt(i),
										  &sampleIds,
										  &sampleSizes,
										  &assignedValues,
										  error_msg);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"SetupDataForOptivecFunctions\"",buf);

			return hr;
		}

		// Calculate weights, according to requested weight method.
		// Once for each set of data used in calibration.
		hr = QalibWeightMethod(wMethod.AllocSysString());
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"QalibWeightMethod\"",buf);

			return hr;
		}

DisplayVector(XExp,sizex,"Assigned Values BEFORE CalibrationCalculations");
DisplayVector(YExp,sizex,"Data BEFORE CalibrationCalculations");

		//---------------------------------------------
		// < ------- CALIBRATION CALCULATIONS ------- >
		//---------------------------------------------
		hr = CalibrationCalculations(calibrationID,
									 algorithmType,
									 &chisquared,
									 newFitParameters,
									 error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
			{
				UserErrorMessage("3", "Calibration calculations failed.", error_msg);
				sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr, "Return from function \"CalibrationCalculations\"", buf);
			}
			return hr;
		}
		//----------------------------------------------
		// </ ------- CALIBRATION CALCULATIONS ------- >
		//----------------------------------------------

//CString temp="AFTER CalibrationCalculations";
//DisplayCString(&temp);

		// Best fit methods.
		hr = BestFitMethods(calibrationID, 
							algorithmType.AllocSysString(), 
							chisquared,
							error_msg);
		if (FAILED(hr))
		{
			if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
			{
				UserErrorMessage("3", "Best fit methods failed", error_msg);
				sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
				LogErrorMessage(hr, "Return from function \"BestFitMethods\"", buf);
			}
			return hr;
		}

//temp="AFTER BestFitMethods";
//DisplayCString(&temp);

	}

	OKMessage(error_msg);

	return S_OK;
}

/*****************************************************************
STDMETHODIMP CQalibT4ServerCalculations::SendCalibrationResults
					(LONG calibrationID, 
					 BSTR *partNumber, 
					 BSTR *waveLength, 
					 BSTR *species, 
					 BSTR *diluent, 
					 BSTR *mold, 
					 BSTR *rotor, 
					 BSTR *cuvette, 
					 BSTR *mode, 
					 BSTR *user, 
					 BSTR *date, 
					 VARIANT *description, 
					 VARIANT *values, 
					 VARIANT *adjValues, 
					 VARIANT *minLimits, 
					 VARIANT *maxLimits, 
					 VARIANT *disposition, 
					 VARIANT *adjDisposition, 
					 VARIANT *calibrationData, 
					 VARIANT *statusData, 
					 VARIANT *rates, 
					 VARIANT *plotData, 
					 BSTR *comment, 
					 VARIANT *error_msg)

  - Get calibration results for given date, from the database.
  -	Send results to client for current date.

INPUT
	*calibrationDate - Date of chemistry results found.
	*Description - Result Labels.
	*Values - Calibration values found in database.
	*Disposition - Status of results.
	*Editable - Change permissions of resulsts.
	*MinLimits - Lower limits for current report values.
	*MaxLimits - Upper limits for current report values.
	*perror_msg - Error message.

OUTPUT
	*Description - Result Labels.
	*Values - Calibration values found in database.
	*Disposition - Status of results.
	*Editable - Change permissions of resulsts.
	*perror_msg - Error message.

LOCAL
	hr - System result codes.
	algorithmType - Type of algorithm for current calibration.

  VB
1	lngCalID_m				LONG	calibrationID, 
2	strChemistry_m			BSTR	*chemistry, 
3	strWavelength_m			BSTR	*waveLength, 
4	strSpecies_m			BSTR	*species, 
5	strDiluent_m			BSTR	*diluent, 
6	strMold_m				BSTR	*mold, 
7	strRotor_m				BSTR	*rotor, 
8	strCuvette_m			BSTR	*cuvette, 
9	strMode_m				BSTR	*mode, 
10	strUser_m				BSTR	*user, 
11	strCalDate_m			BSTR	*date, 
12	passDescriptions(0)		VARIANT *description, 	
13	passValues(0)			VARIANT *originalFitParameters, 	
14	passAdjValues(0)		VARIANT *newFitParameters, 	
15	passMinLimits(0)		VARIANT *minLimits, 	
16	passMaxLimits(0)		VARIANT *maxLimits, 	
17	passDispositions(0)		VARIANT *originalDisposition, 	
18	passAdjDispositions(0)	VARIANT *newDisposition, 	
19	passExpPoint(0)			VARIANT *calibrationData, 	
20	passExpPointStatus(0)	VARIANT *statusData, 	
21	passCalcExpPoint(0)		VARIANT *rates, 	
22	passPlots(0)			VARIANT *plotData, 	
23	strComment_m			BSTR	*comment, 
							VARIANT *error_msg)	

*****************************************************************/
STDMETHODIMP QalibServerMain::SendCalibrationResults(
				LONG	calibrationID, 
				BSTR	*chemistry, 
				BSTR	*waveLength, 
				BSTR	*species, 
				BSTR	*diluent, 
				BSTR	*mold, 
				BSTR	*rotor, 
				BSTR	*cuvette, 
				BSTR	*mode, 
				BSTR	*user, 
				BSTR	*date, 
				VARIANT *description, 
				VARIANT *originalFitParameters, 
				VARIANT *newFitParameters, 
				VARIANT *minLimits, 
				VARIANT *maxLimits, 
				VARIANT *originalDisposition, 
				VARIANT *newDisposition, 
				VARIANT *calibrationData, 
				VARIANT *statusData, 
				VARIANT *rates, 
				VARIANT *plotData, 
				BSTR	*comment, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	CString		algorithmType;
	char		buf[512];

CString temp="&&&&&&&&&&&&&&&&&&&& BEGIN SendCalibrationResults &&&&&&&&&&&&&&&&&&&&";
DisplayCString(&temp);

	// Recovered values for calibrators and controls.
	hr = GetPlotResults(calibrationID, plotData);
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Problem getting recovered values",	error_msg);

		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"GetPlotResults\"", buf);
		return hr;
	}

	// Get calibration values to client. To update report.
	hr = GetCalibrationValues(calibrationID, 
							  chemistry,
							  species,
							  diluent,
							  mold,
							  rotor,
							  cuvette,
							  mode,
							  user,
							  date,
							  comment,
							  error_msg);
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Problem getting calibration values", error_msg);
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"GetCalibrationValues\"", buf);
		return hr;
	}

	// Chemistry wavelength.
	hr = GetChemistryWavelength(*chemistry, waveLength);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"GetChemistryWavelength\"", buf);
		return hr;
	}

	// Get calibration values and "values' status" to send to client.
	hr = GetCalibrationDataAndStatus(calibrationID,
									 calibrationData, 
									 statusData);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"GetCalibrationDataAndStatus\"",	buf);
		return hr;
	}

	// Results depend on what algorithm the calibration used.
	hr = FindCurrentAlgorithmType(calibrationID, &algorithmType);
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Can't find calibration algorithm type", error_msg);
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"FindCurrentAlgorithmType\"", buf);
		return hr;
	}

	// Rates from assigned values.
	hr = RatesFromAssignedValues(calibrationID, 
								 algorithmType, 
								 FitParameters, 
								 rates, 
								 error_msg);
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Problem calculating rates from assigned values", error_msg);
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"RatesFromAssignedValues\"",	buf);
		return hr;
	}

	// Data to display in the report.
	hr = CalibrationResults(calibrationID,
							algorithmType,
							description,
							originalFitParameters,
							newFitParameters,
							minLimits,
							maxLimits,
							originalDisposition,
							newDisposition,
							error_msg);
	if (FAILED(hr))
	{
		UserErrorMessage("3", "Failed to get report results", error_msg);
		sprintf(buf,"Function \"SendCalibrationResults\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"CalibrationResults\"", buf);
		return hr;
	}

	// Set default error message.
	OKMessage(error_msg);

temp="&&&&&&&&&&&&&&&&&&&& END SendCalibrationResults &&&&&&&&&&&&&&&&&&&&";
DisplayCString(&temp);

	return S_OK;
}

/*****************************************************************
STDMETHODIMP QalibServerMain::SendAssignedValues(
				BSTR chemistry, 
				const VARIANT *sampleIDs, 
				VARIANT *curAssignedValues, 
				VARIANT *error_msg)

- Send assigned values to client, from the database.

INPUT
	chemistry - Current chemistry name.
	sampleIDs - List of sample ids, to get assigned values for. 
	*curAssignedValues - Results to return to client.
	error_msg - Error messages.

OUTPUT
	*curAssignedValues - Contains assigned values to return to client.

LOCAL
	hr - System result codes.
	ulPassArraySize - Number of assigned values to send to client.
	curValue - Current data value.
	lLCID - Language code for numerical to string conversion.
	k - Index to current position in SAFEARRAY to pass to client.
	CstrKey - Convert sample id to CString type.
	numberOfSamples - Number of samples, passed as first element in sampleIDs.
	localSampleIDList - List of sample ids.
	i - Index for list traversal.

*****************************************************************/
STDMETHODIMP QalibServerMain::SendAssignedValues(
				BSTR chemistry, 
				const VARIANT *sampleIDs, 
				VARIANT *curAssignedValues, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	ULONG		ulPassArraySize = 0;
	double		curValue;
	DWORD		lLCID;
	long		k;
	CString		CstrKey;
	CString		partNumber;
	short		numberOfSamples;
	char		buf[512];
	CSortStringArray		localSampleIDList;
	CString					sampleIDList;
	CArray<double,double>	assignedValues;

	// Find a Language code, used by conversion functions. i.e. VarR8FromStr.
	lLCID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];  

	//*** define the safearray ***
    SAFEARRAY FAR* psa = NULL;

	//*** define a BSTR to move the strings in and out ***
    BSTR bstr = NULL;

	//*** get the safearray ***
	psa = V_ARRAY(sampleIDs);

	//*** define access number ***
	k = 0;

	// Get number of sets of data.
	hr = SafeArrayGetElement(psa, &k, &bstr);

	// Convert data value from string to integer.
	hr = VarI2FromStr(bstr, lLCID, LOCALE_NOUSEROVERRIDE, &numberOfSamples);

	// Copy sample id list to local variable.
	for (short j=0; j<numberOfSamples; j++)
	{
		k++;
		// Get next sample id.
		hr = SafeArrayGetElement(psa, &k, &bstr);
		// Convert sample id to CString type.
		CstrKey = bstr;

		// Save sample id in local list.
		localSampleIDList.Add(CstrKey);
	}

	hr = FromCStringArrayToCString(&localSampleIDList, &sampleIDList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	hr = GetAssignedValues((CString)chemistry, sampleIDList, &assignedValues, error_msg);
 	if (FAILED(hr))
 	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3", "Problem with getting assigned values", error_msg);

			sprintf(buf,"Function \"SendAssignedValues\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr, "Return from \"GetAssignedValues\" function",buf);
		}

 		return hr;
 	}

	// Calculate size of array to pass to client.
	ulPassArraySize = localSampleIDList.GetUpperBound() + 1;

	// Set size of array to pass to client.
    sabound[0].cElements = ulPassArraySize;
    sabound[0].lLbound = 0;
	//*** create the safearray to return ***
    psa = SafeArrayCreate(VT_R8, 1, sabound);

	// Set return array index.
	k = 0;

	// Put assigned values in VARIANT to return to client.
	for (int i=0; i<assignedValues.GetSize(); i++)
	{
		curValue = assignedValues.GetAt(i);
		// Put current assigned value in array to return.
		hr = SafeArrayPutElement(psa, &k, &curValue);

		k++;
	}

	// Set the return array's type, array of doubles.
	//*** set up the safearray to export it as an array of BSTR  
	V_VT(curAssignedValues) = VT_ARRAY | VT_R8;

	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
    V_ARRAY(curAssignedValues) = psa;

	// Set default error message.
	OKMessage(error_msg);

	return S_OK;
}

/*****************************************************************
STDMETHODIMP CQalibT4ServerCalculations::SendEditableFitParameters
							(LONG calibrationID, 
							 VARIANT *editableFitParameters, 
							 VARIANT *error_msg)

  Send fit parameters that are editable, for the current calibration.

INPUT
	calibrationID		   - Calibration id.
	*editableFitParameters - Parameters that are editable.
	*error_msg			   - Error messages.

OUTPUT
	Editable fit parameters.

LOCAL
	hr - Return code.
	db - Database access object.
	session - Database session object.
    psa_EditableFitParams -	safearray, to pass data to caller.
	sabound[1] - Bounds for return structure.
	k - Index in safearray return structure.

*****************************************************************/
STDMETHODIMP QalibServerMain::SendEditableFitParameters(
				LONG calibrationID, 
				VARIANT *editableFitParameters, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	CDataSource db;
	CSession	session;
	CString		algorithmType;
	BSTR		bstr = NULL;
	CString		temp;

	// Instantiate a command object, to get calibration algorithm.
	CCommand<CAccessor<CdboGetCalibrationAlgorithmAccessor> > cmdGetCalibrationAlgorithm;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "SendEditableFitParameters" );
	if (FAILED(hr))
	{
		UserErrorMessage("3","Can't access database to get editable fit parameters",error_msg);
		LogErrorMessage(hr,"Return from \"OpenDatabase\"","SendEditableFitParameters");
		return hr;
	}

	// Create command to get algorithm.
	hr = cmdGetCalibrationAlgorithm.Create(session, _T("{ ? = CALL dbo.GetCalibrationAlgorithm;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		UserErrorMessage("3","Can't create command to get editable fit parameters",error_msg);
		LogInterfaceErrorMessage(hr,"Create command for \"GetCalibrationAlgorithm\" stored procedure","SendEditableFitParameters");
		return hr;
	}

	// Set database searching criteria.
	// Current calibration id.
	cmdGetCalibrationAlgorithm.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure, to get calibration algorithm.
	hr = cmdGetCalibrationAlgorithm.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		UserErrorMessage("3","Can't get editable fit parameters",error_msg);
		LogInterfaceErrorMessage(hr,"Call \"GetCalibrationAlgorithm\" stored procedure","SendEditableFitParameters");
		return hr;
	}

	algorithmType = cmdGetCalibrationAlgorithm.m_Algorithm;
	cmdGetCalibrationAlgorithm.Close();

	// Close the database.
	db.Close();

	hr = GetEditableFitParameters(calibrationID, algorithmType, editableFitParameters);
	if (FAILED(hr))
	{
		UserErrorMessage("3","Can't get editable fit parameter's values",error_msg);
		LogErrorMessage(hr,"Return from function \"GetEditableFitParameters\"","SendEditableFitParameters");
		return hr;
	}

	// Set default error message.
	OKMessage(error_msg);

	return S_OK;
}

/*****************************************************************
STDMETHODIMP CQalibT4ServerCalculations::SetComment
						(LONG calibrationID, 
						 BSTR *comment, 
						 VARIANT *error_msg)

  Update comment in calibration with given id.

INPUT
	calibrationID - Calibration id.
	*comment	  - Comment to add.
	*error_msg	  - Error messages.

OUTPUT
	None.

LOCAL
	hr - Return code.
	db - Database access object.
	session - Database session object.

*****************************************************************/
STDMETHODIMP QalibServerMain::SetComment(
				LONG calibrationID, 
				BSTR *comment, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Define command to set comment.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboSetCommentAccessor> > cmdSetComment;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "SetComment" );
	if (FAILED(hr))
	{
		UserErrorMessage("3","Can't open database to set comment",error_msg);

		LogErrorMessage(hr,"Return from \"OpenDatabase\"","SetComment");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdSetComment.Create(session, _T("{ ? = CALL dbo.SetComment;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		UserErrorMessage("3","Can't create command to call database to set comment",error_msg);

		LogInterfaceErrorMessage(hr,"Create command for \"SetComment\" stored procedure","SetComment");
		return hr;
	}

	// Set database searching criteria.
	// Current calibration id.
	cmdSetComment.m_calibrationID = calibrationID;

	// Set database updating criteria.
	_tcsncpy(cmdSetComment.m_comment, (CString)*comment,
		sizeof(cmdSetComment.m_comment) /
		sizeof(cmdSetComment.m_comment[0]));
 
	// Execute current command.
	// Call stored procedure, "SetComment".
	hr = cmdSetComment.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		UserErrorMessage("3","Can't set comment",error_msg);

		LogInterfaceErrorMessage(hr,"Call \"SetComment\" stored procedure","SetComment");
		return hr;
	}

	cmdSetComment.Close();

	// Close the database.
	db.Close();

	// Set default error message.
	OKMessage(error_msg);

	return S_OK;
}

/*****************************************************************
STDMETHODIMP QalibServerMain::SendUserRights(
				BSTR user, 
				BSTR password, 
				VARIANT *modeList, 
				VARIANT *chemistryList, 
				VARIANT *error_msg)

	Find the access privilages for current user.

INPUT
	user - Id of user to verify access privilages.
	password - Password of current user.
	modeList - List of mode of access.
	chemistryList - Chemistries current user is allowed to calibrate.
	error_msg - Error reporting.

OUTPUT

LOCAL
	hr - return code.
	seps[] - Characters used by strtok as delimeters.
	*sampleid - Current sample id.
	db - Database access structure.
	session - Session access structure.
	totalNumberOfValues - Number of values.
	noOutliersNumberOfValues - Number of values without outliers.
	cmdSampleSizes - Command to access stored procedure.
	cmdinit - Set properties for command.

*****************************************************************/
STDMETHODIMP QalibServerMain::SendUserRights(
				BSTR user, 
				BSTR password, 
				VARIANT *modeList, 
				VARIANT *chemistryList, 
				VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	CStringArray	tmpModeList;
	CStringArray	tmpChemistryList;
	char			*token;
	char			seps[] = ",\n";
	char			buf[512];
	CString			temp;
	bool			userAccess = true;

	CCommand<CAccessor<CdboUserAllowedAccessAccessor> > cmdUserDbAccess;

	// Does user have database access?
	hr = TestUserAccess(user, password, error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_EMPTY)
		{
			CString temp;
			temp = user;
			sprintf(buf,"User \"%s\" does not have database access!", (LPCTSTR)temp);
			UserErrorMessage("3", buf, error_msg);
		}
						
		return hr;
	}


	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// < --------------- GET USER DATABASE ACCESS --------------- >
	// Open database.
	hr = OpenDatabase( &db, &session, "SendUserRights" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendUserRights\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return hr;
	}

	// Create command to execute stored procedure "UserAllowedAccess".
	hr = cmdUserDbAccess.Create(session, _T("{ ? = CALL dbo.UserAllowedAccess;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendUserRights\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"UserAllowedAccess\" stored procedure",buf);
		return hr;
	}

	// Set database searching criteria.
 	_tcsncpy(cmdUserDbAccess.m_UserName, (CString)user,
 		sizeof(cmdUserDbAccess.m_UserName) /
 		sizeof(cmdUserDbAccess.m_UserName[0]));

	// Get user's access information.
	hr = cmdUserDbAccess.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SendUserRights\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Call \"UserAllowedAccess\" stored procedure", buf);
		return hr;
	}

	temp = user;
	// Is current user allowed to access the database?
	switch (cmdUserDbAccess.m_RETURNVALUE)
	{
		case 1:	// -- Is current user a database user?
			sprintf(buf,"User \"%s\" is not a database user!", (LPCTSTR)temp);
			UserErrorMessage("3", buf, error_msg);			
			return 0x80040E21;
		break;
		case 2:	// -- Does current user have database access?
			sprintf(buf,"User \"%s\" does not have database access!", (LPCTSTR)temp);
			UserErrorMessage("3", buf, error_msg);			
			return 0x80040E21;
		break;
		case 3:	// -- Does current user have Qalibration database access?
			if (!userAccess)
			{
				sprintf(buf,"Illegal password!", (LPCTSTR)temp);
			}
			else
			{
				sprintf(buf,"User \"%s\" does not have \"Qalibration\" database access!", (LPCTSTR)temp);
			}
			UserErrorMessage("3", buf, error_msg);			
			return 0x80040E21;
		break;
		case 4:	// Is the current user allowed to calibrate any chemistries?
			sprintf(buf,"User \"%s\" is not allowed to calibrate any chemistries!", (LPCTSTR)temp);
			UserErrorMessage("3", buf, error_msg);			
			return 0x80040E21;
		break;
	}

	// Put mode in local mode list.
	token = strtok( cmdUserDbAccess.m_ModeList, seps );
	while( token != NULL )
	{
		tmpModeList.Add(token);
		// Get mode.
		token = strtok( NULL, seps );
	}

	// Put chemistry in local list.
	token = strtok( cmdUserDbAccess.m_ChemistryList, seps );
	while( token != NULL )
	{
		tmpChemistryList.Add(token);
		// Get chemistry.
		token = strtok( NULL, seps );
	}

	cmdUserDbAccess.Close();

	// Close the database.
	db.Close();
	// </ --------------- GET USER DATABASE ACCESS --------------- >

	CString tmp;

	//*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	//*** define the safearray ***
    SAFEARRAY FAR* psa = NULL;

	BSTR bstr=NULL;

	// < --------- PUT MODE LIST IN RETURN ARRAY ---------- >
	// Set size of array to pass to client.
    sabound[0].cElements = tmpModeList.GetSize();
    sabound[0].lLbound = 0;
	//*** create the safearray to return ***
    psa = SafeArrayCreate(VT_BSTR, 1, sabound);

	// Set return array index.
	long k = 0;

	// Put list of modes in VARIANT to return to client.
	for (int i=0; i<tmpModeList.GetSize(); i++)
	{
		tmp = tmpModeList.GetAt(i);
		bstr = tmp.AllocSysString();
		hr = SafeArrayPutElement(psa, &k, bstr);

		k++;
	}

	// Set the return array's type, array of strings.
	//*** set up the safearray to export it as an array of BSTR  
	V_VT(modeList) = VT_ARRAY | VT_BSTR;

	// Set return array to contain mode list.
	//*** put the pointer into the VARIANT ***
    V_ARRAY(modeList) = psa;
	// </ --------- PUT MODE LIST IN RETURN ARRAY ---------- >

	// < --------- PUT CHEMISTRY LIST IN RETURN ARRAY ---------- >
	// Set size of array to pass to client.
    sabound[0].cElements = tmpChemistryList.GetSize();
    sabound[0].lLbound = 0;
	//*** create the safearray to return ***
    psa = SafeArrayCreate(VT_BSTR, 1, sabound);

	// Set return array index.
	k = 0;

	// Put assigned values in VARIANT to return to client.
	for (i=0; i<tmpChemistryList.GetSize(); i++)
	{
		tmp = tmpChemistryList.GetAt(i);
		bstr = tmp.AllocSysString();
		hr = SafeArrayPutElement(psa, &k, bstr);

		k++;
	}

	// Set the return array's type, array of strings.
	//*** set up the safearray to export it as an array of BSTR  
	V_VT(chemistryList) = VT_ARRAY | VT_BSTR;

	// Set return array to contain chemistry list.
	//*** put the pointer into the VARIANT ***
    V_ARRAY(chemistryList) = psa;
	// </ --------- PUT CHEMISTRY LIST IN RETURN ARRAY ---------- >

	// Set default error message.
	OKMessage(error_msg);

	return S_OK;
}
