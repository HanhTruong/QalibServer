// dboGetPlotData.H : Declaration of the CdboGetPlotData class

#ifndef __DBOGETPLOTDATA_H_
#define __DBOGETPLOTDATA_H_

class CdboGetPlotDataAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_sampleID[51];
	double m_colValue;

BEGIN_PARAM_MAP(CdboGetPlotDataAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_sampleID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetPlotDataAccessor)
	COLUMN_ENTRY(1, m_colValue)
END_COLUMN_MAP()

};

#endif // __DBOGETPLOTDATA_H_
