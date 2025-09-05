// QalibServerMain.h: Definition of the QalibServerMain class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QALIBSERVERMAIN_H__C5EB02CD_E48F_4B8A_8369_6BC2F07CD49A__INCLUDED_)
#define AFX_QALIBSERVERMAIN_H__C5EB02CD_E48F_4B8A_8369_6BC2F07CD49A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include <mtx.h>
#include "QalibStructs.h"
#include "QalibServer.h"

/////////////////////////////////////////////////////////////////////////////
// QalibServerMain

class QalibServerMain : 
	public CComObjectRoot,
	public CComCoClass<QalibServerMain,&CLSID_QalibServerMain>,
	public IDispatchImpl<IQalibServerMain, &IID_IQalibServerMain, &LIBID_QALIBSERVERLib>, 
	public ISupportErrorInfo
{
public:
	QalibServerMain() {}
BEGIN_COM_MAP(QalibServerMain)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IQalibServerMain)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(QalibServerMain) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_QalibServerMain)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IQalibServerMain
public:
	STDMETHOD(ReceiveClientData)(/*[in]*/ BSTR chemistry,/*[in]*/ INT  outlierMethod,/*[in]*/ BSTR mode,/*[in]*/ BSTR user,/*[in]*/ VARIANT *inputData,/*[in]*/ VARIANT *serialNumber,/*[in]*/ VARIANT *barCode,/*[in]*/ VARIANT *sampleSpecies,/*[in]*/ VARIANT *statusData,/*[in,out]*/ LONG *calibrationID,/*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(SendUserRights)(/*[in]*/ BSTR user,/*[in]*/ BSTR password,/*[out]*/ VARIANT *modeList,/*[out]*/ VARIANT *chemistryList,/*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(SendCalibrationResults)(/*[in]*/ LONG calibrationID,/*[out]*/ BSTR *chemistry,/*[out]*/ BSTR *waveLength,/*[out]*/ BSTR *species,/*[out]*/ BSTR *diluent,/*[out]*/ BSTR *mold,/*[out]*/ BSTR *rotor,/*[out]*/ BSTR *cuvette,/*[out]*/ BSTR *mode,/*[out]*/ BSTR *user,/*[out]*/ BSTR *date,/*[out]*/ VARIANT *description,/*[out]*/ VARIANT *originalFitParameters,/*[out]*/ VARIANT *newFitParameters,/*[out]*/ VARIANT *minLimits,/*[out]*/ VARIANT *maxLimits,/*[out]*/ VARIANT *originalDisposition,/*[out]*/ VARIANT *newDisposition,/*[out]*/ VARIANT *calibrationData,/*[out]*/ VARIANT *statusData,/*[out]*/ VARIANT *rates,/*[out]*/ VARIANT *plotData,/*[out]*/ BSTR	*comment,/*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(SendAssignedValues)(/*[in]*/ BSTR chemistry, /*[in]*/ const VARIANT *sampleIDs, /*[out]*/ VARIANT *curAssignedValues, /*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(SendEditableFitParameters)(/*[in]*/ LONG calibrationID, /*[out]*/ VARIANT *editableFitParameters, /*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(SetComment)(/*[in]*/ LONG calibrationID, /*[in]*/ BSTR *comment, /*[out,retval]*/ VARIANT *error_msg);
	STDMETHOD(GetUpdatedFitParameters)(/*[in]*/ LONG calibrationID, /*[in]*/ VARIANT *newFitParameters, /*[out,retval]*/ VARIANT *error_msg);

protected:
	// Calibration data structure.
	// Current data set to be used for calibration.
	CalibrationDataMap			CalibrationData;
	
	// List of sample values status.
	CalibrationDataStatusMap	CalibrationDataStatus;

	// List of sample species.
	SpeciesMap					SampleSpecies;

	// List of sample types.
	TypesMap					SampleType;

	// List of sample level.
	LevelMap					SampleLevel;

	AssignedValueMap			AssignedValues;

	// Current samples creation dates.
	SampleDatesMap				SampleDates;

	CStringList					testList;
	
};

#endif // !defined(AFX_QALIBSERVERMAIN_H__C5EB02CD_E48F_4B8A_8369_6BC2F07CD49A__INCLUDED_)
