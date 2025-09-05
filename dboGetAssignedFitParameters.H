// dboGetAssignedFitParameters.H : Declaration of the CdboGetAssignedFitParameters class

#ifndef __DBOGETASSIGNEDFITPARAMETERS_H_
#define __DBOGETASSIGNEDFITPARAMETERS_H_

class CdboGetAssignedFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_SampleType[51];
	double m_ccoe;
	double m_Slope;
	double m_SlopeStdError;
	double m_Intercept;
	double m_InterceptStdError;
	double m_colccoe;
	double m_colSlope;
	double m_colSlopeStdError;
	double m_colIntercept;
	double m_colInterceptStdError;

BEGIN_PARAM_MAP(CdboGetAssignedFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_SampleType)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_ccoe)
	COLUMN_ENTRY(5, m_Slope)
	COLUMN_ENTRY(6, m_SlopeStdError)
	COLUMN_ENTRY(7, m_Intercept)
	COLUMN_ENTRY(8, m_InterceptStdError)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetAssignedFitParametersAccessor)
	COLUMN_ENTRY(1, m_colccoe)
	COLUMN_ENTRY(2, m_colSlope)
	COLUMN_ENTRY(3, m_colSlopeStdError)
	COLUMN_ENTRY(4, m_colIntercept)
	COLUMN_ENTRY(5, m_colInterceptStdError)
END_COLUMN_MAP()

};

#endif // __DBOGETASSIGNEDFITPARAMETERS_H_
