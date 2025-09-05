#include <afxtempl.h>

typedef CMap< CString, LPCSTR, int, int > analyteLevel;

class AnalyteType
{
private:
	analyteLevel _currentTypes;

public:
	analyteLevel* GetCurrentTypes();
	bool InitializeAnalyteTypes(AnalyteType *anlyteType);
	AnalyteType();
	~AnalyteType();
};
