// dboGetPlotResults.H : Declaration of the CdboGetPlotResults class

#ifndef __DBOGETPLOTRESULTS_H_
#define __DBOGETPLOTRESULTS_H_

class CdboGetPlotResultsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_plotData[7901];

BEGIN_PARAM_MAP(CdboGetPlotResultsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_plotData)
END_PARAM_MAP()

};

#endif // __DBOGETPLOTRESULTS_H_
