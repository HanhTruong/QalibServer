HRESULT GetFitParsLimits( LONG calibrationID,
						  dVector Lower,
						  dVector Upper,
						  CString algorithm,
						  ui size,
						  CString wavelength,
						  VARIANT *error_msg);

void SetFitParametersLimitsResults(TCHAR *parameterLimits,
								   dVector Lower,
								   dVector Upper,
								   CString algorithm);

void InitializeFitParameters(dVector LowerLimits, 
							 dVector UpperLimits, 
							 dVector FitPars, 
							 dVector YExp, 
							 ui size);

void TestGetFitParameterLimits(CString algorithm,
							   LONG error_code, 
							   CString *msg);
