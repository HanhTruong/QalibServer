/********************************************************************

FILENAME:	Species.cpp
_____________________________________________________________________

MODULE DESCRIPTION:

	Modules to process species related functionality.

_____________________________________________________________________

GLOBAL FUNCTIONS:

	Name			Description


	Name			Description

_____________________________________________________________________

COMPILER:	Visual C++ 6.0			INVOCATION: Build-> Species.cpp

_____________________________________________________________________

HISTORY:

Version	Date		Author				Description
	1.0	04/21/02	Alves, Jeronimo

********************************************************************/
#include "stdafx.h"
#include "Loging.h"
#include "Utils.h"
#include "Species.h"
#include "DbAccess.h"
#include "dboGetCalibrationSpecies.H"
#include "dboGetSampleSpecies.H"
#include "dboGetAllowedSampleSpecies.H"
#include "BusinessRulesSupport.h"

/****************************************************************
HRESULT VerifySpecies(LONG				calibrationID,
					  CStringArray		*sampleIds,
					  CStringArray		*sampleSpecies,
					  CArray<int,int>	*sampleSizes,
					  VARIANT			*error_msg)

	Test species for current calibration.
	// Remove samples that have no data. i.e. size is 0.
	// Find the types of the current calibration's samples.
	// Verify that the current samples ran with the correct species.
	// Get species allowed in calibrations.
	// Species that should be used for current calibration samples.
	// Verify that the expected and actual species match?
	// Verify that the current calibration has an allowed species.

-----------------------------------------------------------------
 INPUT														
	calibrationID		- ID of calibration to calculate rates.
	*sampleIds			- List of sample ids.
	*sampleSpecies		- Species for current runs.
	*sampleSizes		- Number of runs in each sample.
	*error_msg			- Error reporting.

-----------------------------------------------------------------
 OUTPUT														
	calibration species.
	
-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	allowedCalibrationSpecies - List of allowed species for calibrations.
	sampleTypesList	- List of types for sample ids.
	expectedSpecies	- Expected sample species.
	actualSpecies	- Species of current runs.
	buf[512]		- Temporary buffer.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT VerifySpecies(LONG				calibrationID,
					  CStringArray		*sampleIds,
					  CStringArray		*sampleSpecies,
					  CArray<int,int>	*sampleSizes,
					  VARIANT			*error_msg)
{
	HRESULT			hr = S_OK;
	CStringArray	allowedCalibrationSpecies;
	CStringArray	sampleTypesList;
	CStringArray	expectedSpecies;
	CStringArray	actualSpecies;
	char			buf[512];

	// Remove samples that have no data. i.e. size is 0.
	hr = AdjustSamples(sampleIds, sampleSizes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"AdjustSamples\" function",buf);
		return hr;
	}

	// Find the types of the current calibration's samples.
	hr = GetSamplesAnalyteTypes(sampleIds, &sampleTypesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetSamplesAnalyteTypes\" function",buf);
		return hr;
	}

	// Verify that the current samples ran with the correct species.
	hr = VerifySampleSpecies(sampleIds,
							 sampleSpecies,
							 sampleSizes,
							 &actualSpecies,
							 error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,buf,"GetExpectedSampleSpecies");
		return hr;
	}

	// Get species allowed in calibrations.
	hr = GetCalibrationSpecies(&allowedCalibrationSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationSpecies\" function",buf);
		return hr;
	}

	// Species that should be used for current calibration samples.
	hr = GetExpectedSampleSpecies(sampleIds, &expectedSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetExpectedSampleSpecies\" function",buf);
		return hr;
	}

	// Verify that the expected and actual species match?
	hr = VerifyActualVsExpectedSpecies(sampleIds,
									   &expectedSpecies,
									   &actualSpecies,
									   error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"VerifySpecies\", file %s, line %d", __FILE__, __LINE__);
			LogInterfaceErrorMessage(hr,"Return from \"VerifyActualAndExpectedSpecies\" function",buf);
		}
		return hr;
	}

	// Verify that the current calibration has an allowed species.
	hr = VerifyCalibratorSpecies(sampleIds,
								 &sampleTypesList,
								 &actualSpecies,
								 &allowedCalibrationSpecies,
								 error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3", "Wrong calibration species.", error_msg);
		}
		return hr;
	}

	return hr;
}

/****************************************************************
HRESULT VerifyCalibratorSpecies(CStringArray	*sampleIds,
								CStringArray	*sampleTypes,
								CStringArray	*actualSpecies,
								CStringArray	*allowedCalibrationSpecies,
								VARIANT			*error_msg)

	Test species for current calibrators, calibration species is 
	the same as the calibrator species.

-----------------------------------------------------------------
 INPUT														
	*sampleIds - List of sample ids.
	*sampleTypes - Types of samples.
	*actualSpecies - Species in current data.
	*allowedCalibrationSpecies - List of allowed calibration specis.
	*error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT														
	
-----------------------------------------------------------------
 LOCAL
	hr			- Return code.
	notFound	- Search control.
	i			- Temporary index.
	buf[512]	- Temporary buffer.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT VerifyCalibratorSpecies(CStringArray	*sampleIds,
								CStringArray	*sampleTypes,
								CStringArray	*actualSpecies,
								CStringArray	*allowedCalibrationSpecies,
								VARIANT			*error_msg)
{
	HRESULT			hr=S_OK;
	bool			notFound;
	int				i;
	char			buf[512];
	CString			calibrationSpecies;

	notFound = true;
	i = 0;

	// Find calibrator species.
	while((i<sampleIds->GetSize()) && notFound)
	{
		if (sampleTypes->GetAt(i).CompareNoCase("CALIBRATOR") == 0)
		{
			calibrationSpecies = actualSpecies->GetAt(i);
			notFound = false;
		}

		i++;
	}

	// Do all calibrators have the same species.
	for (i=0; i<sampleIds->GetSize(); i++)
	{
		if (sampleTypes->GetAt(i).CompareNoCase("CALIBRATOR") == 0)
		{
			if (actualSpecies->GetAt(i).CompareNoCase(calibrationSpecies) != 0)
			{
				hr = 0x80040E21;
				sprintf(buf,"Calibrators not of the same species");
				UserErrorMessage("3",buf,error_msg);
				return hr;
			}
		}
	}

	// Is current calibration species allowed?
	if (!FindString(&calibrationSpecies, allowedCalibrationSpecies))
	{
		hr = 0x80040E21;
		sprintf(buf,"Invalid calibration species.");
		UserErrorMessage("3",buf,error_msg);
	}

	return hr;
}

/****************************************************************
HRESULT	VerifyActualVsExpectedSpecies(CStringArray	*sampleIds,
									  CStringArray	*sampleSpecies,
									  CStringArray	*actualSpecies,
									  VARIANT		*error_msg)

	Test the species of the current list of samples.

-----------------------------------------------------------------
 INPUT														
	*sampleIds - List of sample ids.
	*sampleSpecies - Sample species list.
	*actualSpecies - Species in current data.
	*error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT														
	
-----------------------------------------------------------------
 LOCAL
	hr			- Return code.
	tSpecies	- Temporary species.
	strFail		- List of sample ids that failed.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT	VerifyActualVsExpectedSpecies(CStringArray	*sampleIds,
									  CStringArray	*sampleSpecies,
									  CStringArray	*actualSpecies,
									  VARIANT		*error_msg)
{
	HRESULT	hr = S_OK;
	CString	failIds;
	CString	failSpecies;
	CString	correctSpecies;

	failIds.Empty();
	failSpecies.Empty();
	correctSpecies.Empty();

	for (int i=0; i<sampleIds->GetSize(); i++)
	{
		if (sampleSpecies->GetAt(i).CompareNoCase(actualSpecies->GetAt(i)) != 0)
		{
			if (failIds.IsEmpty())
			{
				failIds = sampleIds->GetAt(i);
				failSpecies = sampleSpecies->GetAt(i);
				correctSpecies = actualSpecies->GetAt(i);
			}
			else
			{
				failIds = failIds + "," + sampleIds->GetAt(i);
				failSpecies = failSpecies + "," + sampleSpecies->GetAt(i);
				correctSpecies = correctSpecies + "," + actualSpecies->GetAt(i);
			}
		}
	}

	if (!failIds.IsEmpty())
	{
		hr = 0x80040E21;
		failIds = "Samples \"" + failIds + "\" ran as \"" + correctSpecies + "\". Should ran as \"" + failSpecies + "\".";
		UserErrorMessage("3",failIds,error_msg);
	}

	return hr;
}

/****************************************************************
HRESULT VerifySampleSpecies(CStringArray	*sampleIds,
							CStringArray	*sampleSpecies,
							CArray<int,int>	*sampleSizes,
							CStringArray	*actualSpecies,
							VARIANT			*error_msg)

	Test the species of the current list of samples.

-----------------------------------------------------------------
 INPUT														
	*sampleIds - List of sample ids.
	*sampleSpecies - Sample species list.
	*sampleSizes - Sizes of current samples.
	*actualSpecies - Species in current data.
	*error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT														
	
-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	tSpecies		- Temporary species.
	strFail			- List of sample ids that failed.
	speciesIndex	- Index to traverse species array.
	cFail			- Test control.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT VerifySampleSpecies(CStringArray	*sampleIds,
							CStringArray	*sampleSpecies,
							CArray<int,int>	*sampleSizes,
							CStringArray	*actualSpecies,
							VARIANT			*error_msg)
{
	HRESULT	hr = S_OK;
	CString	tSpecies,strFail;
	int		speciesIndex;
	bool	cFail;

	// Traverse species array.
	speciesIndex = 0;

	strFail.Empty();
	actualSpecies->RemoveAll();
	cFail = false;

	for (int i=0; i<sampleIds->GetSize(); i++)
	{
		if (sampleSizes->GetAt(i) > 0)
		{
			tSpecies = sampleSpecies->GetAt(speciesIndex);

			actualSpecies->Add(tSpecies);

			for (int k=speciesIndex; k<(speciesIndex+sampleSizes->GetAt(i)); k++)
			{
				if (tSpecies.CompareNoCase(sampleSpecies->GetAt(k)) != 0)
				{
					cFail = true;
				}
			}

			if (cFail)
			{
				if (strFail.IsEmpty())
				{
					strFail = sampleIds->GetAt(i);
				}
				else
				{
					strFail = strFail + "," + sampleIds->GetAt(i);
				}
			}

			speciesIndex += sampleSizes->GetAt(i);

			cFail = false;
		}
	}

	if (!strFail.IsEmpty())
	{
		hr = 0x80040E21;
		strFail = "Samples " + strFail + " failed species verification.";
		UserErrorMessage("3",strFail,error_msg);
	}

	return hr;
}

/****************************************************************
HRESULT GetExpectedSampleSpecies(CStringArray *listOfSamples, 
								 CStringArray *listOfSpecies)

	Get species that are allowed for current samples.

-----------------------------------------------------------------
 INPUT														
	*listOfSamples - List of sample ids.
	*listOfSpecies - Expected sample species.

-----------------------------------------------------------------
 OUTPUT														
	
-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	db				- Database access structure.
	session			- Database session structure.
	*token			- Current species from db.
	seps[] = ",\n"	- Token list separator.
	l_listOfSamples	- CString type for database access.
	buf[512]		- Error reporting.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT GetExpectedSampleSpecies(CStringArray *listOfSamples, 
								 CStringArray *listOfSpecies)
{
	HRESULT		hr=S_OK;
	CDataSource	db;
	CSession	session;
	char		*token;
	char		seps[] = ",\n";
	CString		l_listOfSamples;
	char		buf[512];

	// Define command to get expected sample species list from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetSampleSpeciesAccessor> > cmdExpectedSampleSpecies;

	// Clear return list.
	listOfSpecies->RemoveAll();

	// Stored procedures expect string format.
	hr = FromCStringArrayToCString(listOfSamples,&l_listOfSamples);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetExpectedSampleSpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetExpectedSampleSpecies" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\"","GetExpectedSampleSpecies");

		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdExpectedSampleSpecies.Create(session, _T("{ ? = CALL dbo.GetSampleSpecies;1 (?,?) }"));

	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetExpectedSampleSpecies\" stored procedure","GetExpectedSampleSpecies");

		return hr;
	}

	// Set search db criteria.
	_tcsncpy(cmdExpectedSampleSpecies.m_listOfSamples, l_listOfSamples,
 		sizeof(cmdExpectedSampleSpecies.m_listOfSamples) /
 		sizeof(cmdExpectedSampleSpecies.m_listOfSamples[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdExpectedSampleSpecies.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetSampleSpecies\" stored procedure","GetExpectedSampleSpecies");

		return hr;
	}

	token = strtok( cmdExpectedSampleSpecies.m_listOfSpecies, seps );
	while( token != NULL )
	{
		listOfSpecies->Add(token);
		// Get next species.
		token = strtok( NULL, seps );
	}

	cmdExpectedSampleSpecies.Close();

	// Close the database.
	db.Close();

	return hr;
}

/****************************************************************
HRESULT GetCalibrationSpecies(CStringArray *calibrationSpecies)

	Get list of species that are allowed for a calibration.

-----------------------------------------------------------------
 INPUT														
	*calibrationSpecies - Species allowed in a calibration.

-----------------------------------------------------------------
 OUTPUT														
	calibrationSpecies with current species list.

-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	db				- Database access structure.
	session			- Database session structure.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT GetCalibrationSpecies(CStringArray *calibrationSpecies)
{
	HRESULT		hr=S_OK;
	CDataSource	db;
	CSession	session;

	// Define command to get Calibration Species list from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetCalibrationSpeciesAccessor> > cmdCalibrationSpecies;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationSpecies" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\"","GetCalibrationSpecies");

		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdCalibrationSpecies.Create(session, _T("{ ? = CALL dbo.GetCalibrationSpecies;1 }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetCalibrationSpecies\" stored procedure","GetCalibrationSpecies");

		return hr;
	}

	// Execute current command.
	// Call stored procedure.
	hr = cmdCalibrationSpecies.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetCalibrationSpecies\" stored procedure","GetCalibrationSpecies");

		return hr;
	}

	while (cmdCalibrationSpecies.MoveNext() == S_OK)
	{
		calibrationSpecies->Add( (LPCTSTR)cmdCalibrationSpecies.m_colCalibrationSpecies);
	}

	cmdCalibrationSpecies.Close();

	// Close the database.
	db.Close();

	return hr;
}

/********************************************************************
FUNCTION:

  HRESULT GetAllowedSampleSpecies(CStringArray *allowedSampleSpecies)

_____________________________________________________________________
DESCRIPTION:
	Query the database for a list of allowed calibration species.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	db - Database access object.
	session - Database session object.

____________________________________________________________________
PARAMETERS:

  *allowedSampleSpecies - List of species allowed for calibrations.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0		04/21/04	Alves		Changed calibration species access.

********************************************************************/
HRESULT GetAllowedSampleSpecies(CStringArray *allowedSampleSpecies)
{
	HRESULT		hr=S_OK;
	CDataSource	db;
	CSession	session;

	// Define command to get allowed sample species from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetAllowedSampleSpeciesAccessor> > cmdAllowedSampleSpecies;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetAllowedSampleSpecies" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\"","GetAllowedSampleSpecies");

		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdAllowedSampleSpecies.Create(session, _T("{ ? = CALL dbo.GetAllowedSampleSpecies;1 }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command for \"GetAllowedSampleSpecies\" stored procedure","GetCalibrationSpecies");

		return hr;
	}

	// Execute current command.
	// Call stored procedure.
	hr = cmdAllowedSampleSpecies.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetCalibrationSpecies\" stored procedure","GetAllowedSampleSpecies");

		return hr;
	}

	while (cmdAllowedSampleSpecies.MoveNext() == S_OK)
	{
		allowedSampleSpecies->Add( (LPCTSTR)cmdAllowedSampleSpecies.m_colSpecies);
	}

	cmdAllowedSampleSpecies.Close();

	// Close the database.
	db.Close();

	return hr;
}

