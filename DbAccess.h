HRESULT TestUserAccess(BSTR user, BSTR password, VARIANT *error_msg);

HRESULT LoadCalibrationDataToDB(BSTR			chemistry,
								BSTR			mode,
								BSTR			user,
								int				mold,
								int				rotor,
								int				cuvette,
								int				barcodelength,
								CString			calibrationSpecies,
								CStringArray	*sampleData,
								CStringArray	*setAttributes,
								CArray<int,int>	*sampleSizes,
								CArray<int,int>	*sampleStatus,
								CStringArray	*sampleIds,
								CStringArray	*sampleSpecies,
								CStringArray	*sampleSerialNumbers,
								CStringArray	*sampleBarCode,
								LONG			*calibrationID,
								VARIANT			*error_msg);

HRESULT GetDataAndStatusForAListOfSamples(LONG calibrationID,
										  CString outliers,
										  CStringArray *sampleIdList,
										  CArray<double,double> *values,
										  CArray<int,int> *status,
										  VARIANT *error_msg);

HRESULT GetSampleValueIDsForIndexing(LONG calibrationID,
									 CArray<long,long> *indexingIDs,
									 VARIANT *error_msg);

HRESULT OpenDatabase(CDataSource *pdb, 
					 CSession *psession, 
					 CString Caller);

HRESULT FindChemistrySpeciesDependencies(CString chemistry,
										 CString *calibrationSpecies);
