#include <VDstd.h> 
#include <MDstd.h> 

HRESULT FromCStringToCStringArray(CString *from, CStringArray *to);
HRESULT FromCStringArrayToCString(CStringArray *from, CString *to);
HRESULT FromVARIANTToCStringArray(VARIANT *from, CStringArray *to);
bool TestCurrentType(LONG calibrationID, CString sampleType);
int TimesElementAppearsInList(CStringArray *serialNumberList, CString element);
int NumberOfUniqueValues(CStringArray *serialNumberList);
bool TopFactor(	CArray<double,double> *assignedValues, double topFactorMinValue);

void LoadCArrayToVector(CArray<double,double> *varStruct, 
						dVector vecStruct);

HRESULT FromCArrayToCString(CArray<double,double> *from, CString *to);
HRESULT FromCArrayToCString(CArray<long,long> *from, CString *to);
HRESULT FromCArrayToCString(CArray<int,int> *from, CString *to);
HRESULT FromVARIANTToCArray(VARIANT *from, CArray<int,int> *to);
HRESULT FromVARIANTToCArray(VARIANT *from, CArray<double,double> *to);
HRESULT FromCStringArrayToCArray(CStringArray *from, CArray<double,double> *to);

bool Subset(CStringArray *searchFor,CStringArray *searchIn);
int FindStringPosition(CString *searchFor, CStringArray *searchIn);

HRESULT SetUnion(CPtrArray *to_unionize,CArray<int,int> *union_ized);

bool FindString(CString *to_find, CStringArray *find_in);

int TransformExcludedIndex(CArray<int,int> *samplesizes, int excludedIndex);

void SetMatrix(dMatrix mClear, ui size, double setto);
double SampleVariance(dVector currentData, ui sampleSize);
double SampleStandardDeviation(dVector currentData, ui sampleSize);