/****************************************************************
HRESULT AdjustSamples(CStringArray *sampleIds, 
					  CArray<int,int> *sampleSizes)

	Remove samples that have no runs.

-----------------------------------------------------------------
 INPUT														
	*sampleIds - List of sample ids.
	*sampleSizes - Sizes of current samples.

-----------------------------------------------------------------
 OUTPUT														
	
-----------------------------------------------------------------
 LOCAL
	hr				- Return code.
	l_sampleIds		- Sample ids.
	l_sampleSizes	- Sample sizes.
	buf[512]		- Error reporting.

----------------------------------------------------------------
 RETURN														
	hr - Error code.											
****************************************************************/
HRESULT AdjustSamples(CStringArray *sampleIds, 
					  CArray<int,int> *sampleSizes)
{
	HRESULT			hr=S_OK;
	CStringArray	l_sampleIds;
	CArray<int,int>	l_sampleSizes;
	char			buf[512];

	l_sampleIds.RemoveAll();
	l_sampleSizes.RemoveAll();
	
	if (sampleIds->GetSize() != sampleSizes->GetSize())
	{
		hr = 0x80040E21;
		sprintf(buf,", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Function \"AdjustSamples\"",buf);
		return hr;
	}

	for (int i=0; i<sampleIds->GetSize(); i++)
	{
		if (sampleSizes->GetAt(i) != 0)
		{
			l_sampleIds.Add(sampleIds->GetAt(i));
			l_sampleSizes.Add(sampleSizes->GetAt(i));
		}
	}

	sampleIds->RemoveAll();
	sampleSizes->RemoveAll();

	sampleIds->Copy(l_sampleIds);
	sampleSizes->Copy(l_sampleSizes);

	return hr;
}

