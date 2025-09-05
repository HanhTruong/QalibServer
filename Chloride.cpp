/*******************************************************************
FILENAME:	Chloride.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions to update data sets required by Chloride.

--------------------------------------------------------------------
FUNCTIONS called from this module:
	CL_Chloride

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	Chloride.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <afxtempl.h>
#include "Loging.h"
#include "ChemistryDependent.h"

/*****************************************************************
HRESULT CL_Chloride(CPtrArray *datasets)

	Update data to Chloride specificity.
	Apply Chloride dependent data pre-processing.
	The values of the data sets are averaged.

INPUT
	datasets - Pointer to current calibration data sets.

OUTPUT
	datasets updated.

LOCAL
	hr - Function status code.
	number_of_sets - Number of data sets.
	tmp_set - Current data set.
	tmp_sum - Sum of elements from current data sets.

RETURN
	Function status.

*****************************************************************/
HRESULT CL_Chloride(CPtrArray *datasets)
{
	HRESULT hr=S_OK;
	int number_of_sets;
	CArray<double,double> *tmp_set;
	double	tmp_sum;

	number_of_sets = datasets->GetSize();

	tmp_set = new CArray<double,double>;

	tmp_set->SetSize(((CArray<double,double> *)datasets->GetAt(0))->GetSize());

	for (int i=0; i<number_of_sets; i++)
	{
		for(int j=0; j<((CArray<double,double> *)datasets->GetAt(i))->GetSize(); j++)
		{
			tmp_sum = 0;

			for (int l=0; l<datasets->GetSize(); l++)
			{
				tmp_sum = tmp_sum + ((CArray<double,double> *)datasets->GetAt(l))->GetAt(j);
			}

			tmp_set->SetAt(j,tmp_sum);
		}
	}

	// Calculate average of data sets values.
	for (i=0; i<tmp_set->GetSize(); i++)
	{
		tmp_set->SetAt(i,tmp_set->GetAt(i) / (double)number_of_sets);
	}

	// Send processed data to caller.
	datasets->SetAt(0,tmp_set);

	// Delete the rest of the datasets.
	// Chloride results in only one data set.
	for (i=1; i<datasets->GetSize(); i++)
	{
		datasets->RemoveAt(i);
	}

	return hr;
}

