// $Revision: 17 $

/*
 * v i d e o v c m . c p p
 *
 * AVI playback Routines
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 17 $
 *
 * $Header: /PcProjectX/avi/Videovcm.cpp 17    20/02/98 15:29 Philipy $
 *
 * $Log: /PcProjectX/avi/Videovcm.cpp $
//
//17    20/02/98 15:29 Philipy
//re-implented AVI
//splash screens can now play demos and AVIs
//
//15    4/11/97 16:26 Philipy
//AVI now plays for stats screens
//implemented scrolling messages (not enabled)
//
//14    27/10/97 10:39 Philipy
//if'd out AVI display on poly routines
//compile with AVI_UsePolys if needed
//
//13    23/10/97 16:49 Philipy
//added tggle (number key 1) for playing AVI on texture.
//(no texture conversion yet, could appear corrupted)
//
//12    21/10/97 13:13 Philipy
//removed AVI stats for demo
//
//11    17/10/97 15:13 Philipy
//
//10    17/10/97 14:23 Philipy
 */



// videoVCM.cpp : Implementation of the CvideoVCM class
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include <windows.h>
#include "d3dapp.h"

#ifdef _DEBUG
  #include <stdio.h>
  #include <assert.h>
  #define DPF printf
  #define ASSERT assert
#else
  #define DPF / ## /
  #define ASSERT / ## /
#endif
void EndProcess (int);

#include "videoVCM.h"
#include "ddraw.h"
#include "d3dmain.h"
#include "tload.h"
#include "2dtextures.h"

extern D3DAppInfo* d3dapp;         // Pointer to read only collection of DD and D3D
extern "C" LPDIRECTDRAWSURFACE		Lp_AVI_DDSurface;

HRESULT HBitmapToDirectDrawSurface(LPDIRECTDRAWSURFACE lpdds, HBITMAP hbm);
extern "C" {
#include "XMem.h"
}


extern "C" void __cdecl
Msg( LPSTR fmt, ... );
extern "C" void DebugPrintf( const char * format, ... );
extern "C" BOOL MovePPMToVideoMemory( TLOADHEADER *Tloadheader, int16 n, LPDIRECTDRAWSURFACE lpSrcTextureSurf );
extern "C" BOOL BlitTextureToVideoMemory( TLOADHEADER *Tloadheader, int16 n, LPDIRECTDRAWSURFACE lpSrcTextureSurf );
extern "C" BOOL CopyDDSurfaceToTextureSurfaces ( LPDIRECTDRAWSURFACE lpDDS_source, LPDIRECTDRAWSURFACE lpDDS_dest1, LPDIRECTDRAWSURFACE lpDDS_dest2 );

extern "C" AVISTREAMINFO VidInfo;
extern "C" TLOADHEADER	Tloadheader;
extern "C" FRAME_INFO * AVI_Poly_Header;
extern "C" FRAME_INFO * AVI_Poly_Header2;
extern "C" void MovieUpdateStatusBarContents (HWND);
extern "C" BOOL	ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer );
BOOL	AVI_ShowStats = FALSE;

extern "C" int AVI_Letterbox;	// the height of top and bottom borders 


///////////////////////////////////////////////////////////////////////////////
// CVideoVCM

CVideoVCM::CVideoVCM ()
{
   m_bBegun = FALSE;

   m_pbiSrc = NULL;
   m_pbiDst = NULL;

   m_pDIBSection = NULL;
   m_pbuf        = NULL;

   m_pDIB = NULL;
   m_pBG  = NULL;

   m_hic = NULL;
}

CVideoVCM::~CVideoVCM ()
{
	vcmClose ();
}

// Note that between vcmOpen() and vcmClose(), the source information does not
//  change.  If it does, open a new one.

