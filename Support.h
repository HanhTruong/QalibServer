/*******************************************************************

FILENAME:	Support.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declarations for support functions.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:
	Not applicable.

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	Support.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include <MDstd.h> 
#include <VDstd.h> 
#include <VUstd.h> 
#include <VLIstd.h> 

void Rate( dVector Y, dVector X, ui size );

HRESULT CorrelationCoefficient(dVector X, 
							   dVector Y, 
							   ui sizeYX,
							   CString algorithm,
							   double *ccoe,
							   VARIANT *error_msg);

HRESULT InsertSigmoidalFitParameters(LONG calibrationID, 
									 dVector FitPars,
									 ui size,
									 double ccoe,
									 double chisquared,
									 double bartlett,
									 double TopError,
									 double BottomError,
									 double SlopeError,
									 double InterceptError);

HRESULT GetWeightMethodDB( LONG calibrationID, CString &wMethod );

HRESULT GoodnessTests( LONG calibrationID, CStringList &TestList );

HRESULT PutCalculatedAssignedValuesInDb(LONG calibrationID,
										CString	sampleType,
										dVector	assignedCoef,
										double	ccoe,
										dVector	vec_AssignedValues,
										dVector	vec_calcAssignedValues,
										ui		sizeallplotData,
										VARIANT *error_msg);

void LoadVariantToVector(VARIANT *varStruct, dVector vecStruct);

//LONG VariantLength(VARIANT *varToSize, VARIANT *error_msg);
HRESULT VariantLength(VARIANT *varToSize, ui *varLength);

double BartlettsTest(dVector Rate,
					 ui		 size,
					 liVector poolsizes,
					 ui		 poolsizelen);

HRESULT GetGraphParameters(LONG calibrationID,
						   BSTR calculatedAssignedType,
						   double *ccoe,
						   double *slope,
						   double *intercept);

HRESULT AssignFitParameters(LONG calibrationID, 
							VARIANT *newFitPars,
							VARIANT *error_msg);

HRESULT FindCurrentAlgorithmType(LONG calibrationID, 
								 CString *algorithmType);

HRESULT GetPlotData(LONG calibrationID, 
 					CString sampleType, 
 					CStringArray *plotList, 
 					CArray<double,double> *plotData,
					CArray<int,int> *noOutliersNumberOfValues);

HRESULT GetSampleDataToPlot(LONG calibrationID,
							CStringArray *plotList,
							CArray<double,double>  *plotData);

HRESULT BuildListOfSamplesToPlot(LONG calibrationID,
								 CString sampleType,
								 CStringArray *plotList);

HRESULT GetPlotSampleSizes(BSTR				calibrationDate, 
						   CStringArray		*plotList, 
						   CArray<int,int>	*totalNumberOfValues,
						   CArray<int,int>	*noOutliersNumberOfValues);

HRESULT GetAssignedValuesToPlot(LONG calibrationID,
								CStringArray *plotList, 
								CArray<double,double> *varAssignedValues, 
								CArray<int,int> *plotSampleSizes,
								VARIANT *error_msg);

HRESULT GetCalibrationIDAssignedValues(LONG calibrationID,
									   CArray<double,double> *assignedValues,
									   VARIANT *error_msg);

HRESULT GetCalibrationIDSamples(LONG calibrationID,
						  CStringArray *sampleIdList);

HRESULT GetCalibrationIDSampleSizes(LONG calibrationID,
									CString restriction,
									CArray<int,int> *sampleSizesList);

HRESULT GetSizesOfAListOfSamples(LONG calibrationID,
								 CStringArray *sampleIdList,
								 CString outliers,
								 CArray<int,int> *sampleSizesList);

void SetFitParametersLimitsResults(TCHAR *parameterLimits,
								   dVector Lower,
								   dVector Upper,
								   CString algorithm);

HRESULT GetCalibrationIDDiluent(LONG calibrationID,
								CString *diluent);

HRESULT GetSampleIdsFromType(LONG calibrationID,
							 CString type,
							 CStringArray *sampleIdsList);

HRESULT GetChemistryNameGivenCalibrationID(LONG calibrationID,
										   CString *ChemistryName);

HRESULT BuildListOfRequiredSamples(LONG calibrationID,
								   CString chemistry,
								   CString sampleType,
								   CString calibrationSpecies,
								   CStringArray *requiredSamplesList,
								   VARIANT *error_msg);

HRESULT ListOfSamplesToUseInCalibration(LONG calibrationID,
										CStringArray *samplesList,
										VARIANT *error_msg);

HRESULT RemoveSamplesWithTooManyOutliers(LONG calibrationID, CStringArray *samplesList);

bool TestSampleStatus(CArray<int,int> *status, int numberOfSets);
