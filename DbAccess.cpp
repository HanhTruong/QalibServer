/********************************************************************

FILENAME:	DbAccess.cpp
_____________________________________________________________________

MODULE DESCRIPTION:
	Database access routines.

_____________________________________________________________________

GLOBAL FUNCTIONS:

	Name			Description

_____________________________________________________________________

COMPILER:	Visual Studio 6.0		INVOCATION: Build->Compile DbAccess.cpp

_____________________________________________________________________

$HISTORY: $

Version	Date		Author		Description
1.0.0	07/19/04	Alves		Database access routines.

********************************************************************/
#include "stdafx.h"
#include "Loging.h"
#include "Utils.h"
#include "ErrorProcessing.h"
#include "Support.h"
#include "dboInsertSampleData.h"
#include "dboGetValuesAndStatus.H"
#include "dboGetSampleValuesIDs.H"
#include "dboGetChemistrySpeciesDependencies.H"

/*************************************************************
HRESULT OpenDatabase( CDataSource *pdb, 
					  CSession *psession, 
					  CString Caller)
	Open database
--------------------------------------------------------------
 INPUT
	*pdb - Pointer to datasource object, for database use.
	*psession - Session to open database.
	Caller - Identifier of caller.

--------------------------------------------------------------
 OUTPUT
	Open databse session.

--------------------------------------------------------------
 LOCAL
		dbinit - Database properties structure.
		hr - System status code.

--------------------------------------------------------------
 RETURN
	None.
**************************************************************/
HRESULT OpenDatabase(CDataSource *pdb, 
					 CSession *psession, 
					 CString Caller)
{
	HRESULT	hr;
	CString	partialMsg;

	// Set properties for database access.
	CDBPropSet	dbinit(DBPROPSET_DBINIT);
	dbinit.AddProperty(DBPROP_AUTH_INTEGRATED, OLESTR("SSPI"));
	dbinit.AddProperty(DBPROP_INIT_CATALOG, OLESTR("Qalibration"));
	dbinit.AddProperty(DBPROP_INIT_DATASOURCE, OLESTR("SQLWIN2000SRVR"));
//	dbinit.AddProperty(DBPROP_INIT_DATASOURCE, OLESTR("Rdswtestsvr"));
	dbinit.AddProperty(DBPROP_INIT_LCID, (long)1033);
//	dbinit.AddProperty(DBPROP_INIT_PROMPT, (short)4);	// Windows 2003 server?

	// Open database.
	hr = pdb->OpenWithServiceComponents(_T("SQLOLEDB.1"), &dbinit);

	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Open database failed.","OpenDatabase");
		return hr;
	}

	// Open a session to database.
	hr = psession->Open(*pdb);
	if (FAILED(hr))
	{
		pdb->Close();

		LogInterfaceErrorMessage(hr,"Open database session failed.","OpenDatabase");

		return hr;
	}

	return S_OK;
}

/*************************************************************
HRESULT TestUserAccess(BSTR user, 
					   BSTR password, 
					   VARIANT *error_msg)

  Is current user allowed access to the database?
--------------------------------------------------------------
 INPUT
	user - User name.
	password - User password.
	error_msg - Error reporting.

--------------------------------------------------------------
 OUTPUT
	Users' permissions.

--------------------------------------------------------------
 LOCAL
	dbinit - Database properties structure.
	hr - System status code.
	db - Database access structure.
	session - Session structure.

--------------------------------------------------------------
 RETURN
	None.
**************************************************************/
HRESULT TestUserAccess(BSTR user, 
					   BSTR password, 
					   VARIANT *error_msg)
{
	HRESULT		hr=S_OK;
	CDataSource	db;
	CSession	session;
	
	// Set properties for database access.
	CDBPropSet	dbinit(DBPROPSET_DBINIT);

	dbinit.AddProperty(DBPROP_AUTH_ENCRYPT_PASSWORD, true);
	dbinit.AddProperty(DBPROP_AUTH_PASSWORD, password);
	dbinit.AddProperty(DBPROP_AUTH_USERID, user);

	dbinit.AddProperty(DBPROP_INIT_CATALOG, OLESTR("Qalibration"));
//	dbinit.AddProperty(DBPROP_INIT_DATASOURCE, OLESTR("Rdswtestsvr"));
	dbinit.AddProperty(DBPROP_INIT_DATASOURCE, OLESTR("SQLWIN2000SRVR"));
	dbinit.AddProperty(DBPROP_INIT_LCID, (long)1033);
	dbinit.AddProperty(DBPROP_INIT_PROMPT, (short)4);

	// Open database.
	hr = db.OpenWithServiceComponents(_T("SQLOLEDB.1"), &dbinit);
	if (hr == S_OK)
	{
		// Open a session to database.
		hr = session.Open(db);
		db.Close();
	}

	if (FAILED(hr))
	{
		UserErrorMessage("3","User id and or password failed!",error_msg);
	}

	db.Close();

	return hr;
}

