HRESULT GetAnalyteLevelList(CString chemistry,
							CString sampleIdList,
							CArray<int,int> *sampleAnalyteLevelList);

HRESULT GetSamplesAnalyteTypes(CStringArray *sampleIdsList,
							   CStringArray *analyteTypesList);

bool FindSampleInCalibration(LONG calibrationID,
							 CString sample,
							 VARIANT *error_msg);
