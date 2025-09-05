/********************************************************************

FILENAME:	FitParameters.cpp
_____________________________________________________________________

MODULE DESCRIPTION:
	Contains functions that process fit parameter functionality.

_____________________________________________________________________

GLOBAL FUNCTIONS:

	Name			Description

_____________________________________________________________________

COMPILER:	Visual C++ 6.0		INVOCATION: Build->FitParameters.cpp

_____________________________________________________________________

HISTORY:

Version	Date		Author		Description
1.0.0.0	07/27/04	Alves		Fit parameter specific processing.

********************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include "Loging.h"
#include "globals.h"
#include "DbAccess.h"
#include "FitParameters.h"

#include "dboGetFitParametersLimits.H"

/****************************************************************
void InitializeFitParameters(dVector LowerLimits, 
							 dVector UpperLimits, 
							 dVector FitPars, 
							 dVector YExp, 
							 ui sizex)

 Initializes fit parameters from the given data.

-----------------------------------------------------------------
 INPUT
	YExp - Experimental data.
	size - Size of experimental data.

-----------------------------------------------------------------
 OUTPUT
	Global fit parameters vector updated to calculated values.

-----------------------------------------------------------------
 LOCAL
	None.

-----------------------------------------------------------------
 RETURN														
	None.														
****************************************************************/
void InitializeFitParameters(dVector LowerLimits, 
							 dVector UpperLimits, 
							 dVector FitPars, 
							 dVector YExp, 
							 ui size)
{
	// Find the largest element of a vector.
	FitPars[Top] = VD_max( YExp, size );

	// Finds the smallest (or the most negative) element of a vector.
	FitPars[Bottom] =  VD_min( YExp, size );

	// Initilize slope and intercept to mid-range of barcode limits.
	FitPars[Slope] = ((UpperLimits[Slope] - LowerLimits[Slope]) / 2) + LowerLimits[Slope];

	FitPars[Intercept] = FitPars[Slope] ;
}

/********************************************************************
FUNCTION:
	SetFitParametersLimitsResults
_____________________________________________________________________
DESCRIPTION:
	Sets global vectors with upper and lower limits for fit parameters.

_____________________________________________________________________
GLOBAL VARIABLES:
____________________________________________________________________
LOCAL VARIABLES:
	token - Current variable from fit parameter list.
	seps[] - Token delimeter.

____________________________________________________________________
PARAMETERS:
	parameterLimits - Current list of fit parameters list.
	Lower - Vector containing lower fit parameter limits.
	Upper - Vector containing upper fit parameter limits.
	algorithm - Algorithm for current chemistry.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0 7/27/4		Alves		Original code.

********************************************************************/
void SetFitParametersLimitsResults(TCHAR *parameterLimits,
								   dVector Lower,
								   dVector Upper,
								   CString algorithm)
{
	char		*token;
	char		seps[] = ",\n";

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		// Get slope's lower limit.
		token = strtok( parameterLimits, seps );
		// Convert parameter limits.
		Lower[Slope] = atof(token);
		// Get slope's upper limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Slope] = atof(token);

		// Get intercept's lower limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Intercept] = atof(token);
		// Get intercept's upper limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Intercept] = atof(token);

		// Get top's lower limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Top] = atof(token);
		// Get top's upper limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Top] = atof(token);

		// Get bottom's lower limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Bottom] = atof(token);
		// Get bottom's upper limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Bottom] = atof(token);
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("EXPONENTIAL") == 0) ||
			 (algorithm.CompareNoCase("FIXEDSLOPE") == 0))
	{
		// Get slope fit parameter.
		token = strtok( parameterLimits, seps );
		// Convert parameter limits.
		Lower[Slope] = atof(token);
		// Get next parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Slope] = atof(token);

		// Get intercept parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Intercept] = atof(token);
		// Get next parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Intercept] = atof(token);
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		// Get slope fit parameter.
		token = strtok( parameterLimits, seps );
		// Convert parameter limits.
		Lower[Slope] = atof(token);
		// Get next parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Slope] = atof(token);

		// Get anchor parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Anchor] = atof(token);
		// Get next parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Anchor] = atof(token);

		// Get curvature parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Lower[Curvature] = atof(token);
		// Get next parameter limit.
		token = strtok( NULL, seps );
		// Convert parameter limits.
		Upper[Curvature] = atof(token);
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
	else
	{
	}
}

