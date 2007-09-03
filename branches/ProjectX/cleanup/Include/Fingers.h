/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Dec 08 11:20:34 1997
 */
/* Compiler settings for fingers.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __fingers_h__
#define __fingers_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IFingerPrint_FWD_DEFINED__
#define __IFingerPrint_FWD_DEFINED__
typedef interface IFingerPrint IFingerPrint;
#endif 	/* __IFingerPrint_FWD_DEFINED__ */


#ifndef __FingerPrint_FWD_DEFINED__
#define __FingerPrint_FWD_DEFINED__

#ifdef __cplusplus
typedef class FingerPrint FingerPrint;
#else
typedef struct FingerPrint FingerPrint;
#endif /* __cplusplus */

#endif 	/* __FingerPrint_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFingerPrint_INTERFACE_DEFINED__
#define __IFingerPrint_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFingerPrint
 * at Mon Dec 08 11:20:34 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][uuid][object] */ 



EXTERN_C const IID IID_IFingerPrint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("FB89952F-6D89-11D1-8438-006097313542")
    IFingerPrint : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Verify( 
            const unsigned char __RPC_FAR *filename) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE VerifyAndDecrypt( 
            const unsigned char __RPC_FAR *filename,
            unsigned char __RPC_FAR *data,
            unsigned long size) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFingerPrintVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFingerPrint __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFingerPrint __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFingerPrint __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Verify )( 
            IFingerPrint __RPC_FAR * This,
            const unsigned char __RPC_FAR *filename);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyAndDecrypt )( 
            IFingerPrint __RPC_FAR * This,
            const unsigned char __RPC_FAR *filename,
            unsigned char __RPC_FAR *data,
            unsigned long size);
        
        END_INTERFACE
    } IFingerPrintVtbl;

    interface IFingerPrint
    {
        CONST_VTBL struct IFingerPrintVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFingerPrint_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFingerPrint_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFingerPrint_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFingerPrint_Verify(This,filename)	\
    (This)->lpVtbl -> Verify(This,filename)

#define IFingerPrint_VerifyAndDecrypt(This,filename,data,size)	\
    (This)->lpVtbl -> VerifyAndDecrypt(This,filename,data,size)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFingerPrint_Verify_Proxy( 
    IFingerPrint __RPC_FAR * This,
    const unsigned char __RPC_FAR *filename);


void __RPC_STUB IFingerPrint_Verify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFingerPrint_VerifyAndDecrypt_Proxy( 
    IFingerPrint __RPC_FAR * This,
    const unsigned char __RPC_FAR *filename,
    unsigned char __RPC_FAR *data,
    unsigned long size);


void __RPC_STUB IFingerPrint_VerifyAndDecrypt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFingerPrint_INTERFACE_DEFINED__ */



#ifndef __FINGERSLib_LIBRARY_DEFINED__
#define __FINGERSLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: FINGERSLib
 * at Mon Dec 08 11:20:34 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_FINGERSLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_FingerPrint;

class DECLSPEC_UUID("FB899530-6D89-11D1-8438-006097313542")
FingerPrint;
#endif
#endif /* __FINGERSLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Dec 08 11:20:34 1997
 */
/* Compiler settings for fingers.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
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

const IID IID_IFingerPrint = {0xFB89952F,0x6D89,0x11D1,{0x84,0x38,0x00,0x60,0x97,0x31,0x35,0x42}};


const IID LIBID_FINGERSLib = {0xFB899522,0x6D89,0x11D1,{0x84,0x38,0x00,0x60,0x97,0x31,0x35,0x42}};


const CLSID CLSID_FingerPrint = {0xFB899530,0x6D89,0x11D1,{0x84,0x38,0x00,0x60,0x97,0x31,0x35,0x42}};


#ifdef __cplusplus
}
#endif

