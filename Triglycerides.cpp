#include "stdafx.h"

/************************************************************
bool TestTriglyceridesAnalyteTypes(CStringArray *analyteTypesList)

	Find if analyte types are correct for Triglyceride, chemistries.

-------------------------------------------------------------
 INPUT
	analyteTypesList - Analyte list to test.

-------------------------------------------------------------
 OUTPUT
	Pass or failure.

-------------------------------------------------------------
 LOCAL
	i - Analyte list index.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
bool TestTriglyceridesAnalyteTypes(CStringArray *analyteTypesList)
{
	for (int i=0; i<analyteTypesList->GetSize(); i++)
	{
		if (analyteTypesList->GetAt(i).CompareNoCase("CALIBRATOR") != 0)
		{
			return false;
		}
	}

	return true;
}
