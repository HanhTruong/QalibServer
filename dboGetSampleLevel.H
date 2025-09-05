// dboGetSampleLevel.H : Declaration of the CdboGetSampleLevel class

#ifndef __DBOGETSAMPLELEVEL_H_
#define __DBOGETSAMPLELEVEL_H_

class CdboGetSampleLevelAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[51];
	TCHAR m_SampleIDList[7901];
	TCHAR m_SampleLevelList[7901];

BEGIN_PARAM_MAP(CdboGetSampleLevelAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_SampleLevelList)
END_PARAM_MAP()

};

#endif // __DBOGETSAMPLELEVEL_H_