long CVideoVCM::vcmOpen (FOURCC fccHandler, BITMAPINFOHEADER * pbiSrc)
{
	m_bBegun = FALSE;

	if (m_hic)
		vcmClose ();

	// DrawDib for helping output bitmaps
	m_hdd = DrawDibOpen ();

	m_hDIBSection = 0;

   if( fccHandler == 0 )
      m_fccHandler = pbiSrc->biCompression;
   else
   	m_fccHandler = fccHandler;

	if (m_fccHandler == FOURCC_IV41)
		m_fccHandler = FOURCC_iv41;

	// Open codec
	m_hic = ICLocate (ICTYPE_VIDEO, fccHandler, pbiSrc, 0L, ICMODE_DECOMPRESS);
	if (!m_hic) return (AVIERR_NOCOMPRESSOR);

   ICDecompressSetPalette (m_hic, 0);
	
	m_pbiSrc = (BITMAPINFOHEADER *) new char [sizeof (BITMAPINFOHEADER) +
	                                          256 * sizeof (RGBQUAD)];
	m_pbiDst = (BITMAPINFOHEADER *) new char [sizeof (BITMAPINFOHEADER) +
	                                          256 * sizeof (RGBQUAD)];
	ASSERT (m_pbiSrc && m_pbiDst);
	if (!m_pbiSrc || !m_pbiDst)
      EndProcess (ERROR_NOT_ENOUGH_MEMORY);

	memcpy (m_pbiSrc, pbiSrc, sizeof (BITMAPINFOHEADER));

	// Initialize destination bitmap header
	memcpy (m_pbiDst, m_pbiSrc, sizeof (BITMAPINFOHEADER));
	// Default destination bitmap header
	m_pbiDst->biBitCount    = 8;
	m_pbiDst->biCompression = BI_RGB;
	m_pbiDst->biSizeImage   = 0;

	// Default to src, dest, out rects: 1:1 scale, 0 offset	
	m_xsrc  = m_ysrc = 0;
	m_dxsrc = m_pbiSrc->biWidth;
	m_dysrc = m_pbiSrc->biHeight;
	
	m_xdst  = m_ydst = 0;
	m_dxdst = m_pbiSrc->biWidth;
	m_dydst = m_pbiSrc->biHeight;

	m_xout  = m_yout = 0;
	m_dxout = m_pbiSrc->biWidth;
	m_dyout = m_pbiSrc->biHeight;

	m_xwnd  = m_ywnd = 0;
	m_dxwnd = m_pbiSrc->biWidth;
	m_dywnd = m_pbiSrc->biHeight;

	// Indeo(r) Video Interactive related structures
	if (m_fccHandler == FOURCC_iv41)
	{
		// Get default Indeo(r) Video Interactive sequence data
		m_r4seqDefault.dwSize        = sizeof (R4_DEC_SEQ_DATA);
		m_r4seqDefault.dwFourCC      = FOURCC_IV41;
		m_r4seqDefault.dwVersion     = SPECIFIC_INTERFACE_VERSION;
		m_r4seqDefault.mtType        = MT_DECODE_SEQ_DEFAULT;
		m_r4seqDefault.oeEnvironment = OE_32;
		m_r4seqDefault.dwFlags       = DECSEQ_KEY|DECSEQ_SCALABILITY|DECSEQ_FILL_TRANSPARENT|
		                               DECSEQ_VALID;
		ICSendMessage (m_hic, ICM_GETCODECSTATE, (DWORD) &m_r4seqDefault, sizeof (R4_DEC_SEQ_DATA));
		memcpy (&m_r4seqCurrent, &m_r4seqDefault, sizeof (R4_DEC_SEQ_DATA));
		m_r4seqCurrent.mtType        = MT_DECODE_SEQ_VALUE;

		// Get default Indeo(r) Video Interactive frame data
		m_r4frameDefault.dwSize        = sizeof (R4_DEC_FRAME_DATA);
		m_r4frameDefault.dwFourCC      = FOURCC_IV41;
		m_r4frameDefault.dwVersion     = SPECIFIC_INTERFACE_VERSION;
		m_r4frameDefault.mtType        = MT_DECODE_FRAME_DEFAULT;
		m_r4frameDefault.oeEnvironment = OE_32;
		m_r4frameDefault.dwFlags       = DECFRAME_TIME_LIMIT|DECFRAME_DECODE_RECT|DECFRAME_VIEW_RECT|
		                                 DECFRAME_BRIGHTNESS|DECFRAME_CONTRAST|DECFRAME_SATURATION|DECFRAME_VALID;
		ICSendMessage (m_hic, ICM_GETCODECSTATE, (DWORD) &m_r4frameDefault, sizeof (R4_DEC_FRAME_DATA));
		memcpy (&m_r4frameCurrent, &m_r4frameDefault, sizeof (R4_DEC_FRAME_DATA));
		m_r4frameCurrent.mtType        = MT_DECODE_FRAME_VALUE;

      // Supply the address of the bounding rectangle for transparency
      m_r4frameCurrent.prBoundingRect = &m_r4BR;
      m_r4frameCurrent.dwFlags = DECFRAME_BOUNDING_RECT | DECFRAME_VALID;
      ICSendMessage (m_hic, ICM_SETCODECSTATE, (DWORD) &m_r4frameCurrent, 
   	                      sizeof (R4_DEC_FRAME_DATA));
	}
			 
	// Create temporary buffer
	m_cbuf = m_pbiSrc->biHeight * ALIGNULONG (m_pbiSrc->biWidth)
	         * m_pbiSrc->biBitCount/8;
	m_pbuf = (unsigned char *)malloc( m_cbuf);
	if (!m_pbuf)
		EndProcess (ERROR_NOT_ENOUGH_MEMORY);

	return (VCMERR_OK);
}

