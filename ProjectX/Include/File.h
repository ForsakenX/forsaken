#ifndef FILE_H
#define FILE_H
/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 8 $
 *
 * $Header: /PcProjectX/Include/file.h 8     25/08/98 2:53p Oliverc $
 *
 * $Log: /PcProjectX/Include/file.h $
 * 
 * 8     25/08/98 2:53p Oliverc
 * Added InitCheckSum() routine and modified checksum calculation to be
 * more position/order-sensistive than before
 * 
 * 7     25/08/98 8:51 Collinsd
 * Added checksum call to all loaded files.
 * 
 * 6     28/01/98 16:10 Oliverc
 * 
 * 5     5/01/98 10:38 Philipy
 * speech sfx implemented - currently defaults to 1 biker & computer only,
 * none selectable
 * 
 * 4     15/10/97 9:37 Collinsd
 * Added logfile/batchfile creation code.
 * 
 * 3     11/08/97 10:12 Collinsd
 * Added override data directory option. ( SFX don't work yet! )
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Define Prototypes for file.c
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
long Get_File_Size( char * Filename );
long Read_File( char * Filename, char * File_Buffer, long Read_Size );
long Write_File( char * Filename, char * File_Buffer, long Write_Size );
void Add_Path( uint8 * Path, uint8 * Src, uint8 * Dest );
void Get_Ext( uint8 * Src, uint8 * Dest );
void GetFilename( uint8 * Src, uint8 * Dest );
void Change_Ext( uint8 * Src, uint8 * Dest, uint8 * Ext );
long DataPath_Get_File_Size( char * Filename );
long DataPath_Read_File( char * Filename, char * File_Buffer, long Read_Size );
FILE * DataPath_fopen( char * Filename, char * Rights );
void AddCommentToLog( const char * format, ... );
void AddCommentToBat( const char * format, ... );
void AddDataPathFileToBat( char * Filename );
void AddFileToBat( char * Filename );
void AddCommandToBat( const char * format, ... );
BOOL File_Exists( char * Filename );
void FileCheckSum( char *fname );
void DataPath_FileCheckSum( char *fname );
void InitCheckSum( LPGUID lpguid );

#endif // FILE_H
