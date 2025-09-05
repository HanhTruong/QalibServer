HRESULT LinearCalibrationResults(LONG	calibrationID,
								 CString algorithm,
								 VARIANT *description, 
								 VARIANT *originalFitParameters, 
								 VARIANT *newFitParameters, 
								 VARIANT *minLimits, 
								 VARIANT *maxLimits, 
								 VARIANT *originalDisposition, 
								 VARIANT *newDisposition);

HRESULT	GetLinearMaxAndMinLimits(LONG	calibrationID,
								 VARIANT *minLimits, 
								 VARIANT *maxLimits,
								 CString algorithm,
								 VARIANT *error_msg);

HRESULT SetLinearDescriptionLabels(VARIANT *description);

HRESULT GetLinearDisposition(VARIANT *fitParameters,
							 VARIANT *minLimits,
							 VARIANT *maxLimits,
							 VARIANT *disposition);
