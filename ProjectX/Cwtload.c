#ifdef SOFTWARE_ENABLE
extern long Vmode;							//0==8bit, 1==16bit.

/*

	==============================================================================


	This file sorts out the textures after they have been loaded by TLoad.c.


  Basically it grabs each texture from d3d and adds it to the internal Tpages.
  Also it sets up the internal tables such as width/height, address, ANDflags, etc.
  
  This avoids the dumb principle of locking/unlocking each texture every time
  an access has to be made. For a 1000-tri scene, this can save up to 1000 locks
  and unlocks per frame.
  
  Call this after a D3D Tload.


	==============================================================================
*/




								   

/*

  cw: Addition, now this sorts out the tables for 16-bit mode.
	Its called here when CWSortOutTextures is called because
	usually textures will only be loaded at start of a level
	or when a new level starts.

	It sets up any tables to do with the current mode or the 
	current palette.




*/
extern	void	MakeClut( long n, unsigned char * Pal, long ck );

extern char Palette[];

char	SuperBlack;		// Because you never can tell what color is going to be used.





// Called from void CWSortOutTextures()


long	CWCurrentBpp;
long	CWRAndMask;
long	CWGAndMask;
long	CWBAndMask;
long	CWRLeftShift;
long	CWGLeftShift;
long	CWBLeftShift;

long	NumberOFRBits;
long	NumberOFGBits;
long	NumberOFBBits;
long	CWMaxR, CWMaxG, CWMaxB;
 




extern void	AddtoTpage( char * Texture, int TWidth, int THeight, long Toff, long Bpp)	;


#include "tload.h"		// Forsaken Tload bits.

extern char * Texture_Buffer;
extern TLOADHEADER	Tloadheader;
extern char	AddedOK[32][64];	//1 = try and add this size, cleared to
							//0 on failed add at that size on a tpage.

long CWTextureNumber;

extern long SurfBpp;
				 






/*

	Scan the palette and find the color which is 0,0,0 to be used as see through.
*/




void	Initialise16BitMode()
{
char			charbuf[160];
DDSURFACEDESC	BBddsd;
HRESULT			ddrval;
long Ander, Shifts, b;

// First find out the bit depth (15/16) and store it off into a global.

	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);

	CWCurrentBpp = BBddsd.ddpfPixelFormat.dwRGBBitCount;  // Get bits-per-pixel.

	if (CWCurrentBpp!=16 && CWCurrentBpp!=15) return;


	CWRAndMask = BBddsd.ddpfPixelFormat.dwRBitMask;		  // Get Red bit-mask.
	CWGAndMask = BBddsd.ddpfPixelFormat.dwGBitMask;		  // Get Grn bit-mask.
	CWBAndMask = BBddsd.ddpfPixelFormat.dwBBitMask;		  // Get Blu bit-mask.

// From the bitmasks work out the leftshift for each color.


// Find how many 0 bits there are until a 1 bit.

		Ander = 1;
		Shifts = 0;


		for ( b=0; b!=31; b++ )
		{
			if ( (CWRAndMask & Ander) ) goto Out1;
			Shifts++;
			Ander <<= 1;
	 	}

Out1:

	 //	if (b==31) Msg("Hang on, got through all the bits" );

		CWRLeftShift = Shifts;


		Ander = 1;
		Shifts = 0;


		for ( b=0; b!=31; b++ )
		{
			if ( (CWGAndMask & Ander) ) goto Out2;
			Shifts++;
			Ander <<= 1;
		}

Out2:

	 //	if (b==31) Msg("Hang on, got through all the bits" );

		CWGLeftShift = Shifts;


		Ander = 1;
		Shifts = 0;


		for ( b=0; b!=31; b++ )
		{
			if ( (CWBAndMask & Ander) ) goto Out3;
			Shifts++;
			Ander <<= 1;
		}
Out3:

	  //	if (b==31) Msg("Hang on, got through all the bits" );

		CWBLeftShift = Shifts;


// Work out max for each channel...


		CWMaxR = CWRAndMask >> CWRLeftShift;
		CWMaxG = CWGAndMask >> CWGLeftShift;
		CWMaxB = CWBAndMask >> CWBLeftShift;

					


// Show the surface information.
/*	sprintf(&charbuf[0],"Surface Bits: %d\n andR=%d andG=%d andB=%d\n shiftR=%d,shiftG=%d,shiftB=%d\nMaxR=%d, MaxG=%d, MaxB=%d",
			CWCurrentBpp,
			CWRAndMask, CWGAndMask, CWBAndMask,
			CWRLeftShift, CWGLeftShift, CWBLeftShift,
			CWMaxR, CWMaxG, CWMaxB );

	Msg( &charbuf[0] );

*/

// Here I need to setup any SMC.


}




extern char CkClut[];

  
extern unsigned short Pal16[];

void CWSortOutTextures()
{

	long TCnt;
	HRESULT ddrval;
	DDSURFACEDESC TPddsd;	//CW
	char *Dst;
	long DestWidth,DestHeight,LineWidth;
	long n;

	long i,j;
	char Found;

	DDSURFACEDESC BBddsd;
  //	DDCOLORKEY ddcolorkey;


	long NumColors,x,y;
	unsigned short * ptr16;
	long c;
	unsigned short pixel;

	char charbuf[160];
	long TRAnd, TGAnd, TBAnd, TSurfBpp;
	LPDIRECTDRAWPALETTE SurfPal;

	PALETTEENTRY TempPalette[256];

	long p;
	unsigned char *CWpal;
	long	CK;
	DDCOLORKEY CWCK;


	 
	CWTextureNumber = 0;

	// Clear the Tpage add cache so it doesn't go slowly.
	for (i=0;i!=32;i++)
			for (j=0;j!=64;j++)
					AddedOK[i][j]=1;	// Test y/n cache for tpage adds.


	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);
	SurfBpp    = BBddsd.ddpfPixelFormat.dwRGBBitCount;  // Get bits-per-pixel.



	Initialise16BitMode();

