/*******************************************************************
FILENAME: Outliers.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions to support outlier calculations.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------

COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	Outliers.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <math.h>

#include "Status.h"
#include "StatusTest.h"

#include "Support.h"
#include "Outliers.h"
#include "Loging.h"
#include "HelperFunctions.h"
#include "globals.h"
#include "Utils.h"
#include "DbAccess.h"
#include "ClientData.h"
#include "BusinessRulesProcess.h"
#include "OutlierCalculations.h"

#include "dboGetCalibrationData.H"
#include "dboGetSampleNames.H"
#include "dboGetSampleSizeIncludeOutliers.H"
#include "dboUpdateDataStatus.H"
#include "dboGetRecoveredValuesFromRates.H"
#include "dboGetCalculatedAssignedValues.H"
#include "dboGetDataStatus.H"
#include "dboGetPartNumber.H"
#include "dboGetUpperOutlierLimits.H"
#include "dboGetLowerOutlierLimits.H"
#include "dboGetApplyOutlierLimits.H"
#include "dboGetCalibrationDataResults.H"

/*****************************************************************
HRESULT Outliers(LONG calibrationID,
				 CString mode,
				 CString algorithm,
				 INT outlierMethod,
				 VARIANT *error_msg)

- Process current chosen outlier method.

INPUT
	calibrationID - Identifier of current calibration.
	mode - Mode of calibration.
	algorithm - Algorithm used for current calibration.
	outlierMethod - Outlier method.
	error_msg - Error message to sent to user.

OUTPUT
	Choice of outlier method.

LOCAL
	hr - return code.

------------------------------------------------------------------
enum {NO_REMOVAL=1, 
	  MANUAL, 
	  HISTORIC, 
	  NO_MEAN_NO_STD, 
	  MEAN_NO_STD, 
	  NO_MEAN_STD, 
	  MEAN_STD};
*****************************************************************/
HRESULT Outliers(LONG calibrationID,
				 CString mode,
				 CString algorithm,
				 INT outlierMethod,
				 VARIANT *error_msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT					hr = S_OK;
	CArray<CString,CString> ApplyOutlier;
	CStringArray			sampleIds;
	CArray<double,double>	Upper,Lower;
	CArray<double,double>	calibrationData;
	CArray<long,long>		indexingList;
	CArray<int,int>			calibrationStatus;
	CArray<int,int>			sampleSizes;
	CPtrArray				datasets;
	CPtrArray				statussets;
	char					buf[512];

	sampleSizes.RemoveAll();

	// < ----- Get calibration data to test for outliers. ----- >
	hr = GetCalibrationOutlierData(calibrationID,
								   &indexingList,
								   &calibrationData,
								   &calibrationStatus,
								   &sampleSizes,
								   &sampleIds,
								   error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr, "Return from function \"GetCalibrationData\"","Outliers");
		return hr;
	}
	// </ ----- Get calibration data to test for outliers. ----- >

	// Extract data sets.
	hr = ExtractSets(&datasets,
					 &statussets,
					 &sampleIds,
					 &sampleSizes,
					 &calibrationStatus,
					 &calibrationData,
					 error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"Outliers\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"ExtractSets\" function",buf);
		return hr;
	}

	RemoveExcludedData(&datasets,&statussets,&indexingList,&sampleIds,&sampleSizes);

	hr = NumberExcluded(calibrationID,mode,&sampleIds,error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_EMPTY)
		{
			UserErrorMessage("3", "Too many samples excluded",error_msg);
		}

		return hr;
	}

	hr = OutlierLimits(calibrationID, 
					   &sampleIds, 
					   &Upper, 
					   &Lower, 
					   &ApplyOutlier, 
					   error_msg);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OutlierLimits\"","Outliers");
		return hr;
	}

	// ***************************************
	// Calculate outliers, using given method.
	// ***************************************

	switch (outlierMethod)
	{
	case NO_REMOVAL:		// 1 - Use data as is.
	break;
	case MANUAL:			// 2 - Outliers removed by operator.
	break;
	case HISTORIC:			// 3 - Use previous methods.

		hr = FindHistoricOutlier(&datasets,
								 &statussets,
								 &sampleSizes,
								 &sampleIds,
								 &Upper,
								 &Lower,
								 &ApplyOutlier,
								 error_msg);
		if (FAILED(hr))
		{
			LogErrorMessage(hr, "Return from function \"FindHistoricOutlier\"", "Outliers");
			return S_OK;
		}
	break;
	case NO_MEAN_NO_STD:	// 4 - Don't know either mean nor standard deviation.
	break;
	case MEAN_NO_STD:		// 5 - Know mean, don't know standard deviation.
		//???????????????????????????????????????????????????????????????????????
	break;
	case NO_MEAN_STD:		// 6 - Don't know mean, know standard deviation.
	break;
	case MEAN_STD:			// 7 - Know mean and standard deviation.
	break;
	default:
		{
			// 0x80040E21  -2147217887  Errors occurred
			LogErrorMessage(0x80040E21, "Unknown outlier method", "Outliers");
			return hr;
		}
	}

	// **********************
	// Update value's status.
	// **********************
	calibrationStatus.RemoveAll();
	hr = SetUnion(&statussets,&calibrationStatus);
	if (FAILED(hr))
	{
		LogErrorMessage(hr, "Return from function \"SetUnion\"", "Outliers");
		return hr;
	}

	hr = UpdateDataStatus(	calibrationID,
							&indexingList,
							&calibrationStatus);
	if (FAILED(hr))
	{
		LogErrorMessage(hr, "Return from function \"UpdateDataStatus\"", "Outliers");
		return hr;
	}

	return hr;
}

