double SigmoidalCorrelationCoefficient(dVector X, 
									   dVector Y,
									   ui sizeYX);

void SigmoidalCalculateAssignedValues(
				CArray<double,double> *fitParameters,
				CArray<double,double> *calibratorData,
				CArray<double,double> *assignedValues,
				CArray<double,double> *calcAssignedValues);

HRESULT SigmoidalRatesFromAssignedValues(
			LONG calibrationID, 
			VARIANT *rates, 
			VARIANT *error_msg);

HRESULT SigmoidalCalculations(LONG calibrationID,
							  CString algorithm,
							  double *chisquared,
							  VARIANT *newFitParameters,
							  VARIANT *error_msg);
