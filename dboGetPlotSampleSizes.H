// dboGetPlotSampleSizes.H : Declaration of the CdboGetPlotSampleSizes class

#ifndef __DBOGETPLOTSAMPLESIZES_H_
#define __DBOGETPLOTSAMPLESIZES_H_

class CdboGetPlotSampleSizesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_SampleIdList[2001];
	TCHAR m_SampleSizesList[2001];
	TCHAR m_SampleNoOutlierSizesList[2001];

BEGIN_PARAM_MAP(CdboGetPlotSampleSizesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_SampleIdList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_SampleSizesList)
	COLUMN_ENTRY(5, m_SampleNoOutlierSizesList)
END_PARAM_MAP()

};

#endif // __DBOGETPLOTSAMPLESIZES_H_
