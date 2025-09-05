// dboGetSampleStatus.H : Declaration of the CdboGetSampleStatus class

#ifndef __DBOGETSAMPLESTATUS_H_
#define __DBOGETSAMPLESTATUS_H_

class CdboGetSampleStatusAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_SampleId[257];
	LONG m_NumberOfSets;
	TCHAR m_SampleStatus[2001];

BEGIN_PARAM_MAP(CdboGetSampleStatusAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_SampleId)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_NumberOfSets)
	COLUMN_ENTRY(5, m_SampleStatus)
END_PARAM_MAP()

};

#endif // __DBOGETSAMPLESTATUS_H_
