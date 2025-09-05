// dboGetCalibrationData.H : Declaration of the CdboGetCalibrationData class

#ifndef __DBOGETCALIBRATIONDATA_H_
#define __DBOGETCALIBRATIONDATA_H_

class CdboGetCalibrationDataAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_sampleIds[3001];
	TCHAR m_sampleSizes[2001];
	TCHAR m_plotData[4001];
	TCHAR m_dataStatus[2001];
	TCHAR m_sampleSpecies[2001];

BEGIN_PARAM_MAP(CdboGetCalibrationDataAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_sampleIds)
	COLUMN_ENTRY(4, m_sampleSizes)
	COLUMN_ENTRY(5, m_plotData)
	COLUMN_ENTRY(6, m_dataStatus)
	COLUMN_ENTRY(7, m_sampleSpecies)
END_PARAM_MAP()

};

#endif // __DBOGETCALIBRATIONDATA_H_
