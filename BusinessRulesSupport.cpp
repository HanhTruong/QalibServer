/*******************************************************************
FILENAME:	BusinessRulesSupport.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for business rules support.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	GetSamplesAnalyteTypes
	GetAnalyteLevelList
	FindSampleInCalibration

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BusinessRulesSupport.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <math.h>
#include "DbAccess.h"
#include "Loging.h"
#include "Support.h"
#include "dboGetSampleLevel.H"
#include "dboGetSampleTypes.H"
#include "Utils.h"

/************************************************************
HRESULT GetSamplesAnalyteTypes(CStringArray *sampleIdsList,
							   CStringArray *analyteTypesList)

	Find analyte levels for current chemistry.
	
-------------------------------------------------------------
 INPUT
	sampleIdList - List of samples to find analyte type for.
	analyteTypesList - List of analyte types found.

-------------------------------------------------------------
 OUTPUT
	List of analyte types.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	seps - Separator for token search.
	sampleid - Sample id, from token string.
	buf - Message build.
	db - Database access session.
	session - Database session.
	l_sampleIdList - Local list of sample ids.
	cmdSampleTypes - Command to access db.
	cmdinit - Set properties for command.

-------------------------------------------------------------
 RETURN
	Function status.

*************************************************************/
HRESULT GetSamplesAnalyteTypes(CStringArray *sampleIdsList,
							   CStringArray *analyteTypesList)
{
	HRESULT			hr;
	char			seps[] = ",\n";
	char			*sampleid;
	char			buf[512];
	CDataSource		db;
	CSession		session;
	CString			l_sampleIdList;
	
	CCommand<CAccessor<CdboGetSampleTypesAccessor> > cmdSampleTypes;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Convert sample id from string array to flat string.
	hr = FromCStringArrayToCString(sampleIdsList, &l_sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetSamplesAnalyteTypes\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSamplesAnalyteTypes" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetSamplesAnalyteTypes");
		return hr;
	}

	hr = cmdSampleTypes.Create (session, _T("{ ? = CALL dbo.GetSampleTypes;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command for stored procedure \"GetSampleLevel\"","GetSamplesAnalyteTypes");
		return hr;
	}

	// Set database searching criteria.
	_tcsncpy(cmdSampleTypes.m_SampleIDList, l_sampleIdList,
		sizeof(cmdSampleTypes.m_SampleIDList) /
		sizeof(cmdSampleTypes.m_SampleIDList[0]));

	// Execute current command.
	// Call stored procedure, "GetSampleTypes".
	hr = cmdSampleTypes.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetSampleTypes\"","GetSamplesAnalyteTypes");
		return hr;
	}

	// Close access command.
	cmdSampleTypes.Close();
	// Close the database.
	db.Close();

	// Set access to first row of record set.
	// Get list of sample types.
	sampleid = strtok( cmdSampleTypes.m_SampleTypesList, seps );
	while( sampleid != NULL )
	{
		analyteTypesList->Add(sampleid);
		//Get next sample type.
		sampleid = strtok( NULL, seps );
	}

	return S_OK;
}

/************************************************************
HRESULT GetAnalyteLevelList(CString chemistry,
							CString sampleIdList,
							CArray<int,int> *sampleAnalyteLevelList)

	Find analyte levels for current chemistry.
	
-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	sample - Sample to search for.
	error_msg - Error message buffer.

	chemistry - Current chemistry.
	sampleIdList - List of samples to find level for.
	sampleAnalyteLevelList - List of analyte levels found.

-------------------------------------------------------------
 OUTPUT
	List of analyte levels.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	seps - Separator for token search.
	sampleid - Sample id, from token string.
	db - Database access struct.
	session - Database session struct.
	cmdSampleLevel - Command to access database.
	cmdinit - Set properties for command.

-------------------------------------------------------------
 RETURN
	Search results.

*************************************************************/
HRESULT GetAnalyteLevelList(CString chemistry,
							CString sampleIdList,
							CArray<int,int> *sampleAnalyteLevelList)
{
	HRESULT			hr;
	char			seps[] = ",\n";
	char			*sampleid;
	CDataSource		db;
	CSession		session;

	CCommand<CAccessor<CdboGetSampleLevelAccessor> > cmdSampleLevel;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetAnalyteLevelList" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr, 
			"Return from \"OpenDatabase\" function",
			"GetAnalyteLevelList");
		return hr;
	}

	hr = cmdSampleLevel.Create (session, _T("{ ? = CALL dbo.GetSampleLevel;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr, 
			"Create command for stored procedure \"GetSampleLevel\"",
			"GetAnalyteLevelList");
		return hr;
	}

	// Set database searching criteria.
	_tcsncpy(cmdSampleLevel.m_chemistry, chemistry,
		sizeof(cmdSampleLevel.m_chemistry) /
		sizeof(cmdSampleLevel.m_chemistry[0]));

	// Set database searching criteria.
	_tcsncpy(cmdSampleLevel.m_SampleIDList, sampleIdList,
		sizeof(cmdSampleLevel.m_SampleIDList) /
		sizeof(cmdSampleLevel.m_SampleIDList[0]));

	// Execute current command.
	// Call stored procedure, "GetSampleLevel".
	hr = cmdSampleLevel.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr, 
			"Call stored procedure \"GetSampleLevel\"",
			"GetAnalyteLevelList");
		return hr;
	}

	// Close access command.
	cmdSampleLevel.Close();
	// Close the database.
	db.Close();

	// Set access to first row of record set.
	// Get list of sample levels.
	sampleid = strtok( cmdSampleLevel.m_SampleLevelList, seps );
	while( sampleid != NULL )
	{
		sampleAnalyteLevelList->Add(atoi(sampleid));
		//Get next sample level.
		sampleid = strtok( NULL, seps );
	}

	return S_OK;
}

/************************************************************
bool FindSampleInCalibration(LONG calibrationID,
							 CString sample,
							 VARIANT *error_msg)

	Find a sample in a calibration.
	Is a given sample in a particular calibarion?

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	sample - Sample to search for.
	error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	None.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	l_sampleIdList - List of sample ids in current calibration.
	buf - Error message buffer.
	found - Search control.
	k - Index to traverse sample sizes list.

-------------------------------------------------------------
 RETURN
	Search results.

*************************************************************/
bool FindSampleInCalibration(LONG calibrationID,
							 CString sample,
							 VARIANT *error_msg)
{
	HRESULT			hr;
	CStringArray	l_sampleIdList;
	char			buf[512];
	bool			found;
	int				k;

	// < --------------- GET SAMPLE NAMES --------------- >
	hr = GetCalibrationIDSamples(calibrationID, &l_sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"FindSampleInCalibration\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(0x80040E21,"Return from \"GetCalibrationIDSamples\" function",buf);
		return false;
	}
	// </ --------------- GET SAMPLE NAMES --------------- >

	k = 0;
	found = false;
	while (k<l_sampleIdList.GetSize() && !found)
	{
		if (l_sampleIdList.GetAt(k).CompareNoCase(sample) == 0)
		{
			found = true;
		}

		k++;
	}

	return found;
}

