#include "stdafx.h"
#include <math.h>
#include <VDstd.h>
#include "Utils.h"
#include "Status.h"

void OutliersUsingStandardDeviation(CArray<int,int> *ptrCalibrationStatus,
									dVector currentData,
									int *ptrStatusIndex,
									int *ptrDataIndex,
									int sampleSize,
									double lower,
									double upper)
{
	double	mean,ssdv;

	mean = VD_mean( currentData, sampleSize ); 

	// Sample standard deviation.
	ssdv = SampleStandardDeviation(currentData,sampleSize);

	// Find outliers.
	for (int k=0; k<sampleSize; k++)
	{
		// Test limits.
		if ((fabs(currentData[k]) < (fabs(mean) - (fabs(lower)*ssdv) )) ||
			(fabs(currentData[k]) > (fabs(mean) + (fabs(upper)*ssdv) ))	   )
		{
			ptrCalibrationStatus->SetAt(*ptrStatusIndex,OUTLIER);
		}

		(*ptrStatusIndex)++;
   	}
}