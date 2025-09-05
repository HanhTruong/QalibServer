/*******************************************************************
FILENAME:	BusinessRulesEnforce.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions that enforce business rules.

--------------------------------------------------------------------
FUNCTIONS called from this module:

		IMPLEMENTED
		-----------
	enforce_rules_functions
	AssignRulesFunctions
	TestBusinessRules
	AssignRulesFunctions
	TestRule

		NOT IMPLEMENTED
		---------------
These functions will be implemented in individual chemistry name files.
i.e. ALB_Rules implemented in Albumin.cpp.

	ALP_Rules
	ALT_Rules
	AMY_Rules
	AST_Rules
	BUN_Rules
	BUN2_Rules
	CA_Rules
	CHOL_Rules
	CK_Rules
	CL_Rules
	CRE_Rules
	CRE2_Rules
	DBIL_Rules
	GGT_Rules
	GLU_Rules
	GLU2_Rules
	HDL_Rules
	IQC_Rules
	K_ENZ_Rules
	MG_Rules
	NA_Rules
	PHOS_Rules
	T_4_1_Rules
	TBIL_Rules
	tCO2_Rules
	TP_Rules
	TRIG_Rules
	UA_Rules

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	BusinessRulesEnforce.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include <math.h>
#include "BusinessRulesEnforce.h"
#include "BusinessRulesProcess.h"
#include "Support.h"
#include "Loging.h"
#include "FunctionArray.h"

enforce_rules enforce_rules_functions[26];
void AssignRulesFunctions();

/*****************************************************************
HRESULT TestBusinessRules(CString				chemistry,
						  CString				mode,
						  CStringArray			*rulesToTest,
						  CArray<double,double>	*ruleValuesToTest,
						  VARIANT				*error_msg)

INPUT
	chemistry - Current chemistry.
	mode - Calibration mode.
	rulesToTest - List of current chemistry's rules to test.
	ruleValuesToTest - Values of current rules.
	error_msg - Error messages.

OUTPUT
	None.

LOCAL
	hr - Function status code.
	rules - Current chemistry's rules, from the db.
	values - Current rule values, from db.
	chemistry_index - Location of chemistry name.
	buf - Error message.

RETURN
	Function status code.

*****************************************************************/
HRESULT TestBusinessRules(CString				chemistry,
						  CString				mode,
						  CStringArray			*rulesToTest,
						  CArray<double,double>	*ruleValuesToTest,
						  VARIANT				*error_msg)
{
	HRESULT					hr = S_OK;
	CStringArray			rules;
	CArray<double,double>	values;
	int						chemistry_index;
	char					buf[512];

	// < ----- GET BUSINESS RULES FROM DB ----- >
	hr = GetBusinessRules(chemistry, mode,&rules, &values);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"TestBusinessRules\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"GetBusinessRules\" function",buf);
		return hr;
	}
	// </ ----- GET BUSINESS RULES FROM DB ----- >

	chemistry_index = FindChemistryIndex(chemistry);

	AssignRulesFunctions();

	hr = enforce_rules_functions[chemistry_index](&rules,
												  &values,
												  rulesToTest,
												  ruleValuesToTest,
												  error_msg);
	if (FAILED(hr))
	{
		if (error_msg->vt == VT_NULL || error_msg->vt == VT_EMPTY)
		{
			sprintf(buf,"Return from \"enforce_rules_functions\" function for chemistry \"%s\".",chemistry);
			LogErrorMessage(hr,buf,"TestBusinessRules");
		}
	}

	return hr;
}

/*****************************************************************
void AssignRulesFunctions()
	Sets a global structure with the names of the chemistry rule functions.

INPUT
	None.

OUTPUT
	enforce_rules_functions structure 
	initialized with chemistry function names.

LOCAL
	None.

RETURN
	None.

*****************************************************************/
void AssignRulesFunctions()
{
	enforce_rules_functions[0] = ALB_Rules;
	enforce_rules_functions[1] = ALP_Rules;
	enforce_rules_functions[2] = ALT_Rules;
	enforce_rules_functions[3] = AMY_Rules;
	enforce_rules_functions[4] = AST_Rules;
	enforce_rules_functions[5] = BUN_Rules;
	enforce_rules_functions[6] = CA_Rules;
	enforce_rules_functions[7] = CHOL_Rules;
	enforce_rules_functions[8] = CK_Rules;
	enforce_rules_functions[9] = CL_Rules;
	enforce_rules_functions[10] = CRE_Rules;
	enforce_rules_functions[11] = DBIL_Rules;
	enforce_rules_functions[12] = GGT_Rules;
	enforce_rules_functions[13] = GLU_Rules;
	enforce_rules_functions[14] = HDL_Rules;
	enforce_rules_functions[15] = IQC_Rules;
	enforce_rules_functions[16] = K_ENZ_Rules;
	enforce_rules_functions[17] = MG_Rules;
	enforce_rules_functions[18] = NA_Rules;
	enforce_rules_functions[19] = PHOS_Rules;
	enforce_rules_functions[20] = T_4_1_Rules;
	enforce_rules_functions[21] = TBIL_Rules;
	enforce_rules_functions[22] = tCO2_Rules;
	enforce_rules_functions[23] = TP_Rules;
	enforce_rules_functions[24] = TRIG_Rules;
	enforce_rules_functions[25] = UA_Rules;
}

