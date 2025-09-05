#include "AnalyteTypeClass.h"

HRESULT ProcessBusinessRules(LONG calibrationID,
							 CString chemistry,
							 CString mode,
							 CString calibrationSpecies,
							 VARIANT *serialNumber,
							 VARIANT *error_msg);

HRESULT GetBusinessRules(CString chemistry, 
						 CString mode, 
						 CStringArray *businessrules, 	
						 CArray<double,double> *ruleValues);

HRESULT TypesOfCurrentDataSets(LONG calibrationID, 
							   AnalyteType *current_analytes);
