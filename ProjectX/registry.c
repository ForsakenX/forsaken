

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

HKEY appHKey = NULL;


/*******************\
|
| Prototypes
|
\*******************/

BOOL InitRegistry();
BOOL CloseRegistry();
BOOL OpenOrCreateRegistry();
LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);

#ifdef DPLAYLOBBY
BOOL InstallDirectPlayRegistry();
#endif

/*******************\
|
| InitRegistry
|
\*******************/

BOOL InitRegistry(void)
{

#ifdef DPLAYLOBBY
	if (!InstallDirectPlayRegistry())
		goto failure;
#endif

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


/**********************\
|
| OpenOrCreateRegistry
|
\**********************/

BOOL OpenOrCreateRegistry()
{

	DWORD	result;
	DWORD	disposition;
	char	description[256];

	/* open or create the GAME_KEY */
	result = RegCreateKeyEx(
				HKEY_LOCAL_MACHINE,			/* registry root */
				/* registry key */
#ifdef DEBUG_ON
				"Software\\ProjectX.debug",
#else
				"Software\\ProjectX",
#endif
				0,							/* reserved */
				NULL,						/* object type */
				REG_OPTION_NON_VOLATILE,	/* save mode */
				KEY_ALL_ACCESS,				/* access rights */
				NULL,						/* lpSecurityAttributes */
				&appHKey,					/* handle */
				&disposition				/* created or opened result */
				);

	// failed
	if ( result != ERROR_SUCCESS )
	{
		// get error message
		FormatMessage(
			  FORMAT_MESSAGE_FROM_SYSTEM,
			  NULL,
			  result,
			  0,
			  (LPSTR)&description,
			  (DWORD)sizeof(description)+1,
			  NULL
		);

		DebugPrintf("OpenOrCreateRegistry: Failed to open registry: %s\n",&description);

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

#ifdef DPLAYLOBBY
/**********************************************\
|
| InstallDirectPlayRegistry
|   Initializes DirectPlay registry entries.
|   Uses the current exe and path used.
|
\**********************************************/

BOOL InstallDirectPlayRegistry()
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
				// registry key
#ifdef DEBUG_ON
				"Software\\Microsoft\\DirectPlay\\Applications\\ProjectX.debug",
#else
				"Software\\Microsoft\\DirectPlay\\Applications\\ProjectX",
#endif
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
			  NULL
			  result,
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
#endif

/*******************\
|
| RegSet
|
\*******************/

LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize)
{
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

