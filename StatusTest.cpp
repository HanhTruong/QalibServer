#include "stdafx.h"
#include <math.h>
#include <afxtempl.h>
#include "Status.h"
#include "Loging.h"
#include "Utils.h"
#include "Support.h"
#include "BusinessRulesProcess.h"

void SampleDataIndexRange(CArray<int,int> *samplesizes,
						  CArray<int,int> *lowIndex,
						  CArray<int,int> *highIndex);

void RemoveExcludedSamples(CArray<int,int> *excludeSamplesIndecies,
						   CArray<int,int> *lowIndex,
						   CArray<int,int> *highIndex,
						   CPtrArray *datasets,
						   CPtrArray *statussets,
   						   CArray<long,long> *indexingList,
						   CStringArray *sampleids,
						   CArray<int,int> *samplesizes);

void RemoveExcludedData(CPtrArray *datasets,
						CPtrArray *statussets,
						CArray<long,long> *indexingList,
						CStringArray *sampleids,
						CArray<int,int> *samplesizes)
{
	int		k,tmp;
	CArray<int,int> excludeSamplesIndices;
	CArray<int,int> excludeValuesIndices;
	CArray<int,int> l_samplesizes;
	CArray<int,int> lowIndices;
	CArray<int,int> highIndices;
	CArray<int,int> numberExcludedFromEachSample;
	CArray<int,int> *current_statusset;
	CArray<double,double> *current_dataset;

	l_samplesizes.RemoveAll();
	l_samplesizes.Copy(*samplesizes);

	excludeValuesIndices.RemoveAll();

	// Find which samples have excluded values.
	for(int i=0; i<((CArray<int,int> *)statussets->GetAt(0))->GetSize(); i++)
	{
//		if (((CArray<int,int> *)statussets->GetAt(0))->GetAt(i) == EXCLUDED)
		if ( (((CArray<int,int> *)statussets->GetAt(0))->GetAt(i) == EXCLUDED) ||
			 (((CArray<int,int> *)statussets->GetAt(0))->GetAt(i) == MOVED_AND_EXCLUDED) )
		{
			excludeValuesIndices.Add(i);
			k = TransformExcludedIndex(samplesizes,i);
			l_samplesizes.SetAt(k,l_samplesizes.GetAt(k) - 1);
		}
	}

	excludeSamplesIndices.RemoveAll();

	for (i=samplesizes->GetSize()-1; i>=0; i--)
	{
		if ((((float)l_samplesizes.GetAt(i)/(float)samplesizes->GetAt(i)) * 100.0) < 67.0)
		{
			excludeSamplesIndices.Add(i);
		}
		else
		{
			tmp = samplesizes->GetAt(i) - l_samplesizes.GetAt(i);
			if (tmp > 0)
			{
				// Remove values that have been excluded.
				// Not enough to remove sample.
				for (int m=0; m<statussets->GetSize(); m++)
				{
					current_statusset = (CArray<int,int> *)statussets->GetAt(m);
					current_dataset = (CArray<double,double> *)datasets->GetAt(m);

					for (int l=0; l<tmp; l++)
					{
						current_statusset->RemoveAt(excludeValuesIndices.GetAt(excludeValuesIndices.GetSize()-1));
						current_dataset->RemoveAt(excludeValuesIndices.GetAt(excludeValuesIndices.GetSize()-1));
						indexingList->RemoveAt(excludeValuesIndices.GetAt(excludeValuesIndices.GetSize()-1));
						excludeValuesIndices.RemoveAt(excludeValuesIndices.GetSize()-1);
					}
				}
			}
		}
	}

	// Remove samples that had too many values excluded.
	if (excludeSamplesIndices.GetSize() > 0)
	{
		SampleDataIndexRange(samplesizes,&lowIndices,&highIndices);

		RemoveExcludedSamples(&excludeSamplesIndices,
							  &lowIndices,&highIndices,
							  datasets,statussets,indexingList,
							  sampleids,&l_samplesizes);
	}

	samplesizes->RemoveAll();
	samplesizes->Copy(l_samplesizes);
}

void RemoveExcludedSamples(CArray<int,int> *excludeSamplesIndices,
						   CArray<int,int> *lowIndices,
						   CArray<int,int> *highIndices,
						   CPtrArray *datasets,
						   CPtrArray *statussets,
						   CArray<long,long> *indexingList,
						   CStringArray *sampleids,
						   CArray<int,int> *samplesizes)
{
	CArray<int,int> *current_statusset;
	CArray<double,double> *current_dataset;

	for (int i=0; i<statussets->GetSize(); i++)
	{
		current_statusset = (CArray<int,int> *)statussets->GetAt(i);
		current_dataset = (CArray<double,double> *)datasets->GetAt(i);

		for (int j=excludeSamplesIndices->GetSize()-1; j>=0; j--)
		{
			sampleids->RemoveAt(excludeSamplesIndices->GetAt(j));
			samplesizes->RemoveAt(excludeSamplesIndices->GetAt(j));

			for (int k=highIndices->GetAt(excludeSamplesIndices->GetAt(j)); k>=lowIndices->GetAt(excludeSamplesIndices->GetAt(j)); k--)
			{
				current_statusset->RemoveAt(k);
				current_dataset->RemoveAt(k);
				indexingList->RemoveAt(k);
			}
		}
	}
}

void SampleDataIndexRange(CArray<int,int> *samplesizes,
						  CArray<int,int> *lowIndex,
						  CArray<int,int> *highIndex)
{
	int low,high;

	low = 0;
	high = 0;

	for (int i=0; i<samplesizes->GetSize(); i++)
	{
		low = high;

		lowIndex->Add(low);
		high = high + samplesizes->GetAt(i);
		highIndex->Add(high - 1);

		low = high + 1;
	}
}

HRESULT NumberExcluded(LONG calibrationID,
					   CString mode,
					   CStringArray *sampleids,
					   VARIANT *error_msg)
{
	HRESULT hr=S_OK;
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
		
		if ((double)(sampleids->GetSize()) < requiredNumberOfCalibrators)
		{
			hr = 0x80040E21;
			UserErrorMessage("3", "Too many samples excluded",error_msg);
			return hr;
		}
	}

	return hr;
}
