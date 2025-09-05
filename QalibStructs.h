/*******************************************************************

FILENAME:	QalibStructs.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declarations of user defined structures.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	QalibStructs.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
typedef struct tagHeader
{
	int	listSize;
	CString sampleid;
	struct tagHeader *pNextListSize;
}HEADER, *PHEADER;

typedef CList<double,double> valueList;
typedef CMap< CString, LPCSTR, valueList*, valueList*& > CalibrationDataMap;

typedef CArray<int,int> statusList;
typedef CMap< CString, LPCSTR, statusList*, statusList*& > CalibrationDataStatusMap;

typedef CMap< CString, LPCSTR, CString, CString > SpeciesMap;

typedef CMap< CString, LPCSTR, CString, CString > TypesMap;

typedef CMap< CString, LPCSTR, CString, CString > SampleDatesMap;

typedef CMap< CString, LPCSTR, int, int > LevelMap;

typedef CMap< CString, LPCSTR, double , double > AssignedValueMap;