/********************************************************
HRESULT FindHistoricOutlier(CPtrArray *datasets,
							CPtrArray *statussets,
							CArray<int,int> *sampleSizes,
							CStringArray *sampleIds,
							CArray<double,double> *UpperOutlierLimits,
							CArray<double,double> *LowerOutlierLimits,
							CArray<CString,CString> *ApplyOutlier,
							VARIANT *error_msg)

- Sets values status, according to outlier calculations.

INPUT
	datasets - Calibration data sets.
	statussets - Status sets.
	sampleSizes - Number of values in each sample.
	sampleIds - Sample identifiers.
	UpperOutlierLimits - Upper limits on outliers.
	LowerOutlierLimits - Lower limits on outliers.
	ApplyOutlier - Type of outlier limits.
	error_msg - Error reporting.

OUTPUT
	Outlier status of each data value.

LOCAL
	hr - return code.
  	currentData - Vector for current list of values.
	listSize - Size of current list.
	i - Index for current data vector.
	mean - Mean of current data.
	seps[] - String separators for token search.
	*token - Current string.
	l_sampleSizes - Local sample sizes.
	l_calibrationStatus - Local value status.
	l_calibrationData - Local data values.

********************************************************/
HRESULT FindHistoricOutlier(CPtrArray *datasets,
							CPtrArray *statussets,
							CArray<int,int> *sampleSizes,
							CStringArray *sampleIds,
							CArray<double,double> *UpperOutlierLimits,
							CArray<double,double> *LowerOutlierLimits,
							CArray<CString,CString> *ApplyOutlier,
							VARIANT *error_msg)
{
	HRESULT					hr;
	int						dataIndex;
	int						statusIndex;
	CArray<int,int>			*ptrCalibrationStatus;
	CArray<double,double>	*ptrCalibrationData;

	for (int k=0; k<datasets->GetSize(); k++)
	{
		ptrCalibrationData = (CArray<double,double> *)datasets->GetAt(k);
		ptrCalibrationStatus = (CArray<int,int> *)statussets->GetAt(k);

		//
		// Find outliers in current data set.
		//
		dataIndex = 0;
		statusIndex = 0;

		for (int i=0; i<sampleIds->GetSize(); i++)
		{
			hr = CalculateSampleOutliers(statussets,
										 ptrCalibrationStatus,
										 ptrCalibrationData,
										 &statusIndex,
										 &dataIndex,
										 sampleSizes->GetAt(i),
										 UpperOutlierLimits->GetAt(i),
										 LowerOutlierLimits->GetAt(i),
										 ApplyOutlier->GetAt(i),
										 error_msg);
			if (FAILED(hr))
			{
				LogErrorMessage(hr,"Return from function \"CalculateSampleOutliers\"","FindHistoricOutlier");
				return hr;
			}
		}
	}

	OKMessage(error_msg);

	return S_OK;
}

