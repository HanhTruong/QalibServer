#include "stdafx.h"
#include "Loging.h"
#include "Support.h"
#include "Utils.h"
#include "ClientData.h"
#include "BarCodes.h"
#include "Rotor.h"
#include "DbAccess.h"
#include "dboGetSampleInstruments.H"
#include "dboGetCalibrationMode.H"
#include "Species.h"

HRESULT GetSampleInstruments(LONG calibrationID, 
							 CString sampleId,
							 CStringArray *instrumentList,
							 VARIANT *error_msg)
{
	HRESULT hr=S_OK;
	CDataSource	db;
	CSession	session;

	// Define command to find sample instruments list.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetSampleInstrumentsAccessor> > cmdGetSampleInstruments;

	// Open database.
	hr = OpenDatabase( &db, &session, "GetSampleInstruments" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetSampleInstruments");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetSampleInstruments.Create(session, _T("{ ? = CALL dbo.GetSampleInstruments;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to call \"GetSampleInstruments\"","GetSampleInstruments");
		return hr;
	}

	// Set database searching criteria.
	cmdGetSampleInstruments.m_calibrationID = calibrationID;

	// Set database searching criteria.
 	_tcsncpy(cmdGetSampleInstruments.m_sampleID, sampleId,
 		sizeof(cmdGetSampleInstruments.m_sampleID) /
 		sizeof(cmdGetSampleInstruments.m_sampleID[0]));

	// Get fit parameters rotor factors.
	// Call stored procedure.
	hr = cmdGetSampleInstruments.Open();
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetSampleInstruments\"","GetRotorAttributes");
		return hr;
	}

	while (cmdGetSampleInstruments.MoveNext() == S_OK)
	{
		instrumentList->Add(cmdGetSampleInstruments.m_colInstrument);
	}

	cmdGetSampleInstruments.Close();

	// Close the database.
	db.Close();

	return hr;
}

HRESULT SendClientDataToDb(BSTR				chemistry,
						   INT				outlierMethod,
						   BSTR				mode,
						   BSTR				user,
						   CString			calibrationSpecies,
						   CStringArray		*inputData,
						   CStringArray		*setAttributes,
						   CStringArray		*serialNumbers,
						   CStringArray		*barCode,
						   CStringArray		*sampleSpecies,
						   CArray<int,int>	*statusData,
						   CArray<int,int>	*sampleSizes,
						   CStringArray		*sampleIds,
						   LONG				*calibrationID,
						   VARIANT			*error_msg)
{
	HRESULT			hr=S_OK;
	int				dataSetLength=0;
	CPtrArray		dataSets;
	char			buf[512];
	CStringArray	barCodeAttributes;
	CStringArray	rotorAttributes;

	// < ----- TEST SAMPLE SPECIES ----- >
	hr = VerifySpecies(*calibrationID,
					   sampleIds,
					   sampleSpecies,
					   sampleSizes,
					   error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"VerifySpecies\"",buf);
		}

		return hr;
	}
	// </ ----- TEST SAMPLE SPECIES ----- >

	// < ----- TEST LIST OF BARCODES ----- >
	hr = VerifyBarCodes(barCode, error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3","Error: Bar code.",error_msg);
		}
		else
		{
			sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"VerifyBarCodes\"",buf);
		}

		return hr;
	}
	// </ ----- TEST LIST OF BARCODES ----- >

	// < ----- GET BARCODE ATTRIBUTES ----- >
	// Any bar code will work here. Assuming VerifyBarCodes worked.
	hr = GetBarCodeAttributes(barCode->GetAt(0), &barCodeAttributes, error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3","Problem with bar code attributes",error_msg);
			sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetBarCodeAttributes\"",buf);
		}

		return hr;
	}
	// </ ----- GET BARCODE ATTRIBUTES ----- >

	// < ----- TEST CHEMISTRY ROTOR AND CUVETTE ATTRIBUTES ----- >
	// Any bar code will work here. Assuming VerifyBarCodes worked.
	hr = GetRotorCuvetteAttributes(barCodeAttributes.GetAt(ROTORNUMBER),
							(CString)chemistry,
							&rotorAttributes, error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3","Problem with rotor",error_msg);
			sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetRotorCuvetteAttributes\"",buf);
		}

		return hr;
	}
	// </ ----- TEST CHEMISTRY ROTOR AND CUVETTE ATTRIBUTES ----- >

	// < ----- CONVERT ROTOR AND CUVETTE ATTRIBUTES TO STORE IN DB ----- >
	int barcodelen,mold,rotor,cuvette;
	mold = atoi(barCodeAttributes.GetAt(MOLDNUMBER));
	rotor = atoi(barCodeAttributes.GetAt(ROTORNUMBER));
	cuvette = atoi(rotorAttributes.GetAt(CUVETTE));
	barcodelen = BARCODELENGTH;

	// Calculate number of data values.
	for (int i=0; i<sampleSizes->GetSize(); i++)
	{
		dataSetLength += sampleSizes->GetAt(i);
	}
	// </ ----- CONVERT ROTOR AND CUVETTE ATTRIBUTES TO STORE IN DB ----- >

	// Enter data into database.
	hr = LoadCalibrationDataToDB(chemistry,
								 mode,
								 user,
								 mold,
								 rotor,
								 cuvette,
								 barcodelen,
								 calibrationSpecies,
								 inputData,
								 setAttributes,
								 sampleSizes,
								 statusData,
								 sampleIds,
								 sampleSpecies,
								 serialNumbers,
								 barCode,
								 calibrationID,
								 error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3","Problem with sending calibration data to server",error_msg);
		}

		sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"LoadCalibrationDataToDB\"",buf);

		return hr;
	}

	return hr;
}

HRESULT CalibrationMode(LONG calibrationID, CString *mode, VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	CDataSource	db;
	CSession	session;

	// Define command to find calibration mode.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetCalibrationModeAccessor> > cmdGetCalibrationMode;

	// Open database.
	hr = OpenDatabase( &db, &session, "CalibrationMode" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","CalibrationMode");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetCalibrationMode.Create(session, _T("{ ? = CALL dbo.GetCalibrationMode;1 (?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to call \"GetCalibrationMode\"","CalibrationMode");
		return hr;
	}

	// Set database searching criteria.
	cmdGetCalibrationMode.m_calibrationID = calibrationID;

	// Get fit parameters rotor factors.
	// Call stored procedure.
	hr = cmdGetCalibrationMode.Open();
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetCalibrationMode\"","CalibrationMode");
		return hr;
	}

	cmdGetCalibrationMode.MoveFirst();

	*mode = cmdGetCalibrationMode.m_colMode;

	cmdGetCalibrationMode.Close();

	// Close the database.
	db.Close();

	return hr;
}
