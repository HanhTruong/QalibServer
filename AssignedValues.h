HRESULT TestAssignedValueLimits(CString chemistry,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues,
								VARIANT *error_msg);

HRESULT GetAssignedValueLimits(CString	chemistry,
							   CArray<double,double> *assignedValueLimits,
							   VARIANT	*error_msg);

HRESULT GetSampleAssignedValues(LONG calibrationID,
								CStringArray *sampleIds,
								CArray<double,double> *assignedValues,
								VARIANT *error_msg);

HRESULT GetAssignedValues(CString chemistry,
						  CString sampleIDList,
						  CArray<double,double> *assignedValues,
						  VARIANT *error_msg);

HRESULT CalculateAssignedValues(CArray<double,double> *fitParameters,
								CArray<double,double> *calibratorData,
								CArray<double,double> *assignedValues,
								CArray<double,double> *calcAssignedValues,
								CString algorithm);

