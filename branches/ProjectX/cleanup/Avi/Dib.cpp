// DIB.CPP
//
// Portions (almost all) taken from Nigel Thompson's "Animation techniques
// in Win32"
//
// Created:		10/27/95
//

#include <windows.h>
#include "dib.h"

extern "C" void __cdecl
Msg( LPSTR fmt, ... );
extern "C" {
#include "XMem.h"
}


// Create a small DIB here so m_pBMI and m_pBits are always valid.
DIB::DIB()
{
    m_pBMI = NULL;
    m_pBits = NULL;
    m_bMyBits = TRUE;
}

DIB::~DIB()
{
    // Free the memory.
    if (m_pBMI != NULL) 
    {
      free (m_pBMI);
    }

    if (m_bMyBits && (m_pBits != NULL)) 
    {
      free (m_pBits);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Private functions

static BOOL IsWinDIB(BITMAPINFOHEADER* pBIH)
{
    //ASSERT(pBIH);
    if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER)) {
        return FALSE;
    }
    return TRUE;
}

static int NumDIBColorEntries(LPBITMAPINFO pBmpInfo) 
{
    BITMAPINFOHEADER* pBIH;
    BITMAPCOREHEADER* pBCH;
    int iColors, iBitCount;

    pBIH = &(pBmpInfo->bmiHeader);
    pBCH = (BITMAPCOREHEADER*) pBIH;

    // Start off by assuming the color table size from
    // the bit-per-pixel field.
    if (IsWinDIB(pBIH)) {
        iBitCount = pBIH->biBitCount;
    } else {
        iBitCount = pBCH->bcBitCount;
    }

    switch (iBitCount) {
    case 1:
        iColors = 2;
        break;
    case 4:
        iColors = 16;
        break;
    case 8:
        iColors = 256;
        break;
    default:
        iColors = 0;
        break;
    }

    // If this is a Windows DIB, then the color table length
    // is determined by the biClrUsed field if the value in
    // the field is nonzero.
    if (IsWinDIB(pBIH) && (pBIH->biClrUsed != 0)) {
        iColors = pBIH->biClrUsed;
    }

    // BUGFIX 18 Oct 94 NigelT
    // Make sure the value is reasonable since some products
    // will write out more then 256 colors for an 8 bpp DIB!!!
    int iMax = 0;
    switch (iBitCount) {
    case 1:
        iMax = 2;
        break;
    case 4:
        iMax = 16;
        break;
    case 8:
        iMax = 256;
        break;
    default:
        iMax = 0;
        break;
    }
    if (iMax) 
    {
        if (iColors > iMax) 
        {
            iColors = iMax;
        }
    }

    if( iBitCount > 8 )
      iColors = 0;

    return iColors;
}


/////////////////////////////////////////////////////////////////////////////
// DIB commands

// Create a new empty DIB.
BOOL DIB::Create(int iWidth, int iHeight, int iBpp)
{
    // Delete any existing stuff.
    if (m_pBMI != NULL) free (m_pBMI);
    if (m_pBits != NULL) free (m_pBits);

	if (iBpp == 8)
    	m_pBMI = (BITMAPINFO*) malloc((sizeof(BITMAPINFOHEADER)
                                  	+ 256 * sizeof(RGBQUAD)));
	else
    	m_pBMI = (BITMAPINFO*) malloc( sizeof(BITMAPINFOHEADER) );


    if (!m_pBMI) {
        //TRACE("Out of memory for DIB header");
        return FALSE;
    }

    // Allocate memory for the bits (DWORD aligned).
    int iBitsSize = ((iWidth * iBpp / 8 + 5) & ~3) * iHeight;
    m_pBits = (BYTE*)malloc( iBitsSize);
    if (!m_pBits) {
        //TRACE("Out of memory for DIB bits");
        free (m_pBMI);
        m_pBMI = NULL;
        return FALSE;
    }

    // Fill in the header info.
    BITMAPINFOHEADER* pBI = (BITMAPINFOHEADER*) m_pBMI;
    pBI->biSize = sizeof(BITMAPINFOHEADER);
    pBI->biWidth = iWidth;
    pBI->biHeight = iHeight;
    pBI->biPlanes = 1;
    pBI->biBitCount = iBpp;
    pBI->biCompression = BI_RGB;
    pBI->biSizeImage = 0;
    pBI->biXPelsPerMeter = 0;
    pBI->biYPelsPerMeter = 0;
    pBI->biClrUsed = 0;
    pBI->biClrImportant = 0;

   if(iBpp <= 8)
   {
       // Create an arbitrary color table (gray scale).
       RGBQUAD* prgb = GetClrTabAddress();
       for (int i = 0; i < 256; i++) {
           prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = (BYTE) i;
           prgb->rgbReserved = 0;
           prgb++;
       }
   }
    // Set all the bits to a known state (black).
    memset(m_pBits, 0, iBitsSize);

    return TRUE;
}

