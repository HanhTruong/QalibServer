HRESULT GetCalibrationValues
					(LONG calibrationID, 
					 BSTR *chemistryName, 
					 BSTR *species, 
					 BSTR *diluent, 
					 BSTR *mold, 
					 BSTR *rotor, 
					 BSTR *cuvette,
					 BSTR *mode,
					 BSTR *user,
					 BSTR *date,
					 BSTR *comment,
					 VARIANT *error_msg);

HRESULT GetCalibrationDataAndStatus(LONG calibrationID, 
									VARIANT *calibrationData,
									VARIANT *dataStatus);

HRESULT SetDescriptionLabels(CString algorithm,
							 VARIANT *description);

HRESULT GetFitParameters(LONG calibrationID,
						 INT fitParameterChoice,
						 CString algorithm,
						 VARIANT *fitParameters);

HRESULT	GetMaxAndMinLimits(LONG	calibrationID,
						   VARIANT *minLimits, 
						   VARIANT *maxLimits,
						   CString algorithm,
						   VARIANT *error_msg);

HRESULT GetDisposition(CString algorithm,
					   VARIANT *fitParameters,
					   VARIANT *minLimits,
					   VARIANT *maxLimits,
					   VARIANT *disposition);

HRESULT CalibrationResults(LONG	calibrationID,
						   CString algorithm,
						   VARIANT *description, 
						   VARIANT *originalFitParameters, 
						   VARIANT *newFitParameters, 
						   VARIANT *minLimits, 
						   VARIANT *maxLimits, 
						   VARIANT *originalDisposition, 
						   VARIANT *newDisposition,
						   VARIANT *error_msg);

HRESULT GetPlotResults(LONG calibrationID,
					   VARIANT *plotData);

HRESULT GetChemistryWavelength(BSTR chemistry, 
							   BSTR *waveLength);
