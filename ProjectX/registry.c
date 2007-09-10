
/*******************\
|
| Includes
|
\*******************/

#include "registry.h"
#include <stdio.h>
#include "new3d.h"

/*******************\
|
| Globals
|
\*******************/

HKEY  ghCondemnedKey = NULL;
DWORD gdwCondemnedDisp;


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
    return RegQueryValueEx(ghCondemnedKey, lptszName, NULL, &dwType, lpData, lpdwDataSize);
}

