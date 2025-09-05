/*******************************************************************

FILENAME:	BarCodes.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for bar codes processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	VerifyBarCodes
	GetBarCodeAttributes
	GetCalibrationBarCode

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BarCodes.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include "BarCodes.h"
#include "Support.h"
#include "Loging.h"
#include "DbAccess.h"
#include "dboGetCalibrationBarCode.H"


/*****************************************************************
HRESULT VerifyBarCodes(CStringArray *sampleBarCode, 
					   VARIANT *error_msg)

  Test current bar code format.

INPUT
	sampleBarCode - List of bar codes to verify.
	error_msg - Report error.

OUTPUT
	None.

LOCAL
	hr - Function status code.
	barCodeHeader - Bar code header.
	i - Barcode list index.
	buf[512] - Error reporting buffer.

RETURN
	hr - Function status.
*****************************************************************/
HRESULT VerifyBarCodes(CStringArray *sampleBarCode, 
					   VARIANT *error_msg)
{
	HRESULT hr;
	CString	barCodeHeader;
	int		i;
	char	buf[512];

	// Extract barcode header.
	barCodeHeader = sampleBarCode->GetAt(0).Left(16);

	i = 1;

	while (i<sampleBarCode->GetSize())
	{
		if (barCodeHeader.CompareNoCase(sampleBarCode->GetAt(i).Left(16)) != 0)
		{
			hr = 0x80040E21;

			UserErrorMessage("3","Only one barcode per calibration!",error_msg);

			sprintf(buf,"Function \"VerifyBarCodes\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"GetBarCodeAttributes\"",buf);

			return hr;
		}

		i++;
	}

	return S_OK;
}


/*****************************************************************
HRESULT GetBarCodeAttributes(CString barCode, 
							 CStringArray *barCodeAttributes, 
							 VARIANT *error_msg)

  Get bar code contents.

INPUT
	barCode - Bar code to search for attributes.
	barCodeAttributes - Attributes of current bar code.
	error_msg - Report error.

OUTPUT
	barCodeAttributes.

LOCAL
	hr - Function status code.
	barcodeHeader - Extracted bar code header.
	rotorNumber - Extracted rotor number.
	expirationDate - Extracted expiration date.
	barcodeFormatNumber - Extracted bar code format number.
	rotorLotNumber  - Extracted lot number.
	moldNumber  - Extracted mold number.

RETURN
	hr - Function status.

*****************************************************************/
HRESULT GetBarCodeAttributes(CString barCode, 
							 CStringArray *barCodeAttributes, 
							 VARIANT *error_msg)
{
	HRESULT hr = S_OK;

	CString	barcodeHeader;
	CString	rotorNumber;
	CString	expirationDate;
	CString barcodeFormatNumber;
	CString rotorLotNumber;
	CString moldNumber;

	// -----------------------------------------------------
	// Barcode format, from left to right:
	//	rotor number 2 characters
	//	format number 1 character
	//	expiration date 6 characters
	//	rotor lot number 7 characters
	// -----------------------------------------------------
	// Rotor lot format, from left to right:
	//
	//	1 character - last number of the year of production.
	//
	//	2 characters - week that the rotor lot was made.
	//
	//	1 character - day of the week
	//			1 = Monday
	//			2 = Tuesday
	//			3 = Wedday
	//			4 = Thursday
	//			5 = Friday
	//			6 = Saturday
	//			7 = Sunday
	//
	//	1 character - build of the day, "A-J". 
	//				  "C" for calibration lots.
	//
	//	1 character - line lot was build on. "A-I" will be used.
	//				  A=Semi Auto Line
	//				  B=Auto Line
	//
	//	1 character - mold rotor was build on.
	//	1 character - the package lot of that rotor lot.
	// -----------------------------------------------------
	barcodeHeader = barCode.Left(BARCODEHEADER);

	rotorNumber = barcodeHeader.Left(2);
	barCodeAttributes->Add(rotorNumber);

	barcodeFormatNumber = barcodeHeader.Mid(2,1);
	barCodeAttributes->Add(barcodeFormatNumber);

	expirationDate = barcodeHeader.Mid(3,6);
	barCodeAttributes->Add(expirationDate);

	rotorLotNumber = barcodeHeader.Right(7);
	barCodeAttributes->Add(rotorLotNumber);

	moldNumber = rotorLotNumber.Mid(6,1);
	barCodeAttributes->Add(moldNumber);

	return hr;
}


/*****************************************************************
HRESULT GetCalibrationBarCode(LONG calibrationID,
							  CString *barCode,
							  VARIANT *error_msg)

INPUT
	calibrationID - Identifier of current calibration.
	barCode - Bar code.
	error_msg - Error messages.

OUTPUT
	barCode - Containing current bar code.

LOCAL
	hr - Function status.
	db - Database connection.
	session - Single database access session.
	cmdGetBarCode - Instantiate a command object, to get barcode.
	cmdinit - Set properties for command.

RETURN

*****************************************************************/
HRESULT GetCalibrationBarCode(LONG calibrationID,
							  CString *barCode,
							  VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	CDataSource db;
	CSession	session;

	// Instantiate a command object, to get barcode.
	CCommand<CAccessor<CdboGetCalibrationBarCodeAccessor> > cmdGetBarCode;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationBarCode" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetCalibrationBarCode");
		return hr;
	}

	// Create command to execute stored procedure "GetChemistryName".
	hr = cmdGetBarCode.Create(session, _T("{ ? = CALL dbo.GetCalibrationBarCode;1 (?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();
		LogInterfaceErrorMessage(hr,"Create command for \"GetCalibrationBarCode\" stored procedure","GetChemistryNameGivenCalibrationID");
		return hr;
	}

	// Set database search criterium.
	cmdGetBarCode.m_calibrationID = calibrationID;

	// Get sample names.
	hr = cmdGetBarCode.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();
		LogInterfaceErrorMessage(hr,"Call \"GetCalibrationBarCode\" stored procedure","GetChemistryNameGivenCalibrationID");
		return hr;
	}

	cmdGetBarCode.MoveFirst();

	*barCode = cmdGetBarCode.m_colBarCode;

	cmdGetBarCode.Close();

	// Close the database.
	db.Close();

	return hr;
}
