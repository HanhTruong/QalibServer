/*******************************************************************
FILENAME:	Controls.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Main module for calibration processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	TestCurrentControls

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	Controls.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include "Support.h"
#include "Loging.h"

/*****************************************************************
int TestCurrentControls(LONG calibrationID)

INPUT
	calibrationID - Current calibration id.

OUTPUT
	none.

LOCAL
	hr - Function status.
	sampleIds - Sample list.
	ret_value - Number of controls.

RETURN
	Number of controls in current calibration.

*****************************************************************/
int TestCurrentControls(LONG calibrationID)
{
	HRESULT			hr;
	CStringArray	sampleIds;
	int ret_value = 0;

	// Get calibrator type sample names.
	hr = GetSampleIdsFromType(calibrationID,(CString)"CONTROL",&sampleIds);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSampleIdsFromType\"","TestCurrentControls");
		return hr;
	}

	ret_value = sampleIds.GetSize();

	return ret_value;
}

