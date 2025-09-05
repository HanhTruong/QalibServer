// dboGetRotorFactors.H : Declaration of the CdboGetRotorFactors class

#ifndef __DBOGETROTORFACTORS_H_
#define __DBOGETROTORFACTORS_H_

class CdboGetRotorFactorsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	LONG m_rotornumber;
	TCHAR m_colFactorID[257];

BEGIN_PARAM_MAP(CdboGetRotorFactorsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_rotornumber)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetRotorFactorsAccessor)
	COLUMN_ENTRY(1, m_colFactorID)
END_COLUMN_MAP()

};

#endif // __DBOGETROTORFACTORS_H_
