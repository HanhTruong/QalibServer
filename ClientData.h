HRESULT ConvertClientData(VARIANT			*v_clientData,
						  VARIANT			*v_serialNumber, 
						  VARIANT			*v_barCode,
						  VARIANT			*v_sampleSpecies,
						  VARIANT			*v_statusData,
  						  CStringArray		*c_clientData,
						  CStringArray		*c_serialNumbers,
						  CStringArray		*c_barCode,
						  CStringArray		*c_sampleSpecies,
						  CArray<int,int>	*c_statusData);

HRESULT ProcessClientData(BSTR		chemistry,
						  INT		outlierMethod,
						  BSTR		mode,
						  BSTR		user, 
  						  CString	*calibrationSpecies,
						  VARIANT	*v_inputData,
						  VARIANT	*v_serialNumber, 
						  VARIANT	*v_barCode,
						  VARIANT	*v_sampleSpecies,
						  VARIANT	*v_statusData,
						  LONG		*calibrationID,
						  VARIANT	*error_msgs);

int FindNumberOfDataSets(CStringArray *inputData,
						 CArray<int,int> *sampleSizes);

HRESULT ConvertClientData(VARIANT			*v_sampleData,
						  VARIANT			*v_serialNumber, 
						  VARIANT			*v_barCode,
						  VARIANT			*v_sampleSpecies,
						  VARIANT			*v_statusData,
  						  CStringArray		*c_sampleData,
						  CStringArray		*c_serialNumbers,
						  CStringArray		*c_barCode,
						  CStringArray		*c_sampleSpecies,
						  CArray<int,int>	*c_statusData);

HRESULT ExtractClientData(CStringArray		*inputData,
						  CArray<int,int>	*sampleSizes,
						  CStringArray		*sampleIds,
						  CStringArray		*setAttributes);

HRESULT UpdateCalibrationData(LONG				calibrationID,
							  CPtrArray			*datasets,
							  CPtrArray			*statussets,
							  CStringArray		*sampleIds,
							  CArray<int,int>	*sampleSizes,
							  VARIANT			*error_msg);

HRESULT ExtractSets(CPtrArray *datasets,
					CPtrArray *statussets,
					CStringArray *sampleids,
					CArray<int,int> *samplesizes,
					CArray<int,int> *status,
					CArray<double,double> *values,
					VARIANT *error_msgs);

HRESULT ChemistryDependencies(LONG calibrationID,
							  CPtrArray *datasets,
							  VARIANT *error_msgs);

HRESULT SetupDataForOptivecFunctions(CArray<double,double> *dataSet,
									 CStringArray *sampleIds,
									 CArray<int,int> *sampleSizes,
									 CArray<double,double> *assignedValues,
									 VARIANT *error_msgs);
