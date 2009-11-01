#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

int Msg( LPSTR fmt, ... );

void DebugPrintf( const char * format, ... );
void DebugState( const char * str );

BOOL StringFromGUID(LPCGUID lpguid, LPSTR lpsz);
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
HRESULT GUIDFromString( char *lpStr, GUID * pGuid);

#ifdef __cplusplus
};
#endif

#endif