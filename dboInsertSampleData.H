// dboInsertSampleData.H : Declaration of the CdboInsertSampleData class

#ifndef __DBOINSERTSAMPLEDATA_H_
#define __DBOINSERTSAMPLEDATA_H_

class CdboInsertSampleDataAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	LONG m_mold;
	LONG m_rotor;
	TCHAR m_species[257];
	TCHAR m_mode[257];
	TCHAR m_user[257];
	TCHAR m_sampleData[8001];
	TCHAR m_sampleSizes[7901];
	TCHAR m_sampleStatus[7901];
	TCHAR m_sampleIds[8001];
	TCHAR m_sampleSpecies[8001];
	TCHAR m_sampleSerialNumbers[8001];
//	TCHAR m_sampleBarCode[1024];
	TCHAR m_sampleBarCode[65000];
	LONG m_BarCodeLen;
	TCHAR m_setAttributes[1001];
	TCHAR m_Comment[1001];
	LONG m_calibrationID;

BEGIN_PARAM_MAP(CdboInsertSampleDataAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_mold)
	COLUMN_ENTRY(4, m_rotor)
	COLUMN_ENTRY(5, m_species)
	COLUMN_ENTRY(6, m_mode)
	COLUMN_ENTRY(7, m_user)
	COLUMN_ENTRY(8, m_sampleData)
	COLUMN_ENTRY(9, m_sampleSizes)
	COLUMN_ENTRY(10, m_sampleStatus)
	COLUMN_ENTRY(11, m_sampleIds)
	COLUMN_ENTRY(12, m_sampleSpecies)
	COLUMN_ENTRY(13, m_sampleSerialNumbers)
	COLUMN_ENTRY(14, m_sampleBarCode)
	COLUMN_ENTRY(15, m_BarCodeLen)
	COLUMN_ENTRY(16, m_setAttributes)
	COLUMN_ENTRY(17, m_Comment)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(18, m_calibrationID)
END_PARAM_MAP()

};

#endif // __DBOINSERTSAMPLEDATA_H_
