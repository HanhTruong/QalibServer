// dboGetWeightMethod.H : Declaration of the CdboGetWeightMethod class

#ifndef __DBOGETWEIGHTMETHOD_H_
#define __DBOGETWEIGHTMETHOD_H_

class CdboGetWeightMethodAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colWeightMethod[51];

BEGIN_PARAM_MAP(CdboGetWeightMethodAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetWeightMethodAccessor)
	COLUMN_ENTRY(1, m_colWeightMethod)
END_COLUMN_MAP()

};

#endif // __DBOGETWEIGHTMETHOD_H_
