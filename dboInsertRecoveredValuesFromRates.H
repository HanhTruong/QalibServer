// dboInsertRecoveredValuesFromRates.H : Declaration of the CdboInsertRecoveredValuesFromRates class

#ifndef __DBOINSERTRECOVEREDVALUESFROMRATES_H_
#define __DBOINSERTRECOVEREDVALUESFROMRATES_H_

class CdboInsertRecoveredValuesFromRatesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_SampleType[51];
	double m_ccoe;
	double m_Slope;
	double m_SlopeStdError;
	double m_Intercept;
	double m_InterceptStdError;
	TCHAR m_Assigned[7001];
	TCHAR m_Recovered[7001];

BEGIN_PARAM_MAP(CdboInsertRecoveredValuesFromRatesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_SampleType)
	COLUMN_ENTRY(4, m_ccoe)
	COLUMN_ENTRY(5, m_Slope)
	COLUMN_ENTRY(6, m_SlopeStdError)
	COLUMN_ENTRY(7, m_Intercept)
	COLUMN_ENTRY(8, m_InterceptStdError)
	COLUMN_ENTRY(9, m_Assigned)
	COLUMN_ENTRY(10, m_Recovered)
END_PARAM_MAP()

};

#endif // __DBOINSERTRECOVEREDVALUESFROMRATES_H_
