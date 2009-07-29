
/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dappi.h
 *
 *  Internal header.  Part of D3DApp.
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

#ifndef __D3DAPPI_H__
#define __D3DAPPI_H__

/*
 * INCLUDED HEADERS
 */
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <search.h>
//#include <ddraw.h>
#include <d3d9.h>
#include "d3dapp.h"
#include "d3dmacs.h"
#include "d3dmain.h"
#include "lclib.h"  /* lclib is a override for standard string lib */

#ifdef __cplusplus
extern "C" {
#endif

#define TRILINEAR_MENU_OPTION

/*
 * MACROS
 */
#undef ATTEMPT
#define ATTEMPT(x) if (!(x)) goto exit_with_error
#undef RELEASE
#define RELEASE(x) if (x) { x->lpVtbl->Release(x); x = NULL; }
#undef MAX
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#undef MIN
#define MIN(x, y) ((x) > (y)) ? (y) : (x)
#undef ZEROMEM
#define ZEROMEM(x) memset(&x, 0, sizeof(x))

/*
 * GLOBAL VARIABLES
 *    see d3dapp.c for descriptions
 */
extern D3DAppInfo d3dappi;
extern D3DAppRenderState d3dapprs;
extern BOOL bD3DAppInitialized;
extern HRESULT LastError;
//extern LPDIRECTDRAWCLIPPER lpClipper;
//extern LPDIRECTDRAWPALETTE lpPalette;
extern BOOL(*D3DDeviceDestroyCallback)(LPVOID);
extern LPVOID D3DDeviceDestroyCallbackContext;
//extern BOOL(*D3DDeviceCreateCallback)(int, int, LPDIRECT3DVIEWPORT*, LPVOID);
extern LPVOID D3DDeviceCreateCallbackContext;
extern BOOL bPrimaryPalettized;
extern BOOL bPaletteActivate;
extern BOOL bIgnoreWM_SIZE;
//extern PALETTEENTRY ppe[256];
//extern PALETTEENTRY Originalppe[256];
extern char LastErrorString[256];
extern SIZE szLastClient;
extern SIZE szBuffers;
extern int CallbackRefCount;

#ifdef __cplusplus
};
#endif

/*
 * INTERNAL FUNCTION PROTOTYPES
 */
BOOL D3DAppISetRenderState(void);
BOOL D3DAppIEnumDrivers(void);
BOOL D3DAppIPickDriver(int* driver, DWORD depths);
BOOL D3DAppICreateD3D(void);
BOOL D3DAppIEnumTextureFormats(void);
BOOL D3DAppICreateZBuffer(int w, int h, int driver);
BOOL D3DAppICreateDevice(int driver);
BOOL D3DAppISetCoopLevel(HWND hwnd, BOOL bFullscreen);
BOOL D3DAppISetDisplayMode(int w, int h, int bpp);
//BOOL D3DAppICheckForPalettized(void);
BOOL D3DAppIRestoreDispMode(void);
BOOL D3DAppIVerifyDriverAndMode(int* lpdriver, int* lpmode);
BOOL D3DAppIFilterDrivers(int mode);
DWORD D3DAppTotalVideoMemory(void);
DWORD D3DAppFreeVideoMemory(void);
BOOL D3DAppIEnumDisplayModes(void);
BOOL D3DAppIPickDisplayMode(int* mode, DWORD depths);
BOOL D3DAppISetDispMode(int w, int h, int bpp);
//BOOL D3DAppICreateDD(DWORD flags);
BOOL D3DAppIFilterDisplayModes(int driver);
/* bjd
HRESULT D3DAppICreateSurface(LPDDSURFACEDESC lpDDSurfDesc,
                LPDIRECTDRAWSURFACE FAR *lpDDSurface);
HRESULT D3DAppIGetSurfDesc(LPDDSURFACEDESC lpDDSurfDesc,
                           LPDIRECTDRAWSURFACE lpDDSurf);
*/
BOOL D3DAppICreateBuffers(HWND hwnd, int w, int h, int bpp,BOOL bFullscreen);
//BOOL D3DAppIRememberWindowsMode(void);
BOOL D3DAppIClearBuffers(void);
DWORD D3DAppIBPPToDDBD(int bpp);
void D3DAppIReleasePathList(void);
void D3DAppISetClientSize(HWND hwnd, int w,int h,BOOL bReturnFromFullscreen);
void D3DAppIGetClientWin(HWND hwnd);
void D3DAppISetDefaults(void);
BOOL D3DAppICallDeviceDestroyCallback(void);
BOOL D3DAppICallDeviceCreateCallback(int w, int h);

