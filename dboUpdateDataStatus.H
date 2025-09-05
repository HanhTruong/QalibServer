// dboUpdateDataStatus.H : Declaration of the CdboUpdateDataStatus class

#ifndef __DBOUPDATEDATASTATUS_H_
#define __DBOUPDATEDATASTATUS_H_

class CdboUpdateDataStatusAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_indexingList[8001];
	TCHAR m_dataStatus[2001];

BEGIN_PARAM_MAP(CdboUpdateDataStatusAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_indexingList)
	COLUMN_ENTRY(4, m_dataStatus)
END_PARAM_MAP()

};

#endif // __DBOUPDATEDATASTATUS_H_