/********************************************************
HRESULT CalculateSampleOutliers(CArray<int,int> *ptrCalibrationStatus,
								CArray<double,double> *ptrCalibrationData,
								int *ptrStatusIndex,
								int *ptrDataIndex,
								int sampleSize,
								double lower,
								double upper,
								CString apply,
								VARIANT *error_msg)

- Sets values status, according to outlier calculations.

INPUT
	*ptrCalibrationStatus - Data status list.
	*ptrCalibrationData - Calibration data.
	*ptrStatusIndex - Index to traverse status list.
	*ptrDataIndex - Index to traverse data list.
	sampleSize - Size of current sample.
	lower - Lower outlier limit.
	upper - Upper outlier limit.
	apply - Method to apply outlier limits.
	*error_msg - Error reporting.

OUTPUT
	Updated sample status.

LOCAL
	hr - return code.
  	currentData - Vector for current list of values.
	listSize - Size of current list.
	k - Index for current data vector.
	mean - Mean of current data.
	seps[] - String separators for token search.
	*token - Current string.

********************************************************/
HRESULT CalculateSampleOutliers(CPtrArray *statussets,
								CArray<int,int> *ptrCalibrationStatus,
								CArray<double,double> *ptrCalibrationData,
								int *ptrStatusIndex,
								int *ptrDataIndex,
								int sampleSize,
								double lower,
								double upper,
								CString apply,
								VARIANT *error_msg)
{
	dVector		currentData;
	double		mean;

	// Define a vector to accomodate current sample.
	currentData = VD_vector( sampleSize );

	// Copy current sample list to a vector.
	for (int k=0; k<sampleSize; k++)
	{
		// Get current sample value.
		currentData[k] = ptrCalibrationData->GetAt(*ptrDataIndex);
		(*ptrDataIndex)++;
	}

	mean = VD_mean( currentData, sampleSize ); 

	if (apply.CompareNoCase("PERCENT") == 0)
	{	// Apply outlier limits as a percentage

		// Find outliers.
		for (k=0; k<sampleSize; k++)
		{
			// Test limits.
			if ((fabs(currentData[k]) < (fabs(mean) - (fabs(lower) * fabs(mean)))) ||
				(fabs(currentData[k]) > (fabs(mean) + (fabs(upper) * fabs(mean)))))
			{
				UpdateValueStatus(statussets,*ptrStatusIndex);
			}

			(*ptrStatusIndex)++;
   		}
	}
	else if (apply.CompareNoCase("MULTIPLY") == 0)
	{	// Apply outlier limits as a multipier

	}
	else if (apply.CompareNoCase("SDFACTOR") == 0)
	{	// Apply outlier limits as a factor of the sample standard deviation.
		OutliersUsingStandardDeviation(ptrCalibrationStatus,
									   currentData,
									   ptrStatusIndex,
									   ptrDataIndex,
									   sampleSize,
									   lower,
									   upper);
	}
	else if (apply.CompareNoCase("SCALAR") == 0)
	{	// Apply outlier limits as numeric addition or subtraction

		// Find outliers.
		for (k=0; k<sampleSize; k++)
		{
			// Test limits.
			if ((fabs(currentData[k]) < (fabs(mean) - fabs(lower))) ||
				(fabs(currentData[k]) > (fabs(mean) + fabs(upper))))
			{
				UpdateValueStatus(statussets,*ptrStatusIndex);
			}

			(*ptrStatusIndex)++;
   		}
	}
	else
	{	// Error unknown outlier application.
		UserErrorMessage("3", "Wrong outlier limit type",error_msg);
		// 0x80040E21  -2147217887  Errors occurred
		LogErrorMessage(0x80040E21,"Illegal outlier limit application type","CalculateSampleOutliers");
		return 0x80040E21;
	}

	OKMessage(error_msg);

	V_free(currentData);

	return S_OK;
}

