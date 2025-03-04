#include	<fcntl.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<stdarg.h>
#include	<stdio.h>

#include	"main.h"
#include	"file.h"
#include	"util.h"

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
#define		stat	_stat
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
#include	<time.h>      // for file_time function
#define		O_BINARY 	0 // no such thing on unixa
#endif

#include <unistd.h>

#ifndef S_IREAD
#define S_IREAD 0400
#endif

#ifndef S_IWRITE
#define S_IWRITE 0200
#endif

extern bool Debug;

FILE * file_open(char * filename, char * mode)
{
	return fopen(convert_path(filename), mode);
}

void touch_file( char* path )
{
	if(!File_Exists(path))
		Write_File(path, "", 0);
}

bool is_folder( char* str )
{
	static struct stat s;
	char * path = convert_path(str);
	if ( stat( path, &s ) == 0 && s.st_mode & S_IFDIR )
		return true;
	DebugPrintf("folder '%s' is not a directory\n",path);
	return false;
}

int folder_exists( char *pathspec, ... )
{
	char * path;
	static char pathname[ 256 ];
	static struct stat s;
	va_list args;

	va_start( args, pathspec );
	vsnprintf( pathname, sizeof(pathname)-1, pathspec, args );
	va_end( args );

	pathname[sizeof(pathname)-1] = 0;

	path = convert_path(pathname);

	if ( stat( path, &s ) )
	{
		// no such path exists...attempt to create a directory with that name
#ifdef WIN32
		return mkdir( path ) == 0;
#else
		return mkdir( path, 0777 ) == 0;
#endif
	}
	else if ( s.st_mode & S_IFDIR )
	{
		// path exists and is a directory
		return 1;
	}
	else
	{
		// path exists but is not a directory
		DebugPrintf("'%s' is not a folder\n",path);
		return 0;
	}
	DebugPrintf("folder '%s' does not exist\n",path);
	return 0;
}

bool File_Exists( char * str )
{
	char * path = convert_path(str);
	int rval  = (access( path, 0 ) == 0);
	DebugPrintf("file: check exists '%s' = %s\n",
		path, (rval)?"exits":"missing");
	return rval;
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
		DebugPrintf("file '%s' does not exist\n",path);
		perror("stat");
		return 0;
	}

#endif
}

#ifdef WIN32
bool file_time( const char * path, struct filetime *t )
{
	HANDLE hfile;
	FILETIME Time;
	SYSTEMTIME systime;

	hfile = CreateFile( path,	// pointer to name of the file 
						GENERIC_READ,	// read mode 
						FILE_SHARE_READ,	// share mode 
						NULL,	// pointer to security descriptor 
						OPEN_EXISTING,	// how to create 
						FILE_ATTRIBUTE_NORMAL,	// file attributes 
						NULL 	// handle to file with attributes to copy  
						);
	if ( hfile == INVALID_HANDLE_VALUE )
	{
		DebugPrintf("failed to retrieve file '%s' modification time\n", path);
		// TODO: print windows-specific error string?
		return false;
	}
	GetFileTime( hfile,	NULL,  NULL, &Time );
	FileTimeToSystemTime( &Time, &systime );
	//sprintf( CurrentSavedGameDate, "saved %d-%d-%d at %2d:%02d", systime.wMonth, systime.wDay, systime.wYear, systime.wHour, systime.wMinute );
	t->year = systime.wYear;
	t->month = systime.wMonth;
	t->day = systime.wDay;
	t->hour = systime.wHour;
	t->minute = systime.wMinute;
	t->second = systime.wSecond;
	CloseHandle(hfile);

	return true;
}
#else
bool file_time( const char * path, struct filetime *t )
{
	struct stat st;
	struct tm lt;

	if ( stat( convert_path(path), &st ) != 0 )
	{
		DebugPrintf("failed to retrieve file '%s' modification time\n", path);
		perror("stat");
		return false;
	}
	gmtime_r( &st.st_mtime, &lt );
	t->year = lt.tm_year + 1900;
	t->month = lt.tm_mon + 1;
	t->day = lt.tm_mday;
	t->hour = lt.tm_hour;
	t->minute = lt.tm_min;
	t->second = lt.tm_sec;

	return true;
}
#endif

long Read_File( char * Filename, char * File_Buffer, long Read_Size )
{
	int	Handle = -1;
	long	Bytes_Read = 0;

	char * path = convert_path(Filename);

	// get the size of the file
	if( Read_Size == 0 ) 
		Read_Size = Get_File_Size( path );

	// open the file handle
	Handle = open( path, O_RDONLY | O_BINARY );

	// file opened successfully
	if( Handle != -1 )
	{
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

bool delete_file( char * str )
{
#ifdef WIN32
	return DeleteFile( str );
#else // ! WIN32
	char * path = convert_path(str);
	return (unlink( path )==0);
#endif
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

#include <glob.h>

static glob_t glob_handle;
static int glob_pos = -1;

static char *find_strip_path( char *path )
{
	char *c = strrchr(path, '/');
	if( c )
		return &c[1];
	else
		return path;
}

static int find_globerr( const char *epath, int eerrno )
{
	DebugPrintf("file: glob error while searching for \"%s\": %s\n", epath, strerror(eerrno));
	DebugPrintf("Trying to continue anyway...\n");
	return 0;
}

char* find_file( char * str )
{
	char * path = convert_path(str);
	DebugPrintf("file: find_file called with %s\n",path);
	if (glob_pos >= 0)
		globfree(&glob_handle);
	switch( glob(path, 0, find_globerr, &glob_handle) )
	{
		case 0:
			glob_pos = 0;
			DebugPrintf("file: found file \"%s\" (1 of %d)\n", glob_handle.gl_pathv[0], glob_handle.gl_pathc);
			return find_strip_path(glob_handle.gl_pathv[glob_pos++]);
			break;
		case GLOB_NOSPACE:
			DebugPrintf("file: find_file: ran out of memory!\n");
			break;
		case GLOB_ABORTED:
			DebugPrintf("file: find_file: read error!\n");
			break;
		case GLOB_NOMATCH:
			DebugPrintf("file: find_file: no match\n");
			break;
		default:
			DebugPrintf("file: find_file: unknown return value from glob()\n");
			break;
	}
	return NULL;
}

char* find_next_file( void )
{
	if (glob_pos < 0 || glob_pos >= (int)glob_handle.gl_pathc)
		return NULL;
	DebugPrintf("file: found file \"%s\" (%d of %d)\n", glob_handle.gl_pathv[glob_pos], glob_pos+1, glob_handle.gl_pathc);
	return find_strip_path(glob_handle.gl_pathv[glob_pos++]);
}

void find_close( void )
{
	globfree(&glob_handle);
	glob_pos = -1;
}

#endif