long CVideoVCM::vcmClose ()
{
	if (m_pbiSrc) delete [] m_pbiSrc;
	if (m_pbiDst) delete [] m_pbiDst;
	m_pbiSrc = m_pbiDst = 0L;

	if (m_pbuf)
	{
		free(m_pbuf);
		m_pbuf = 0L;
	}

	if (m_hdd)
	{
		DrawDibClose (m_hdd);
		m_hdd = 0;
	}
	
	if (m_hic)
	{
		ICClose (m_hic);
		m_hic = 0L;
	}

   if(m_pDIB)
   {
      //delete m_pDIB;
      m_pDIB = NULL;
   }
   
   if(m_pBG)
   {
      delete m_pBG;
      m_pBG = NULL;
   }

   m_hic = NULL;

	m_fccHandler = 0;

	return (VCMERR_OK);
}

// vcmBegin() and vcmEnd() (de)initializes a series of vcmDraw()'s
// The user must end and restart a sequence when the destination
//  parameters change.
// Note that if the source information changes vcmOpen()/vcmClose()
//  must be used (since fcc might be different).
// fInit initializes the sequence (do the first time and when resetting
//  parameters

long CVideoVCM::vcmBegin (HDC hdc, float frate)
{
	long rval;

	if (!m_hic || !hdc)
		return (VCMERR_INTERNAL);

	m_hdc = hdc;

	m_hmemDC = CreateCompatibleDC (m_hdc);

   /*
       If there is a background image present, the destination header
       must refplect its dimensions
    */

   if(m_pBG)
   {
      memcpy(m_pbiDst, m_pBG->GetBitmapInfoAddress(), sizeof(BITMAPINFOHEADER) );

		m_cbuf = m_pbiDst->biHeight * ALIGNULONG (m_pbiDst->biWidth)
		         * m_pbiDst->biBitCount/8;

		m_pbuf = (unsigned char *) realloc (m_pbuf, m_cbuf);
   }


	rval =
	ICDecompressExQuery (m_hic, 0,
         			      m_pbiSrc, NULL, m_xsrc, m_ysrc, m_dxsrc, m_dysrc,
         			      m_pbiDst, NULL, m_xdst, m_ydst, m_dxdst, m_dydst);

	if (rval) 
	   return (rval);

	// Determine whether we can use dib section
	int bpp = GetDeviceCaps (m_hdc, BITSPIXEL);
	
	rval =
	ICDecompressExBegin (m_hic, 0,
          			      m_pbiSrc, NULL, m_xsrc, m_ysrc, m_dxsrc, m_dysrc,
          			      m_pbiDst, NULL, m_xdst, m_ydst, m_dxdst, m_dydst);

	if (rval) 
      return (rval);

   // Initialize palette
	m_hpalSection = m_hpalOld_DC = m_hpalOld_MemDC = 0;
	if (bpp == 8 || m_pbiDst->biBitCount == 8)
	{
		rval = ICDecompressGetPalette (m_hic, m_pbiSrc, m_pbiDst);

		LOGPALETTE * pLogPal = (LOGPALETTE *) new char [sizeof (LOGPALETTE) +
		                       256 * sizeof (PALETTEENTRY)];
		if (!pLogPal)
			EndProcess (ERROR_NOT_ENOUGH_MEMORY);
      
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;

		PALETTEENTRY * ppe   = pLogPal->palPalEntry;
		RGBQUAD * pquad = (RGBQUAD *) (((char *)m_pbiDst) + sizeof (BITMAPINFOHEADER));
		for (int i = 0; i < 256; i++)
		{
			ppe[i].peRed   = pquad[i].rgbRed;
			ppe[i].peGreen = pquad[i].rgbGreen;
			ppe[i].peBlue  = pquad[i].rgbBlue;
			ppe[i].peFlags = 0;
		}

		m_hpalSection = CreatePalette (pLogPal);
		delete [] pLogPal;

		m_hpalOld_DC = SelectPalette (m_hdc, m_hpalSection, FALSE);
		RealizePalette (m_hdc);
		m_hpalOld_MemDC = SelectPalette (m_hmemDC, m_hpalSection, FALSE);
	}

	// Create DIB section large enough to hold the background image, if present

	m_hDIBSection =
	CreateDIBSection ( m_hdc,
	                   (const BITMAPINFO *)m_pbiDst,
			  		       DIB_RGB_COLORS,
			             (void **)&m_pDIBSection,
			             0, 0);

	m_hbmOld = SelectObject (m_hmemDC, m_hDIBSection);

	/*
       Create a 'wrapper' around the decompressed frames of video in order
       to use the DIB functions for copying, etc.
    */

   if(m_pBG)
   {
   	m_pDIB = new DIB(); // Fill in the values later when gbUseDrawDIB is determined
      
   	if(m_pDIB)
   		m_pDIB->Create((BITMAPINFO*)m_pbiDst, (BYTE*)m_pDIBSection);

      m_pBG->MapColorsToPalette(m_hpalSection);

      m_pBG->CopyBits(m_pDIB, 0, 0, m_dxwnd, m_dywnd,
                              0, 0);		
	}	


	SetStretchBltMode (m_hdc, COLORONCOLOR);

	m_bBegun = TRUE;

	return (AVIERR_OK);
}

