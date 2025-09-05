void RemoveExcludedData(CPtrArray *datasets,
						CPtrArray *statussets,
						CArray<long,long> *indexingList,
						CStringArray *sampleids,
						CArray<int,int> *samplesizes);

HRESULT NumberExcluded(LONG calibrationID,
					   CString mode,
					   CStringArray *sampleids,
					   VARIANT *error_msg);
