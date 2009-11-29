
/*===================================================================
	Header files
===================================================================*/

#include	"main.h"
#include	"file.h"
#include	"util.h"

#include	<fcntl.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<stdarg.h>

/*===================================================================
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
#ifdef WIN32
	static struct _stat stat;
	if ( _stat( path, &stat ) == 0 && stat.st_mode & _S_IFDIR )
		return TRUE;
#endif
	return FALSE;
}

int folder_exists( char *pathspec, ... )
{
#ifdef WIN32
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
#endif
	return 0;
}


/*===================================================================
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



/*===================================================================
	Procedure	:		Write File ( Creating file if non exists )
	Input		:		char	*	Filename
				:		char	*	Buffer to Write
				:		long		Bytes to Write ( 0 = All )
	Output		:		long		Number of bytes Written
===================================================================*/
long Write_File( char * Filename, char * File_Buffer, long Write_Size )
{
#ifdef WIN32
	int		Handle = -1;
	long	Bytes_Written = 0;

	Handle = _open( Filename, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR ,
							  _S_IREAD | _S_IWRITE );
	if( Handle != -1 ) {
		Bytes_Written = _write( Handle, File_Buffer, Write_Size );
		_close( Handle );
	}

	return ( Bytes_Written );
#endif
	return 0;
}

/*===================================================================
	Procedure	:		Change Extension of Filename
	Input		:		uint8	*	Src Filename
				:		uint8	*	Dest Filename
				:		uint8	*	Extension to add
	Output		:		Nothing
===================================================================*/
void Change_Ext( const char * Src, char * Dest, const char * Ext )
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

/*===================================================================
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

/*===================================================================
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

/*===================================================================
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

/*===================================================================
	Procedure	:		Return Size of File given Filename
	Input		:		char	*	Filename
	Output		:		long		Size of File
===================================================================*/
long Get_File_Size( char * Filename )
{
#ifdef WIN32
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
#endif
	return 0;
}

/*===================================================================
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
#ifdef WIN32
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
#endif
}

/*===================================================================
	Procedure	:		Add Comment to log
	Input		:		const char * format, .....
	Output		:		Nothing
===================================================================*/

// log file name
char * LogFilename = "Logs\\projectx.log";

// pointer to log file
FILE * fp;

// add comment to log
void AddCommentToLog( char * str )
{
	if(!Debug)return;

	if(!fp)
		fp = fopen( LogFilename, "w" );

	if( fp )
	{
		fprintf( fp, "%s", str );
		fflush(fp);
	}
}

#ifdef WIN32
#include <windows.h>
#endif

BOOL delete_file( char * path )
{
#ifdef WIN32
	return DeleteFile( path );
#endif
	return 1;
}

//
// Find File Wrapper
//

#ifdef WIN32

static HANDLE find_file_handle;
static WIN32_FIND_DATA find_file_info;

char* find_file( char * path ) // ex: ./www/*.html
{
	find_file_handle = FindFirstFile( path, (LPWIN32_FIND_DATA) &find_file_info );
	if ( find_file_handle == INVALID_HANDLE_VALUE )
		return NULL;
	return &find_file_info.cFileName[0];
}

char* find_next_file( void ) // iterate next file
{
	if(!FindNextFile( find_file_handle, (LPWIN32_FIND_DATA) &find_file_info ))
		return NULL;
	return &find_file_info.cFileName[0];
}

void find_close( void )
{
	FindClose(find_file_handle);
}

#else

char* find_file( char * path )
{
	return NULL;
}

char* find_next_file( void )
{
	return NULL;
}

void find_close( void )
{
}

#endif
