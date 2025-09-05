// dboGetCalculatedAssignedValues.H : Declaration of the CdboGetCalculatedAssignedValues class

#ifndef __DBOGETCALCULATEDASSIGNEDVALUES_H_
#define __DBOGETCALCULATEDASSIGNEDVALUES_H_

class CdboGetCalculatedAssignedValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	LONG m_colCalculatedAssignedValuesID;
	TCHAR m_colSampleType[51];
	double m_colccoe;
	double m_colSlope;
	double m_colSlopeStdError;
	double m_colIntercept;
	double m_colInterceptStdError;

BEGIN_PARAM_MAP(CdboGetCalculatedAssignedValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetCalculatedAssignedValuesAccessor)
	COLUMN_ENTRY(1, m_colCalculatedAssignedValuesID)
	COLUMN_ENTRY(2, m_colSampleType)
	COLUMN_ENTRY(3, m_colccoe)
	COLUMN_ENTRY(4, m_colSlope)
	COLUMN_ENTRY(5, m_colSlopeStdError)
	COLUMN_ENTRY(6, m_colIntercept)
	COLUMN_ENTRY(7, m_colInterceptStdError)
END_COLUMN_MAP()

};

#endif // __DBOGETCALCULATEDASSIGNEDVALUES_H_
