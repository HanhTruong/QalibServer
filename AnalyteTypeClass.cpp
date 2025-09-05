/*******************************************************************
FILENAME:	AnalyteTypeClass.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Analyte type data structure.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	AnalyteType
	~AnalyteType
	GetCurrentTypes
	InitializeAnalyteTypes
	GetAllowedAnalyteTypes

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	AnalyteTypeClass.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include "AnalyteTypeClass.h"
#include "dboGetSampleTypeIDs.H"
#include "Support.h"
#include "Loging.h"
#include "DbAccess.h"

HRESULT GetAllowedAnalyteTypes(AnalyteType *anlyteType);

/*****************************************************************
AnalyteType::AnalyteType()

	Analyte type constructor.

INPUT
	None.

OUTPUT
	Pointer to an analyte class.

LOCAL
	None.

RETURN
	None.
*****************************************************************/
AnalyteType::AnalyteType()
{
}

/*****************************************************************
AnalyteType::~AnalyteType()

INPUT
	None.

OUTPUT
	Return class memory to system.

LOCAL
	None.

RETURN
	None.
*****************************************************************/
AnalyteType::~AnalyteType()
{
}

/*****************************************************************
analyteLevel* AnalyteType::GetCurrentTypes()

INPUT
	None.

OUTPUT
	None.

LOCAL
	None.

RETURN
	Pointer to current analyte class.

*****************************************************************/
analyteLevel* AnalyteType::GetCurrentTypes()
{
	return &_currentTypes;
}

/*****************************************************************
bool AnalyteType::InitializeAnalyteTypes(AnalyteType *anlyteType)

  Get analytes for Albumin from the database and put them in
the analyte class.

INPUT
	*anlyteType - List to initialize.

OUTPUT
	*anlyteType - Structure initialized with current analytes.

LOCAL
	hr
	buf[512]

RETURN
	Does current chemistry have analyte types.

*****************************************************************/
bool AnalyteType::InitializeAnalyteTypes(AnalyteType *anlyteType)
{
	HRESULT		hr;
	char		buf[512];

	hr = GetAllowedAnalyteTypes(anlyteType);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"InitializeAnalyteTypes\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"GetAllowedAnalyteTypes\" function", buf);

		return false;
	}

	return true;
}

/*****************************************************************
HRESULT GetAllowedAnalyteTypes(AnalyteType *anlyteType)

	Get Albumin's analyte types from the database.	

INPUT
	*anlyteType - List to initialize.

OUTPUT
	*anlyteType - Contains Albumin's current analytes.

LOCAL
	hr					- Handle returned from safearray object.
	db					- Database access structure.
	session				- Database session structure.
	cmdGetAnalyteTypes	- Command to get data from the database.
	cmdinit				- Database access options.

RETURN
	Function status.

*****************************************************************/
HRESULT GetAllowedAnalyteTypes(AnalyteType *anlyteType)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get allowed types.
	CCommand<CAccessor<CdboGetSampleTypeIDsAccessor> > cmdGetAnalyteTypes;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetAllowedAnalyteTypes" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr, 
			"Return from \"OpenDatabase\" function",
			"GetAllowedAnalyteTypes");
		return hr;
	}

	// < --------------- GET SAMPLE NAMES --------------- >
	// Create command to execute stored procedure "GetSampleTypeIDs".
	hr = cmdGetAnalyteTypes.Create(session, _T("{ ? = CALL dbo.GetSampleTypeIDs;1 }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr, 
			"Create command for \"GetSampleTypeIDs\" stored procedure",
			"GetAllowedAnalyteTypes");
		return hr;
	}

	// Get sample names.
	hr = cmdGetAnalyteTypes.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr, 
			"Call \"GetSampleTypeIDs\" stored procedure",
			"GetAllowedAnalyteTypes");
		return hr;
	}

	// Fill sample id list.
	while (cmdGetAnalyteTypes.MoveNext() == S_OK)
	{
		anlyteType->GetCurrentTypes()->SetAt((CString)cmdGetAnalyteTypes.m_colSampleTypeID,0);
	}

	cmdGetAnalyteTypes.Close();
	// </ --------------- GET SAMPLE NAMES --------------- >

	// Close the database.
	db.Close();

	return S_OK;
}