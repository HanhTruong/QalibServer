HRESULT SetFixedSlope(LONG calibrationID, dVector lParams);
HRESULT GetFixedSlopeValues(CString diluent, CString partNumber);
HRESULT GetFixedSlopeValues(CString diluent, 
							CString chemistry,
							double	*value,
							double	*lower,
							double	*upper);
HRESULT FixedSlopeCalculations(LONG calibrationID,
							   CString algorithm,
							   double *chisquared,
							   VARIANT *newFitParameters,
							   VARIANT *error_msg);

