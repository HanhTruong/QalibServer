HRESULT RatesFromAssignedValues(LONG calibrationID, 
								CString algorithm,
								dVector linearParameters,
								VARIANT *rates,
								VARIANT *error_msg);

HRESULT RemoveCONTROLData(LONG calibrationID, 
						  CStringArray *sampleIDsList);

void InitializeGlobals(CString algorithm);

void CalculateWeights(dVector weights, ui size);

void CalculateWeights(dVector weights, 
					  dVector Rate, 
					  ui	  size, 
					  liVector poolsizes, 
					  ui	  poolsizelen);

void CalculateWeights();

void DerivModel( dVector dYdAi, dVector X, ui size, unsigned iPar );
int NumberOfFreeParameters();
int NumberOfFixedParameters(iVector statusVector, int statusSize);

HRESULT QalibNonlinearFitWithWeight(double *chisquared);

HRESULT CalibrationCalculations(LONG calibrationID,
								CString algorithm,
								double *chisquared,
								VARIANT *newFitParameters,
								VARIANT *error_msg);

HRESULT QalibWeightMethod(BSTR wMethod);
