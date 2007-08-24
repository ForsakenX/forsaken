#include <objbase.h>
#include "fingers.h"

extern "C" BOOL ValidFingerPrint( char *filename )
{
	IFingerPrint* fp = NULL;
	BOOL	success = FALSE;
	HRESULT	hr;
	
	::CoInitialize( NULL );	//may not be needed if you have already done it in your code.

	hr = ::CoCreateInstance(CLSID_FingerPrint, NULL, CLSCTX_INPROC_SERVER, IID_IFingerPrint, (void**) &fp );

	if( FAILED(hr) )
	{
		//fail fingerprint verification
	}
	else
	{
		success = SUCCEEDED(fp->Verify( (const unsigned char *)filename));
		fp->Release();
		//fingerprint verified - do game code
	}

	::CoUninitialize();	//only needed if you called CoInitialize()

	return( success );
}