/*****************************************************************
HRESULT LoadCalibrationDataToDB(BSTR					chemistry,
								BSTR					mode,
								BSTR					user,
								int						mold,
								int						rotor,
								int						cuvette,
								int						barcodelength,
								CArray<double,double>	*sampleData,
								CArray<int,int>			*sampleSizes,
								CArray<int,int>			*sampleStatus,
								CStringArray			*sampleIds,
								CStringArray			*sampleSpecies,
								CStringArray			*sampleSerialNumbers,
								CStringArray			*sampleBarCode,
								LONG					*calibrationID)

- Enters current calibration data into database.

INPUT
	mode - Operational mode.
	user - Operator running the current calibration.
	sampleData - Calibration data.
	sampleSizes - Number of values in each sample.
	sampleStatus - Sample value status.
	sampleIds - Sample identifiers.
	sampleSpecies - Sample species.
	sampleSerialNumbers - Serial number of analyzers.
	sampleBarCode - Bar codes.
	calibrationID - Calibtration id.

OUTPUT
	Current data in database.

LOCAL
	hr - Handle returned from database object.
	cSampleData - Database object.
	mapPos - Position in current samples list traversal.
	listPos - Position in current in sample list traversal.
	CstrKey - Current sample name.
	tmpPoolData - Sample data string to send to stored procedure.
	tmpStatusData - Status of data values in string format.
	tmpPoolSizes - Sample sizes in string format.
	tmpIdList - Hold list of samples.
	ptValuesList - Pointer to current sample list.
	ptStatusList - Pointer to current status list.
	curValue - Current numerical, double, sample value.
	curStatus - Current numerical, integer, status.
	i - Index to add value delimeter, in tmpPoolData.
	listSize - Size of current sample.
	strCurValue[80] - Buffer for double to string conversion.
	localSampleIDList - Buffer for sample id list manipulations.
*****************************************************************/
HRESULT LoadCalibrationDataToDB(BSTR			chemistry,
								BSTR			mode,
								BSTR			user,
								int				mold,
								int				rotor,
								int				cuvette,
								int				barcodelength,
								CString			calibrationSpecies,
								CStringArray	*sampleData,
								CStringArray	*setAttributes,
								CArray<int,int>	*sampleSizes,
								CArray<int,int>	*sampleStatus,
								CStringArray	*sampleIds,
								CStringArray	*sampleSpecies,
								CStringArray	*sampleSerialNumbers,
								CStringArray	*sampleBarCode,
								LONG			*calibrationID,
								VARIANT			*error_msg)
{
	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	char			buf[16000];
	CString			l_sampleData;
	CString			l_sampleSizes;
	CString			l_sampleStatus;
	CString			l_sampleIds;
	CString			l_sampleSpecies;
	CString			l_sampleSerialNumbers;
	CString			l_sampleBarCode;
	CString			l_setAttributes;

	CString	CTemp; // debug only
	static char sTemp[128];

	CCommand<CAccessor<CdboInsertSampleDataAccessor> > cmdInsertSampleData;
	
	// Set properties for open
	CDBPropSet	cmdinit(DBPROPSET_ROWSET);
//	cmdinit.AddProperty(DBPROP_IRowsetChange, true);
//	cmdinit.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_INSERT);

	// < ---------- COVERT DATA STRUCTURES to CString. ---------- >
	// < ------- REQURED FORMAT TO CALL STORED PROCEDURES ------- >
	// Convert sample data list from CArray to CString.
	hr = FromCStringArrayToCString(sampleData,&l_sampleData);
CTemp = "l_sampleData";
DisplayCString(&CTemp);
DisplayCString(&l_sampleData);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCArrayToCString\" function",buf);
		return hr;
	}
	
	// Convert sample sizes list from CArray to CString.
	hr = FromCArrayToCString(sampleSizes, &l_sampleSizes);
