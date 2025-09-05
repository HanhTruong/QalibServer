// dboGetListOfSamplesToPlot.H : Declaration of the CdboGetListOfSamplesToPlot class

#ifndef __DBOGETLISTOFSAMPLESTOPLOT_H_
#define __DBOGETLISTOFSAMPLESTOPLOT_H_

class CdboGetListOfSamplesToPlotAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_type[51];
	TCHAR m_colSampleID[257];

BEGIN_PARAM_MAP(CdboGetListOfSamplesToPlotAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_type)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetListOfSamplesToPlotAccessor)
	COLUMN_ENTRY(1, m_colSampleID)
END_COLUMN_MAP()

};

#endif // __DBOGETLISTOFSAMPLESTOPLOT_H_
