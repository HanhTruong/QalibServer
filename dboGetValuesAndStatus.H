// dboGetValuesAndStatus.H : Declaration of the CdboGetValuesAndStatus class

#ifndef __DBOGETVALUESANDSTATUS_H_
#define __DBOGETVALUESANDSTATUS_H_

class CdboGetValuesAndStatusAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_outliers[257];
	TCHAR m_SampleIdList[7901];
	TCHAR m_SampleStatus[2001];
	TCHAR m_SampleValues[4001];

BEGIN_PARAM_MAP(CdboGetValuesAndStatusAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_outliers)
	COLUMN_ENTRY(4, m_SampleIdList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(5, m_SampleStatus)
	COLUMN_ENTRY(6, m_SampleValues)
END_PARAM_MAP()

};

#endif // __DBOGETVALUESANDSTATUS_H_
