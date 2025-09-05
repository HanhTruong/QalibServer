/*******************************************************************
FILENAME:	BusinessRulesProcess.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Module for business rules processing.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	ProcessBusinessRules
	GetBusinessRules
	TypesOfCurrentDataSets

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BusinessRulesProcess.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include "Support.h"
#include "DbAccess.h"
#include "Loging.h"
#include "Utils.h"
#include "dboGetChemistryBusinessRules.H"
#include "BusinessRulesSupport.h"
#include "BusinessRules.h"
#include "BusinessRulesProcess.h"
#include "T4.h"

/********************************************************
HRESULT ProcessBusinessRules(LONG calibrationID,
							 BSTR partNumber,
							 VARIANT *serialNumberList,
							 VARIANT *error_msg)

	Process current business rules.

INPUT
	calibrationID - Identifier for current calibration.
	partNumber - Part number of current calibration.
	serialNumberList - List of instrument serial numbers.
	error_msg - Error reporting.

OUTPUT
	Application of each business rule.

LOCAL
	hr - return code.
	rulesList - List of rules.
	buf[512] - Error buffer.
	ruleValues - Values of current rules.
	current_analytes - Current sample types.

********************************************************/
HRESULT ProcessBusinessRules(LONG calibrationID,
							 CString chemistry,
							 CString mode,
							 CString calibrationSpecies,
							 VARIANT *serialNumberList,
							 VARIANT *error_msg)
{
	HRESULT					hr;
	CStringArray			rulesList;
	char					buf[512];
	CArray<double,double>	ruleValues;
	AnalyteType				current_analytes;

	// < ----- INITIALIZE CURRENT ANALYTE TYPES ----- >
	// Find list of current valid analyte types, from db.
	if (current_analytes.InitializeAnalyteTypes(&current_analytes))
	{
		hr = TypesOfCurrentDataSets(calibrationID, &current_analytes);
		if (FAILED(hr))
		{
			sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"current_analytes.InitializeAnalyteTypes\" function",buf);
			return hr;
		}
	}
	else
	{
		hr = 0x80040E21;

		// Is there a user error message, from lower processing?
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(hr,"Return from \"current_analytes.InitializeAnalyteTypes\" function",buf);
		}

		return hr;
	}
	// </ ----- INITIALIZE CURRENT ANALYTE TYPES ----- >

	// < ----- IF MODE IS "Experimental", IGNORE ALL RULES ----- >
	if (mode.CompareNoCase("Experimental") == 0)
	{
		return S_OK;
	}
	// </ ----- IF MODE IS "Experimental", IGNORE ALL RULES ----- >

	// < ----- GET BUSINESS RULES FROM DB ----- >
	hr = GetBusinessRules(chemistry, mode, &rulesList, &ruleValues);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetBusinessRules\" function",buf);
		return hr;
	}
	// </ ----- GET BUSINESS RULES FROM DB ----- >

	// < ----- PROCESS EACH RULE ----- >
	// Process each rule, for current part number, chemistry.
	for (int i=0; i<rulesList.GetSize(); i++)
	{
		if (rulesList.GetAt(i).CompareNoCase("80_20_TEMPLATE") == 0)
		{	// Use 80% of data from a wavelength and 20% of another.
			hr = Template_80_20();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem the 80-20 template.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Template_80_20\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("DILUENT_FACTORS") == 0)
		{
			hr = Diluent_Factors();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with diluent factors.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Diluent_Factors\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("DIWATER") == 0)
		{	// Calibration should have deionized water.
			hr = DeIonizedWater(calibrationID,chemistry,error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with deionized water sample.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"DeIonizedWater\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("ESTABLISH_T4_TOP_FACTOR") == 0)
		{	// Test two high calibrators in order to establish top factor.
			hr = Establish_T4_Top_Factor(calibrationID, 
										 chemistry, 
										 ruleValues.GetAt(i), 
										 error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with T4 top factors.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Establish_T4_Top_Factor\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("LEVEL_LIMITS") == 0)
		{	// Does this chemistry have processing that depends on level limits?
			hr = Level_Limits();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with level limits.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Level_Limits\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("MAX_INSTRUMENT_SAMPLE_RUNS") == 0)
		{	// Maximum number of times a sample can be run in the same sample.
			hr = Max_Instrument_Sample_Runs(calibrationID,
											ruleValues.GetAt(i),
											error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with instrument sample runs.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Max_Instrument_Sample_Runs\" function",buf);
				}

				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("MIN_NUMBER_INSTRUMENTS") == 0)
		{	// Minimum number of instruments for each calibration.
			hr = Min_Number_Instruments(ruleValues.GetAt(i), 
										serialNumberList, 
										error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem number of instruments.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Min_Number_Instruments\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("MIN_NUMBER_OF_DISKS_PER_SAMPLE") == 0)
		{	// Minimum number of disks that each sample must be ran in.

			hr = Min_Number_Of_Disks_Per_Sample(calibrationID, 
												ruleValues.GetAt(i),
												error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3","Problem number of disks per sample.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Min_Number_Of_Disks_Per_Sample\" function",buf);
				}

				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("MOLD_FACTORS") == 0)
		{
			hr = Mold_Factors();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with mold factors.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Mold_Factors\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("NUMBER_OF_CALIBRATORS") == 0)
		{	// Minimum number of calibrators.
			hr = Number_Of_Calibrators(calibrationID,chemistry,
									   calibrationSpecies,
									   ruleValues.GetAt(i),
									   error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with the number of calibrators.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr, "Return from \"Number_Of_Calibrators\" function", buf);
				}

				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("NUMBER_OF_CONTROLS") == 0)
		{	// Minimum number of controls.
			hr = Number_Of_Controls(calibrationID,chemistry,calibrationSpecies,
									ruleValues.GetAt(i),error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with number of controls.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr, "Return from \"Number_Of_Controls\" function", buf);
				}

				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("NUMBER_THRESHOLD_SAMPLES") == 0)
		{	// Number of threshold samples in this calibration.
			hr = Number_Threshold_Samples();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem number of threshold samples.", error_msg);
					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Number_Threshold_Samples\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("ROTOR_FACTORS") == 0)
		{	
			// Does this chemistry have processing that depends on individual rotors?
			hr = Rotor_Factors();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with rotor factors.", error_msg);
					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Rotor_Factors\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("SALINE") == 0)
		{	// Saline must be one of the samples.
			hr = Saline(calibrationID, chemistry, error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with saline sample.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Saline\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("SAMPLE_BLANK") == 0)
		{	// Sample blank.
			hr = Sample_Blank();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with sample blank.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Sample_Blank\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("THRESHOLD_VALUE") == 0)
		{	// Threshold values.
			hr = Threshold_Value();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with threshold values.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Threshold_Value\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("WAVELENGTHS") == 0)
		{	// Wavelengths of data to be calibrated.
			hr = Wavelengths();
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with wavelengths.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Wavelengths\" function",buf);
				}
				return hr;
			}
		}
		else if (rulesList.GetAt(i).CompareNoCase("ZERO_T4_LEVEL") == 0)
		{	// One of the T4 levels must be 0 ug/dl.
			hr = Zero_T4_Level(calibrationID, chemistry, error_msg);
			if (FAILED(hr))
			{
				// Is there a user error message, from lower processing?
				if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
				{
					UserErrorMessage("3", "Problem with zero T4 level.", error_msg);

					sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr,"Return from \"Zero_T4_Level\" function",buf);
				}
				return hr;
			}
		}
		else
		{
			sprintf(buf,"Function \"ProcessBusinessRules\", file %s, line %d", __FILE__, __LINE__);
			LogErrorMessage(0x80040E21,"Unknown Business Rule",buf);
			return 0x80040E21;
		}
	}
	// </ ----- PROCESS EACH RULE ----- >

	return S_OK;
}