/*****************************************************************
HRESULT TestRule(CString	ruleToTest,
				 double		valueToTest,
				 CString	ruleRef,
				 double		valueRef,
				 VARIANT	*result_msg)

INPUT
	ruleToTest - List of rules to test.
	valueToTest - Values of rules to test.
	ruleRef - Reference rules to test against.
	valueRef - Reference rule values to test against.
	result_msg - Error message.

OUTPUT
	enforce_rules_functions structure 
	initialized with chemistry function names.

LOCAL
	currentValueToTest - Testing value.
	currentValueReference - Reference value.
	y - Convert double to integer and faction parts.
	buf - Error messages.

RETURN
	None.

*****************************************************************/
HRESULT TestRule(CString	ruleToTest,
				 double		valueToTest,
				 CString	ruleRef,
				 double		valueRef,
				 VARIANT	*result_msg)
{
	double	currentValueToTest, currentValueReference, y;
	char	buf[512];

	y = modf(valueToTest,&currentValueToTest);
	y = modf(valueRef,&currentValueReference);

	if (ruleToTest.CompareNoCase("NUMBER_OF_CALIBRATORS") == 0)
	{
		if (valueToTest < valueRef)
		{
			sprintf(buf,"Number of calibrators is \"%.f\". Must be at least \"%.f\".",currentValueToTest,currentValueReference);
			UserErrorMessage("3", buf,result_msg);
			return 0x80040E21;
		}
	}
	else if (ruleToTest.CompareNoCase("MIN_NUMBER_INSTRUMENTS") == 0)
	{
		if (valueToTest < valueRef)
		{
			sprintf(buf,"Number of instruments is \"%.f\". Minimum allowed is \"%.f\".",currentValueToTest,currentValueReference);
			UserErrorMessage("3", buf,result_msg);
			return 0x80040E21;
		}
	}
	else if (ruleToTest.CompareNoCase("MIN_NUMBER_OF_DISKS_PER_SAMPLE") == 0)
	{
		if (valueToTest < valueRef)
		{
			sprintf(buf,"Number of disks is \"%.f\". Minimum allowed is \"%.f\".",currentValueToTest,currentValueReference);
			UserErrorMessage("3", buf,result_msg);
			return 0x80040E21;
		}
	}
	else if (ruleToTest.CompareNoCase("MAX_INSTRUMENT_SAMPLE_RUNS") == 0)
	{
		if (valueToTest < valueRef)
		{
			sprintf(buf,"Number of instruments per sample is \"%.f\". Maximum allowed is \"%.f\".",currentValueToTest,currentValueReference);
			UserErrorMessage("3", buf,result_msg);
			return 0x80040E21;
		}
	}
	else if (ruleToTest.CompareNoCase("WAVELENGTHS") == 0)
	{
		if (valueToTest < valueRef)
		{
			sprintf(buf,"Number of wavelengths is \"%.f\". Must be at least \"%.f\".",currentValueToTest,currentValueReference);
			UserErrorMessage("3", buf,result_msg);
			return 0x80040E21;
		}
	}
	else
	{
		sprintf(buf,"Error: Unknown rule!");
		UserErrorMessage("3", buf,result_msg);
		return 0x80040E21;
	}

	return S_OK;
}

/*
	Alkaline Phosphatase
*/
HRESULT ALP_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Alanine Aminotransferase
*/
HRESULT ALT_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Amylase
*/
HRESULT AMY_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Aspartate Aminotransferase
*/
HRESULT AST_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Urea Nitrogen
*/
HRESULT BUN_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT BUN2_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
CA_Human	Calcium
CA_Human_R44	Calcium
CA_Vet	Calcium
CA_Vet26	Calcium
*/
HRESULT CA_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
CHOL_Human	Cholesterol
CHOL_Human_R46	Cholesterol
CHOL_Vet	Cholesterol
CHOL_Vet_R40	Cholesterol
*/
HRESULT CHOL_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Creatine Kinase
*/
HRESULT CK_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
Apply Chloride dependent data pre-processing.
The values of the data sets are averaged.
Chloride
*/
HRESULT CL_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Creatinine
*/
HRESULT CRE_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT CRE2_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Direct Bilirubin 
*/
HRESULT DBIL_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
Gamma Glutamyl Transferase 
*/
HRESULT GGT_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Glucose
*/
HRESULT GLU_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT GLU2_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
HDL High Density Lipoprotein Perciptating reagent 2
*/
HRESULT HDL_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
IQC	Instrument Qualit Control
*/
HRESULT IQC_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Potassium Enzyme
*/
HRESULT K_ENZ_Rules(CStringArray			*rulesReference,
				    CArray<double,double>	*ruleValuesReference,
				    CStringArray			*rulesToTest,
				    CArray<double,double>	*ruleValuesToTest,
				    VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Magnesium
*/
HRESULT MG_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Sodium
*/
HRESULT NA_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
	Phosphorous
*/
HRESULT PHOS_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
Thyroxine
*/
HRESULT T_4_1_Rules(CStringArray			*rulesReference,
				    CArray<double,double>	*ruleValuesReference,
				    CStringArray			*rulesToTest,
				    CArray<double,double>	*ruleValuesToTest,
				    VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
TBIL_Human	Total Bilirubin
TBIL_Vet_DilE	Total Bilirubin
TBIL_Vet_DilH	Total Bilirubin
*/
HRESULT TBIL_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT tCO2_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT TP_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
TRIG	Triglycerides Dye
*/
HRESULT TRIG_Rules(CStringArray				*rulesReference,
				   CArray<double,double>	*ruleValuesReference,
				   CStringArray				*rulesToTest,
				   CArray<double,double>	*ruleValuesToTest,
				   VARIANT					*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT UA_Rules(CStringArray			*rulesReference,
				 CArray<double,double>	*ruleValuesReference,
				 CStringArray			*rulesToTest,
				 CArray<double,double>	*ruleValuesToTest,
				 VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	return hr;
}
