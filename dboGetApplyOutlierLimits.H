// dboGetApplyOutlierLimits.H : Declaration of the CdboGetApplyOutlierLimits class

#ifndef __DBOGETAPPLYOUTLIERLIMITS_H_
#define __DBOGETAPPLYOUTLIERLIMITS_H_

class CdboGetApplyOutlierLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[51];
	TCHAR m_SampleIDList[7901];
	TCHAR m_ApplyOutliers[7901];

BEGIN_PARAM_MAP(CdboGetApplyOutlierLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_ApplyOutliers)
END_PARAM_MAP()

};

#endif // __DBOGETAPPLYOUTLIERLIMITS_H_
