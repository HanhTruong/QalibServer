/*******************************************************************

FILENAME:	HelperFunctions.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declaration for helper functions.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	HelperFunctions.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include <afx.h>

class CSortStringArray : public CStringArray {
public:
   void Sort();
private:
   BOOL CompareAndSwap(int pos);
};

enum {NO_REMOVAL=1, MANUAL, HISTORIC, NO_MEAN_NO_STD, MEAN_NO_STD, NO_MEAN_STD, MEAN_STD};

void CopyCArrayToVector(CArray<int,int> *from, liVector to);
void CopyCArrayToVector(CArray<double,double> *from, dVector to);
void ExpandAssignedValuesIntoVector(CArray<double,double> *assignedValues, 
									dVector assignedValuesVector,
									liVector sampleSizes);

