#include "stdafx.h"
#include <math.h>
#include "dboTestSampleType.H"
#include "Support.h"
#include "DbAccess.h"
#include "Loging.h"
#include "AnalyteTypeClass.h"

/*****************************************************************
HRESULT FromVARIANTToCStringArray(VARIANT *from, CStringArray *to)

	Convert a string from VARIANT to CStringArray.

-----------------------------------------------------------------
INPUT
	from - Source of string to change.
	to - Destination of string to change.

-----------------------------------------------------------------
OUTPUT
	Converted string.

-----------------------------------------------------------------
LOCAL
	hr - Result code.
	i - Safearray index.
	S_OK - Return system code.

-----------------------------------------------------------------
RETURN
	S_OK - Return code.

*****************************************************************/
HRESULT FromVARIANTToCStringArray(VARIANT *from, CStringArray *to)
{
	HRESULT			hr;
	LONG			i;

	//*** define the safearray ***
    SAFEARRAY FAR* psa_from = NULL;				// samples to plot.

	// set psa_plotList safearray pointer to the current plot list.
	psa_from = V_ARRAY(from);

	//*** define a BSTR to move the strings in and out ***
    BSTR bstr = NULL;

	i = 0;		// plotList index.
	// Get assigned values for current pool list.
	while ((hr = SafeArrayGetElement(psa_from, &i, &bstr)) == S_OK)
	{
		to->Add((CString)bstr);
		i++;
 	}

	return S_OK;
}

/*****************************************************************
HRESULT FromCStringToCStringArray(CString *from, CStringArray *to)

	Convert a string from CString to CStringArray.

-----------------------------------------------------------------
INPUT
	from - Source of string to change.
	to - Destination of string to change.

-----------------------------------------------------------------
OUTPUT
	Converted string.

-----------------------------------------------------------------
LOCAL
	tmp - Temporary string, to add to destination string.
	S_OK - Return system code.

-----------------------------------------------------------------
RETURN
	S_OK - Return code.

*****************************************************************/
HRESULT FromCStringToCStringArray(CString *from, CStringArray *to)
{
	char		seps[] = ",\n";
	char		*token;

	if(!(from->IsEmpty()))
	{
		TCHAR	*tchar_temp;
		
		tchar_temp = new TCHAR[from->GetLength() + 1];
		_tcscpy(tchar_temp, *from);

		token = strtok( tchar_temp, seps );
		while( token != NULL )
		{
			to->Add(token);
			// Get next from string.
			token = strtok( NULL, seps );
		}

		delete tchar_temp;
	}

	return S_OK;
}

/*****************************************************************
HRESULT FromCStringArrayToCString(CStringArray *from, CString *to)

	Convert a string from CStringArray to CString.

-----------------------------------------------------------------
INPUT
	from - Source of string to change.
	to - Destination of string to change.

-----------------------------------------------------------------
OUTPUT
	Converted string.

-----------------------------------------------------------------
LOCAL
	tmp - Temporary string, to add to destination string.
	S_OK - Return system code.

-----------------------------------------------------------------
RETURN
	S_OK - Return code.

*****************************************************************/
HRESULT FromCStringArrayToCString(CStringArray *from, CString *to)
{
	CString tmp;
	to->Empty();

	for (int i=0; i<from->GetSize(); i++)
	{
		tmp = from->GetAt(i);
		*to += tmp.GetBuffer(tmp.GetLength());
		*to += ",";
	}

	to->Delete(to->GetLength() -1, 1);

	return S_OK;
}

