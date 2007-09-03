/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 23 $
 *
 * $Header: /PcProjectX/File.c 23    25/08/98 4:10p Oliverc $
 *
 * $Log: /PcProjectX/File.c $
 * 
 * 23    25/08/98 4:10p Oliverc
 * Modified checksum routine again to make even better (I hope)
 * 
 * 22    25/08/98 15:44 Phillipd
 * 
 * 21    25/08/98 3:01p Oliverc
 * 
 * 20    25/08/98 2:53p Oliverc
 * Added InitCheckSum() routine and modified checksum calculation to be
 * more position/order-sensistive than before
 * 
 * 19    25/08/98 9:45 Collinsd
 * Added debug info
 * 
 * 18    25/08/98 8:50 Collinsd
 * Added checksum call to all loaded files.
 * 
 * 17    24/08/98 5:53p Oliverc
 * Added FileCheckSum() routine for multiplayer levels
 * 
 * 16    11/04/98 16:44 Collinsd
 * 
 * 15    14/01/98 10:12 Collinsd
 * Add logfile and batchfile works again.
 * 
 * 14    9/01/98 16:02 Philipy
 * 
 * 13    9/01/98 15:57 Philipy
 * fixed FileExists bug
 * 
 * 12    9/01/98 11:48 Collinsd
 * 
 * 11    9/01/98 11:01 Collinsd
 * Stuff back to normal and goldbars not get carried over levels.
 * 
 * 10    5/01/98 10:37 Philipy
 * speech sfx implemented - currently defaults to 1 biker & computer only,
 * none selectable
 * 
 * 9     6/12/97 16:35 Collinsd
 * Debuginfo put on #define
 * 
 * 8     5/12/97 14:11 Collinsd
 * Allocated ships fixed.
 * 
 * 7     15/10/97 9:37 Collinsd
 * Added logfile/batchfile creation code.
 * 
 * 6     13/10/97 19:36 Collinsd
 * 
 * 5     13/08/97 15:09 Collinsd
 * Taken out debug messages.
 * 
 * 4     11/08/97 10:12 Collinsd
 * Added override data directory option. ( SFX don't work yet! )
 * 
 * 3     11/25/96 5:14p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Header files
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<io.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	<dplay.h>
#include	"typedefs.h"
#include	"d3dmain.h"
#include	"file.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
//#define	DEBUG_DATAPATH	1

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	External Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	int	 use_data_path;
extern	char data_path[ 128 ];
extern	char normdata_path[ 128 ];

void DebugPrintf( const char * format, ... );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Globals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
LONGLONG	LevelCheckSum = 0;
BOOL	CreateBatchFile = FALSE;
BOOL	CreateLogFile = FALSE;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Return Size of File given Filename
	Input		:		char	*	Filename
	Output		:		long		Size of File
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
long Get_File_Size( char * Filename )
{
	int		Handle = -1;
	long	Read_Size = 0;
#ifdef	WATCOM
	Handle = open( Filename, O_RDONLY | O_BINARY );
	if( Handle != -1 )
	{
		Read_Size = filelength( Handle );
		close( Handle );
	}

#else
	Handle = _open( Filename, _O_RDONLY | _O_BINARY );
	if( Handle != -1 )
	{
		Read_Size = _filelength( Handle );
		_close( Handle );
	}
#endif

	return ( Read_Size );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		See if file exists
	Input		:		char	*	Filename
	Output		:		BOOL		TRUE if exists
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL File_Exists( char * Filename )
{
	/*
	int		Handle = -1;
#ifdef	WATCOM
	Handle = open( Filename, O_RDONLY | O_BINARY );
	if( Handle != -1 )
	{
		close( Handle );
		return TRUE;
	}

#else
	Handle = _open( Filename, _O_RDONLY | _O_BINARY );
	if( Handle != -1 )
	{
		close( Handle );
		return TRUE;
	}
#endif
	*/
	if ( !_access( Filename, 00 ) )
		return TRUE;
	else
		return FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Read Part or All of File Into Memory
	Input		:		char	*	Filename
				:		char	*	Buffer to Load into
				:		long		Bytes to Read ( 0 = All )
	Output		:		long		Number of bytes Read
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
long Read_File( char * Filename, char * File_Buffer, long Read_Size )
{
	int		Handle = -1;
	long	Bytes_Read = 0;

#ifdef	WATCOM
	Handle = open( Filename, O_RDONLY | O_BINARY );
	if( Handle != -1 )
	{
		if( Read_Size == 0 ) Read_Size = filelength( Handle );
		Bytes_Read = read( Handle, File_Buffer, Read_Size );
		if( Bytes_Read == -1 ) Bytes_Read = 0;
		close( Handle );
	}
#else
	Handle = _open( Filename, _O_RDONLY | _O_BINARY );
	if( Handle != -1 )
	{
		if( Read_Size == 0 ) Read_Size = _filelength( Handle );
		Bytes_Read = _read( Handle, File_Buffer, Read_Size );
		if( Bytes_Read == -1 ) Bytes_Read = 0;
		_close( Handle );
	}
#endif

	return ( Bytes_Read );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Write File ( Creating file if non exists )
	Input		:		char	*	Filename
				:		char	*	Buffer to Write
				:		long		Bytes to Write ( 0 = All )
	Output		:		long		Number of bytes Written
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
long Write_File( char * Filename, char * File_Buffer, long Write_Size )
{
	int		Handle = -1;
	long	Bytes_Written = 0;

#ifdef	WATCOM
	Handle = open( Filename, O_CREAT | O_TRUNC | O_BINARY | O_RDWR);
	if( Handle != -1 ) {
		Bytes_Written = write( Handle, File_Buffer, Write_Size );
		close( Handle );
	}
#else
	Handle = _open( Filename, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR ,
							  _S_IREAD | _S_IWRITE );
	if( Handle != -1 ) {
		Bytes_Written = _write( Handle, File_Buffer, Write_Size );
		_close( Handle );
	}
#endif
	return ( Bytes_Written );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Change Extension of Filename
	Input		:		uint8	*	Src Filename
				:		uint8	*	Dest Filename
				:		uint8	*	Extension to add
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Change_Ext( uint8 * Src, uint8 * Dest, uint8 * Ext )
{
	uint8	*	Char_Ptr;

	Char_Ptr = ( Src + strlen( Src ) ) -1;

	while( Char_Ptr != Src && *Char_Ptr != '\\' && *Char_Ptr != ':' && *Char_Ptr != '.' ) Char_Ptr--;

	if( *Char_Ptr == '.' )
	{
		while( Src != Char_Ptr ) *Dest++ = *Src++;
		strcpy( Dest, Ext );
	}
	else
	{
		strcpy( Dest, Src );
		Dest += strlen( Src );
		strcpy( Dest, Ext );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Get Filename from path\filename
	Input		:		uint8	*	Src path\Filename
				:		uint8	*	Dest Filename
				:		uint8	*	Extension to add
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetFilename( uint8 * Src, uint8 * Dest )
{
	uint8	*	Char_Ptr;

	Char_Ptr = ( Src + strlen( Src ) ) -1;

	while( Char_Ptr != Src && *Char_Ptr != '\\' && *Char_Ptr != ':' ) Char_Ptr--;

	if( Char_Ptr == Src )
	{
		strcpy( Dest, Src );
		return;
	}

	if( Char_Ptr != ( Src + strlen( Src ) - 1 ) )
	{
		strcpy( Dest, ( Char_Ptr + 1 ) );
	}
	else
	{
		*Dest = 0;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Change Extension of Filename
	Input		:		uint8	*	Src Filename
				:		uint8	*	Dest to put ext
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Get_Ext( uint8 * Src, uint8 * Dest )
{
	uint8	*	Char_Ptr;

	Char_Ptr = ( Src + strlen( Src ) ) -1;

	while( Char_Ptr != Src && *Char_Ptr != '\\' && *Char_Ptr != ':' && *Char_Ptr != '.' ) Char_Ptr--;

	if( *Char_Ptr == '.' )
	{
		Char_Ptr++;
		while( *Char_Ptr ) *Dest++ = *Char_Ptr++;
		*Dest = 0;
	}
	else
	{
		*Dest = 0;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add path to filename
	Input		:		uint8	*	Path\
				:		uint8	*	Src Filename
				:		uint8	*	Dest Path\Filename
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Add_Path( uint8 * Path, uint8 * Src, uint8 * Dest )
{
	strcpy( Dest, Path );
	Dest = ( Dest + strlen( Path ) );
	strcpy( Dest, Src );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Return Size of File given Filename
				:		data\ as base directory
				:		scan override dir first, then normal
	Input		:		char	*	Filename
	Output		:		long		Size of File
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
long DataPath_Get_File_Size( char * Filename )
{
	int		Handle = -1;
	long	Read_Size = 0;
	char	TempFilename[ 256 ];
#ifdef	WATCOM

	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], Filename, &TempFilename[0] );
		Handle = open( &TempFilename[ 0 ], O_RDONLY | O_BINARY );

		if( Handle != -1 )
		{
#ifdef DEBUG_DATAPATH
			DebugPrintf( "DataPath_Get_File_Size() : %s\n", &TempFilename[ 0 ] );
#endif

			Read_Size = filelength( Handle );
			close( Handle );
			return( Read_Side );
		}
	}

	Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[0] );
	Handle = open( &TempFilename[ 0 ], O_RDONLY | O_BINARY );
	if( Handle != -1 )
	{
#ifdef DEBUG_DATAPATH
		DebugPrintf( "Get_File_Size() : %s\n", &TempFilename[ 0 ] );
#endif
		Read_Size = filelength( Handle );
		close( Handle );
	}
	
#else

	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], Filename, &TempFilename[0] );
		Handle = _open( &TempFilename[ 0 ], _O_RDONLY | _O_BINARY );

		if( Handle != -1 )
		{
#ifdef DEBUG_DATAPATH
			DebugPrintf( "DataPath_Get_File_Size() : %s\n", &TempFilename[ 0 ] );
#endif
			Read_Size = _filelength( Handle );
			_close( Handle );
			return( Read_Size );
		}
	}

	Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[0] );
	Handle = _open( &TempFilename[ 0 ], _O_RDONLY | _O_BINARY );
	if( Handle != -1 )
	{
#ifdef DEBUG_DATAPATH
		DebugPrintf( "Get_File_Size() : %s\n", &TempFilename[ 0 ] );
#endif
		Read_Size = _filelength( Handle );
		_close( Handle );
	}

#endif

	return ( Read_Size );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Read Part or All of File Into Memory
				:		data\ as base directory
				:		read from override dir first, then normal
	Input		:		char	*	Filename
				:		char	*	Buffer to Load into
				:		long		Bytes to Read ( 0 = All )
	Output		:		long		Number of bytes Read
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
long DataPath_Read_File( char * Filename, char * File_Buffer, long Read_Size )
{
	int		Handle = -1;
	long	Bytes_Read = 0;
	char	TempFilename[ 256 ];

#ifdef	WATCOM
	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], Filename, &TempFilename[0] );
		Handle = open( &TempFilename[ 0 ], O_RDONLY | O_BINARY );
	
		if( Handle != -1 )
		{
#ifdef DEBUG_DATAPATH
			DebugPrintf( "DataPath_Read_File() : %s\n", &TempFilename[ 0 ] );
#endif
			if( Read_Size == 0 ) Read_Size = filelength( Handle );
			Bytes_Read = read( Handle, File_Buffer, Read_Size );
			if( Bytes_Read == -1 ) Bytes_Read = 0;
			close( Handle );
			return ( Bytes_Read );
		}
	}

	Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[0] );
	Handle = open( &TempFilename[ 0 ], O_RDONLY | O_BINARY );

	if( Handle != -1 )
	{
#ifdef DEBUG_DATAPATH
		DebugPrintf( "Read_File() : %s\n", &TempFilename[ 0 ] );
#endif
		if( Read_Size == 0 ) Read_Size = filelength( Handle );
		Bytes_Read = read( Handle, File_Buffer, Read_Size );
		if( Bytes_Read == -1 ) Bytes_Read = 0;
		close( Handle );
	}

#else
	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], Filename, &TempFilename[0] );
		Handle = _open( &TempFilename[ 0 ], _O_RDONLY | _O_BINARY );

		if( Handle != -1 )
		{
#ifdef DEBUG_DATAPATH
			DebugPrintf( "DataPath_Read_File() : %s\n", &TempFilename[ 0 ] );
#endif
			if( Read_Size == 0 ) Read_Size = _filelength( Handle );
			Bytes_Read = _read( Handle, File_Buffer, Read_Size );
			if( Bytes_Read == -1 ) Bytes_Read = 0;
			_close( Handle );
			return ( Bytes_Read );
		}
	}

	Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[0] );
	Handle = _open( &TempFilename[ 0 ], _O_RDONLY | _O_BINARY );

	if( Handle != -1 )
	{
#ifdef DEBUG_DATAPATH
		DebugPrintf( "Read_File() : %s\n", &TempFilename[ 0 ] );
#endif
		if( Read_Size == 0 ) Read_Size = _filelength( Handle );
		Bytes_Read = _read( Handle, File_Buffer, Read_Size );
		if( Bytes_Read == -1 ) Bytes_Read = 0;
		_close( Handle );
	}
