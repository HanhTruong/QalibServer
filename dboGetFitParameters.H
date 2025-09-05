// dboGetFitParameters.H : Declaration of the CdboGetFitParameters class

#ifndef __DBOGETFITPARAMETERS_H_
#define __DBOGETFITPARAMETERS_H_

class CdboGetFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	LONG m_whichFitParameters;
	double m_colChiSquared;
	double m_colCorrelationCoefficient;
	double m_colBartlettTest;
	double m_colTop;
	double m_colBottom;
	double m_colSlope;
	double m_colIntercept;
	double m_colTopstandarderror;
	double m_colBottomstandarderror;
	double m_colSlopestandarderror;
	double m_colInterceptstandarderror;

BEGIN_PARAM_MAP(CdboGetFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_whichFitParameters)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetFitParametersAccessor)
	COLUMN_ENTRY(1, m_colChiSquared)
	COLUMN_ENTRY(2, m_colCorrelationCoefficient)
	COLUMN_ENTRY(3, m_colBartlettTest)
	COLUMN_ENTRY(4, m_colTop)
	COLUMN_ENTRY(5, m_colBottom)
	COLUMN_ENTRY(6, m_colSlope)
	COLUMN_ENTRY(7, m_colIntercept)
	COLUMN_ENTRY(8, m_colTopstandarderror)
	COLUMN_ENTRY(9, m_colBottomstandarderror)
	COLUMN_ENTRY(10, m_colSlopestandarderror)
	COLUMN_ENTRY(11, m_colInterceptstandarderror)
END_COLUMN_MAP()
};

#endif // __DBOGETFITPARAMETERS_H_
