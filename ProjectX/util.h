#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

BOOL DebugLog;
void DebugPrintf( const char * format, ... );
void DebugState( const char * str );

BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
HRESULT GUIDFromString( char *lpStr, GUID * pGuid);

#endif