// dboGetOutlierLimits.H : Declaration of the CdboGetOutlierLimits class

#ifndef __DBOGETOUTLIERLIMITS_H_
#define __DBOGETOUTLIERLIMITS_H_

class CdboGetOutlierLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_PartNumber[51];
	TCHAR m_SampleIDList[2001];
	TCHAR m_UpperOutliers[7901];

BEGIN_PARAM_MAP(CdboGetOutlierLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_PartNumber)
	COLUMN_ENTRY(3, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_UpperOutliers)
END_PARAM_MAP()
};

#endif // __DBOGETOUTLIERLIMITS_H_
