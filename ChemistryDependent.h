typedef HRESULT (*chemistry_function)(CPtrArray *);

HRESULT ALB_Albumin_Bromo_Cresol(CPtrArray *datasets);
HRESULT ALP_Alkaline_Phosphatase(CPtrArray *datasets);
HRESULT ALT_Alanine_Aminotransferase(CPtrArray *datasets);
HRESULT AMY_Amylase(CPtrArray *datasets);
HRESULT AST_Aspartate_Aminotransferase(CPtrArray *datasets);
HRESULT BUN_Urea_Nitrogen(CPtrArray *datasets);
HRESULT CA_Calcium(CPtrArray *datasets);
HRESULT CHOL_Cholesterol(CPtrArray *datasets);
HRESULT CK_Creatine_Kinase(CPtrArray *datasets);
HRESULT CL_Chloride(CPtrArray *datasets);
HRESULT CRE_Creatinine(CPtrArray *datasets);
HRESULT DBIL_Direct_Bilirubin(CPtrArray *datasets);
HRESULT GGT_Gamma_Glutamyl_Transferase(CPtrArray *datasets);
HRESULT GLU_Glucose(CPtrArray *datasets);
HRESULT HDL_High_Density_Lipoprotein_Perciptating_reagent(CPtrArray *datasets);
HRESULT IQC_Instrument_Qualit_Control(CPtrArray *datasets);
HRESULT K_ENZ_Potassium(CPtrArray *datasets);
HRESULT MG_Magnesium(CPtrArray *datasets);
HRESULT NA_Sodium(CPtrArray *datasets);
HRESULT PHOS_Phosphorous(CPtrArray *datasets);
HRESULT T_4_1_Thyroxine(CPtrArray *datasets);
HRESULT TBIL_Total_Bilirubin(CPtrArray *datasets);
HRESULT tCO2_Total_Carbon_Dioxide(CPtrArray *datasets);
HRESULT TP_Total_Protein(CPtrArray *datasets);
HRESULT TRIG_Triglycerides(CPtrArray *datasets);
HRESULT UA_Uric_Acid(CPtrArray *datasets);

HRESULT ChemistryDependencies(LONG calibrationID,
							  CPtrArray *datasets,
							  VARIANT *error_msgs);

//int FindChemistryIndex(CString chemistry);
void AssignFunctions();
