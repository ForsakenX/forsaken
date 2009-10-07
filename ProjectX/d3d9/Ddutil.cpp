#if 0 // bjd
/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/

#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
//#include <ddraw.h>
#include "ddutil.h"
#include "ddsurfhand.h"
#include "math.h"

/*
 * Defines
 */
#define	MAXMIPMAP 8
#define	MIPMAPMIN 16


extern "C" {
	#include "util.h"
	extern BOOL bPrimaryPalettized;
	extern double Gamma;
}

/*
 *  DDLoadBitmap
 *
 *  create a DirectDrawSurface from a bitmap resource.
 *
 */

#ifdef DEBUG_ON

IDirectDrawSurface * DDLoadBitmapDebug(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy, char *in_file, int in_line )
{
    HBITMAP             hbm;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    IDirectDrawSurface *pdds;
	static int			bitmaps;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
	//    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);

	//    if (hbm == NULL)
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

	DebugPrintf( "DDLoadBitmap: bitmap %2d = %s\n", bitmaps++, szBitmap );

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

    //
    // create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;

	if ( !MakeDDSurf( pdd, &ddsd, &pdds, NULL, in_file, in_line ) )
		return NULL;

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    DeleteObject(hbm);

    return pdds;
}

#else

IDirectDrawSurface * DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP             hbm;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    IDirectDrawSurface *pdds;
	static int			bitmaps;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
//    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);

//    if (hbm == NULL)
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

	DebugPrintf( "DDLoadBitmap: bitmap %2d = %s\n", bitmaps++, szBitmap );

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

    //
    // create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;

	 if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
        return NULL;

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    DeleteObject(hbm);

    return pdds;
}

#endif // DEBUG_ON

/*
 *  DDLoadBitmapOverlay
 *
 *  create a DirectDrawSurface from a bitmap resource.
 *
 */
extern "C" IDirectDrawSurface * DDLoadBitmapOverlay(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP             hbm;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    IDirectDrawSurface *pdds;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
//    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);
//    if (hbm == NULL)

        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

    //
    // create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;

	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd.ddpfPixelFormat.dwFourCC = BI_RGB;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 16;


    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
        return NULL;

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    DeleteObject(hbm);

    return pdds;
}


/*
 *  DDReLoadBitmap
 *
 *  load a bitmap from a file or resource into a directdraw surface.
 *  normaly used to re-load a surface after a restore.
 *
 */
HRESULT DDReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap)
{
    HBITMAP             hbm;
    HRESULT             hr;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
//    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

//    if (hbm == NULL)
        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }

    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }


    DeleteObject(hbm);
    return hr;
}

HRESULT HBitmapToDirectDrawSurface(LPDIRECTDRAWSURFACE pdds, HBITMAP hbm)
{ 
    HRESULT hr;

	hr = DDCopyTextureBitmap(pdds, hbm, 0, 0, 0, 0);
	return hr;
}
/*
 *  DDCopyBitmap
 *
 *  draw a bitmap into a DirectDrawSurface
 *
 */
