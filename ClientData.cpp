/*******************************************************************
FILENAME:	ClientData.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions to process data from the client, calibration data.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	ProcessClientData
	ExtractClientData
	ConvertClientData
	FindNumberOfDataSets
	UpdateCalibrationData
	SetupDataForOptivecFunctions
	ExtractSets

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	ClientData.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include <iostream.h>
#include "ClientData.h"
#include "Species.h"
#include "DbUtils.h"
#include "Loging.h"
#include "Support.h"
#include "HelperFunctions.h"
#include "globals.h"
#include "Utils.h"
#include "CalibrationCalculations.h"
#include "dboGetSampleValues.H"
#include "DbAccess.h"
#include "Outliers.h"
#include "BusinessRulesEnforce.h"

/*****************************************************************
HRESULT ProcessClientData(BSTR		chemistry,
						  INT		outlierMethod,
						  BSTR		mode,
						  BSTR		user,
						  CString	*calibrationSpecies,
						  VARIANT	*v_inputData,
						  VARIANT	*v_serialNumber, 
						  VARIANT	*v_barCode,
						  VARIANT	*v_sampleSpecies,
						  VARIANT	*v_statusData,
						  LONG		*calibrationID,
						  VARIANT	*error_msg)

INPUT
	chemistry - Current chemistry being calibrated.
	outlierMethod - Method to calculate outliers.
	mode - Calibration mode.
	user - Name of operator.
	calibrationSpecies - Calibration species.
	v_inputData - Calibration data.
	v_serialNumber - Analyzer serial numbers.
	v_barCode - Bar codes.
	v_sampleSpecies - Species of each sample.
	v_statusData - Status of data.
	calibrationID - Calibration identifier.
	error_msg - Error message.

OUTPUT
	Calibration data in the database.

LOCAL
	hr - Function status code.
	buf - Message build.
	inputData - Calibration data, in string format.
	serialNumbers - Serial numbers in string format.
	barCode - Bar code in string format.
	sampleSpecies - Sample species in string format.
	statusData - Data status in int format.
	sampleSizes - Sample sizes in int format.
	sampleIds - Sample ids in string array format.
	setAttributes - Attributes of data sets.

RETURN
	Function status code.

*****************************************************************/
HRESULT ProcessClientData(BSTR		chemistry,
						  INT		outlierMethod,
						  BSTR		mode,
						  BSTR		user,
						  CString	*calibrationSpecies,
						  VARIANT	*v_inputData,
						  VARIANT	*v_serialNumber, 
						  VARIANT	*v_barCode,
						  VARIANT	*v_sampleSpecies,
						  VARIANT	*v_statusData,
						  LONG		*calibrationID,
						  VARIANT	*error_msg)
{
	HRESULT			hr;
	char			buf[512];
	CStringArray	inputData;
	CStringArray	serialNumbers;
	CStringArray	barCode;
	CStringArray	sampleSpecies;
	CArray<int,int>	statusData;
	CArray<int,int>	sampleSizes;
	CStringArray	sampleIds;
	CStringArray	setAttributes;

	// Convert client data from COM, VARIANT, to MFC structures.
	hr = ConvertClientData(v_inputData,v_serialNumber,
						   v_barCode,v_sampleSpecies,
						   v_statusData,&inputData,
						   &serialNumbers,&barCode,
						   &sampleSpecies,&statusData);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ProcessClientData\", file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"ConvertClientData\"" << endl;
		return hr;
	}

	// Extract data using metadata.
	hr = ExtractClientData(&inputData,&sampleSizes,&sampleIds,&setAttributes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ProcessClientData\", file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"ExtractClientData\"" << endl;
		return hr;
	}

	// < ----- FIND CALIBRATION SPECIES ----- >
	hr = FindCalibrationSpecies(chemistry,
								&sampleIds,
								&sampleSizes,
								&sampleSpecies,
								calibrationSpecies,
								error_msg);
	if (FAILED(hr))
	{
		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"SendClientDataToDb\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from function \"FindCalibrationSpecies\"",buf);
		}

		return hr;
	}
	// </ ----- FIND CALIBRATION SPECIES ----- >

	// < ----- SEND CLIENT DATA TO THE DATABASE ----- >
	hr = SendClientDataToDb(chemistry,
							outlierMethod,
							mode,
							user,
							*calibrationSpecies,
							&inputData,
							&setAttributes,
							&serialNumbers,
							&barCode,
							&sampleSpecies,
							&statusData,
							&sampleSizes,
							&sampleIds,
							calibrationID,
							error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ProcessClientData\", file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"SendClientDataToDb\"" << endl;
		return hr;
	}
	// </ ----- SEND CLIENT DATA TO THE DATABASE ----- >

	return S_OK;
}

