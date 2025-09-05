#include "stdafx.h"

int FindChemistryIndex(CString chemistry)
{
	CMap< CString, LPCSTR, int, int > ChemistryIndex;
	int ret_index = -1;
	CString l_chemistry;

	l_chemistry = chemistry;
	l_chemistry.MakeUpper();

	ChemistryIndex.SetAt("ALB_BCG",0);
//	ChemistryIndex.SetAt("ALB_BCP_Human",0);
	ChemistryIndex.SetAt("ALB_BCP_HUMAN",0);
	ChemistryIndex.SetAt("ALB_BCP_HUMAN_0",0);
	ChemistryIndex.SetAt("ALB_BCP_HUMAN_1",0);
	ChemistryIndex.SetAt("ALB_BCP_VET",0);
	ChemistryIndex.SetAt("ALP",1);
	ChemistryIndex.SetAt("ALT",2);
	ChemistryIndex.SetAt("AMY",3);
	ChemistryIndex.SetAt("AST",4);
	ChemistryIndex.SetAt("BUN",5);
	ChemistryIndex.SetAt("BUN2",5);
	ChemistryIndex.SetAt("CA_HUMAN",6);
	ChemistryIndex.SetAt("CA_HUMAN_R44",6);
	ChemistryIndex.SetAt("CA_VET",6);
	ChemistryIndex.SetAt("CA_VET26",6);
	ChemistryIndex.SetAt("CA_VET_0",6);
	ChemistryIndex.SetAt("CA_VET_1",6);
	ChemistryIndex.SetAt("CHOL_HUMAN",7);
	ChemistryIndex.SetAt("CHOL_HUMAN_R46",7);
	ChemistryIndex.SetAt("CHOL_VET",7);
	ChemistryIndex.SetAt("CHOL_VET_R40",7);
	ChemistryIndex.SetAt("CK",8);
	ChemistryIndex.SetAt("CL",9);
	ChemistryIndex.SetAt("CRE",10);
	ChemistryIndex.SetAt("CRE2",10);
	ChemistryIndex.SetAt("DBIL",11);
	ChemistryIndex.SetAt("GGT",12);
	ChemistryIndex.SetAt("GLU",13);
	ChemistryIndex.SetAt("GLU2",13);
//	ChemistryIndex.SetAt("HDL",14);
	ChemistryIndex.SetAt("HDL_CUV1",14);
	ChemistryIndex.SetAt("HDL_CUV2",14);
	ChemistryIndex.SetAt("IQC",15);
	ChemistryIndex.SetAt("K_ENZ",16);
	ChemistryIndex.SetAt("MG",17);
	ChemistryIndex.SetAt("NA",18);
	ChemistryIndex.SetAt("PHOS",19);
	ChemistryIndex.SetAt("T_4_1",20);
	ChemistryIndex.SetAt("TBIL_HUMAN",21);
	ChemistryIndex.SetAt("TBIL_VET_DILE",21);
	ChemistryIndex.SetAt("TBIL_VET_DILH",21);
	ChemistryIndex.SetAt("TCO2",22);
	ChemistryIndex.SetAt("TP",23);
	ChemistryIndex.SetAt("TRIG",24);
	ChemistryIndex.SetAt("UA",25);

//	ChemistryIndex.Lookup(chemistry,ret_index);
	ChemistryIndex.Lookup(l_chemistry,ret_index);

	return ret_index;
}

