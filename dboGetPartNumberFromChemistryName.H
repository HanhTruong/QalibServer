// dboGetPartNumberFromChemistryName.H : Declaration of the CdboGetPartNumberFromChemistryName class

#ifndef __DBOGETPARTNUMBERFROMCHEMISTRYNAME_H_
#define __DBOGETPARTNUMBERFROMCHEMISTRYNAME_H_

class CdboGetPartNumberFromChemistryNameAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistryName[257];
	TCHAR m_colPartNumber[257];

BEGIN_PARAM_MAP(CdboGetPartNumberFromChemistryNameAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistryName)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetPartNumberFromChemistryNameAccessor)
	COLUMN_ENTRY(1, m_colPartNumber)
END_COLUMN_MAP()

};

#endif // __DBOGETPARTNUMBERFROMCHEMISTRYNAME_H_