extern "C" HRESULT DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC                 hdcImage;
    HDC                 hdc;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    HRESULT             hr;
	RGBQUAD	Colours[256];
	int		numofcolours;
	int		i;
	BYTE  GammaTab[256];
	double k;
	int r,g,b;
    unsigned long m;
    int s;
	int red_shift, red_scale;
	int green_shift, green_scale;
	int blue_shift, blue_scale;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;

	// recover in release build
	if (Gamma <= 0)
	    Gamma = 1.0;
	k = 255.0/pow(255.0, 1.0/Gamma);
	for (i = 0; i <= 255; i++)
	{
	    GammaTab[i] = (BYTE)(k*(pow((double)i, 1.0/Gamma)));
		if( i )
		{
			if( !GammaTab[i] )
				GammaTab[i] = 1;
		}
	}

    //
    // make sure this surface is restored.
    //
    pdds->Restore();

    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);


	//
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
    dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;


    //
    // get size of surface.
    //
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    pdds->GetSurfaceDesc(&ddsd);


    if( ddsd.ddpfPixelFormat.dwRBitMask )
	{
		for (s = 0, m = ddsd.ddpfPixelFormat.dwRBitMask; !(m & 1);
															   s++, m >>= 1);
		red_shift = s;
		red_scale = 255 / (ddsd.ddpfPixelFormat.dwRBitMask >> s);
	}else{
		red_scale = 0;
	}
    if( ddsd.ddpfPixelFormat.dwGBitMask )
	{
		for (s = 0, m = ddsd.ddpfPixelFormat.dwGBitMask; !(m & 1);
															   s++, m >>= 1);
		green_shift = s;
		green_scale = 255 / (ddsd.ddpfPixelFormat.dwGBitMask >> s);
	}else{
		green_scale = 0;
	}
    if( ddsd.ddpfPixelFormat.dwBBitMask )
	{
		for (s = 0, m = ddsd.ddpfPixelFormat.dwBBitMask; !(m & 1);
															   s++, m >>= 1);
		blue_shift = s;
		blue_scale = 255 / (ddsd.ddpfPixelFormat.dwBBitMask >> s);
	}else{
		blue_scale = 0;
	}
	blue_scale *= 2;
	red_scale *= 2;
	green_scale *= 2;

	if( bm.bmBitsPixel <= 8 )
	{
		numofcolours = GetDIBColorTable( hdcImage,	// handle of device context whose DIB is of interest 
										0,			// color table index of first entry to retrieve
										256,			// number of color table entries to retrieve
										&Colours[0]);		// pointer to buffer that receives color table entries
		for( i = 0 ; i < numofcolours ; i ++ )
		{
			r = Colours[i].rgbRed;
			g = Colours[i].rgbGreen;
			b = Colours[i].rgbBlue;

			Colours[i].rgbRed = GammaTab[Colours[i].rgbRed];
			Colours[i].rgbGreen = GammaTab[Colours[i].rgbGreen];
			Colours[i].rgbBlue = GammaTab[Colours[i].rgbBlue];


			if( ( r + g + b ) != 0 )
			{
				if( ( Colours[i].rgbRed <= red_scale ) && ( Colours[i].rgbGreen <= green_scale ) && ( Colours[i].rgbBlue <= blue_scale )	)
				{
					Colours[i].rgbBlue = blue_scale;
				}
			}
		}
		SetDIBColorTable( hdcImage,	// handle of device context whose DIB is of interest 
						  0,			// color table index of first entry to retrieve
						  numofcolours,			// number of color table entries to retrieve
						  &Colours[0]);		// pointer to buffer that receives color table entries
	}

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
		SetStretchBltMode( hdc, HALFTONE  ); 
		SetBrushOrgEx( hdc , 0 , 0 , NULL );
		SetStretchBltMode( hdcImage, HALFTONE  ); 
		SetBrushOrgEx( hdcImage , 0 , 0 , NULL );
	    StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    DeleteDC(hdcImage);

    return hr;
}

extern "C" HRESULT DDCopyTextureBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC                 hdcImage;
    HDC                 hdc;
    DDSURFACEDESC       ddsd;
    HRESULT             hr;
	DIBSECTION			dib;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;

    //
    // make sure this surface is restored.
    //
    pdds->Restore();

    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(dib), &dib);    // get size of bitmap
    dx = dx == 0 ? dib.dsBm.bmWidth  : dx;    // use the passed size, unless zero
    dy = dy == 0 ? dib.dsBm.bmHeight : dy;

    //
    // get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    DeleteDC(hdcImage);

    return hr;
}

//
//  DDLoadPalette
//
//  Create a DirectDraw palette object from a bitmap resoure
//
//  if the resource does not exist or NULL is passed create a
//  default 332 palette.
//
extern "C" IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette* ddpal;
    int                 i;
    int                 n;
    int                 fh;
//    HRSRC               h;
//    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        ape[256];
//    RGBQUAD *           prgb;

    //
    // build a 332 palette as the default.
    //
    for (i=0; i<256; i++)
    {
        ape[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE)0;
    }

    //
    // get a pointer to the bitmap resource.
    //
