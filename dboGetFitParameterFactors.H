// dboGetFitParameterFactors.H : Declaration of the CdboGetFitParameterFactors class

#ifndef __DBOGETFITPARAMETERFACTORS_H_
#define __DBOGETFITPARAMETERFACTORS_H_

class CdboGetFitParameterFactorsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_rotorID;
	TCHAR m_colFitParameterID[257];
	double m_colValue;
	TCHAR m_colApplyFactor[257];

BEGIN_PARAM_MAP(CdboGetFitParameterFactorsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_rotorID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetFitParameterFactorsAccessor)
	COLUMN_ENTRY(1, m_colFitParameterID)
	COLUMN_ENTRY(2, m_colValue)
	COLUMN_ENTRY(3, m_colApplyFactor)
END_COLUMN_MAP()

};

#endif // __DBOGETFITPARAMETERFACTORS_H_
