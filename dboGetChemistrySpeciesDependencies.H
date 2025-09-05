// dboGetChemistrySpeciesDependencies.H : Declaration of the CdboGetChemistrySpeciesDependencies class

#ifndef __DBOGETCHEMISTRYSPECIESDEPENDENCIES_H_
#define __DBOGETCHEMISTRYSPECIESDEPENDENCIES_H_

class CdboGetChemistrySpeciesDependenciesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	TCHAR m_species[257];

BEGIN_PARAM_MAP(CdboGetChemistrySpeciesDependenciesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_species)
END_PARAM_MAP()

};

#endif // __DBOGETCHEMISTRYSPECIESDEPENDENCIES_H_
