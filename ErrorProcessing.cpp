#include "stdafx.h"
#include <afx.h>
#include <OLEDBERR.H>
#include <fstream.h>

#include "ErrorProcessing.h"
#include "Loging.h"

HRESULT DataBaseError(LONG	  error_number, 
					  CString error_source,
					  VARIANT *error_msg)
{
	HRESULT hr = 0x80040E21; // 0x80040E21  -2147217887  Errors occurred

	switch(error_number)
	{
	case 19000:
		// Code 19000 from stored procedure "InsertRecoveredValuesFromRates"
		// Called from function "PutCalculatedAssignedValuesInDb", 
		//		in source code file Support.cpp.
		// Assigned values list is empty.
		LogErrorMessage(hr,"Assigned values list is empty","DataBaseError");
		break;
	case 19001:
		// Code 19001 from stored procedure "InsertRecoveredValuesFromRates"
		// Called from function "PutCalculatedAssignedValuesInDb", 
		//		in source code file Support.cpp.
		// Recovered values list is empty.
		LogErrorMessage(hr,"Recovered values list is empty","DataBaseError");
		break;

	case 19002:
		// Code 19002 from stored procedure "InsertRecoveredValuesFromRates"
		// Called from function "PutCalculatedAssignedValuesInDb", 
		//		in source code file Support.cpp.
		// The number of "Assigned values" does not equal 
		// the number of "Recovered values".
		// Assigned and Recovered values are 1 to 1.
		LogErrorMessage(hr,"Number of recovered and assigned values, must be equal","DataBaseError");
		break;

	case 19003:
		// Code 19003 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		// No data values to insert.
		LogErrorMessage(hr,"There is no calibration data","DataBaseError");
		break;
	case 19004:
		// Code 19004 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", in source file QalibServerMain.cpp.
		LogErrorMessage(hr,"Data do not have status values","DataBaseError");
		break;
	case 19005:
		// Code 19005 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		LogErrorMessage(hr,"Samples have no data","DataBaseError");
		break;
	case 19006:
		// Code 19006 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		// Sample id list is empty.
		LogErrorMessage(hr,"No samples ids to insert","DataBaseError");
		break;
	case 19007:
		// Code 19007 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		// The number of "Assigned values" does not equal 
		//		the number of "Recovered values".
		// Assigned and Recovered values are 1 to 1.
		LogErrorMessage(hr,"Number of recovered and assigned values, must be equal","DataBaseError");
		break;
	case 19013:
		// Code 19013 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		// No instrument ids to insert.
		LogErrorMessage(hr,"No instrument ids","DataBaseError");
		break;
	case 19014:
		// Code 19014 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB", 
		//		in source file QalibServerMain.cpp.
		// The number of "instrument ids" does not equal 
		//		the number of "date values".
		LogErrorMessage(hr,"Number of values, and instrument ids, must be equal","DataBaseError");
		UserErrorMessage("3","Each data value must have an instrument source.",error_msg);

		break;

	case 19008:
		// Code 19008 from stored procedure "UpdateDataStatus"
		// Called from function "UpdateDataStatus", in source file "Outliers.cpp".
		// Index list is empty.
		LogErrorMessage(hr,"List of status indicies is empty","DataBaseError");
		break;
	case 19009:
		// Code 19009 from stored procedure "UpdateDataStatus"
		// Called from function "UpdateDataStatus" in source file "Outliers.cpp".
		// Status list is empty.
		LogErrorMessage(hr,"Status list is empty","DataBaseError");
		break;
	case 19010:
		// Code 19010 from stored procedure "UpdateDataStatus"
		// Called from function "UpdateDataStatus" in source file "Outliers.cpp".
		// Number of indecies must equal number of status.
		// Order of status must be recoverable.
		// Each status must be traced back to the value it applies to.
		LogErrorMessage(hr,"Number of indicies and status values, must be equal","DataBaseError");
		break;
	case 19011:
		// Code 19011 from stored procedure "GetAssignedValues"
		// Called from function "GetAssignedValuesDB" in source file "QalibServerMain.cpp".
		// List of samples is empty.
		LogErrorMessage(hr,"Sample list is empty","DataBaseError");
		break;
	case 19012:
		// Code 19012 from stored procedure "GetResultsAssignedValues"
		// Called from function "GetAssignedValues" in source file "Support.cpp".
		// Invalid part number.
		LogErrorMessage(hr,"Invalid part number","DataBaseError");
		break;
	case 19015:
		// Code 19015 from stored procedure "GetSampleTypes"
		// Called from function "UpdateDataStatus" in source file "Outliers.cpp".
		// Number of indecies must equal number of status.
		// Order of status must be recoverable.
		// Each status must be traced back to the value it applies to.
		LogErrorMessage(hr,"Number of indicies and status values, must be equal","DataBaseError");
		break;
	case 19016:
		// Code 19016 from stored procedure "InsertSampleData"
		// Called from function "LoadCalibrationDataToDB" in source file "QalibServerMain.cpp".
		// Number of barcodes must equal number of status.
		LogErrorMessage(hr,"Number of barcodes and values, must be equal","DataBaseError");
		break;
	case 19017:
		// Code 19017 from stored procedure "ExtractSubstring"
		// Called from stored procedure "InsertSampleData".
		// Number of barcodes must equal number of status.
		LogErrorMessage(hr,"Empty strings have no substrings.","DataBaseError");
		break;
	case 19018:
		// Code 19018 from stored procedure "GetSampleSpecies"
		// -- Called from: GetExpectedSampleSpecies, Species.cpp.
		LogErrorMessage(hr,"Sample list should not be empty.","DataBaseError");
		break;

	default:
		// These error should be interface errors.
		LogInterfaceErrorMessage(hr, "Default", "DataBaseError");
		break;
	}

	return hr;
}