long CVideoVCM::vcmEnd ()
{
	m_bBegun = FALSE;

	if (!m_hdc)
		return (VCMERR_INTERNAL);

	//long rval = ICDecompressExEnd (m_hic);

   if(m_pDIB)
   {
      delete m_pDIB;
   }

	// Clean up palette
	if (m_hpalOld_DC)
		SelectPalette (m_hdc, m_hpalOld_DC, FALSE);
	if (m_hpalOld_MemDC)
		SelectPalette (m_hmemDC, m_hpalOld_MemDC, FALSE);

	// Clean up DIB section
	if (m_hDIBSection)
	{
		DeleteObject (m_hDIBSection);
		m_hDIBSection = 0;
	}

	if (m_hbmOld)
	{
		SelectObject (m_hmemDC, m_hbmOld);
		DeleteDC (m_hmemDC);

		DeleteObject (m_hDIBSection);
	}
	
	m_hdc = 0;

	return (VCMERR_OK);
}

// vcmDrawStart/vcmDrawStop are not absolutely necessary but some codecs
// use them to do timing (to tell when playing real time)

long CVideoVCM::vcmDrawStart (float frate)
{
	// Send ICM_DRAW_BEGIN.
	// this is only for telling	the codec what our frame rate is - zero out all other members.
	ICDrawBegin (m_hic,
	             0, 0, 0, 0,
	             0, 0, 0, 0,
	             NULL,
	             0, 0, 0, 0,
	             (DWORD) (1.0/frate * 1000.0 * 1000.0), // dwRate
	             (DWORD) (1000*1000));                  // dwScale

	// Send ICM_DRAW_START.
	ICDrawStart (m_hic);

	return (VCMERR_OK);
}

long CVideoVCM::vcmDrawStop ()
{
	// Send ICM_DRAW_STOP
	ICDrawStop (m_hic);

	// Send ICM_DRAW_END
	ICDrawEnd (m_hic);
	
	return (VCMERR_OK);
}

