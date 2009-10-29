#if 1 // bjd		    
/*==========================================================================
 *
 *  File: Tload.c
 *	loads all the textures necessary for a level..
 *  rescaling if nescessary...
***************************************************************************/

/*===================================================================
		Include File...	
===================================================================*/
#include "typedefs.h"
#include "tload.h"
#include "xmem.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif


#define BLANK_RED		(255)
#define BLANK_GREEN		(255)
#define BLANK_BLUE		(255)

/*===================================================================
		Externals...	
===================================================================*/

extern	BOOL	TexturesEnabled;

/*===================================================================
		Globals...	
===================================================================*/
#define	MAXSCALE 3

TLOADHEADER	Tloadheader;
int Print4x5Text( char * Text , int x , int y , int col );
double	Gamma = 1.0;
/*===================================================================
	Procedure	:		Build a Gamma Correction table 
	Input		:		double GammaValue
	Output		:		Nothing
===================================================================*/
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

/*===================================================================
	Procedure	:		Init a Tloadheader
	Input		:		TLOADHEADER *
	Output		:		BOOL FALSE/TRUE
===================================================================*/
BOOL InitTload( TLOADHEADER * Tloadheader  )
{
	int i;

	Tloadheader->num_texture_files = 0;
	for( i = 0 ; i < MAXTPAGESPERTLOAD ; i++ )
	{
		Tloadheader->lpTexture[i]     = NULL; // texture
		memset(&Tloadheader->lpMat[i], 0, sizeof(D3DMATERIAL9));
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


/*===================================================================
	Procedure	:		Load All textures associated with a level
	Input		:		TLOADHEADER *
	Output		:		BOOL FALSE/TRUE
===================================================================*/
BOOL Tload( TLOADHEADER * Tloadheader  )
{
	int	i,e;
	int LeastScaledThatCanbe;
	int LeastScaledThatCanbeScale;

	BuildGammaTab( Gamma );
 
	// Tloadheader is not valid until everything has been done..
	Tloadheader->state = FALSE;

	// allocate space for placeholder file names
	for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
	{
		if ( Tloadheader->PlaceHolder[ i ] )
		{
			Tloadheader->PlaceHolderFile[ i ] = ( char * )malloc( sizeof( char ) * 256 );
			DebugPrintf( "Tload: Placeholder ( texture %2d ) xsize %d ysize %d\n", i, Tloadheader->Xsize[ i ], Tloadheader->Ysize[ i ] );
		}
	}
	
	// get the stats for the Currently files..
	for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
	{
		Tloadheader->CurScale[i] = 0;

		if ( !Tloadheader->PlaceHolder[ i ] )
		{
			DebugPrintf( "Tload: texture file %2d = %s\n", i, Tloadheader->ImageFile[ i ] );
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

	if ( Tloadheader->num_texture_files != 0 )
	{
		//	load in and convert all textures
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

	// Tloadheader is valid
	Tloadheader->state = TRUE;

	return( TRUE );
}

/*===================================================================
	Procedure	:		Make a material for all textures associated with Tloadheader
	Input		;		TLOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/
BOOL TloadCreateMaterials( TLOADHEADER * Tloadheader )
{
	D3DMATERIAL9 mat;

    int i;
	/*	create a default material for each texture */
	memset(&mat, 0, sizeof(D3DMATERIAL9));
	mat.Diffuse.r = (D3DVALUE)1.0;
	mat.Diffuse.g = (D3DVALUE)1.0;
	mat.Diffuse.b = (D3DVALUE)1.0;
	mat.Diffuse.a = (D3DVALUE)1.0;
	mat.Ambient.r = (D3DVALUE)1.0;
	mat.Ambient.g = (D3DVALUE)1.0;
	mat.Ambient.b = (D3DVALUE)1.0;
	mat.Specular.r = (D3DVALUE)0.0;
	mat.Specular.g = (D3DVALUE)0.0;
	mat.Specular.b = (D3DVALUE)0.0;
	mat.Power = (float)0.0;

	for (i = 0; i<Tloadheader->num_texture_files; i++)
	{
		Tloadheader->lpMat[i] = mat;

		if (FAILED(FSSetMaterial(&mat)))
		{
			Msg( "Couldnt Create Material for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
#if 0 // bjd - CHECK
		/* create the material and header */	
	    mat.hTexture = Tloadheader->hTex[i];
	    if (render_info.lpD3D->lpVtbl->CreateMaterial(render_info.lpD3D, &Tloadheader->lpMat[i], NULL) != D3D_OK)
		{
			Msg( "Couldnt Create Material for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
	    if (Tloadheader->lpMat[i]->lpVtbl->SetMaterial(Tloadheader->lpMat[i], &mat) != D3D_OK)
		{
			Msg( "Couldnt Set Material for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
	    if (Tloadheader->lpMat[i]->lpVtbl->GetHandle(Tloadheader->lpMat[i], render_info.lpD3DDevice, &Tloadheader->hMat[i]) != D3D_OK)
		{
			Msg( "Couldnt Get Handle for %s\n", &Tloadheader->ImageFile[i] );
			return FALSE;
		}
#endif
	}
    return TRUE;
}

/*===================================================================
	Procedure	:		
	Input		;		TLOADHEADER * , int n 
	Output		:		FLASE/TRUE
===================================================================*/

BOOL
TloadTextureSurf( TLOADHEADER * Tloadheader , int n)
{
    LPDIRECT3DTEXTURE9 lpSrcTexture = NULL;

	char NewName2[256];

    /*
     * Release the surface if it is hanging around
     */

	//LPDIRECT3DTEXTURE9  lpTexture[MAXTPAGESPERTLOAD]; /* texture objs */

    RELEASE(Tloadheader->lpTexture[n]);

    /*
     * Create a surface in system memory and load the PPM file into it.
     * Query for the texture interface.
     */
	if ( !Tloadheader->PlaceHolder[ n ] )
	{
		// change extension of file name
		Change_Ext( &Tloadheader->ImageFile[n][0], &NewName2[ 0 ], ".BMP" );

		// if file exists
		if( File_Exists( &NewName2[ 0 ] ) )
		{
			if( Tloadheader->MipMap[n] )
			{
				FSCreateTexture(&lpSrcTexture, &NewName2[0], 0, 0, 0, &Tloadheader->ColourKey[n]);
			}
			else
			{
				FSCreateTexture(&lpSrcTexture, &NewName2[0], 0, 0, 1, &Tloadheader->ColourKey[n]);
			}
		}
	}

	Tloadheader->lpTexture[n] = lpSrcTexture;
	lpSrcTexture = NULL;

	return TRUE;
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

	if ( Tloadheader->PlaceHolder[ n ] )
	{
		free( Tloadheader->PlaceHolderFile[ n ] );
		Tloadheader->PlaceHolderFile[ n ] = NULL;
	}

//    Tloadheader->hTex[n] = 0;
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
//		RELEASE(Tloadheader->lpMat[i]);
    }

	InitTload( Tloadheader  );
  
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
    for (i = 0; i < Tloadheader->num_texture_files; i++)
        ATTEMPT(TloadTextureSurf( Tloadheader , i));
    return TRUE;

exit_with_error:
    for ( ; i < Tloadheader->num_texture_files; i++) {
        TloadReleaseTexture( Tloadheader , i);
    }
    return FALSE;
}

/*===================================================================
	Procedure	:		Get the width and height for a PPM
	Input		:		TLOADHEADER * , int , lpname, uint16 * , uint16 * 
	Output		:		BOOL FALSE/TRUE
===================================================================*/
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


/*===================================================================
	Procedure	:		Find a texture from a tloadheader Ignores the Path
	Input		:		TLOADHEADER * , char * Name
	Output		:		-1 if no match found otherwise number of texture
===================================================================*/

int16	FindTexture( TLOADHEADER * Tloadheader , char * Name )
{
	int	i;
	char * pnt1;
	char * pnt2;

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
/*===================================================================
	Procedure	:		Add a texture to a tloadheader
	Input		:		TLOADHEADER * , char * Name , ColourKey
	Output		:		-1 if too many tpages
===================================================================*/

int16	AddTexture( TLOADHEADER * Tloadheader , char * Name , uint16 ColourKey  , BOOL Scale , BOOL MipMap, int16 xsize, int16 ysize )
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

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

#endif