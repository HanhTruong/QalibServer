// dboGetGoodnessOfFitTests.H : Declaration of the CdboGetGoodnessOfFitTests class

#ifndef __DBOGETGOODNESSOFFITTESTS_H_
#define __DBOGETGOODNESSOFFITTESTS_H_

class CdboGetGoodnessOfFitTestsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colTest[51];

BEGIN_PARAM_MAP(CdboGetGoodnessOfFitTestsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetGoodnessOfFitTestsAccessor)
	COLUMN_ENTRY(1, m_colTest)
END_COLUMN_MAP()

};

#endif // __DBOGETGOODNESSOFFITTESTS_H_
