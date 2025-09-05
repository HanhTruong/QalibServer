#include "stdafx.h"
#include "Loging.h"
#include "Support.h"
#include "globals.h"

/*****************************************************************
HRESULT UpdateLinearFitParameters(LONG calibrationID,
								  VARIANT *error_msg)

Update slope and intercept, depending on algorithm and chemistry.


INPUT
	calibrationID - Current calibration id.
	error_msg - Error reporting.

OUTPUT
	fitParameters - New values for fit parameters.

LOCAL
	hr - Return code.

*****************************************************************/
HRESULT UpdateLinearFitParameters(LONG calibrationID,
								  VARIANT *error_msg)
{
	HRESULT hr=S_OK;
	CString algorithm;
	char buf[512];

	// < ----- GET ALGORITHM TYPE ----- >
	// Fit parameters have to be updated depending on algorithm type.
	hr = FindCurrentAlgorithmType(calibrationID, &algorithm);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"UpdateLinearFitParameters\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, "Return from function \"FindCurrentAlgorithmType\"", buf);
		return hr;
	}
	// </ ----- GET ALGORITHM TYPE ----- >

	if (algorithm.CompareNoCase("FIXEDSLOPE") == 0)
	{
		// Intercept calibration factor from (-regression_intercept/regression_slope)
		// regression_intercept returned in LinearParams[1] from VD_linifitwW.
		FitParameters[1] = -FitParameters[1] / FitParameters[0];

		// Slope calibration factor from 1/regression_slope
		// regression_slope returned in fitParameters[0] from VD_linifitwW.
		FitParameters[0] = 1 / FitParameters[0];
	}
	else if (algorithm.CompareNoCase("LINEAR") == 0)
	{
		// < ----- TEST PARAMETER's STATUS ----- >
		if (FitParameterStatus[1] == 1)
		{
			// Intercept calibration factor from (-regression_intercept/regression_slope)
			// regression_intercept returned in LinearParams[1] from VD_linifitwW.
			FitParameters[1] = -FitParameters[1] / FitParameters[0];
		}

		if (FitParameterStatus[0] == 1)
		{
			// Slope calibration factor from 1/regression_slope
			// regression_slope returned in LinearParams[0] from VD_linifitwW.
			FitParameters[0] = 1 / FitParameters[0];
		}
		// </ ----- TEST PARAMETER's STATUS ----- >
	}

	return hr;
}
