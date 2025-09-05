// dboGetAssignedValueLimits.H : Declaration of the CdboGetAssignedValueLimits class

#ifndef __DBOGETASSIGNEDVALUELIMITS_H_
#define __DBOGETASSIGNEDVALUELIMITS_H_

class CdboGetAssignedValueLimitsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistryID[257];
	double m_colLower;
	double m_colUpper;

BEGIN_PARAM_MAP(CdboGetAssignedValueLimitsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistryID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetAssignedValueLimitsAccessor)
	COLUMN_ENTRY(1, m_colLower)
	COLUMN_ENTRY(2, m_colUpper)
END_COLUMN_MAP()

};

#endif // __DBOGETASSIGNEDVALUELIMITS_H_
