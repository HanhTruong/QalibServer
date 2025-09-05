// dboUserAllowedAccess.H : Declaration of the CdboUserAllowedAccess class

#ifndef __DBOUSERALLOWEDACCESS_H_
#define __DBOUSERALLOWEDACCESS_H_

class CdboUserAllowedAccessAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_UserName[257];
	TCHAR m_ModeList[2001];
	TCHAR m_ChemistryList[2001];

BEGIN_PARAM_MAP(CdboUserAllowedAccessAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_UserName)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_ModeList)
	COLUMN_ENTRY(4, m_ChemistryList)
END_PARAM_MAP()

};

#endif // __DBOUSERALLOWEDACCESS_H_
