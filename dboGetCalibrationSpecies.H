// dboGetCalibrationSpecies.H : Declaration of the CdboGetCalibrationSpecies class

#ifndef __DBOGETCALIBRATIONSPECIES_H_
#define __DBOGETCALIBRATIONSPECIES_H_

class CdboGetCalibrationSpeciesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_colCalibrationSpecies[257];

BEGIN_PARAM_MAP(CdboGetCalibrationSpeciesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetCalibrationSpeciesAccessor)
	COLUMN_ENTRY(1, m_colCalibrationSpecies)
END_COLUMN_MAP()

};

#endif // __DBOGETCALIBRATIONSPECIES_H_
