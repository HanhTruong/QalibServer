HRESULT QuadraticCalculations(LONG calibrationID,
							  CString algorithm,
							  double *chisquared,
							  VARIANT *newFitParameters,
							  VARIANT *error_msg);

void QuadraticCalculateAssignedValues(
				CArray<double,double> *fitParameters,
				CArray<double,double> *calibratorData,
				CArray<double,double> *calcAssignedValues);

HRESULT EvaluateQuadraticFitParameters(VARIANT *newFitPars,
									   VARIANT *error_msg);

double QuadraticChiSquared(dVector LinearParams,
						   dVector X,
						   dVector Y,
						   dVector W,
						   ui s );

HRESULT InsertQuadraticFitParameters(LONG calibrationID,
									 dVector FitPars,
									 double ccoe,
									 double slopeError,
									 double anchorError,
									 double curvatureError,
									 double chi_squared,
									 double bartlett);

HRESULT QuadraticRatesFromAssignedValues(LONG calibrationID,
										 dVector linearParameters,
										 VARIANT *rates,
										 VARIANT *error_msg);

double QuadraticCorrelationCoefficient(dVector X, 
									   dVector Y,
									   ui sizeYX);
