void InitializeExponentialFitParameters(dVector FitPars);
HRESULT ExponentialLinearFitWithWeight(double *chisquared);
void ExponentialRate( dVector BasFuncs, double x, unsigned size);
HRESULT ExponentialRatesFromAssignedValues(LONG calibrationID,
										   dVector fitParameters,
										   VARIANT *rates,
										   VARIANT *error_msg);
HRESULT ExponentialCalculations(LONG calibrationID,
								CString algorithm,
								double *chisquared,
								VARIANT *newFitParameters,
								VARIANT *error_msg);
void ExponentialCalculateAssignedValues(CArray<double,double> *fitParameters,
										CArray<double,double> *calibratorData,
										CArray<double,double> *calcAssignedValues);

double ExponentialCorrelationCoefficient(dVector X, 
										 dVector Y,
										 ui sizeYX);

double ExponentialChiSquared(dVector LinearParams, 
							 dVector X, 
							 dVector Y, 
							 dVector W, 
							 ui s );
