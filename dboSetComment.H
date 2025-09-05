// dboSetComment.H : Declaration of the CdboSetComment class

#ifndef __DBOSETCOMMENT_H_
#define __DBOSETCOMMENT_H_

class CdboSetCommentAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_comment[1001];

BEGIN_PARAM_MAP(CdboSetCommentAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_comment)
END_PARAM_MAP()

};

#endif // __DBOSETCOMMENT_H_
