// dboGetSampleSizeExcludeOutliers.H : Declaration of the CdboGetSampleSizeExcludeOutliers class

#ifndef __DBOGETSAMPLESIZEEXCLUDEOUTLIERS_H_
#define __DBOGETSAMPLESIZEEXCLUDEOUTLIERS_H_

class CdboGetSampleSizeExcludeOutliersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_sampleID[51];
	LONG m_column0;

BEGIN_PARAM_MAP(CdboGetSampleSizeExcludeOutliersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_sampleID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleSizeExcludeOutliersAccessor)
	COLUMN_ENTRY(1, m_column0)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLESIZEEXCLUDEOUTLIERS_H_
