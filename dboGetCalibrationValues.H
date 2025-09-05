// dboGetCalibrationValues.H : Declaration of the CdboGetCalibrationValues class

#ifndef __DBOGETCALIBRATIONVALUES_H_
#define __DBOGETCALIBRATIONVALUES_H_

class CdboGetCalibrationValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colChemistryName[257];
	TCHAR m_column0[24];
	TCHAR m_colMold[51];
	TCHAR m_colRotor[51];
	TCHAR m_colSpecies[257];
	TCHAR m_colMode[51];
	TCHAR m_colUser[257];
	TCHAR m_colComment[1001];

BEGIN_PARAM_MAP(CdboGetCalibrationValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetCalibrationValuesAccessor)
	COLUMN_ENTRY(1, m_colChemistryName)
	COLUMN_ENTRY(2, m_column0)
	COLUMN_ENTRY(3, m_colMold)
	COLUMN_ENTRY(4, m_colRotor)
	COLUMN_ENTRY(5, m_colSpecies)
	COLUMN_ENTRY(6, m_colMode)
	COLUMN_ENTRY(7, m_colUser)
	COLUMN_ENTRY(8, m_colComment)
END_COLUMN_MAP()

};

#endif // __DBOGETCALIBRATIONVALUES_H_
