/*******************************************************************
FILENAME:	ChemistryDependent.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions for chemistry dependent calibration.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	ChemistryDependencies
	AssignChemistryFunctions

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	ChemistryDependent.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include "ChemistryDependent.h"
#include "Support.h"
#include "Loging.h"
#include "Chloride.h"
#include "FunctionArray.h"

chemistry_function chemistry_functions[26];

void AssignChemistryFunctions();

HRESULT ChemistryDependencies(LONG calibrationID,
							  CPtrArray *datasets,
							  VARIANT *error_msgs)
{
	HRESULT hr=S_OK;
	CString chemistry;
	int chemistry_index;
	char buf[512];

	hr = GetChemistryNameGivenCalibrationID(calibrationID,&chemistry);
	if (FAILED(hr))
	{
		LogInterfaceErrorMessage(hr,"Return from \"GetChemistryNameGivenCalibrationID\" function","ChemistryDependencies");
		return hr;
	}

	chemistry_index = FindChemistryIndex(chemistry);

	AssignChemistryFunctions();

	hr = chemistry_functions[chemistry_index](datasets);
	if (FAILED(hr))
	{
		sprintf(buf,"Return from \"chemistry_functions\" function for chemistry \"%s\".",chemistry);
		LogInterfaceErrorMessage(hr,buf,"ChemistryDependencies");
		return hr;
	}

	return hr;
}

void AssignChemistryFunctions()
{
	chemistry_functions[0] = ALB_Albumin_Bromo_Cresol;
	chemistry_functions[1] = ALP_Alkaline_Phosphatase;
	chemistry_functions[2] = ALT_Alanine_Aminotransferase;
	chemistry_functions[3] = AMY_Amylase;
	chemistry_functions[4] = AST_Aspartate_Aminotransferase;
	chemistry_functions[5] = BUN_Urea_Nitrogen;
	chemistry_functions[6] = CA_Calcium;
	chemistry_functions[7] = CHOL_Cholesterol;
	chemistry_functions[8] = CK_Creatine_Kinase;
	chemistry_functions[9] = CL_Chloride;
	chemistry_functions[10] = CRE_Creatinine;
	chemistry_functions[11] = DBIL_Direct_Bilirubin;
	chemistry_functions[12] = GGT_Gamma_Glutamyl_Transferase;
	chemistry_functions[13] = GLU_Glucose;
	chemistry_functions[14] = HDL_High_Density_Lipoprotein_Perciptating_reagent;
	chemistry_functions[15] = IQC_Instrument_Qualit_Control;
	chemistry_functions[16] = K_ENZ_Potassium;
	chemistry_functions[17] = MG_Magnesium;
	chemistry_functions[18] = NA_Sodium;
	chemistry_functions[19] = PHOS_Phosphorous;
	chemistry_functions[20] = T_4_1_Thyroxine;
	chemistry_functions[21] = TBIL_Total_Bilirubin;
	chemistry_functions[22] = tCO2_Total_Carbon_Dioxide;
	chemistry_functions[23] = TP_Total_Protein;
	chemistry_functions[24] = TRIG_Triglycerides;
	chemistry_functions[25] = UA_Uric_Acid;
}

/*
ALB_BCG	Albumin (BCG - Bromo Cresol Green)
ALB_BCP_Human	Albumin (BCP - Bromo Cresol Purple)
ALB_BCP_Vet		Albumin (BCP - Bromo Cresol Purple)
*/
HRESULT ALB_Albumin_Bromo_Cresol(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT ALP_Alkaline_Phosphatase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT ALT_Alanine_Aminotransferase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT AMY_Amylase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT AST_Aspartate_Aminotransferase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT BUN_Urea_Nitrogen(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT BUN2_Urea_Nitrogen(CPtrArray *datasets)
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
HRESULT CA_Calcium(CPtrArray *datasets)
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
HRESULT CHOL_Cholesterol(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT CK_Creatine_Kinase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT CRE_Creatinine(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT CRE2_Creatinine(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT DBIL_Direct_Bilirubin(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT GGT_Gamma_Glutamyl_Transferase(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT GLU_Glucose(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT GLU2_Glucose(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
HDL High Density Lipoprotein Perciptating reagent 2
*/
HRESULT HDL_High_Density_Lipoprotein_Perciptating_reagent(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
IQC	Instrument Qualit Control
*/
HRESULT IQC_Instrument_Qualit_Control(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT K_ENZ_Potassium(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT MG_Magnesium(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT NA_Sodium(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT PHOS_Phosphorous(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT T_4_1_Thyroxine(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
TBIL_Human	Total Bilirubin
TBIL_Vet_DilE	Total Bilirubin
TBIL_Vet_DilH	Total Bilirubin
*/
HRESULT TBIL_Total_Bilirubin(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT tCO2_Total_Carbon_Dioxide(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT TP_Total_Protein(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

/*
TRIG	Triglycerides Dye
*/
HRESULT TRIG_Triglycerides(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}

HRESULT UA_Uric_Acid(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	return hr;
}
