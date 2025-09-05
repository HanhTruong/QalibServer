// dboGetRecoveredValuesFromRates.H : Declaration of the CdboGetRecoveredValuesFromRates class

#ifndef __DBOGETRECOVEREDVALUESFROMRATES_H_
#define __DBOGETRECOVEREDVALUESFROMRATES_H_

class CdboGetRecoveredValuesFromRatesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_inAssignedValuesID;
	double m_colAssigned;
	double m_colRecovered;

BEGIN_PARAM_MAP(CdboGetRecoveredValuesFromRatesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_inAssignedValuesID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetRecoveredValuesFromRatesAccessor)
	COLUMN_ENTRY(1, m_colAssigned)
	COLUMN_ENTRY(2, m_colRecovered)
END_COLUMN_MAP()

};

#endif // __DBOGETRECOVEREDVALUESFROMRATES_H_