// Create a CDIB structure from existing header and bits. The DIB
// won't delete the bits and makes a copy of the header.
BOOL DIB::Create(BITMAPINFO* pBMI, BYTE* pBits)
{
    // Delete any existing stuff.
    if (m_pBMI != NULL) 
      free  (m_pBMI);

    // Allocate memory for the header.
	if (pBMI->bmiHeader.biBitCount == 8)
    	m_pBMI = (BITMAPINFO*) malloc( sizeof(BITMAPINFOHEADER)
                                   	+ 256 * sizeof(RGBQUAD) );
	else
    	m_pBMI = (BITMAPINFO*) malloc( sizeof(BITMAPINFOHEADER) );

    if (!m_pBMI) 
    {
        return FALSE;
    }

    memcpy(m_pBMI, pBMI, sizeof(BITMAPINFOHEADER) +
           NumDIBColorEntries(pBMI) * sizeof(RGBQUAD));
   
    if(m_bMyBits && (m_pBits != NULL)) 
      free (m_pBits);

    m_pBits = pBits;
    m_bMyBits = FALSE; // We don't own them
    return TRUE;
}

// Load a DIB from a file.
BOOL DIB::Load
   (
   HWND     hwnd, 
   LPCTSTR  lpszFileName, 
   int      bpp
   )
{
   HANDLE hFile;
   BITMAPINFO* pBmpInfo = NULL;
   BYTE* pBits = NULL;
   BYTE* tempBits = NULL;
   BITMAPFILEHEADER bmfh;
   DWORD iBytes;
   BITMAPINFOHEADER bmih;
   int iColors;
   int iColorTableSize;
   int iBitsSize;
   int iBISize;

   // Open the file.
   if((hFile=CreateFile(lpszFileName, GENERIC_READ,
                        FILE_SHARE_READ, NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_RANDOM_ACCESS,
                        0L)) == INVALID_HANDLE_VALUE) 
   {
      goto $abort;
   }

   // Read the file header to get the file size and to
   // find out where the bits start in the file.

   if (((ReadFile( hFile, &bmfh, sizeof(BITMAPFILEHEADER), &iBytes, NULL) 
      == FALSE)) || (iBytes != sizeof(BITMAPFILEHEADER))) 
   {
      DWORD dwError = GetLastError();
      CloseHandle(hFile);
      goto $abort;
   }

   // Check that we have the magic 'BM' at the start.
   if (bmfh.bfType != 0x4D42) 
   {
      CloseHandle(hFile);
      goto $abort;
   }

   // Make a wild guess that the file is in Windows DIB
   // format and read the BITMAPINFOHEADER. If the file turns
   // out to be a PM DIB file we'll convert it later.

   if (((ReadFile( hFile,
                  &bmih, 
                  sizeof(BITMAPINFOHEADER),
                  &iBytes,
                  NULL)==FALSE)) || (iBytes != (DWORD)sizeof(BITMAPINFOHEADER))) 
   {
      CloseHandle(hFile);
      goto $abort;
   }

   /*
   **  Windows DIB format support only
   */

   if (bmih.biSize != sizeof(BITMAPINFOHEADER)) 
   {
      CloseHandle(hFile);
      //MessageBox(hwnd, "Not a Windows DIB.", "KPlay error", MB_ICONEXCLAMATION	);
	  Msg("Not a Windows DIB");
      goto $abort;
   }

   /* 
   **  8-bit bitmap support only
   */

   if (bmih.biBitCount != 8) 
   {
      CloseHandle(hFile);
      //MessageBox(hwnd, "Not a 256 color bitmap.", "KPlay error", MB_ICONEXCLAMATION	);
	  Msg("Not a 256 colour bitmap");
      goto $abort;
   }


   // Work out how much memory we need for the BITMAPINFO
   // structure, color table and then for the bits.  
   // Allocate the memory blocks.
   // Copy the BmpInfoHdr we have so far,
   // and then read in the color table from the file.

   iColors = NumDIBColorEntries((LPBITMAPINFO) &bmih);
   iColorTableSize = iColors * sizeof(RGBQUAD);
   
   // always allocate enough room for 256 entries
   
   iBISize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
   iBitsSize = bmfh.bfSize - bmfh.bfOffBits;

   // Allocate the memory for the header.
   pBmpInfo = (LPBITMAPINFO) malloc( iBISize);
   if (!pBmpInfo) 
   {
      CloseHandle(hFile);                             // Out of memory for DIB header
      goto $abort;
   }

   memset(pBmpInfo, 0, iBISize);

   // Copy the header we already have.
   memcpy(pBmpInfo, &bmih, sizeof(BITMAPINFOHEADER));

   pBmpInfo->bmiHeader.biBitCount = bpp;

   // Read the color table from the file.
   if(iColors)
   {
      if (((ReadFile( hFile, 
                     (LPSTR)pBmpInfo + sizeof(BITMAPINFOHEADER), 
                     iColorTableSize,
                     &iBytes,
                     NULL )==FALSE)) || (iBytes != (DWORD)iColorTableSize)) 
      {
         CloseHandle(hFile);                          // Failed to read color table
         goto $abort;
      }

      // Allocate the memory for the bits
      // and read the bits from the file.
      tempBits = pBits = (BYTE*) malloc( iBitsSize);
      if (!pBits) 
      {
         CloseHandle(hFile);                          // Out of memory for DIB bits
         goto $abort;
      }
   }

   // read the bits
   if(((ReadFile( hFile, 
                  (LPSTR)pBits, 
                  iBitsSize,
                  &iBytes,
                  NULL )==FALSE)) || (iBytes != (DWORD)iBitsSize)) 
   {
      goto $abort;                                    // Failed to read bits
   }

   /* 
   **  If the BMP file bitdepth is not the same as that requested, 
   **  the bits must be converted.
   */

	if (bpp != bmih.biBitCount)  // bmih.biBitCount == 8 for now
   {
		int x, y;
		BYTE *pOld ;
		RGBTRIPLE *pNew ;
		RGBQUAD *lpRGB ;
      unsigned short* p16New;

		iBitsSize = ( (bmih.biWidth * bpp/8 + 3) & ~3 ) * bmih.biHeight;
		pBits = (BYTE *)malloc( iBitsSize );

      switch(bpp)
      {
         case 24: /* Convert to 24-bit */
      		pOld  = tempBits;
      		pNew  = (RGBTRIPLE *)pBits;
      		lpRGB = (RGBQUAD *)((LPBYTE)pBmpInfo + sizeof( BITMAPINFOHEADER ));
            for (y=0; y<bmih.biHeight; y++ )
            {
      			for (x=0; x<bmih.biWidth; x++ )
      			{
      				(*pNew).rgbtRed   = lpRGB[ *pOld ].rgbRed;
      				(*pNew).rgbtGreen = lpRGB[ *pOld ].rgbGreen;
      				(*pNew).rgbtBlue  = lpRGB[ *pOld ].rgbBlue;
      				pOld++;
      				pNew++;
      			}
      			pOld += ((bmih.biWidth + 3) & ~3) - bmih.biWidth ;
      			pNew = (RGBTRIPLE *)((BYTE *)pNew + 
      						(((bmih.biWidth * bpp/8 + 3) & ~3) - 
      						   bmih.biWidth * bpp/8 ));
      		}
            break;

         case 16: /* Convert to 16-bit (555) */
      		pOld  = tempBits;
      		p16New  = (unsigned short*)pBits;
      		lpRGB = (RGBQUAD *)((LPBYTE)pBmpInfo + sizeof( BITMAPINFOHEADER ));
            for (y=0; y<bmih.biHeight; y++ )
            {
      			for (x=0; x<bmih.biWidth; x++ )
      			{
      				(*p16New) = 0 |
      				            (lpRGB[ *pOld ].rgbRed   >> 3) << 10 |
      				            (lpRGB[ *pOld ].rgbGreen >> 3) << 5  |
      				            (lpRGB[ *pOld ].rgbBlue  >> 3);
      				pOld++;
      				p16New++;
      			}
      			pOld += ((bmih.biWidth + 3) & ~3) - bmih.biWidth ;
      			p16New = (unsigned short*)((BYTE *)p16New + 
      						(((bmih.biWidth * bpp/8 + 3) & ~3) - 
      						   bmih.biWidth * bpp/8 ));
      		}
            break;
         }    

		free (tempBits);		
      bmih.biBitCount = bpp;
	}

  

    // Everything went OK.
    if (m_pBMI != NULL) 
      free (m_pBMI);

    m_pBMI = pBmpInfo; 

    if (m_pBits != NULL) 
      free (m_pBits);

    m_pBits = pBits;
    m_bMyBits = TRUE;
    return TRUE;
                
$abort: // Something went wrong.
    if (pBmpInfo) 
      free (pBmpInfo);

    if (pBits) 
      free (pBits);

    return FALSE;    
}


