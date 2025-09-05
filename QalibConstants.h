// QalibConstants.h: interface for the QalibConstants class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QALIBCONSTANTS_H__AB0C61FB_F085_4EA8_A53F_91B05336B72A__INCLUDED_)
#define AFX_QALIBCONSTANTS_H__AB0C61FB_F085_4EA8_A53F_91B05336B72A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class QalibConstants  
{
private:
	double max_double_value;
	double min_double_value;

public:
	QalibConstants();
	virtual ~QalibConstants();
	double GetMaxDoubleValue();
	double GetMinDoubleValue();
};

#endif // !defined(AFX_QALIBCONSTANTS_H__AB0C61FB_F085_4EA8_A53F_91B05336B72A__INCLUDED_)
