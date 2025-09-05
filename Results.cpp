#include "stdafx.h"
#include "dboGetCalibrationValues.H"
#include "dboGetAnyFitParameters.H"
#include "dboGetPlotResults.H"
#include "DbAccess.h"
#include "Loging.h"
#include "Support.h"
#include "globals.h"
#include "Outliers.h"
#include "Results.h"
#include "SigmoidalResults.h"
#include "LinearResults.h"
#include "Utils.h"
#include "Rotor.h"
#include "dboGetChemistryWavelength.H"
#include "QuadraticResults.h"
#include "LinearResults.h"
#include "SigmoidalResults.h"

/*******************************************************************

FILENAME:	Results.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Main module for calibration report processing.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:
	HRESULT CalibrationResults

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	Results.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/

/*******************************************************
HRESULT CalibrationResults(BSTR *calibrationDate,
						   VARIANT *Description,
						   VARIANT *Values,
						   VARIANT *MinLimits,
						   VARIANT *MaxLimits,
						   VARIANT *error_msg)

Get calibration results.
Format results to send to client for report.

--------------------------------------------------------
INPUT
	*calibrationDate - Calibration date.
	*Description - Label of value.
	*Values - Numerical magnitude of value.
	*MinLimits - Minimum accepted value.
	*MaxLimits - Maximum accepted value.

--------------------------------------------------------
OUTPUT
	Sample lengths.

--------------------------------------------------------
LOCAL
	hr - Return code.
	curValue - Current value.
	lLCID - Language code, used by conversion functions. i.e. VarR8FromStr.
	i - Index to SAFEARRAYs.
	db - Database access object.
	session - Database session object.
    bstr - Define a BSTR to move the strings in and out.
	cur_max_value - Maximum values allowed.
	cur_min_value - Minimum values allowed.
	cmdGetResults - Define command to get data from database.
	statusTrue - String to hold "PASS" label.
	statusFalse - String to hold "FAIL" label.
	yChange - String to hold allow change lable.
	nChange - String to hold do not allow change lable. 

--------------------------------------------------------
RETURN
	Status code, hr.

*******************************************************/
HRESULT CalibrationResults(LONG	calibrationID,
						   CString algorithm,
						   VARIANT *description, 
						   VARIANT *originalFitParameters, 
						   VARIANT *newFitParameters, 
						   VARIANT *minLimits, 
						   VARIANT *maxLimits, 
						   VARIANT *originalDisposition, 
						   VARIANT *newDisposition,
						   VARIANT *error_msg)
{
	HRESULT		hr;

	CDataSource db;
	CSession	session;

	// Set report description labels.
	hr = SetDescriptionLabels( algorithm, description);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"SetDescriptionLabels\"","CalibrationResults");
		return hr;
	}

	// Get original fit parameters.
	hr = GetFitParameters( calibrationID, 0, algorithm, originalFitParameters);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetFitParameters\". Original fit parameters","CalibrationResults");
		return hr;
	}

	// Get new fit parameters.
	hr = GetFitParameters( calibrationID, 1, algorithm, newFitParameters);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetFitParameters\". New fit parameters","CalibrationResults");
		return hr;
	}

	// Get maximum and minimum limits.
	hr = GetMaxAndMinLimits(calibrationID, minLimits, maxLimits, algorithm, error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetMaxAndMinLimits\"","CalibrationResults");
		return hr;
	}

	// Set disposition for new fit parameters.
	hr = GetDisposition(algorithm,
						newFitParameters, 
						minLimits, 
						maxLimits, 
						newDisposition);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetDisposition\". New fit parameters","CalibrationResults");
		return hr;
	}

	// Set disposition for original fit parameters.
	hr = GetDisposition(algorithm,
						originalFitParameters, 
						minLimits, 
						maxLimits, 
						originalDisposition);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetDisposition\". Original fit parameters","CalibrationResults");
		return hr;
	}

	return S_OK;
}