/********************************************************
HRESULT GetBusinessRules(CString chemistry, 
						 CString mode,
						 CStringArray *businessrules,
						 CArray<double,double> *ruleValues)

- Get analyte types for current calibration samples.

INPUT
	chemistry - Chemistry name.
	businessrules - List of rules for current chemistry.
	mode - Calibration mode.
	ruleValues - List of values for current rules.

OUTPUT
	List of rules, and rule values, used in current calibration.

LOCAL
	hr - return code.
	db - Database object.
	session - Session object.
	buf[256] - Error buffer.

********************************************************/
HRESULT GetBusinessRules(CString chemistry, 
						 CString mode,
						 CStringArray *businessrules,
						 CArray<double,double> *ruleValues)
{
	HRESULT		hr;
	CDataSource	db;
	CSession	session;
	char		buf[256];
	char		seps[] = ",\n";
	char		*token;

	// Instantiate a command object, to get business rules.
	CCommand<CAccessor<CdboGetChemistryBusinessRulesAccessor> > cmdGetBusinessRules;
	
	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "GetBusinessRules" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetBusinessRules\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return hr;
	}

	// < ===== GET BUSINESS RULES ===== >
	hr = cmdGetBusinessRules.Create(session, _T("{ ? = CALL dbo.GetChemistryBusinessRules;1 (?,?,?) }"));
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetBusinessRules\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"GetChemistryBusinessRules\" stored procedure",buf);
		return hr;
	}

	// < ----- SET DATABASE SEARCH CRITERIA ----- >
 	_tcsncpy(cmdGetBusinessRules.m_chemistry, chemistry,
 		sizeof(cmdGetBusinessRules.m_chemistry) /
 		sizeof(cmdGetBusinessRules.m_chemistry[0]));

 	_tcsncpy(cmdGetBusinessRules.m_mode,mode,
 		sizeof(cmdGetBusinessRules.m_mode) /
 		sizeof(cmdGetBusinessRules.m_mode[0]));
	// </ ----- SET DATABASE SEARCH CRITERIA ----- >

	// Execute current command.
	// Call stored procedure.
	hr = cmdGetBusinessRules.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetBusinessRules\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call \"GetChemistryBusinessRules\" stored procedure",buf);
		return hr;
	}

	// < ----- BUSINESS RULES, RULE VALUES ----- >
	token = strtok( cmdGetBusinessRules.m_resultlist, seps );
	while( token != NULL )
	{
		businessrules->Add(token);
		// Get next rule id.
		token = strtok( NULL, seps );

		ruleValues->Add(atof(token));
		// Get next value.
		token = strtok( NULL, seps );
	}
	// </ ----- BUSINESS RULES, RULE VALUES ----- >
	
	cmdGetBusinessRules.Close();
	// </ ===== GET BUSINESS RULES ===== >

	// Close the database.
	db.Close();

	return S_OK;
}

