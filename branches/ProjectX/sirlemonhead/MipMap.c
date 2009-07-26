
/*===================================================================
		Include File...	
===================================================================*/

#include "typedefs.h"
#include "tload.h"
#include "MipMap.h"
#include "XMem.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

//#define TESTMIPMAP

int	Lod_Red[8]	= {  255, 0,   0,   255,   0, 255, 255,   0 };
int	Lod_Green[8]= {  0,   255, 0,   255, 255,   0, 255,   0 };
int	Lod_Blue[8] = {  0,   0,   255, 0,   255, 255, 255,   0 };

extern	BYTE  GammaTab[256];

/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/
/*
 * LoadSurface
 * Loads a ppm file into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 * Scale...divide X and Y by Scale..
 */

BOOL InitMipMap(LPCSTR lpName , int16 Scale , MIPMAPDESC * MipMapDesc )
{
    DWORD dwWidth, dwHeight;
    int i, j,e;
    FILE *fp;
    CHAR buf[100];
	int XScale,YScale;
	uint8 * uint8pnt;

    /*
     * Find the image file and open it
     */

	fp = fopen( (char*) lpName, "rb" );
    if (fp == NULL) {
		Msg( "Cannot find %s.\n", lpName);
        return FALSE;
    }

    /*
     * Is it a PPM file?
     */

    fgets(buf, sizeof buf, fp);
    if (lstrcmp(buf, "P6\n")) {
        fclose(fp);
		Msg( "%s Is Not a PPM File\n", lpName);
        return FALSE;
    }

    /*
     * Skip any comments
     */

    do {
        fgets(buf, sizeof buf, fp);
    } while (buf[0] == '#');

    /*
     * Read the width and height
     */

    sscanf(buf, "%d %d\n", &dwWidth, &dwHeight);
    fgets(buf, sizeof buf, fp); /* skip next line */

	XScale = Scale;
	YScale = Scale;

	// Some cards Only Like Square Textures...
/* bjd - TODO
	if( d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly )
	{
		if( dwHeight != dwWidth )
		{
			if( dwHeight > dwWidth )
				YScale++;
			if( dwWidth > dwHeight )
				XScale++;
		}
	}
*/
	for( i = 0 ; i < MAXMIPMAP ; i++ )
	{
		MipMapDesc->Data[i] = NULL;
	}
	
	MipMapDesc->Height = dwHeight / ( 1 << YScale );
	MipMapDesc->Width = dwWidth / ( 1 << XScale );


	MipMapDesc->Data[0] = (uint8*) malloc( dwWidth * dwHeight * 3 );

	uint8pnt = MipMapDesc->Data[0];
	if( !uint8pnt )
		return FALSE;
	for (j = 0; j < (int)dwHeight; j++)
	{
		for (i = 0; i < (int)dwWidth; i++)
		{
#ifndef TESTMIPMAP
			*uint8pnt++ = GammaTab[getc(fp)];
			*uint8pnt++ = GammaTab[getc(fp)];
			*uint8pnt++ = GammaTab[getc(fp)];
#else
			*uint8pnt++ = Lod_Red[0];
			*uint8pnt++ = Lod_Green[0];
			*uint8pnt++ = Lod_Blue[0];
			getc(fp);
			getc(fp);
			getc(fp);
#endif
			if( XScale )
			{
				for( e = 0 ; e < ( ( 1 << XScale ) -1 ) ; e++ )
				{
					getc(fp);
					getc(fp);
					getc(fp);
				}
			}
		}
		if( YScale )
		{
			for( e = 0 ; e < ( ( 1 << YScale ) -1 ) * (int) dwWidth ; e++ )
			{
				getc(fp);
				getc(fp);
				getc(fp);
			}
		}

    }
    fclose(fp);
    return TRUE;
}

