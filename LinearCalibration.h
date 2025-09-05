double LinearCorrelationCoefficient(dVector X,
									dVector Y,
									ui		n);

HRESULT InsertLinearFitParameters(LONG calibrationID,
								  dVector FitPars,
								  double ccoe,
								  double slopeError,
								  double interceptError,
								  double chi_squared,
								  double bartlett);

void LinearCalculateAssignedValues(CArray<double,double> *fitParameters,
								   CArray<double,double> *calibratorData,
								   CArray<double,double> *calcAssignedValues);

HRESULT LinearRatesFromAssignedValues(LONG calibrationID, 
									  dVector linearParameters, 
									  VARIANT *rates,
									  VARIANT *error_msg);

HRESULT EvaluateLinearFitParameters(VARIANT *newFitPars, VARIANT *error_msg);

HRESULT LinearFitCalibration(LONG calibrationID,
							 CString algorithm,
							 dVector fitParameters,		// A
							 dMatrix Covariance,		// Covar
							 iVector AStatus,			// Status
							 unsigned numberFitPars,	// number of fit parameters.
							 dVector assignedValues,	// X
						     dVector analyzerData,		// Y
						     dVector Weights,			// InvVar
						     ui sizex,					// sizex
							 double *chisquared,
							 VARIANT *error_msg);

double LinearChiSquared(dVector LinearParams, dVector X, dVector Y, dVector W, ui s );

HRESULT LinearCalibration(LONG		calibrationID,
						  VARIANT	*newFitPars,
						  CString	algorithm,
						  double	*chisquared,
						  VARIANT	*error_msg);

HRESULT LinearCalculations(LONG calibrationID,
						   CString algorithm,
						   double *chisquared,
						   VARIANT *newFitParameters,
						   VARIANT *error_msg);
