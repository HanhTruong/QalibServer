HRESULT Template_80_20();

HRESULT Number_Of_Controls(LONG calibrationID,
						   CString chemistry,
						   CString calibrationSpecies,
						   double minNumberOfControls,
						   VARIANT *error_msg);

HRESULT DeIonizedWater(LONG calibrationID,
					   CString chemistry,
					   VARIANT *error_msg);

HRESULT Diluent_Factors();


HRESULT Level_Limits();

HRESULT Max_Instrument_Sample_Runs(
			LONG calibrationID,
			double maxSampleRuns, 
			VARIANT *error_msg);

HRESULT Number_Of_Calibrators(LONG calibrationID,
							  CString chemistry,
							  CString calibrationSpecies,
							  double minNumberOfCalibrators,
							  VARIANT *error_msg);

HRESULT Min_Number_Instruments(double minNumberOfInstruments, 
							   VARIANT *vSerialNumberList,
							   VARIANT *error_msg);

HRESULT Min_Number_Of_Disks_Per_Sample(
			LONG calibrationID, 
			double numberOfDisks, 
			VARIANT *error_msg);

HRESULT Mold_Factors();
HRESULT Number_Threshold_Samples();
HRESULT Rotor_Factors();
HRESULT Saline(LONG calibrationID, CString chemistry, VARIANT *error_msg);
HRESULT Sample_Blank();
HRESULT Threshold_Value();
HRESULT Wavelengths();

bool TopFactor(	CArray<double,double> *assignedValues);