/*****************************************************************
HRESULT ExtractClientData(CStringArray		*inputData,
						  CArray<int,int>	*sampleSizes,
						  CStringArray		*sampleIds,
						  CStringArray		*setAttributes)

INPUT
	inputData - Calibration data.
	sampleSizes - Sample sizes.
	sampleIds - Sample ids.
	setAttributes - Set attributes.

OUTPUT
	sampleSizes - Sample sizes extracted from inputData.
	sampleIds - Sample ids extracted from inputData.
	setAttributes - Set attributes extracted from inputData.

LOCAL
	hr - Function status.
	numberOfSets - Number of sets of data.
	numberOfSamples - Number of samples in calibration.

RETURN
	Function status.

*****************************************************************/
HRESULT ExtractClientData(CStringArray		*inputData,
						  CArray<int,int>	*sampleSizes,
						  CStringArray		*sampleIds,
						  CStringArray		*setAttributes)
{
	HRESULT hr=S_OK;
	int		numberOfSets;
	int		numberOfSamples;

	// Get number of sets.
	numberOfSets = atoi(inputData->GetAt(0));

	// Remove number of sets from input data.
	inputData->RemoveAt(0);

	// Extract set attributes
	for (int i=0; i<numberOfSets; i++)
	{
		setAttributes->Add(inputData->GetAt(0));
		inputData->RemoveAt(0);
	}

	// Extract number of samples from input data.
	numberOfSamples = atoi(inputData->GetAt(0));

	// Remove number of samples from input data.
	inputData->RemoveAt(0);

	for (i=0; i<numberOfSamples; i++)
	{
		sampleSizes->Add(atoi(inputData->GetAt(0)));
		inputData->RemoveAt(0);
		sampleIds->Add(inputData->GetAt(0));
		inputData->RemoveAt(0);
	}

	return hr;
}

