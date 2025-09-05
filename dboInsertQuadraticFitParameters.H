// dboInsertQuadraticFitParameters.H : Declaration of the CdboInsertQuadraticFitParameters class

#ifndef __DBOINSERTQUADRATICFITPARAMETERS_H_
#define __DBOINSERTQUADRATICFITPARAMETERS_H_

class CdboInsertQuadraticFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	double m_ChiSquared;
	double m_CorrelationCoefficient;
	double m_BartlettTest;
	double m_Slope;
	double m_SlopeStdError;
	double m_Anchor;
	double m_AnchorStdError;
	double m_Curvature;
	double m_CurvatureStdError;

BEGIN_PARAM_MAP(CdboInsertQuadraticFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_ChiSquared)
	COLUMN_ENTRY(4, m_CorrelationCoefficient)
	COLUMN_ENTRY(5, m_BartlettTest)
	COLUMN_ENTRY(6, m_Slope)
	COLUMN_ENTRY(7, m_SlopeStdError)
	COLUMN_ENTRY(8, m_Anchor)
	COLUMN_ENTRY(9, m_AnchorStdError)
	COLUMN_ENTRY(10, m_Curvature)
	COLUMN_ENTRY(11, m_CurvatureStdError)
END_PARAM_MAP()

};

#endif // __DBOINSERTQUADRATICFITPARAMETERS_H_
