/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 2 $
 *
 * $Header: /PcProjectX/Include/DDSurfhand.h 2     28/01/98 12:56 Oliverc $
 *
 * $Log: /PcProjectX/Include/DDSurfhand.h $
 * 
 * 2     28/01/98 12:56 Oliverc
 * Changed allocation checking to working on DEBUG_ON flag instead of the
 * generic _DEBUG
 * 
 * 1     26/01/98 19:05 Philipy
 * 
 * 4     12/01/97 3:27p Phillipd
 * 
 * 3     1/10/97 4:25p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

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
