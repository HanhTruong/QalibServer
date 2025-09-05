// dboTestSampleType.H : Declaration of the CdboTestSampleType class

#ifndef __DBOTESTSAMPLETYPE_H_
#define __DBOTESTSAMPLETYPE_H_

class CdboTestSampleTypeAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_type[51];

BEGIN_PARAM_MAP(CdboTestSampleTypeAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_type)
END_PARAM_MAP()

};

#endif // __DBOTESTSAMPLETYPE_H_
