/*******************************************************************

FILENAME: Loging.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Loging utilities.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:
	void InitializeLogFile(char *InitialMessage);
	void CleanUpLogFile(char *CloseMessage);
	void CurrentSystemDate(char *curDate);
	void DisplayVariant(VARIANT *displayVariant, CString message)
	void DisplayVector(dVector vecDisp, ui size, CString message);

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	Loging.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include "Loging.h"

log_file LogFile;

/************************************************************
void InitializeLogFile(char *InitialMessage)

	Initializes log file.
	Open log file.
	Add initial message to log file.
	Add current date to log file.

-------------------------------------------------------------
 INPUT
	InitialMessage - Message to be sent to log file.

-------------------------------------------------------------
 OUTPUT
	Log file setup for logging.

-------------------------------------------------------------
 LOCAL
	None.

-------------------------------------------------------------
 RETURN
	None.

*************************************************************/
void InitializeLogFile(char *InitialMessage)
{
	char *new_line = "\n";

	strcpy(logfilename,"C:\\Calibration\\Logs\\LogFile.out");

	LogFile.clear();

	// < --------- basic_ofstream --------- >
	LogFile.open(logfilename,std::ios::out | std::ios::app);

	CurrentSystemDate(curDate);

	LogFile.write("\n***************************************\n", strlen("\n***************************************\n"));

	LogFile.write(curDate, strlen(curDate));
	LogFile.write(new_line, strlen(new_line));

	LogFile.write(InitialMessage, strlen(InitialMessage));
	LogFile.write(new_line, strlen(new_line));

	// </ --------- basic_ofstream --------- >
}

bool InitializeLogFile()
{
	strcpy(logfilename,"C:\\Calibration\\Logs\\LogFile.out");

	LogFile.clear();

	// < --------- basic_ofstream --------- >
	LogFile.open(logfilename,std::ios::out | std::ios::app);

	if (LogFile.is_open())
	{
		return true;
	}
	else
	{
		return false;
	}
	// </ --------- basic_ofstream --------- >
}

void CleanUpLogFile()
{
	if (LogFile.is_open())
	{
		LogFile.flush();
		LogFile.close();
	}
}

/**************************************
void CleanUpLogFile(char *CloseMessage)

	Add closing messages to log file.
	Close file.

---------------------------------------
 INPUT
	CloseMessage - End error message.

---------------------------------------
 OUTPUT
	Logging file updated.

---------------------------------------
 LOCAL
	None.

---------------------------------------
 RETURN
	None.

**************************************/
void CleanUpLogFile(char *CloseMessage)
{
	char *new_line = "\n";

	LogFile.write(CloseMessage, strlen(CloseMessage));

	LogFile.write("\n***************************************", strlen("\n***************************************"));

	LogFile.write(new_line, strlen(new_line));

	LogFile.flush();
	LogFile.close();
}

/************************************************************
HRESULT ErrorMessage(HRESULT phr, 
					 CString error_source, 
					 CString severity, 
					 CString error_description, 
					 VARIANT *error_msg)

	Display current error messages.

-------------------------------------------------------------
 INPUT
	phr - Error code to process.
	error_source - Where error occured.
	severity - Severity of error.
	error_description - Type of error.
	*error_msg - Error message buffer.

-------------------------------------------------------------
 OUTPUT
	Error messages.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	k - Index for SAFEARRAYs.
	*ptrMsgText - Buffer to format error message.
	partialMsg - Buffer to format error message as a CString.
	psa_error_msg - SAFEARRAY pointer to set error message.
	error_sabound[1] - Error message array boundary.

-------------------------------------------------------------
 RETURN
	Error code.

*************************************************************/
HRESULT ErrorMessage(HRESULT phr, CString error_source, CString severity, 
					 CString error_description, VARIANT *error_msg)
{
	HRESULT	hr;
	char	*new_line = "\n";
	long	k;
	char	*ptrMsgText;
	CString	partialMsg;

    SAFEARRAY FAR* psa_error_msg = NULL;
    SAFEARRAYBOUND error_sabound[1];

	// Set size of error array to pass to client.
	error_sabound[0].cElements = 2;
	error_sabound[0].lLbound = 0;
	psa_error_msg = SafeArrayCreate(VT_BSTR, 1, error_sabound);

	k=0;
	hr = SafeArrayPutElement(psa_error_msg, &k, severity.AllocSysString());
	k++;
	hr = SafeArrayPutElement(psa_error_msg, &k, error_description.AllocSysString());

	// Set the return error message, array of VT_BSTR.
	// VT_BSTR, strings. Type of data to return in perror_msg.
	// Which values are editable. i.e. can be changed.
	V_VT(error_msg) = VT_ARRAY | VT_BSTR;
	// Set return Editable array to contain error messages.
	V_ARRAY(error_msg) = psa_error_msg;

	// Build error message.
	partialMsg.FormatMessage(_T("Error: Source %1!s!. %2!s!."), 
		error_source.GetBuffer(error_source.GetLength()),
		error_description.GetBuffer(error_description.GetLength()));

	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(ptrMsgText);
	}
	else
	{
		LogFile.write(ptrMsgText, strlen(ptrMsgText));
		LogFile.write(new_line, strlen(new_line));			
	}

	partialMsg.FormatMessage(_T("\tFailure code: %1!x! \t%2!d!"), phr, phr);
	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	CleanUpLogFile(ptrMsgText);

	return phr;
}