/*****************************************************************
HRESULT ConvertClientData(VARIANT			*v_clientData,
						  VARIANT			*v_serialNumber, 
						  VARIANT			*v_barCode,
						  VARIANT			*v_sampleSpecies,
						  VARIANT			*v_statusData,
  						  CStringArray		*c_clientData,
						  CStringArray		*c_serialNumbers,
						  CStringArray		*c_barCode,
						  CStringArray		*c_sampleSpecies,
						  CArray<int,int>	*c_statusData)

	Convert calibration data from VARIANT format to MFC format.

INPUT
	inputData - Calibration data.
	sampleSizes - Sample sizes.
	sampleIds - Sample ids.
	setAttributes - Set attributes.

	v_clientData - Calibration data in VARIANT format.
	v_serialNumber - Serial number data in VARIANT format.
	v_barCode - Bar code in VARIANT format.
	v_sampleSpecies - Sample species data in VARIANT format.
	v_statusData - Data status in VARIANT format.
  	c_clientData  - Calibration data in MFC format.
	c_serialNumbers - Calibration data in MFC format.
	c_barCode - Calibration data in MFC format.
	c_sampleSpecies - Calibration data in MFC format.
	c_statusData - Calibration data in MFC format.

OUTPUT
  	c_clientData  - Calibration data in MFC format.
	c_serialNumbers - Calibration data in MFC format.
	c_barCode - Calibration data in MFC format.
	c_sampleSpecies - Calibration data in MFC format.
	c_statusData - Calibration data in MFC format.

LOCAL
	hr - Function status.
	buf - Error messages.

RETURN
	Function status.

*****************************************************************/
HRESULT ConvertClientData(VARIANT			*v_clientData,
						  VARIANT			*v_serialNumber, 
						  VARIANT			*v_barCode,
						  VARIANT			*v_sampleSpecies,
						  VARIANT			*v_statusData,
  						  CStringArray		*c_clientData,
						  CStringArray		*c_serialNumbers,
						  CStringArray		*c_barCode,
						  CStringArray		*c_sampleSpecies,
						  CArray<int,int>	*c_statusData)
{
	HRESULT hr;
	char	buf[512];

	// Convert client data from VARIANT to CStringArray.
 	hr = FromVARIANTToCStringArray(v_clientData,c_clientData);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ConvertClientData\", calibration data. file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"FromVARIANTToCStringArray\"" << endl;
		return hr;
	}

	// Convert serial number list from VARIANT to CStringArray.
 	hr = FromVARIANTToCStringArray(v_serialNumber,c_serialNumbers);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ConvertClientData\", serial numbers. file %s, line %d", __FILE__, __LINE__);
		cout << "Return from function \"FromVARIANTToCStringArray\"" << endl;
		return hr;
	}

	// Convert barcode list from VARIANT to CStringArray.
 	hr = FromVARIANTToCStringArray(v_barCode,c_barCode);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ConvertClientData\", barcode. file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"FromVARIANTToCStringArray\"" << endl;
		return hr;
	}

	// Convert species list from VARIANT to CStringArray.
 	hr = FromVARIANTToCStringArray(v_sampleSpecies,c_sampleSpecies);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ConvertClientData\", species. file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"FromVARIANTToCStringArray\"" << endl;
		return hr;
	}

	// Convert data status list from VARIANT to CArray.
 	hr = FromVARIANTToCArray(v_statusData,c_statusData);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ConvertClientData\", status. file %s, line %d", __FILE__, __LINE__);
		cout << buf << "Return from function \"FromVARIANTToCArray\"" << endl;
		return hr;
	}

	return S_OK;
}


/*****************************************************************
int FindNumberOfDataSets(CStringArray		*inputData,
						 CArray<int,int>	*sampleSizes)

	Find the number of data sets in current calibration.

INPUT
	inputData - Calibration data.
	sampleSizes - Sample sizes.

OUTPUT
	number of data sets.

LOCAL
	lengthOfCurrentDataSet - Size of current data set.
	lengthOfInputData - Total number of data values.
	numberOfDataSets - Number of data sets.

RETURN
	number of data sets.

*****************************************************************/
int FindNumberOfDataSets(CStringArray		*inputData,
						 CArray<int,int>	*sampleSizes)
{
	int lengthOfCurrentDataSet=0;
	int lengthOfInputData=0;
	int numberOfDataSets=0;

	// Calculate number of data values.
	for (int i=0; i<sampleSizes->GetSize(); i++)
	{
		lengthOfCurrentDataSet += sampleSizes->GetAt(i);
	}

	// Length of input data.
	lengthOfInputData = inputData->GetSize();

	if (lengthOfInputData >= lengthOfCurrentDataSet)
	{
		// Data must contain an integral number of data sets.
		if ((lengthOfInputData % lengthOfCurrentDataSet) == 0) 
		{
			numberOfDataSets = lengthOfInputData/lengthOfCurrentDataSet;
		}
	}

	return numberOfDataSets;
}

/********************************************************
HRESULT UpdateCalibrationData(LONG				calibrationID,
							  CPtrArray			*datasets,
							  CPtrArray			*statussets,
							  CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  VARIANT			*error_msg)

Gather all the data that is required to run a calibration.

Update calibration data, to run calibration calculations
for updated fit parameters.

INPUT
	calibrationID - Identifier of calibration to be updated.
	datasets - Data sets for current calibration.
	statussets - Status of data values.
	sampleIds - Sample ids for current calibration.
	sampleSizes - Sizes of current samples.
	error_msg - Error reporting.

OUTPUT
	Calibration data structures updated.

RETURN
	Status code, hr.

********************************************************/
HRESULT UpdateCalibrationData(LONG				calibrationID,
							  CPtrArray			*datasets,
							  CPtrArray			*statussets,
							  CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  VARIANT			*error_msg)
{
	HRESULT					hr = S_OK;
	CArray<int,int>			calibrationValuesStatus;
	CArray<double,double>	assignedValues;
	CArray<double,double>	sampleValues;
	char					buf[7999];

//CString temp="++++ BEGIN UpdateCalibrationData ++++";
//DisplayCString(&temp);

	// < ----- GET LIST OF SAMPLES THAT PASS ALL TESTS FOR CALIBRATION ----- >
	hr = ListOfSamplesToUseInCalibration(calibrationID,sampleIds,error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"ListOfSamplesToUseInCalibration\"","UpdateCalibrationData");
		return hr;
	}
	// < ----- GET LIST OF SAMPLES THAT PASS ALL TESTS FOR CALIBRATION ----- >

