/*******************************************************************

FILENAME:	Albumin.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:

	Albumin specific rules.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	FindStringPosition
	TestRule

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	Albumin.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/

#include "stdafx.h"
#include <math.h>
#include "Loging.h"
#include "Utils.h"
#include "BusinessRulesEnforce.h"

/*****************************************************************
HRESULT ALB_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)

	Rules specific to Albumin.
	ALB_BCG	Albumin (BCG - Bromo Cresol Green)
	ALB_BCP_Human	Albumin (BCP - Bromo Cresol Purple)
	ALB_BCP_Vet	Albumin (BCP - Bromo Cresol Purple)

INPUT
	rulesReference - Albumin valid rules.
	ruleValuesReference - Albumin rule values.
	rulesToTest - Given rules to compare to Albumin rules list.
	ruleValuesToTest - Given rule values to compare to Albumin rule values.
	error_msg - Error reporting buffer.

OUTPUT
	Database updated with current calibration data and results.

	calibrationID - Identifier for current calibration.
	error_msg	  - Error message.

LOCAL
	hr		 - Handle returned from safearray object.
	buf[512] - Message buffer.
	i		 - Index for rules to test array.
	pos		 - Position of rule to test in Albumin rules list.

RETURN
	Function status, hr.

*****************************************************************/
HRESULT ALB_Rules(CStringArray			*rulesReference,
				  CArray<double,double>	*ruleValuesReference,
				  CStringArray			*rulesToTest,
				  CArray<double,double>	*ruleValuesToTest,
				  VARIANT				*error_msg)
{
	HRESULT hr=S_OK;
	char	buf[512];
	int		pos;

	for (int i=0; i<rulesToTest->GetSize(); i++)
	{
		pos = FindStringPosition(&rulesToTest->GetAt(i), rulesReference);
		if (pos > -1)
		{
			hr = TestRule(rulesToTest->GetAt(i),
						  ruleValuesToTest->GetAt(i),
						  rulesReference->GetAt(pos),
						  ruleValuesReference->GetAt(pos),
						  error_msg);
			if (FAILED(hr))
			{
				if (error_msg->vt == VT_EMPTY)
				{
					sprintf(buf,"Function \"ALB_Rules\", file %s, line %d", __FILE__, __LINE__);
					LogErrorMessage(hr, "Return from function \"TestRule\"", buf);
				}

				return hr;
			}
		}
	}

	return hr;
}

