			    
/*==========================================================================
 *
 *  File: Tload.c
 *	loads all the textures necessary for a level..
 *  rescaling if nescessary...
***************************************************************************/

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Include File...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#include "typedefs.h"
#include "tload.h"
#include "mipmap.h"
#include "ddutil.h"
#include "xmem.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif


#define BLANK_RED		(255)
#define BLANK_GREEN		(255)
#define BLANK_BLUE		(255)

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Externals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

extern	BOOL	TexturesEnabled;

extern	PALETTEENTRY ppe[256];
extern	BOOL	PowerVR;
extern	BOOL	Is3Dfx;
extern	BOOL	Is3Dfx2;
extern	void DebugPrintf( const char * format, ... );
BOOL FreeTextureMemory( int * TMem);

extern void __cdecl D3DAppISetErrorString( LPSTR fmt, ... );
extern char* D3DAppErrorToString(HRESULT error);

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Globals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#define	MAXSCALE 3
TLOADNAME	TloadNames[MAXTPAGESPERTLOAD];

TLOADHEADER	Tloadheader;
BOOL	Pal332 = FALSE;
int		TextureMemory = 0;
BOOL	MipMap = TRUE;
BOOL	DontColourKey = FALSE;
BOOL	NoTextureScaling = FALSE;
int Print4x5Text( char * Text , int x , int y , int col );
double	Gamma = 1.0;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Build a Gamma Correction table 
	Input		:		double GammaValue
	Output		:		Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BYTE  GammaTab[256];
