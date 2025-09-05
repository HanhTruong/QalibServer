// dboGetSampleInstruments.H : Declaration of the CdboGetSampleInstruments class

#ifndef __DBOGETSAMPLEINSTRUMENTS_H_
#define __DBOGETSAMPLEINSTRUMENTS_H_

class CdboGetSampleInstrumentsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_sampleID[257];
	TCHAR m_colInstrument[257];

BEGIN_PARAM_MAP(CdboGetSampleInstrumentsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	COLUMN_ENTRY(3, m_sampleID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleInstrumentsAccessor)
	COLUMN_ENTRY(1, m_colInstrument)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLEINSTRUMENTS_H_
