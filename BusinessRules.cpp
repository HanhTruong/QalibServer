/*******************************************************************

FILENAME:	BusinessRules.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for business rules processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:

		IMPLEMENTED
		-----------
	Number_Of_Controls
	DeIonizedWater
	Max_Instrument_Sample_Runs
	Number_Of_Calibrators
	Min_Number_Instruments
	Min_Number_Of_Disks_Per_Sample
	Saline(LONG calibrationID

		NOT IMPLEMENTED
		---------------
	Number_Threshold_Samples
	Rotor_Factors
	Sample_Blank
	Threshold_Value
	Wavelengths

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BusinessRules.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <math.h>
#include "Support.h"
#include "Loging.h"
#include "AnalyteTypeClass.h"
#include "Utils.h"
#include "DbUtils.h"
#include "BusinessRulesSupport.h"

HRESULT Template_80_20()
{
	return S_OK;
}

/************************************************************
HRESULT Number_Of_Controls(LONG calibrationID,
						   CString chemistry,
						   VARIANT *error_msg)

	Implement NUMBER_OF_CONTROLS business rule.
	Number of controls required for each chemistry.
	
-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	chemistry - Current chemistry.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	buf[512] - Log error messages.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Number_Of_Controls(LONG calibrationID,
						   CString chemistry,
						   CString calibrationSpecies,
						   double minNumberOfControls,
						   VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	char	buf[1024];
	CString l_controlList,l_requiredSamplesList;

	CStringArray controlList;
	CStringArray requiredSamplesList;

	// < ----- GET LIST OF CONTROLS IN CURRENT CALIBRATION ----- >
	hr = BuildListOfSamplesToPlot(calibrationID,"CONTROL",&controlList);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Error: Number of controls in chemistry \"%s\".", chemistry);
			UserErrorMessage("3", buf, error_msg);
			sprintf(buf,"Function \"Number_Of_Controls\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"BuildListOfSamplesToPlot\" function",buf);
		}
		return hr;
	}
	// </ ----- GET LIST OF CONTROLS IN CURRENT CALIBRATION ----- >

	// < ----- TEST NUMBER OF CONTROLS ----- >
	if (controlList.GetSize() < minNumberOfControls)
	{
		hr = 0x80040E21;
		
		sprintf(buf,"Chemistry \"%s\" has %d controls. %d are required.",chemistry,controlList.GetSize(),int(minNumberOfControls));
		UserErrorMessage("3", buf, error_msg);
		return hr;
	}
	// < ----- TEST NUMBER OF CONTROLS ----- >

	// < ----- GET LIST OF REQUIRED CONTROLS FOR CURRENT CHEMISTRY ----- >
	hr = BuildListOfRequiredSamples(calibrationID,
								    chemistry,
									"CONTROL",
									calibrationSpecies,
								    &requiredSamplesList,
								    error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Error: Number of controls in chemistry \"%s\".", chemistry);
			UserErrorMessage("3", buf, error_msg);
			sprintf(buf,"Function \"Number_Of_Controls\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"BuildListOfRequiredSamples\" function",buf);
		}
		return hr;
	}
	// </ ----- GET LIST OF REQUIRED CONTROLS FOR CURRENT CHEMISTRY ----- >

	// < ----- CONVERT REQUIREMENT LISTs FOR ERROR REPORTING ----- >
	hr = FromCStringArrayToCString(&controlList, &l_controlList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Number_Of_Controls\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	
	hr = FromCStringArrayToCString(&requiredSamplesList,&l_requiredSamplesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Number_Of_Controls\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// < ----- CONVERT REQUIREMENT LISTs FOR ERROR REPORTING ----- >

	// < ----- DOES CURRENT CHEMISTRY HAVE THE REQUIRED SAMPLES? ----- >
	if (!Subset(&requiredSamplesList,&controlList))
	{
		hr = 0x80040E21;

		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Chemistry \"%s\" contains \"%s\" controls. These \"%s\"are required.",chemistry,l_controlList,l_requiredSamplesList);
			UserErrorMessage("3", buf, error_msg);
		}
		return hr;
	}
	// </ ----- DOES CURRENT CHEMISTRY HAVE THE REQUIRED SAMPLES? ----- >

	return hr;
}

/************************************************************
HRESULT Saline(LONG calibrationID, 
			   CString chemistry, 
			   VARIANT *error_msg)

	Implement DIWATER business rule.
	Verify that current calibration includes a DIWATER sample.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	chemistry - Current calibration chemistry.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that current calibration run with a DIWATER 
	sample.

-------------------------------------------------------------
 LOCAL
	buf - Error message.

-------------------------------------------------------------
 RETURN
	Status code.

*************************************************************/
HRESULT DeIonizedWater(LONG calibrationID,
					   CString chemistry,
					   VARIANT *error_msg)
{
	char	buf[512];

	if (FindSampleInCalibration(calibrationID,"DIWATER",error_msg))
	{
		return S_OK;	
	}
	else
	{
		sprintf(buf, "Chemistry \"%s\" requires a \"Deionized water\" pool.", (LPCTSTR)chemistry);
		UserErrorMessage("3", buf, error_msg);

		return 0x80040E21;
	}
}

