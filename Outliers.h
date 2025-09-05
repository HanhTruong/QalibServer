HRESULT Outliers(LONG calibrationID,
				 CString mode,
				 CString algorithm,
				 INT outlierMethod,
				 VARIANT *error_msg);

HRESULT OutlierLimits(LONG calibrationID,
					  CStringArray *sampleIds, 
					  CArray<double,double> *Upper, 
					  CArray<double,double> *Lower,
					  CArray<CString,CString> *ApplyOutlier,
					  VARIANT *error_msg);

HRESULT FindHistoricOutlier(CPtrArray *datasets,
							CPtrArray *statussets,
							CArray<int,int> *sampleSizes,
							CStringArray *sampleIds,
							CArray<double,double> *UpperOutlierLimits,
							CArray<double,double> *LowerOutlierLimits,
							CArray<CString,CString> *ApplyOutlier,
							VARIANT *error_msg);

HRESULT GetCalibrationData(LONG calibrationID,
						   CStringArray *calibrationResults,
						   CArray<int,int> *calibrationStatus);

HRESULT UpdateDataStatus(LONG calibrationID,
						 CArray<long,long> *indexingList,
						 CArray<int,int> *calibrationStatus);

HRESULT GetCalibrationDataStatus(LONG calibrationID, 
								 VARIANT *StatusData);

HRESULT CalculateSampleOutliers(CPtrArray *statussets,
								CArray<int,int> *ptrCalibrationStatus,
								CArray<double,double> *ptrCalibrationData,
								int *ptrStatusIndex,
								int *ptrDataIndex,
								int sampleSize,
								double lower,
								double upper,
								CString apply,
								VARIANT *error_msg);

HRESULT GetCalibrationOutlierData(LONG calibrationID,
								  CArray<long,long> *indexingIDs,
								  CArray<double,double> *calibrationData,
								  CArray<int,int> *calibrationStatus,
								  CArray<int,int> *sampleSizes,
								  CStringArray *sampleIds,
								  VARIANT *error_msg);

HRESULT RemoveOutlierValues(CPtrArray *datasets,
							CPtrArray *statussets,
							CArray<int,int> *samplesizes,
							VARIANT *error_msgs);

HRESULT TestOutlierResults(LONG calibrationID,
						   CString mode,
						   VARIANT *error_msg);

void UpdateValueStatus(CPtrArray *statussets,
					   int statusIndex);