long CVideoVCM::vcmDraw (unsigned char * pbits, DWORD dwICflags)
{
   int xpos, ypos;
   long rval;
   RECT    dest;
   HRESULT hr;
   DDBLTFX fx;
   LPDIRECT3DTEXTURE lpSrcTexture = NULL;
   LPDIRECTDRAWSURFACE lpdds = NULL;
	
	// Make sure we initialized 
	// the play thread might try to play before completely initialized
	if (!m_bBegun)
	{
		return (ICERR_OK);
	}

	unsigned char * pout;

	pout = m_pDIBSection;

   /*
       Clean up the last dirty rectangle if ther is a background image.
   */

	if(m_pBG)
   {
      m_pBG->CopyBits(m_pDIB,  (m_r4LastBR.dwX  ),          // Destination
                               (m_r4LastBR.dwY  ),
                               (m_r4LastBR.dwWidth ), 
                               (m_r4LastBR.dwHeight),
                               (m_r4LastBR.dwX  ),          // Source
                               (m_r4LastBR.dwY ));
   }

   // The GUI thread might change the origin while playing. Keep a copy for
   // calculating the bounding rectangle later
   
   xpos = m_xdst;
   ypos = m_ydst;

	rval = ICDecompressEx( m_hic,
                          dwICflags,
 	                       m_pbiSrc, pbits, m_xsrc, m_ysrc, m_dxsrc, m_dysrc,
 	                       m_pbiDst, pout,    xpos, m_dywnd - ypos - (m_dyout/m_dydst)*m_dydst, m_dxdst, m_dydst);

	if (rval)
	{
		// Normal in case of ICM_HURRYUP flag (rval = 1)
		return (rval);
	}

   // adjust the rect to window coordinates

   if(m_pBG)
   {
      memcpy(&m_r4LastBR, &m_r4BR, sizeof(R4_RECT) );

      m_r4LastBR.dwX += xpos;
      m_r4LastBR.dwY += ypos;
   }

   // Draw the frame. Should only draw the portion of the window that has
   // changed from the last update. Draws entire frame now.

	if (m_dxdst == m_dxout && m_dydst == m_dyout)
	{
		// get DIB handle
		m_hDIBSection = SelectObject(m_hmemDC,  m_hbmOld);

		// copy to DD surface
		hr = HBitmapToDirectDrawSurface(Lp_AVI_DDSurface, m_hDIBSection);
		if (hr != DD_OK)
			DebugPrintf("Videovcm.cpp: CVideoVCM::vcmDraw() error copying bitmap to DD surface\n");

		memset(&fx, 0, sizeof(DDBLTFX));
		fx.dwSize = sizeof(DDBLTFX);
		dest.top = AVI_Letterbox;
		dest.left = 0;
		dest.right = d3dapp->ThisMode.w;
		dest.bottom = d3dapp->ThisMode.h - AVI_Letterbox;

		//hr = d3dapp->lpBackBuffer->Blt( &dest, Lp_AVI_DDSurface, NULL, DDBLT_WAIT , &fx );
		hr = d3dapp->lpFrontBuffer->Blt( &dest, Lp_AVI_DDSurface, NULL, DDBLT_WAIT , &fx );
		if( hr != DD_OK )
		{
			DebugPrintf("Videovcm.cpp: vcmDraw() AVI Direct Draw Blit error (%s)\n", D3DAppErrorToString(hr));
			return(hr);
		} 

		if (AVI_ShowStats)
			MovieUpdateStatusBarContents (myglobs.hWndMain);

//		D3DAppShowBackBuffer(D3DAPP_SHOWALL);

			//DebugPrintf("Blit here HBITMAP address %x\n", &m_hDIBSection);
			m_hbmOld = SelectObject(m_hmemDC, m_hDIBSection);


			//rval = BitBlt (m_hdc,    m_xwnd, m_ywnd, m_dxwnd, m_dywnd,        // Destination
			//	            m_hmemDC, 0, 0,                                    // Source
			//	            SRCCOPY);


			if (rval == FALSE)
			{
				rval = GetLastError ();
				return (rval);
			}

			GdiFlush (); // CreateDIBSection() batches frames
		             // Must flush before we overwrite
	}   
	else
	{
		rval =
		DrawDibDraw( m_hdd,   m_hdc, m_xout, m_yout, m_dxout, m_dyout,
		             m_pbiDst, pout, m_xdst, m_ydst, m_dxdst, m_dydst,
			          0L);
		if (rval == FALSE)
		{
			return (rval);
		}
	}
	
	return (VCMERR_OK);
}

long CVideoVCM::vcmDrawIn (unsigned char * pbits, DWORD dwICflags)
{
	// Make sure we initialized 
	// the play thread might try to play before completely initialized
	if (!m_bBegun)
	{
		return (ICERR_OK);
	}

   // If we are not displaying frames, IVI still writes to the buffer
	unsigned char * pout;

	pout = m_pbuf;

	long rval =
	ICDecompressEx( m_hic, dwICflags,
            		 m_pbiSrc, pbits, 0, 0, m_dxsrc, m_dysrc,
            		 m_pbiDst, pout,  0, 0, m_dxsrc, m_dysrc  );

	if (rval)
	{
		// Normal in case of ICM_HURRYUP flag (rval = 1)
		return (rval);
	}

	return (VCMERR_OK);
}

void CVideoVCM::vcmGetSrcRect (RECT * rect)
{
	rect->left   = m_xsrc;
	rect->top    = m_ysrc;
	rect->right  = m_xsrc+m_dxsrc;
	rect->bottom = m_ysrc+m_dysrc;
}

void CVideoVCM::vcmSetSrcRect (RECT * rect)
{
	m_xsrc = rect->left;
	m_ysrc = rect->top;
	m_dxsrc = rect->right - rect->left;
	m_dysrc = rect->bottom - rect->top;
}

//  DestRect describes the position of the decompressed frame of video within
//  the output buffer. If the dimensions of DestRect differ from those of
//  SrcRect, the decompressor will stretch the frame.

void CVideoVCM::vcmGetDestRect (RECT * rect)
{
	rect->left   = m_xdst;
	rect->top    = m_ydst;
	rect->right  = m_xdst+m_dxdst;
	rect->bottom = m_ydst+m_dydst;
}

void CVideoVCM::vcmSetDestRect (RECT * rect)
{
	m_xdst = rect->left;
	m_ydst = rect->top;
	m_dxdst = rect->right - rect->left;
	m_dydst = rect->bottom - rect->top;
}