//	if (SurfBpp!=8) return;

// First, we clear out the internal texture RAM ready for Tpage adding..


 	memset (Texture_Buffer,  199 ,32*65536);

	n = 0;	// Texture number.

// Now we loop through all the surfaces, adding each texture.

// Get surface description...

	for (n=0; n!=Tloadheader.num_texture_files;n++)
	{

		memset( &TPddsd,0,sizeof(DDSURFACEDESC)	);

		ddrval = D3DAppIGetSurfDesc(&TPddsd,Tloadheader.lpTextureSurf[n]);

	// Lock surface and get address.
	   	ddrval = Tloadheader.lpTextureSurf[n]->lpVtbl->Lock( Tloadheader.lpTextureSurf[n], NULL, &TPddsd,0,NULL);

		TSurfBpp = TPddsd.ddpfPixelFormat.dwRGBBitCount;  // Get bits-per-pixel.

		TRAnd = TPddsd.ddpfPixelFormat.dwRBitMask;		  // Get Red bit-mask.
		TGAnd = TPddsd.ddpfPixelFormat.dwGBitMask;		  // Get Grn bit-mask.
		TBAnd = TPddsd.ddpfPixelFormat.dwBBitMask;		  // Get Blu bit-mask.

		//sprintf( &charbuf[0],"BPP:%d, rAnd=%d, gAnd=%d, bAnd=%d", TSurfBpp, TRAnd, TGAnd, TBAnd );
		//Msg( &charbuf[0] );




	 
// ==========================================================================================================



	   //	sprintf(&charbuf[0],"THandle is %d",Tloadheader.hTex[n]);
	   //	Msg(&charbuf[0]);

		Dst = TPddsd.lpSurface;
		DestWidth = TPddsd.dwWidth;		// Get width.
		DestHeight = TPddsd.dwHeight;	// Get height.
		LineWidth = TPddsd.lPitch;		// Get pitch.


		



//============================================================================================================

// Create CLUT number N here.....


// Now we know that in 16-bit mode the surfaces are still 8-bit, we can make up the CLUT by using the palette
// associated with the surface.
// We must decompose it into RGB then make the palette up..
// We assume here that the palette is 6:6:6 as usual..

	   	ddrval = Tloadheader.lpTextureSurf[n]->lpVtbl->GetPalette( Tloadheader.lpTextureSurf[n], &SurfPal );
		if (ddrval!=DD_OK)
			Msg("Problem getting the palette from Texture surface.");


// Find out if this palette has colorkey enabled........

/*
typedef struct _DDCOLORKEY{ 
    DWORD dwColorSpaceLowValue; 
    DWORD dwColorSpaceHighValue; 
} DDCOLORKEY,FAR* LPDDCOLORKEY; 
  */





		CK = 0;
		ddrval = Tloadheader.lpTextureSurf[ n ]->lpVtbl->GetColorKey( Tloadheader.lpTextureSurf[n],DDCKEY_SRCBLT , &CWCK );

	   //	if (ddrval != DD_OK ) Msg(" Problem getting colorkey information for surface.");
		

		if (ddrval == DDERR_NOCOLORKEY || ddrval == DDERR_NOCOLORKEYHW ) CK = 0;
		else
		{
			if ( CWCK.dwColorSpaceLowValue == CWCK.dwColorSpaceHighValue ) CK= 1; else CK = 0;
		}







// read 256 palette entries...


//		ddrval = Tloadheader.lpTextureSurf[n]->lpVtbl->GetEntries( SurfPal, 0, 0, 256, &TempPalette[0] );
		ddrval = SurfPal->lpVtbl->GetEntries( SurfPal, 0, 0, 256, &TempPalette[0] );
		if (ddrval!=DD_OK)
			Msg("Problem getting palette ENTRIES from Texture surface.");
		
	
// Now we have the palette, display it.

	  	MakeClut( n, &TempPalette[0], CK );		// Add this clut to the bunch, also makes 32 fades of it too.
 
//============================================================================================================
			

		// Now we scan to find if there are any 0 pixels.
		// If there are we mark it as transparent.


// *************** (( HERE WE DO THE ADD TO INTERNAL TPAGES )) ****************

	  //Msg("Adding to Tpage");


		Dst = TPddsd.lpSurface;



//		sprintf(&charbuf[0],"Adding texture number %d, size: %d x %d", n, DestWidth, DestHeight );
//		Msg( &charbuf[0] );


		AddtoTpage ( Dst, DestWidth, DestHeight, 0, TSurfBpp);
//		AddtoTpage ( Dst, 2, 2, 0);
		CWTextureNumber++;


// Show size of texture.
   //		sprintf(&charbuf[0], "t#%d, Addr=%d, Width=%d, Height=%d, Pitch=%d",n,Dst,DestWidth,DestHeight,LineWidth);
   //		Msg(&charbuf[0]);
	


// Unlock texture surface...
		Tloadheader.lpTextureSurf[n]->lpVtbl->Unlock( Tloadheader.lpTextureSurf[n], NULL );

// Release it....

		// Tloadheader.lpTextureSurf[n]->lpVtbl->Release( Tloadheader.lpTextureSurf[n] );
		RELEASE( Tloadheader.lpTextureSurf[n] );



	}
  	
}

#endif