/*************************************************************
HRESULT GetCalibrationValues
					(LONG calibrationID, 
					 BSTR *partNumber, 
					 BSTR *species, 
					 BSTR *diluent, 
					 BSTR *mold, 
					 BSTR *rotor, 
					 BSTR *cuvette,
					 BSTR *mode,
					 BSTR *user,
					 BSTR *date,
					 BSTR *comment)

--------------------------------------------------------------
 INPUT
	*calibrationID - Identifier for current calibration.
	*partNumber - Part number.
	*species - Calibration species.
	*diluent - Diluent for calibration.
	*mold - Mold for rotor.
	*rotor - Rotor number.
	*cuvette - Cuvette containing chemistry to calibrate.
	*mode - Mode of operation.
	*user - Current user.
	*date - Date of calibration.
	*comment - Comment on fit parameters.

--------------------------------------------------------------
 OUTPUT
	Values used for current calibration.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.
	cmdGetCalibrationValues - Object for db results.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT GetCalibrationValues(LONG calibrationID, 
							 BSTR *chemistryName, 
							 BSTR *species, 
							 BSTR *diluent, 
							 BSTR *mold, 
							 BSTR *rotor, 
							 BSTR *cuvette,
							 BSTR *mode,
							 BSTR *user,
							 BSTR *date,
							 BSTR *comment,
							 VARIANT *error_msg)
{
	CDataSource db;
	CSession	session;
	HRESULT		hr;
	char		buf[512];
	CString		lChemistry,lRotor;

	// Define command to get data from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetCalibrationValuesAccessor> > cmdGetCalibrationValues;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationValues" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetCalibrationValues");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetCalibrationValues.Create (session, _T("{ ? = CALL dbo.GetCalibrationValues;1 (?) }"));
 	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetCalibrationValues\"","GetCalibrationValues");
		return hr;
	}

	// Set database searching criteria.
	// Calibration id, used to query db.
	cmdGetCalibrationValues.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetCalibrationValues.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetCalibrationValues\"","GetCalibrationValues");
		return hr;
	}

	CString temp;

	cmdGetCalibrationValues.MoveFirst();

	lChemistry = cmdGetCalibrationValues.m_colChemistryName;
	*chemistryName = lChemistry.AllocSysString();

	temp = cmdGetCalibrationValues.m_colMold;
	*mold = temp.AllocSysString();

	lRotor = cmdGetCalibrationValues.m_colRotor;
	*rotor = lRotor.AllocSysString();

	temp = cmdGetCalibrationValues.m_colSpecies;
	*species = temp.AllocSysString();

	temp = cmdGetCalibrationValues.m_colMode;
	*mode = temp.AllocSysString();
	
	temp = cmdGetCalibrationValues.m_colUser;
	*user = temp.AllocSysString();
	
	temp = cmdGetCalibrationValues.m_column0;
	*date = temp.AllocSysString();

	temp = cmdGetCalibrationValues.m_colComment;
	*comment = temp.AllocSysString();

	cmdGetCalibrationValues.Close();
	
	// Close the database.
	db.Close();

	// < ----- GET CURRENT DILUENT ----- >
	hr = GetCalibrationIDDiluent(calibrationID,&temp);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"GetCalibrationIDDiluent\" function", buf);
		return hr;
	}

	*diluent = temp.AllocSysString();
	// </ ----- GET CURRENT DILUENT ----- >

	// < ----- CUVETTE NUMBER ----- >
	CStringArray rotorAttributes;
	CString		 lrotorAttributes;

	hr = GetRotorCuvetteAttributes(lRotor,lChemistry,&rotorAttributes,error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"GetRotorCuvetteAttributes\" function", buf);
		return hr;
	}

	hr = FromCStringArrayToCString(&rotorAttributes,&lrotorAttributes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"GetRotorCuvetteAttributes\" function", buf);
		return hr;
	}

	*cuvette = lrotorAttributes.AllocSysString();
	// </ ----- CUVETTE NUMBER ----- >

	return S_OK;
}


/*************************************************************
HRESULT GetCalibrationDataAndStatus(LONG calibrationID, 
									VARIANT *calibrationData,
									VARIANT *dataStatus)

--------------------------------------------------------------
 INPUT
	*calibrationID - Identifier for current calibration.
	*calibrationData - Return calibration data.
	*dataStatus - Return calibration data's status.

--------------------------------------------------------------
 OUTPUT
	Values used for current calibration.
	Status of values in current calibration.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.
	cmdGetCalibrationDataAndStatus - Object for db results.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT GetCalibrationDataAndStatus(LONG calibrationID, 
									VARIANT *calibrationData,
									VARIANT *dataStatus)
{
	HRESULT					hr;
	long					i;
	CArray<int,int>			sampleSizes;
	CArray<int,int>			calibrationStatus;
	CArray<double,double>	l_calibrationData;
	CStringArray			calibrationResults;
	
	//*** define the safearray ***
    SAFEARRAY FAR* psa_StatusData = NULL;
    SAFEARRAY FAR* psa_Data = NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND data_sabound[1];
    SAFEARRAYBOUND status_sabound[1];

	hr = GetCalibrationData(calibrationID,&calibrationResults,&calibrationStatus);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetCalibrationData\"","GetCalibrationDataAndStatus");
		return hr;
	}

	// Calculate size of SAFEARRAY to pass to client.
	LONG NumberOfRows = calibrationResults.GetSize();

	// Set size of array to pass to client.
	data_sabound[0].cElements = NumberOfRows;
	data_sabound[0].lLbound = 0;

	//*** create the safearray to return ***
    psa_Data = SafeArrayCreate(VT_BSTR, 1, data_sabound);    

	CString curID;
	BSTR	bstr=NULL;

	i=0;

	// Put sample sizes in return SAFEARRAY. Metadata.
	for (int k=0; k<calibrationResults.GetSize(); k++)
	{
		// Get current sample length and convert it to a string.
		curID = calibrationResults.GetAt(k);
		bstr = curID.AllocSysString();
		// Put current sample length in return SAFEARRAY.
		hr = SafeArrayPutElement(psa_Data, &i, bstr);
		i++;
	}

	// Set return status SAFEARRAY.
	status_sabound[0].cElements = calibrationStatus.GetSize();
	status_sabound[0].lLbound = 0;
	psa_StatusData = SafeArrayCreate(VT_I4, 1, status_sabound);    
	
	i=0;
	int tmpStatus;
	for (k=0; k<calibrationStatus.GetSize(); k++)
	{
		tmpStatus = calibrationStatus.GetAt(k);
		// Put current value status in return SAFEARRAY.
		hr = SafeArrayPutElement(psa_StatusData, &i, &tmpStatus);
		i++;
	}

	// Set status array type to 32-bit integer.
	//*** set up the safearray to export it as an array of integers.  
	V_VT(dataStatus) = VT_ARRAY | VT_I4;
    V_ARRAY(dataStatus) = psa_StatusData;

	// Set calibration value's array type to double.
	V_VT(calibrationData) = VT_ARRAY | VT_BSTR;
    V_ARRAY(calibrationData) = psa_Data;

	return S_OK;
}

/*************************************************************
HRESULT SetDescriptionLabels(CString algorithm,
							 VARIANT *description)

--------------------------------------------------------------
 INPUT
	algorithm - Current calibration algorithm.
				Report format depends on algorithm.

	*description - Description lables.

--------------------------------------------------------------
 OUTPUT
	Description labels for current calibration.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.
	cmdGetCalibrationDataAndStatus - Object for db results.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT SetDescriptionLabels(CString algorithm,
							 VARIANT *description)
{
	HRESULT			hr=S_OK;
	char			buf[512];

	// Report format depends on chemistry's algorithm.
	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = SetSigmoidalDescriptionLabels(description);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"SetDescriptionLabels\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"SetSigmoidalDescriptionLabels\"",buf);
			return hr;
		}
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		hr = SetLinearDescriptionLabels(description);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"SetDescriptionLabels\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"SetLinearDescriptionLabels\"",buf);
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		hr = SetQuadraticDescriptionLabels(description);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"SetDescriptionLabels\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"SetQuadraticDescriptionLabels\"",buf);
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = SetLinearDescriptionLabels(description);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"SetDescriptionLabels\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"SetLinearDescriptionLabels\"",buf);
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
		LogErrorMessage(0x80040E21,"Unknown algorithm type","SetDescriptionLabels");
		return hr;
	}

	return S_OK;
}

/*************************************************************
HRESULT GetFitParameters(LONG calibrationID, 
						 VARIANT *originalFitParameters, 
						 VARIANT *newFitParameters)

--------------------------------------------------------------
 INPUT
	calibrationID - Calibration id.
	*originalFitParameters - To return original fit parameters.
	*newFitParameters - To return new fit parameters.

--------------------------------------------------------------
 OUTPUT
	Original and new fit parameters.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.
	cmdGetFitParameters - Object for db results.
	i - Index to returned safearrays.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT GetFitParameters(LONG calibrationID,
						 INT fitParameterChoice,
						 CString algorithm,
						 VARIANT *fitParameters)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	long		i;
	char		seps[] = ",\n";
	char		*token;
	double		curValue;
	CArray<double,double> l_fitParams;

	//*** define the safearray, for return VARIANTs ***
    SAFEARRAY FAR* psa_fitParameters = NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	// Define command to get data and status from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetAnyFitParametersAccessor> > cmdGetFitParameters;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetFitParameters");
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetFitParameters");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetFitParameters.Create (session, _T("{ ? = CALL dbo.GetAnyFitParameters;1 (?,?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetAnyFitParameters\"","GetAnyFitParameters");
		return hr;
	}

	// Set database searching criteria.
	// Calibration id, used to query db.
	cmdGetFitParameters.m_calibrationID = calibrationID;

	cmdGetFitParameters.m_whichFitParameters = fitParameterChoice;

	_tcsncpy(cmdGetFitParameters.m_algorithm, algorithm,
		sizeof(cmdGetFitParameters.m_algorithm) /
		sizeof(cmdGetFitParameters.m_algorithm[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetFitParameters.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetAnyFitParameters\"","GetFitParameters");
		return hr;
	}

	// Convert sample ids.
	token = strtok( cmdGetFitParameters.m_fitParameters, seps );
	while( token != NULL )
	{
		l_fitParams.Add(atof(token));
		// Get next sample id.
		token = strtok( NULL, seps );
	}

	cmdGetFitParameters.Close();

	// Close the database.
	db.Close();

	// Set size of return array.
	sabound[0].cElements = l_fitParams.GetSize();
	sabound[0].lLbound = 0;
	//*** create the safearray to return ***
    psa_fitParameters = SafeArrayCreate(VT_R8, 1, sabound);    

	i = 0;

	for (int k=0; k<l_fitParams.GetSize(); k++)
	{
		curValue = l_fitParams.GetAt(k);
		hr = SafeArrayPutElement(psa_fitParameters, &i, &curValue);
		i++;
	}

	// Set status array type to double.
	//*** set up the safearray to export it as an array of doubles.
	V_VT(fitParameters) = VT_ARRAY | VT_R8;
    V_ARRAY(fitParameters) = psa_fitParameters;

	return S_OK;
}

/*************************************************************
HRESULT	GetMaxAndMinLimits(LONG	calibrationID,
						   VARIANT *minLimits, 
						   VARIANT *maxLimits,
						   CString algorithm,
						   VARIANT *error_msg)

--------------------------------------------------------------
 INPUT
	calibrationID - Calibration id.
	*minLimits - Return minimum limits.
	*maxLimits - Return maximum limits.
	algorithm - Algorithm of current chemistry.

--------------------------------------------------------------
 OUTPUT
	Minimum and maximum fit parameter limits.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	i - Index to returned safearrays.
	cur_max_value - Maximum value for a double.
	cur_min_value = Minimum value for a double.
    sabound[1] - Define the SAFEARRAYBOUND structure.
				 Bounds for return arrays.
	psaMinLimits - Safearray to hold minimum limits.
    psaMaxLimits - Safearray to hold maximum limits.

--------------------------------------------------------------
 RETURN
	hr - Error code.

*************************************************************/
HRESULT	GetMaxAndMinLimits(LONG	calibrationID,
						   VARIANT *minLimits, 
						   VARIANT *maxLimits,
						   CString algorithm,
						   VARIANT *error_msg)
{
	HRESULT	hr;

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = GetSigmoidalMaxAndMinLimits(calibrationID,
										 minLimits, 
										 maxLimits,
										 algorithm,
										 error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from \"GetSigmoidalMaxAndMinLimits\" function","GetMaxAndMinLimits");
			return hr;
		}
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		hr = GetLinearMaxAndMinLimits(calibrationID,
									  minLimits, 
									  maxLimits,
									  algorithm,
									  error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from \"GetLinearMaxAndMinLimits\" function","GetMaxAndMinLimits");
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		hr = GetQuadraticMaxAndMinLimits(calibrationID,
										 minLimits, 
										 maxLimits,
										 algorithm,
										 error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from \"GetQuadraticMaxAndMinLimits\" function","GetMaxAndMinLimits");
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = GetLinearMaxAndMinLimits(calibrationID,
									  minLimits, 
									  maxLimits,
									  algorithm,
									  error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr,"Return from \"GetLinearMaxAndMinLimits\" function","GetMaxAndMinLimits");
			return hr;
		}
	}

	return S_OK;
}

