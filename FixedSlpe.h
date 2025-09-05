HRESULT FixedSlopeOutlierLimits(LONG calibrationID,
					  TCHAR *sampleIds, 
					  CArray<double,double> *Upper, 
					  CArray<double,double> *Lower,
					  CArray<CString,CString> *ApplyOutlier,
					  VARIANT *error_msg);
