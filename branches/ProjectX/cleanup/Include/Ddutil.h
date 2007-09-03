/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 9 $
 *
 * $Header: /PcProjectX/Include/Ddutil.h 9     3/26/98 4:59p Phillipd $
 *
 * $Log: /PcProjectX/Include/Ddutil.h $
 * 
 * 9     3/26/98 4:59p Phillipd
 * 
 * 8     3/14/98 3:32p Phillipd
 * 
 * 7     28/01/98 12:57 Oliverc
 * Changed allocation checking to working on DEBUG_ON flag instead of the
 * generic _DEBUG
 * 
 * 6     26/01/98 18:23 Philipy
 * fixed video memory leaks
 * splash screens now display after release view, and call InitScene,
 * InitView after  completion
 * 
 * 5     16/10/97 18:13 Philipy
 * 
 * 4     8/15/96 4:46p Phillipd
 * 
 * 3     7/24/96 2:11p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

extern IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap);

#ifdef DEBUG_ON
extern IDirectDrawSurface * DDLoadBitmapDebug(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy, char *in_file, int in_line );
#else
extern IDirectDrawSurface * DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);
#endif // DEBUG_ON

extern IDirectDrawSurface * DDLoadBitmapOverlay(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);
extern HRESULT              DDReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap);
extern HRESULT              DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
extern HRESULT				DDCopyTextureBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
extern DWORD                DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);
extern HRESULT              DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);
extern IDirectDrawSurface * DDLoadBitmapTexture(IDirectDraw *pdd, LPCSTR szBitmap, LPDDSURFACEDESC ddsd , int Scale, BOOL bSquareOnly);
extern	IDirectDrawSurface * DDLoadBitmapTextureMipMap(IDirectDraw *pdd, LPCSTR szBitmap, LPDDSURFACEDESC ddsd , int Scale , BOOL bSquareOnly);
extern BOOL HasBmpGotRealBlack( LPCSTR szBitmap);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
