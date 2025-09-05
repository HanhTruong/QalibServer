// dboGetAssignedValues.H : Declaration of the CdboGetAssignedValues class

#ifndef __DBOGETASSIGNEDVALUES_H_
#define __DBOGETASSIGNEDVALUES_H_

class CdboGetAssignedValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_ChemistryName[51];
	TCHAR m_SampleID[7901];
	TCHAR m_SampleStatus[513];
	TCHAR m_AssignedValues[4901];

BEGIN_PARAM_MAP(CdboGetAssignedValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_ChemistryName)
	COLUMN_ENTRY(3, m_SampleID)
	SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(4, m_SampleStatus)
	COLUMN_ENTRY(5, m_AssignedValues)
END_PARAM_MAP()

};

#endif // __DBOGETASSIGNEDVALUES_H_
