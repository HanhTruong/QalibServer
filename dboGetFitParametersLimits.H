// dboGetFitParametersLimits.H : Declaration of the CdboGetFitParametersLimits class

#ifndef __DBOGETFITPARAMETERSLIMITS_H_
#define __DBOGETFITPARAMETERSLIMITS_H_

class CdboGetFitParametersLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_algorithm[257];
	TCHAR m_parameterLimits[7901];

BEGIN_PARAM_MAP(CdboGetFitParametersLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_algorithm)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_parameterLimits)
END_PARAM_MAP()

};

#endif // __DBOGETFITPARAMETERSLIMITS_H_