void BuildGammaTab( double GammaValue )
{
	double k;
	int i;

	// recover in release build
	if (GammaValue <= 0)
	    GammaValue = 1.0;
	
	k = 255.0/pow(255.0, 1.0/GammaValue);
	
	for (i = 0; i <= 255; i++)
	{
	    GammaTab[i] = (BYTE)(k*(pow((double)i, 1.0/GammaValue)));
		if( i )
		{
			if( !GammaTab[i] )
				GammaTab[i] = 1;
		}

	}
};

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Init a Tloadheader
	Input		:		TLOADHEADER *
	Output		:		BOOL FALSE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL InitTload( TLOADHEADER * Tloadheader  )
{
	int i;

	Tloadheader->num_texture_files = 0;
	for( i = 0 ; i < MAXTPAGESPERTLOAD ; i++ )
	{
		Tloadheader->lpTextureSurf[i] = NULL; // surfaces
		Tloadheader->lpTexture[i]     = NULL; // texture
		Tloadheader->hTex[i]          = 0;	  // texture handle
		Tloadheader->lpMat[i]         = NULL; // material
		Tloadheader->hMat[i]          = 0;	  // material handle
		Tloadheader->CurScale[i]      = 0;	  // handle
		Tloadheader->Scale[i]		  = FALSE;// Should it scale??

		Tloadheader->MipMap[i]		  = FALSE;// Should it have Mip Maps
		Tloadheader->PlaceHolder[i]	  = FALSE;// Is it a placeholder ( for subsequent dynamicly loaded textures )
		Tloadheader->LOD[i]			  = FALSE;// How many Levels of Mip Map

		Tloadheader->Xsize[ i ] = 0;		
		Tloadheader->Ysize[ i ] = 0;
		
		Tloadheader->PlaceHolderFile[ i ] = NULL;
	}
	return	TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Load All textures associated with a level
	Input		:		TLOADHEADER *
	Output		:		BOOL FALSE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL Tload( TLOADHEADER * Tloadheader  )
{
	DWORD	Estimate;
	int		i,e;
	int16	bpp;
	int16	temp;
	uint16	Xsize,Ysize;
	int LeastScaledThatCanbe;
	int LeastScaledThatCanbeScale;

	BuildGammaTab( Gamma );

	// work out the current Bytes per pixel...min of 8...
	if( d3dappi.ThisTextureFormat.bPalettized != 0 )
	{
		temp = d3dappi.ThisTextureFormat.IndexBPP;
	}else{
		temp = d3dappi.ThisTextureFormat.RedBPP + d3dappi.ThisTextureFormat.GreenBPP + d3dappi.ThisTextureFormat.BlueBPP;
	}
	bpp = 0;
	while( temp > 0 )
	{
		bpp++;
		temp -= 8;
	}
 
	// Tloadheader is not valid until everything has been done..
	Tloadheader->state = FALSE;

	if( d3dappi.Driver[d3dappi.CurrDriver].bIsHardware )
	{
		// Find out how much video memory there is before...

		Tloadheader->VidMemBefore = 0;

		if( !FreeTextureMemory( &Tloadheader->VidMemBefore ) )
		{
			Tloadheader->VidMemBefore = D3DAppFreeVideoMemory();
		}
		if ( TextureMemory )
		{
			Tloadheader->VidMemBefore = TextureMemory * 1024;
		}
	}


	// allocate space for placeholder file names
	for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
	{
		if ( Tloadheader->PlaceHolder[ i ] )
		{
			Tloadheader->PlaceHolderFile[ i ] = ( char * )malloc( sizeof( char ) * 256 );
//			DebugPrintf( "Tload: Placeholder ( texture %2d ) xsize %d ysize %d\n", i, Tloadheader->Xsize[ i ], Tloadheader->Ysize[ i ] );
		}
	}
	

	if( d3dappi.Driver[d3dappi.CurrDriver].bIsHardware != 0 && d3dappi.Driver[d3dappi.CurrDriver].bDoesTextures != 0)
	{
		// store the current Bytes per pixel...min of 8...
		Tloadheader->CurrentBPP = bpp;
		
		
		// get the stats for the Currently files..
		for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
		{
			Tloadheader->CurScale[i] = 0;

			if ( !Tloadheader->PlaceHolder[ i ] )
			{
//				DebugPrintf( "Tload: texture file %2d = %s\n", i, Tloadheader->ImageFile[ i ] );
				if (TloadGetStats( Tloadheader , i , (char*) &Tloadheader->ImageFile[i] ,
								&Tloadheader->Xsize[i] , &Tloadheader->Ysize[i] ) != TRUE)
				{
					Msg( "TLoad() Failed on %s\n", Tloadheader->ImageFile[i] );
					return FALSE;
				}
			}
		}
		
		for( e = 0 ; e < Tloadheader->num_texture_files*MAXSCALE ; e ++ )
		{
			// work out how much video memory they need....
			Estimate = 65536;
			for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
			{
				Xsize = Tloadheader->Xsize[i] / ( 1 << Tloadheader->CurScale[i] );
				Ysize = Tloadheader->Ysize[i] / ( 1 << Tloadheader->CurScale[i] );
				// Only Square Textures for PowerVR
				if( d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly )
				{
					if( Xsize != Ysize )
					{
						if( Xsize > Ysize )
							Xsize = Ysize;
						if( Ysize > Xsize )
							Ysize = Xsize;
					}
				}
				if( MipMap && Tloadheader->MipMap[i] )
				{
					Xsize = (int) (Xsize * 1.4F);
					Ysize = (int) (Ysize * 1.4F);
				}

				Tloadheader->SizeInVidMem[i] = Xsize * Ysize * Tloadheader->CurrentBPP;
				Estimate += Tloadheader->SizeInVidMem[i];
			}
		
			Tloadheader->VidMemEstimate = Estimate;
			// if it fits then break out and load them....
			// Take this out if you want to show off the Riva 128...
			if(	(Tloadheader->VidMemEstimate <= Tloadheader->VidMemBefore) || NoTextureScaling )
				break;
			
			LeastScaledThatCanbe = -1;
			LeastScaledThatCanbeScale = MAXSCALE;

			for( i = Tloadheader->num_texture_files-1 ; i >= 0 ; i-- )
			{
				if( Tloadheader->Scale[i] && ( Tloadheader->CurScale[i] < LeastScaledThatCanbeScale ) )
				{
					LeastScaledThatCanbeScale = Tloadheader->CurScale[i];
					LeastScaledThatCanbe = i;
				}
			}

			if( LeastScaledThatCanbe != -1 )
			{
				Tloadheader->CurScale[LeastScaledThatCanbe]++;
			}else{
				// couldnt find any more to scale....
				break;
			}
		}
	}

	if ( Tloadheader->num_texture_files !=0)
	{
		/*	load in and convert all textures */
		if ( TloadAllTextures( Tloadheader ) != TRUE)
		{
			Msg( "TLoadAllTextures() Failed\n" );
			return FALSE;
		}
		// if there are any textures then create materials for them
		if ( TloadCreateMaterials( Tloadheader ) != TRUE)
		{
			Msg( "TLoadCreateMaterials() Failed\n" );
			return FALSE;
		}
	}
	
	
	if( d3dappi.Driver[d3dappi.CurrDriver].bIsHardware != 0 )
	{
		// Find out how much video memory there is after...
		Tloadheader->VidMemAfter = D3DAppFreeVideoMemory();
	}
	
	// Tloadheader is valid
	Tloadheader->state = TRUE;

	return( TRUE );
}

BOOL SysTload( SYSTEMMEMTPAGES *SysTextures, int num_tpages )
{
	int i;

	for (i = 0; i < num_tpages; i++)
	{
		SysTextures[i].lpSrcTextureSurf = NULL;

		SysTextures[i].lpSrcTextureSurf = LoadPPMToSystemMemory(
												SysTextures[i].FileName, 
												Tloadheader.CurScale[ SysTextures[i].VidTPageIndex ] );

		if (SysTextures[i].lpSrcTextureSurf == NULL)
		{
			DebugPrintf( "SysTLoad() Failed on %s\n", SysTextures[i].FileName );
			return FALSE;
		}
	}

	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Make a material for all textures associated with Tloadheader
	Input		;		TLOADHEADER *
	Output		:		FLASE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL TloadCreateMaterials( TLOADHEADER * Tloadheader )
{
	D3DMATERIAL mat;
    int i;
	/*	create a default material for each texture */
	memset(&mat, 0, sizeof(D3DMATERIAL));
	mat.dwSize = sizeof(D3DMATERIAL);
	mat.diffuse.r = (D3DVALUE)1.0;
	mat.diffuse.g = (D3DVALUE)1.0;
	mat.diffuse.b = (D3DVALUE)1.0;
	mat.diffuse.a = (D3DVALUE)1.0;
	mat.ambient.r = (D3DVALUE)1.0;
	mat.ambient.g = (D3DVALUE)1.0;
	mat.ambient.b = (D3DVALUE)1.0;
	mat.specular.r = (D3DVALUE)0.0;
	mat.specular.g = (D3DVALUE)0.0;
	mat.specular.b = (D3DVALUE)0.0;
	mat.power = (float)0.0;

	if( d3dappi.Driver[d3dappi.CurrDriver].bIsHardware )
		mat.dwRampSize = 1;
	else
		mat.dwRampSize = 16;

	for (i = 0; i<Tloadheader->num_texture_files; i++)
	{
		/* create the material and header */	
	    mat.hTexture = Tloadheader->hTex[i];
	    if (d3dappi.lpD3D->lpVtbl->CreateMaterial(d3dappi.lpD3D, &Tloadheader->lpMat[i], NULL) != D3D_OK)
		{
			Msg( "Couldnt Create Material for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
	    if (Tloadheader->lpMat[i]->lpVtbl->SetMaterial(Tloadheader->lpMat[i], &mat) != D3D_OK)
		{
			Msg( "Couldnt Set Material for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
	    if (Tloadheader->lpMat[i]->lpVtbl->GetHandle(Tloadheader->lpMat[i], d3dappi.lpD3DDevice, &Tloadheader->hMat[i]) != D3D_OK)
		{
			Msg( "Couldnt Get Handle for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
	}
    return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		
	Input		;		TLOADHEADER * , int n 
	Output		:		FLASE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

BOOL
TloadTextureSurf( TLOADHEADER * Tloadheader , int n)
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
    LPDIRECTDRAWPALETTE lpDstPalette = NULL;
    PALETTEENTRY ppe[256];
    DWORD pcaps;
	char NewName2[256];

    /*
     * Release the surface if it is hanging around
     */

    RELEASE(Tloadheader->lpTextureSurf[n]);

    /*
     * Create a surface in system memory and load the PPM file into it.
     * Query for the texture interface.
     */

	// only load source texture if not a placeholder
	if ( !Tloadheader->PlaceHolder[ n ] )
	{
		// change extension of file name
		Change_Ext( &Tloadheader->ImageFile[n][0], &NewName2[ 0 ], ".BMP" );

		// if file exists
		if( File_Exists( &NewName2[ 0 ] ) )
		{
			if( !HasBmpGotRealBlack( &NewName2[0] ) )
			{
				// override colourkey if bmp doesnt have a real black as its first colour....
				Tloadheader->ColourKey[n] = FALSE;
			}
			if( MipMap && Tloadheader->MipMap[n] )
			{
				lpSrcTextureSurf = DDLoadBitmapTextureMipMap( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
			}else{
				lpSrcTextureSurf = DDLoadBitmapTexture( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
			}
		}

		if ( !lpSrcTextureSurf )
		{
			DebugPrintf( "You should have a .bmp version of this ppm, %s\n", Tloadheader->ImageFile[n][0] );

			if (bPrimaryPalettized)
			{
				lpSrcTextureSurf = TloadSurfaceScale8BitPrimary(d3dappi.lpDD, Tloadheader->ImageFile[n],
													  &d3dappi.ThisTextureFormat.ddsd,
													  DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n]);
			}else{

				if( MipMap && Tloadheader->MipMap[n] )
				{
					lpSrcTextureSurf = LoadMipMap(d3dappi.lpDD, Tloadheader->ImageFile[n],
						&d3dappi.ThisTextureFormat.ddsd,
						DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n]);
				}else{
					lpSrcTextureSurf = TloadSurfaceScale(d3dappi.lpDD, Tloadheader->ImageFile[n],
						&d3dappi.ThisTextureFormat.ddsd,
						DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n]);
				}

			}
		}

		if (!lpSrcTextureSurf){
			Msg( "TloadTextureSurf() !lpSrcTextureSurf\n" );
			goto exit_with_error;
		}

		LastError = lpSrcTextureSurf->lpVtbl->QueryInterface(lpSrcTextureSurf,
												 &IID_IDirect3DTexture,
												 (LPVOID*)&lpSrcTexture);
		if (LastError != DD_OK){
			Msg( "TloadTextureSurf() !lpSrcTextureSurf->lpVtbl->QueryInterface\n" );
			goto exit_with_error;
		}

		LastError = D3DAppIGetSurfDesc(&ddsd, lpSrcTextureSurf);

		if (LastError != DD_OK){
			Msg( "TloadTextureSurf() !D3DAppIGetSurfDesc(&ddsd, lpSrcTextureSurf);\n" );
		    goto exit_with_error;
		}

	}else
	{
		// do not allow mip map placeholders
		if( MipMap && Tloadheader->MipMap[n] ){
			Msg( "TloadTextureSurf() MipMap && Tloadheader->MipMap[n]  \n" );
			return FALSE;
		}
		
		// we still need surface description for placeholder...
		memcpy(&ddsd, &d3dappi.ThisTextureFormat.ddsd, sizeof(DDSURFACEDESC));

		ddsd.dwSize = sizeof(DDSURFACEDESC);
		ddsd.dwWidth = Tloadheader->Xsize[ n ];
		ddsd.dwHeight = Tloadheader->Ysize[ n ];

		// if using square Textures, ensure placeholder is square
		if( d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly )
		{
			if( ddsd.dwHeight != ddsd.dwWidth )
			{
				if( ddsd.dwHeight > ddsd.dwWidth )
				{
					ddsd.dwHeight >>= 1;
				}
				else
				{
					ddsd.dwWidth >>= 1;
				}
			}
		}
	}



    /*
     * Create an empty texture surface to load the source texture into.
     * The DDSCAPS_ALLOCONLOAD flag allows the DD driver to wait until the
     * load call to allocate the texture in memory because at this point,
     * we may not know how much memory the texture will take up (e.g. it
     * could be compressed to an unknown size in video memory).
     */
    
	if( MipMap && Tloadheader->MipMap[n] )
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | 
			DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
		ddsd.ddsCaps.dwCaps =   DDSCAPS_VIDEOMEMORY | DDSCAPS_COMPLEX | 
			DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_ALLOCONLOAD;
	}else{
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD; // | DDSCAPS_3DDEVICE;
//	    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE;
	}

	LastError = D3DAppICreateSurface(&ddsd, &Tloadheader->lpTextureSurf[n]);
    if (LastError != DD_OK) {
		Msg( "TloadTextureSurf() ! D3DAppICreateSurface(&ddsd, &Tloadheader->lpTextureSurf[n])  \n" );
        goto exit_with_error;
    }
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        pcaps = DDPCAPS_4BIT;
    } else {
        pcaps = 0;
    }

	//Msg("pcaps = %d", pcaps);
	//Msg("ddsd.ddpfPixelFormat.dwFlags = %d", ddsd.ddpfPixelFormat.dwFlags);

    if (pcaps) {
        memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
        LastError = d3dappi.lpDD->lpVtbl->CreatePalette(d3dappi.lpDD, pcaps,
                                                 ppe, &lpDstPalette, NULL);
        if (LastError != DD_OK) {
			Msg( "TloadTextureSurf() ! d3dappi.lpDD->lpVtbl->CreatePalette  \n" );
            goto exit_with_error;
        }
        LastError = Tloadheader->lpTextureSurf[n]->lpVtbl->SetPalette(Tloadheader->lpTextureSurf[n],
                                lpDstPalette);
        if (LastError != DD_OK) {
			Msg( "TloadTextureSurf() ! Tloadheader->lpTextureSurf[n]->lpVtbl->SetPalette  \n" );
            goto exit_with_error;
        }
    }

	/*
	 * Try out adding color key to surface
	 */
	if( Tloadheader->ColourKey[n] && d3dappi.Driver[d3dappi.CurrDriver].bTransparency && !DontColourKey )
	{
		DDCOLORKEY ddcolorkey;
		ddcolorkey.dwColorSpaceLowValue = RGB_MAKE( 0, 0, 0 ); //RGB_MAKE( 255 , 0 , 255 );
		ddcolorkey.dwColorSpaceHighValue = ddcolorkey.dwColorSpaceLowValue;
		LastError = Tloadheader->lpTextureSurf[n]->lpVtbl->SetColorKey( Tloadheader->lpTextureSurf[n],
			DDCKEY_SRCBLT, &ddcolorkey);
		if (LastError != DD_OK) {
			Msg( "TloadTextureSurf() ! Tloadheader->lpTextureSurf[n]->lpVtbl->SetColorKey  \n" );
			goto exit_with_error;
		}
	}

    /*
     * Query our destination surface for a texture interface
     */
    LastError = Tloadheader->lpTextureSurf[n]->lpVtbl->QueryInterface(Tloadheader->lpTextureSurf[n],
                                             &IID_IDirect3DTexture,
                                             (LPVOID*)&Tloadheader->lpTexture[n]);
    if (LastError != DD_OK) {
		Msg( "TloadTextureSurf() ! Tloadheader->lpTextureSurf[n]->lpVtbl->QueryInterface  \n" );
        goto exit_with_error;
    }
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */

    
 	if ( !Tloadheader->PlaceHolder[ n ] )
	{

		// test
		//IDirectDrawPalette *pal = NULL;
		//lpSrcTextureSurf->lpVtbl->GetPalette(lpSrcTextureSurf, &pal);
		//Msg("%p\n", pal);

		LastError = Tloadheader->lpTexture[n]->lpVtbl->Load(Tloadheader->lpTexture[n], lpSrcTexture);

	//	LastError =	Tloadheader->lpTextureSurf[n]->lpVtbl->Blt(Tloadheader->lpTextureSurf[n],
	//													   NULL,lpSrcTextureSurf,
	//													   NULL, DDBLT_WAIT, NULL);
		
		if (LastError != DD_OK) {
			D3DAppISetErrorString(
				"TloadTextureSurf() ! Tloadheader->lpTexture[n]->lpVtbl->Load: %s %d \n",
				D3DAppErrorToString(LastError),
				Tloadheader->MipMap[n]
			);
			Msg("%s",LastErrorString);
		    //goto exit_with_error;
	    }
		/* 
	     * Now we are done with the source texture
	     */
		RELEASE(lpSrcTexture);
	    RELEASE(lpSrcTextureSurf);
	}

    /*
     * Did the texture end up in video memory?
     */
    LastError = D3DAppIGetSurfDesc(&ddsd, Tloadheader->lpTextureSurf[n]);
    if (LastError != DD_OK) {
		Msg( "TloadTextureSurf() ! D3DAppIGetSurfDesc  \n" );
        goto exit_with_error;
    }
    if (!(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY))
	{
        Tloadheader->bTexturesInVideo[n] = FALSE;
	}else{
		Tloadheader->bTexturesInVideo[n] = TRUE;
	}
    
    return TRUE;

exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    RELEASE(lpDstPalette);
    RELEASE(Tloadheader->lpTexture[n]);
    RELEASE(Tloadheader->lpTextureSurf[n]);
    return FALSE;       
}

/*
 * TloadGethTex
 * Get a texture handle from the current D3D device for this texture and save
 * it in the MasterhTex list and public texture handle list.
 */
BOOL
TloadGethTex(TLOADHEADER * Tloadheader , int n)
{
    if (d3dappi.ThisDriver.bIsHardware && !Tloadheader->bTexturesInVideo[n]) {
        goto exit_with_error;
    }

    
	LastError = Tloadheader->lpTexture[n]->lpVtbl->GetHandle(Tloadheader->lpTexture[n],
                               d3dappi.lpD3DDevice, &Tloadheader->hTex[n]);
    if (LastError != DD_OK) {
        goto exit_with_error;
    }
    return TRUE;
exit_with_error:
    Tloadheader->hTex[n] = 0;
    return TRUE;
//    return FALSE;       
}

/*
 * TloadReleaseTexture
 * Release this texture surface and texture interface.  Remember, a texture
 * handle is NOT an object and does not need to be released or destroyed.
 * The handle is no longer valid after the device is destroyed, so set it to
 * zero here.
 */
void
TloadReleaseTexture(TLOADHEADER * Tloadheader, int n)
{
    RELEASE(Tloadheader->lpTexture[n]);
    RELEASE(Tloadheader->lpTextureSurf[n]);

	if ( Tloadheader->PlaceHolder[ n ] )
	{
		free( Tloadheader->PlaceHolderFile[ n ] );
		Tloadheader->PlaceHolderFile[ n ] = NULL;
	}

    Tloadheader->hTex[n] = 0;
}

/*
 * ReleaseTloadheader
 * Release all texture surfaces and texture interfaces and materials
 * Release Execute buffers
 */
void
ReleaseTloadheader( TLOADHEADER * Tloadheader )
{
    int i;
    for (i = 0; i < Tloadheader->num_texture_files; i++)
	{
        TloadReleaseTexture( Tloadheader , i);
		RELEASE(Tloadheader->lpMat[i]);
    }

	InitTload( Tloadheader  );
  
}

void ReleaseSysTload( SYSTEMMEMTPAGES * SysTextures, int *num_tpages )
{
	int i;

	for (i = 0; i < *num_tpages; i++)
	{
		RELEASE(SysTextures[i].lpSrcTextureSurf);
	}

	*num_tpages = 0;
}

/*
 * TloadAllTextures
 * Load all texture surfaces, qeury them for texture interfaces and get
 * handles for them from the current D3D driver.
 */
BOOL
TloadAllTextures(TLOADHEADER * Tloadheader)
{
    int i;
    if (d3dappi.ThisDriver.bDoesTextures) {

        for (i = 0; i < Tloadheader->num_texture_files; i++) {
            ATTEMPT(TloadTextureSurf( Tloadheader , i));
            ATTEMPT(TloadGethTex( Tloadheader , i));
        }
    } else {
    }
    return TRUE;

exit_with_error:
    for ( ; i < Tloadheader->num_texture_files; i++) {
        TloadReleaseTexture( Tloadheader , i);
    }
    return FALSE;
}

LPDIRECTDRAWSURFACE CreateTextureSurf(LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpFormat, DWORD memoryflag, DWORD dwWidth, DWORD dwHeight)
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpDDS;
    HRESULT ddrval;

    memcpy(&ddsd, lpFormat, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;

	if( dwHeight != dwWidth )
	{
		DebugPrintf("Tload.c: CreateTextureSurf() Tried to create a non-square texture surface - aborting");
		exit(1);
	}

	ddsd.dwHeight = dwHeight;
	ddsd.dwWidth = dwWidth;
	
    ddrval = lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK)
	{
		DebugPrintf("Tload.c: CreateTextureSurf() unable to create texture surface - aborting");
		exit(1);
	}

	return lpDDS;
}

/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/
/*
 * LoadSurface
 * Loads a ppm file into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 * Scale...divide X and Y by Scale..
 */
LPDIRECTDRAWSURFACE
TloadSurfaceScale( LPDIRECTDRAW lpDD, LPCSTR lpName,
                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale )
{
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC ddsd, format;
    D3DCOLOR colors[256];
    D3DCOLOR c;
    DWORD dwWidth, dwHeight;
    int i, j,e;
    FILE *fp;
    char *lpC;
    CHAR buf[100];
    LPDIRECTDRAWPALETTE lpDDPal;
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
    HRESULT ddrval;
	int r, g, b, a, k;
	int XScale,YScale;

	char TempFilename[ 256 ];

    /*
     * Find the image file and open it
     */

	strcpy( &TempFilename[ 0 ], (char *) lpName );

	fp = fopen( &TempFilename[ 0 ], "rb" );

    if (fp == NULL) {
		Msg( "Cannot find %s.\n", lpName);
        return NULL;
    }

    /*
     * Is it a PPM file?
     */

    fgets(buf, sizeof buf, fp);
    if (lstrcmp(buf, "P6\n")) {
        fclose(fp);
		Msg( "%s Is Not a PPM File\n", lpName);
        return NULL;
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

    /*
     * Create a surface of the given format using the dimensions of the PPM
     * file.
     */

    memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }
    memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;

	XScale = Scale;
	YScale = Scale;

	// PowerVR Only Like Square Textures...
	if( d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly && dwHeight != dwWidth )
	{
			if( dwHeight > dwWidth )
				YScale++;
			if( dwWidth > dwHeight )
				XScale++;
	}

	ddsd.dwHeight = dwHeight / ( 1 << YScale );
	ddsd.dwWidth = dwWidth / ( 1 << XScale );
	
    ddrval = lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK) {
		Msg("CreateSurface for texture failed (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }

    /*
     * Lock the surface so it can be filled with the PPM file
     */

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddrval = lpDDS->lpVtbl->Lock(lpDDS, NULL, &ddsd, 0, NULL);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Lock failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }
    /*

     * The method of loading depends on the pixel format of the dest surface
     */

    if (!bQuant) {

        /*
         * The texture surface is not palettized
         */

        unsigned long* lpLP;
        unsigned short* lpSP;
        unsigned char* lpCP;
        unsigned long m;
        int s;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift, alpha_scale;

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
                for (j = 0; j < (int)ddsd.dwHeight; j++) {

                    /*
                     * Point to next row in texture surface
                     */

					lpLP = (unsigned long*)(((char*)ddsd.lpSurface) +
						                                    ddsd.lPitch * j);


                    for (i = 0; i < (int)ddsd.dwWidth; i++) {

                        /*
                         * Read each value, scale it and shift it into position
                         */

                        r = GammaTab[getc(fp)] / red_scale;
                        g = GammaTab[getc(fp)] / green_scale;
                        b = GammaTab[getc(fp)] / blue_scale;
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
						
                    	if( XScale )
						{
							for( e = 0 ; e < ( ( 1 << XScale ) -1 ) ; e++ )
							{
								r = getc(fp);
		                        g = getc(fp);
		                        b = getc(fp);
							}
						}
					}
                    if( YScale )
					{
						for( e = 0 ; e < ( ( 1 << YScale ) -1 ) * (int) dwWidth ; e++ )
						{
							r = getc(fp);
	                        g = getc(fp);
	                        b = getc(fp);
						}
					}
                }
                break;
            case 16 :
                for (j = 0; j < (int)ddsd.dwHeight; j++) {

					lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +
						                               ddsd.lPitch * j);

                    for (i = 0; i < (int)ddsd.dwWidth; i++) {

						r = GammaTab[getc(fp)];
                        g = GammaTab[getc(fp)];
                        b = GammaTab[getc(fp)];
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
                    	if( XScale )
						{
							for( e = 0 ; e < ( ( 1 << XScale ) -1 ) ; e++ )
							{
								r = getc(fp);
		                        g = getc(fp);
		                        b = getc(fp);
							}
						}
					}
                    if( YScale )
					{
						for( e = 0 ; e < ( ( 1 << YScale ) -1 ) * (int) dwWidth ; e++ )
						{
							r = getc(fp);
	                        g = getc(fp);
	                        b = getc(fp);
						}
					}
                }
                break;
            case 8:
                for (j = 0; j < (int)ddsd.dwHeight; j++) {
					lpCP = (unsigned char*)(((char*)ddsd.lpSurface) +
						                                    ddsd.lPitch * j);

                    for (i = 0; i < (int)ddsd.dwWidth; i++) {
                        r = GammaTab[getc(fp)]/ red_scale;
                        g = GammaTab[getc(fp)]/ green_scale;
                        b = GammaTab[getc(fp)]/ blue_scale;
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
                    	if( XScale )
						{
							for( e = 0 ; e < ( ( 1 << Scale ) -1 ) ; e++ )
							{
								r = getc(fp);
		                        g = getc(fp);
		                        b = getc(fp);
							}
						}
					}
                    if( YScale )
					{
						for( e = 0 ; e < ( ( 1 << Scale ) -1 ) * (int) dwWidth ; e++ )
						{
							r = getc(fp);
	                        g = getc(fp);
	                        b = getc(fp);
						}
					}
                }
                break;
            default:

                /*
                 * This wasn't a format I recognize
                 */

                lpDDS->lpVtbl->Unlock(lpDDS, NULL);
                fclose(fp);
                lpDDS->lpVtbl->Release(lpDDS);
				Msg("Unknown pixel format (loadtex).");
                return NULL;
        }

        /*
         * Unlock the texture and return the surface pointer
         */

        lpDDS->lpVtbl->Unlock(lpDDS, NULL);
        fclose(fp);
        return (lpDDS);
    }

    /*
     * We assume the 8-bit palettized case
     */

	// Allways make color 0 black for transparency..
	colors[0] = RGB_MAKE(0, 0, 0);
	color_count = 1;    /* number of colors in the texture */
    for (j = 0; j < (int)ddsd.dwHeight; j++)
	{

        /*
         * Point to next row in surface
         */

        lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
        for (i = 0; i < (int)ddsd.dwWidth; i++)
		{

            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */

            r = GammaTab[getc(fp)];
            g = GammaTab[getc(fp)];
            b = GammaTab[getc(fp)];

			if ( Pal332 )
			{
				r += Random_Range( 31 );
				if ( r > 255 ) r = 255;
				if ( r < 0 ) r = 0;
				g += Random_Range( 31 );
				if ( g > 255 ) g = 255;
				if ( g < 0 ) g = 0;
				b += Random_Range( 31 );
				if ( b > 255 ) b = 255;
				if ( b < 0 ) b = 0;

				r &= 0xE0;
				g &= 0xE0;
				b &= 0xC0;
				r |= 0x08;
				g |= 0x08;
				b |= 0x10;
			}
            c = RGB_MAKE(r, g, b);

            /*
             * Search for this color in a table of colors in this texture
             */

            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break;
            if (k == color_count) {

                /*
                 * This is a new color, so add it to the list
                 */

                color_count++;

                /*
                 * More than 256 and we fail (8-bit) 
                 */

                if (color_count > psize) {
                    color_count--;
                    k = color_count - 1;
                    //goto burst_colors;
                }
                colors[k] = c;
            }

            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */

            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
            if( XScale )
			{
				for( e = 0 ; e < ( ( 1 << XScale ) -1 ) ; e++ )
				{
					r = getc(fp);
				    g = getc(fp);
				    b = getc(fp);
				}
			}
		}
		if( YScale )
		{
			for( e = 0 ; e < ( ( 1 << YScale ) -1 ) * (int) dwWidth ; e++ )
			{
				r = getc(fp);
			    g = getc(fp);
			    b = getc(fp);
			}
		}
    }

    /*
     * Close the file and unlock the surface
     */

    fclose(fp);
    lpDDS->lpVtbl->Unlock(lpDDS, NULL);

//burst_colors:
    if (color_count > psize) {

        /*
         * If there are more than 256 colors, we overran our palette
         */

        lpDDS->lpVtbl->Unlock(lpDDS, NULL);
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Palette burst. (loadtex).\n");
        return (NULL);
    }

    /*
     * Create a palette with the colors in our color table
     */

    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
    for (i = 0; i < color_count; i++) {
        ppe[i].peRed = (unsigned char)RGB_GETRED(colors[i]);
        ppe[i].peGreen = (unsigned char)RGB_GETGREEN(colors[i]);
        ppe[i].peBlue = (unsigned char)RGB_GETBLUE(colors[i]);
    }

    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */

    for (; i < 256; i++)
        ppe[i].peFlags = D3DPAL_RESERVED;

    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */

    ddrval = lpDD->lpVtbl->CreatePalette(lpDD,
                                         DDPCAPS_INITIALIZE | pcaps | DDPCAPS_ALLOW256,
                                         ppe, &lpDDPal, NULL);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Create palette failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return (NULL);
    }

    /*
     * Finally,bind the palette to the surface
     */

    ddrval = lpDDS->lpVtbl->SetPalette(lpDDS, lpDDPal);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
        lpDDPal->lpVtbl->Release(lpDDPal);
		Msg("SetPalette failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return (NULL);
    }

    lpDDPal->lpVtbl->Release(lpDDPal);

    return lpDDS;
}



/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/
/*
 * LoadSurface
 * Loads A BLANK VERSION OF a ppm file into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 * Scale...divide X and Y by Scale..
 */
LPDIRECTDRAWSURFACE
TloadSurfaceBlank( LPDIRECTDRAW lpDD, LPCSTR lpName,
                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale , TLOADHEADER * Tloadheader , int16 n)
{
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC ddsd, format;
    D3DCOLOR colors[256];
    D3DCOLOR c;
    DWORD dwWidth, dwHeight;
    int i, j;
    char *lpC;
    LPDIRECTDRAWPALETTE lpDDPal;
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
    HRESULT ddrval;
	int r, g, b, a, k;
	int XScale,YScale;

    /*
     * Create a surface of the given format using the dimensions of the PPM
     * file.
     */

	dwWidth =  Tloadheader->Xsize[n];
	dwHeight =  Tloadheader->Ysize[n];
	
	memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }
    memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;

	XScale = Scale;
	YScale = Scale;
	// PowerVR Only Like Square Textures...
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

	

	
	ddsd.dwHeight = dwHeight / ( 1 << YScale );
	ddsd.dwWidth = dwWidth / ( 1 << XScale );
	
    ddrval = lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK) {
		Msg("CreateSurface for texture failed (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }
    /*
     * Lock the surface so it can be filled with the PPM file
     */
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddrval = lpDDS->lpVtbl->Lock(lpDDS, NULL, &ddsd, 0, NULL);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Lock failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }
    /*
     * The method of loading depends on the pixel format of the dest surface
     */
    if (!bQuant) {
        /*
         * The texture surface is not palettized
         */
        unsigned long* lpLP;
        unsigned short* lpSP;
        unsigned char* lpCP;
        unsigned long m;
        int s;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift, alpha_scale;
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
                for (j = 0; j < (int)ddsd.dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
					lpLP = (unsigned long*)(((char*)ddsd.lpSurface) +
						                                    ddsd.lPitch * j);


                    for (i = 0; i < (int)ddsd.dwWidth; i++) {
                        /*
                         * Read each value, scale it and shift it into position
                         */
                        r = BLANK_RED / red_scale;
                        g = BLANK_GREEN / green_scale;
                        b = BLANK_BLUE / blue_scale;
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
                for (j = 0; j < (int)ddsd.dwHeight; j++) {

					lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +
						                               ddsd.lPitch * j);

                    for (i = 0; i < (int)ddsd.dwWidth; i++) {

                        r = BLANK_RED;
                        g = BLANK_GREEN;
                        b = BLANK_BLUE;
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
                for (j = 0; j < (int)ddsd.dwHeight; j++) {
					lpCP = (unsigned char*)(((char*)ddsd.lpSurface) +
						                                    ddsd.lPitch * j);

                    for (i = 0; i < (int)ddsd.dwWidth; i++) {
                        r = BLANK_RED / red_scale;
                        g = BLANK_GREEN / green_scale;
                        b = BLANK_BLUE / blue_scale;
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
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->lpVtbl->Unlock(lpDDS, NULL);
                lpDDS->lpVtbl->Release(lpDDS);
				Msg("Unknown pixel format (loadtex).");
                return NULL;
        }
        /*
         * Unlock the texture and return the surface pointer
         */
        lpDDS->lpVtbl->Unlock(lpDDS, NULL);
        return (lpDDS);
    }

    /*
     * We assume the 8-bit palettized case
     */
	// Allways make color 0 black for transparency..
	colors[0] = RGB_MAKE(0, 0, 0);
	color_count = 1;    /* number of colors in the texture */
    for (j = 0; j < (int)ddsd.dwHeight; j++)
	{
        /*
         * Point to next row in surface
         */
        lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
        for (i = 0; i < (int)ddsd.dwWidth; i++)
		{
            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */
			r = BLANK_RED;
			g = BLANK_GREEN;
			b = BLANK_BLUE;
            c = RGB_MAKE(r, g, b);
            /*
             * Search for this color in a table of colors in this texture
             */
            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break;
            if (k == color_count) {
                /*
                 * This is a new color, so add it to the list
                 */
                color_count++;
                /*
                 * More than 256 and we fail (8-bit) 
                 */
                if (color_count > psize) {
                    color_count--;
                    k = color_count - 1;
                    //goto burst_colors;
                }
                colors[k] = c;
            }
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
		}
    }
    /*
     * Close the file and unlock the surface
     */
    lpDDS->lpVtbl->Unlock(lpDDS, NULL);

//burst_colors:
    if (color_count > psize) {
        /*
         * If there are more than 256 colors, we overran our palette
         */
        lpDDS->lpVtbl->Unlock(lpDDS, NULL);
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Palette burst. (loadtex).\n");
        return (NULL);
    }

    /*
     * Create a palette with the colors in our color table
     */
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
    for (i = 0; i < color_count; i++) {
        ppe[i].peRed = (unsigned char)RGB_GETRED(colors[i]);
        ppe[i].peGreen = (unsigned char)RGB_GETGREEN(colors[i]);
        ppe[i].peBlue = (unsigned char)RGB_GETBLUE(colors[i]);
    }
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
        ppe[i].peFlags = D3DPAL_RESERVED;
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    ddrval = lpDD->lpVtbl->CreatePalette(lpDD,
                                         DDPCAPS_INITIALIZE | pcaps | DDPCAPS_ALLOW256,
                                         ppe, &lpDDPal, NULL);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
		Msg("Create palette failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return (NULL);
    }
    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->lpVtbl->SetPalette(lpDDS, lpDDPal);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
        lpDDPal->lpVtbl->Release(lpDDPal);
		Msg("SetPalette failed while loading surface (loadtex).\n%s", D3DAppErrorToString(ddrval));
        return (NULL);
    }

    lpDDPal->lpVtbl->Release(lpDDPal);

    return lpDDS;
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Get the width and height for a PPM
	Input		:		TLOADHEADER * , int , lpname, uint16 * , uint16 * 
	Output		:		BOOL FALSE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
TloadGetStats( TLOADHEADER * Tloadheader , int i ,LPCSTR lpName , uint16 * Width , uint16 * Height )
{
    DWORD dwWidth, dwHeight;
    FILE *fp;
    CHAR buf[100];
	int8 TempFilename[ 256 ];
	char NewName2[256];
	BITMAPINFOHEADER Header;

	Change_Ext( &Tloadheader->ImageFile[i][0], &NewName2[ 0 ], ".BMP" );

	if( File_Exists( &NewName2[ 0 ] ) )
	{
		fp = fopen( &NewName2[ 0 ], "rb" );
		fseek( fp , sizeof(BITMAPFILEHEADER) ,0 );
		fread( &Header , sizeof(BITMAPINFOHEADER) , 1 , fp );

		fclose(fp);

		*Width = (uint16) abs(Header.biWidth);
		*Height = (uint16) abs(Header.biHeight);

	}else{
		/*
		 * Find the image file and open it
		 */
		strcpy( &TempFilename[ 0 ], (char *) lpName );
		fp = fopen( &TempFilename[ 0 ], "rb" );
		if (fp == NULL) {
			Msg( "Couldnt Find %s\n", lpName );
			return FALSE;
		}
		// Is it a PPM file?
		fgets(buf, sizeof buf, fp);
		if (lstrcmp(buf, "P6\n")) {
			fclose(fp);
			Msg( "%s Is Not a PPM File\n", lpName );
			return FALSE;
		}
		// Skip any comments
		do {
			fgets(buf, sizeof buf, fp);
		} while (buf[0] == '#');
		// Read the width and height
		sscanf(buf, "%d %d\n", &dwWidth, &dwHeight);
		// close the file
		fclose(fp);
		// store the width and heights
		*Width = (uint16) dwWidth;
		*Height = (uint16) dwHeight;
	}
	return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Find a texture from a tloadheader Ignores the Path
	Input		:		TLOADHEADER * , char * Name
	Output		:		-1 if no match found otherwise number of texture
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

int16	FindTexture( TLOADHEADER * Tloadheader , char * Name )
{
	int	i;
	char * pnt1;
	char * pnt2;

	if( !d3dappi.Driver[d3dappi.CurrDriver].bDoesTextures )
		return 0;
	
	pnt1 = strrchr( Name, '\\' );
	if( pnt1 )
	{
		pnt1++;
	}else{
		pnt1 = Name;
	}

	for( i = 0 ; i < Tloadheader->num_texture_files ; i++ )
	{
		pnt2 = strrchr( &Tloadheader->ImageFile[i][0] , '\\' );
		if( pnt2 )
		{
			pnt2++;
		}else{
			pnt2 = &Tloadheader->ImageFile[i][0];
		}

		if( _stricmp( pnt1, pnt2 ) == 0 )
			return i;

//		if( _stricmp( (char*) Name, (char*) &Tloadheader->ImageFile[i] ) == 0 )
//			return i;
	}
	return -1;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:		Add a texture to a tloadheader
	Input		:		TLOADHEADER * , char * Name , ColourKey
	Output		:		-1 if too many tpages
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

int16	AddTexture( TLOADHEADER * Tloadheader , char * Name , uint16 ColourKey , BOOL Scale , BOOL MipMap, int16 xsize, int16 ysize )
{
	int16 i;
	char * NamePnt;

	// are we just loading placeholder?
	if ( !Name[ 0 ] )
	{
	 	i = Tloadheader->num_texture_files;
		Tloadheader->ColourKey[i] = ColourKey;
		Tloadheader->Scale[i] = FALSE;		// cannot allow scaling of placeholder!
		Tloadheader->MipMap[i] = MipMap;
		Tloadheader->ImageFile[i][0] = 0;
		Tloadheader->PlaceHolder[i] = TRUE;
		Tloadheader->num_texture_files++;

		Tloadheader->Xsize[ i ] = xsize;		
		Tloadheader->Ysize[ i ] = ysize;		

		return i;
	}

	// has the Texture allready been Added ??
	if( ( i = FindTexture( Tloadheader , Name ) ) != -1 )
		return i;										// found file of same name....

	if( Tloadheader->num_texture_files >= MAXTPAGESPERTLOAD )
	{
		return( -1 );
	}

	i = Tloadheader->num_texture_files;
	Tloadheader->ColourKey[i] = ColourKey;
	Tloadheader->Scale[i] = Scale;
	Tloadheader->MipMap[i] = MipMap;
	NamePnt = (char *) &Tloadheader->ImageFile[i];
	while( ( *NamePnt++ = *Name++ ) != 0 );
		Tloadheader->PlaceHolder[i] = FALSE;
	Tloadheader->num_texture_files++;
	return i;
}


// Image file - name of PPM file to load
// Scale by - scaling required (0 = no scaling)
LPDIRECTDRAWSURFACE LoadPPMToSystemMemory( char *ImageFile, int ScaleBy )
{
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
	char NewName2[256];

	Change_Ext( ImageFile, &NewName2[ 0 ], ".BMP" );

	if( File_Exists( &NewName2[ 0 ] ) )
		lpSrcTextureSurf = DDLoadBitmapTexture(
								d3dappi.lpDD,
								&NewName2[0],
								&d3dappi.ThisTextureFormat.ddsd,
								(int) ScaleBy,
								d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );

	/*
	 * Create a surface in system memory and load the PPM file into it.
	 * Query for the texture interface.
	 */

	if( !lpSrcTextureSurf )
		lpSrcTextureSurf = TloadSurfaceScale(
								d3dappi.lpDD,
								ImageFile,
								&d3dappi.ThisTextureFormat.ddsd,
								DDSCAPS_SYSTEMMEMORY,
								(int16)ScaleBy); // dont use data dir

    if (!lpSrcTextureSurf)
        goto exit_with_error;

	return lpSrcTextureSurf;


exit_with_error:

    RELEASE(lpSrcTextureSurf);

    return NULL;       

}

BOOL BlitTextureToVideoMemory( TLOADHEADER *Tloadheader, int16 n, LPDIRECTDRAWSURFACE lpSrcTextureSurf )
{
    LPDIRECTDRAWSURFACE lpdds = NULL;
    DDBLTFX fx;

	LastError = Tloadheader->lpTexture[n]->lpVtbl->QueryInterface(Tloadheader->lpTexture[n],
																	&IID_IDirectDrawSurface, 
																	(LPVOID*)&lpdds);
    if (LastError != DD_OK)
	{
		DebugPrintf("BlitTextureToVideoMemory() unable to query for DD surface\n");
		return FALSE;
	}

	memset(&fx, 0, sizeof(DDBLTFX));
	fx.dwSize = sizeof(DDBLTFX);

	LastError = lpdds->lpVtbl->Blt( lpdds, NULL, lpSrcTextureSurf, NULL, DDBLT_WAIT , &fx );
	if( LastError != DD_OK )
	{
		DebugPrintf("BlitTextureToVideoMemory() Direct Draw Blit error (%s)\n", D3DAppErrorToString(LastError));
		return FALSE;
	} 

	RELEASE (lpdds);
	return TRUE;
}


// Tloadheader - array of all textures currently in video memory
// n - index num of texture to replace (must be same size as new texture!!!)
// Surface  - system memory surface to replace video memory surface
BOOL MovePPMToVideoMemory( TLOADHEADER *Tloadheader, int16 n, LPDIRECTDRAWSURFACE lpSrcTextureSurf )
{
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;

    LastError = lpSrcTextureSurf->lpVtbl->QueryInterface(lpSrcTextureSurf,
                                             &IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);

    if (LastError != DD_OK) goto exit_with_error;


    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    LastError = Tloadheader->lpTexture[n]->lpVtbl->Load(Tloadheader->lpTexture[n], lpSrcTexture);

    if (LastError != DD_OK)
	{
		Msg( "Load Texture falied for %s", &Tloadheader->ImageFile[n] );
		goto exit_with_error;
	}

    /* 
     * Now we are done with the source texture, but we keep surface in system mem for subsequent reloads
     */
    RELEASE(lpSrcTexture);
    return TRUE;
exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    return FALSE;   
}

BOOL
TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16 n )
{
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
	//HRESULT	result;
	char NewName2[256];

	/*
		Change_Ext( &Tloadheader->ImageFile[n][0], &NewName2[ 0 ], ".BMP" );
		GoodFileName = File_Exists( &NewName2[ 0 ] );
	*/

	if( !Tloadheader->PlaceHolderFile[ n ] || !Tloadheader->PlaceHolderFile[ n ][ 0 ] )
		return FALSE;

	Change_Ext( Tloadheader->PlaceHolderFile[ n ], NewName2, ".BMP" );

	if( File_Exists( &NewName2[0] ) )
	{
		if( MipMap && Tloadheader->MipMap[n] )
		{
			
			lpSrcTextureSurf = DDLoadBitmapTextureMipMap( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
		}else{
			lpSrcTextureSurf = DDLoadBitmapTexture( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
		}
	}

	if( !lpSrcTextureSurf )
	{
		if ( !Tloadheader->PlaceHolderFile[ n ][0] )
			return FALSE;
		if ( !File_Exists( Tloadheader->PlaceHolderFile[ n ] ) )
			return FALSE;
		lpSrcTextureSurf = LoadPPMToSystemMemory( Tloadheader->PlaceHolderFile[ n ], Tloadheader->CurScale[ n ] );
	}

	if ( !lpSrcTextureSurf )
	{
		Msg( "TloadReloadPlaceHolder() Failed on %s\n", Tloadheader->PlaceHolderFile[ n ] );
		return FALSE;
	}

	if (!( MovePPMToVideoMemory( Tloadheader, n, lpSrcTextureSurf ) ))
	{
		Msg( "Load Texture falied for %s", Tloadheader->PlaceHolderFile[ n ] );
		return FALSE;
	}

	/*
	// copy to vid mem
	if ( lpSrcTextureSurf->lpVtbl->QueryInterface(lpSrcTextureSurf, &IID_IDirect3DTexture, (LPVOID*)&lpSrcTexture) != DD_OK )
	{
	    RELEASE(lpSrcTexture);
	    RELEASE(lpSrcTextureSurf);
		return FALSE;
	}

	
//    if ( Tloadheader->lpTexture[ n ]->lpVtbl->Load(Tloadheader->lpTexture[ n ], lpSrcTexture) != DD_OK )
	result = Tloadheader->lpTexture[ n ]->lpVtbl->Load(Tloadheader->lpTexture[ n ], lpSrcTexture);
	
	if( result != DD_OK )
	{
		Msg( "Load Texture falied for %s", Tloadheader->PlaceHolderFile[ n ] );
	    RELEASE(lpSrcTexture);
	    RELEASE(lpSrcTextureSurf);
		return FALSE;
	}

    RELEASE(lpSrcTexture);
*/

	return TRUE;
}

/*
 * TloadReloadTextureSurf
 * Reloads a lost and restored texture surface
 */
BOOL
TloadReloadTextureSurf(TLOADHEADER * Tloadheader , int16 n)
{
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
	char NewName2[256];

    /*
     * Create a surface in system memory and load the PPM file into it.
     * Query for the texture interface.
     */

	Change_Ext( &Tloadheader->ImageFile[n][0], &NewName2[ 0 ], ".BMP" );

	if( File_Exists( &NewName2[ 0 ] ) )
	{
		if( MipMap && Tloadheader->MipMap[n] )
		{
			
			lpSrcTextureSurf = DDLoadBitmapTextureMipMap( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
		}else{
			lpSrcTextureSurf = DDLoadBitmapTexture( d3dappi.lpDD , &NewName2[0], &d3dappi.ThisTextureFormat.ddsd , (int) Tloadheader->CurScale[n] , d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly );
		}
	}
	if( !lpSrcTextureSurf )
	{
		if( MipMap && Tloadheader->MipMap[n] )
		{
			lpSrcTextureSurf = LoadMipMap(d3dappi.lpDD, Tloadheader->ImageFile[n],
				&d3dappi.ThisTextureFormat.ddsd,
				DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n]);
		}else{
			lpSrcTextureSurf = TloadSurfaceScale(d3dappi.lpDD, Tloadheader->ImageFile[n],
				&d3dappi.ThisTextureFormat.ddsd,
				DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n]);
		}
	}
    if (!lpSrcTextureSurf)
        goto exit_with_error;
    LastError = lpSrcTextureSurf->lpVtbl->QueryInterface(lpSrcTextureSurf,
                                             &IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (LastError != DD_OK) goto exit_with_error;
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    LastError = Tloadheader->lpTexture[n]->lpVtbl->Load(Tloadheader->lpTexture[n], lpSrcTexture);

    if (LastError != DD_OK)
	{
		Msg( "Load Texture falied for %s", &Tloadheader->ImageFile[n] );
		goto exit_with_error;
	}

    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    return TRUE;
exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    return FALSE;       
}


/*
 * TloadBlankTextureSurf
 * Reloads a blank texture surface
 */
BOOL
TloadBlankTextureSurf(TLOADHEADER * Tloadheader , int16 n)
{
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;

    /*
     * Create a surface in system memory and load the PPM file into it.
     * Query for the texture interface.
     */
    lpSrcTextureSurf = TloadSurfaceBlank(d3dappi.lpDD, Tloadheader->ImageFile[n],
                                          &d3dappi.ThisTextureFormat.ddsd,
                                          DDSCAPS_SYSTEMMEMORY, Tloadheader->CurScale[n], Tloadheader ,  n  );
    if (!lpSrcTextureSurf)
        goto exit_with_error;
    LastError = lpSrcTextureSurf->lpVtbl->QueryInterface(lpSrcTextureSurf,
                                             &IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (LastError != DD_OK) goto exit_with_error;
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    LastError = Tloadheader->lpTexture[n]->lpVtbl->Load(Tloadheader->lpTexture[n], lpSrcTexture);

    if (LastError != DD_OK)
	{
		Msg( "Load Texture falied for %s", &Tloadheader->ImageFile[n] );
		goto exit_with_error;
	}

    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    return TRUE;
exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    return FALSE;       
}




/*
 * TloadCheckForLostSurfaces
 */
#define TLOADCHECKSURF(x) if (x) {                                               \
                        if (x->lpVtbl->IsLost(x) == DDERR_SURFACELOST) {    \
                            LastError = x->lpVtbl->Restore(x);              \
                            if (LastError != DD_OK) goto exit_with_error;   \
                            b = TRUE;                                       \
                        }                                                   \
                     }
BOOL
TloadCheckForLostSurfaces(TLOADHEADER * Tloadheader)
{
	BOOL b;
    int16 i;
    for (i = 0; i < Tloadheader->num_texture_files; i++)
	{
        b = FALSE;
        TLOADCHECKSURF(Tloadheader->lpTextureSurf[i]);
        if (b)
		{
            if ( !Tloadheader->PlaceHolder[ i ] )
			{
				ATTEMPT(TloadReloadTextureSurf( Tloadheader , i));
			}
			else
			{
				// we are not bothered if TloadReloadPlaceHolder returns FALSE, since current texture may not exist,
				// which indicates that the texture is not currently displayed.
				TloadReloadPlaceHolder( Tloadheader, i );
			}
        }
    }
    return TRUE;
exit_with_error:
    return FALSE;       
}







/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/
/*
 * LoadSurface
 * Loads a ppm file into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 * Scale...0 = No Scale...1 = Half X....2 = Half X and Y
 */
LPDIRECTDRAWSURFACE
TloadSurfaceScale8BitPrimary( LPDIRECTDRAW lpDD, LPCSTR lpName,
                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale )
{
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC ddsd, format;
    DWORD dwWidth, dwHeight;
    int i, j;
    FILE *fp;
    char *lpC;
    CHAR buf[100];
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
    HRESULT ddrval;
	char	TempName[128];
	char	* NamePnt;
	char	TempFilename[ 128 ];

	NamePnt = &TempName[0];
	while( ( *NamePnt++ = *lpName++ ) != 0 );
	TempName[0] = 0x38;
    
	/*
     * Find the image file and open it
     */

	strcpy( &TempFilename[ 0 ], (char *) lpName );

	fp = fopen( &TempFilename[ 0 ], "rb" );

    if (fp == NULL) {
        D3DAppISetErrorString("Cannot find %s.\n", &TempName[0]);
        return NULL;
    }
    /*
     * Is it a PPM file?
     */
    fgets(buf, sizeof buf, fp);
    if (lstrcmp(buf, "P6\n")) {
        fclose(fp);
        D3DAppISetErrorString("%s is not a PPM file.\n", &TempName[0]);
        return NULL;
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
    /*
     * Create a surface of the given format using the dimensions of the PPM
     * file.
     */
    memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }
    memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;

	if( Scale == 0 )
	{
		ddsd.dwHeight = dwHeight;
		ddsd.dwWidth = dwWidth;
	}else{
		if( Scale == 1 )
		{
	 		ddsd.dwHeight = dwHeight/2;
			ddsd.dwWidth = dwWidth;
		}else{
	 		ddsd.dwHeight = dwHeight/2;
			ddsd.dwWidth = dwWidth/2;
		}
	}

    ddrval = lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK) {
        D3DAppISetErrorString("CreateSurface for texture failed (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }
    /*
     * Lock the surface so it can be filled with the PPM file
     */
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddrval = lpDDS->lpVtbl->Lock(lpDDS, NULL, &ddsd, 0, NULL);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
        D3DAppISetErrorString("Lock failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return NULL;
    }


    
	lpPalette->lpVtbl->GetEntries(lpPalette, 0, 0, 256,
                                  &ppe[0]);

	
	/*
     * We assume the 8-bit palettized case
     */
    color_count = psize;    /* number of colors in the texture */
    for (j = 0; j < (int)ddsd.dwHeight; j++)
	{
        /*
         * Point to next row in surface
         */
        lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
        for (i = 0; i < (int)ddsd.dwWidth; i++)
		{
            int r, g, b, k;
            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */
            r = GammaTab[getc(fp)];
            g = GammaTab[getc(fp)];
            b = GammaTab[getc(fp)];
            /*
             * Search for this color in a table of colors in this texture
             */
            for (k = 0; k < color_count; k++)
			{
                if ( ( r == ppe[k].peRed ) && ( g == ppe[k].peGreen )&& ( b == ppe[k].peBlue ) )
					break;
			}
            if (k == color_count)
			{
				// the color just did not match...
				k = 0;
            }
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
			if( Scale != 0)
			{
				getc(fp);
				getc(fp);
				getc(fp);
			}

        }
		if( Scale == 2)
		{
		    for (i = 0; i < (int)dwWidth; i++)
			{
		        getc(fp);
		        getc(fp);
		        getc(fp);
			}
		}
    }
    /*
     * Close the file and unlock the surface
     */
    fclose(fp);
    lpDDS->lpVtbl->Unlock(lpDDS, NULL);

    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->lpVtbl->SetPalette(lpDDS, lpPalette);
    if (ddrval != DD_OK) {
        lpDDS->lpVtbl->Release(lpDDS);
        D3DAppISetErrorString("SetPalette failed while loading surface (loadtex).\n%s",
                              D3DAppErrorToString(ddrval));
        return (NULL);
    }


    return lpDDS;
}

BOOL FreeTextureMemory( int * TMem)
{
    HRESULT ddrval;
    DWORD dwTotalMem;
    DWORD dwFreeMem;
    DDSCAPS DDSCaps;
	LPDIRECTDRAW2 lpDD2;

    IDirectDraw_QueryInterface(d3dappi.lpDD,&IID_IDirectDraw2,&lpDD2);

#if 0
    DDSCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
    ddrval = IDirectDraw2_GetAvailableVidMem(lpDD2, &DDSCaps, &dwTotalMem, &dwFreeMem );
    if(ddrval != DD_OK)
    {
        dwTotalMem = 0;
        dwFreeMem = 0;
    };

    DDSCaps.dwCaps = DDSCAPS_LOCALVIDMEM;
    ddrval = IDirectDraw2_GetAvailableVidMem(lpDD2, &DDSCaps, &dwTotalMem, &dwFreeMem );
    if(ddrval != DD_OK)
    {
        dwTotalMem = 0;
        dwFreeMem = 0;
    };

    DDSCaps.dwCaps = DDSCAPS_NONLOCALVIDMEM;
    ddrval = IDirectDraw2_GetAvailableVidMem(lpDD2, &DDSCaps, &dwTotalMem, &dwFreeMem );
    if(ddrval != DD_OK)
    {
        dwTotalMem = 0;
        dwFreeMem = 0;
    };
#endif

    DDSCaps.dwCaps = DDSCAPS_TEXTURE;
    ddrval = IDirectDraw2_GetAvailableVidMem(lpDD2, &DDSCaps, &dwTotalMem, &dwFreeMem );
    if(ddrval != DD_OK)
    {
		return FALSE;
    };

	*TMem = dwFreeMem;

	RELEASE(lpDD2);

    return TRUE; 
};



#ifdef OPT_ON
#pragma optimize( "", off )
#endif
