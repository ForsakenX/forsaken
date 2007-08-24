// DIB.H
//
// Portions (almost all) taken from Nigel Thompson's "Animation techniques in Win32"
//
// Created:		10/27/95
//

#ifndef _DIB_H_
#define _DIB_H_


class DIB {
public:
	DIB();
	~DIB();

	BITMAPINFO* GetBitmapInfoAddress()
		{return m_pBMI;}
	BYTE* GetBitsAddress()
		{return m_pBits;}
	RGBQUAD* GetClrTabAddress()
        {return (m_pBMI->bmiHeader.biBitCount <= 8) ? (LPRGBQUAD)(((BYTE*)(m_pBMI)) 
            + sizeof(BITMAPINFOHEADER)) : NULL;}       // Pointer to color table
	int GetNumClrEntries();


   BOOL Create(BITMAPINFO* pBMI, BYTE* pBits); // Create from existing mem
	BOOL Create(int width, int height, int iBpp = 8);

   void* GetPixelAddress(int x, int y);
	virtual BOOL Load(HWND hwnd, LPCSTR lpszFileName, int bpp);
   virtual int GetWidth() {return DibWidth();}   // Image width
   virtual int GetHeight() {return DibHeight();} // Image height
	virtual BOOL MapColorsToPalette(HPALETTE hPal);
   virtual void GetRect(RECT* pRect);
   virtual void CopyBits(DIB* pDIB, 
                         int xd, int yd,
                         int w,  int h,
                         int xs, int ys);
   inline int GetBitDepth( void )
      {return m_pBMI->bmiHeader.biBitCount;}


protected:
	BITMAPINFO* m_pBMI;
	BYTE* m_pBits;
   BOOL  m_bMyBits;  // TRUE if DIB owns bits

private:
	int DibWidth()
		{return m_pBMI->bmiHeader.biWidth;}
	int DibHeight()
		{return m_pBMI->bmiHeader.biHeight;}
   int StorageWidth()
      {return (m_pBMI->bmiHeader.biWidth * 
        		 m_pBMI->bmiHeader.biBitCount / 8 + 3) & ~3;}

};

#endif // _DIB_H
