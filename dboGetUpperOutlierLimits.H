// dboGetUpperOutlierLimits.H : Declaration of the CdboGetUpperOutlierLimits class

#ifndef __DBOGETUPPEROUTLIERLIMITS_H_
#define __DBOGETUPPEROUTLIERLIMITS_H_

class CdboGetUpperOutlierLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[51];
	TCHAR m_SampleIDList[7901];
	TCHAR m_UpperOutliers[7901];

BEGIN_PARAM_MAP(CdboGetUpperOutlierLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_SampleIDList)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_UpperOutliers)
END_PARAM_MAP()

};

#endif // __DBOGETUPPEROUTLIERLIMITS_H_