//DisplayCStringArray(sampleIds,"sampleIds AFTER ListOfSamplesToUseInCalibration");

	// Get data and status for current list of samples, localIndexSampleIDs.
	hr = GetDataAndStatusForAListOfSamples(calibrationID,
										   "EXCLUDED",
										   sampleIds,
										   &sampleValues,
										   &calibrationValuesStatus,
										   error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"UpdateCalibrationData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetDataAndStatusForAListOfSamples\" function",buf);
		return hr;
	}

//DisplayCArray(&sampleValues,"Data AFTER GetDataAndStatusForAListOfSamples");

	// </ ----- SAMPLE SIZES ----- >
	// Get size of a list of samples, include outliers.
	// Third parameter, "VALID", do not include EXCLUDED,"MOVED and EXCLUDED" and OUTLIER data.
	hr = GetSizesOfAListOfSamples(calibrationID,sampleIds,"VALID",sampleSizes);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"UpdateCalibrationData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"GetSizesOfAListOfSamples\"",buf);
		return hr;
	}
	// </ ----- SAMPLE SIZES ----- >

//DisplayCArray(sampleSizes,"sampleSizes AFTER GetSizesOfAListOfSamples");

	// Extract data sets.
	hr = ExtractSets(datasets,
					 statussets,
					 sampleIds,
					 sampleSizes,
					 &calibrationValuesStatus,
					 &sampleValues,
					 error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"UpdateCalibrationData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"ExtractSets\" function",buf);
		return hr;
	}

//DisplayDataSets(datasets,"Data sets AFTER ExtractSets");
//DisplayStatusSets(statussets,"Status sets AFTER ExtractSets");

	// All sample ids in list, localSampleIDs, will be included in calculations.
	// Remove values that are outliers. 
	// Number of outliers are not enough to eliminate any sample.
	hr = RemoveOutlierValues(datasets,
							 statussets,
							 sampleSizes,
							 error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"UpdateCalibrationData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"RemoveOutlierValues\" function",buf);
		return hr;
	}

//DisplayDataSets(datasets,"Data sets AFTER RemoveOutlierValues");
//DisplayStatusSets(statussets,"Status sets AFTER RemoveOutlierValues");
//DisplayCArray(sampleSizes,"sampleSizes AFTER RemoveOutlierValues");

//temp="++++ END UpdateCalibrationData ++++";
//DisplayCString(&temp);

	return hr;
}

