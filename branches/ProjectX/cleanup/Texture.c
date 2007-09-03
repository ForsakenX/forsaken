/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 8 $
 *
 * $Header: /PcProjectX/Texture.c 8     11/29/97 4:35p Phillipd $
 *
 * $Log: /PcProjectX/Texture.c $
 * 
 * 8     11/29/97 4:35p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 7     11/08/97 10:12 Collinsd
 * Added override data directory option. ( SFX don't work yet! )
 * 
 * 6     10/30/96 2:55p Phillipd
 * 
 * 5     10/23/96 4:24p Phillipd
 * Lots of crap taken out of D3dapp and its associated functions and
 * files....
 * 
 * 4     18/10/96 17:46 Collinsd
 * Changed all file loading to load from tidy directories.
 * 
 * 3     10/18/96 2:21p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: texture.c
 *
 *  Loads and manages textures.  Part of D3DApp.
 *
 *  D3DApp is a collection of helper functions for Direct3D applications.
 *  D3DApp consists of the following files:
 *      d3dapp.h    Main D3DApp header to be included by application
 *      d3dappi.h   Internal header
 *      d3dapp.c    D3DApp functions seen by application.
 *      ddcalls.c   All calls to DirectDraw objects except textures
 *      d3dcalls.c  All calls to Direct3D objects except textures
 *      texture.c   Texture loading and managing texture list
 *      misc.c      Miscellaneous calls
 */

#include "typedefs.h"
#include "d3dappi.h"
#include "XMem.h"

/*
 * STATIC FUNCTION DECLARATIONS
 */
static void D3DAppIAddPathList(const char *path);
static void D3DAppIInitialisePathList();

extern	int use_data_path;
extern	char data_path[ 128 ];

/***************************************************************************/
/*                         Finding Textures                                */
/***************************************************************************/

#define MAXPATH    256
#define PATHSEP    ';'
#define FILESEP    '\\'
#define MAXCONTENTS 25

static int PathListInitialised = FALSE;

/*
 * PathList structure
 * A list of directories in which to search for the texture.
 */
static struct {
    int count;
    char *contents[MAXCONTENTS];
} PathList;

/*
 * D3DAppIAddPathList
 * Add this string to the search path list
 */
static void 
D3DAppIAddPathList(const char *path)
{
    char *p;
    char *elt;
    int len;

    while (path) {
        p = LSTRCHR(path, PATHSEP);
        if (p)
            len = p - path;
        else
            len = lstrlen(path);
        elt = (char *) malloc(len + 1);
        if (elt == NULL)
            return;
        lstrcpyn(elt, path, len + 1);
        elt[len] = '\0';
        PathList.contents[PathList.count] = elt;
        PathList.count++;
        if (p)
            path = p + 1;
        else
            path = NULL;
        if (PathList.count == MAXCONTENTS)
            return;
    }
    return;
}

/*
 * D3DAppIInitialisePathList
 * Create a search path with the D3DPATH env. var and D3D Path registry entry
 */
static void 
D3DAppIInitialisePathList()
{
    char* path;

    if (PathListInitialised)
        return;
    PathListInitialised = TRUE;

    PathList.count = 0;
    path = getenv("D3DPATH");

    D3DAppIAddPathList(".");
    if (path != NULL) {
        D3DAppIAddPathList(path);
        return;
    }
    D3DAppIAddPathList(".\\Data");
    if (path != NULL) {
        D3DAppIAddPathList(path);
        return;
    }

    D3DAppIAddPathList(".\\Data\\textures");
    if (path != NULL) {
        D3DAppIAddPathList(path);
        return;
    }

	if( use_data_path )
	{
	    D3DAppIAddPathList( &data_path[ 0 ] );
	    if (path != NULL) {
	        D3DAppIAddPathList(path);
	        return;
	    }
	}

}


/*
 * D3DAppIFindFile
 * Find and open a file using the current search path.
 */
FILE*
D3DAppIFindFile(const char *name, const char *mode)
{
    FILE *fp;
    char buf[MAXPATH];
    static char filesep[] = {FILESEP, 0};
    int i;

    D3DAppIInitialisePathList();

    fp = fopen(name, mode);
    if (fp != NULL)
        return fp;

    for (i = 0; i < PathList.count; i++) {
        lstrcpy(buf, PathList.contents[i]);
        lstrcat(buf, filesep);
        lstrcat(buf, name);
        fp = fopen(buf, mode);
        if (fp)
            return fp;
    }
    return NULL;
}

/*
 * D3DAppIReleasePathList
 * Release the path list for program termination
 */
void
D3DAppIReleasePathList(void)
{
    int i;
    for (i = 0; i < PathList.count; i++) {
        free(PathList.contents[i]);
        PathList.contents[i] = NULL;
    }
    PathList.count = 0;
    PathListInitialised = FALSE;
}

