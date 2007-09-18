

/*******************\
|
| Includes
|
\*******************/

#include "new3d.h"
#include "winreg.h"
#include "d3dmain.h"
#include "direct.h"
#include "stdwin.h"
#include <windows.h>
#include <dplay.h>
#include "quat.h"
#include "CompObjects.h"
#include "Object.h"
#include "mydplay.h"
#include "Title.h"
#include "dpthread.h"
#include <stdio.h>
#include <shellapi.h>



/*******************\
|
| Externals
|
\*******************/

extern BOOL Debug;
extern void DebugPrintf( const char * format, ... );


/*******************\
|
| Globals
|
\*******************/

char dirname  [256];  // C:\ProjectX
char basename [256];  // ProjectX.exe
char appname  [256];  // ProjectX
char lobbyKey [256];  // Software\\Microsoft\\DirectPlay\\Applications\\ProjectX
char appKey   [256];  // Software\\ProjectX
HKEY appHKey = NULL;


/*******************\
|
| Prototypes
|
\*******************/

BOOL InitRegistry(void);
BOOL GetRegistrySettings(void);
BOOL CloseRegistry(void);
BOOL OpenOrCreateRegistry(void);
BOOL InstallDirectPlayRegistry(void);
LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);


/*******************\
|
| InitRegistry
|
\*******************/

BOOL InitRegistry(void)
{

	if (!GetRegistrySettings())
		goto failure;

	if (!InstallDirectPlayRegistry())
		goto failure;

	if (!OpenOrCreateRegistry())
		goto failure;

	return TRUE;

failure:

	Msg("%s %s %s %s",
		"A registry error occurred.\n",
		"You may not be able to save/access settings.\n",
		"You may not be able to play networked games.\n",
		"A Lobby may not be able to find the installation.");

	DebugPrintf("Registry Error...\n");

	return FALSE;
}


/*******************\
|
| GetRegistrySettings
|
\*******************/

BOOL GetRegistrySettings(void)
{

	// temporary holders
	char buffer[ 256 ];
	char *strptr;
	char *strptr2;

	//
	//  Window truncates long names to dos 8.3 format
	//  We must use this to expand to the full path
	//

	// get the long path
	DWORD returned_size  = 0;
	char long_path[4096] = "";

	// try to get the path
	returned_size = GetLongPathName( __argv[0], long_path, 4096 );

	// some type of error
	if ( returned_size == 0 )
	{
		Msg("Could not get full exe path: %d",GetLastError());
		DebugPrintf("Could not get full exe Path: %d",GetLastError());
		return FALSE;
	}

	// path is way to long fo...
	if ( returned_size > 4096 )
	{
		Msg("Path to exe is to long");
		DebugPrintf("Path to exe is to long");
		return FALSE;
	}

	// full path to exe used
	// C:\ProjectX\ProjectX.exe
	strcpy( buffer, (const char *)&long_path );

	// get pointer to last occurance of \ character
	strptr = strrchr( buffer, '\\' );

	// set the last \ to 0
	*strptr = 0;

	// check
	if ( strptr == NULL )
	{
		DebugPrintf("Could not get last occurance of '\\'\n");
		return FALSE;
	}

    // get the dirname
	// C:\ProjectX
	strncpy((char*)&dirname,(char*)&buffer,(sizeof(dirname)-1));

	// get the basename
	// ProjectX.exe
	strncpy((char*)&basename,(char*)&strptr[1],(sizeof(basename)-1));

    // get pointer to last occurance of . character
	strptr2 = strrchr( (char*)&strptr[1], '.' );

	// check
	if ( strptr2 == NULL )
	{
		DebugPrintf("Could not get last occurance of '.'\n");
		return FALSE;
	}

	// set the last . to 0
	*strptr2 = 0;

    // get the appname
	// ProjectX
    strncpy((char*)&appname,(char*)&strptr[1],(sizeof(appname)-1));

	// setup lobby key
	_snprintf(lobbyKey,(sizeof(lobbyKey)-1),"%s\\%s","Software\\Microsoft\\DirectPlay\\Applications",&appname);

	// setup app key
	_snprintf(appKey,(sizeof(lobbyKey)-1),"%s\\%s","Software",&appname);

	// success
	return TRUE;

}


/**********************\
|
| OpenOrCreateRegistry
|
\**********************/

BOOL OpenOrCreateRegistry(void)
{

	DWORD	result;
	DWORD	disposition;
	char	description[256];

	/* open or create the GAME_KEY */
	result = RegCreateKeyEx(
				HKEY_LOCAL_MACHINE,			/* registry root */
				(const char*)&appKey,		/* registry key */
				0,							/* reserved */
				NULL,						/* object type */
				REG_OPTION_NON_VOLATILE,	/* save mode */
				KEY_ALL_ACCESS,				/* access rights */
				NULL,						/* lpSecurityAttributes */
				&appHKey,				/* handle */
				&disposition				/* created or opened result */
				);

	// failed
	if ( result != ERROR_SUCCESS )
	{
		// get error message
		FormatMessage(
			  FORMAT_MESSAGE_FROM_SYSTEM,
			  &result,
			  0,
			  0,
			  (LPSTR)&description,
			  (DWORD)sizeof(description)+1,
			  NULL
		);

		DebugPrintf("OpenOrCreateRegistry: Failed to open registry\n");

		// print error message
		Msg("%s %s %s %s",
			"Failed to open application registry!\n",
			"You will not be able to use saved settings!\n",
			"error: ",
			&description);

		// error
		return FALSE;

	}

	
	/* success */
	if ( Debug )
	{

		/* print status */
		if ( disposition == REG_CREATED_NEW_KEY )

			DebugPrintf("OpenOrCreateRegistry: Registry Key Created\n");

		else
		if ( disposition == REG_OPENED_EXISTING_KEY )

			DebugPrintf("OpenOrCreateRegistry: Registry Key Openned\n");

		else

			DebugPrintf("OpenOrCreateRegistry: Registry Unknown Disposition\n");

	}

	/* success */
	return TRUE;

}


