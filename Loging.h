/*******************************************************************

FILENAME:	Loging.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declarations of loging functions.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	Loging.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
//#include <fstream>
#include <fstream>
#include <iostream>
#include <iostream.h>
#include <VDstd.h> 
#include <MDstd.h> 

//typedef std::basic_ofstream<char> log_file;
typedef std::basic_ofstream<char, std::char_traits<char> > log_file;

static char	logfilename[256];
static char	curDate[128];

extern log_file LogFile;

void InitializeLogFile(char *InitialMessage);
void CleanUpLogFile(char *CloseMessage);
void CurrentSystemDate(char *curDate);
void DisplayVariant(VARIANT *displayVariant, CString message);
void DisplayVector(dVector vecDisp, ui size, CString message);
void DisplayVector(uVector vecDisp, ui size, CString message);
void DisplayVector(liVector vecDisp, ui size, CString message);

HRESULT ErrorMessage(HRESULT phr, 
					 CString error_source, 
					 CString severity, 
					 CString error_description, 
					 VARIANT *error_msg);

HRESULT OKMessage(VARIANT *error_msg);


void UserErrorMessage(CString severity,
					  CString error_description,
					  VARIANT *error_msg);

void LogInterfaceErrorMessage(HRESULT phr, 
					 CString what, 
					 CString where);

void LogErrorMessage(HRESULT phr, 
					 CString what, 
					 CString where);

void DisplayCArray(CArray<double,double> *displayVariant, CString source);
void DisplayCArray(CArray<int,int> *displayVariant, CString source);
void DisplayCArray(CArray<long,long> *displayVariant, CString source);
void DisplayCArray(CArray<CString,CString> *displayVariant, CString source);
void DisplayCStringArray(CStringArray *displayCStringArray, CString source);

void DisplayMatrix(dMatrix mDisp, ui size, CString message);
void DisplayVector(iVector vecDisp, ui size, CString message);
void DisplayCString(CString *displayCString, CString source);

void DisplayStatusSets(CPtrArray *statussets, CString source);
void DisplayDataSets(CPtrArray *datasets, CString source);

void CleanUpLogFile();
bool InitializeLogFile();
void DisplayCString(CString *displayCString);
