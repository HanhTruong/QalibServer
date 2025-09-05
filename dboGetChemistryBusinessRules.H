// dboGetChemistryBusinessRules.H : Declaration of the CdboGetChemistryBusinessRules class

#ifndef __DBOGETCHEMISTRYBUSINESSRULES_H_
#define __DBOGETCHEMISTRYBUSINESSRULES_H_

class CdboGetChemistryBusinessRulesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	TCHAR m_mode[257];
	TCHAR m_resultlist[7001];

BEGIN_PARAM_MAP(CdboGetChemistryBusinessRulesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_mode)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_resultlist)
END_PARAM_MAP()

};

#endif // __DBOGETCHEMISTRYBUSINESSRULES_H_