void LogInterfaceErrorMessage(HRESULT phr, CString what, CString where)
{
	char	*new_line = "\n";
	char	*ptrMsgText;
	CString	partialMsg;

	IErrorInfo *	pErrorInfo;
	HRESULT			lhr;
	GUID			pGUID;
	BSTR			pBstrSource;
	BSTR			pBstrDescription;
	char			buffer[256];

	partialMsg.Empty();

	partialMsg += "What - ";
	partialMsg += what + "\n";
	partialMsg += "Where - ";
	partialMsg += where + "\n";

	partialMsg += "GUID - ";

	GetErrorInfo(0,&pErrorInfo);

	lhr = pErrorInfo->GetGUID(&pGUID);
	sprintf(buffer,"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
			pGUID.Data1,
			pGUID.Data2,
			pGUID.Data3,
			pGUID.Data4[0],
			pGUID.Data4[1],
			pGUID.Data4[2],
			pGUID.Data4[3],
			pGUID.Data4[4],
			pGUID.Data4[5],
			pGUID.Data4[6],
			pGUID.Data4[7]);
		
	partialMsg += (CString)buffer;

	partialMsg += "Source - ";
	lhr = pErrorInfo->GetSource(&pBstrSource);
	partialMsg += (CString)pBstrSource;
	partialMsg += "\n";

	partialMsg += "Description - ";
	lhr = pErrorInfo->GetDescription(&pBstrDescription);
	partialMsg += (CString)pBstrDescription;
	partialMsg += "\n";

	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(ptrMsgText);
	}
	else
	{
		LogFile.write(ptrMsgText, strlen(ptrMsgText));
		LogFile.write(new_line, strlen(new_line));			
	}

	partialMsg.FormatMessage(_T("\tFailure code: %1!x! \t%2!d!"), phr, phr);
	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	CleanUpLogFile(ptrMsgText);
}

void LogErrorMessage(HRESULT phr, CString what, CString where)
{
	char	*new_line = "\n";
	char	*ptrMsgText;
	CString	partialMsg;

	partialMsg.Empty();

	partialMsg += "What - ";
	partialMsg += what + "\n";
	partialMsg += "Where - ";
	partialMsg += where + "\n";

	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(ptrMsgText);
	}
	else
	{
		LogFile.write(ptrMsgText, strlen(ptrMsgText));
		LogFile.write(new_line, strlen(new_line));			
	}

	partialMsg.FormatMessage(_T("\tFailure code: %1!x! \t%2!d!"), phr, phr);
	// Change error message format.
	ptrMsgText = partialMsg.GetBuffer(partialMsg.GetLength());

	CleanUpLogFile(ptrMsgText);
}

