// dboGetSampleSizeIncludeOutliers.H : Declaration of the CdboGetSampleSizeIncludeOutliers class

#ifndef __DBOGETSAMPLESIZEINCLUDEOUTLIERS_H_
#define __DBOGETSAMPLESIZEINCLUDEOUTLIERS_H_

class CdboGetSampleSizeIncludeOutliersAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_searchSampleId[51];
	LONG m_column0;

BEGIN_PARAM_MAP(CdboGetSampleSizeIncludeOutliersAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_searchSampleId)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleSizeIncludeOutliersAccessor)
	COLUMN_ENTRY(1, m_column0)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLESIZEINCLUDEOUTLIERS_H_
