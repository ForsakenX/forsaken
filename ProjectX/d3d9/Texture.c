
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

static void D3DAppIInitialisePathList()
{

	char *path;

	// leave if we allready did this
    if (PathListInitialised)
        return;

	// start off at 0
    PathList.count = 0;

	// get d3d path
    path = getenv("D3DPATH");
    if (path != NULL)
		// add it
        D3DAppIAddPathList(path);

	// add current path to the list
    D3DAppIAddPathList(".");

	// add textures to the list
    D3DAppIAddPathList("data\\textures");

	// were done
    PathListInitialised = TRUE;

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

