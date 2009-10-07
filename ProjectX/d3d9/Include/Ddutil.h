#if 0 // bjd
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

#endif