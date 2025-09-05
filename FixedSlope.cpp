#include "stdafx.h"
#include "Support.h"
#include "DbAccess.h"
#include "Loging.h"
#include "globals.h"
#include "FixedSlope.h"
#include "dboGetFixedSlopeValues.H"
#include "LinearCalibration.h"

HRESULT SetFixedSlope(LONG calibrationID, dVector lParams)
{
	HRESULT hr;
	CString diluent, chemistry;
	double	value, lower, upper;
	char	buf[512];

	// < ------ Find diluent for calibration with id calibrationID. ------ >
	hr = GetCalibrationIDDiluent(calibrationID, &diluent);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SetFixedSlope\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"GetCalibrationIDDiluent\" function",	buf);
		return hr;
	}
	// < ------ Find diluent for calibration with id calibrationID. ------ >

	// < ------- Find chemistry name from calibrationID. ------ >
	hr = GetChemistryNameGivenCalibrationID(calibrationID, &chemistry);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SetFixedSlope\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"GetChemistryNameGivenCalibrationID\" function",	buf);
		return hr;
	}
	// </ ------- Find chemistry name from calibrationID. ------ >

	// < ------- Get fixed slope values. ------- >
	hr = GetFixedSlopeValues(diluent, chemistry, &value, &lower, &upper);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"SetFixedSlope\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"GetFixedSlopeValues\" function",	buf);
		return hr;
	}
	// </ ------- Get fixed slope values. ------- >

	// Set fit parameter to fixed slope value.
	lParams[0] = 1.0/value;

	return S_OK;
}

HRESULT GetFixedSlopeValues(CString diluent, 
							CString chemistry,
							double	*value,
							double	*lower,
							double	*upper)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;
	char		buf[256];

	// Instantiate a command object, to get fixed slope values.
	CCommand<CAccessor<CdboGetFixedSlopeValuesAccessor> > cmdGetFixeSlopeValues;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetFixedSlopeValues" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetFixedSlopeValues\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from \"OpenDatabase\" function", buf);
		return hr;
	}

	// < --------------- GET FIXED SLOPE VALUES --------------- >
	hr = cmdGetFixeSlopeValues.Create(session, _T("{ ? = CALL dbo.GetFixedSlopeValues;1 (?,?) }"));
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetFixedSlopeValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, 
			"Create command for \"GetFixedSlopeValues\" stored procedure",
			buf);
		return hr;
	}

	// Set database searching criteria.
 	_tcsncpy(cmdGetFixeSlopeValues.m_chemistry, chemistry,
 		sizeof(cmdGetFixeSlopeValues.m_chemistry) /
 		sizeof(cmdGetFixeSlopeValues.m_chemistry[0]));

	_tcsncpy(cmdGetFixeSlopeValues.m_Diluent, diluent,
 		sizeof(cmdGetFixeSlopeValues.m_Diluent) /
 		sizeof(cmdGetFixeSlopeValues.m_Diluent[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetFixeSlopeValues.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetFixedSlopeValues\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call \"GetFixedSlopeValues\" stored procedure",buf);
		return hr;
	}

	cmdGetFixeSlopeValues.MoveFirst();
	*value = cmdGetFixeSlopeValues.m_colSlope;
	*lower = cmdGetFixeSlopeValues.m_colLowerLimitPercent;
	*upper = cmdGetFixeSlopeValues.m_colUpperLimitPercent;

	cmdGetFixeSlopeValues.Close();
	// </ --------------- GET FIXED SLOPE VALUES --------------- >

	// Close the database.
	db.Close();

	return S_OK;
}

HRESULT FixedSlopeCalculations(LONG calibrationID,
							   CString algorithm,
							   double *chisquared,
							   VARIANT *newFitParameters,
							   VARIANT *error_msg)
{
	HRESULT	hr;
	char	buf[512];

	// Is this calculation for updated fit parameters?
	if (newFitParameters->vt == VT_EMPTY)
	{
		hr = SetFixedSlope(calibrationID, FitParameters);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,	"Return from function \"SetFixedSlope\"", buf);
			return hr;
		}
	}
	else
	{
		// Parse new fit parameter array.
		hr = EvaluateLinearFitParameters(newFitParameters, error_msg);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"EvaluateLinearFitParameters\"",buf);
			return hr;
		}
	}

	hr = LinearCalibration(calibrationID,
						   newFitParameters,
						   algorithm,
						   chisquared,
						   error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetUpdatedFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,	"Return from function \"LinearCalibration\"", buf);
		return hr;
	}

	return S_OK;
}
