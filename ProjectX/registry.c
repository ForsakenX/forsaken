
/*******************\
|
| Includes
|
\*******************/

#include "registry.h"
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

/*******************\
|
| Globals
|
\*******************/

HKEY  ghCondemnedKey = NULL;


/*******************\
|
| Prototypes
|
\*******************/

BOOL InitRegistry(void);
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
	int i = 1;
	i = (InstallDirectPlayRegistry()) ? 1 : 0;
	i = ((i)&&OpenOrCreateRegistry()) ? 1 : 0;
	return (i) ? TRUE : FALSE;
}


/*******************\
|
| RegSet
|
\*******************/

BOOL CloseRegistry(void)
{
	if ( !ghCondemnedKey ) return FALSE;
	RegCloseKey( ghCondemnedKey );
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
				REGISTRY_GAME_KEY,			/* registry key */
				0,							/* reserved */
				NULL,						/* object type */
				REG_OPTION_NON_VOLATILE,	/* save mode */
				KEY_ALL_ACCESS,				/* access rights */
				NULL,						/* lpSecurityAttributes */
				&ghCondemnedKey,			/* handle */
				&disposition				/* created or opened result */
				);

	// successfull
	if ( result == ERROR_SUCCESS )
	{
		/*
		if ( disposition == REG_CREATED_NEW_KEY )
		{
			Msg("Registry Key Created");
		}
		else
		if ( disposition == REG_OPENED_EXISTING_KEY )
		{
			Msg("Registry Key Openned");
		}
		else
		{
			Msg("Unknown Disposition");
		}
		*/
	}
	// failed
	else
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
		Msg("%s %s %s %s",
			"Failed to open registry!\n",
			"You will not be able to use saved settings!\n",
			"error: ",
			&description);

		// error
		return FALSE;
	}

	/* success */
	return TRUE;
}


/**********************************************\
|
| InstallRegistry
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
	
	char buf[ 256 ];
	char *appname;
	
	BOOL failed = FALSE;

	/* open or create the GAME_KEY */
	result = RegCreateKeyEx(
				HKEY_LOCAL_MACHINE,			/* registry root */
				REGISTRY_LOBBY_KEY,			/* registry key */
				0,							/* reserved */
				NULL,						/* object type */
				REG_OPTION_NON_VOLATILE,	/* save mode */
				KEY_ALL_ACCESS,				/* access rights */
				NULL,						/* lpSecurityAttributes */
				&hKey,						/* handle */
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

		// print error message
		Msg("%s %s %s",
			"Failed to open/create DirectPlay registry!\n",
			"Error: ",
			&description);

		// failed
		goto failed;

	}

	// successfully opened/created registry key

	/*
	if ( disposition == REG_CREATED_NEW_KEY )
	{
		Msg("Registry Key Created");
	}
	else
	if ( disposition == REG_OPENED_EXISTING_KEY )
	{
		Msg("Registry Key Openned");
	}
	else
	{
		Msg("Unknown Disposition");
	}
	*/

	// set the guid
	StringFromGUID( &PROJX_GUID, (LPSTR)&buf );
	if(RegSetValueEx(hKey, "Guid", 0, REG_SZ,
	             (CONST BYTE *)buf, (sizeof( char ) * (strlen( buf )+1)) ) != ERROR_SUCCESS)
	{
		Msg("Failed to set the DirectPlay GUID\n");
		failed = TRUE;
	}

	// set the current working directory
	_getcwd( buf, 256 );
	if(RegSetValueEx(hKey, "CurrentDirectory", 0, REG_SZ,
	                (CONST BYTE *)buf, (sizeof( char ) * (strlen( buf )+1)) ) != ERROR_SUCCESS)
	{
		Msg("DPlay: Failed to set CurrentDirectory");
		failed = TRUE;
	}

	// full path to exe used
	// C:\ProjectX\ProjectX.exe
	strcpy( buf, __argv[ 0 ] );

	/* get pointer to last occurance of "\" character */
	appname = strrchr( buf, 92 );

	// set file name
	// appname[1] start after the last "\" until "\0"
	// ProjectX.exe
	if( appname != NULL &&
		(RegSetValueEx(hKey, "File", 0, REG_SZ,
		              (CONST BYTE *)&appname[ 1 ],
					  (sizeof(char) * (strlen(appname)+1))) != ERROR_SUCCESS))
	{
		Msg("DPlay: Failed to set File");
		failed = TRUE;
	}

	/* set the last "\" to "\0" */
	/* effectively sets the end of the buff character array to the last "\" */
	*appname = 0;

	// set path
	if(RegSetValueEx(hKey, "Path", 0, REG_SZ,
	                (CONST BYTE *)buf,
					(sizeof(char) * (strlen( buf )+1))) != ERROR_SUCCESS)
	{
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
	if ( ! ghCondemnedKey )	return -1;
	return RegSetValueEx(ghCondemnedKey, lptszName, 0, REG_BINARY, lpData, dwSize );
}


/*******************\
|
| RegSetA
|   Stores data as an ascii string in the registry
|
\*******************/


LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize)
{
	if ( ! ghCondemnedKey )	return -1;
    return RegSetValueEx(ghCondemnedKey, lptszName, 0, REG_SZ, lpData, dwSize);
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
	if ( ! ghCondemnedKey )	return -1;
    return RegQueryValueEx(ghCondemnedKey, lptszName, NULL, &dwType, lpData, lpdwDataSize);
}

