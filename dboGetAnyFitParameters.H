// dboGetAnyFitParameters.H : Declaration of the CdboGetAnyFitParameters class

#ifndef __DBOGETANYFITPARAMETERS_H_
#define __DBOGETANYFITPARAMETERS_H_

class CdboGetAnyFitParametersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	LONG m_whichFitParameters;
	TCHAR m_algorithm[257];
	TCHAR m_fitParameters[7901];

BEGIN_PARAM_MAP(CdboGetAnyFitParametersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_whichFitParameters)
	COLUMN_ENTRY(4, m_algorithm)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(5, m_fitParameters)
END_PARAM_MAP()

};

#endif // __DBOGETANYFITPARAMETERS_H_
