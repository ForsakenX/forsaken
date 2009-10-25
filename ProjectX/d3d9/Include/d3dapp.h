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
	int		rate;			  /* refresh rate (HZ) */
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

#ifdef __cplusplus
};
#endif

#endif // __D3DAPP_H__
