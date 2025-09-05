/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Oct 20 15:13:38 2005
 */
/* Compiler settings for C:\Qalib\QalibServer\QalibServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __QalibServer_h__
#define __QalibServer_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IQalibServerMain_FWD_DEFINED__
#define __IQalibServerMain_FWD_DEFINED__
typedef interface IQalibServerMain IQalibServerMain;
#endif 	/* __IQalibServerMain_FWD_DEFINED__ */


#ifndef __QalibServerMain_FWD_DEFINED__
#define __QalibServerMain_FWD_DEFINED__

#ifdef __cplusplus
typedef class QalibServerMain QalibServerMain;
#else
typedef struct QalibServerMain QalibServerMain;
#endif /* __cplusplus */

#endif 	/* __QalibServerMain_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IQalibServerMain_INTERFACE_DEFINED__
#define __IQalibServerMain_INTERFACE_DEFINED__

/* interface IQalibServerMain */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IQalibServerMain;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88FAF4C5-AE8B-4E05-B523-60087F1BCDC9")
    IQalibServerMain : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUpdatedFitParameters( 
            /* [in] */ LONG calibrationID,
            /* [in] */ VARIANT __RPC_FAR *newFitParameters,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetComment( 
            /* [in] */ LONG calibrationID,
            /* [in] */ BSTR __RPC_FAR *comment,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendEditableFitParameters( 
            /* [in] */ LONG calibrationID,
            /* [out] */ VARIANT __RPC_FAR *editableFitParameters,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendAssignedValues( 
            /* [in] */ BSTR chemistry,
            /* [in] */ const VARIANT __RPC_FAR *sampleIDs,
            /* [out] */ VARIANT __RPC_FAR *curAssignedValues,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendCalibrationResults( 
            /* [in] */ LONG calibrationID,
            /* [out] */ BSTR __RPC_FAR *chemistry,
            /* [out] */ BSTR __RPC_FAR *waveLength,
            /* [out] */ BSTR __RPC_FAR *species,
            /* [out] */ BSTR __RPC_FAR *diluent,
            /* [out] */ BSTR __RPC_FAR *mold,
            /* [out] */ BSTR __RPC_FAR *rotor,
            /* [out] */ BSTR __RPC_FAR *cuvette,
            /* [out] */ BSTR __RPC_FAR *mode,
            /* [out] */ BSTR __RPC_FAR *user,
            /* [out] */ BSTR __RPC_FAR *date,
            /* [out] */ VARIANT __RPC_FAR *description,
            /* [out] */ VARIANT __RPC_FAR *originalFitParameters,
            /* [out] */ VARIANT __RPC_FAR *newFitParameters,
            /* [out] */ VARIANT __RPC_FAR *minLimits,
            /* [out] */ VARIANT __RPC_FAR *maxLimits,
            /* [out] */ VARIANT __RPC_FAR *originalDisposition,
            /* [out] */ VARIANT __RPC_FAR *newDisposition,
            /* [out] */ VARIANT __RPC_FAR *calibrationData,
            /* [out] */ VARIANT __RPC_FAR *statusData,
            /* [out] */ VARIANT __RPC_FAR *rates,
            /* [out] */ VARIANT __RPC_FAR *plotData,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendUserRights( 
            /* [in] */ BSTR user,
            /* [in] */ BSTR password,
            /* [out] */ VARIANT __RPC_FAR *modeList,
            /* [out] */ VARIANT __RPC_FAR *chemistryList,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReceiveClientData( 
            /* [in] */ BSTR chemistry,
            /* [in] */ INT outlierMethod,
            /* [in] */ BSTR mode,
            /* [in] */ BSTR user,
            /* [in] */ VARIANT __RPC_FAR *inputData,
            /* [in] */ VARIANT __RPC_FAR *serialNumber,
            /* [in] */ VARIANT __RPC_FAR *barCode,
            /* [in] */ VARIANT __RPC_FAR *sampleSpecies,
            /* [in] */ VARIANT __RPC_FAR *statusData,
            /* [out][in] */ LONG __RPC_FAR *calibrationID,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQalibServerMainVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IQalibServerMain __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IQalibServerMain __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IQalibServerMain __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUpdatedFitParameters )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ LONG calibrationID,
            /* [in] */ VARIANT __RPC_FAR *newFitParameters,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetComment )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ LONG calibrationID,
            /* [in] */ BSTR __RPC_FAR *comment,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendEditableFitParameters )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ LONG calibrationID,
            /* [out] */ VARIANT __RPC_FAR *editableFitParameters,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAssignedValues )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ BSTR chemistry,
            /* [in] */ const VARIANT __RPC_FAR *sampleIDs,
            /* [out] */ VARIANT __RPC_FAR *curAssignedValues,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendCalibrationResults )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ LONG calibrationID,
            /* [out] */ BSTR __RPC_FAR *chemistry,
            /* [out] */ BSTR __RPC_FAR *waveLength,
            /* [out] */ BSTR __RPC_FAR *species,
            /* [out] */ BSTR __RPC_FAR *diluent,
            /* [out] */ BSTR __RPC_FAR *mold,
            /* [out] */ BSTR __RPC_FAR *rotor,
            /* [out] */ BSTR __RPC_FAR *cuvette,
            /* [out] */ BSTR __RPC_FAR *mode,
            /* [out] */ BSTR __RPC_FAR *user,
            /* [out] */ BSTR __RPC_FAR *date,
            /* [out] */ VARIANT __RPC_FAR *description,
            /* [out] */ VARIANT __RPC_FAR *originalFitParameters,
            /* [out] */ VARIANT __RPC_FAR *newFitParameters,
            /* [out] */ VARIANT __RPC_FAR *minLimits,
            /* [out] */ VARIANT __RPC_FAR *maxLimits,
            /* [out] */ VARIANT __RPC_FAR *originalDisposition,
            /* [out] */ VARIANT __RPC_FAR *newDisposition,
            /* [out] */ VARIANT __RPC_FAR *calibrationData,
            /* [out] */ VARIANT __RPC_FAR *statusData,
            /* [out] */ VARIANT __RPC_FAR *rates,
            /* [out] */ VARIANT __RPC_FAR *plotData,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendUserRights )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ BSTR user,
            /* [in] */ BSTR password,
            /* [out] */ VARIANT __RPC_FAR *modeList,
            /* [out] */ VARIANT __RPC_FAR *chemistryList,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveClientData )( 
            IQalibServerMain __RPC_FAR * This,
            /* [in] */ BSTR chemistry,
            /* [in] */ INT outlierMethod,
            /* [in] */ BSTR mode,
            /* [in] */ BSTR user,
            /* [in] */ VARIANT __RPC_FAR *inputData,
            /* [in] */ VARIANT __RPC_FAR *serialNumber,
            /* [in] */ VARIANT __RPC_FAR *barCode,
            /* [in] */ VARIANT __RPC_FAR *sampleSpecies,
            /* [in] */ VARIANT __RPC_FAR *statusData,
            /* [out][in] */ LONG __RPC_FAR *calibrationID,
            /* [retval][out] */ VARIANT __RPC_FAR *error_msg);
        
        END_INTERFACE
    } IQalibServerMainVtbl;

    interface IQalibServerMain
    {
        CONST_VTBL struct IQalibServerMainVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQalibServerMain_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQalibServerMain_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQalibServerMain_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQalibServerMain_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IQalibServerMain_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IQalibServerMain_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IQalibServerMain_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IQalibServerMain_GetUpdatedFitParameters(This,calibrationID,newFitParameters,error_msg)	\
    (This)->lpVtbl -> GetUpdatedFitParameters(This,calibrationID,newFitParameters,error_msg)

#define IQalibServerMain_SetComment(This,calibrationID,comment,error_msg)	\
    (This)->lpVtbl -> SetComment(This,calibrationID,comment,error_msg)

#define IQalibServerMain_SendEditableFitParameters(This,calibrationID,editableFitParameters,error_msg)	\
    (This)->lpVtbl -> SendEditableFitParameters(This,calibrationID,editableFitParameters,error_msg)

#define IQalibServerMain_SendAssignedValues(This,chemistry,sampleIDs,curAssignedValues,error_msg)	\
    (This)->lpVtbl -> SendAssignedValues(This,chemistry,sampleIDs,curAssignedValues,error_msg)

#define IQalibServerMain_SendCalibrationResults(This,calibrationID,chemistry,waveLength,species,diluent,mold,rotor,cuvette,mode,user,date,description,originalFitParameters,newFitParameters,minLimits,maxLimits,originalDisposition,newDisposition,calibrationData,statusData,rates,plotData,comment,error_msg)	\
    (This)->lpVtbl -> SendCalibrationResults(This,calibrationID,chemistry,waveLength,species,diluent,mold,rotor,cuvette,mode,user,date,description,originalFitParameters,newFitParameters,minLimits,maxLimits,originalDisposition,newDisposition,calibrationData,statusData,rates,plotData,comment,error_msg)

#define IQalibServerMain_SendUserRights(This,user,password,modeList,chemistryList,error_msg)	\
    (This)->lpVtbl -> SendUserRights(This,user,password,modeList,chemistryList,error_msg)

#define IQalibServerMain_ReceiveClientData(This,chemistry,outlierMethod,mode,user,inputData,serialNumber,barCode,sampleSpecies,statusData,calibrationID,error_msg)	\
    (This)->lpVtbl -> ReceiveClientData(This,chemistry,outlierMethod,mode,user,inputData,serialNumber,barCode,sampleSpecies,statusData,calibrationID,error_msg)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_GetUpdatedFitParameters_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ LONG calibrationID,
    /* [in] */ VARIANT __RPC_FAR *newFitParameters,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_GetUpdatedFitParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_SetComment_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ LONG calibrationID,
    /* [in] */ BSTR __RPC_FAR *comment,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_SetComment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_SendEditableFitParameters_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ LONG calibrationID,
    /* [out] */ VARIANT __RPC_FAR *editableFitParameters,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_SendEditableFitParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_SendAssignedValues_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ BSTR chemistry,
    /* [in] */ const VARIANT __RPC_FAR *sampleIDs,
    /* [out] */ VARIANT __RPC_FAR *curAssignedValues,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_SendAssignedValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_SendCalibrationResults_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ LONG calibrationID,
    /* [out] */ BSTR __RPC_FAR *chemistry,
    /* [out] */ BSTR __RPC_FAR *waveLength,
    /* [out] */ BSTR __RPC_FAR *species,
    /* [out] */ BSTR __RPC_FAR *diluent,
    /* [out] */ BSTR __RPC_FAR *mold,
    /* [out] */ BSTR __RPC_FAR *rotor,
    /* [out] */ BSTR __RPC_FAR *cuvette,
    /* [out] */ BSTR __RPC_FAR *mode,
    /* [out] */ BSTR __RPC_FAR *user,
    /* [out] */ BSTR __RPC_FAR *date,
    /* [out] */ VARIANT __RPC_FAR *description,
    /* [out] */ VARIANT __RPC_FAR *originalFitParameters,
    /* [out] */ VARIANT __RPC_FAR *newFitParameters,
    /* [out] */ VARIANT __RPC_FAR *minLimits,
    /* [out] */ VARIANT __RPC_FAR *maxLimits,
    /* [out] */ VARIANT __RPC_FAR *originalDisposition,
    /* [out] */ VARIANT __RPC_FAR *newDisposition,
    /* [out] */ VARIANT __RPC_FAR *calibrationData,
    /* [out] */ VARIANT __RPC_FAR *statusData,
    /* [out] */ VARIANT __RPC_FAR *rates,
    /* [out] */ VARIANT __RPC_FAR *plotData,
    /* [out] */ BSTR __RPC_FAR *comment,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_SendCalibrationResults_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_SendUserRights_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ BSTR user,
    /* [in] */ BSTR password,
    /* [out] */ VARIANT __RPC_FAR *modeList,
    /* [out] */ VARIANT __RPC_FAR *chemistryList,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_SendUserRights_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQalibServerMain_ReceiveClientData_Proxy( 
    IQalibServerMain __RPC_FAR * This,
    /* [in] */ BSTR chemistry,
    /* [in] */ INT outlierMethod,
    /* [in] */ BSTR mode,
    /* [in] */ BSTR user,
    /* [in] */ VARIANT __RPC_FAR *inputData,
    /* [in] */ VARIANT __RPC_FAR *serialNumber,
    /* [in] */ VARIANT __RPC_FAR *barCode,
    /* [in] */ VARIANT __RPC_FAR *sampleSpecies,
    /* [in] */ VARIANT __RPC_FAR *statusData,
    /* [out][in] */ LONG __RPC_FAR *calibrationID,
    /* [retval][out] */ VARIANT __RPC_FAR *error_msg);


void __RPC_STUB IQalibServerMain_ReceiveClientData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQalibServerMain_INTERFACE_DEFINED__ */



#ifndef __QALIBSERVERLib_LIBRARY_DEFINED__
#define __QALIBSERVERLib_LIBRARY_DEFINED__

/* library QALIBSERVERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_QALIBSERVERLib;

EXTERN_C const CLSID CLSID_QalibServerMain;

#ifdef __cplusplus

class DECLSPEC_UUID("68EF2CCA-E9AE-4FBA-972C-6C6C6C6FA44A")
QalibServerMain;
#endif
#endif /* __QALIBSERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