void UserErrorMessage(CString severity,
					  CString error_description,
					  VARIANT *error_msg)
{
	HRESULT	hr;
	char	*new_line = "\n";
	long	k;
	CString	partialMsg;

    SAFEARRAY FAR* psa_error_msg = NULL;
    SAFEARRAYBOUND error_sabound[1];

	// Set size of error array to pass to client.
	error_sabound[0].cElements = 2;
	error_sabound[0].lLbound = 0;
	psa_error_msg = SafeArrayCreate(VT_BSTR, 1, error_sabound);

	k=0;
	hr = SafeArrayPutElement(psa_error_msg, &k, severity.AllocSysString());
	k++;
	hr = SafeArrayPutElement(psa_error_msg, &k, error_description.AllocSysString());

	// Set the return error message, array of VT_BSTR.
	// VT_BSTR, strings. Type of data to return in perror_msg.
	// Which values are editable. i.e. can be changed.
	V_VT(error_msg) = VT_ARRAY | VT_BSTR;
	// Set return Editable array to contain error messages.
	V_ARRAY(error_msg) = psa_error_msg;
}

/************************************************************
HRESULT OKMessage(VARIANT *error_msg)

	Display current OK message.

-------------------------------------------------------------
 INPUT
	*error_msg - OK message buffer.

-------------------------------------------------------------
 OUTPUT
	OK message.

-------------------------------------------------------------
 LOCAL
	hr - Return code.
	k - Index for SAFEARRAYs.
	psa_error_msg - SAFEARRAY pointer to set error message.
	error_sabound[1] - Error message array boundary.

-------------------------------------------------------------
 RETURN
	None.

*************************************************************/
HRESULT OKMessage(VARIANT *error_msg)
{
	HRESULT	hr;
	long	k;

    SAFEARRAY FAR* psa_error_msg = NULL;
    SAFEARRAYBOUND error_sabound[1];

	BSTR code = SysAllocString(L"0");
	BSTR msg = SysAllocString(L"OK");

	// Set size of error array to pass to client.
	error_sabound[0].cElements = 2;
	error_sabound[0].lLbound = 0;
	psa_error_msg = SafeArrayCreate(VT_BSTR, 1, error_sabound);

	k=0;
	hr = SafeArrayPutElement(psa_error_msg, &k, code);
	k++;
	hr = SafeArrayPutElement(psa_error_msg, &k, msg);

	// Set the return error message, array of VT_BSTR.
	// VT_BSTR, strings. Type of data to return in perror_msg.
	// Which values are editable. i.e. can be changed.
	V_VT(error_msg) = VT_ARRAY | VT_BSTR;
	// Set return Editable array to contain error messages.
	V_ARRAY(error_msg) = psa_error_msg;

	return S_OK;
}

/***********************************************
void CurrentSystemDate(char *curDate)

	Gets the current date and time.

------------------------------------------------
INPUT
	curDate - String to return current date.

------------------------------------------------
LOCAL
	*newtime - Pointer to system time structure.
	am_pm[] - Time setting.
	long_time - Sytem time structure.

------------------------------------------------
OUTPUT
	none.

------------------------------------------------
RETURN
	none.
***********************************************/
void CurrentSystemDate(char *curDate)
{
	struct tm *newtime;
	char am_pm[] = "AM";
	time_t long_time;

	time( &long_time );					/* Get time as long integer. */
	newtime = localtime( &long_time );	/* Convert to local time. */

	if( newtime->tm_hour > 12 )			/* Set up extension. */
		strcpy( am_pm, "PM" );
	if( newtime->tm_hour > 12 )			/* Convert from 24-hour */
		newtime->tm_hour -= 12;			/*   to 12-hour clock.  */
	if( newtime->tm_hour == 0 )			/*Set hour to 12 if midnight. */
		newtime->tm_hour = 12;

//	sprintf(curDate,"%.19s %s\n", asctime( newtime ), am_pm );
	sprintf(curDate,"%.19s %s", asctime( newtime ), am_pm );
}

