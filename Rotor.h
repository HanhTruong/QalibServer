typedef enum ROTORATTRIBUTES {CUVETTE};

HRESULT GetRotorCuvetteAttributes(CString rotorNumber,
								  CString chemistry,
								  CStringArray *rotorAttributes,
								  VARIANT *error_msg);

bool TestRotorFactors(LONG calibrationID,
					  CString chemistry,
  					  CString mode,
					  VARIANT *error_msg);

HRESULT RotorFactors(LONG calibrationID,
					 CString chemistry,
					 VARIANT *error_msg);

HRESULT ProcessRotorFactors(CString chemistry,
							CStringArray *rotorFactorsList,
							int	rotorNumber,
							VARIANT *error_msg);

HRESULT GetChemistryRotorFactors(CString chemistry, 
								 int l_rotornumber,
								 CStringArray *barCodeAttributes,
								 VARIANT *error_msg);

