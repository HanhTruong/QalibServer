HRESULT	GetSigmoidalMaxAndMinLimits(LONG	calibrationID,
									VARIANT *minLimits, 
									VARIANT *maxLimits,
									CString algorithm,
									VARIANT *error_msg);

HRESULT SetSigmoidalDescriptionLabels(VARIANT *description);

HRESULT GetSigmoidalDisposition(VARIANT *fitParameters,
								VARIANT *minLimits,
								VARIANT *maxLimits,
								VARIANT *disposition);
