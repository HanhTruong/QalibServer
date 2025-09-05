#define BARCODEHEADER 16
#define BARCODELENGTH 110

typedef enum BARCODEATTRIBUTES {ROTORNUMBER, BARCODEFORMATNUMBER, EXPIRATIONDATE, ROTORLOTNUMBER, MOLDNUMBER};

HRESULT VerifyBarCodes(CStringArray *l_sampleBarCode, 
					   VARIANT *error_msg);

HRESULT GetBarCodeAttributes(CString barCode, 
							 CStringArray *barCodeAttributes, 
							 VARIANT *error_msg);

HRESULT GetCalibrationBarCode(LONG calibrationID,
							  CString *barCode,
							  VARIANT *error_msg);