CTemp = "l_sampleSizes";
DisplayCString(&CTemp);
DisplayCString(&l_sampleSizes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample status list from CArray to CString.
	hr = FromCArrayToCString(sampleStatus, &l_sampleStatus);
CTemp = "l_sampleStatus";
DisplayCString(&CTemp);
DisplayCString(&l_sampleStatus);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample ids list from CStringArray to CString.
	hr = FromCStringArrayToCString(sampleIds, &l_sampleIds);
CTemp = "l_sampleIds";
DisplayCString(&CTemp);
DisplayCString(&l_sampleIds);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample species list from CStringArray to CString.
	hr = FromCStringArrayToCString(sampleSpecies, &l_sampleSpecies);
CTemp = "l_sampleSpecies";
DisplayCString(&CTemp);
DisplayCString(&l_sampleSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample Serial Numbers list from CStringArray to CString.
	hr = FromCStringArrayToCString(sampleSerialNumbers, &l_sampleSerialNumbers);
CTemp = "l_sampleSerialNumbers";
DisplayCString(&CTemp);
DisplayCString(&l_sampleSerialNumbers);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample bar code list from CStringArray to CString.
	hr = FromCStringArrayToCString(sampleBarCode, &l_sampleBarCode);
CTemp = "l_sampleBarCode";
DisplayCString(&CTemp);
DisplayCString(&l_sampleBarCode);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// Convert sample attributes list from CStringArray to CString.
	hr = FromCStringArrayToCString(setAttributes, &l_setAttributes);
CTemp = "l_setAttributes";
DisplayCString(&CTemp);
DisplayCString(&l_setAttributes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// </ ---------- COVERT DATA STRUCTURES to CString. ---------- >
	// </ ------- REQURED FORMAT TO CALL STORED PROCEDURES ------- >

	// Open database.
	hr = OpenDatabase( &db, &session, "LoadCalibrationDataToDB" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);

		return hr;
	}


	// < --------------- ENTER SAMPLE DATA IN DB --------------- >
	// Create command to execute stored procedure, "InsertSampleData".
	hr = cmdInsertSampleData.Create(session, _T("{ ? = CALL dbo.InsertSampleData;1 (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		sprintf(buf,"Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"InsertSampleData\" stored procedure",buf);
		return hr;
	}

//			@chemistry		varchar(256),
	// Set first stored procedure parameter with chemistry name.
CTemp = "chemistry";
DisplayCString(&CTemp);
DisplayCString(&(CString)chemistry);
 	_tcsncpy(cmdInsertSampleData.m_chemistry, (CString)chemistry,
 		sizeof(cmdInsertSampleData.m_chemistry) /
 		sizeof(cmdInsertSampleData.m_chemistry[0]));

//			@mold			int,
	// Set second stored procedure parameter with current mold.
CTemp = "mold";
DisplayCString(&CTemp);
sprintf (sTemp, "%d", mold);
DisplayCString(&(CString)sTemp);
	cmdInsertSampleData.m_mold = mold;

//			@rotor			int,
	// Set third stored procedure parameter with rotor number.
CTemp = "rotor";
DisplayCString(&CTemp);
sprintf (sTemp, "%d", rotor);
DisplayCString(&(CString)sTemp);
 	cmdInsertSampleData.m_rotor = rotor;
	
//			@species			varchar(256),
	// Set fourth stored procedure parameter with calibration species.
CTemp = "calibrationSpecies";
DisplayCString(&CTemp);
DisplayCString(&calibrationSpecies);
 	_tcsncpy(cmdInsertSampleData.m_species, calibrationSpecies,
 		sizeof(cmdInsertSampleData.m_species) /
 		sizeof(cmdInsertSampleData.m_species[0]));

//			@mode			varchar(256),
	// Set fifth stored procedure parameter with "user's" calibration mode.
CTemp = "mode";
DisplayCString(&CTemp);
DisplayCString(&(CString)mode);
 	_tcsncpy(cmdInsertSampleData.m_mode, (CString)mode,
 		sizeof(cmdInsertSampleData.m_mode) /
 		sizeof(cmdInsertSampleData.m_mode[0]));

//			@user			varchar(256),
	// Set sixth stored procedure parameter with user id.
CTemp = "user";
DisplayCString(&CTemp);
DisplayCString(&(CString)user);
 	_tcsncpy(cmdInsertSampleData.m_user, (CString)user,
 		sizeof(cmdInsertSampleData.m_user) /
 		sizeof(cmdInsertSampleData.m_user[0]));

//			@sampleData		varchar(8000),
	// Set seventh stored procedure parameter with sample data.
 	_tcsncpy(cmdInsertSampleData.m_sampleData, l_sampleData,
 		sizeof(cmdInsertSampleData.m_sampleData) /
 		sizeof(cmdInsertSampleData.m_sampleData[0]));

//			@sampleSizes		varchar(7900),
	// Set eighth stored procedure parameter with sample sizes.
 	_tcsncpy(cmdInsertSampleData.m_sampleSizes, l_sampleSizes,
 		sizeof(cmdInsertSampleData.m_sampleSizes) /
 		sizeof(cmdInsertSampleData.m_sampleSizes[0]));

//			@sampleStatus		varchar(7900),
	// Set nineth stored procedure parameter with sample data status.
 	_tcsncpy(cmdInsertSampleData.m_sampleStatus, l_sampleStatus,
 		sizeof(cmdInsertSampleData.m_sampleStatus) /
 		sizeof(cmdInsertSampleData.m_sampleStatus[0]));
//			@sampleIds		varchar(8000),
	// Set tenth stored procedure parameter with current sample id list.
 	_tcsncpy(cmdInsertSampleData.m_sampleIds, l_sampleIds,
 		sizeof(cmdInsertSampleData.m_sampleIds) /
 		sizeof(cmdInsertSampleData.m_sampleIds[0]));

//			@sampleSpecies		varchar(8000),
	// Set eleventh stored procedure parameter with current sample species.
 	_tcsncpy(cmdInsertSampleData.m_sampleSpecies, l_sampleSpecies,
 		sizeof(cmdInsertSampleData.m_sampleSpecies) /
 		sizeof(cmdInsertSampleData.m_sampleSpecies[0]));

//			@sampleSerialNumbers	varchar(8000),
	// Set twelfth stored procedure parameter with instrument serial numbers.
 	_tcsncpy(cmdInsertSampleData.m_sampleSerialNumbers, l_sampleSerialNumbers,
 		sizeof(cmdInsertSampleData.m_sampleSerialNumbers) /
 		sizeof(cmdInsertSampleData.m_sampleSerialNumbers[0]));


//			@sampleBarCode		text,
	// Set thirteenth stored procedure parameter with bar code list.
 	_tcsncpy(cmdInsertSampleData.m_sampleBarCode, l_sampleBarCode,
 		sizeof(cmdInsertSampleData.m_sampleBarCode) /
 		sizeof(cmdInsertSampleData.m_sampleBarCode[0]));

//			@BarCodeLen		int,
	// Set fourteenth stored procedure parameter with current instrument list.
CTemp = "barcodelength";
DisplayCString(&CTemp);
sprintf (sTemp, "%d", barcodelength);
DisplayCString(&(CString)sTemp);
 	cmdInsertSampleData.m_BarCodeLen = barcodelength;

//			@sampleAttributes		varchar(1000),
	// Set fifteenth stored procedure parameter with data set attributes.
 	_tcsncpy(cmdInsertSampleData.m_setAttributes, l_setAttributes,
 		sizeof(cmdInsertSampleData.m_setAttributes) /
 		sizeof(cmdInsertSampleData.m_setAttributes[0]));

//			@Comment		varchar(1000),
	// Set sixteenth stored procedure parameter with calibration comment.
 	_tcsncpy(cmdInsertSampleData.m_Comment, " ",
 		sizeof(cmdInsertSampleData.m_Comment) /
 		sizeof(cmdInsertSampleData.m_Comment[0]));

	// Call stored procedure
	hr = cmdInsertSampleData.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(buf,"HT - Function \"LoadCalibrationDataToDB\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call to stored procedure \"InsertSampleData\"",buf);
		
		return hr;
	}

//			@calibrationID		int output)
	*calibrationID = cmdInsertSampleData.m_calibrationID;

	cmdInsertSampleData.Close();

	// Close the database.
	db.Close();
	// </ --------------- ENTER SAMPLE DATA IN DB --------------- >

	if (cmdInsertSampleData.m_RETURNVALUE != 0)
	{
CTemp = "HT - Database Error";
DisplayCString(&CTemp);
		hr = DataBaseError(cmdInsertSampleData.m_RETURNVALUE,"LoadCalibrationDataToDB",error_msg);
		return hr;
	}

	return S_OK;
}