// The OutRect describes where the decompressed frame should be placed in the
// display surface. It may also describe a stretch value if the dimensions of 
// OutRect differ from DstRect.

void CVideoVCM::vcmGetOutRect (RECT * rect)
{
      rect->left   = m_xout;
      rect->top    = m_yout;
      rect->right  = m_xout + m_dxout;
      rect->bottom = m_yout + m_dyout;
}

void CVideoVCM::vcmSetOutRect (RECT * rect)
{
	m_xout = rect->left;
	m_yout = rect->top;
	m_dxout = rect->right - rect->left;
	m_dyout = rect->bottom - rect->top;

   if(!m_pBG)
   {
      m_xwnd = m_xout;
      m_ywnd = m_yout;
      m_dxwnd = m_dxout;
      m_dywnd = m_dyout;
   }
}

//  WndRect describes the dimensions of the current frame, including the
//  backgound image, if present.

void CVideoVCM::vcmGetWndRect (RECT * rect)
{
      rect->left   = m_xwnd;
      rect->top    = m_ywnd;
      rect->right  = m_xwnd + m_dxwnd;
      rect->bottom = m_ywnd + m_dywnd;
}

 
/*--------------------------------------------------------------------------
  Get the bounding rectangle for the previously decoded frame. Indeo video 
  interactive will report the bounding rectangle of the portion of the frame 
  it has decoded. If the frame has not been encoded for transparency, this 
  rectangle will include the entire frame. Returns error if not IVI.
--------------------------------------------------------------------------*/

long CVideoVCM::vcmGetBoundRect(RECT * rect)
{
   if(m_fccHandler != FOURCC_iv41)
      return VCMERR_INTERNAL;

   rect->left   = m_r4LastBR.dwX;
   rect->top    = m_r4LastBR.dwY;
   rect->right  = rect->left + m_r4LastBR.dwWidth;
   rect->bottom = rect->top  + m_r4LastBR.dwHeight;

   return 0;
}


void CVideoVCM::vcmGetSrcBi (BITMAPINFOHEADER * pbiSrc)
{
	memcpy (pbiSrc, m_pbiSrc, sizeof (BITMAPINFOHEADER));
}

void CVideoVCM::vcmSetSrcBi (BITMAPINFOHEADER * pbiSrc)
{
	memcpy (m_pbiSrc, pbiSrc, sizeof (BITMAPINFOHEADER));
}

void CVideoVCM::vcmGetDestBi (BITMAPINFOHEADER * pbiDst)
{
	memcpy (pbiDst, m_pbiDst, sizeof (BITMAPINFOHEADER));
}

void CVideoVCM::vcmSetDestBi (BITMAPINFOHEADER * pbiDst)
{
   if(!m_pBG)
   {
   	memcpy (m_pbiDst, pbiDst, sizeof (BITMAPINFOHEADER));

   	// Reallocate temporary memory buffer if too small
   	if (pbiDst->biSizeImage > (DWORD) m_cbuf)
   	{
   		m_cbuf = pbiDst->biHeight * ALIGNULONG (m_pbiDst->biWidth)
   		         * m_pbiDst->biBitCount/8;
   		m_pbuf = (unsigned char *) realloc (m_pbuf, m_cbuf);
   	}
   }
}

long CVideoVCM::vcmQueryDest (BITMAPINFOHEADER * pbi,
                              int x , int y ,
                              int dx , int dy )
{
   BITMAPINFOHEADER* pbmih;

	if (x == 0) x = m_xdst;
	if (y == 0) y = m_ydst;
	if (dx == 0) dx = m_dxdst;
	if (dy == 0) dy = m_dydst;
   if (pbi == NULL)
      pbmih = m_pbiDst;
   else
      pbmih = pbi;

   if( (x < 0) || 
       (y < 0) ||
       (x + dx > pbmih->biWidth) ||
       (y + dy > pbmih->biHeight) )
      return -1;
	
	long rval =
	ICDecompressExQuery( m_hic, 0,
			               m_pbiSrc, NULL, m_xsrc, m_ysrc, m_dxsrc, m_dysrc,
			               pbmih,    NULL, x, y, dx, dy);
	if (rval)
		return (rval);



   // The image must be decompressed to a size that will fit into the 
   // background image, if present.

   if(m_pBG)
   {
      if( ( m_dxwnd < pbmih->biWidth  ) ||
          ( m_dywnd < pbmih->biHeight ) )
         return -1;          
   }

	return (ICERR_OK);
}