/********************************************************************
FUNCTION:
HRESULT FindCalibrationSpecies(BSTR				chemistry,
							   CStringArray		*sampleIds,
							   CArray<int,int>	*sampleSizes,
							   CStringArray		*sampleSpecies,
							   CString			*calibrationSpecies,
							   VARIANT			*error_msg)

_____________________________________________________________________
DESCRIPTION:
	Find the species for the current chemistry and calibration.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	analyteTypesList - Sample types.
	buf[512] - Error message.

____________________________________________________________________
PARAMETERS:
chemistry - Current chemistry name.
sampleIds - List of samples.
sampleSizes - Sizes of samples.
sampleSpecies - Species of each sample.
calibrationSpecies - Species for overall calibration.
error_msg - Error reporting.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author				Description
1.0		04/21/04	Alves,Jeronimo		Created.

********************************************************************/
HRESULT FindCalibrationSpecies(BSTR				chemistry,
							   CStringArray		*sampleIds,
							   CArray<int,int>	*sampleSizes,
							   CStringArray		*sampleSpecies,
							   CString			*calibrationSpecies,
							   VARIANT			*error_msg)
{
	HRESULT			hr = S_OK;
	CStringArray	analyteTypesList;
	char			buf[512];

	hr = GetSamplesAnalyteTypes(sampleIds,&analyteTypesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"FindCalibrationSpecies\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetSamplesAnalyteTypes\" function",buf);
		return hr;
	}

	calibrationSpecies->Empty();

	// Calibration species depend on chemistry species dependencies.
	hr = VerifyCalibrationSpecies(chemistry,
							 sampleIds,
							 sampleSizes,
							 sampleSpecies,
							 &analyteTypesList,
							 calibrationSpecies,
							 error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"FindCalibrationSpecies\", file %s, line %d", __FILE__, __LINE__);
			LogInterfaceErrorMessage(hr,"Return from \"VerifyCalibrationSpecies\" function",buf);
		}
	}

	return hr;
}

