// dboInsertSigmoidalFitParameters.H : Declaration of the CdboInsertSigmoidalFitParameters class

#ifndef __DBOINSERTSIGMOIDALFITPARAMETERS_H_
#define __DBOINSERTSIGMOIDALFITPARAMETERS_H_

class CdboInsertSigmoidalFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	double m_ChiSquared;
	double m_CorrelationCoefficient;
	double m_BartlettTest;
	double m_Top;
	double m_TopStdError;
	double m_Bottom;
	double m_BottomStdError;
	double m_Slope;
	double m_SlopeStdError;
	double m_Intercept;
	double m_InterceptStdError;

BEGIN_PARAM_MAP(CdboInsertSigmoidalFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_ChiSquared)
	COLUMN_ENTRY(4, m_CorrelationCoefficient)
	COLUMN_ENTRY(5, m_BartlettTest)
	COLUMN_ENTRY(6, m_Top)
	COLUMN_ENTRY(7, m_TopStdError)
	COLUMN_ENTRY(8, m_Bottom)
	COLUMN_ENTRY(9, m_BottomStdError)
	COLUMN_ENTRY(10, m_Slope)
	COLUMN_ENTRY(11, m_SlopeStdError)
	COLUMN_ENTRY(12, m_Intercept)
	COLUMN_ENTRY(13, m_InterceptStdError)
END_PARAM_MAP()

};

#endif // __DBOINSERTSIGMOIDALFITPARAMETERS_H_
