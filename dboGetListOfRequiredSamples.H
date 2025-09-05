// dboGetListOfRequiredSamples.H : Declaration of the CdboGetListOfRequiredSamples class

#ifndef __DBOGETLISTOFREQUIREDSAMPLES_H_
#define __DBOGETLISTOFREQUIREDSAMPLES_H_

class CdboGetListOfRequiredSamplesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	TCHAR m_species[257];
	TCHAR m_type[51];
	TCHAR m_colSampleID[257];

BEGIN_PARAM_MAP(CdboGetListOfRequiredSamplesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_species)
	COLUMN_ENTRY(4, m_type)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetListOfRequiredSamplesAccessor)
	COLUMN_ENTRY(1, m_colSampleID)
END_COLUMN_MAP()

};

#endif // __DBOGETLISTOFREQUIREDSAMPLES_H_
