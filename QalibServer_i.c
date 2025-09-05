/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Oct 20 15:13:38 2005
 */
/* Compiler settings for C:\Qalib\QalibServer\QalibServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IQalibServerMain = {0x88FAF4C5,0xAE8B,0x4E05,{0xB5,0x23,0x60,0x08,0x7F,0x1B,0xCD,0xC9}};


const IID LIBID_QALIBSERVERLib = {0x06DE59A6,0xD867,0x48A5,{0x8A,0x6A,0xEA,0x9E,0xE9,0x9E,0x4A,0x19}};


const CLSID CLSID_QalibServerMain = {0x68EF2CCA,0xE9AE,0x4FBA,{0x97,0x2C,0x6C,0x6C,0x6C,0x6F,0xA4,0x4A}};


#ifdef __cplusplus
}
#endif