/*************************************************************
HRESULT GetDisposition(CString algoritmn,
					   VARIANT *fitParameters,
					   VARIANT *minLimits,
					   VARIANT *maxLimits,
					   VARIANT *disposition)

  Sets disposition for each fit parameter.

--------------------------------------------------------------
 INPUT
	algorithm - Algorithm of current calibration.
	*fitParameters - List of fit parameters to set disposition.
	*minLimits - Lower limits for fit parameters.
	*maxLimits - Upper limits for fit parameters.
	*disposition - Dispositon status.

--------------------------------------------------------------
 OUTPUT
	Disposition status for fit parameters.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	buf - Error messages.
--------------------------------------------------------------
 RETURN
	hr - Error code.

*************************************************************/
HRESULT GetDisposition(CString algorithm,
					   VARIANT *fitParameters,
					   VARIANT *minLimits,
					   VARIANT *maxLimits,
					   VARIANT *disposition)
{
	HRESULT hr;
	char	buf[512];

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		hr = GetSigmoidalDisposition(fitParameters,minLimits,maxLimits,disposition);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetDisposition\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetSigmoidalDisposition\"",buf);
			return hr;
		}
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		hr = GetLinearDisposition(fitParameters,minLimits,maxLimits,disposition);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetDisposition\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetLinearDisposition\"",buf);
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		hr = GetQuadraticDisposition(fitParameters,minLimits,maxLimits,disposition);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetDisposition\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetQuadraticDisposition\"",buf);
			return hr;
		}
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		hr = GetLinearDisposition(fitParameters,minLimits,maxLimits,disposition);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetDisposition\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetLinearDisposition\"",buf);
			return hr;
		}
	}

	return S_OK;
}