#endif

	return ( Bytes_Read );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Open file using fopen
				:		data\ as base directory
				:		read from override dir first, then normal
	Input		:		char	*	Filename
				:		char	*	Access right
	Output		:		FILE	*	FilePtr
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * DataPath_fopen( char * Filename, char * Rights )
{
	FILE	*	fp;
	char		TempFilename[ 256 ];
	char		TempFilename2[ 256 ];

	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], Filename, &TempFilename[ 0 ] );
		fp = fopen( &TempFilename[ 0 ], Rights );
		if( fp != NULL ) return( fp );
	}

	Add_Path( &normdata_path[ 0 ], Filename, &TempFilename2[ 0 ] );
	fp = fopen( &TempFilename2[ 0 ], Rights );

#if 0
	if( !fp )
	{
		Msg( "Cannot open /n%s/n%s/n", &TempFilename[ 0 ], &TempFilename2[ 0 ] );
	}
#endif

	return( fp );
}

char * LogFilename = "projectx.log";
char * BatchFilename = "filesused.bat";

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Comment to log
	Input		:		const char * format, .....
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddCommentToLog( const char * format, ... )
{
#ifndef FINAL_RELEASE
	FILE	*	fp;
    static char buf1[256], buf2[512];
	va_list		args;

	if( !CreateLogFile ) return;

	fp = fopen( LogFilename, "a" );

	if( fp )
	{
		va_start( args, format );
		vsprintf( buf1, format, args );
		wsprintf( buf2, "%hs", buf1 );
		fprintf( fp, "%s", buf2 );
		va_end( args );
		fclose( fp );
	}
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Comment to Batch file
	Input		:		const char * format, .....
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddCommentToBat( const char * format, ... )
{
#ifndef FINAL_RELEASE
	FILE	*	fp;
    static char buf1[256], buf2[512];
	va_list		args;

	if( !CreateBatchFile ) return;

	fp = fopen( BatchFilename, "a" );

	if( fp )
	{
		va_start( args, format );
		vsprintf( buf1, format, args );
		wsprintf( buf2, "%hs", buf1 );

		fprintf( fp, "\nREM %s\n\n", buf2 );

		va_end( args );
		fclose( fp );
	}
#endif
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add DataPath Filename to batch file
	Input		:		char	*	Filename
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddDataPathFileToBat( char * Filename )
{
#ifndef FINAL_RELEASE
	FILE	*	fp = NULL;
	FILE	*	fp2 = NULL;
	char		TempFilename[ 512 ];

	if( !Filename ) return;
	if( !Filename[ 0 ] ) return;
	if( !CreateBatchFile ) return;

	fp = fopen( BatchFilename, "a" );

	if( fp )
	{
		if( use_data_path )
		{
			Add_Path( &data_path[ 0 ], Filename, &TempFilename[ 0 ] );
			fp2 = fopen( &TempFilename[ 0 ], "r" );

			if( !fp2 )
			{
				Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[ 0 ] );
				fp2 = fopen( &TempFilename[ 0 ], "r" );
			}
		}
		else
		{
			Add_Path( &normdata_path[ 0 ], Filename, &TempFilename[ 0 ] );
			fp2 = fopen( &TempFilename[ 0 ], "r" );
		}

		if( fp2 )
		{
			fprintf( fp, "copy %%1\\%s %%2\\%s\n", &TempFilename[ 0 ], &TempFilename[ 0 ] );
			fclose( fp2 );
		}

		fclose( fp );
	}
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Filename to batch file
	Input		:		char	*	Filename
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddFileToBat( char * Filename )
{
#ifndef FINAL_RELEASE
	FILE	*	fp = NULL;

	if( !Filename ) return;
	if( !Filename[ 0 ] ) return;
	if( !CreateBatchFile ) return;

	fp = fopen( BatchFilename, "a" );

	if( fp )
	{
		fprintf( fp, "copy %%1\\%s %%2\\%s\n", Filename, Filename );

		fclose( fp );
	}
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Comment to Batch file
	Input		:		const char * format, .....
	Output		:		Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddCommandToBat( const char * format, ... )
{
#ifndef FINAL_RELEASE
	FILE	*	fp;
    static char buf1[256], buf2[512];
	va_list		args;

	if( !CreateBatchFile ) return;

	fp = fopen( BatchFilename, "a" );

	if( fp )
	{
		va_start( args, format );
		vsprintf( buf1, format, args );
		wsprintf( buf2, "%hs", buf1 );
		fprintf( fp, "%s", buf2 );
		va_end( args );
		fclose( fp );
	}
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Compute checksum for file
	Input		:		const char * fname
	Output		:		Nothing
	Side effect :		LevelCheckSum updated
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DataPath_FileCheckSum( char *fname )
{
	FILE	*	fp = NULL;
	FILE	*	fp2 = NULL;
	char		TempFilename[ 512 ];

	if( use_data_path )
	{
		Add_Path( &data_path[ 0 ], fname, &TempFilename[ 0 ] );
		fp2 = fopen( &TempFilename[ 0 ], "r" );

		if( !fp2 )
		{
			Add_Path( &normdata_path[ 0 ], fname, &TempFilename[ 0 ] );
			fp2 = fopen( &TempFilename[ 0 ], "r" );
		}
	}
	else
	{
		Add_Path( &normdata_path[ 0 ], fname, &TempFilename[ 0 ] );
		fp2 = fopen( &TempFilename[ 0 ], "r" );
	}

	if( fp2 )
	{
		fclose( fp2 );
		FileCheckSum( &TempFilename[ 0 ] );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Compute checksum for file
	Input		:		const char * fname
	Output		:		Nothing
	Side effect :		LevelCheckSum updated
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FileCheckSum( char *fname )
{
	FILE *f;
	char buf[ 4096 ];
	uint32 *pdata;
	LONGLONG data;
	uint32 current, size, shift;
	uint32 mask[] = {
		0x00000000L,
		0x000000FFL,
		0x0000FFFFL,
		0x00FFFFFFL };

	if( !fname ) return;
	if( !fname[ 0 ] ) return;

	strncpy( buf, fname, sizeof( buf ) );
	_strlwr( buf );
	if ( !strstr( buf, "data\\levels\\" ) )
		return;
	f = fopen( fname, "rb" );
	if ( !f )
		return;

	while ( !feof( f ) && !ferror( f ) )
	{
		size = fread( (void *) buf, 1, sizeof( buf ), f );
		for ( current = shift = 0; current < size; current += 3 )
		{
			pdata = (uint32 *) &buf[ current ];
			data = *pdata & mask[ ( size - current < 3 ) ? size - current : 3 ];
			LevelCheckSum += data << shift;
			shift = ( shift + 7 ) & 31;
		}
	}

	fclose( f );

	pdata = (uint32 *) &LevelCheckSum;
	DebugPrintf( "FileCheckSum() : %s : 0x%08X%08X\n", fname, pdata[ 1 ], pdata[ 0 ] );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Initialise checksum
	Input		:		LPGUID lpguid
	Output		:		Nothing
	Side effect :		LevelCheckSum reset using lpguid
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitCheckSum( LPGUID lpguid )
{
	uint8 *buf;
	uint32 *pdata;
	LONGLONG data;
	uint32 current, size, shift;
	uint32 mask[] = {
		0x00000000L,
		0x000000FFL,
		0x0000FFFFL,
		0x00FFFFFFL };

	buf = (uint8 *) lpguid;
	LevelCheckSum = 0;
	size = sizeof( *lpguid );
	for ( current = shift = 0; current < size; current += 3 )
	{
		pdata = (uint32 *) &buf[ current ];
		data = *pdata & mask[ ( size - current < 3 ) ? size - current : 3 ];
		LevelCheckSum += data << shift;
		shift = ( shift + 7 ) & 31;
	}

	DebugPrintf( "InitCheckSum() : 0x" );
	for ( current = 1; current <= size; current++ )
	{
		DebugPrintf( "%02X", buf[ size - current ] );
	}
	pdata = (uint32 *) &LevelCheckSum;
	DebugPrintf( " : 0x%08X%08X\n", pdata[ 1 ], pdata[ 0 ] );
}