/********************************************************************
FUNCTION:
HRESULT VerifyCalibrationSpecies(BSTR			chemistry,
							  CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  CStringArray		*sampleSpecies,
							  CStringArray		*analyteTypesList,
							  CString			*calibrationSpecies,
							  VARIANT			*error_msg)

_____________________________________________________________________
DESCRIPTION:
	Test the calibration species.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	analyteTypesList - Sample types.
	buf[512] - Error message.
	speciesIndex - Index to species list.
	found - Search control.
	i - General index.
	k - Intermediate index.
	sampleLimit - Upper index for sample species.

____________________________________________________________________
PARAMETERS:
chemistry - Current chemistry name.
sampleIds - List of samples.
sampleSizes - Sizes of samples.
sampleSpecies - Species of each sample.
analyteTypesList - Types of samples, in sample list.
calibrationSpecies - Species for overall calibration.
error_msg - Error reporting.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author				Description
1.0		04/21/04	Alves,Jeronimo		Created.

********************************************************************/
HRESULT VerifyCalibrationSpecies(BSTR			chemistry,
							  CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  CStringArray		*sampleSpecies,
							  CStringArray		*analyteTypesList,
							  CString			*calibrationSpecies,
							  VARIANT			*error_msg)
{
	HRESULT	hr=S_OK;
	int		speciesIndex;
	bool	found = false;
	int		i = 0;
	int		k;
	int		sampleLimit=0;
	char	buf[512];

	// Find first calibrator species.
	while (i<sampleIds->GetSize() && !found)
	{
		if (analyteTypesList->GetAt(i).CompareNoCase("CALIBRATOR") == 0)
		{
			speciesIndex = 0;
			for (k=0; k<i; k++)
			{
				speciesIndex += sampleSizes->GetAt(k);
			}

			*calibrationSpecies = sampleSpecies->GetAt(speciesIndex);
			found = true;
		}

		i++;
	}

	hr = VerifySampleIdSpecies(sampleIds,sampleSizes,analyteTypesList,calibrationSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySampleIdSpecies\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"VerifySampleIdSpecies\" function",buf);
		return hr;
	}

	if (!calibrationSpecies->IsEmpty())
	{
		hr = FindChemistrySpeciesDependencies((CString)chemistry,calibrationSpecies);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"VerifyCalibrationSpecies\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"FindChemistrySpeciesDependencies\" function",buf);
		}
	}
	else
	{
		*calibrationSpecies = "N/A";
	}

	return hr;
}

