/*******************************************************************

FILENAME:	NewFitParameters.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	New fit parameter module to process new fit parameters.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	NewFitParameters.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
// NewFitParameters.cpp : Process new fit parameters.

#include "stdafx.h"
#include <math.h>
#include <MDstd.h> 
#include "DbAccess.h"
#include "Loging.h"
#include "Support.h"
#include "dboGetEditableFitParameters.H"

/*************************************************************
HRESULT GetEditableFitParameters(
				LONG calibrationID, 
				CString algorithm,
				VARIANT *editableFitParameters)

Get editable fit parameters.

--------------------------------------------------------------
INPUT
	calibrationID - Calibration to find fit parameters.
	algorithm - Algorithm of given calibration id.
	*error_msg - Error reporting.
	
--------------------------------------------------------------
OUTPUT
	Editable fit parameters.

--------------------------------------------------------------
RETURN
	Results standard code.

*************************************************************/
HRESULT GetEditableFitParameters(
				LONG calibrationID,
				CString algorithm,
				VARIANT *editableFitParameters)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	CString		temp;
	BSTR		bstr=NULL;
	long		k=0;
	char		*token;
	char		seps[] = ",\n";

	CStringArray localFitParameters;

	//*** define a safearray, to pass data to caller. ***
    SAFEARRAY FAR* psa_EditableFitParams = NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	// Define command to get editable fit parameters.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetEditableFitParametersAccessor> > cmdGetEditableFitParameters;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_IRowsetScroll, true);
	cmdinit.AddProperty(DBPROP_CLIENTCURSOR, true);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetEditableFitParameters");
	if (FAILED(hr))
	{
		LogErrorMessage(hr, 
				"Return from \"OpenDatabase\"",
				"GetEditableFitParameters");
		return hr;
	}

	// < ------------ EDITABLE FIT PARAMETERS ------------ >
	// Create command to execute stored procedure.
	hr = cmdGetEditableFitParameters.Create(session, _T("{ ? = CALL dbo.GetEditableFitParameters;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr, 
			"Create command for \"GetEditableFitParameters\" stored procedure",
			"GetEditableFitParameters");
		return hr;
	}

	// Set database searching criteria.
	// Current calibration id.
	cmdGetEditableFitParameters.m_calibrationID = calibrationID;

	// Set second stored procedure parameter with algorithm.
 	_tcsncpy(cmdGetEditableFitParameters.m_algorithm, algorithm,
 		sizeof(cmdGetEditableFitParameters.m_algorithm) /
 		sizeof(cmdGetEditableFitParameters.m_algorithm[0]));

	// Execute current command.
	// Call stored procedure, "GetEditableFitParameters".
	hr = cmdGetEditableFitParameters.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr, 
			"Call \"GetEditableFitParameters\" stored procedure",
			"GetEditableFitParameters");
		return hr;
	}

	// Get fit parameters' ids and values.
	token = strtok( cmdGetEditableFitParameters.m_fitParameters, seps );
	while( token != NULL )
	{
		localFitParameters.Add(token);
		token = strtok( NULL, seps );
	}

	cmdGetEditableFitParameters.Close();
	// </ ------------ EDITABLE FIT PARAMETERS ------------ >

	// Close database.
	db.Close();

	// Set size of array to pass to client.
	// Factor 2, is to accommodate label and value.
    sabound[0].cElements = localFitParameters.GetSize();
    sabound[0].lLbound = 0;

	//*** create the safearray to return ***
    psa_EditableFitParams = SafeArrayCreate(VT_BSTR, 1, sabound);

	k = 0;
	
	for (int i=0; i<localFitParameters.GetSize(); i++)
	{
		temp = localFitParameters.GetAt(i);
		bstr = temp.AllocSysString();
		hr = SafeArrayPutElement(psa_EditableFitParams, &k, bstr);
		k++;
	}

	// Set the return array's type, array of VT_BSTR.
	// Type of data to return in editableFitParameters.
	//*** set up the safearray to export it as an array of strings.  
	V_VT(editableFitParameters) = VT_ARRAY | VT_BSTR;

	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
    V_ARRAY(editableFitParameters) = psa_EditableFitParams;

	return S_OK;
}

/*************************************************************
HRESULT GetLinearEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters)

Get linear editable fit parameters.

--------------------------------------------------------------
INPUT
	calibrationID - Calibration to find fit parameters.
	*editableFitParameters - Return fit parameters.
	
--------------------------------------------------------------
OUTPUT
	Linear editable fit parameters.

--------------------------------------------------------------
RETURN
	Results standard code.

*************************************************************/
HRESULT GetLinearEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters)
{
	return S_OK;
}

/*************************************************************
HRESULT GetQuadraticEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters)

Get quadratic editable fit parameters.

--------------------------------------------------------------
INPUT
	calibrationID - Calibration to find fit parameters.
	*editableFitParameters - Return fit parameters.

--------------------------------------------------------------
OUTPUT
	Quadratic editable fit parameters.

--------------------------------------------------------------
RETURN
	Results standard code.

*************************************************************/
HRESULT GetQuadraticEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters)
{
	return S_OK;
}

/*************************************************************
HRESULT GetMinMaxDecayEditableFitParameters(
						calibrationID, 
						editableFitParameters)

Get min_max_decay editable fit parameters.

--------------------------------------------------------------
INPUT
	calibrationID - Calibration to find fit parameters.
	*editableFitParameters - Return fit parameters.
	
--------------------------------------------------------------
OUTPUT
	Linear editable fit parameters.

--------------------------------------------------------------
RETURN
	Results standard code.

*************************************************************/
HRESULT GetMinMaxDecayEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters)
{
	return S_OK;
}

void SetFitParameters(CArray<double,double> *fitParameters, 
					  dVector dFitParameters, 
					  CString algorithm)
{

	fitParameters->RemoveAll();
	
	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		fitParameters->Add(dFitParameters[0]);
		fitParameters->Add(dFitParameters[1]);
		fitParameters->Add(dFitParameters[2]);
		fitParameters->Add(dFitParameters[3]);
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		fitParameters->Add(dFitParameters[0]);
		fitParameters->Add(dFitParameters[1]);
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		fitParameters->Add(dFitParameters[0]);
		fitParameters->Add(dFitParameters[1]);
		fitParameters->Add(dFitParameters[2]);
	}
	else if (algorithm.CompareNoCase("EXPONENTIAL") == 0)
	{
		fitParameters->Add(dFitParameters[0]);
		fitParameters->Add(dFitParameters[1]);
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
}
