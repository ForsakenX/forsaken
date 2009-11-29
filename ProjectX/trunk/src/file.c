#include	"main.h"
#include	"file.h"
#include	"util.h"

#include	<fcntl.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<stdarg.h>
#include	<stdio.h>
#include	<stdlib.h>

#ifdef WIN32
#include	<io.h>		// for various things
#include	<ctype.h>	// for tolower
#include	<windows.h> 	// for DeleteFile
#include	<direct.h>	// for _mkdir
#define		mkdir	_mkdir
// following are in <io.h>
#define		access	_access
#define		open	_open
#define		close	_close
#define		read	_read
#define		write	_write
//
#define		O_BINARY 	_O_BINARY 
#define		O_RDONLY 	_O_RDONLY 
#define		O_CREAT 	_O_CREAT 
#define		O_TRUNC		_O_TRUNC
#define		O_RDWR		_O_RDWR
#define		S_IFDIR		_S_IFDIR
#define		S_IREAD		_S_IREAD
#define		S_IWRITE	_S_IWRITE
#else // ! WIN32
#define		O_BINARY 	0 // no such thing on unixa
#endif

extern BOOL Debug;

static char* convert_path( char* _str )
{
#ifdef WIN32
	return _str;
#else
	static char temp[500];
	char * str = temp;
	strncpy( temp, _str, sizeof(temp) );
	while (*str)
	{
		if (*str == '\\')
			*str = '/';
		else
			*str = (char) tolower(*str);
		str++;
	}
	return temp;
#endif
}

void touch_file( char* str )
{
	char * path = convert_path(str);
	if(!File_Exists(path))
		Write_File(path, "", 0);
}

BOOL is_folder( char* str )
{
	static struct _stat stat;
	char * path = convert_path(str);
	if ( _stat( path, &stat ) == 0 && stat.st_mode & S_IFDIR )
		return TRUE;
	return FALSE;
}

int folder_exists( char *pathspec, ... )
{
	char * path;
	static char pathname[ 256 ];
	static struct _stat stat;
	va_list args;

	va_start( args, pathspec );
	vsprintf( pathname, pathspec, args );
	va_end( args );

	path = convert_path(pathname);

	if ( _stat( path, &stat ) )
	{
		// no such path exists...attempt to create a directory with that name
#ifdef WIN32
		return mkdir( path ) == 0;
#else
		return mkdir( path, 0777 ) == 0;
#endif
	}
	else if ( stat.st_mode & S_IFDIR )
	{
		// path exists and is a directory
		return 1;
	}
	else
	{
		// path exists but is not a directory
		return 0;
	}
	return 0;
}

BOOL File_Exists( char * str )
{
	char * path = convert_path(str);
	return access( path, 00 ) == 0;
}

long Write_File( char * str, char * File_Buffer, long Write_Size )
{
	int		Handle = -1;
	long	Bytes_Written = 0;

	char * path = convert_path(str);

	Handle = open( path, O_CREAT | O_TRUNC | O_BINARY | O_RDWR ,
							  S_IREAD | S_IWRITE );
	if( Handle != -1 ) {
		Bytes_Written = write( Handle, File_Buffer, Write_Size );
		close( Handle );
	}

	return ( Bytes_Written );
}

long Get_File_Size( char * Filename )
{
#ifdef WIN32

	int		Handle = -1;
	long	Read_Size = 0;

	// open the file
	Handle = open( Filename, O_RDONLY | O_BINARY );

	// opened successfully
	if( Handle != -1 )
	{
		// get the size
		Read_Size = _filelength( Handle );

		// close the file
		close( Handle );

	}

	// return the size
	return ( Read_Size );

#else // ! WIN32

	struct stat st;

	char * path = convert_path(Filename);

    if ( !stat( path, &st ) ) 
	{
		return (long) st.st_size;
    }
    else 
	{
		perror("stat");
		return -1;
    }

#endif
}

long Read_File( char * Filename, char * File_Buffer, long Read_Size )
{
	int		Handle = -1;
	long	Bytes_Read = 0;

	// open the file handle
	Handle = open( Filename, O_RDONLY | O_BINARY );

	// file opened successfully
	if( Handle != -1 )
	{
		// get the size of the file
		if( Read_Size == 0 ) Read_Size = _filelength( Handle );

		// read in the file
		Bytes_Read = read( Handle, File_Buffer, Read_Size );

		// set bytes read to 0
		// if no data was read
		if( Bytes_Read == -1 ) Bytes_Read = 0;

		// close up the file
		close( Handle );

	}

	// return the size of the file
	return ( Bytes_Read );
}

BOOL delete_file( char * str )
{
#ifdef WIN32
	return DeleteFile( str );
#else // ! WIN32
	char * path = convert_path(str);
	return (unlink( path )==0);
#endif
}

void AddCommentToLog( char * str )
{
	static FILE * logfile_fp;

	if(!Debug)
		return;

	if(!logfile_fp)
		logfile_fp = fopen( convert_path("Logs\\projectx.log"), "w" );

	if( logfile_fp )
	{
		fprintf( logfile_fp, "%s", str );
		fflush( logfile_fp );
	}
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

char* find_file( char * str )
{
	char * path = convert_path(str);
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
