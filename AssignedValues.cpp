/*******************************************************************

FILENAME:	AssignedValues.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions to process assigned values.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	TestAssignedValueLimits
	GetAssignedValueLimits
	GetSampleAssignedValues
	GetAssignedValues

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	AssignedValues.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include "Loging.h"
#include "DbAccess.h"
#include "Utils.h"
#include "Support.h"
#include "AssignedValues.h"
#include "dboGetAssignedValueLimits.H"
#include "dboGetAssignedValues.H"
#include "SigmoidalCalculations.h"
#include "LinearCalibration.h"
#include "QuadraticCalculations.h"
#include "ExponentialCalculations.h"

/*****************************************************************
HRESULT TestAssignedValueLimits(CString chemistry,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues,
								VARIANT *error_msg)

	Test assigned value limits for current chemistry.

INPUT
	chemistry		- Current chemistry to calibrate. 
	*sampleIds		- List of samples.
	*assignedValues	- List of assigned values.
	error_msg		- Error reporting buffer.

OUTPUT
	sampleIds		- Updated list of samples.
	assignedValues	- Updated list of assigned values.

LOCAL
	hr					- Handle returned from safearray object.
	buf[512]			- Message buffer.
	l_sampleIds			- Temporary sample list.
	l_assignedValues	- Temporary assigned values list.
	assignedValueLimits - Assigned values limits.

RETURN
	Function status.

*****************************************************************/
HRESULT TestAssignedValueLimits(CString chemistry,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues,
								VARIANT *error_msg)
{
	HRESULT					hr;
	char					buf[512];
	CStringArray			l_sampleIds;
	CArray<double,double>	l_assignedValues;
	CArray<double,double>	assignedValueLimits;

	assignedValueLimits.RemoveAll();
	l_sampleIds.RemoveAll();
	l_assignedValues.RemoveAll();

	// < ----- GET ASSIGNED VALUES FOR CHEMISTRY ----- >
	hr = GetAssignedValueLimits(chemistry,&assignedValueLimits,error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"TestAssignedValueLimits\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetAssignedValueLimits\" function",buf);
		return hr;
	}
	// </ ----- GET ASSIGNED VALUES FOR CHEMISTRY ----- >

	// If the current chemistry has assigned values limits, test them.
	if (assignedValueLimits.GetSize() > 0)
	{
		for (int i=0; i<sampleIds->GetSize(); i++)
		{
			if ((assignedValues->GetAt(i) >= assignedValueLimits.GetAt(0)) && 
				(assignedValues->GetAt(i) <= assignedValueLimits.GetAt(1)))
			{
				l_sampleIds.Add(sampleIds->GetAt(i));
				l_assignedValues.Add(assignedValues->GetAt(i));
			}
		}

		sampleIds->RemoveAll();
		assignedValues->RemoveAll();
		sampleIds->Copy(l_sampleIds);
		assignedValues->Copy(l_assignedValues);
	}

	return S_OK;
}

/*****************************************************************
HRESULT GetAssignedValueLimits(CString	chemistry,
							   CArray<double,double> *assignedValueLimits,
							   VARIANT	*error_msg)

	Get assigned value limits for current chemistry.

INPUT
	chemistry			- Current chemistry to calibrate. 
	assignedValueLimits	- Assigned values from the db.
	error_msg			- Error reporting buffer.

OUTPUT
	assignedValueLimits	- Assigned values from the db.

LOCAL
	hr				 - Handle returned from safearray object.
	db - Database access structure.
	session - Database session structure.
	cmdGetAssingedValueLimits - Instantiate a command object, to get assigned value limits.
	cmdinit - Set properties for command.

RETURN
	Function status.

*****************************************************************/
HRESULT GetAssignedValueLimits(CString	chemistry,
							   CArray<double,double> *assignedValueLimits,
							   VARIANT	*error_msg)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get assigned value limits.
	CCommand<CAccessor<CdboGetAssignedValueLimitsAccessor> > cmdGetAssingedValueLimits;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetAssignedValueLimits" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetAssignedValueLimits");
		return hr;
	}

	// < ----- GET ASSIGNED VALUE LIMITS ----- >
	// Create command to execute stored procedure "GetAssignedValueLimits".
	hr = cmdGetAssingedValueLimits.Create(session, _T("{ ? = CALL dbo.GetAssignedValueLimits;1 (?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetAssignedValueLimits\" stored procedure","GetAssignedValueLimits");
		return hr;
	}

	// Set database search criterium.
	// Search based on chemistry name.
 	_tcsncpy(cmdGetAssingedValueLimits.m_chemistryID, chemistry,
 		sizeof(cmdGetAssingedValueLimits.m_chemistryID) /
 		sizeof(cmdGetAssingedValueLimits.m_chemistryID[0]));

	// Get assigned value limits.
	hr = cmdGetAssingedValueLimits.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetAssignedValueLimits\" stored procedure","GetAssignedValueLimits");
		return hr;
	}

	cmdGetAssingedValueLimits.MoveFirst();

	if (cmdGetAssingedValueLimits.m_RETURNVALUE > 0)
	{
		assignedValueLimits->Add(cmdGetAssingedValueLimits.m_colLower);
		assignedValueLimits->Add(cmdGetAssingedValueLimits.m_colUpper);
	}

	cmdGetAssingedValueLimits.Close();
	// </ ----- GET ASSIGNED VALUE LIMITS ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

