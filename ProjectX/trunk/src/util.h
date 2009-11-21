#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

int Msg( const char * fmt, ... );

void DebugPrintf( const char * format, ... );
void DebugState( const char * str );

#ifdef __cplusplus
};
#endif

#endif
