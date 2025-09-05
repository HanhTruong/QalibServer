// dboGetAssignedValuesToPlot.H : Declaration of the CdboGetAssignedValuesToPlot class

#ifndef __DBOGETASSIGNEDVALUESTOPLOT_H_
#define __DBOGETASSIGNEDVALUESTOPLOT_H_

class CdboGetAssignedValuesToPlotAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_sampleID[51];
	double m_colAssignedValue;

BEGIN_PARAM_MAP(CdboGetAssignedValuesToPlotAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_sampleID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetAssignedValuesToPlotAccessor)
	COLUMN_ENTRY(1, m_colAssignedValue)
END_COLUMN_MAP()

};

#endif // __DBOGETASSIGNEDVALUESTOPLOT_H_
