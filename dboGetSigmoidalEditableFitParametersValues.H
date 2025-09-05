// dboGetSigmoidalEditableFitParametersValues.H : Declaration of the CdboGetSigmoidalEditableFitParametersValues class

#ifndef __DBOGETSIGMOIDALEDITABLEFITPARAMETERSVALUES_H_
#define __DBOGETSIGMOIDALEDITABLEFITPARAMETERSVALUES_H_

class CdboGetSigmoidalEditableFitParametersValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	double m_colBottom;
	double m_colIntercept;
	double m_colSlope;
	double m_colTop;

BEGIN_PARAM_MAP(CdboGetSigmoidalEditableFitParametersValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSigmoidalEditableFitParametersValuesAccessor)
	COLUMN_ENTRY(1, m_colBottom)
	COLUMN_ENTRY(2, m_colIntercept)
	COLUMN_ENTRY(3, m_colSlope)
	COLUMN_ENTRY(4, m_colTop)
END_COLUMN_MAP()

};

#endif // __DBOGETSIGMOIDALEDITABLEFITPARAMETERSVALUES_H_