/*****************************************************************
HRESULT GetSampleAssignedValues(LONG calibrationID,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues)

Get assigned values for samples in "sampleIds" list used in 
	calibration that has identifier "calibrationID".

-----------------------------------------------------------------
INPUT
	calibrationID - Id of calibration to get plot data for.
	sampleIds - List of sample ids.
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
HRESULT GetSampleAssignedValues(LONG calibrationID,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues,
								VARIANT *error_msg)
{
	HRESULT		hr;
	CString		lSampleIdList;
	CString		lchemistryName;
	char		buf[256];


	// < ----- GET CHEMISTRY NAME ----- >
	hr = GetChemistryNameGivenCalibrationID(calibrationID, &lchemistryName);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdateAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"GetChemistryNameGivenCalibrationID\" function", buf);
		return hr;
	}
	// </ ----- GET CHEMISTRY NAME ----- >

	// < ----- SAMPLE NAMES FROM CStringArray TO CString ----- >
	hr = FromCStringArrayToCString(sampleIds, &lSampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdateAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"FromCStringArrayToCString\" function", buf);
		return hr;
	}
	// </ ----- SAMPLE NAMES FROM CStringArray TO CString ----- >

	// < ----- GET ASSIGNED VALUES ----- >
	hr = GetAssignedValues(lchemistryName, lSampleIdList, assignedValues, error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdateAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Return from \"GetAssignedValues\" function", buf);
		return hr;
	}
	// </ ----- GET ASSIGNED VALUES ----- >

	// < ----- TEST ASSIGNED VALUE LIMITS ----- >
	hr = TestAssignedValueLimits(lchemistryName, sampleIds, assignedValues, error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetSampleAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"TestAssignedValueLimits\" function", buf);
		return hr;
	}
	// </ ----- TEST ASSIGNED VALUE LIMITS ----- >

	return S_OK;
}

/*****************************************************************
HRESULT GetAssignedValues(CString chemistry,
						  CString sampleIDList,
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
HRESULT GetAssignedValues(CString chemistry,
						  CString sampleIDList,
						  CArray<double,double> *assignedValues,
						  VARIANT *error_msg)
{
	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	CStringArray	tmpSampleIDList;
	CString			noAssignedValueList;
	CArray<int,int>	sampleStatus;
	char			*token;
	char			seps[] = ",\n";
	char			buf[512];

	// Instantiate a command object, to get assigned values.
	CCommand<CAccessor<CdboGetAssignedValuesAccessor> > cmdGetAssignedValues;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// < ----- CONVERT SAMPLE ID LIST ----- >
	hr = FromCStringToCStringArray(&sampleIDList, &tmpSampleIDList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"FromCStringToCStringArray\" function",buf);

		return hr;
	}
	// </ ----- CONVERT SAMPLE ID LIST ----- >

	// Open database.
	hr = OpenDatabase( &db, &session, "GetAssignedValues" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetAssignedValues, Support.cpp");
		return hr;
	}

	// < ----- GET ASSIGNED VALUES ----- >
	// Create command to execute stored procedure, "GetAssignedValues".
	hr = cmdGetAssignedValues.Create(session, _T("{ ? = CALL dbo.GetAssignedValues;1 (?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for \"GetAssignedValues\" stored procedure","GetAssignedValues, Support.cpp");
		return hr;
	}

	// Set database searching criteria.
 	_tcsncpy(cmdGetAssignedValues.m_ChemistryName, chemistry,
 		sizeof(cmdGetAssignedValues.m_ChemistryName) /
 		sizeof(cmdGetAssignedValues.m_ChemistryName[0]));

 	_tcsncpy(cmdGetAssignedValues.m_SampleID, sampleIDList,
 		sizeof(cmdGetAssignedValues.m_SampleID) /
 		sizeof(cmdGetAssignedValues.m_SampleID[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetAssignedValues.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, "Call \"GetAssignedValues\" stored procedure",	buf);
		return hr;
	}

	// Convert sample ids.
	token = strtok( cmdGetAssignedValues.m_AssignedValues, seps );
	while( token != NULL )
	{
		assignedValues->Add(atof(token));
		// Get next sample id.
		token = strtok( NULL, seps );
	}

	// Test sample's status.
	token = strtok( cmdGetAssignedValues.m_SampleStatus, seps );
	while( token != NULL )
	{
		sampleStatus.Add(atoi(token));

		// Get next sample id.
		token = strtok( NULL, seps );
	}
	
	cmdGetAssignedValues.Close();
	// </ ----- GET ASSIGNED VALUES ----- >
	
	// Close the database.
	db.Close();

//DisplayCArray(assignedValues,"AFTER stored procedure GetAssignedValues");

	// < ----- TEST SAMPLE STATUS ----- >
	noAssignedValueList.Empty();
	for (int i=0; i<sampleStatus.GetSize(); i++)
	{
		if (sampleStatus.GetAt(i) == 0)
		{
			if (noAssignedValueList.GetLength() > 0)
			{
				noAssignedValueList = noAssignedValueList + ", \"" + tmpSampleIDList.GetAt(i) + "\"";
			}
			else
			{
				noAssignedValueList = "\"" + tmpSampleIDList.GetAt(i) + "\"";
			}
		}
	}

	if (noAssignedValueList.GetLength() > 0)
	{
		// 0x80040E21  -2147217887  Errors occurred
		hr = 0x80040E21;

		noAssignedValueList = "Samples " + noAssignedValueList + " have no assigned values.";

		UserErrorMessage("3", noAssignedValueList, error_msg);

		return hr;
	}
	// </ ----- TEST SAMPLE STATUS ----- >

//temp="---- END GetAssignedValues ----";
//DisplayCString(&temp);

	return S_OK;
}

/*************************************************************
HRESULT CalculateAssignedValues(CArray<double,double> *fitParameters,
								CArray<double,double> *calibratorData,
								CArray<double,double> *assignedValues,
								CArray<double,double> *calcAssignedValues,
								CString algorithm)

 	Calculate assigned values for current calibrators data.

--------------------------------------------------------------
 INPUT
	vcalibratorData	- Vector to contain calibrator data.
	calcAssignedValues	- List of current calibrator id.

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
HRESULT CalculateAssignedValues(CArray<double,double> *fitParameters,
								CArray<double,double> *calibratorData,
								CArray<double,double> *assignedValues,
								CArray<double,double> *calcAssignedValues,
								CString algorithm)
{
	HRESULT		hr;
	char		buf[512];

	// Add current results to database. Depend on algorithm type.
	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		SigmoidalCalculateAssignedValues(fitParameters,
										 calibratorData,	
										 assignedValues,	
										 calcAssignedValues);
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		LinearCalculateAssignedValues(fitParameters,
									  calibratorData,	
									  calcAssignedValues);
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		QuadraticCalculateAssignedValues(fitParameters,
										 calibratorData,
										 calcAssignedValues);
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		ExponentialCalculateAssignedValues(fitParameters,
										   calibratorData,
										   calcAssignedValues);
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
		hr = 0x80040E21;
		sprintf(buf,"Function \"CalculateAssignedValues\", file %s, line %d", __FILE__, __LINE__);
		// 0x80040E21  -2147217887  Errors occurred
		LogErrorMessage(hr, "Unknown algorithm", buf);
		return hr;
	}

	return S_OK;
}
