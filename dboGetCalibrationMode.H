// dboGetCalibrationMode.H : Declaration of the CdboGetCalibrationMode class

#ifndef __DBOGETCALIBRATIONMODE_H_
#define __DBOGETCALIBRATIONMODE_H_

class CdboGetCalibrationModeAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colMode[51];

BEGIN_PARAM_MAP(CdboGetCalibrationModeAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetCalibrationModeAccessor)
	COLUMN_ENTRY(1, m_colMode)
END_COLUMN_MAP()

};

#endif // __DBOGETCALIBRATIONMODE_H_
