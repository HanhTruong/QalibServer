/*******************************************************************

FILENAME:	NewFitParameters.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Declaration for new fit parameter functions.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	

INVOCATION: Build -> Compile	NewFitParameters.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/
HRESULT GetEditableFitParameters(
				LONG calibrationID,
				CString algorithm,
				VARIANT *editableFitParameters);

HRESULT GetLinearEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters);

HRESULT GetQuadraticEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters);

HRESULT GetMinMaxDecayEditableFitParameters(
						LONG calibrationID, 
						VARIANT *editableFitParameters);

void SetFitParameters(CArray<double,double> *fitParameters, 
					  dVector dFitParameters, 
					  CString algorithm);

