// dboGetAllowedSampleSpecies.H : Declaration of the CdboGetAllowedSampleSpecies class

#ifndef __DBOGETALLOWEDSAMPLESPECIES_H_
#define __DBOGETALLOWEDSAMPLESPECIES_H_

class CdboGetAllowedSampleSpeciesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_colSpecies[257];

BEGIN_PARAM_MAP(CdboGetAllowedSampleSpeciesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetAllowedSampleSpeciesAccessor)
	COLUMN_ENTRY(1, m_colSpecies)
END_COLUMN_MAP()
};

#endif // __DBOGETALLOWEDSAMPLESPECIES_H_