/*****************************************************************
HRESULT FromCArrayToCString(CArray *from, CString *to)

	Convert a string from CArray to CString.

-----------------------------------------------------------------
INPUT
	from - Source of string to change.
	to - Destination of string to change.

-----------------------------------------------------------------
OUTPUT
	Converted string.

-----------------------------------------------------------------
LOCAL
	tmp - Temporary string, to add to destination string.
	S_OK - Return system code.

-----------------------------------------------------------------
RETURN
	S_OK - Return code.

*****************************************************************/
HRESULT FromCArrayToCString(CArray<long,long> *from, CString *to)
{
	char	buf[128];

	to->Empty();
	
	for (int i=0; i<from->GetSize(); i++)
	{
		sprintf(buf,"%ld", from->GetAt(i));
		*to += buf;
		*to += ",";
	}

	to->Delete(to->GetLength() -1, 1);

	return S_OK;
}

/*
	Convert a type "from" to type of "to".
*/
HRESULT FromCArrayToCString(CArray<double,double> *from, CString *to)
{
	char	buf[128];

	to->Empty();
	
	for (int i=0; i<from->GetSize(); i++)
	{
		sprintf(buf,"%.15f", from->GetAt(i));
		*to += buf;
		*to += ",";
	}

	to->Delete(to->GetLength() -1, 1);

	return S_OK;
}

/*
	Convert a type "from" to type of "to".
*/
HRESULT FromCArrayToCString(CArray<int,int> *from, CString *to)
{
	char	buf[128];

	to->Empty();
	
	for (int i=0; i<from->GetSize(); i++)
	{
		sprintf(buf,"%d", from->GetAt(i));
		*to += buf;
		*to += ",";
	}

	to->Delete(to->GetLength() -1, 1);

	return S_OK;
}

/*
	Convert a type "from" to type of "to".
*/
HRESULT FromCStringArrayToCArray(CStringArray *from, 
								 CArray<double,double> *to)
{
	to->RemoveAll();
	
	for (int i=0; i<from->GetSize(); i++)
	{
		to->Add(atof(from->GetAt(i)));
	}

	return S_OK;
}

/*
	Convert a VARIANT type "from" to "to" of type "CArray<int,int>".
*/
HRESULT FromVARIANTToCArray(VARIANT *from, CArray<int,int> *to)
{
	HRESULT		hr;
	LONG		i;
	DWORD		lLCID;
	short		curValue;

	// Find a Language code, used by conversion functions. i.e. VarR8FromStr.
	lLCID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

	//*** define the safearray ***
    SAFEARRAY FAR* psa_from = NULL;				// samples to plot.

	// set psa_plotList safearray pointer to the current plot list.
	psa_from = V_ARRAY(from);

	//*** define a BSTR to move the strings in and out ***
    BSTR bstr = NULL;

	i = 0;		// VARIANT index.
	// Get current value from VARIANT.
	while ((hr = SafeArrayGetElement(psa_from, &i, &bstr)) == S_OK)
	{
		// Convert current VARIANT value to integer.
		hr = VarI2FromStr(bstr, lLCID, LOCALE_NOUSEROVERRIDE, &curValue);

		to->Add(curValue);
		i++;
 	}

	return S_OK;
}

/*
	Convert a VARIANT type "from" to "to" of type "CArray<double,double>".
*/
HRESULT FromVARIANTToCArray(VARIANT *from, CArray<double,double> *to)
{
	HRESULT		hr;
	LONG		i;
	DWORD		lLCID;
	double		curValue;

	// Find a Language code, used by conversion functions. i.e. VarR8FromStr.
	lLCID = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

	//*** define the safearray ***
    SAFEARRAY FAR* psa_from = NULL;				// samples to plot.

	// set psa_plotList safearray pointer to the current plot list.
	psa_from = V_ARRAY(from);

	//*** define a BSTR to move the strings in and out ***
    BSTR bstr = NULL;

	i = 0;		// VARIANT index.
	// Get current value from VARIANT.
	while ((hr = SafeArrayGetElement(psa_from, &i, &bstr)) == S_OK)
	{
		// Convert current VARIANT value to double value.
		hr = VarR8FromStr(bstr,lLCID,LOCALE_NOUSEROVERRIDE,&curValue);

		to->Add(curValue);
		i++;
 	}

	return S_OK;
}

