#include "stdafx.h"
#include <math.h>

#include "Loging.h"

#include "AnalyteTypeClass.h"
#include "Utils.h"
#include "Support.h"
#include "BusinessRulesSupport.h"
#include "AssignedValues.h"

/************************************************************
HRESULT Zero_T4_Level(LONG calibrationID,
					  BSTR partNumber,
					  VARIANT *error_msg)

	Implement ZERO_T4_LEVEL business rule.
	One of the T4 levels must be 0 ug/dl.
	Find zero level analyte for Thyroxine, T4, chemistry.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find zero level.
	partNumber - Chemistry to find zero level.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	T4 zero level.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	assignedValues - List of assigned values to search for
					 to factors.
	buf[256] - Log error messages.

	sampleIDList - List of sample ids.
	assignedValues - Assigned values list.
	sampleAnalyteLevelList - List of analyte levels.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Zero_T4_Level(LONG calibrationID,
					  CString chemistry,
					  VARIANT *error_msg)
{
	HRESULT					hr;
	CString					sampleIDList;
	CStringArray			a_sampleIDList;
	CArray<double,double>	assignedValues;
	char					buf[512];
	CArray<int,int>			sampleAnalyteLevelList;

	// < --------------- GET SAMPLE NAMES --------------- >
	hr = GetCalibrationIDSamples(calibrationID, &a_sampleIDList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Zero_T4_Level\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ --------------- GET SAMPLE NAMES --------------- >

	hr = FromCStringArrayToCString(&a_sampleIDList,&sampleIDList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Zero_T4_Level\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}

	// < --------------- GET ASSIGNED VALUES --------------- >
	hr = GetAssignedValues(chemistry, sampleIDList, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Zero_T4_Level\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetAssignedValues\" function",buf);
		return hr;
	}
	// </ -------------- GET ASSIGNED VALUES --------------- >

	// < -------------- GET ANALYTE LEVEL --------------- >
	hr = GetAnalyteLevelList(chemistry, sampleIDList, &sampleAnalyteLevelList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Zero_T4_Level\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetAnalyteLevelList\" function",buf);
		return hr;
	}
	// </ -------------- GET ANALYTE LEVEL --------------- >

	int i;
	bool found;
	
	i = 0;
	found = false;

	while ((i<sampleAnalyteLevelList.GetSize()) && !found)
	{
		if (sampleAnalyteLevelList.GetAt(i) == 0)
		{
			if (assignedValues.GetAt(i) > 0.0)
			{
				UserErrorMessage("3", 
					"This T4 data set does not have a zero level calibrator!", 
					error_msg);

				return 0x80040E21;
			}
			else
			{
				found = true;
			}
		}

		i++;
	}

	return S_OK;
}

/************************************************************
bool TestT4AnalyteTypes(CStringArray *analyteTypesList, 
						AnalyteType *current_analytes, 
						double minNumberOfCalibrators,
						VARIANT *error_msg)

	Find if analyte types are correct for T4, Thyroxine, chemistry.

-------------------------------------------------------------
 INPUT
	analyteTypesList - Analyte list to test.
	current_analytes - Object containing current valid analyte types. 
	minNumberOfCalibrators - Minimum number of calibrators.
	error_msg - Error reporting.

-------------------------------------------------------------
 OUTPUT
	Pass or failure.

-------------------------------------------------------------
 LOCAL
	calibratorCounter - Count number of calibrators.
	controlCounter - Count number of controls.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
bool TestT4AnalyteTypes(CStringArray *analyteTypesList, 
						AnalyteType *current_analytes, 
						double minNumberOfCalibrators,
						VARIANT *error_msg)
{
	int		calibratorCounter, controlCounter;
	char	buf[512],buf1[512];
	double	y,n;

	current_analytes->GetCurrentTypes()->Lookup("CALIBRATOR", calibratorCounter);
	current_analytes->GetCurrentTypes()->Lookup("CONTROL", controlCounter);

	if (calibratorCounter < minNumberOfCalibrators)
	{
		y = modf(minNumberOfCalibrators, &n);

		sprintf(buf,"Number of calibrators %d is too low. Should be %.f.", calibratorCounter, n);
	}

	if (controlCounter > 0)
	{
		if (calibratorCounter < minNumberOfCalibrators)
		{
			strcat(buf,"\n");
			sprintf(buf1,"This data set has %d controls! No controls allowed in T4.", controlCounter);
			strcat(buf,buf1);
		}
		else
		{
			sprintf(buf,"This data set has %d controls! No controls allowed in T4.", controlCounter);
		}

		UserErrorMessage("3", buf, error_msg);
	}

	if ((calibratorCounter < minNumberOfCalibrators) || (controlCounter > 0))
	{
		return false;
	}
	return true;
}

/************************************************************
HRESULT Establish_T4_Top_Factor(
			LONG calibrationID, 
			CString partNumber, 
			VARIANT *error_msg)

	Implement ESTABLISH_T4_TOP_FACTOR business rule.

	Find top factors for Thyroxine, T4, chemistry.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	partNumber - Chemistry to find top factors.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	T4 top factors.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	assignedValues - List of assigned values to search for
					 to factors.
	buf[256] - Log error messages.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Establish_T4_Top_Factor(
			LONG calibrationID, 
			CString partNumber,
			double	topFactorMinValue,
			VARIANT *error_msg)
{
	HRESULT					hr;
	CArray<double,double>	assignedValues;
	char					buf[256];

	hr = GetCalibrationIDAssignedValues(calibrationID, &assignedValues, error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Establish_T4_Top_Factor\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr, 
			"Return from \"GetCalibrationIDAssignedValues\" function",
			buf);
		return hr;
	}

	if (TopFactor(&assignedValues,topFactorMinValue))
	{
		return S_OK;
	}
	else
	{
		double y,n;
		y = modf(topFactorMinValue, &n);

		sprintf(buf,"This T4 data set must have at least two top factors greater than %.f!", n);
		UserErrorMessage("3", buf, error_msg);

		return 0x80040E21;
	}
}