HRESULT VerifySampleIdSpecies(CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  CStringArray		*analyteTypesList,
							  CString			*speciesSampleName)
{
	HRESULT			hr = S_OK;
	CString			temp;
	CStringArray	allowedSampleSpecies;
	bool			found = false;
	int				k;
	int				i = 0;
	char			buf[512];

	hr = GetAllowedSampleSpecies(&allowedSampleSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"VerifySampleIdSpecies\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetAllowedSampleSpecies\" function",buf);
	}

	speciesSampleName->Empty();

	// Find first calibrator with a species in name.
	while (i<sampleIds->GetSize() && !found)
	{
		if (analyteTypesList->GetAt(i).CompareNoCase("CALIBRATOR") == 0)
		{
			temp = sampleIds->GetAt(i);
			temp.MakeUpper();
			k = 0;
			while (k<allowedSampleSpecies.GetSize() && !found)
			{
				if (temp.Find(allowedSampleSpecies.GetAt(k),0) >= 0)
				{
					*speciesSampleName = allowedSampleSpecies.GetAt(k);
					found = true;
				}

				k++;
			}
		}

		i++;
	}

	// Found a species name in sample id?
	if (!speciesSampleName->IsEmpty())
	{
		i = 0;
		found = false;
		// Search rest of sample ids for species name.
		while (i<sampleIds->GetSize() && !found)
		{
			if (analyteTypesList->GetAt(i).CompareNoCase("CALIBRATOR") == 0)
			{
				temp = sampleIds->GetAt(i);
				temp.MakeUpper();

				// If any calibrator does not have the correct species name.
				// Fail calibration species tests. 
				// All calibrators must have the same species.
				if (temp.Find(*speciesSampleName,0) < 0)
				{
					found = true;
					speciesSampleName->Empty();
				}
			}
			
			i++;
		}
	}

	return hr;
}