/*****************************************************************
bool TestCurrentType(LONG calibrationID, CString sampleType)

	Test if current sample type "sampleType" was used in 
calibration with id "calibrationID".

-----------------------------------------------------------------
INPUT
	calibrationID - Id of calibration to get plot data for.
	sampleType - Sample type to find.

-----------------------------------------------------------------
OUTPUT
	Assigned values for current calibration.

-----------------------------------------------------------------
LOCAL
	hr - Return system code.
	db - Database access object.
	session - Session object to access db.
	buf[256] - Buffer for error message.
	found - Results.

-----------------------------------------------------------------
RETURN
	found.

*****************************************************************/
bool TestCurrentType(LONG calibrationID, CString sampleType)
{
	HRESULT		hr;
	CDataSource db;
	CSession	session;
	char		buf[256];
	bool		found;

	// Instantiate a command object, to test sample type.
	CCommand<CAccessor<CdboTestSampleTypeAccessor> > cmdTestSampleType;

	// Set properties for command.
	CDBPropSet cmdinit(DBPROPSET_ROWSET);

	// Open database.
	hr = OpenDatabase( &db, &session, "TestCurrentType" );
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"TestCurrentType\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from \"OpenDatabase\" function",buf);
		return false;
	}

	// < --------------- TEST CURRENT SAMPLE TYPE --------------- >
	hr = cmdTestSampleType.Create(session, _T("{ ? = CALL dbo.TestSampleType;1 (?,?) }"));
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		sprintf(buf,"Function \"TestCurrentType\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Create command for \"TestSampleType\" stored procedure",buf);
		return false;
	}

	// Set search criterium. Stored procedure's in variables.
	// Calibration identifier and sample type.
	cmdTestSampleType.m_calibrationID = calibrationID;

	_tcsncpy(cmdTestSampleType.m_type, sampleType,
 		sizeof(cmdTestSampleType.m_type) /
 		sizeof(cmdTestSampleType.m_type[0]));

	// Get sample names.
	hr = cmdTestSampleType.Open(&cmdinit,NULL,true);
	if (FAILED(hr))
	{
		// Close the database.
		db.Close();

		sprintf(buf,"Function \"TestCurrentType\", file %s, line %d", __FILE__, __LINE__);
		LogInterfaceErrorMessage(hr,"Call \"TestSampleType\" stored procedure",buf);
		return false;
	}

	if (cmdTestSampleType.m_RETURNVALUE == 1)
	{
		found = true;
	}
	else
	{
		found = false;
	}

	cmdTestSampleType.Close();
	// </ --------------- TEST CURRENT SAMPLE TYPE --------------- >
	
	// Close the database.
	db.Close();

	return found;
}

/************************************************************
int TimesElementAppearsInList(CStringArray *serialNumberList, 
							 CString element)

	Find number of unique serial numbers.

-------------------------------------------------------------
 INPUT
	serialNumberList - List of serial numbers.

-------------------------------------------------------------
 OUTPUT
	Boolean decision, dependent on search results.

-------------------------------------------------------------
 LOCAL
	timesFound - Number of times a value exists.
	k - Index of current comparison.
	s - Length of original list.

-------------------------------------------------------------
 RETURN
	 Number of times a value is in list.

*************************************************************/
int TimesElementAppearsInList(CStringArray *serialNumberList, 
							 CString element)
{
	int timesFound;
	int	k;
	int	s;

	timesFound = 0;
	k = 0;
	
	s = serialNumberList->GetSize();

	for (int i=0; i<s; i++)
	{
		if (serialNumberList->GetAt(k).CompareNoCase(element) == 0)
		{
			serialNumberList->RemoveAt(k);
			timesFound++;
		}
		else
		{
			k++;
		}
	}

	return timesFound;
}

