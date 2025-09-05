// dboGetSampleSizes.H : Declaration of the CdboGetSampleSizes class

#ifndef __DBOGETSAMPLESIZES_H_
#define __DBOGETSAMPLESIZES_H_

class CdboGetSampleSizesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_CalibrationID;
	TCHAR m_outliers[6];
	TCHAR m_SampleIdList[2001];
	TCHAR m_SampleSizesList[2001];

BEGIN_PARAM_MAP(CdboGetSampleSizesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_CalibrationID)
	COLUMN_ENTRY(3, m_outliers)
	COLUMN_ENTRY(4, m_SampleIdList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(5, m_SampleSizesList)
END_PARAM_MAP()


};

#endif // __DBOGETSAMPLESIZES_H_