/********************************************************
HRESULT SetupDataForOptivecFunctions(CArray<double,double> *dataSet,
									 CStringArray *sampleIds,
									 CArray<int,int> *sampleSizes,
									 CArray<double,double> *assignedValues,
									 VARIANT *error_msgs)

	Set up data for OptiVec functions.


INPUT
	dataSet - Data set for current calibration.
	sampleIds - Sample ids for current calibration.
	sampleSizes - Sizes of current samples.
	assignedValues - Assigned values for calibration.
	error_msg - Error reporting.

OUTPUT
	Data structures for OptiVec functions.

RETURN
	Status code, hr.

********************************************************/
HRESULT SetupDataForOptivecFunctions(CArray<double,double> *dataSet,
									 CStringArray *sampleIds,
									 CArray<int,int> *sampleSizes,
									 CArray<double,double> *assignedValues,
									 VARIANT *error_msgs)
{
	HRESULT	hr=S_OK;

	// ------------------------------------------------------
	// ***** SETUP DATA FOR OPTIVEC FUNCTIONS *****
	// ------------------------------------------------------
	// Set global "numberofsamples", 
	// from the bound of the current list of samples ids.
	numberofsamples = sampleIds->GetSize();

	// Allocate memory for vector containing sizes of each sample.
	PoolSizes = VLI_vector( numberofsamples );

	// < ----- SAMPLE LENGTHS in PoolSizes vector ----- >
	CopyCArrayToVector(sampleSizes, PoolSizes);
	// </ ----- SAMPLE LENGTHS in PoolSizes vector ----- >

	// Total number of data points, for all samples.
	sizex = VLI_sum(PoolSizes, numberofsamples);

	// Needed for Weights and XExp, rates, array.
	Weights = VD_vector( sizex );
	// Allocate memory for assigned values.
	XExp = VD_vector( sizex );
	// Allocate memory for rates, ie. analyzer data.
	YExp = VD_vector( sizex );

	// < ----- ASSIGNED VALUES in XExp vector ----- >
	ExpandAssignedValuesIntoVector(assignedValues,XExp,PoolSizes);
	// </ ----- ASSIGNED VALUES in XExp vector ----- >

	// < ----- SAMPLE VALUES in YExp vector ----- >
	CopyCArrayToVector(dataSet, YExp);
	// </ ----- SAMPLE VALUES in YExp vector ----- >
	// ------------------------------------------------------
	// ***** SETUP DATA FOR OPTIVEC FUNCTIONS *****
	// ------------------------------------------------------

	return hr;
}

/********************************************************
HRESULT ExtractSets(CPtrArray *datasets,
					CPtrArray *statussets,
					CStringArray *sampleids,
					CArray<int,int> *samplesizes,
					CArray<int,int> *status,
					CArray<double,double> *values,
					VARIANT *error_msgs)

	Extract data sets from input/client/calibration data.
	Calibration data comes in as a flat array.

INPUT
	datasets - Extract data sets into this structure.
	statussets - Extract data value status into this structure.
	sampleids - Sample ids for current calibration.
	samplesizes - Sizes of current samples.
	status - Status set of current data set.
	values - Calibration data.
	error_msg - Error reporting.

OUTPUT
	Data and status structures.

RETURN
	Status code, hr.

********************************************************/
HRESULT ExtractSets(CPtrArray *datasets,
					CPtrArray *statussets,
					CStringArray *sampleids,
					CArray<int,int> *samplesizes,
					CArray<int,int> *status,
					CArray<double,double> *values,
					VARIANT *error_msgs)
{
	HRESULT					hr=S_OK;
	CArray<double,double>	*ptr_ldataset;
	CArray<int,int>			*ptr_lstatusset;
	int						number_of_values;
	int						sum_of_samplesizes;
	int						t_sample_sizes;
	int						values_index;
	int						set_index;
	char					buf[512];

	// Find length of data sets.
	sum_of_samplesizes = 0;
	for (int i=0; i<samplesizes->GetSize(); i++)
	{
		sum_of_samplesizes = sum_of_samplesizes + samplesizes->GetAt(i);
	}

	number_of_values = values->GetSize();

	// Length of data sets is an integer factor
	// total number of data values.
	if ((number_of_values % sum_of_samplesizes) == 0)
	{
		t_sample_sizes = sum_of_samplesizes;

		values_index = 0;
		set_index = 0;

		while (values_index < number_of_values)
		{
			ptr_ldataset = new CArray<double,double>;
			ptr_lstatusset = new CArray<int,int>;

			for (i=set_index; i<t_sample_sizes; i++)
			{
				ptr_ldataset->Add(values->GetAt(values_index));
				ptr_lstatusset->Add(status->GetAt(values_index));
				values_index++;
			}

			set_index = values_index;
			t_sample_sizes = t_sample_sizes + values_index;

			datasets->Add(ptr_ldataset);
			statussets->Add(ptr_lstatusset);
		}
	}
	else
	{
		hr = 0x80040E21;
		sprintf(buf,"Function \"ExtractSets\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Calibration data does not have an integral number of sets!",buf);
		return hr;
	}

	return hr;
}