/*****************************************************************
HRESULT OutlierLimits(LONG calibrationID,
					  TCHAR sampleIds, 
					  CArray<double,double> *Upper, 
					  CArray<double,double> *Lower,
					  CArray<CString,CString> *ApplyOutlier,
					  VARIANT *error_msg)

- Returns Historic outlier method for a given part number, chemistry.

INPUT
	calibrationID - Calibration id.
	*Upper - Upper outlier limit.
	*Lower - Lower outlier limit.
	sampleIds - List of sample names.
	ApplyOutlier - Methods to apply outlier limits.
	error_msg - Error reporting.

OUTPUT
	Upper and lower outlier limits.

LOCAL
	hr - return code.
	db - Database access object.
	session - Session object.
*****************************************************************/
HRESULT OutlierLimits(LONG calibrationID,
					  CStringArray *sampleIds, 
					  CArray<double,double> *Upper, 
					  CArray<double,double> *Lower,
					  CArray<CString,CString> *ApplyOutlier,
					  VARIANT *error_msg)
{
	CDataSource db;
	CSession	session;
	HRESULT		hr;
	CString		chemistry;
	CString		l_sampleIds;
	char		seps[] = ",\n";
	char		*token;
	char		buf[512];

	// Get upper outlier limits command.
	CCommand<CAccessor<CdboGetUpperOutlierLimitsAccessor> > cmdGetUpperOutlierLimits;
	// Get lower outlier limits command.
	CCommand<CAccessor<CdboGetLowerOutlierLimitsAccessor> > cmdGetLowerOutlierLimits;	
	// Get apply methods for "outlier limits" command.
	CCommand<CAccessor<CdboGetApplyOutlierLimitsAccessor> > cmdGetApplyOutlierLimits;

	// < ----- GET CHEMISTRY NAME ----- >
	hr = GetChemistryNameGivenCalibrationID(calibrationID,&chemistry);
	if (FAILED(hr))
 	{
		sprintf(buf,"Function \"OutlierLimits\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" stored procedure",buf);
 		return hr;
 	}
	// </ ----- GET CHEMISTRY NAME ----- >

	hr = FromCStringArrayToCString(sampleIds,&l_sampleIds);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"FromCStringArrayToCString\"","OutlierLimits");
		return hr;
	}

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "OutlierLimits");
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","OutlierLimits");
		return hr;
	}

	// < ----- GET UPPER OUTLIER LIMITS FOR ALL SAMPLES ----- >
	// Create command to execute stored procedure.
	hr = cmdGetUpperOutlierLimits.Create (session, _T("{ ? = CALL dbo.GetUpperOutlierLimits;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetUpperOutlierLimits\"","OutlierLimits");
		return hr;
	}

	_tcsncpy(cmdGetUpperOutlierLimits.m_chemistry, chemistry,
 		sizeof(cmdGetUpperOutlierLimits.m_chemistry) /
 		sizeof(cmdGetUpperOutlierLimits.m_chemistry[0]));

	_tcsncpy(cmdGetUpperOutlierLimits.m_SampleIDList, l_sampleIds,
 		sizeof(cmdGetUpperOutlierLimits.m_SampleIDList) /
 		sizeof(cmdGetUpperOutlierLimits.m_SampleIDList[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetUpperOutlierLimits.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetUpperOutlierLimits\"","OutlierLimits");
		return hr;
	}

	// Move upper outlier limits from DB result set to parameter array.
	token = strtok( cmdGetUpperOutlierLimits.m_UpperOutliers, seps );
	while( token != NULL )
	{
		Upper->Add(atof(token));
		// Get next id.
		token = strtok( NULL, seps );
	}

	cmdGetUpperOutlierLimits.Close();
	// </ ----- GET UPPER OUTLIER LIMITS FOR ALL SAMPLES ----- >

	// < ----- GET LOWER OUTLIER LIMITS FOR ALL SAMPLES ----- >
	// Create command to execute stored procedure.
	hr = cmdGetLowerOutlierLimits.Create (session, _T("{ ? = CALL dbo.GetLowerOutlierLimits;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetLowerOutlierLimits\"","OutlierLimits");
		return hr;
	}

	_tcsncpy(cmdGetLowerOutlierLimits.m_chemistry, chemistry,
 		sizeof(cmdGetLowerOutlierLimits.m_chemistry) /
 		sizeof(cmdGetLowerOutlierLimits.m_chemistry[0]));

	_tcsncpy(cmdGetLowerOutlierLimits.m_SampleIDList, l_sampleIds,
 		sizeof(cmdGetLowerOutlierLimits.m_SampleIDList) /
 		sizeof(cmdGetLowerOutlierLimits.m_SampleIDList[0]));

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetLowerOutlierLimits.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetLowerOutlierLimits\"","OutlierLimits");
		return hr;
	}

	// Move lower outlier limits from DB result set to parameter array.
	token = strtok( cmdGetLowerOutlierLimits.m_LowerOutliers, seps );
	while( token != NULL )
	{
		Lower->Add(atof(token));
		// Get next id.
		token = strtok( NULL, seps );
	}

	cmdGetLowerOutlierLimits.Close();
	// </ ----- GET LOWER OUTLIER LIMITS FOR ALL SAMPLES ----- >

	// < ----- GET OUTLIER LIMITS' APPLICATION METHOD FOR ALL SAMPLES ----- >
	hr = cmdGetApplyOutlierLimits.Create (session, _T("{ ? = CALL dbo.GetApplyOutlierLimits;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetApplyOutlierLimits\"","OutlierLimits");
		return hr;
	}

	_tcsncpy(cmdGetApplyOutlierLimits.m_chemistry, chemistry,
 		sizeof(cmdGetApplyOutlierLimits.m_chemistry) /
 		sizeof(cmdGetApplyOutlierLimits.m_chemistry[0]));

	_tcsncpy(cmdGetApplyOutlierLimits.m_SampleIDList, l_sampleIds,
 		sizeof(cmdGetApplyOutlierLimits.m_SampleIDList) /
 		sizeof(cmdGetApplyOutlierLimits.m_SampleIDList[0]));

	// Execute current command.
	// Get apply outlier method.
	hr = cmdGetApplyOutlierLimits.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetApplyOutlierLimits\"","OutlierLimits");
		return hr;
	}

	// Move outlier limits' application method from DB result set to parameter array.
	token = strtok( cmdGetApplyOutlierLimits.m_ApplyOutliers, seps );
	while( token != NULL )
	{
		ApplyOutlier->Add(token);
		// Get next id.
		token = strtok( NULL, seps );
	}

	cmdGetApplyOutlierLimits.Close();
	// </ ----- GET OUTLIER LIMITS' APPLICATION METHOD FOR ALL SAMPLES ----- >

	// Close the database.
	db.Close();

	return S_OK;
}

