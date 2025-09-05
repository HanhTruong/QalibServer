// dboInsertLinearFitParameters.H : Declaration of the CdboInsertLinearFitParameters class

#ifndef __DBOINSERTLINEARFITPARAMETERS_H_
#define __DBOINSERTLINEARFITPARAMETERS_H_

class CdboInsertLinearFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	double m_ChiSquared;
	double m_CorrelationCoefficient;
	double m_BartlettTest;
	double m_Slope;
	double m_SlopeStdError;
	double m_Intercept;
	double m_InterceptStdError;

BEGIN_PARAM_MAP(CdboInsertLinearFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_ChiSquared)
	COLUMN_ENTRY(4, m_CorrelationCoefficient)
	COLUMN_ENTRY(5, m_BartlettTest)
	COLUMN_ENTRY(6, m_Slope)
	COLUMN_ENTRY(7, m_SlopeStdError)
	COLUMN_ENTRY(8, m_Intercept)
	COLUMN_ENTRY(9, m_InterceptStdError)
END_PARAM_MAP()

};

#endif // __DBOINSERTLINEARFITPARAMETERS_H_