/************************************************************
int NumberOfUniqueValues(CStringArray *serialNumberList)

	Find number of unique serial numbers.

-------------------------------------------------------------
 INPUT
	serialNumberList - List of serial numbers.

-------------------------------------------------------------
 OUTPUT
	Boolean decision, dependent on search results.

-------------------------------------------------------------
 LOCAL
	serialNumber - Current serial number.
	timesFound - Number of times current serial number was found.
	uniqueValues - Unique values counter.

-------------------------------------------------------------
 RETURN
	 Number of unique values in list.

*************************************************************/
int NumberOfUniqueValues(CStringArray *serialNumberList)
{
	CString serialNumber;
	int timesFound;
	int	uniqueValues;

	uniqueValues = 0;
	timesFound = 0;
	while (serialNumberList->GetSize() > 0)
	{
		serialNumber = serialNumberList->GetAt(0);

		timesFound = TimesElementAppearsInList(serialNumberList, serialNumber);

		if (timesFound > 0)
		{
			uniqueValues++;
		}
	}

	return uniqueValues;
}

/************************************************************
bool TopFactor(	CArray<double,double> *assignedValues, 
				double topFactorMinValue)

	Find top factors in given assigned values list.

-------------------------------------------------------------
 INPUT
	assignedValues - Assigned values list.
	topFactorMinValue - Minimum value allowed in top factors.

-------------------------------------------------------------
 OUTPUT
	Boolean decision, dependent on search results.

-------------------------------------------------------------
 LOCAL
	i - Assigned values list index.
	numberTopFactors - Top factor counter.

-------------------------------------------------------------
 RETURN
	Search results.

*************************************************************/
bool TopFactor(CArray<double,double> *assignedValues, 
			   double topFactorMinValue)
{
	int	i = 0;
	int	numberTopFactors = 0;

	// Find top factors.
	while (i<assignedValues->GetSize())
	{
		if (assignedValues->GetAt(i) > topFactorMinValue)
		{
			numberTopFactors++;
		}
		i++;
	}

	if (numberTopFactors < 2)
	{
		return false;
	}

	return true;
}

/************************************************************
void LoadCArrayToVector(CArray<double,double> *varStruct, 
						dVector vecStruct)

	Copy data in CArray,MFC, format to dVector,OptiVec, format.

-------------------------------------------------------------
 INPUT
	*varStruct - Structure with CArray format.
	vecStruct - Structure with OptiVec format.

-------------------------------------------------------------
 OUTPUT
	vecStruct - filled with a copy of varStruct.

-------------------------------------------------------------
 LOCAL
	i - Index to both structures.

-------------------------------------------------------------
 RETURN
	NONE.

*************************************************************/
void LoadCArrayToVector(CArray<double,double> *varStruct, 
						dVector vecStruct)
{
	// Get current data value. Put it in vector.
	for (int i=0; i<varStruct->GetSize(); i++)
	{
		vecStruct[i] = varStruct->GetAt(i);
	}
}

/************************************************************
bool Subset(CStringArray *searchFor,CStringArray *searchIn)

	Find a list in another list.

-------------------------------------------------------------
 INPUT
	searchFor - String to search for in "searchIn".
	searchIn - Where to find list "searchFor".

-------------------------------------------------------------
 OUTPUT
	Boolean decision, dependent on search results.

-------------------------------------------------------------
 LOCAL
	i - Index to search in searchFor list.
	j - Index to search in searchIn list.
	found - Loop control for searchIn list.
	o_found - Loop control for searchFor list.

-------------------------------------------------------------
 RETURN
	true | false, depend on search results.

*************************************************************/
bool Subset(CStringArray *searchFor,CStringArray *searchIn)
{
	int j,i;
	bool found,o_found;

	i=0;
	o_found=true;

	while(i<searchFor->GetSize() && o_found)
	{
		j=0;
		found=false;

		while(j<searchIn->GetSize() && !found)
		{
			if (searchFor->GetAt(i).CompareNoCase(searchIn->GetAt(j)) == 0)
			{
				found = true;
			}

			j++;
		}

		if (!found)
		{
			o_found = false;
		}

		i++;
	}

	return found;
}