#if 0
    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER)LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD*)((BYTE*)lpbi + lpbi->biSize);

        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            ape[i].peRed   = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue  = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else
#endif
	if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            BYTE r = ape[i].peRed;
            ape[i].peRed  = ape[i].peBlue;
            ape[i].peBlue = r;
		}
    }

    pdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 , ape, &ddpal, NULL);

    return ddpal;
}

/*
 * DDColorMatch
 *
 * convert a RGB color to a pysical color.
 *
 * we do this by leting GDI SetPixel() do the color matching
 * then we lock the memory and see what it got mapped to.
 */
extern "C" DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    //
    //  use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
        SetPixel(hdc, 0, 0, rgb);               // set our value
        pdds->ReleaseDC(hdc);
    }

    //
    // now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;

    if (hres == DD_OK)
    {
        dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
        pdds->Unlock(NULL);
    }

    //
    //  now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }

    return dw;
}

/*
 * DDSetColorKey
 *
 * set a color key for a surface, given a RGB.
 * if you pass CLR_INVALID as the color key, the pixel
 * in the upper-left corner will be used.
 */
extern "C" HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;

    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}


IDirectDrawSurface * DDLoadBitmapTexture(IDirectDraw *pdd, LPCSTR szBitmap, LPDDSURFACEDESC ddsd , int Scale , BOOL bSquareOnly)
{
    HBITMAP             hbm;
    BITMAP              bm;
    IDirectDrawSurface *pdds;
	DDSURFACEDESC ddsd2;
    DWORD dwWidth, dwHeight;
	LPDIRECTDRAWPALETTE ddpal;

	ddsd2 = *ddsd;
	ddsd2.dwFlags |= DDSD_CAPS;
	ddsd2.ddsCaps.dwCaps |=	DDSCAPS_SYSTEMMEMORY;
	int XScale,YScale;

	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

	dwWidth = bm.bmWidth;
	dwHeight = bm.bmHeight;
    //
    // create a DirectDrawSurface for this bitmap
    //
    memcpy(&ddsd2, ddsd, sizeof(DDSURFACEDESC));
    ddsd2.dwSize = sizeof(DDSURFACEDESC);
    ddsd2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;


	
	XScale = Scale;
	YScale = Scale;

	if( bSquareOnly )
	{
		if( dwHeight != dwWidth )
		{
			if( dwHeight > dwWidth )
				YScale++;
			if( dwWidth > dwHeight )
				XScale++;
		}
	}
	
	ddsd2.dwHeight = dwHeight / ( 1 << YScale );
	ddsd2.dwWidth = dwWidth / ( 1 << XScale );
	
	if (pdd->CreateSurface(&ddsd2, &pdds, NULL) != DD_OK)
		return NULL;


	if (bPrimaryPalettized)
	{
		ddpal = DDLoadPalette( pdd ,"data\\pictures\\pal.bmp");
	}else{
		ddpal = DDLoadPalette( pdd , szBitmap );
	}
	pdds->SetPalette( ddpal );

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    DeleteObject(hbm);
    return pdds;
}



/*Build the MipMaps for a MIPMAPDESC structure with the toplevel filled*/
int FindMipMapLod( int Width , int Height , int Count )
{
	Width /= 2;
	Height /= 2;

	if( Width < MIPMAPMIN || Height < MIPMAPMIN || Count >= MAXMIPMAP )
		return Count;


	return FindMipMapLod( Width , Height , Count+1 );
}