// Get the number of color table entries.
int DIB::GetNumClrEntries()
{
    return NumDIBColorEntries(m_pBMI);
}

BOOL DIB::MapColorsToPalette(HPALETTE pPal)
{
   if(!pPal)
   {
      return FALSE;
   }

   if(m_pBMI->bmiHeader.biBitCount != 8)
   {
      return FALSE;
   }

   LPRGBQUAD pctThis = GetClrTabAddress();

   BYTE imap[256];
   int iChanged = 0;

   for(int i =0; i<256; i++)
   {
      imap[i] = (BYTE) GetNearestPaletteIndex(pPal, RGB(pctThis->rgbRed,
                                                        pctThis->rgbGreen,
                                                        pctThis->rgbBlue));
      pctThis++;
      if(imap[i] != i) iChanged++;
   }

   // Now map the DIB bits.
   BYTE* pBits = (BYTE*)GetBitsAddress();
   int iSize = StorageWidth() * DibHeight();
   while(iSize--)
   {
      *pBits = imap[*pBits];
      pBits++;
   }

   // Now reset the DIB color table so that its RGB values match
   // those in the palette.
   PALETTEENTRY pe[256];
   GetPaletteEntries(pPal, 0, 256, pe);
   pctThis = GetClrTabAddress();
   for(i=0; i<256; i++)
   {
      pctThis->rgbRed   = pe[i].peRed;
      pctThis->rgbGreen = pe[i].peGreen;
      pctThis->rgbBlue  = pe[i].peBlue;
      pctThis++;
   }

   return TRUE;
}

