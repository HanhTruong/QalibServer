// dboGetSampleValues.H : Declaration of the CdboGetSampleValues class

#ifndef __DBOGETSAMPLEVALUES_H_
#define __DBOGETSAMPLEVALUES_H_

class CdboGetSampleValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_sampleID[51];
	double m_colValue;

BEGIN_PARAM_MAP(CdboGetSampleValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_sampleID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleValuesAccessor)
	COLUMN_ENTRY(1, m_colValue)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLEVALUES_H_
