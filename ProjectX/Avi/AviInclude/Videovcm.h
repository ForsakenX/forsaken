// videoVCM.h : Interface of the CVideoVCM class
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#ifndef _AVIVCM_H_
#define _AVIVCM_H_

#define OLE2ANSI
#include <vfw.h>
#undef OLE2ANSI

#define ALIGNULONG(bytes) ((((bytes) + 3) / 4) * 4)

// For Indeo(R) Video Interactive specific routines
#include "datatype.h"
#include "vfw_spec.h"

// DIB stuff
#include "dib.h"

#define FOURCC_iv41 mmioFOURCC('i','v','4','1')
#define FOURCC_IV41 mmioFOURCC('I','V','4','1')

// Error codes

#define VCMERR_OK		0L
#define VCMERR_INTERNAL		-1L

///////////////////////////////////////////////////////////////////////////////
// CVideoVCM Defininition

class CVideoVCM
{
public:
	CVideoVCM ();
	~CVideoVCM ();

// Operations

   long  vcmOpen (FOURCC fccHandler, BITMAPINFOHEADER * pbiSrc);	
	long	vcmClose ();

	long	vcmBegin (HDC hdc, float frate);
	long	vcmEnd ();

	long	vcmDrawStart (float frate);
	long	vcmDrawStop ();

	long	vcmDraw (unsigned char * pbits, DWORD dwICflags = 0L);
	long	vcmDrawIn (unsigned char *pbits, DWORD dwICflags = 0L);

	inline FOURCC vcmGetFOURCC () { return m_fccHandler; }

// Handles

	HIC		m_hic;
	HDC		m_hdc;
	HDRAWDIB	m_hdd;

	FOURCC		m_fccHandler;

// Information

	void vcmGetSrcRect (RECT * rect);
	void vcmSetSrcRect (RECT * rect);
	void vcmGetDestRect (RECT * rect);
	void vcmSetDestRect (RECT * rect);
	void vcmGetOutRect (RECT * rect);
	void vcmSetOutRect (RECT * rect);
	void vcmGetWndRect (RECT * rect);
   long vcmGetBoundRect(RECT * rect);

	void vcmGetSrcBi (BITMAPINFOHEADER * pbiSrc);
	void vcmSetSrcBi (BITMAPINFOHEADER * pbiSrc);
	void vcmGetDestBi (BITMAPINFOHEADER * pbiDst);
	void vcmSetDestBi (BITMAPINFOHEADER * pbiDst);
	
	long vcmQueryDest (BITMAPINFOHEADER * pbiDst,
	                   int x = 0, int y = 0, int dx = 0, int dy = 0);

// Indeo(R) Video Interactive operations

	void vcmGetBSC (long *bval, long *sval, long *cval, BOOL fDefault); // -255 to +255
	void vcmSetBSC (long bval, long sval, long cval, BOOL fDefault);    // -255 to +255
	void vcmGetScalability (float frate, int *cpu_percentage, BOOL fDefault);
	void vcmSetScalability (float frate, int cpu_percentage, BOOL fDefault);
   BOOL vcmSetZoomMode(int altMode, BOOL fDefault);
   BOOL vcmSetPalette(LOGPALETTE* pLogPal);
   BOOL vcmSetBackground(DIB* pDIB);

	void vcmSetAccessKey (DWORD dwKey);

protected:
	BOOL	m_bBegun;
	// Source, Destination, and Output
	BITMAPINFOHEADER * m_pbiSrc;
	BITMAPINFOHEADER * m_pbiDst;

	long	m_xsrc, m_ysrc, m_dxsrc, m_dysrc;
	long	m_xdst, m_ydst, m_dxdst, m_dydst;
	long	m_xout, m_yout, m_dxout, m_dyout;
	long	m_xwnd, m_ywnd, m_dxwnd, m_dywnd;

	// Indeo(R) Video Interactive specific structures
	R4_DEC_SEQ_DATA   m_r4seqCurrent, m_r4seqDefault;
	R4_DEC_FRAME_DATA m_r4frameCurrent, m_r4frameDefault;

   // bounding rectangles for transparencies   
   R4_RECT m_r4BR;  
   R4_RECT m_r4LastBR;  

   // DIBs for transparency background
   DIB* m_pDIB;
   DIB* m_pBG;
	
	// DIB Section
	HBITMAP m_hDIBSection;
	unsigned char * m_pDIBSection;
public:
	HPALETTE	m_hpalSection;

private:
	
	// Temporary buffer used internally
	int		m_cbuf;
	unsigned char *	m_pbuf;

	HDC		m_hmemDC;
	HBITMAP		m_hbmOld;
	HPALETTE	m_hpalOld_DC, m_hpalOld_MemDC;
};

#endif // _AVIVCM_H_
