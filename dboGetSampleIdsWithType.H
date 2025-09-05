// dboGetSampleIdsWithType.H : Declaration of the CdboGetSampleIdsWithType class

#ifndef __DBOGETSAMPLEIDSWITHTYPE_H_
#define __DBOGETSAMPLEIDSWITHTYPE_H_

class CdboGetSampleIdsWithTypeAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_type[257];
	TCHAR m_colSampleID[257];

BEGIN_PARAM_MAP(CdboGetSampleIdsWithTypeAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_type)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleIdsWithTypeAccessor)
	COLUMN_ENTRY(1, m_colSampleID)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLEIDSWITHTYPE_H_
