
#include "windows.h"
#include <stdio.h>
#include "typedefs.h"
#include "file.h"
#include "util.h"

extern BOOL Debug;
BOOL DebugLog = FALSE;
void DebugPrintf( const char * format, ... )
{

  static char buf1[256], buf2[512];
  va_list args;

  // command line switch
  if ( ! Debug )
    return;

  va_start( args, format );
  vsprintf( buf1, format, args );
  wsprintf( buf2, "%hs", buf1 );

  // send string to debugger
  OutputDebugString( buf2 );

  va_end( args );

  // add the comment to the log file
  if( DebugLog )
	AddCommentToLog( buf2 );

}

// prints a message only if it wasn't the last one to be printed...
// this way you can debug game state without getting a message every single loop
// for instance in the title screen if you print "in title screen" it will only print once
// instead of every single loop....
void DebugState( const char * str )
{
	static const char * last;
	if ( !Debug ) 
		return;
	if ( last && strcmp( str, last ) == 0 )
		return;
	last = str;
	DebugPrintf( str );
}


/*
 * IsEqualGuid
 *
 * Determines if two guids are equal
 */

BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2)
{
   return (
      ((PLONG) lpguid1)[0] == ((PLONG) lpguid2)[0] &&
      ((PLONG) lpguid1)[1] == ((PLONG) lpguid2)[1] &&
      ((PLONG) lpguid1)[2] == ((PLONG) lpguid2)[2] &&
      ((PLONG) lpguid1)[3] == ((PLONG) lpguid2)[3]);
}

BOOL StringFromGUID(LPCGUID lpguid, LPSTR lpsz) 
{ 
    int i;
    const BYTE * pBytes = (const BYTE *) lpguid; 
	static const char szDigits[] = "0123456789ABCDEF";
	static const BYTE GuidMap[] = { 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 
									8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
 
    *lpsz++ = '{'; 
 
    for (i = 0; i < sizeof(GuidMap); i++) 
    { 
        if (GuidMap[i] == '-') 
        { 
            *lpsz++ = '-'; 
        } 
        else 
        { 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0xF0) >> 4 ]; 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0x0F) ]; 
        } 
    } 
    *lpsz++ = '}'; 
    *lpsz   = '\0'; 
 
    return TRUE; 
}

//////////////////////////////////////////////////////////////////////
// GetDigit(), ConvertField(), and GUIDFromString()
//////////////////////////////////////////////////////////////////////


// convert a hex char to an int - used by str to guid conversion
// we wrote our own, since the ole one is slow, and requires ole32.dll
// we use ansi strings here, since guids won't get internationalized
static int GetDigit(LPSTR lpstr)
{
	char ch = *lpstr;
    
    if (ch >= '0' && ch <= '9')
        return(ch - '0');
    if (ch >= 'a' && ch <= 'f')
        return(ch - 'a' + 10);
    if (ch >= 'A' && ch <= 'F')
        return(ch - 'A' + 10);
    return(0);
}

// walk the string, writing pairs of bytes into the byte stream (guid)
// we need to write the bytes into the byte stream from right to left
// or left to right as indicated by fRightToLeft
static void ConvertField(LPBYTE lpByte,LPSTR * ppStr,int iFieldSize,BOOL fRightToLeft)
{
	int i;

	for (i=0;i<iFieldSize ;i++ )
	{
		// don't barf on the field separators
		if ('-' == **ppStr) (*ppStr)++; 
		if (fRightToLeft == TRUE)
		{
			// work from right to left within the byte stream
			*(lpByte + iFieldSize - (i+1)) = 16*GetDigit(*ppStr) + GetDigit((*ppStr)+1);
		} 
		else 
		{
			// work from  left to right within the byte stream
			*(lpByte + i) = 16*GetDigit(*ppStr) + GetDigit((*ppStr)+1);
		}
		*ppStr+=2; // get next two digit pair
	}
} // ConvertField


// convert the passed in string to a real GUID
// walk the guid, setting each byte in the guid to the two digit hex pair in the
// passed string
HRESULT GUIDFromString( char *lpStr, GUID * pGuid)
{
	BYTE * lpByte; // byte index into guid
	int iFieldSize; // size of current field we're converting
	// since its a guid, we can do a "brute force" conversion
	
	// make sure we have a {xxxx-...} type guid
	if ('{' !=  *lpStr) return E_FAIL;
	lpStr++;
	
	lpByte = (BYTE *)pGuid;
	// data 1
	iFieldSize = sizeof(unsigned long);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 2
	iFieldSize = sizeof(unsigned short);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 3
	iFieldSize = sizeof(unsigned short);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 4
	iFieldSize = 8*sizeof(unsigned char);
	ConvertField(lpByte,&lpStr,iFieldSize,FALSE);
	lpByte += iFieldSize;

	// make sure we ended in the right place
	if ('}' != *lpStr) 
	{
		memset(pGuid,0,sizeof(GUID));
		return E_FAIL;
	}

	return S_OK;
}// GUIDFromString

//
// Creates a message box and returns ok/cancel press
//

// temporarily jumps to desktop mode
#include "render.h"
#include "title.h"
extern render_info_t render_info;
extern void MenuGoFullScreen( MENUITEM *Item );
extern void SetCursorClip( BOOL clip );
int Msg( const char * msg, ... )
{
	BOOL was_fullscreen = render_info.bFullscreen;

	char txt[ 1024 ];
	va_list args;
	int res;

	va_start( args, msg );
	vsprintf( txt, msg, args);
	va_end( args );

    if (render_info.bFullscreen)
	{
		// switch to window mode
		// other wise pop up will get stuck behind main window
		MenuGoFullScreen(NULL);
		// push main window to background so popup shows
        SetWindowPos(render_info.window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	// release mouse so they can interact with message box
	SetCursorClip( FALSE );

    res = MessageBox( render_info.window, txt, "Forsaken", MB_OKCANCEL | MB_ICONEXCLAMATION );

    if (was_fullscreen)
	{
		// switch back to fullscreen
		MenuGoFullScreen(NULL);
        SetWindowPos(render_info.window, HWND_TOPMOST, 0, 0, 0, 0,  SWP_NOSIZE | SWP_NOMOVE);
		SetCursorClip( TRUE ); // don't do this in window mode just let them click back on the window
	}

	DebugPrintf( txt );

	// IDCANCEL	Cancel button was selected.
	// IDOK	OK button was selected.
	return res;
}