/*
	Find string "searchFor" in "searchIn" array of strings.
*/
int FindStringPosition(CString *searchFor, CStringArray *searchIn)
{
	int i;

	i=0;

	while(i<searchIn->GetSize())
	{
		if (searchFor->CompareNoCase(searchIn->GetAt(i)) == 0)
		{
			return i;
		}
		i++;
	}

	return -1;
}

/*
	Set "union_ized" array to union of arrays in "to_unionize".
*/
HRESULT SetUnion(CPtrArray *to_unionize,
				 CArray<int,int> *union_ized)
{
	HRESULT hr=S_OK;

	union_ized->RemoveAll();

	for(int i=0; i<to_unionize->GetSize(); i++)
	{
		for(int k=0; k<((CArray<int,int> *)to_unionize->GetAt(i))->GetSize(); k++)
		{
			union_ized->Add(((CArray<int,int> *)to_unionize->GetAt(i))->GetAt(k));
		}
	}

	return hr;
}

/*
	Does string "find_in" contain sub-string "to_fing"?
*/
bool FindString(CString *to_find, CStringArray *find_in)
{
	bool ret_code = false;
	int	i;

	i=0;
	while ((i<find_in->GetSize()) && !ret_code)
	{
		if (to_find->CompareNoCase(find_in->GetAt(i)) == 0)
		{
			ret_code = true;
		}

		i++;
	}

	return ret_code;
}

/*
	Which sample size in "samplesizes" does the "excludedIndex" corresponds to?
*/
int TransformExcludedIndex(CArray<int,int> *samplesizes, int excludedIndex)
{
	int sum;
	int k;
	bool found;

	sum = 0;
	k = 0;
	found = false;
	while (!found && (k<(samplesizes->GetSize() - 1)))
	{
		sum = sum + samplesizes->GetAt(k);

		if (excludedIndex < sum)
		{
			found = true;
		}
		else
		{
			k++;
		}
	}

	return k;
}

/*
	Set a given matrix, "matrixToSet" to value in "setto".
*/
void SetMatrix(dMatrix matrixToSet, ui size, double setto)
{
	for (ui r=0; r<size; r++)
	{
		for (ui c=0; c<size; c++)
		{
			matrixToSet[r][c] = setto;
		}
	}
}

/*
					 (sum((X[i])^2) - (sum(X[i]))^2)/n
	SampleVariance = ---------------------------------
								 n - 1

	Above formula from:
	Introductory Statistics
	Fifth Edition
	Neil A. Weiss
	FORMULA 3.1,	Page: 149
*/
double SampleVariance(dVector currentData, ui sampleSize)
{
	double sum_of_squares_of_vector_elements;
	double square_of_sum_of_vector_elements;
	double numerator;

	// Function Sum-of-squares
	// Description ssq = sum( X[i]^2 )
	// Return value sum of the squares of the vector elements.
	sum_of_squares_of_vector_elements = VD_ssq( currentData, sampleSize );

	// double VD_sum( dVector X, ui size );
	// Description sum(X[i])
	square_of_sum_of_vector_elements  = VD_sum( currentData, sampleSize );

	// Description (sum(X[i]))^2
	square_of_sum_of_vector_elements *= square_of_sum_of_vector_elements;

	numerator = sum_of_squares_of_vector_elements - 
				(square_of_sum_of_vector_elements / sampleSize);

	return ( numerator / (sampleSize - 1) );
}

/*
	SampleStandardDeviation = sqrt(SampleVariance)
*/
double SampleStandardDeviation(dVector currentData, ui sampleSize)
{
	return ( sqrt(SampleVariance(currentData, sampleSize)) );
}
