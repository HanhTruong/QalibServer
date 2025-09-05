// dboGetPartNumber.H : Declaration of the CdboGetPartNumber class

#ifndef __DBOGETPARTNUMBER_H_
#define __DBOGETPARTNUMBER_H_

class CdboGetPartNumberAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colPartNumber[257];

BEGIN_PARAM_MAP(CdboGetPartNumberAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetPartNumberAccessor)
	COLUMN_ENTRY(1, m_colPartNumber)
END_COLUMN_MAP()

};

#endif // __DBOGETPARTNUMBER_H_