/*Build the MipMaps for a MIPMAPDESC structure with the toplevel filled*/
int BuildMipMaps( int Width , int Height , MIPMAPDESC * MipMapDesc , int Count )
{
	uint8* Src;
	uint8* Dest;
	int x,y,value,channel;
	Src = MipMapDesc->Data[Count-1];

	Width /= 2;
	Height /= 2;

	if( Width < MIPMAPMIN || Height < MIPMAPMIN || Count >= MAXMIPMAP )
		return Count;

	MipMapDesc->Data[Count] = (uint8*) malloc( Width * Height * 3 );

	Dest = MipMapDesc->Data[Count];

	if( !Dest )
		return Count;

	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
		{
#ifndef TESTMIPMAP
			for(channel=0;channel<3;channel++)
			{
				value = ((int)ColorOf(Src, 2*x, 2*y, channel , Width * 2 )+
						(int)ColorOf(Src, 2*x+1, 2*y, channel, Width * 2)+
						(int)ColorOf(Src, 2*x+1, 2*y+1, channel, Width * 2)+
						(int)ColorOf(Src, 2*x, 2*y+1, channel, Width * 2))/4;
				ColorOf(Dest, x, y, channel, Width )=(unsigned char)value;
			}
#else
		ColorOf(Dest, x, y, 0, Width )=(unsigned char)Lod_Red[Count];
		ColorOf(Dest, x, y, 1, Width )=(unsigned char)Lod_Green[Count];
		ColorOf(Dest, x, y, 2, Width )=(unsigned char)Lod_Blue[Count];
#endif
		}
	}

	return BuildMipMaps( Width , Height , MipMapDesc , Count+1 );
}

#if 0 // bjd - CHECK
/*Load and create a mipmapped texture surface*/
LPDIRECTDRAWSURFACE LoadMipMap(LPDIRECTDRAW lpDD, LPCSTR lpName, 
                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale )
{
	LPDIRECTDRAWSURFACE lpDDS;
	DDSURFACEDESC ddsd, format;
	DWORD dwWidth, dwHeight;
	HRESULT ddrval;
	LPDIRECTDRAWSURFACE lpDDS_top;    
	DDSCAPS ddscaps;
	int LOD;
	MIPMAPDESC MipMapDesc;
	uint8* uint8pnt;

	//load the ppm file into a simple image structure defined in mipmap.h
	if( !InitMipMap( lpName , Scale , &MipMapDesc ) )
		return NULL;
	MipMapDesc.LOD = BuildMipMaps( MipMapDesc.Width , MipMapDesc.Height , &MipMapDesc , 1 );

	dwHeight = (DWORD) MipMapDesc.Height;
	dwWidth = (DWORD) MipMapDesc.Width;

	/*Setup the descriptor to create the surface*/
	memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
	memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
		| DDSD_MIPMAPCOUNT;
	ddsd.dwMipMapCount=MipMapDesc.LOD;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | 
		DDSCAPS_MIPMAP | memoryflag;
	ddsd.dwHeight = dwHeight;
	ddsd.dwWidth = dwWidth;
	
	ddrval = lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDS, NULL);
	if (ddrval != DD_OK)
		return (NULL);
	lpDDS_top=lpDDS;
	
	/*fill the MipMaps by looping through each Level of Detail*/    
	ddscaps.dwCaps=DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
	
	for(LOD=0;LOD<MipMapDesc.LOD;LOD++)
	{
		/*Build a descriptor to lock the new mip map level*/
		memset(&ddsd, 0, sizeof(DDSURFACEDESC));
		ddsd.dwSize = sizeof(DDSURFACEDESC);
		ddrval = lpDDS->lpVtbl->Lock(lpDDS, NULL, &ddsd, 0, NULL);
		if (ddrval != DD_OK) 
		{
			lpDDS->lpVtbl->Release(lpDDS);
			return NULL;
		}
		
		/*Get the new mipmap image dependent on LOD*/

		uint8pnt = MipMapDesc.Data[LOD];
		
#if 0 // bjd - CHECK		
		/*Pack and copy the texture to its surface location*/
		if( !PackAndCopy( ddsd.lpSurface, uint8pnt , lpFormat, ddsd.lPitch , dwWidth , dwHeight ) )
			return NULL;
#endif
	
		lpDDS->lpVtbl->Unlock(lpDDS, NULL);
		
		/*Get the next mipmap level*/
		ddrval=lpDDS->lpVtbl->GetAttachedSurface(lpDDS, &ddscaps, &lpDDS);
		dwHeight /= 2;
		dwWidth /= 2;

		
		if(ddrval!=DD_OK && lpDDS!=NULL)
		{
			lpDDS->lpVtbl->Release(lpDDS);
			return NULL;
		}
	}
	
	/*free the loaded image data and return the surface*/

	for(LOD=0;LOD<MipMapDesc.LOD;LOD++)
	{
		free( MipMapDesc.Data[LOD] );
	}
	return (lpDDS_top);
}
#endif


