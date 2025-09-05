// dboGetSampleTypes.H : Declaration of the CdboGetSampleTypes class

#ifndef __DBOGETSAMPLETYPES_H_
#define __DBOGETSAMPLETYPES_H_

class CdboGetSampleTypesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_SampleIDList[7901];
	TCHAR m_SampleTypesList[7901];

BEGIN_PARAM_MAP(CdboGetSampleTypesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_SampleTypesList)
END_PARAM_MAP()

};

#endif // __DBOGETSAMPLETYPES_H_
