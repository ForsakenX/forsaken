#ifndef __D3DAPP_H__
#define __D3DAPP_H__

//#include <ddraw.h>
#include <d3d9.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef float D3DVALUE, *LPD3DVALUE;

typedef int D3DMATRIXHANDLE;

typedef struct _D3DVIEWPORT { 
    DWORD    dwSize; 
    DWORD    dwX; 
    DWORD    dwY; 
    DWORD    dwWidth; 
    DWORD    dwHeight; 
    D3DVALUE dvScaleX; 
    D3DVALUE dvScaleY; 
    D3DVALUE dvMaxX; 
    D3DVALUE dvMaxY; 
    D3DVALUE dvMinZ; 
    D3DVALUE dvMaxZ; 
} D3DVIEWPORT, *LPD3DVIEWPORT; 

typedef struct {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
    D3DVALUE	ScaleX; 
    D3DVALUE	ScaleY; 
} render_viewport_t;

typedef struct {
    int     w;                /* width */
    int     h;                /* height */
    int     bpp;              /* bits per pixel */
} render_display_mode_t;

typedef struct {
    int                     NumModes;				/* number of available display modes */
    int                     CurrMode;				/* number of current display mode (only when fullscreen) */
    render_display_mode_t * Mode;					/* desc avail modes */
    render_display_mode_t   ThisMode;				/* description of this mode, identical to Mode[CurrMode] */
    BOOL                    bFullscreen;			/* in fullscreen exclusive mode? */
    render_display_mode_t   WindowsDisplay;			/* current Windows disply mode */
    SIZE                    szClient;				/* dimensions of client win */
    POINT                   pClientOnPrimary;		/* position of client area */
    POINT                   pWindow;				/* position of win */
    BOOL                    bPaused;				/* the app is paused */
    BOOL                    bAppActive;				/* the app is active */
    BOOL                    bMinimized;				/* app window is minimized */
    BOOL                    bRenderingIsOK;			/* All objects etc. necessary rendering are in ok */
} render_info_t;

/*
 * FUNCTION PROTOTYPES
 */

/*
 * D3DAppWindowProc
 * To be truly effective, D3DApp should be allowed to trap incoming window
 * messages such as WM_SIZE.  Call D3DAppWindowProc at the begining of the
 * application's main window WindowProc with the message information.  If
 * bStopProcessing is set to TRUE, stop processing the message and return
 * lresult.
 */
BOOL D3DAppWindowProc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                      UINT message, WPARAM wParam, LPARAM lParam);

/*
 * D3DAppFullscreen
 * Places the app in a fullscreen mode using the current driver.
 */
BOOL D3DAppFullscreen(int mode);

/*
 * D3DAppWindow
 * Places the application in windowed mode at the given client size.  If w
 * and h are D3DAPP_YOUDECIDE, D3DApp will decide on a suitable client size.
 * If called while in fullscreen, restores the display mode and returns the
 * hooked window to the size it was before a call to D3DAppFullscreen or to
 * the size specified.
 */
BOOL D3DAppWindowMode( int mode );
BOOL D3DAppWindow(int w, int h, int bpp);

#ifdef __cplusplus
};
#endif

#endif // __D3DAPP_H__