// Get a pointer to a pixel.
// NOTE: DIB scan lines are DWORD aligned. The scan line 
// storage width may be wider than the scan line image width
// so calc the storage width by rounding the image width 
// to the next highest DWORD value.
void* DIB::GetPixelAddress(int x, int y)
{
    int iWidth;

    // Make sure it's in range and if it isn't return zero.
    if ((x >= DibWidth()) 
    || (y >= DibHeight())) {
        return NULL;
    }

    // Calculate the scan line storage width.
    iWidth = StorageWidth();
    return m_pBits + (DibHeight()-y-1) * iWidth + x * m_pBMI->bmiHeader.biBitCount/8;
}

// Get the bounding rectangle.
void DIB::GetRect(RECT* pRect)
{
    pRect->top = 0;
    pRect->left = 0;
    pRect->bottom = DibHeight();
    pRect->right = DibWidth();
}

// Copy a rectangle of the DIB to another DIB.
void DIB::CopyBits(DIB* pdibDest, 
                   int xd, int yd,
                   int w,  int h,
                   int xs, int ys)
{
	if ( xs < 0 )
	{
		w += xs;
		xd -= xs;
		xs = 0;
	}
	else if ( xs > DibWidth()-w )
	{
		if ( xs < DibWidth() )
			w = DibWidth()-xs;
		else
			return;
	}

	if ( xd < 0 )
	{
		w += xd;
	 	xs -= xd;
		xd = 0;
	}
	else if ( ( w > 0 ) && ( xd > pdibDest->DibWidth()-w ) )
	{
		w = pdibDest->DibWidth()-xd;
	}


	if ( ys < 0 )
	{
		h += ys;
		yd -= ys;
		ys = 0;
	}
	else if ( ys > DibHeight()-h )
	{
		if ( ys < DibHeight() )
			h = DibHeight()-ys;
		else
			return;
	}
	if ( yd < 0 )
	{
		h += yd;
		ys -= yd;
		yd = 0;
	}
	else if ( ( h > 0 ) && ( yd > pdibDest->DibHeight()-h ) )
	{
		h = pdibDest->DibHeight()-yd;
	}

   if (w <= 0 || h <= 0) return;

	h = min( h, DibHeight() - ys) ;
	h = min( h, pdibDest->DibHeight() - yd );

   // Get pointers to the start points in the source and destination
   // DIBs. Note that the start points will be the bottom-left
   // corner of the DIBs because the scan lines are reversed in memory.
   BYTE* pSrc  = (BYTE*)GetPixelAddress(xs, ys + h - 1);
   BYTE* pDest = (BYTE*)pdibDest->GetPixelAddress(xd, yd + h - 1);

   // Get the scan line widths of each DIB.
   int iScanS = StorageWidth();
   int iScanD = pdibDest->StorageWidth();

	w = min( w, DibWidth() - xs );
	w = min( w, pdibDest->DibWidth() - xd );
	if (w <= 0) return;

   // Copy the lines.
   while (h--) 
   {
      memcpy(pDest, pSrc, w * m_pBMI->bmiHeader.biBitCount / 8 );
      pSrc += iScanS;
      pDest += iScanD;
   }
}          

