void DisplayVariant(VARIANT *displayVariant, CString message)
{
    SAFEARRAY FAR*	psa_displayVariant;		// sample plot list.
	HRESULT			hr;
	LONG			cElements, lLBound, lUBound;
	char			buf[512];

	long i=0;

	if (LogFile.is_open() == 0)
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		InitializeLogFile(buf);
	}
	else
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		LogFile.write(buf, strlen(buf));
	}


	if (LogFile.is_open() == 0)
	{
		sprintf(buf,"BEGIN DisplayVariant %s\n", message);
		InitializeLogFile(buf);
	}
	else
	{
		sprintf(buf,"BEGIN DisplayVariant %s\n", message);
		LogFile.write(buf, strlen(buf));
	}

	// set psa_plotList safearray pointer to the current plot list.
	psa_displayVariant = V_ARRAY(displayVariant);

	// Get array lower bound.
	hr = SafeArrayGetLBound(psa_displayVariant, 1, &lLBound);
	if (FAILED(hr))
	{
		LogFile.write("Array lower bound failed!", strlen("Array lower bound failed!"));
	}
	else
	{
		// Get array upper bound.
		hr = SafeArrayGetUBound(psa_displayVariant, 1, &lUBound);
		if(FAILED(hr))
		{
			LogFile.write("Array upper bound failed!\n", strlen("Array upper bound failed!\n"));
		}
		else
		{
			// Number of elements in array.
			cElements = lUBound-lLBound+1;

			sprintf(buf,"Number of elements to display = %d\n", cElements);
			LogFile.write(buf, strlen(buf));

			// Test array's type.
			if ( displayVariant->vt == 8200 )	// if ( displayVariant->vt == VT_BSTR )
			{
				BSTR bstr = NULL;

				LogFile.write("Current array type = BSTR\n", strlen("Current array type = BSTR\n"));

				// Get current string to display.
				while ((hr = SafeArrayGetElement(psa_displayVariant, &i, &bstr)) == S_OK)
				{
					sprintf(buf,"%s\n", (CString)bstr);
					LogFile.write(buf, strlen(buf));
					i++;
				}

				sprintf(buf,"Number of BSTRs = %d\n", i);
				LogFile.write(buf, strlen(buf));
			}
			else if ( displayVariant->vt == 8197 )	// if ( displayVariant->vt == VT_R8 )
			{
				double data_value;
			
				LogFile.write("Current array type = VT_R8\n", strlen("Current array type = VT_R8\n"));

				// Get current double value to display.
				while ((hr = SafeArrayGetElement(psa_displayVariant, &i, &data_value)) == S_OK)
				{
					sprintf(buf,"%.15f\n", data_value);
					LogFile.write(buf, strlen(buf));
					i++;
				}
				
				sprintf(buf,"Number of VT_R8s = %d\n", i);
				LogFile.write(buf, strlen(buf));
			}
		}
	}

	sprintf(buf,"END vector %s\n", message);
	CleanUpLogFile(buf);
}

void DisplayVector(iVector vecDisp, ui size, CString message)
{
	char	buf[256];

	if (LogFile.is_open() == 0)
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		InitializeLogFile(buf);
	}
	else
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		LogFile.write(buf, strlen(buf));
	}

	for (ui i=0; i<size; i++)
	{
		sprintf(buf,"%d\n", vecDisp[i]);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END vector %s\n", message);
	CleanUpLogFile(buf);
}

void DisplayVector(dVector vecDisp, ui size, CString message)
{
	char	buf[256];

	if (LogFile.is_open() == 0)
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		InitializeLogFile(buf);
	}
	else
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		LogFile.write(buf, strlen(buf));
	}

	for (ui i=0; i<size; i++)
	{
		sprintf(buf,"%.15f\n", vecDisp[i]);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END vector %s\n", message);
	CleanUpLogFile(buf);
}

void DisplayVector(uVector vecDisp, ui size, CString message)
{

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile("BEGIN DisplayVector");
	}
	else
	{
		LogFile << "BEGIN DisplayVector" << endl;
	}

	LogFile << message << endl;

	for (ui i=0; i<size; i++)
	{
		LogFile << vecDisp[i] << endl;
	}

	CleanUpLogFile("END DisplayVector");
}

void DisplayVector(liVector vecDisp, ui size, CString message)
{
	char	buf[256];

	if (LogFile.is_open() == 0)
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		InitializeLogFile(buf);
	}
	else
	{
		sprintf(buf,"BEGIN vector %s\n", message);
		LogFile.write(buf, strlen(buf));
	}

	for (ui i=0; i<size; i++)
	{
		sprintf(buf,"%d\n", vecDisp[i]);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END vector %s\n", message);
	CleanUpLogFile(buf);
}

