// dboGetDiluent.H : Declaration of the CdboGetDiluent class

#ifndef __DBOGETDILUENT_H_
#define __DBOGETDILUENT_H_

class CdboGetDiluentAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colDiluent[257];

BEGIN_PARAM_MAP(CdboGetDiluentAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetDiluentAccessor)
	COLUMN_ENTRY(1, m_colDiluent)
END_COLUMN_MAP()

};

#endif // __DBOGETDILUENT_H_