/*****************************************************************
HRESULT GetCalibrationData(LONG calibrationID,
						   TCHAR *indexingIDs,
						   TCHAR *calibrationData,
						   TCHAR *calibrationStatus,
						   TCHAR *sampleSizes,
						   TCHAR *sampleIds)

	Get calibration data for given calibration id.

INPUT
	calibrationID - Calibration id.
	*indexingIDs - Data sequence.
	*calibrationData - Calibration values.
	*calibrationStatus - Value status.
	*sampleSizes - Sample sizes.
	*sampleIds - Sample ids.

OUTPUT
	Data sequence in "indexingIDs" array.
	Calibration values in "calibrationData" array.
	Value status in "calibrationStatus" array. 
	Sample sizes in "sampleSizes" array.
	Sample ids in "sampleIds" array.

LOCAL
	hr - return code.
	db - Database access object.
	session - Session object.

	localIndexSampleIDs - Sample ids, used to get sizes.
	localSampleIDs - Ids to pass to caller.
	strSampleLengths - Lengths to pass to caller.
	localIndexingIDs - Indexing ids to pass to caller.
	localCalibrationData - Data to return to caller.
	localCalibrationStatus - Status to pass to caller.
	localSampleLengths - Decimal formated sample sizes.
	buf[128] - Buffer to convert types.

*****************************************************************/
HRESULT GetCalibrationData(LONG calibrationID,
						   CStringArray *calibrationResults,
						   CArray<int,int> *calibrationStatus)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;
	char		*token;
	char		seps[] = ",\n";

	CCommand<CAccessor<CdboGetCalibrationDataResultsAccessor> > cmdDataResults;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// < ----- GET CALIBRATION DATA ----- >
	// Open database.
	hr = OpenDatabase(&db,&session,"GetCalibrationData");
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetCalibrationData");
		return hr;
	}

	hr = cmdDataResults.Create (session, _T("{ ? = CALL dbo.GetCalibrationDataResults;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to access \"GetCalibrationDataResults\" stored procedure","GetCalibrationData");
		return hr;
	}

	// Set database searching criteria.
	cmdDataResults.m_CalibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure, "GetCalibrationDataResults".
	hr = cmdDataResults.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetCalibrationDataResults\"","GetCalibrationData");
		return hr;
	}
		
	// Convert results.
	token = strtok( cmdDataResults.m_resultsset, seps );
	while( token != NULL )
	{
		calibrationResults->Add(token);
		// Get next result value.
		token = strtok( NULL, seps );
	}

	// Convert status.
	token = strtok( cmdDataResults.m_resultsstatus, seps );
	while( token != NULL )
	{
		calibrationStatus->Add(atoi(token));
		// Get next status.
		token = strtok( NULL, seps );
	}

	cmdDataResults.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/*****************************************************************
HRESULT UpdateDataStatus(LONG calibrationID,
						 CArray<int,int> *indexingIDs,
						 CArray<int,int> *calibrationStatus)

	Update values' status.

INPUT
	calibrationID - Calibration id.
	*indexingIDs - Data sequence.
	*calibrationStatus - Value status.

OUTPUT
	Updated database status.

LOCAL
	hr - return code.
	db - Database access object.
	session - Session object.
*****************************************************************/
HRESULT UpdateDataStatus(LONG calibrationID,
						 CArray<long,long> *indexingIDs,
						 CArray<int,int> *calibrationStatus)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;
	CString		l_indexingIDs;
	CString		l_calibrationStatus;
	CCommand<CAccessor<CdboUpdateDataStatusAccessor> > cmdUpdateStatus;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);
	cmdinit.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE);

	hr = FromCArrayToCString(indexingIDs, &l_indexingIDs);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"FromCArrayToCString\" function","UpdateDataStatus");
		return hr;
	}

	hr = FromCArrayToCString(calibrationStatus, &l_calibrationStatus);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"FromCArrayToCString\" function","UpdateDataStatus");
		return hr;
	}

	// Open database.
	hr = OpenDatabase( &db, &session, "UpdateDataStatus" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","UpdateDataStatus");
		return hr;
	}

	hr = cmdUpdateStatus.Create (session, _T("{ ? = CALL dbo.UpdateDataStatus;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Create command to access \"UpdateDataStatus\" stored procedure","UpdateDataStatus");
		return hr;
	}

	// Set database searching criteria.
	cmdUpdateStatus.m_calibrationID = calibrationID;

	_tcsncpy(cmdUpdateStatus.m_indexingList, l_indexingIDs,
 		sizeof(cmdUpdateStatus.m_indexingList) /
 		sizeof(cmdUpdateStatus.m_indexingList[0]));

	_tcsncpy(cmdUpdateStatus.m_dataStatus, l_calibrationStatus,
 		sizeof(cmdUpdateStatus.m_dataStatus) /
 		sizeof(cmdUpdateStatus.m_dataStatus[0]));

	// Execute current command.
	hr = cmdUpdateStatus.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		LogInterfaceErrorMessage(hr,"Call stored procedure \"UpdateDataStatus\"","UpdateDataStatus");
		return hr;
	}

	// Close access command.
	cmdUpdateStatus.Close();

	// Close the database.
	db.Close();

	return S_OK;
}

