// dboGetSampleNames.H : Declaration of the CdboGetSampleNames class

#ifndef __DBOGETSAMPLENAMES_H_
#define __DBOGETSAMPLENAMES_H_

class CdboGetSampleNamesAccessor
{
public:
	LONG m_RETURNVALUE;
	LONG m_calibrationID;
	TCHAR m_colSampleID[51];

BEGIN_PARAM_MAP(CdboGetSampleNamesAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_calibrationID)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleNamesAccessor)
	COLUMN_ENTRY(1, m_colSampleID)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLENAMES_H_