HRESULT Diluent_Factors()
{
	return S_OK;
}

HRESULT Level_Limits()
{
	return S_OK;
}

/************************************************************
HRESULT Max_Instrument_Sample_Runs(
			LONG calibrationID,
			int maxSampleRuns, 
			VARIANT *error_msg)

	Implement MAX_INSTRUMENT_SAMPLE_RUNS business rule.
	Maximum number of times a sample can be run in the same instrument.

	Verify the number of times each sample ran in a single instrument.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	vSerialNumberList - List of instrument serial numbers.
	maxSampleRuns - Number of times a sample should ran in a
					single instrument.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that samples run in the minimum number of
	instruments.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	buf[512] - Log error messages.
	sampleSerialNumberList - Local array for list of instrument serial numbers.
	serialNumber - Current serial number being tested.
	timesFound - Number of times serial number was found in list.
	sampleIdList - List of sample ids.
	sampleSizesList - List of sample sizes.
	l_sampleIdsList - Local list of sample ids.
	sampleInstrumentList - List of instruments used in current sample.
	j - Index to traverse serial number list.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Max_Instrument_Sample_Runs(
			LONG calibrationID,
			double maxSampleRuns, 
			VARIANT *error_msg)
{
	HRESULT			hr;
	CStringArray	sampleSerialNumberList;
	CString			serialNumber;
	int				timesFound;
	char			buf[512];
	CArray<int,int> sampleSizesList;
	CStringArray	l_sampleIdsList;
	CStringArray	sampleInstrumentList;
	int				j;

	hr = GetCalibrationIDSamples(calibrationID, &l_sampleIdsList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Max_Instrument_Sample_Runs\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}

	j = 0;

	for (int i=0; i<l_sampleIdsList.GetSize(); i++)
	{
		sampleInstrumentList.SetSize(0);

		hr = GetSampleInstruments(calibrationID,l_sampleIdsList.GetAt(i),
								  &sampleInstrumentList,error_msg);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"Max_Instrument_Sample_Runs\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"GetSampleInstruments\" function",buf);
			return hr;
		}

		// Verify that each sample ran in the correct number of instruments.
		while (sampleInstrumentList.GetSize() > 0)
		{
			serialNumber = sampleInstrumentList.GetAt(0);

			timesFound = TimesElementAppearsInList(&sampleInstrumentList, serialNumber);

			if (timesFound > maxSampleRuns)
			{
				double y,n;
				y = modf(maxSampleRuns, &n);

				sprintf(buf,"Sample \"%s\" was run \"%d\" times in instrument \"%s\".\nMaximum \"%.f\" allowed!", (LPCTSTR)l_sampleIdsList.GetAt(i),timesFound, (LPCTSTR)serialNumber, n);
				UserErrorMessage("3", buf, error_msg);

				return 0x80040E21;
			}
		}
	}

	return S_OK;
}

/************************************************************
HRESULT Number_Of_Calibrators(LONG calibrationID,
							  CString chemistry,
							  double minNumberOfCalibrators,
							  VARIANT *error_msg)

	Implement NUMBER_OF_CALIBRATORS business rule.
	Minimum number of analytes, samples, required to carry out a calibration.

	Verify the number of analytes in a calibration are within set limits.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	chemistry - Chemistry in current calibration.
	minNumberOfCalibrators - Set limit for number of analytes.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that current calibration processed within 
	requirements.

-------------------------------------------------------------
 LOCAL
	hr					  - Return code.
	buf[1024]			  - Log error messages.
	l_calibratorList	  - Local list of sample calibrators.
							CString format. Error reporting.
	l_requiredSamplesList -	Local list of required sample calibrators.
							CString format. Error reporting.
	calibratorList		  - List of calibrators in current calibration.
	requiredSamplesList	  - List of calibrators required for
						    current chemistry.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Number_Of_Calibrators(LONG calibrationID,
							  CString chemistry,
							  CString calibrationSpecies,
							  double minNumberOfCalibrators,
							  VARIANT *error_msg)
{
	HRESULT hr = S_OK;
	char	buf[1024];
	CString l_calibratorList,l_requiredSamplesList;

	CStringArray calibratorList;
	CStringArray requiredSamplesList;

	// < ----- GET LIST OF CALIBRATORS IN CURRENT CALIBRATION ----- >
	hr = BuildListOfSamplesToPlot(calibrationID,"CALIBRATOR",&calibratorList);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Error: Number of calibrators in chemistry \"%s\".", chemistry);
			UserErrorMessage("3", buf, error_msg);
			sprintf(buf,"Function \"Number_Of_Calibrators\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"BuildListOfSamplesToPlot\" function",buf);
		}
		return hr;
	}
	// </ ----- GET LIST OF CALIBRATORS IN CURRENT CALIBRATION ----- >

	// < ----- TEST NUMBER OF CALIBRATORS ----- >
	if (calibratorList.GetSize() < minNumberOfCalibrators)
	{
		hr = 0x80040E21;
		
		sprintf(buf,"Chemistry \"%s\" has %d calibrators. %d are required.",chemistry,calibratorList.GetSize(),int(minNumberOfCalibrators));
		UserErrorMessage("3", buf, error_msg);
		return hr;
	}
	// < ----- TEST NUMBER OF CALIBRATORS ----- >

	// < ----- GET LIST OF REQUIRED CALIBRATORS FOR CURRENT CHEMISTRY ----- >
	hr = BuildListOfRequiredSamples(calibrationID,
								    chemistry,
									"CALIBRATOR",
									calibrationSpecies,
								    &requiredSamplesList,
								    error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Error: Number of calibrators in chemistry \"%s\".", chemistry);
			UserErrorMessage("3", buf, error_msg);
			sprintf(buf,"Function \"Number_Of_Calibrators\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"BuildListOfRequiredSamples\" function",buf);
		}
		return hr;
	}
	// </ ----- GET LIST OF REQUIRED CALIBRATORS FOR CURRENT CHEMISTRY ----- >

	// < ----- CONVERT REQUIREMENT LISTs FOR ERROR REPORTING ----- >
	hr = FromCStringArrayToCString(&calibratorList, &l_calibratorList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Number_Of_Calibrators\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	
	hr = FromCStringArrayToCString(&requiredSamplesList,&l_requiredSamplesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Number_Of_Calibrators\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"FromCStringArrayToCString\" function",buf);
		return hr;
	}
	// < ----- CONVERT REQUIREMENT LISTs FOR ERROR REPORTING ----- >

	// < ----- DOES CURRENT CHEMISTRY HAVE THE REQUIRED SAMPLES? ----- >
	if (!Subset(&requiredSamplesList,&calibratorList))
	{
		hr = 0x80040E21;

		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Chemistry \"%s\" contains \"%s\" calibrators. These \"%s\"are required.",chemistry,l_calibratorList,l_requiredSamplesList);
			UserErrorMessage("3", buf, error_msg);
		}
		return hr;
	}
	// </ ----- DOES CURRENT CHEMISTRY HAVE THE REQUIRED SAMPLES? ----- >

	return hr;
}

/************************************************************
HRESULT Min_Number_Instruments(int minNumberOfInstruments, 
							   VARIANT *vSerialNumberList,
							   VARIANT *error_msg)

	Implement MIN_NUMBER_INSTRUMENTS business rule.
	Minimum number of instruments for each calibration.
	Find number of unique instruments, serial numbers.

-------------------------------------------------------------
 INPUT
	minNumberOfInstruments - Minimum number of instruments allowed.
	vSerialNumberList - List of instrument serial numbers.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that samples run in the minimum number of
	instruments.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	buf[512] - Log error messages.

	uniqueValues - Number of unique serial numbers found.
	serialNumberList - Local list of serial numbers.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Min_Number_Instruments(double minNumberOfInstruments, 
							   VARIANT *vSerialNumberList,
							   VARIANT *error_msg)
{
	HRESULT	hr;
	int uniqueValues;
	CStringArray serialNumberList;
	char	buf[512];

	hr = FromVARIANTToCStringArray(vSerialNumberList, &serialNumberList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Min_Number_Instruments\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"FromVARIANTToCStringArray\" function",buf);
		return hr;
	}

	uniqueValues = NumberOfUniqueValues(&serialNumberList);
	if (uniqueValues < minNumberOfInstruments)
	{
		double y,n;
		y = modf(minNumberOfInstruments, &n);

		sprintf(buf, "%d instruments were used. Minimum allowed is %.f.", uniqueValues, n);
		UserErrorMessage("3", buf, error_msg);

		return 0x80040E21;
	}

	return S_OK;
}

/************************************************************
HRESULT Min_Number_Of_Disks_Per_Sample(
			LONG calibrationID, 
			int numberOfDisks, 
			VARIANT *error_msg)

	Implement MIN_NUMBER_OF_DISKS_PER_SAMPLE business rule.

	Verify that each sample ran in a certain number of disks.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	numberOfDisks - Minimun number of disks that each 
					sample must be run.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that samples run in the minimum number of
	disks.
-------------------------------------------------------------
 LOCAL
	hr - Return code.
	buf[256] - Log error messages.
	sampleSizesList - List of sample sizes.
	sizeAllowed - Is the current size allowed.
	i - Index to traverse sample sizes list.
	sampleIdList - List of sample ids in string format.
	l_sampleIdList - List of sample ids in array of ints.
	buf[512] - Error message buffer.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Min_Number_Of_Disks_Per_Sample(
			LONG calibrationID, 
			double numberOfDisks, 
			VARIANT *error_msg)
{
	HRESULT			hr = S_OK;
	CArray<int,int> sampleSizesList;
	bool			sizeAllowed;
	int				i;
	CStringArray	sampleIdList;
	char			buf[512];

	// < ----- GET SAMPLE NAMES ----- >
	hr = GetCalibrationIDSamples(calibrationID, &sampleIdList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Min_Number_Of_Disks_Per_Sample\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ ----- GET SAMPLE NAMES ----- >

	// < ----- GET SAMPLE SIZES ----- >
	hr = GetCalibrationIDSampleSizes(calibrationID,"EXCLUDED",&sampleSizesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Min_Number_Of_Disks_Per_Sample\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSampleSizes\" function",buf);
		return hr;
	}
	// </ ----- GET SAMPLE SIZES ----- >
	
	// < ----- TEST SAMPLE SIZES ----- >
	i = 0;
	sizeAllowed = true;
	while ((i<sampleSizesList.GetSize()) && sizeAllowed )
	{
		if (sampleSizesList.GetAt(i) < numberOfDisks)
		{
			double y,n;
			y = modf(numberOfDisks, &n);

			sprintf(buf, "Sample \"%s\" was run in %d disks.\nMinimum allowed is %.f.", (LPCTSTR)sampleIdList.GetAt(i), sampleSizesList.GetAt(i), n);
			UserErrorMessage("3", buf, error_msg);

			sizeAllowed = false;

			hr = 0x80040E21;
		}
		i++;
	}
	// </ ----- TEST SAMPLE SIZES ----- >

	return hr;
}

HRESULT Mold_Factors()
{
	return S_OK;
}

HRESULT Number_Threshold_Samples()
{
	return S_OK;
}

HRESULT Rotor_Factors()
{
	return S_OK;
}

/************************************************************
HRESULT Saline(LONG calibrationID, 
			   CString chemistry, 
			   VARIANT *error_msg)

	Implement SALINE business rule.
	Verify that current calibration includes a SALINE sample.

-------------------------------------------------------------
 INPUT
	calibrationID - Id of calibration to find top factors.
	chemistry - Current calibration chemistry.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Verification that current calibration run with a SALINE 
	sample.

-------------------------------------------------------------
 LOCAL
	NONE.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT Saline(LONG calibrationID, 
			   CString chemistry, 
			   VARIANT *error_msg)
{
	char			buf[512];

	if (FindSampleInCalibration(calibrationID,"SALINE",error_msg))
	{
		return S_OK;	
	}
	else
	{
		sprintf(buf, "Chemistry \"%s\" requires a \"Saline\" pool.", (LPCTSTR)chemistry);
		UserErrorMessage("3", buf, error_msg);

		return 0x80040E21;
	}
}

HRESULT Sample_Blank()
{
	return S_OK;
}

HRESULT Threshold_Value()
{
	return S_OK;
}

HRESULT Wavelengths()
{
	return S_OK;
}

