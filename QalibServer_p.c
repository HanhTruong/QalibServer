/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Oct 20 15:13:38 2005
 */
/* Compiler settings for C:\Qalib\QalibServer\QalibServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "QalibServer.h"

#define TYPE_FORMAT_STRING_SIZE   1037                              
#define PROC_FORMAT_STRING_SIZE   473                               

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IQalibServerMain, ver. 0.0,
   GUID={0x88FAF4C5,0xAE8B,0x4E05,{0xB5,0x23,0x60,0x08,0x7F,0x1B,0xCD,0xC9}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IQalibServerMain_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static const unsigned short IQalibServerMain_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    40,
    80,
    120,
    166,
    332,
    384
    };

static const MIDL_SERVER_INFO IQalibServerMain_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IQalibServerMain_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IQalibServerMain_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IQalibServerMain_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(14) _IQalibServerMainProxyVtbl = 
{
    &IQalibServerMain_ProxyInfo,
    &IID_IQalibServerMain,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IQalibServerMain::GetUpdatedFitParameters */ ,
    (void *)-1 /* IQalibServerMain::SetComment */ ,
    (void *)-1 /* IQalibServerMain::SendEditableFitParameters */ ,
    (void *)-1 /* IQalibServerMain::SendAssignedValues */ ,
    (void *)-1 /* IQalibServerMain::SendCalibrationResults */ ,
    (void *)-1 /* IQalibServerMain::SendUserRights */ ,
    (void *)-1 /* IQalibServerMain::ReceiveClientData */
};


static const PRPC_STUB_FUNCTION IQalibServerMain_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IQalibServerMainStubVtbl =
{
    &IID_IQalibServerMain,
    &IQalibServerMain_ServerInfo,
    14,
    &IQalibServerMain_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            },
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure GetUpdatedFitParameters */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 10 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter calibrationID */

/* 16 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter newFitParameters */

/* 22 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter error_msg */

/* 28 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 30 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 32 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 34 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 36 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetComment */

/* 40 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 48 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	NdrFcShort( 0x8 ),	/* 8 */
/* 54 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter calibrationID */

/* 56 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 60 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter comment */

/* 62 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 64 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 66 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter error_msg */

/* 68 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 70 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 72 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 74 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 76 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 78 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendEditableFitParameters */

/* 80 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 82 */	NdrFcLong( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 88 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	NdrFcShort( 0x8 ),	/* 8 */
/* 94 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x4,		/* 4 */

	/* Parameter calibrationID */

/* 96 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 98 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter editableFitParameters */

/* 102 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 104 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 106 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter error_msg */

/* 108 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 110 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 112 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 116 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendAssignedValues */

/* 120 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 128 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 134 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter chemistry */

/* 136 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 138 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 140 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter sampleIDs */

/* 142 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 144 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 146 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter curAssignedValues */

/* 148 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 150 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 152 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter error_msg */

/* 154 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 156 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 158 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 160 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 162 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendCalibrationResults */

/* 166 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 168 */	NdrFcLong( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 174 */	NdrFcShort( 0x68 ),	/* x86, MIPS, PPC Stack size/offset = 104 */
#else
			NdrFcShort( 0xd0 ),	/* Alpha Stack size/offset = 208 */
#endif
/* 176 */	NdrFcShort( 0x8 ),	/* 8 */
/* 178 */	NdrFcShort( 0x8 ),	/* 8 */
/* 180 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x19,		/* 25 */

	/* Parameter calibrationID */

/* 182 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 184 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter chemistry */

/* 188 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 190 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 192 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter waveLength */

/* 194 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 196 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 198 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter species */

/* 200 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 202 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 204 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter diluent */

/* 206 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 208 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 210 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter mold */

/* 212 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 214 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 216 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter rotor */

/* 218 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 220 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 222 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter cuvette */

/* 224 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 226 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 228 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter mode */

/* 230 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 232 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 234 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter user */

/* 236 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 238 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 240 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter date */

/* 242 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 244 */	NdrFcShort( 0x2c ),	/* x86, MIPS, PPC Stack size/offset = 44 */
#else
			NdrFcShort( 0x58 ),	/* Alpha Stack size/offset = 88 */
#endif
/* 246 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter description */

/* 248 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 250 */	NdrFcShort( 0x30 ),	/* x86, MIPS, PPC Stack size/offset = 48 */
#else
			NdrFcShort( 0x60 ),	/* Alpha Stack size/offset = 96 */
#endif
/* 252 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter originalFitParameters */

/* 254 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 256 */	NdrFcShort( 0x34 ),	/* x86, MIPS, PPC Stack size/offset = 52 */
#else
			NdrFcShort( 0x68 ),	/* Alpha Stack size/offset = 104 */
#endif
/* 258 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter newFitParameters */

/* 260 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 262 */	NdrFcShort( 0x38 ),	/* x86, MIPS, PPC Stack size/offset = 56 */
#else
			NdrFcShort( 0x70 ),	/* Alpha Stack size/offset = 112 */
#endif
/* 264 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter minLimits */

/* 266 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 268 */	NdrFcShort( 0x3c ),	/* x86, MIPS, PPC Stack size/offset = 60 */
#else
			NdrFcShort( 0x78 ),	/* Alpha Stack size/offset = 120 */
#endif
/* 270 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter maxLimits */

/* 272 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 274 */	NdrFcShort( 0x40 ),	/* x86, MIPS, PPC Stack size/offset = 64 */
#else
			NdrFcShort( 0x80 ),	/* Alpha Stack size/offset = 128 */
#endif
/* 276 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter originalDisposition */

/* 278 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 280 */	NdrFcShort( 0x44 ),	/* x86, MIPS, PPC Stack size/offset = 68 */
#else
			NdrFcShort( 0x88 ),	/* Alpha Stack size/offset = 136 */
#endif
/* 282 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter newDisposition */

/* 284 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 286 */	NdrFcShort( 0x48 ),	/* x86, MIPS, PPC Stack size/offset = 72 */
#else
			NdrFcShort( 0x90 ),	/* Alpha Stack size/offset = 144 */
#endif
/* 288 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter calibrationData */

/* 290 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 292 */	NdrFcShort( 0x4c ),	/* x86, MIPS, PPC Stack size/offset = 76 */
#else
			NdrFcShort( 0x98 ),	/* Alpha Stack size/offset = 152 */
#endif
/* 294 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter statusData */

/* 296 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 298 */	NdrFcShort( 0x50 ),	/* x86, MIPS, PPC Stack size/offset = 80 */
#else
			NdrFcShort( 0xa0 ),	/* Alpha Stack size/offset = 160 */
#endif
/* 300 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter rates */

/* 302 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 304 */	NdrFcShort( 0x54 ),	/* x86, MIPS, PPC Stack size/offset = 84 */
#else
			NdrFcShort( 0xa8 ),	/* Alpha Stack size/offset = 168 */
#endif
/* 306 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter plotData */

/* 308 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 310 */	NdrFcShort( 0x58 ),	/* x86, MIPS, PPC Stack size/offset = 88 */
#else
			NdrFcShort( 0xb0 ),	/* Alpha Stack size/offset = 176 */
#endif
/* 312 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter comment */

/* 314 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 316 */	NdrFcShort( 0x5c ),	/* x86, MIPS, PPC Stack size/offset = 92 */
#else
			NdrFcShort( 0xb8 ),	/* Alpha Stack size/offset = 184 */
#endif
/* 318 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter error_msg */

/* 320 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
#ifndef _ALPHA_
/* 322 */	NdrFcShort( 0x60 ),	/* x86, MIPS, PPC Stack size/offset = 96 */
#else
			NdrFcShort( 0xc0 ),	/* Alpha Stack size/offset = 192 */
#endif
/* 324 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 328 */	NdrFcShort( 0x64 ),	/* x86, MIPS, PPC Stack size/offset = 100 */
#else
			NdrFcShort( 0xc8 ),	/* Alpha Stack size/offset = 200 */
#endif
/* 330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendUserRights */

/* 332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 340 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 346 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter user */

/* 348 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 350 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 352 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter password */

/* 354 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 356 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 358 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter modeList */

/* 360 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 362 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 364 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter chemistryList */

/* 366 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 368 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 370 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Parameter error_msg */

/* 372 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 374 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 376 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 380 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReceiveClientData */

/* 384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 392 */	NdrFcShort( 0x34 ),	/* x86, MIPS, PPC Stack size/offset = 52 */
#else
			NdrFcShort( 0x68 ),	/* Alpha Stack size/offset = 104 */
#endif
/* 394 */	NdrFcShort( 0x10 ),	/* 16 */
/* 396 */	NdrFcShort( 0x10 ),	/* 16 */
/* 398 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0xc,		/* 12 */

	/* Parameter chemistry */

/* 400 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 402 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 404 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter outlierMethod */

/* 406 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 408 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 410 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mode */

/* 412 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 414 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 416 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter user */

/* 418 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 420 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 422 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter inputData */

/* 424 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 426 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 428 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter serialNumber */

/* 430 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 432 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 434 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter barCode */

/* 436 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 438 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 440 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter sampleSpecies */

/* 442 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 444 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 446 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter statusData */

/* 448 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 450 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 452 */	NdrFcShort( 0x3c4 ),	/* Type Offset=964 */

	/* Parameter calibrationID */

/* 454 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 456 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 458 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter error_msg */

/* 460 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 462 */	NdrFcShort( 0x2c ),	/* x86, MIPS, PPC Stack size/offset = 44 */
#else
			NdrFcShort( 0x58 ),	/* Alpha Stack size/offset = 88 */
#endif
/* 464 */	NdrFcShort( 0x3d6 ),	/* Type Offset=982 */

	/* Return value */

/* 466 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 468 */	NdrFcShort( 0x30 ),	/* x86, MIPS, PPC Stack size/offset = 48 */
#else
			NdrFcShort( 0x60 ),	/* Alpha Stack size/offset = 96 */
#endif
/* 470 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x0,	/* FC_RP */
/*  4 */	NdrFcShort( 0x3c0 ),	/* Offset= 960 (964) */
/*  6 */	
			0x12, 0x0,	/* FC_UP */
/*  8 */	NdrFcShort( 0x3a8 ),	/* Offset= 936 (944) */
/* 10 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 12 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 14 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 16 */	NdrFcShort( 0x2 ),	/* Offset= 2 (18) */
/* 18 */	NdrFcShort( 0x10 ),	/* 16 */
/* 20 */	NdrFcShort( 0x2b ),	/* 43 */
/* 22 */	NdrFcLong( 0x3 ),	/* 3 */
/* 26 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 28 */	NdrFcLong( 0x11 ),	/* 17 */
/* 32 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 34 */	NdrFcLong( 0x2 ),	/* 2 */
/* 38 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 40 */	NdrFcLong( 0x4 ),	/* 4 */
/* 44 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 46 */	NdrFcLong( 0x5 ),	/* 5 */
/* 50 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 52 */	NdrFcLong( 0xb ),	/* 11 */
/* 56 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 58 */	NdrFcLong( 0xa ),	/* 10 */
/* 62 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 64 */	NdrFcLong( 0x6 ),	/* 6 */
/* 68 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (282) */
/* 70 */	NdrFcLong( 0x7 ),	/* 7 */
/* 74 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 76 */	NdrFcLong( 0x8 ),	/* 8 */
/* 80 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (288) */
/* 82 */	NdrFcLong( 0xd ),	/* 13 */
/* 86 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (312) */
/* 88 */	NdrFcLong( 0x9 ),	/* 9 */
/* 92 */	NdrFcShort( 0xee ),	/* Offset= 238 (330) */
/* 94 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 98 */	NdrFcShort( 0xfa ),	/* Offset= 250 (348) */
/* 100 */	NdrFcLong( 0x24 ),	/* 36 */
/* 104 */	NdrFcShort( 0x304 ),	/* Offset= 772 (876) */
/* 106 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 110 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (876) */
/* 112 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 116 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (880) */
/* 118 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 122 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (884) */
/* 124 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 128 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (888) */
/* 130 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 134 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (892) */
/* 136 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 140 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (896) */
/* 142 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 146 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (884) */
/* 148 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 152 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (888) */
/* 154 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 158 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (900) */
/* 160 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 164 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (896) */
/* 166 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 170 */	NdrFcShort( 0x2de ),	/* Offset= 734 (904) */
/* 172 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 176 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (908) */
/* 178 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 182 */	NdrFcShort( 0x2da ),	/* Offset= 730 (912) */
/* 184 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 188 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (916) */
/* 190 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 194 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (920) */
/* 196 */	NdrFcLong( 0x10 ),	/* 16 */
/* 200 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 202 */	NdrFcLong( 0x12 ),	/* 18 */
/* 206 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 208 */	NdrFcLong( 0x13 ),	/* 19 */
/* 212 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 214 */	NdrFcLong( 0x16 ),	/* 22 */
/* 218 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 220 */	NdrFcLong( 0x17 ),	/* 23 */
/* 224 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 226 */	NdrFcLong( 0xe ),	/* 14 */
/* 230 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (928) */
/* 232 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 236 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (940) */
/* 238 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 242 */	NdrFcShort( 0x27e ),	/* Offset= 638 (880) */
/* 244 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 248 */	NdrFcShort( 0x27c ),	/* Offset= 636 (884) */
/* 250 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 254 */	NdrFcShort( 0x27a ),	/* Offset= 634 (888) */
/* 256 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 260 */	NdrFcShort( 0x274 ),	/* Offset= 628 (888) */
/* 262 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 266 */	NdrFcShort( 0x26e ),	/* Offset= 622 (888) */
/* 268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* Offset= 0 (272) */
/* 274 */	NdrFcLong( 0x1 ),	/* 1 */
/* 278 */	NdrFcShort( 0x0 ),	/* Offset= 0 (278) */
/* 280 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (279) */
/* 282 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 286 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 288 */	
			0x12, 0x0,	/* FC_UP */
/* 290 */	NdrFcShort( 0xc ),	/* Offset= 12 (302) */
/* 292 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 294 */	NdrFcShort( 0x2 ),	/* 2 */
/* 296 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 298 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 300 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 302 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 306 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (292) */
/* 308 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 310 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 312 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 314 */	NdrFcLong( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 324 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 326 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 328 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 330 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 332 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 340 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 342 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 344 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 346 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 348 */	
			0x12, 0x0,	/* FC_UP */
/* 350 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (858) */
/* 352 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 354 */	NdrFcShort( 0x18 ),	/* 24 */
/* 356 */	NdrFcShort( 0xa ),	/* 10 */
/* 358 */	NdrFcLong( 0x8 ),	/* 8 */
/* 362 */	NdrFcShort( 0x58 ),	/* Offset= 88 (450) */
/* 364 */	NdrFcLong( 0xd ),	/* 13 */
/* 368 */	NdrFcShort( 0x78 ),	/* Offset= 120 (488) */
/* 370 */	NdrFcLong( 0x9 ),	/* 9 */
/* 374 */	NdrFcShort( 0x94 ),	/* Offset= 148 (522) */
/* 376 */	NdrFcLong( 0xc ),	/* 12 */
/* 380 */	NdrFcShort( 0xbc ),	/* Offset= 188 (568) */
/* 382 */	NdrFcLong( 0x24 ),	/* 36 */
/* 386 */	NdrFcShort( 0x114 ),	/* Offset= 276 (662) */
/* 388 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 392 */	NdrFcShort( 0x130 ),	/* Offset= 304 (696) */
/* 394 */	NdrFcLong( 0x10 ),	/* 16 */
/* 398 */	NdrFcShort( 0x148 ),	/* Offset= 328 (726) */
/* 400 */	NdrFcLong( 0x2 ),	/* 2 */
/* 404 */	NdrFcShort( 0x160 ),	/* Offset= 352 (756) */
/* 406 */	NdrFcLong( 0x3 ),	/* 3 */
/* 410 */	NdrFcShort( 0x178 ),	/* Offset= 376 (786) */
/* 412 */	NdrFcLong( 0x14 ),	/* 20 */
/* 416 */	NdrFcShort( 0x190 ),	/* Offset= 400 (816) */
/* 418 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (417) */
/* 420 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 422 */	NdrFcShort( 0x4 ),	/* 4 */
/* 424 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 428 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 430 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 432 */	NdrFcShort( 0x4 ),	/* 4 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x1 ),	/* 1 */
/* 438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 442 */	0x12, 0x0,	/* FC_UP */
/* 444 */	NdrFcShort( 0xffffff72 ),	/* Offset= -142 (302) */
/* 446 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 448 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 450 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 452 */	NdrFcShort( 0x8 ),	/* 8 */
/* 454 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 456 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 458 */	NdrFcShort( 0x4 ),	/* 4 */
/* 460 */	NdrFcShort( 0x4 ),	/* 4 */
/* 462 */	0x11, 0x0,	/* FC_RP */
/* 464 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (420) */
/* 466 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 468 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 470 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 482 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 484 */	NdrFcShort( 0xffffff54 ),	/* Offset= -172 (312) */
/* 486 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 488 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	NdrFcShort( 0x6 ),	/* Offset= 6 (500) */
/* 496 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 498 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 500 */	
			0x11, 0x0,	/* FC_RP */
/* 502 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (470) */
/* 504 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 508 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 516 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 518 */	NdrFcShort( 0xffffff44 ),	/* Offset= -188 (330) */
/* 520 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 522 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 524 */	NdrFcShort( 0x8 ),	/* 8 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x6 ),	/* Offset= 6 (534) */
/* 530 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 532 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 534 */	
			0x11, 0x0,	/* FC_RP */
/* 536 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (504) */
/* 538 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 540 */	NdrFcShort( 0x4 ),	/* 4 */
/* 542 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 546 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 548 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 550 */	NdrFcShort( 0x4 ),	/* 4 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x1 ),	/* 1 */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	0x12, 0x0,	/* FC_UP */
/* 562 */	NdrFcShort( 0x17e ),	/* Offset= 382 (944) */
/* 564 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 566 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 568 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x6 ),	/* Offset= 6 (580) */
/* 576 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 578 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 580 */	
			0x11, 0x0,	/* FC_RP */
/* 582 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (538) */
/* 584 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 586 */	NdrFcLong( 0x2f ),	/* 47 */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 596 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 598 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 600 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 602 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 604 */	NdrFcShort( 0x1 ),	/* 1 */
/* 606 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 608 */	NdrFcShort( 0x4 ),	/* 4 */
/* 610 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 612 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 614 */	NdrFcShort( 0x10 ),	/* 16 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0xa ),	/* Offset= 10 (628) */
/* 620 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 622 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 624 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (584) */
/* 626 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 628 */	
			0x12, 0x0,	/* FC_UP */
/* 630 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (602) */
/* 632 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 634 */	NdrFcShort( 0x4 ),	/* 4 */
/* 636 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 640 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 642 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 644 */	NdrFcShort( 0x4 ),	/* 4 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x1 ),	/* 1 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	0x12, 0x0,	/* FC_UP */
/* 656 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (612) */
/* 658 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 660 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 662 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 664 */	NdrFcShort( 0x8 ),	/* 8 */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x6 ),	/* Offset= 6 (674) */
/* 670 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 672 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 674 */	
			0x11, 0x0,	/* FC_RP */
/* 676 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (632) */
/* 678 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 680 */	NdrFcShort( 0x8 ),	/* 8 */
/* 682 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 684 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 686 */	NdrFcShort( 0x10 ),	/* 16 */
/* 688 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 690 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 692 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (678) */
			0x5b,		/* FC_END */
/* 696 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 698 */	NdrFcShort( 0x18 ),	/* 24 */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0xa ),	/* Offset= 10 (712) */
/* 704 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 706 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 708 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (684) */
/* 710 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 712 */	
			0x11, 0x0,	/* FC_RP */
/* 714 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (470) */
/* 716 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 718 */	NdrFcShort( 0x1 ),	/* 1 */
/* 720 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 726 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 728 */	NdrFcShort( 0x8 ),	/* 8 */
/* 730 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 732 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 734 */	NdrFcShort( 0x4 ),	/* 4 */
/* 736 */	NdrFcShort( 0x4 ),	/* 4 */
/* 738 */	0x12, 0x0,	/* FC_UP */
/* 740 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (716) */
/* 742 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 744 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 746 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 748 */	NdrFcShort( 0x2 ),	/* 2 */
/* 750 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 754 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 756 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 758 */	NdrFcShort( 0x8 ),	/* 8 */
/* 760 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 762 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 764 */	NdrFcShort( 0x4 ),	/* 4 */
/* 766 */	NdrFcShort( 0x4 ),	/* 4 */
/* 768 */	0x12, 0x0,	/* FC_UP */
/* 770 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (746) */
/* 772 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 774 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 776 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 778 */	NdrFcShort( 0x4 ),	/* 4 */
/* 780 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 786 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 790 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 792 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 794 */	NdrFcShort( 0x4 ),	/* 4 */
/* 796 */	NdrFcShort( 0x4 ),	/* 4 */
/* 798 */	0x12, 0x0,	/* FC_UP */
/* 800 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (776) */
/* 802 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 804 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 806 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 808 */	NdrFcShort( 0x8 ),	/* 8 */
/* 810 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 816 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x8 ),	/* 8 */
/* 820 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 822 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 824 */	NdrFcShort( 0x4 ),	/* 4 */
/* 826 */	NdrFcShort( 0x4 ),	/* 4 */
/* 828 */	0x12, 0x0,	/* FC_UP */
/* 830 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (806) */
/* 832 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 834 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 836 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 838 */	NdrFcShort( 0x8 ),	/* 8 */
/* 840 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 842 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 844 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 850 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 852 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 854 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (836) */
/* 856 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 858 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 860 */	NdrFcShort( 0x28 ),	/* 40 */
/* 862 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (844) */
/* 864 */	NdrFcShort( 0x0 ),	/* Offset= 0 (864) */
/* 866 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 868 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 870 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 872 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (352) */
			0x5b,		/* FC_END */
/* 876 */	
			0x12, 0x0,	/* FC_UP */
/* 878 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (612) */
/* 880 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 882 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 884 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 886 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 888 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 890 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 892 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 894 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 896 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 898 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 900 */	
			0x12, 0x0,	/* FC_UP */
/* 902 */	NdrFcShort( 0xfffffd94 ),	/* Offset= -620 (282) */
/* 904 */	
			0x12, 0x10,	/* FC_UP */
/* 906 */	NdrFcShort( 0xfffffd96 ),	/* Offset= -618 (288) */
/* 908 */	
			0x12, 0x10,	/* FC_UP */
/* 910 */	NdrFcShort( 0xfffffdaa ),	/* Offset= -598 (312) */
/* 912 */	
			0x12, 0x10,	/* FC_UP */
/* 914 */	NdrFcShort( 0xfffffdb8 ),	/* Offset= -584 (330) */
/* 916 */	
			0x12, 0x10,	/* FC_UP */
/* 918 */	NdrFcShort( 0xfffffdc6 ),	/* Offset= -570 (348) */
/* 920 */	
			0x12, 0x10,	/* FC_UP */
/* 922 */	NdrFcShort( 0x2 ),	/* Offset= 2 (924) */
/* 924 */	
			0x12, 0x0,	/* FC_UP */
/* 926 */	NdrFcShort( 0xfffffc62 ),	/* Offset= -926 (0) */
/* 928 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 930 */	NdrFcShort( 0x10 ),	/* 16 */
/* 932 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 934 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 936 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 938 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 940 */	
			0x12, 0x0,	/* FC_UP */
/* 942 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (928) */
/* 944 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 946 */	NdrFcShort( 0x20 ),	/* 32 */
/* 948 */	NdrFcShort( 0x0 ),	/* 0 */
/* 950 */	NdrFcShort( 0x0 ),	/* Offset= 0 (950) */
/* 952 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 954 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 956 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 958 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 960 */	NdrFcShort( 0xfffffc4a ),	/* Offset= -950 (10) */
/* 962 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 964 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0x10 ),	/* 16 */
/* 970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 972 */	NdrFcShort( 0xfffffc3a ),	/* Offset= -966 (6) */
/* 974 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 976 */	NdrFcShort( 0x6 ),	/* Offset= 6 (982) */
/* 978 */	
			0x13, 0x0,	/* FC_OP */
/* 980 */	NdrFcShort( 0xffffffdc ),	/* Offset= -36 (944) */
/* 982 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 984 */	NdrFcShort( 0x0 ),	/* 0 */
/* 986 */	NdrFcShort( 0x10 ),	/* 16 */
/* 988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 990 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (978) */
/* 992 */	
			0x11, 0x0,	/* FC_RP */
/* 994 */	NdrFcShort( 0x2 ),	/* Offset= 2 (996) */
/* 996 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 998 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1000 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0xfffffd34 ),	/* Offset= -716 (288) */
/* 1006 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1008 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1014) */
/* 1010 */	
			0x13, 0x0,	/* FC_OP */
/* 1012 */	NdrFcShort( 0xfffffd3a ),	/* Offset= -710 (302) */
/* 1014 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1016 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1010) */
/* 1024 */	
			0x11, 0x0,	/* FC_RP */
/* 1026 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (982) */
/* 1028 */	
			0x11, 0x0,	/* FC_RP */
/* 1030 */	NdrFcShort( 0xfffffff0 ),	/* Offset= -16 (1014) */
/* 1032 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1034 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

const CInterfaceProxyVtbl * _QalibServer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IQalibServerMainProxyVtbl,
    0
};

const CInterfaceStubVtbl * _QalibServer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IQalibServerMainStubVtbl,
    0
};

PCInterfaceName const _QalibServer_InterfaceNamesList[] = 
{
    "IQalibServerMain",
    0
};

const IID *  _QalibServer_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _QalibServer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _QalibServer, pIID, n)

int __stdcall _QalibServer_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_QalibServer_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo QalibServer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _QalibServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _QalibServer_StubVtblList,
    (const PCInterfaceName * ) & _QalibServer_InterfaceNamesList,
    (const IID ** ) & _QalibServer_BaseIIDList,
    & _QalibServer_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
