/*******************************************************************

FILENAME:	QalibConstants.cpp

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Main module for calibration report processing.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:
	QalibConstants()
	~QalibConstants()
	GetMaxDoubleValue()
	GetMinDoubleValue()

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0

--------------------------------------------------------------------
INVOCATION: 
	Build->Compile	QalibConstants.cpp

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
// QalibConstants.cpp: implementation of the QalibConstants class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "QalibConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**********************************
QalibConstants::QalibConstants()

	Constants class constructor.
	Initializes constants.

-----------------------------------
 INPUT
	None.

-----------------------------------
 OUTPUT
	Initilized constants.

-----------------------------------
 LOCAL
	None.

-----------------------------------
 RETURN														
	None.														
**********************************/
QalibConstants::QalibConstants()
{
	max_double_value = 999999999;
	min_double_value = -999999999;
}

/*********************************
QalibConstants::~QalibConstants()

	Constants class destructor.
	Default behavior.

----------------------------------
 INPUT
	None.

----------------------------------
 OUTPUT
	None.

----------------------------------
 LOCAL
	None.

----------------------------------
 RETURN														
	None.														
*********************************/
QalibConstants::~QalibConstants()
{

}

/*****************************************
double QalibConstants::GetMaxDoubleValue()

 Return maximum value for a double.

------------------------------------------
 INPUT
	None.

------------------------------------------
 OUTPUT
	None.

------------------------------------------
 LOCAL
	None.

------------------------------------------
 RETURN														
	Maximum value for a double.														

*****************************************/
double QalibConstants::GetMaxDoubleValue()
{
	return max_double_value;
}

/*****************************************
double QalibConstants::GetMinDoubleValue()

 Return minimum value for a double.

------------------------------------------
 INPUT
	None.

------------------------------------------
 OUTPUT
	None.

------------------------------------------
 LOCAL
	None.

------------------------------------------
 RETURN														
	Minimum value for a double.

*****************************************/
double QalibConstants::GetMinDoubleValue()
{
	return min_double_value;
}
