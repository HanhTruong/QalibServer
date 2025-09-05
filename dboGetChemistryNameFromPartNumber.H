// dboGetChemistryNameFromPartNumber.H : Declaration of the CdboGetChemistryNameFromPartNumber class

#ifndef __DBOGETCHEMISTRYNAMEFROMPARTNUMBER_H_
#define __DBOGETCHEMISTRYNAMEFROMPARTNUMBER_H_

class CdboGetChemistryNameFromPartNumberAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_partNumber[257];
	TCHAR m_colChemistryName[257];

BEGIN_PARAM_MAP(CdboGetChemistryNameFromPartNumberAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_partNumber)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetChemistryNameFromPartNumberAccessor)
	COLUMN_ENTRY(1, m_colChemistryName)
END_COLUMN_MAP()

};

#endif // __DBOGETCHEMISTRYNAMEFROMPARTNUMBER_H_