// replaces "hr = RecoveredValuesFromRates(calibrationID, plotData);"
HRESULT GetPlotResults(LONG calibrationID,
					   VARIANT *plotData)
{
	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	CStringArray	l_plotData;
	BSTR			temp;
	char			*token;
	char			seps[] = ",\n";

	// Disposition safearray to return.
    SAFEARRAY FAR* psa_plotData = NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	// Instantiate a command object, to get assigned values.
	CCommand<CAccessor<CdboGetPlotResultsAccessor> > cmdGetPlotResults;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetPlotResults" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetPlotResults, Support.cpp");
		return hr;
	}

	// < --------------- GET ASSIGNED VALUES --------------- >
	// Create command to execute stored procedure, "GetPlotResults".
	hr = cmdGetPlotResults.Create(session, _T("{ ? = CALL dbo.GetPlotResults;1 (?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetPlotResults\" stored procedure","GetPlotResults, Support.cpp");
		return hr;
	}

	// Set database searching criteria.
	cmdGetPlotResults.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetPlotResults.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetPlotResults\" stored procedure","GetPlotResults, Support.cpp");
		return hr;
	}

	// Convert sample ids.
	token = strtok( cmdGetPlotResults.m_plotData, seps );
	while( token != NULL )
	{
		l_plotData.Add(token);
		// Get next sample id.
		token = strtok( NULL, seps );
	}

	cmdGetPlotResults.Close();
	// </ --------------- GET ASSIGNED VALUES --------------- >

	// Close the database.
	db.Close();

	// Set size of arrays to pass to client.
	sabound[0].cElements = l_plotData.GetSize();
	sabound[0].lLbound = 0;

	psa_plotData = SafeArrayCreate(VT_BSTR, 1, sabound);

	long i=0;

	for(int k=0; k<l_plotData.GetSize(); k++)
	{
		temp = l_plotData.GetAt(k).AllocSysString();

		hr = SafeArrayPutElement(psa_plotData, &i, temp);

		i++;
	}

	// Set status array type to BSTR.
	//*** set up the safearray to export it as an array of strings.
	V_VT(plotData) = VT_ARRAY | VT_BSTR;
    V_ARRAY(plotData) = psa_plotData;

	return S_OK;
}