/*************************************************************
HRESULT GetCalibrationDataStatus(LONG calibrationID, 
								 VARIANT *StatusData)

--------------------------------------------------------------
 INPUT
	*calibrationID - Identifier for current calibration.
	*StatusData - Data statuses.

--------------------------------------------------------------
 OUTPUT
	Data status of current calibration.

--------------------------------------------------------------
 LOCAL
	hr - Error code.
	db - Object to access database.
	session - Represents a single database access session.

--------------------------------------------------------------
 RETURN
	hr - Error code.
*************************************************************/
HRESULT GetCalibrationDataStatus(LONG calibrationID, 
								 VARIANT *StatusData)
{
	CDataSource db;
	CSession	session;
	HRESULT		hr;
	long		i,k;

	CArray<long,long> lStatus;

	//*** define the safearray ***
    SAFEARRAY FAR* psa_StatusData = NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	// Define command to get data status from database.
	// Instantiate a command object, to run stored procedure.
	CCommand<CAccessor<CdboGetDataStatusAccessor> > cmdGetDataStatus;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetCalibrationDataStatus" );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"OpenDatabase\"","GetCalibrationDataStatus");
		return hr;
	}

	// Create command to execute stored procedure.
	hr = cmdGetDataStatus.Create (session, _T("{ ? = CALL dbo.GetDataStatus;1 (?) }"));

	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Create command to access stored procedure \"GetDataStatus\"","GetCalibrationDataStatus");
		return hr;
	}

	// Set database searching criteria.
	// Calibration id, used to query db.
	cmdGetDataStatus.m_calibrationID = calibrationID;

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetDataStatus.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Call stored procedure \"GetDataStatus\"","GetCalibrationDataStatus");
		return hr;
	}

	while (cmdGetDataStatus.MoveNext() == S_OK)
	{
		lStatus.Add(cmdGetDataStatus.m_colStatus);
	}

	// Set size of array to pass to client.
	sabound[0].cElements = lStatus.GetSize();
	sabound[0].lLbound = 0;

	//*** create the safearray to return ***
    psa_StatusData = SafeArrayCreate(VT_I4, 1, sabound);    

	i=0;
	char strStatus[80];
	CString temp;
	BSTR bstr=NULL;
	for (int j=0; j<lStatus.GetSize(); j++)
	{
		k = lStatus.GetAt(j);
		// Convert size from integer to string.
		sprintf(strStatus, "%d",k);
		temp = strStatus;
		bstr = temp.AllocSysString();
		hr = SafeArrayPutElement(psa_StatusData, &i, bstr);

		i++;
	}

	cmdGetDataStatus.Close();

	// Close the database.
	db.Close();

	// Set plotValue's array type to 32-bit integer.
	//*** set up the safearray to export it as an array of integers.  
	V_VT(StatusData) = VT_ARRAY | VT_I4;

	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
    V_ARRAY(StatusData) = psa_StatusData;

	return S_OK;
}

