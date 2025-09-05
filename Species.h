HRESULT VerifySpecies(LONG				calibrationID,
					  CStringArray		*sampleIds,
					  CStringArray		*sampleSpecies,
					  CArray<int,int>	*sampleSizes,
					  VARIANT			*error_msg);

HRESULT GetCalibrationSpecies(CStringArray *calibrationSpecies);

HRESULT VerifyCalibratorSpecies(CStringArray	*sampleIds,
								CStringArray	*sampleTypes,
								CStringArray	*actualSpecies,
								CStringArray	*allowedCalibrationSpecies,
								VARIANT			*error_msg);

HRESULT GetExpectedSampleSpecies(CStringArray *listOfSamples, 
								 CStringArray *listOfSpecies);

HRESULT VerifySampleSpecies(CStringArray	*sampleIds,
							CStringArray	*sampleSpecies,
							CArray<int,int>	*sampleSizes,
							CStringArray	*actualSpecies,
							VARIANT			*error_msg);

HRESULT	VerifyActualVsExpectedSpecies(CStringArray	*sampleIds,
									  CStringArray	*sampleSpecies,
									  CStringArray	*actualSpecies,
									  VARIANT		*error_msg);

HRESULT AdjustSamples(CStringArray *sampleIds, CArray<int,int> *sampleSizes);

HRESULT FindCalibrationSpecies(BSTR		chemistry,
							   CStringArray *sampleIds,
							   CArray<int,int> *sampleSizes,
							   CStringArray	*sampleSpecies,
							   CString *calibrationSpecies,
							   VARIANT *error_msg);

HRESULT GetAllowedSampleSpecies(CStringArray *allowedSampleSpecies);

HRESULT VerifyCalibrationSpecies(BSTR				chemistry,
								 CStringArray		*sampleIds,
								 CArray<int,int>	*sampleSizes,
								 CStringArray		*sampleSpecies,
								 CStringArray		*analyteTypesList,
								 CString			*calibrationSpecies,
								 VARIANT			*error_msg);

HRESULT VerifySampleIdSpecies(CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  CStringArray		*analyteTypesList,
							  CString			*calibrationSpecies);