///////////////////////////////////////////////////////////////////////////////
//
// vcmSetPalette(LOGPALETTE* pLogPal)
//
// PURPOSE:  This function changes sets the palette 
//
// INPUT:    pLogPal: Address of the logical palette containing the new entries. 
//                    If pLogPal is NULL, the default palette is asserted
//
// OUTPUT:   returns TRUE if successfull, FALSE if not.
//
// COMMENTS: The palette can be changed at any time during playback, but will
//           only be asserted at ICM_DECOMPRESSEX_BEGIN. Within the context of
//           KPLAY, this is accomplished after this function has returned, in 
//           the vcmBegin() function of this module. The order of messages that 
//           are being sent to the codec is as follows:
//
//                1. ICM_DECOMPRESSEX_END
//                2. ICM_DECOMPRESS_SET_PALETTE
//                3. ICM_DECOMPRESSEX_BEGIN
//           
//           To change the palette, the ICDecompressSetPalette function is
//           used, with the second parameter pointing to a BITMAPINFO struct
//           which contains the palette information. If this parameter is NULL
//           the codec will switch to its default palette.

BOOL CVideoVCM::vcmSetPalette(LOGPALETTE* pLogPal)
{

   long rval;
   if(pLogPal == NULL)  // Use the default palette
   {
      rval = ICDecompressSetPalette (m_hic, 0);
   }
   else                 // Use the supplied palette
   {
      // Copy the new palette info to the bitmap
   	PALETTEENTRY* ppe = pLogPal->palPalEntry;
   	RGBQUAD* pquad = (RGBQUAD *) (((char *)m_pbiDst) + sizeof (BITMAPINFOHEADER));

      // For active palette we just use the first 236 entries of the given palette
      int iNumEntries = (pLogPal->palNumEntries > 236) ? 236 : pLogPal->palNumEntries;
   	for (int i=0; i < iNumEntries; i++)
   	{
   		pquad[i+10].rgbRed   = ppe[i].peRed;
   		pquad[i+10].rgbGreen = ppe[i].peGreen;
   		pquad[i+10].rgbBlue  = ppe[i].peBlue;
      }

      // IVI expects a full 256 color palette to work with, including the 
      // system colors.
      
      // Set the palette used by the codec
      m_pbiDst->biClrUsed = 256;
      rval = ICDecompressSetPalette (m_hic, m_pbiDst); // use the new palette

      if(rval)
         ICDecompressSetPalette (m_hic, 0);

   }

   return (rval == 0);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Indeo(r) Video Interactive Specific Routines
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CVideoVCM::vcmSetBackground(DIB* pDIB)
{
	if (m_fccHandler != FOURCC_iv41)
		return FALSE;

   if( pDIB->GetWidth() < m_dxout || 
       pDIB->GetHeight() < m_dyout  )
      return FALSE;

   // Reset
   if(m_pBG) delete m_pBG;

	if(!pDIB)
      return FALSE;

	m_pBG  = pDIB;

   m_xwnd  = m_ywnd = 0;
   m_dxwnd = m_pBG->GetWidth();
   m_dywnd = m_pBG->GetHeight();

   // center the image in the background
   m_xdst = m_dxwnd/2 - m_dxdst/2;
   m_ydst = m_dywnd/2 - m_dydst/2;

   // Do not draw transparent pixels
   m_r4seqCurrent.fFillTransparentPixels = 0;
   m_r4seqCurrent.dwFlags = DECSEQ_FILL_TRANSPARENT | DECSEQ_VALID;
   ICSendMessage (m_hic, ICM_SETCODECSTATE, (DWORD) &m_r4seqCurrent, 
	                      sizeof (R4_DEC_SEQ_DATA));


   return TRUE;
}




///////////////////////////////////////////////////////////////////////////////
//
// vcmSetZoomMode(int altMode, BOOL fDefault)
//
// PURPOSE:  This function changes the zoom by 2 mode for the Indeo video 
//           interactive codec. Specifically, it toggles between the pixel
//           doubling and the alternate line zoom by 2 modes.
//
// INPUT:    altMode = 1 - pixel doubling mode
//                     2 - alternate line mode
//
//           fDefault = (not used)
//
// OUTPUT:   TRUE is successful, FALSE if not
//
// COMMENTS: support for build 70, build 62 no longer supported
//

BOOL CVideoVCM::vcmSetZoomMode(int altMode, BOOL fDefault)
{

	long rval;

	if (m_fccHandler != FOURCC_iv41)
		return FALSE;

   // Set the appropriate flag
   m_r4seqCurrent.dwFlags = DECSEQ_VALID | DECSEQ_ALT_LINE;
	m_r4seqCurrent.mtType  = MT_DECODE_SEQ_VALUE;

   // Set/Unset the DECSEQ_ALT_LINE flag
   if(altMode == 1)
      m_r4seqCurrent.fAltLine = FALSE;
   else
      m_r4seqCurrent.fAltLine = TRUE;

   // Reset the current codec state
	rval = ICSendMessage (m_hic, ICM_SETCODECSTATE,
	                      (DWORD)&m_r4seqCurrent, 
	                      (DWORD)m_r4seqCurrent.dwSize);

   return (rval == ICERR_OK);


}



void CVideoVCM::vcmGetBSC (long *bval, long *sval, long *cval, BOOL fDefault)
{
	long rval;

	if (m_fccHandler != FOURCC_iv41)
		return;

	m_r4frameCurrent.dwFlags = DECFRAME_BRIGHTNESS | DECFRAME_SATURATION | DECFRAME_CONTRAST |
	                           DECFRAME_VALID;

	if (fDefault)
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_DEFAULT;
		rval = ICSendMessage (m_hic, ICM_GETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}
	else
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_VALUE;
		rval = ICSendMessage (m_hic, ICM_GETCODECSTATE,
	                (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}

	*bval = m_r4frameCurrent.lBrightness;
	*sval = m_r4frameCurrent.lSaturation;
	*cval = m_r4frameCurrent.lContrast;
}

void CVideoVCM::vcmSetBSC (long bval, long sval, long cval, BOOL fDefault)
{
	long rval;
	
	if (m_fccHandler != FOURCC_iv41)
		return;

	if (bval < -255 || bval > 255 ||
	    sval < -255 || sval > 255 ||
	    cval < -255 || cval > 255)
		return;

	m_r4frameCurrent.dwFlags = DECFRAME_BRIGHTNESS | DECFRAME_SATURATION | DECFRAME_CONTRAST |
	                           DECFRAME_VALID;

	if (fDefault)
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_DEFAULT;
		rval = ICSendMessage (m_hic, ICM_SETCODECSTATE,
	                (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}
	else
	{
		m_r4frameCurrent.lBrightness = bval;
		m_r4frameCurrent.lSaturation = sval;
		m_r4frameCurrent.lContrast   = cval;
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_VALUE;
		
		rval = ICSendMessage (m_hic, ICM_SETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, sizeof(R4_DEC_FRAME_DATA));
		
	
	}
}

void CVideoVCM::vcmGetScalability (float frate, int * ms, BOOL fDefault)
{
	long rval;

	if (m_fccHandler != FOURCC_iv41)
		return;

	m_r4frameCurrent.dwFlags = DECFRAME_TIME_LIMIT | DECFRAME_VALID;

	if (fDefault)
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_DEFAULT;
		rval = ICSendMessage (m_hic, ICM_GETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}
	else
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_VALUE;
		rval = ICSendMessage (m_hic, ICM_GETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}

	DWORD dwTimeLimit = m_r4frameCurrent.dwTimeLimit;
	*ms = int (dwTimeLimit);
}

void CVideoVCM::vcmSetScalability (float frate, int ms, BOOL fDefault)
{
	long rval;

	if (m_fccHandler != FOURCC_iv41)
		return;

	DWORD dwTimeLimit = DWORD (ms);
	
	m_r4frameCurrent.dwFlags = DECFRAME_TIME_LIMIT | DECFRAME_VALID;

	if (fDefault)
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_DEFAULT;
		rval = ICSendMessage (m_hic, ICM_SETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}
	else
	{
		m_r4frameCurrent.mtType = MT_DECODE_FRAME_VALUE;
		m_r4frameCurrent.dwTimeLimit = dwTimeLimit;
		rval = ICSendMessage (m_hic, ICM_SETCODECSTATE,
		        (DWORD)&m_r4frameCurrent, (DWORD)m_r4frameCurrent.dwSize);
	}
}

void CVideoVCM::vcmSetAccessKey (DWORD dwKey)
{
	if (m_fccHandler != FOURCC_iv41)
		return;

	m_r4seqCurrent.mtType = MT_DECODE_SEQ_VALUE;
	m_r4seqCurrent.dwFlags = DECSEQ_KEY | DECSEQ_VALID;

	long rval =
	ICSendMessage (m_hic, ICM_GETCODECSTATE,
	               (DWORD)&m_r4seqCurrent, (DWORD)m_r4seqCurrent.dwSize);

	m_r4seqCurrent.fEnabledKey = 1;
	m_r4seqCurrent.dwKey = dwKey;

	rval =
	ICSendMessage (m_hic, ICM_SETCODECSTATE,
	               (DWORD)&m_r4seqCurrent, (DWORD)m_r4seqCurrent.dwSize);
}