HRESULT GetChemistryWavelength(BSTR chemistry, BSTR *waveLength)
{
	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	CStringArray	l_wavelengths;
	CString			t_wavelenths;
	CString			l_chemistry;
	char			buf[512];

	l_chemistry = chemistry;

	// Instantiate a command object, to get chemistry's wavelength.
	CCommand<CAccessor<CdboGetChemistryWavelengthAccessor> > cmdGetChemistryWavelength;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
//	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetChemistryWavelength" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetChemistryWavelength, Support.cpp");
		return hr;
	}

	// < ----- GET CHEMISTRY WAVELENGTH ----- >
	// Create command to execute stored procedure, "GetChemistryWavelength".
	hr = cmdGetChemistryWavelength.Create(session, _T("{ ? = CALL dbo.GetChemistryWavelength;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetPlotResults\" stored procedure","GetPlotResults, Support.cpp");
		return hr;
	}

	// Set database searching criteria.
// 	_tcsncpy(cmdGetChemistryWavelength.m_chemistryID, (CString)chemistry,
 	_tcsncpy(cmdGetChemistryWavelength.m_chemistryID, l_chemistry,
 		sizeof(cmdGetChemistryWavelength.m_chemistryID) /
 		sizeof(cmdGetChemistryWavelength.m_chemistryID[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetChemistryWavelength.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Call stored procedure\"GetChemistryWavelength\"","GetChemistryWavelength, Support.cpp");
//		LogInterfaceErrorMessage(hr,"Call \"GetChemistryWavelength\" stored procedure","GetPlotResults, Support.cpp");
		return hr;
	}

	while (cmdGetChemistryWavelength.MoveNext() == S_OK)
	{
		l_wavelengths.Add(cmdGetChemistryWavelength.m_colWavelength);
	}

	cmdGetChemistryWavelength.Close();
	// </ ----- GET CHEMISTRY WAVELENGTH ----- >

	// Close the database.
	db.Close();

	hr = FromCStringArrayToCString(&l_wavelengths, &t_wavelenths);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetChemistryWavelength\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"FromCStringArrayToCString\" function", buf);
		return hr;
	}

	*waveLength = t_wavelenths.AllocSysString();

	return S_OK;
}
