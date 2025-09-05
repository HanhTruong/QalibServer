// dboRotorCuvetteAttributes.H : Declaration of the CdboRotorCuvetteAttributes class

#ifndef __DBOROTORCUVETTEATTRIBUTES_H_
#define __DBOROTORCUVETTEATTRIBUTES_H_

class CdboRotorCuvetteAttributesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_rotorID;
	TCHAR m_chemistry[257];
	LONG m_colCuvette;

BEGIN_PARAM_MAP(CdboRotorCuvetteAttributesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_rotorID)
	COLUMN_ENTRY(3, m_chemistry)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboRotorCuvetteAttributesAccessor)
	COLUMN_ENTRY(1, m_colCuvette)
END_COLUMN_MAP()

};

#endif // __DBOROTORCUVETTEATTRIBUTES_H_
