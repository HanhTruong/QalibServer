// dboGetSampleSpecies.H : Declaration of the CdboGetSampleSpecies class

#ifndef __DBOGETSAMPLESPECIES_H_
#define __DBOGETSAMPLESPECIES_H_

class CdboGetSampleSpeciesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_listOfSamples[7901];
	TCHAR m_listOfSpecies[7901];

BEGIN_PARAM_MAP(CdboGetSampleSpeciesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_listOfSamples)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_listOfSpecies)
END_PARAM_MAP()

};

#endif // __DBOGETSAMPLESPECIES_H_
