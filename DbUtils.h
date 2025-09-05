HRESULT GetSampleInstruments(LONG calibrationID, 
							 CString sampleId,
							 CStringArray *instrumentList,
							 VARIANT *error_msg);

HRESULT SendClientDataToDb(BSTR				chemistry,
						   INT				outlierMethod,
						   BSTR				mode,
						   BSTR				user,
						   CString			calibrationSpecies,
						   CStringArray		*inputData,
						   CStringArray		*setAttributes,
						   CStringArray		*serialNumbers,
						   CStringArray		*barCode,
						   CStringArray		*sampleSpecies,
						   CArray<int,int>	*statusData,
						   CArray<int,int>	*sampleSizes,
						   CStringArray		*sampleIds,
						   LONG				*calibrationID,
						   VARIANT			*error_msg);

HRESULT CalibrationMode(LONG calibrationID, CString *mode, VARIANT *error_msg);