/********************************************************
HRESULT TypesOfCurrentDataSets(LONG calibrationID, 
							   AnalyteType *current_analytes)

- Get analyte types for current calibration samples.

INPUT
	calibrationID - Identifier for current calibration.
	current_analytes - List of allowed analyte types.

OUTPUT
	List of analyte types used in current calibration.

LOCAL
	hr - return code.
	analyteTypesList - List of analytes.
	l_sampleIdsList - List of samples, in array format.
	sampleIdList - List of samples, in string format.
	typeCounter - Count the number of types.

********************************************************/
HRESULT TypesOfCurrentDataSets(LONG calibrationID, 
							   AnalyteType *current_analytes)
{
	HRESULT			hr;
	CStringArray	analyteTypesList;
	CStringArray	l_sampleIdsList;
	int				typeCounter;
	char			buf[512];

	// < --------------- GET SAMPLE NAMES --------------- >
	hr = GetCalibrationIDSamples(calibrationID, &l_sampleIdsList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"TypesOfCurrentDataSets\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Return from \"GetCalibrationIDSamples\" function",buf);
		return hr;
	}
	// </ --------------- GET SAMPLE NAMES --------------- >

	// < --------------- GET ANALYTE TYPES --------------- >
	hr = GetSamplesAnalyteTypes(&l_sampleIdsList, &analyteTypesList);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"TypesOfCurrentDataSets\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr, 
			"Return from \"GetSamplesAnalyteTypes\" function",
			buf);
		return hr;
	}
	// </ --------------- GET ANALYTE TYPES --------------- >

	typeCounter = 0;
	for (int i=0; i<analyteTypesList.GetSize(); i++)
	{
		current_analytes->GetCurrentTypes()->Lookup(analyteTypesList.GetAt(i), typeCounter);
		current_analytes->GetCurrentTypes()->SetAt(analyteTypesList.GetAt(i), ++typeCounter);
	}

	return S_OK;
}