IDirectDrawSurface * DDLoadBitmapTextureMipMap(IDirectDraw *pdd, LPCSTR szBitmap, LPDDSURFACEDESC ddsd , int Scale , BOOL bSquareOnly)
{
    HBITMAP             hbm;
    BITMAP              bm;
    IDirectDrawSurface *pdds;
	DDSURFACEDESC ddsd2;
    DWORD dwWidth, dwHeight;
	LPDIRECTDRAWPALETTE ddpal;
	int LodCount;
	int LOD;
	int XScale,YScale;
	DDSCAPS ddscaps;
	HRESULT ddrval;
	LPDIRECTDRAWSURFACE lpDDS_top;    

	ddsd2 = *ddsd;
	ddsd2.dwFlags |= DDSD_CAPS;
	ddsd2.ddsCaps.dwCaps |=	DDSCAPS_SYSTEMMEMORY;
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

	dwWidth = bm.bmWidth;
	dwHeight = bm.bmHeight;
    //
    // create a DirectDrawSurface for this bitmap
    //
    ddsd2 = *ddsd;
    ddsd2.dwSize = sizeof(DDSURFACEDESC);

	XScale = Scale;
	YScale = Scale;

	if( bSquareOnly )
	{
		if( dwHeight != dwWidth )
		{
			if( dwHeight > dwWidth )
				YScale++;
			if( dwWidth > dwHeight )
				XScale++;
		}
	}
	
	ddsd2.dwHeight = dwHeight / ( 1 << YScale );
	ddsd2.dwWidth = dwWidth / ( 1 << XScale );

	ddsd2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
		| DDSD_MIPMAPCOUNT;
	ddsd2.dwMipMapCount= LodCount = FindMipMapLod( ddsd2.dwWidth , ddsd2.dwHeight , 1 );

	ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | DDSCAPS_MIPMAP | DDSCAPS_SYSTEMMEMORY;
	ddsd2.dwHeight = dwHeight;
	ddsd2.dwWidth = dwWidth;

	if (pdd->CreateSurface(&ddsd2, &pdds, NULL) != DD_OK)
		return NULL;

	lpDDS_top = pdds;    

	if (bPrimaryPalettized)
	{
		ddpal = DDLoadPalette( pdd ,"data\\pictures\\pal.bmp");
	}else{
		ddpal = DDLoadPalette( pdd , szBitmap );
	}
	pdds->SetPalette( ddpal );

	ddscaps.dwCaps=DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

	for(LOD=0;LOD<LodCount;LOD++)
	{
#if 0

		memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
		ddsd2.dwSize = sizeof(DDSURFACEDESC);
		ddrval = pdds->Lock( NULL, &ddsd2, 0, NULL);
		if (ddrval != DD_OK) 
		{
			switch( ddrval )
			{
			case DDERR_INVALIDOBJECT:
				pdds->Release();
				return NULL;
			case DDERR_INVALIDPARAMS: 
				pdds->Release();
				return NULL;
			case DDERR_OUTOFMEMORY: 
				pdds->Release();
				return NULL;
			case DDERR_SURFACEBUSY: 
				pdds->Release();
				return NULL;
			case DDERR_SURFACELOST: 
				pdds->Release();
				return NULL;
			case DDERR_WASSTILLDRAWING:
				pdds->Release();
				return NULL;
			}



			pdds->Release();
			return NULL;
		}
#endif

	    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
		/*Get the next mipmap level*/
		ddrval=pdds->GetAttachedSurface( &ddscaps, &pdds);
		
		if(ddrval!=DD_OK && pdds!=NULL)
		{
			pdds->Release();
			return NULL;
		}
	}

    DeleteObject(hbm);
    return lpDDS_top;
}


//
//  DDLoadPalette
//
//  Create a DirectDraw palette object from a bitmap resoure
//
//  if the resource does not exist or NULL is passed create a
//  default 332 palette.
//
extern "C" BOOL HasBmpGotRealBlack( LPCSTR szBitmap)
{
    int                 i;
    int                 n = 0;
    int                 fh;
//    HRSRC               h;
//    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        ape[256];
//    RGBQUAD *           prgb;
	BOOL	RealBlack = FALSE;

    //
    // get a pointer to the bitmap resource.
    //
#if 0
    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER)LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD*)((BYTE*)lpbi + lpbi->biSize);

        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            ape[i].peRed   = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue  = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else
#endif
		if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            BYTE r = ape[i].peRed;
            ape[i].peRed  = ape[i].peBlue;
            ape[i].peBlue = r;
		}
    }
	// Only check the first colour...
	if( n )
		n = 1;
    for(i=0; i<n; i++ )
	{
		if( ape[i].peRed == 0 && ape[i].peGreen == 0 && ape[i].peBlue == 0 )
			return TRUE;
	}
    return FALSE;
}
#endif