/****************************************************************
HRESULT GetFitParsLimits( LONG calibrationID,
						  dVector Lower,
						  dVector Upper,
						  CString algorithm,
						  ui size,
						  CString wavelength,
						  VARIANT *error_msg)

 Get fit parameter limits from the database.

-----------------------------------------------------------------
 INPUT
	calibrationDate - Date of calibration to get fit parameter limits.
	Lower - Vector containing lower limits.
	Upper - Vector containing upper limits.
	size - Length of upper and lower limits.
	error_msg - Error reporting.

-----------------------------------------------------------------
 OUTPUT
	Lower and Upper vector filled with limit values.

-----------------------------------------------------------------
 LOCAL														
	hr - System return code.									
	db - Database structure.									
	session - Session required by database access.				
	cmdGetParameterLimits - Command to call stored procedure.	
	cmdinit - Command properties.

-----------------------------------------------------------------
 RETURN														
	Lower and Upper vector with database limit values.			
****************************************************************/
HRESULT GetFitParsLimits( LONG calibrationID,
						  dVector Lower,
						  dVector Upper,
						  CString algorithm,
						  ui size,
						  CString wavelength,
						  VARIANT *error_msg)
{
	HRESULT		hr=S_OK;
	CDataSource	db;
	CSession	session;

	// Define command to get Fit Parameter Limits from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetFitParametersLimitsAccessor> > cmdGetParameterLimits;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetFitParsLimits");
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Open database failed","GetFitParsLimits");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetParameterLimits.Create (session, _T("{ ? = CALL dbo.GetFitParametersLimits;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();
		LogInterfaceErrorMessage(hr,"Create command, to call stored procedure GetFitParametersLimits","GetFitParsLimits");
		return hr;
	}

	// Set database searching criteria.
	// Calibration id and algorithm.
	cmdGetParameterLimits.m_calibrationID = calibrationID;

	_tcsncpy(cmdGetParameterLimits.m_algorithm, algorithm,
		sizeof(cmdGetParameterLimits.m_algorithm) /
		sizeof(cmdGetParameterLimits.m_algorithm[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetParameterLimits.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();
		LogInterfaceErrorMessage(hr,"Call to \"GetFitParametersLimits\" stored procedure","GetFitParsLimits, Support.cpp");
		return hr;
	}

	// GetFitParametersLimits response depend on current algorithm.
	if (cmdGetParameterLimits.m_RETURNVALUE > 0)
	{
		CString temp;

		// Close the database.
		db.Close();

		hr = 0x80040E21;

		TestGetFitParameterLimits(algorithm,cmdGetParameterLimits.m_RETURNVALUE,&temp);
		LogErrorMessage(hr,"Return from stored procedure \"GetFitParametersLimits\"",temp);

		return hr;
	}

	SetFitParametersLimitsResults( cmdGetParameterLimits.m_parameterLimits,
								   Lower,
								   Upper,
								   algorithm);

	cmdGetParameterLimits.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/*
*/
void TestGetFitParameterLimits(CString algorithm, LONG error_code, CString *msg)
{

	if (algorithm.CompareNoCase("SIGMOIDAL") == 0)
	{
		// Set return message, depending on error code.
		switch(error_code)
		{
			case 1:
				*msg = "Fit parameter limits not found in [SigmoidalFitParameterLimits] table.";
			break;
		}
	}
	else if ((algorithm.CompareNoCase("LINEAR") == 0) ||
			 (algorithm.CompareNoCase("EXPONENTIAL") == 0))
	{
		// Set return message, depending on error code.
		switch(error_code)
		{
			case 1:
				*msg = "Chemistry id and/or species not found in [Calibration] table.";
			break;		
			case 2:
				*msg = "Wavelength not found in [CalibrationSampleValues] table.";
			break;
			case 3:
				*msg = "Fit parameter limits not found in [LinearFitParameterLimitsWavelengthDependent] table.";
			break;
			case 4:
				*msg = "Fit parameter limits not found in [LinearFitParameterLimitsSpeciesDependent] table.";
			break;
			case 5:
				*msg = "Fit parameter limits not found in [LinearFitParameterLimits] table.";
			break;
		}
	}
	else if (algorithm.CompareNoCase("FIXEDSLOPE") == 0)
	{
		// Set return message, depending on error code.
		switch(error_code)
		{
			case 1:
				*msg = "Slope limits not found in [SlopesAndLimits] table.";
			break;
			
			case 2:
				*msg = "Intercept limits not found in [InterceptLimits] table.";
			break;
			
			case 3:
				*msg = "Diluent limits not found in [Rotor] table.";
			break;
		}
	}
	else if (algorithm.CompareNoCase("QUADRATIC") == 0)
	{
		// Set return message, depending on error code.
		switch(error_code)
		{
			case 1:
				*msg = "Slope limits not found in table [SlopesAndLimits].";
			break;
			
			case 2:
				*msg = "Intercept limits not found in [InterceptLimits] table.";
			break;
			
			case 3:
				*msg = "Diluent or chemistry names not found in [Rotor] table.";
			break;
		}
	}
	else if (algorithm.CompareNoCase("MIN_MAX_DECAY") == 0)
	{
	}
}