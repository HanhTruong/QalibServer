// dboGetSampleTypeIDs.H : Declaration of the CdboGetSampleTypeIDs class

#ifndef __DBOGETSAMPLETYPEIDS_H_
#define __DBOGETSAMPLETYPEIDS_H_

class CdboGetSampleTypeIDsAccessor
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_colSampleTypeID[51];

BEGIN_PARAM_MAP(CdboGetSampleTypeIDsAccessor)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CdboGetSampleTypeIDsAccessor)
	COLUMN_ENTRY(1, m_colSampleTypeID)
END_COLUMN_MAP()

};

#endif // __DBOGETSAMPLETYPEIDS_H_


