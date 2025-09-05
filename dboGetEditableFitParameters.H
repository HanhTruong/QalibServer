// dboGetEditableFitParameters.H : Declaration of the CdboGetEditableFitParameters class

#ifndef __DBOGETEDITABLEFITPARAMETERS_H_
#define __DBOGETEDITABLEFITPARAMETERS_H_

class CdboGetEditableFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_algorithm[257];
	TCHAR m_fitParameters[7901];

BEGIN_PARAM_MAP(CdboGetEditableFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_algorithm)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_fitParameters)
END_PARAM_MAP()

};

#endif // __DBOGETEDITABLEFITPARAMETERS_H_