/********************************************************************
FUNCTION:
	GetCalibrationOutlierData
_____________________________________________________________________
DESCRIPTION:
	Find data for current calibration.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	buf - Temporary message.

____________________________________________________________________
PARAMETERS:
	calibrationID - Calibration identifier.
	indexingIDs - List of data order.
	calibrationData - Data.
	calibrationStatus - Status of data values.
	sampleSizes - Sizes of samples.
	sampleIds - List of sample ids.
	error_msg - Error reporting.
____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	08/06/04	Alves		New function.

********************************************************************/
HRESULT GetCalibrationOutlierData(LONG calibrationID,
								  CArray<long,long> *indexingIDs,
								  CArray<double,double> *calibrationData,
								  CArray<int,int> *calibrationStatus,
								  CArray<int,int> *sampleSizes,
								  CStringArray *sampleIds,
								  VARIANT *error_msg)
{
	HRESULT			hr;
	char			buf[512];

	// Get calibrator type sample names.
	hr = GetSampleIdsFromType(calibrationID,(CString)"CALIBRATOR",sampleIds);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSampleIdsFromType\"","GetCalibrationOutlierData");
		return hr;
	}

	// Get sizes of "CALIBRATOR" type samples.
	hr = GetSizesOfAListOfSamples(calibrationID,sampleIds,"EXCLUDED",sampleSizes);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSizesOfAListOfSamples\"","GetCalibrationOutlierData");
		return hr;
	}

	// Get data and status for current list of samples.
	hr = GetDataAndStatusForAListOfSamples(calibrationID,
										   "INCLUDED",
										   sampleIds,
										   calibrationData,
										   calibrationStatus,
										   error_msg);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationOutlierData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetDataAndStatusForAListOfSamples\" function",buf);
		return hr;
	}

	// Get data and status for current list of samples.
	hr = GetSampleValueIDsForIndexing(calibrationID,
									  indexingIDs,
									  error_msg);

	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetCalibrationOutlierData\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetSampleValueIDsForIndexing\" function",buf);
		return hr;
	}

	return S_OK;
}

/********************************************************************
FUNCTION:
	RemoveOutlierValues
_____________________________________________________________________
DESCRIPTION:
	Remove outlier data.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	k - Number of sample with outlier.
	l_samplesizes - Temporary sample sizes.

____________________________________________________________________
PARAMETERS:
	datasets - Current data sets.
	statussets - Current status sets.
	samplesizes - Sizes of current samples.
	error_msgs - Error reporting.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	08/06/04	Alves		New function.

********************************************************************/
HRESULT RemoveOutlierValues(CPtrArray *datasets,
							CPtrArray *statussets,
							CArray<int,int> *samplesizes,
							VARIANT *error_msgs)
{
	HRESULT hr=S_OK;
	int		k;
	CArray<int,int> l_samplesizes;

	l_samplesizes.RemoveAll();
	l_samplesizes.Copy(*samplesizes);

	// Find which sample the current outlier belongs to.
	for (int i=0; i<statussets->GetSize(); i++)
	{
		for(int j=(((CArray<int,int> *)statussets->GetAt(i))->GetSize()-1); j>=0; j--)
		{
			if ( (((CArray<int,int> *)statussets->GetAt(i))->GetAt(j) != VALID) &&
				 (((CArray<int,int> *)statussets->GetAt(i))->GetAt(j) != VALID_MOVED) )
			{
				k = TransformExcludedIndex(&l_samplesizes, j);

				samplesizes->SetAt(k,samplesizes->GetAt(k) - 1);

				for (int l=0; l<datasets->GetSize(); l++)
				{
					((CArray<double,double> *)datasets->GetAt(l))->RemoveAt(j);
				}
			}
		}
	}

	return hr;
}

