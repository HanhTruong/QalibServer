// dboGetFixedSlopeValues.H : Declaration of the CdboGetFixedSlopeValues class

#ifndef __DBOGETFIXEDSLOPEVALUES_H_
#define __DBOGETFIXEDSLOPEVALUES_H_

class CdboGetFixedSlopeValuesAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_chemistry[257];
	TCHAR m_Diluent[51];
	double m_colSlope;
	double m_colLowerLimitPercent;
	double m_colUpperLimitPercent;

BEGIN_PARAM_MAP(CdboGetFixedSlopeValuesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_chemistry)
	COLUMN_ENTRY(3, m_Diluent)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetFixedSlopeValuesAccessor)
	COLUMN_ENTRY(1, m_colSlope)
	COLUMN_ENTRY(2, m_colLowerLimitPercent)
	COLUMN_ENTRY(3, m_colUpperLimitPercent)
END_COLUMN_MAP()

};

#endif // __DBOGETFIXEDSLOPEVALUES_H_