void DisplayMatrix(dMatrix mDisp, ui size, CString message)
{
	char	buf[512];

	sprintf(buf,"\nBEGIN Matrix %s\n", message);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	for (ui r=0; r<size; r++)
	{
		for (ui c=0; c<size; c++)
		{
			sprintf(buf,"\t%.15f\t", mDisp[r][c]);
			LogFile.write(buf, strlen(buf));
		}
		sprintf(buf,"\n");
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END Matrix %s\n\n", message);
	CleanUpLogFile(buf);
}

void DisplayCArray(CArray<double,double> *displayVariant, CString source)
{
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayVariant->GetSize() > 0)
	{
		for (int i=0; i<displayVariant->GetSize(); i++)
		{
//			sprintf(buf,"%f\n", displayVariant->GetAt(i));
			sprintf(buf,"%.15f\n", displayVariant->GetAt(i));
			LogFile.write(buf, strlen(buf));
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayCArray(CArray<int,int> *displayVariant, CString source)
{
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayVariant->GetSize() > 0)
	{
		for (int i=0; i<displayVariant->GetSize(); i++)
		{
			sprintf(buf,"%d\n", displayVariant->GetAt(i));
			LogFile.write(buf, strlen(buf));
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayCArray(CArray<long,long> *displayVariant, CString source)
{
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayVariant->GetSize() > 0)
	{
		for (int i=0; i<displayVariant->GetSize(); i++)
		{
			sprintf(buf,"%d\n", displayVariant->GetAt(i));
			LogFile.write(buf, strlen(buf));
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayCArray(CArray<CString,CString> *displayVariant, CString source)
{
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayVariant->GetSize() > 0)
	{
		for (int i=0; i<displayVariant->GetSize(); i++)
		{
			sprintf(buf,"%s\n", displayVariant->GetAt(i));
			LogFile.write(buf, strlen(buf));
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayCStringArray(CStringArray *displayCStringArray, CString source)
{
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayCStringArray->GetSize() > 0)
	{
		for (int i=0; i<displayCStringArray->GetSize(); i++)
		{
			sprintf(buf,"%s\n", displayCStringArray->GetAt(i));
			LogFile.write(buf, strlen(buf));
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayCString(CString *displayCString)
{
	static char			buf[32000];

	if (InitializeLogFile())
	{
		if (displayCString->GetLength() > 0)
		{
			sprintf(buf,"%s\n", *displayCString);
			LogFile.write(buf, strlen(buf));
		}
		else
		{
			sprintf(buf,"\t%s is empty!\n", displayCString);
			LogFile.write(buf, strlen(buf));
		}

		CleanUpLogFile();
	}
}

void DisplayCString(CString *displayCString, CString source)
{
	char			buf[256];

	sprintf(buf,"BEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (displayCString->GetLength() > 0)
	{
		sprintf(buf,"%s\n", *displayCString);
		LogFile.write(buf, strlen(buf));
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n", source);
	CleanUpLogFile(buf);
}

void DisplayDataSets(CPtrArray *datasets, CString source)
{
	CArray<double,double> *current_dataset;
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (datasets->GetSize() > 0)
	{
		for (int i=0; i<datasets->GetSize(); i++)
		{
			current_dataset = (CArray<double,double> *)datasets->GetAt(i);

			for (int j=0; j<current_dataset->GetSize(); j++)
			{
				sprintf(buf,"%.15f\n", current_dataset->GetAt(j));
				LogFile.write(buf, strlen(buf));
			}
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}

void DisplayStatusSets(CPtrArray *statussets, CString source)
{
	CArray<int,int> *current_statusset;
	char			buf[256];

	sprintf(buf,"\n\nBEGIN %s\n", source);

	if (LogFile.is_open() == 0)
	{
		InitializeLogFile(buf);
	}
	else
	{
		LogFile.write(buf, strlen(buf));
	}

	if (statussets->GetSize() > 0)
	{
		for (int i=0; i<statussets->GetSize(); i++)
		{
			current_statusset = (CArray<int,int> *)statussets->GetAt(i);

			for (int j=0; j<current_statusset->GetSize(); j++)
			{
				sprintf(buf,"%d\n", current_statusset->GetAt(j));
				LogFile.write(buf, strlen(buf));
			}
		}
	}
	else
	{
		sprintf(buf,"\t%s is empty!\n", source);
		LogFile.write(buf, strlen(buf));
	}

	sprintf(buf,"END %s\n\n", source);
	CleanUpLogFile(buf);
}