/********************************************************************
FUNCTION:
	TestOutlierResults
_____________________________________________________________________
DESCRIPTION:
	Tests data for number of outliers, in each sample.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	hr - Return code.
	userMsg - User error reporting.
	sampleIds - List of samples in calibration.
	fail_sampleIds - List of sample ids that failed.
	sampleSizesWOutliers - Sizes of samples including outliers.
	sampleSizesNoOutliers - Sizes of samples excluding outliers.
____________________________________________________________________
PARAMETERS:
	calibrationID - Calibration identifier.
	mode - Calibration mode.
	error_msg - Error reporting.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	08/06/04	Alves		New function.

********************************************************************/
HRESULT TestOutlierResults(LONG calibrationID,
						   CString mode,
						   VARIANT *error_msg)
{
	HRESULT			hr=S_OK;
	CString			userMsg;
	CStringArray	sampleIds;
	CStringArray	fail_sampleIds;
	CArray<int,int> sampleSizesWOutliers;
	CArray<int,int> sampleSizesNoOutliers;

	// Get calibrator names for current calibration.
	hr = GetSampleIdsFromType(calibrationID,(CString)"CALIBRATOR",&sampleIds);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSampleIdsFromType\"","TestOutlierResults");
		return hr;
	}

	// Get sizes of "CALIBRATOR" type samples. Include outliers.
	hr = GetSizesOfAListOfSamples(calibrationID,&sampleIds,"EXCLUDED",&sampleSizesWOutliers);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSizesOfAListOfSamples\"","TestOutlierResults");
		return hr;
	}

	// Get sizes of "CALIBRATOR" type samples. Exclude outliers.
	hr = GetSizesOfAListOfSamples(calibrationID,&sampleIds,"VALID",&sampleSizesNoOutliers);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from function \"GetSizesOfAListOfSamples\"","TestOutlierResults");
		return hr;
	}

	// < ----- FIND SAMPLES WITH TOO MANY OUTLIERS ----- >
	fail_sampleIds.RemoveAll();

	for (int i=0; i<sampleIds.GetSize(); i++)
	{
		if ((((float)sampleSizesNoOutliers.GetAt(i)/(float)sampleSizesWOutliers.GetAt(i)) * 100.0) < 67.0)
		{
			fail_sampleIds.Add(sampleIds.GetAt(i));
		}
	}
	// </ ----- FIND SAMPLES WITH TOO MANY OUTLIERS ----- >

	// < ----- SET SAMPLE OUTLIER USER MESSAGE ----- >
	userMsg.Empty();
	if (fail_sampleIds.GetSize() > 0)
	{
		char	buf[512];
		CString chemistry;
		CString calibratorRule = "NUMBER_OF_CALIBRATORS";
		CStringArray businessRules;
		CArray<double,double> ruleValues;

		// < ----- GET CHEMISTRY NAME ----- >
		hr = GetChemistryNameGivenCalibrationID(calibrationID,&chemistry);
		if (FAILED(hr))
		{
			LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" function","ChemistryDependencies");
			return hr;
		}
		// </ ----- GET CHEMISTRY NAME ----- >

		// < ----- GET BUSINESS RULES FROM DB ----- >
		hr = GetBusinessRules(chemistry, mode, &businessRules, &ruleValues);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"TestBusinessRules\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"GetBusinessRules\" function",buf);
			return hr;
		}
		// </ ----- GET BUSINESS RULES FROM DB ----- >

		int rule_pos = FindStringPosition(&calibratorRule, &businessRules);
		if (rule_pos > -1)
		{
			double requiredNumberOfCalibrators, y;
			y = modf(ruleValues.GetAt(rule_pos),&requiredNumberOfCalibrators);     /* fraction=y and integer=currentNumberOfCalibrators */

			if ((double)(sampleIds.GetSize() - fail_sampleIds.GetSize()) < requiredNumberOfCalibrators)
			{
				userMsg = "Samples \"";

				for (i=0; i<fail_sampleIds.GetSize(); i++)
				{
					userMsg += fail_sampleIds.GetAt(i) + ",";
				}

				userMsg.Delete(userMsg.GetLength() -1, 1);

				userMsg += "\", have too many outliers!";

				UserErrorMessage("3",userMsg,error_msg);

				hr = 0x80040E21;
			}
		}
	}
	// </ ----- SET SAMPLE OUTLIER USER MESSAGE ----- >

	return hr;
}

/********************************************************************
FUNCTION:
	UpdateValueStatus
_____________________________________________________________________
DESCRIPTION:
	Update status for all sets of data.
_____________________________________________________________________
GLOBAL VARIABLES:
	None.
____________________________________________________________________
LOCAL VARIABLES:
	ptrCalibrationStatus - Current status set.

____________________________________________________________________
PARAMETERS:
	statussets - Status sets.
	statusIndex - Index for current status.

____________________________________________________________________
TESTING HISTORY:

_____________________________________________________________________

MODIFICATIONS:

Version	Date		Author		Description
1.0.0.0	08/06/04	Alves		New function.

********************************************************************/
void UpdateValueStatus(CPtrArray *statussets,
					   int statusIndex)
{
	CArray<int,int>	*ptrCalibrationStatus;

	for (int k=0; k<statussets->GetSize(); k++)
	{
		ptrCalibrationStatus = (CArray<int,int> *)statussets->GetAt(k);
		ptrCalibrationStatus->SetAt(statusIndex,OUTLIER);
	}
}