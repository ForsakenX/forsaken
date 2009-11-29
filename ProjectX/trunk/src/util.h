#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void strtoupper(char *str);

int Msg( const char * fmt, ... );

void DebugPrintf( const char * format, ... );
void DebugState( const char * str );

void Change_Ext( const char * Src, char * Dest, const char * Ext );
void Add_Path( uint8 * Path, uint8 * Src, uint8 * Dest );
void Get_Ext( uint8 * Src, uint8 * Dest );
void GetFilename( uint8 * Src, uint8 * Dest );

#ifdef __cplusplus
};
#endif

#endif
