#ifndef FILE_H
#define FILE_H

#include "Typedefs.h"

int folder_exists( char *pathspec, ... );
long Get_File_Size( char * Filename );
long Read_File( char * Filename, char * File_Buffer, long Read_Size );
long Write_File( char * Filename, char * File_Buffer, long Write_Size );
void Add_Path( uint8 * Path, uint8 * Src, uint8 * Dest );
void Get_Ext( uint8 * Src, uint8 * Dest );
void GetFilename( uint8 * Src, uint8 * Dest );
void Change_Ext( uint8 * Src, uint8 * Dest, uint8 * Ext );
long Get_File_Size( char * Filename );
long Read_File( char * Filename, char * File_Buffer, long Read_Size );
void AddCommentToLog( const char * format, ... );
BOOL File_Exists( char * Filename );
void WriteToFile( char* path, const char * format, ... );

#endif // FILE_H