#if 0 // bjd - CHECK
BOOL PackAndCopy(void *dest, uint8 * uint8pnt, LPDDSURFACEDESC lpFormat, int pitch , DWORD dwWidth , DWORD dwHeight )
{

    DDSURFACEDESC format;
    int i, j;
	int r, g, b, a;
	unsigned long* lpLP;
	unsigned short* lpSP;
	unsigned char* lpCP;
	unsigned long m;
	int s;
	int red_shift, red_scale;
	int green_shift, green_scale;
	int blue_shift, blue_scale;
	int alpha_shift, alpha_scale;

    memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
	/*
	* Determine the red, green and blue masks' shift and scale.
	*/
	for (s = 0, m = format.ddpfPixelFormat.dwRBitMask; !(m & 1);
	s++, m >>= 1);
	red_shift = s;
	red_scale = 255 / (format.ddpfPixelFormat.dwRBitMask >> s);
	for (s = 0, m = format.ddpfPixelFormat.dwGBitMask; !(m & 1);
	s++, m >>= 1);
	green_shift = s;
	green_scale = 255 / (format.ddpfPixelFormat.dwGBitMask >> s);
	for (s = 0, m = format.ddpfPixelFormat.dwBBitMask; !(m & 1);
	s++, m >>= 1);
	blue_shift = s;
	blue_scale = 255 / (format.ddpfPixelFormat.dwBBitMask >> s);
	if( (format.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) != 0 )
	{
		for (s = 0, m = format.ddpfPixelFormat.dwRGBAlphaBitMask ; !(m & 1);
		s++, m >>= 1);
		alpha_shift = s;
		alpha_scale = 255 / (format.ddpfPixelFormat.dwRGBAlphaBitMask >> s);
	}else{
		alpha_shift = 0;
		alpha_scale = 0;
	}
	
	/*
	* Each RGB bit count requires different pointers
	*/
	switch (format.ddpfPixelFormat.dwRGBBitCount) {
	case 32 :
		for (j = 0; j < (int)dwHeight; j++) {
		/*
		* Point to next row in texture surface
			*/
			lpLP = (unsigned long*)(((char*)dest) +
				pitch * j);
			
			
			for (i = 0; i < (int)dwWidth; i++) {
			/*
			* Read each value, scale it and shift it into position
				*/
				r = *uint8pnt++ / red_scale;
				g = *uint8pnt++ / green_scale;
				b = *uint8pnt++ / blue_scale;
				a = 0;
				if( ( r + g + b ) != 0 )
				{
					if( ( r < red_scale ) && ( g < green_scale ) && ( b < blue_scale )	)
					{
						b = blue_scale;
					}
				}
				
				if( alpha_scale != 0 )
				{
					if( r+g+b != 0 ) a = 255 / alpha_scale;
				}
				*lpLP = (r << red_shift) | (g << green_shift) |
					(b << blue_shift) | ( a << alpha_shift );
				lpLP++;
			}
		}
		break;
	case 16 :
		for (j = 0; j < (int)dwHeight; j++) {
			
			lpSP = (unsigned short*)(((char*)dest) +
				pitch * j);
			
			for (i = 0; i < (int)dwWidth; i++) {
				
				r = *uint8pnt++;
				g = *uint8pnt++;
				b = *uint8pnt++;
				if( ( r + g + b ) != 0 )
				{
					if( ( r < red_scale ) && ( g < green_scale ) && ( b < blue_scale )	)
					{
						b = blue_scale;
					}
				}
				r /= red_scale;
				g /= green_scale;
				b /= blue_scale;
				a = 0;
				
				if( alpha_scale != 0 )
				{
					if( r+g+b != 0 ) a = 255 / alpha_scale;
				}
				*lpSP = (r << red_shift) | (g << green_shift) |
					(b << blue_shift) | ( a << alpha_shift );
				lpSP++;
			}
		}
		break;
	case 8:
		for (j = 0; j < (int)dwHeight; j++) {
			lpCP = (unsigned char*)(((char*)dest) +
				pitch * j);
			
			for (i = 0; i < (int)dwWidth; i++) {
				r = *uint8pnt++ / red_scale;
				g = *uint8pnt++ / green_scale;
				b = *uint8pnt++ / blue_scale;
				a = 0;
				if( ( r + g + b ) != 0 )
				{
					if( ( r < red_scale ) && ( g < green_scale ) && ( b < blue_scale )	)
					{
						b = blue_scale;
					}
				}
				if( alpha_scale != 0 )
				{
					if( r+g+b != 0 ) a = 255 / alpha_scale;
				}
				*lpCP = (r << red_shift) | (g << green_shift) | 
					(b << blue_shift) | ( a << alpha_shift );
				lpCP++;
			}
		}
		break;
	default:
		//This wasn't a format I recognize
		return FALSE;
	}
	return TRUE;
}
#endif

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

