HRESULT GetQuadraticDisposition(VARIANT *fitParameters,
								VARIANT *minLimits,
								VARIANT *maxLimits,
								VARIANT *disposition);

HRESULT SetQuadraticDescriptionLabels(VARIANT *description);

HRESULT	GetQuadraticMaxAndMinLimits(LONG	calibrationID,
									VARIANT *minLimits,
									VARIANT *maxLimits,
									CString algorithm,
									VARIANT *error_msg);
