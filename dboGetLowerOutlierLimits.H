// dboGetLowerOutlierLimits.H : Declaration of the CdboGetLowerOutlierLimits class

#ifndef __DBOGETLOWEROUTLIERLIMITS_H_
#define __DBOGETLOWEROUTLIERLIMITS_H_

class CdboGetLowerOutlierLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[51];
	TCHAR m_SampleIDList[7901];
	TCHAR m_LowerOutliers[7901];

BEGIN_PARAM_MAP(CdboGetLowerOutlierLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_LowerOutliers)
END_PARAM_MAP()

};

#endif // __DBOGETLOWEROUTLIERLIMITS_H_
