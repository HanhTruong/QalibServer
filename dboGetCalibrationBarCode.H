// dboGetCalibrationBarCode.H : Declaration of the CdboGetCalibrationBarCode class

#ifndef __DBOGETCALIBRATIONBARCODE_H_
#define __DBOGETCALIBRATIONBARCODE_H_

class CdboGetCalibrationBarCodeAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colBarCode[111];

BEGIN_PARAM_MAP(CdboGetCalibrationBarCodeAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetCalibrationBarCodeAccessor)
	COLUMN_ENTRY(1, m_colBarCode)
END_COLUMN_MAP()

};

#endif // __DBOGETCALIBRATIONBARCODE_H_
