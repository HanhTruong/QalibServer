// dboGetChemistryWavelength.H : Declaration of the CdboGetChemistryWavelength class

#ifndef __DBOGETCHEMISTRYWAVELENGTH_H_
#define __DBOGETCHEMISTRYWAVELENGTH_H_

class CdboGetChemistryWavelengthAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistryID[257];
	TCHAR m_colWavelength[257];

BEGIN_PARAM_MAP(CdboGetChemistryWavelengthAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistryID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetChemistryWavelengthAccessor)
	COLUMN_ENTRY(1, m_colWavelength)
END_COLUMN_MAP()

};

#endif // __DBOGETCHEMISTRYWAVELENGTH_H_
