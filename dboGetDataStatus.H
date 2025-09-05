// dboGetDataStatus.H : Declaration of the CdboGetDataStatus class

#ifndef __DBOGETDATASTATUS_H_
#define __DBOGETDATASTATUS_H_

class CdboGetDataStatusAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	LONG m_colStatus;

BEGIN_PARAM_MAP(CdboGetDataStatusAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetDataStatusAccessor)
	COLUMN_ENTRY(1, m_colStatus)
END_COLUMN_MAP()

};

#endif // __DBOGETDATASTATUS_H_
