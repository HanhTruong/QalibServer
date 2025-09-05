/*******************************************************************

FILENAME:	HelperFunctions.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Functions to carry out Calibration calculations.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:
	void CSortStringArray::Sort()
	BOOL CSortStringArray::CompareAndSwap(int pos)
	double ddslope(double AV, double intercept, double slope, double bottom, double top)
	double ddintercept(double AV, double intercept, double slope, double bottom, double top)
	double ddbottom(double AV, double intercept, double slope)
	double ddtop(double AV, double intercept, double slope)

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	HelperFunctions.cpp
		  
--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <string.h>
#include <math.h> 
#include <time.h>

#include "Loging.h"
#include "Support.h"
#include "globals.h"
#include "HelperFunctions.h"
#include "QalibStructs.h"

extern ui numberofsamples;

/*****************************************************************
void CSortStringArray::Sort()
	Sort method for an array strings, used for list of sample ids.
	Sort by bubble sort. SHOULD WORK FOR SMALL LISTS.
INPUT
	none.

OUTPUT
	CStringArray of CSortStringArray object will be sorted.

LOCAL
	dNotDone - Control array parsing.
*****************************************************************/
void CSortStringArray::Sort()
{
   BOOL bNotDone = TRUE;

   while (bNotDone)
   {
      bNotDone = FALSE;
      for(int pos = 0;pos < GetUpperBound();pos++)
         bNotDone |= CompareAndSwap(pos);
   }
}

/*****************************************************************
BOOL CSortStringArray::CompareAndSwap(int pos)
	Compares two consecutive positions within the string array.
	Moves them accordingly. Implementing a bubble sort.

INPUT
	none.

OUTPUT
	temp - Third location needed to switch two elements.
	posFirst - Current position being compared.
	posNext - Position of element being compared to.

LOCAL
	dNotDone - Control array parsing.
*****************************************************************/
BOOL CSortStringArray::CompareAndSwap(int pos)
{
   CString temp;
   int posFirst = pos;
   int posNext = pos + 1;

   if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
   {
      temp = GetAt(posFirst);
      SetAt(posFirst, GetAt(posNext));
      SetAt(posNext, temp);
      return TRUE;

   }
   return FALSE;
}

void CopyCArrayToVector(CArray<int,int> *from, liVector to)
{
	for (int i=0; i<from->GetSize(); i++)
	{
		to[i] = from->GetAt(i);
	}
}
		
void CopyCArrayToVector(CArray<double,double> *from, dVector to)
{
	for (int i=0; i<from->GetSize(); i++)
	{
		to[i] = from->GetAt(i);
	}
}

void ExpandAssignedValuesIntoVector(CArray<double,double> *assignedValues, 
									dVector assignedValuesVector,
									liVector sampleSizes)
{
	int l=0;

	for (int i=0; i<assignedValues->GetSize(); i++)
	{
		for (int k=0; k<PoolSizes[i]; k++)
		{
			// Add current sample assigned value.
			XExp[l] = assignedValues->GetAt(i);
			l++;
		}
	}
}