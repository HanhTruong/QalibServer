// dboGetChemistryName.H : Declaration of the CdboGetChemistryName class

#ifndef __DBOGETCHEMISTRYNAME_H_
#define __DBOGETCHEMISTRYNAME_H_

class CdboGetChemistryNameAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colChemistryID[257];

BEGIN_PARAM_MAP(CdboGetChemistryNameAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetChemistryNameAccessor)
	COLUMN_ENTRY(1, m_colChemistryID)
END_COLUMN_MAP()

};

#endif // __DBOGETCHEMISTRYNAME_H_
