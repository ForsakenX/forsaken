
/*==========================================================================
 *
 *
 *  File:       DDSurfhand.h
 *  Content:    Direct Draw Surface handle include file
 *
 *
 ***************************************************************************/

#ifndef DDSURFHAND_INCLUDED
#define DDSURFHAND_INCLUDED

#ifdef DEBUG_ON

#define MakeDDSurf( A, B, C, D, E, F )	XMakeDDSurf( (A), (B), (C), (D), (E), (F), __FILE__, __LINE__ )
#define ReleaseDDSurf( A )	{XReleaseDDSurf( (A), __FILE__, __LINE__ ); A = NULL;}

#define DDLoadBitmap( A, B, C, D ) DDLoadBitmapDebug( A, B, C, D, __FILE__, __LINE__ ) 

/*
 * fn prototypes
 */

#ifdef __cplusplus
extern "C" {
#endif

void DDSurf_Init( void );
BOOL XMakeDDSurf(  IDirectDraw *pdd , LPDDSURFACEDESC lpDDSurfaceDesc , LPDIRECTDRAWSURFACE FAR *lplpDDSurface , IUnknown FAR *pUnkOuter, char *prev_in_file, int prev_in_line, char *in_file, int in_line );

void XReleaseDDSurf( LPDIRECTDRAWSURFACE Pnt, char *in_file, int in_line );
int UnMallocedDDSurfBlocks( void );

#ifdef __cplusplus
};
#endif

#else // DEBUG_ON

#ifdef __cplusplus
extern "C" {
#endif			  RELEASE

BOOL MakeDDSurf(  IDirectDraw *pdd , LPDDSURFACEDESC lpDDSurfaceDesc , LPDIRECTDRAWSURFACE FAR *lplpDDSurface , IUnknown FAR *pUnkOuter );
#ifdef __cplusplus
#define ReleaseDDSurf( x ) {if (x != NULL) {x->Release(); x = NULL;}}
#else
#define ReleaseDDSurf( x ) {if (x != NULL) {x->lpVtbl->Release( x ); x = NULL;}}
#endif

#ifdef __cplusplus
};
#endif

#endif // DEBUG_ON

#endif