/*******************\
|
| CloseRegistry
|
\*******************/

BOOL CloseRegistry(void)
{
	if ( !appHKey ) return FALSE;
	RegCloseKey( appHKey );
	return TRUE;
}


/**********************************************\
|
| InstallDirectPlayRegistry
|   Initializes DirectPlay registry entries.
|   Uses the current exe and path used.
|
\**********************************************/

BOOL InstallDirectPlayRegistry( void )
{

	DWORD	result;
	DWORD	disposition;
	char	description[256];
	HKEY	hKey = NULL;
	BOOL	failed = FALSE;
	char	buffer[ 256 ];

	//
	//  Open/Create Registry Key
	// 

	// open or create the GAME_KEY 
	result = RegCreateKeyEx(
				HKEY_LOCAL_MACHINE,			// registry root
				(const char*)&lobbyKey,		// registry key
				0,							// reserved
				NULL,						// object type
				REG_OPTION_NON_VOLATILE,	// save mode
				KEY_ALL_ACCESS,				// access rights 
				NULL,						// lpSecurityAttributes
				&hKey,						// handle
				&disposition				// created or opened result
				);

	// failed
	if ( result != ERROR_SUCCESS )
	{

		// get error message
		FormatMessage(
			  FORMAT_MESSAGE_FROM_SYSTEM,
			  &result,
			  0,
			  0,
			  (LPSTR)&description,
			  (DWORD)sizeof(description)+1,
			  NULL
		);

		// print error message
		Msg("%s %s %s",
			"Failed to open/create DirectPlay registry!\n",
			"Error: ",
			&description);

		// failed
		goto failed;

	}

	// successfully opened/created registry key

	if ( ! Debug )
	{

		if ( disposition == REG_CREATED_NEW_KEY )

			DebugPrintf("DirectPlay Registry: Registry Key Created\n");

		else
		if ( disposition == REG_OPENED_EXISTING_KEY )

			DebugPrintf("DirectPlay Registry: Registry Key Openned\n");

		else

			DebugPrintf("DirectPlay Registry: Unknown Disposition\n");

	}


	//
	//  Set the registry values
	// 


	// set the guid
	// this is set dynamically in Stdwin.h
	StringFromGUID( &PROJX_GUID, (LPSTR)&buffer );
	if(RegSetValueEx(hKey, "Guid", 0, REG_SZ,
	             (CONST BYTE *)buffer, (sizeof( char ) * (strlen( buffer )+1)) ) != ERROR_SUCCESS)
	{
		DebugPrintf("Failed to set the DirectPlay GUID\n");
		Msg("Failed to set the DirectPlay GUID\n");
		failed = TRUE;
	}

	// set the current working directory
	_getcwd( buffer, 256 );
	if(RegSetValueEx(hKey, "CurrentDirectory", 0, REG_SZ,
	                (CONST BYTE *)buffer, (sizeof( char ) * (strlen( buffer )+1)) ) != ERROR_SUCCESS)
	{
		DebugPrintf("DPlay: Failed to set CurrentDirectory\n");
		Msg("DPlay: Failed to set CurrentDirectory");
		failed = TRUE;
	}

	// set the basename: ProjectX.exe
	if( RegSetValueEx(hKey, "File", 0, REG_SZ,
		              (CONST BYTE *)&basename,
					  (sizeof(char) * (strlen(basename)+1))) != ERROR_SUCCESS )
	{
		DebugPrintf("DPlay: Failed to set File\n");
		Msg("DPlay: Failed to set File");
		failed = TRUE;
	}

	// set the dirname: C:\ProjectX
	if(RegSetValueEx(hKey, "Path", 0, REG_SZ,
	                (CONST BYTE *)&dirname,
					(sizeof(char) * (strlen( buffer )+1))) != ERROR_SUCCESS)
	{
		DebugPrintf("DPlay: Failed to set Path\n");
		Msg("DPlay: Failed to set Path");
		failed = TRUE;
	}

	// close the registry handel
	if ( hKey )
		RegCloseKey( hKey );

	// success
	if ( ! failed )
		return TRUE;

failed:

	Msg("%s %s",	
		"You may not be able to play networked games!\n",
		"Lobbies may not be able to find this installation!\n");

	return FALSE;
}


/*******************\
|
| RegSet
|
\*******************/

LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize)
{
#ifdef UNICODE
    dwSize *= 2; // calc number of bytes
#endif
	if ( ! appHKey )	return -1;
	return RegSetValueEx(appHKey, lptszName, 0, REG_BINARY, lpData, dwSize );
}


/*******************\
|
| RegSetA
|   Stores data as an ascii string in the registry
|
\*******************/


LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize)
{
	if ( ! appHKey )	return -1;
    return RegSetValueEx(appHKey, lptszName, 0, REG_SZ, lpData, dwSize);
}


/*******************\
|
| RegSetA
|   Queries a value from the registry
|
\*******************/


LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize)
{
    DWORD dwType;
	if ( ! appHKey )	return -1;
    return RegQueryValueEx(appHKey, lptszName, NULL, &dwType, lpData, lpdwDataSize);
}

