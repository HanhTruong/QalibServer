#include "stdafx.h"
#include "Loging.h"
#include "DbAccess.h"
#include "Support.h"
#include "Utils.h"
#include "BarCodes.h"
#include "dboRotorCuvetteAttributes.H"
#include "BusinessRulesProcess.h"
#include "dboGetRotorFactors.H"

/*************************************************************
HRESULT GetRotorCuvetteAttributes(CString rotorNumber,
						   CString chemistry,
						   CStringArray *rotorAttributes, 
						   VARIANT *error_msg)

--------------------------------------------------------------
 INPUT
	rotorNumber - Rotor number, to find attributes for.
	chemistry - Chemistry name.
	rotorAttributes - Return rotor attributes.
	error_msg - Error reporting.

--------------------------------------------------------------
 OUTPUT
	rotorAttributes - Rotor attributes, in this variable.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.
	cmdGetRotorAttributes - Object for db results.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT GetRotorCuvetteAttributes(CString rotorNumber,
								  CString chemistry,
								  CStringArray *rotorAttributes,
								  VARIANT *error_msg)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;
	int			l_rotorNumber;
	char		buffer[20];
	char		userMsg[512];

	// Define command to find rotor attributes used for the current calibration.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboRotorCuvetteAttributesAccessor> > cmdGetRotorAttributes;

	l_rotorNumber = atoi(rotorNumber);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetRotorCuvetteAttributes" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function","GetRotorCuvetteAttributes");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetRotorAttributes.Create(session, _T("{ ? = CALL dbo.RotorCuvetteAttributes;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to call \"RotorCuvetteAttributes\"","GetRotorAttributes");
		return hr;
	}

	// Set database searching criteria.
	cmdGetRotorAttributes.m_rotorID = l_rotorNumber;

	// Set database searching criteria.
	_tcsncpy(cmdGetRotorAttributes.m_chemistry, chemistry,
		sizeof(cmdGetRotorAttributes.m_chemistry) /
		sizeof(cmdGetRotorAttributes.m_chemistry[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetRotorAttributes.Open();
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"RotorCuvetteAttributes\"","GetRotorAttributes");
		return hr;
	}

	while (cmdGetRotorAttributes.MoveNext() == S_OK)
	{
		_itoa(cmdGetRotorAttributes.m_colCuvette, buffer, 10 );
		rotorAttributes->Add(buffer);
	}

	cmdGetRotorAttributes.Close();

	// Close the database.
	db.Close();

	if (rotorAttributes->GetSize() > 0)
	{
		hr=S_OK;
	}
	else
	{
		hr=0x80040E21;

		sprintf(userMsg,"Chemistry \"%s\", is not in rotor \"%d\"!", chemistry, l_rotorNumber);
		UserErrorMessage("3", userMsg, error_msg);
	}

	return hr;
}

