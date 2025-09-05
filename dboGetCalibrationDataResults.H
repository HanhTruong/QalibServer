// dboGetCalibrationDataResults.H : Declaration of the CdboGetCalibrationDataResults class

#ifndef __DBOGETCALIBRATIONDATARESULTS_H_
#define __DBOGETCALIBRATIONDATARESULTS_H_

class CdboGetCalibrationDataResultsAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_CalibrationID;
	TCHAR m_resultsset[4001];
	TCHAR m_resultsstatus[2001];

BEGIN_PARAM_MAP(CdboGetCalibrationDataResultsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_CalibrationID)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_resultsset)
	COLUMN_ENTRY(4, m_resultsstatus)
END_PARAM_MAP()

};

#endif // __DBOGETCALIBRATIONDATARESULTS_H_