BOOL Init3DRenderer(HWND hwnd, D3DAppInfo** D3DApp);
/*
void D3DAppIMergeRectLists(int* dstnum, LPD3DRECT dst, int src1num,
                           LPD3DRECT src1, int src2num, LPD3DRECT src2);
void D3DAppICopyRectList(int* dstnum, LPD3DRECT dst, int srcnum,
                         LPD3DRECT src);
BOOL D3DAppIHandleWM_SIZE(LRESULT* lresult, HWND hwnd, UINT message,
                          WPARAM wParam, LPARAM lParam);
*/

/* bjd - move this stuff somewhere more appropriate */
typedef struct RENDEROBJECT
{
	LPDIRECT3DVERTEXBUFFER9 lpD3DVertexBuffer;
	int startVert;
	int numVerts;
	LPDIRECT3DTEXTURE9 texture;

	/* add whatever else needed.. */
} RENDEROBJECT;

typedef struct RENDERSTATE
{
	int blah; // temp
} RENDERSTATE;

HRESULT FSBeginScene();
HRESULT FSEndScene();
HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int size);
HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, D3DLVERTEX **verts);
HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject);
HRESULT FSDrawVertexBuffer(RENDEROBJECT *renderObject);
void FSReleaseRenderObject(RENDEROBJECT *renderObject);
HRESULT FSSetViewPort(D3DVIEWPORT9 *newViewPort);
HRESULT FSGetViewport(D3DVIEWPORT9 *returnViewPort);
HRESULT FSSetViewPort(D3DVIEWPORT9 *newViewPort);
HRESULT FSSetMatrix(D3DTRANSFORMSTATETYPE type, const D3DMATRIX *matrix);
HRESULT FSGetMatrix(D3DTRANSFORMSTATETYPE type, D3DMATRIX *matrix);

typedef struct _OLDD3DLVERTEX {
    union {
    D3DVALUE     x;             /* Homogeneous coordinates */
    D3DVALUE     dvX;
    };
    union {
    D3DVALUE     y;
    D3DVALUE     dvY;
    };
    union {
    D3DVALUE     z;
    D3DVALUE     dvZ;
    };
    DWORD            dwReserved;
    union {
    D3DCOLOR     color;         /* Vertex color */
    D3DCOLOR     dcColor;
    };
    union {
    D3DCOLOR     specular;      /* Specular component of vertex */
    D3DCOLOR     dcSpecular;
    };
    union {
    D3DVALUE     tu;            /* Texture coordinates */
    D3DVALUE     dvTU;
    };
    union {
    D3DVALUE     tv;
    D3DVALUE     dvTV;
    };
} OLDD3DLVERTEX, *LPOLDD3DLVERTEX;

/* bjd - remove when done. taken from SDK */
#define D3DCLIP_LEFT                0x00000001L
#define D3DCLIP_RIGHT               0x00000002L
#define D3DCLIP_TOP					0x00000004L
#define D3DCLIP_BOTTOM              0x00000008L
#define D3DCLIP_FRONT               0x00000010L
#define D3DCLIP_BACK                0x00000020L

void __cdecl D3DAppISetErrorString( LPSTR fmt, ... );

void __cdecl dpf( LPSTR fmt, ... );

FILE * D3DAppIFindFile(const char *name, const char *mode);
#endif // __D3DAPPI_H__
