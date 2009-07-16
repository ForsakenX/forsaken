
/*/*===================================================================
	Header files
===================================================================*/

#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<direct.h>
#include	<io.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	"typedefs.h"
#include	"d3dmain.h"
#include	"file.h"
#include	"util.h"

/*/*===================================================================
	External Variables
===================================================================*/

extern BOOL Debug;

// creates the file if not found
void touch_file( char* path )
{
	if(!File_Exists(path))
		Write_File(path, "", 0);
}

BOOL is_folder( char* path )
{
	static struct _stat stat;
	if ( _stat( path, &stat ) == 0 && stat.st_mode & _S_IFDIR )
		return TRUE;
	return FALSE;
}

int folder_exists( char *pathspec, ... )
{
	static char pathname[ 256 ];
	static struct _stat stat;
	va_list args;

	va_start( args, pathspec );
	vsprintf( pathname, pathspec, args );
	va_end( args );
	if ( _stat( pathname, &stat ) )
	{
		// no such path exists...attempt to create a directory with that name
		return !_mkdir( pathname );
	}
	else if ( stat.st_mode & _S_IFDIR )
	{
		// path exists and is a directory
		return 1;
	}
	else
	{
		// path exists but is not a directory
		return 0;
	}
}


/*/*===================================================================
	Procedure	:		See if file exists
	Input		:		char	*	Filename
	Output		:		BOOL		TRUE if exists
===================================================================*/
BOOL File_Exists( char * Filename )
{
	if ( !_access( Filename, 00 ) )
		return TRUE;
	//DebugPrintf("File does not exist: %s\n", Filename);
	return FALSE;
}



/*/*===================================================================
	Procedure	:		Write File ( Creating file if non exists )
	Input		:		char	*	Filename
				:		char	*	Buffer to Write
				:		long		Bytes to Write ( 0 = All )
	Output		:		long		Number of bytes Written
===================================================================*/
long Write_File( char * Filename, char * File_Buffer, long Write_Size )
{
	int		Handle = -1;
	long	Bytes_Written = 0;

	Handle = _open( Filename, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR ,
							  _S_IREAD | _S_IWRITE );
	if( Handle != -1 ) {
		Bytes_Written = _write( Handle, File_Buffer, Write_Size );
		_close( Handle );
	}

	return ( Bytes_Written );
}

/*/*===================================================================
	Procedure	:		Change Extension of Filename
	Input		:		uint8	*	Src Filename
				:		uint8	*	Dest Filename
				:		uint8	*	Extension to add
	Output		:		Nothing
===================================================================*/
void Change_Ext( uint8 * Src, uint8 * Dest, uint8 * Ext )
{
	uint8	*	Char_Ptr;

	int length = strlen( Src );

	if ( ! length )
	{
		strcpy( Dest, Ext ); // set the extension
		Msg("Change_Ext called with Src empty!");
		return;
	}

	Char_Ptr = ( Src + length ) -1;

	while( Char_Ptr != Src && *Char_Ptr != '\\' && *Char_Ptr != ':' && *Char_Ptr != '.' ) Char_Ptr--;

	if( *Char_Ptr == '.' )
	{
		while( Src != Char_Ptr )
			*Dest++ = *Src++;
		strcpy( Dest, Ext );
	}
	else
	{
		strcpy( Dest, Src );
		Dest += strlen( Src );
		strcpy( Dest, Ext );
	}
}

/*/*===================================================================
	Procedure	:		Get Filename from path\filename
	Input		:		uint8	*	Src path\Filename
				:		uint8	*	Dest Filename
				:		uint8	*	Extension to add
	Output		:		Nothing
===================================================================*/
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

/*/*===================================================================
	Procedure	:		Change Extension of Filename
	Input		:		uint8	*	Src Filename
				:		uint8	*	Dest to put ext
	Output		:		Nothing
===================================================================*/
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

/*/*===================================================================
	Procedure	:		Add path to filename
	Input		:		uint8	*	Path\
				:		uint8	*	Src Filename
				:		uint8	*	Dest Path\Filename
	Output		:		Nothing
===================================================================*/
void Add_Path( uint8 * Path, uint8 * Src, uint8 * Dest )
{
	strcpy( Dest, Path );
	Dest = ( Dest + strlen( Path ) );
	strcpy( Dest, Src );
}

/*/*===================================================================
	Procedure	:		Return Size of File given Filename
	Input		:		char	*	Filename
	Output		:		long		Size of File
===================================================================*/
long Get_File_Size( char * Filename )
{

	int		Handle = -1;
	long	Read_Size = 0;

	// open the file
	Handle = _open( Filename, _O_RDONLY | _O_BINARY );

	// opened successfully
	if( Handle != -1 )
	{
		// get the size
		Read_Size = _filelength( Handle );

		// close the file
		_close( Handle );

	}

	// return the size
	return ( Read_Size );

}

/*/*===================================================================
	Procedure	:		Read Part or All of File Into Memory
				:		data\ as base directory
				:		read from override dir first, then normal
	Input		:		char	*	Filename
				:		char	*	Buffer to Load into
				:		long		Bytes to Read ( 0 = All )
	Output		:		long		Number of bytes Read
===================================================================*/
long Read_File( char * Filename, char * File_Buffer, long Read_Size )
{

	int		Handle = -1;
	long	Bytes_Read = 0;

	// open the file handle
	Handle = _open( Filename, _O_RDONLY | _O_BINARY );

	// file opened successfully
	if( Handle != -1 )
	{
		// get the size of the file
		if( Read_Size == 0 ) Read_Size = _filelength( Handle );

		// read in the file
		Bytes_Read = _read( Handle, File_Buffer, Read_Size );

		// set bytes read to 0
		// if no data was read
		if( Bytes_Read == -1 ) Bytes_Read = 0;

		// close up the file
		_close( Handle );

	}

	// return the size of the file
	return ( Bytes_Read );

}

/*/*===================================================================
	Procedure	:		Add Comment to log
	Input		:		const char * format, .....
	Output		:		Nothing
===================================================================*/

// log file name
char * LogFilename = "Logs\\projectx.log";

// pointer to log file
FILE * fp;

// add comment to log
void AddCommentToLog( const char * format, ... )
{
    static char buf1[256], buf2[512];
	va_list		args;

	// only log if debuggin on
	if(!Debug)return;

	// no file open
	if(!fp)
		fp = fopen( LogFilename, "w" );

	if( fp )
	{
		va_start( args, format );
		vsprintf( buf1, format, args );
		wsprintf( buf2, "%hs", buf1 );
		fprintf( fp, "%s", buf2 );
		va_end( args );
		fflush(fp);
	}
}

// use this for whatever you want

void WriteToFile( char* path, const char * format, ... )
{
    static char buf1[256], buf2[512];
	va_list		args;

	FILE * fp = fopen( path, "a" );

	if( fp )
	{
		va_start( args, format );
		vsprintf( buf1, format, args );
		wsprintf( buf2, "%hs", buf1 );
		fprintf( fp, "%s", buf2 );
		va_end( args );
		fflush(fp);
		fclose(fp);
	}
	else
	{
		DebugPrintf("WriteToFile() Failed to Open File.\n");
	}
}
