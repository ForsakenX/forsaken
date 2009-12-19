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
void Get_Ext( uint8 * Src, uint8 * Dest );
void GetFilename( uint8 * Src, uint8 * Dest );

char* convert_path( char* _str );
char * convert_char( char from, char to, char* in );

#ifdef __cplusplus
};
#endif

#endif
