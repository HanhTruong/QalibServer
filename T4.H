bool TestT4AnalyteTypes(CStringArray *analyteTypesList, 
						AnalyteType *current_analytes, 
						double minNumberOfCalibrators,
						VARIANT *error_msg);

HRESULT Establish_T4_Top_Factor(LONG calibrationID, 
								CString partNumber,
								double	topFactorMinValue,
								VARIANT *error_msg);

HRESULT Zero_T4_Level(LONG calibrationID,
					  CString chemistry,
					  VARIANT *error_msg);
