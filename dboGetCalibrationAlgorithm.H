// dboGetCalibrationAlgorithm.H : Declaration of the CdboGetCalibrationAlgorithm class

#ifndef __DBOGETCALIBRATIONALGORITHM_H_
#define __DBOGETCALIBRATIONALGORITHM_H_

class CdboGetCalibrationAlgorithmAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_Algorithm[257];

BEGIN_PARAM_MAP(CdboGetCalibrationAlgorithmAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(3, m_Algorithm)
END_PARAM_MAP()

};

#endif // __DBOGETCALIBRATIONALGORITHM_H_
