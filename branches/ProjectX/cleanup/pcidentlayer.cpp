#include "afxwin.h"
#include "afxext.h"
#include "afxtempl.h"
#include "objbase.h"
#include "pcidentlayer.h"

#include "BA_FingerPrint.h"

extern "C" int IsAuthorised ( void )
{
	CString 		filename = "forsaken.aut";
	BOOL			authorised;
	BA_FingerPrint 	fingerprint;

	authorised = fingerprint.Verify(filename);
	return(authorised == TRUE);
}