HRESULT GetDataAndStatusForAListOfSamples(LONG calibrationID,
										  CString outliers,
										  CStringArray *sampleIdList,
										  CArray<double,double> *values,
										  CArray<int,int> *status,
										  VARIANT *error_msg)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	CString		l_sampleIdList;
	char		*token;
	char		seps[] = ",\n";
	char		buf[512];

	CCommand<CAccessor<CdboGetValuesAndStatusAccessor> > cmdGetGetSampleValuesAndStatus;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// < ----- STORED PROCEDURE REQUIRE A FLAT STRING ----- >
	hr = FromCStringArrayToCString(sampleIdList,&l_sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetDataAndStatusForAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// </ ----- STORED PROCEDURE REQUIRE A FLAT STRING ----- >

	// < ----- GET SAMPLE VALUES AND STATUS ----- >
	// Open database.
	hr = OpenDatabase( &db, &session, "GetDataAndStatusForAListOfSamples" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetDataAndStatusForAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return hr;
	}

	// Create command to execute stored procedure "GetValuesAndStatus".
	hr = cmdGetGetSampleValuesAndStatus.Create(session, _T("{ ? = CALL dbo.GetValuesAndStatus;1 (?,?,?,?,?) }"));
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetDataAndStatusForAListOfSamples\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"GetValuesAndStatus\" stored procedure",buf);
		return hr;
	}

	// Set database search criterium.
	cmdGetGetSampleValuesAndStatus.m_calibrationID = calibrationID;

	// Set database searching criteria.
 	_tcsncpy(cmdGetGetSampleValuesAndStatus.m_SampleIdList, l_sampleIdList,
 		sizeof(cmdGetGetSampleValuesAndStatus.m_SampleIdList) /
 		sizeof(cmdGetGetSampleValuesAndStatus.m_SampleIdList[0]));

	// Set database searching criteria.
 	_tcsncpy(cmdGetGetSampleValuesAndStatus.m_outliers, outliers,
 		sizeof(cmdGetGetSampleValuesAndStatus.m_outliers) /
 		sizeof(cmdGetGetSampleValuesAndStatus.m_outliers[0]));

	// Get sample sizes.
	hr = cmdGetGetSampleValuesAndStatus.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call \"GetValuesAndStatus\" stored procedure","GetDataAndStatusForAListOfSamples");
		return hr;
	}

	// Convert sample value.
	token = strtok( cmdGetGetSampleValuesAndStatus.m_SampleValues, seps );
	while( token != NULL )
	{
		values->Add(atof(token));
		// Get next sample value.
		token = strtok( NULL, seps );
	}

	// Convert value status.
	token = strtok( cmdGetGetSampleValuesAndStatus.m_SampleStatus, seps );
	while( token != NULL )
	{
		status->Add(atoi(token));
		// Get next value status.
		token = strtok( NULL, seps );
	}

	cmdGetGetSampleValuesAndStatus.Close();
	// </ ----- GET SAMPLE VALUES AND STATUS ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT GetSampleValueIDsForIndexing(LONG calibrationID,
									 CArray<long,long> *indexingIDs,
									 VARIANT *error_msg)
{

	HRESULT			hr;
	CDataSource		db;
	CSession		session;
	char			*token;
	char			seps[] = ",\n";

	CCommand<CAccessor<CdboGetSampleValuesIDsAccessor> > cmdValueIDs;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSampleValueIDsForIndexing" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetSampleValueIDsForIndexing");
		return hr;
	}

	// < ----- GET VALUEid ----- >
	hr = cmdValueIDs.Create (session, _T("{ ? = CALL dbo.GetSampleValuesIDs;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to access \"GetSampleValuesIDs\" stored procedure","GetSampleValueIDsForIndexing");
		return hr;
	}

	// Set database searching criteria.
	cmdValueIDs.m_calibrationID = calibrationID;

	_tcsncpy(cmdValueIDs.m_type, (CString)"CALIBRATOR",
		sizeof(cmdValueIDs.m_type) /
		sizeof(cmdValueIDs.m_type[0]));

	// Execute current command.
	// Call stored procedure, "GetCalibrationData".
	hr = cmdValueIDs.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetSampleValuesIDs\"","GetSampleValueIDsForIndexing");
		return hr;
	}

	// Convert value index.
	token = strtok( cmdValueIDs.m_SampleValueIDs, seps );
	while( token != NULL )
	{
		indexingIDs->Add(atof(token));
		// Get next value index.
		token = strtok( NULL, seps );
	}

	// Close access command.
	cmdValueIDs.Close();
	// </ ----- GET VALUEid ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT FindChemistrySpeciesDependencies(CString chemistry,
										 CString *calibrationSpecies)
{
	HRESULT			hr;
	CDataSource		db;
	CSession		session;

	CCommand<CAccessor<CdboGetChemistrySpeciesDependenciesAccessor> > cmdDependencies;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSampleValueIDsForIndexing" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","FindChemistrySpeciesDependencies");
		return hr;
	}

	// < ----- GET VALUEid ----- >
	hr = cmdDependencies.Create (session, _T("{ ? = CALL dbo.GetChemistrySpeciesDependencies;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to access \"GetChemistrySpeciesDependencies\" stored procedure","FindChemistrySpeciesDependencies");
		return hr;
	}

	// Set database searching criteria.
	_tcsncpy(cmdDependencies.m_chemistry, chemistry,
		sizeof(cmdDependencies.m_chemistry) /
		sizeof(cmdDependencies.m_chemistry[0]));

	_tcsncpy(cmdDependencies.m_species, *calibrationSpecies,
		sizeof(cmdDependencies.m_species) /
		sizeof(cmdDependencies.m_species[0]));

	// Execute current command.
	// Call stored procedure, "GetCalibrationData".
	hr = cmdDependencies.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetChemistrySpeciesDependencies\"","FindChemistrySpeciesDependencies");
		return hr;
	}

	*calibrationSpecies = cmdDependencies.m_species;

	// Close access command.
	cmdDependencies.Close();
	// </ ----- GET VALUEid ----- >

	// Close the database.
	db.Close();

	return S_OK;
}
