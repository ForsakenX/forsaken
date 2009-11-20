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

#include "tload.h"
#include "xmem.h"
#include "util.h"
#include "stdio.h"

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
		memset(&Tloadheader->lpMat[i], 0, sizeof(RENDERMATERIAL));
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
void render_gamma_correction( double gamma );
BOOL Tload( TLOADHEADER * Tloadheader  )
{
	int	i,e;
	int LeastScaledThatCanbe;
	int LeastScaledThatCanbeScale;

	render_gamma_correction( Gamma );
 
	// Tloadheader is not valid until everything has been done..
	Tloadheader->state = FALSE;

	// allocate space for placeholder file names
	for( i = 0 ; i < Tloadheader->num_texture_files ; i ++ )
	{
		if ( Tloadheader->PlaceHolder[ i ] )
		{
			if(!Tloadheader->PlaceHolderFile[ i ])
				Tloadheader->PlaceHolderFile[ i ] = ( char * )malloc( sizeof( char ) * 256 );
			DebugPrintf( "Tload: Placeholder ( texture %2d )\n", i );
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
	RENDERMATERIAL mat;

    int i;
	/*	create a default material for each texture */
	memset(&mat, 0, sizeof(RENDERMATERIAL));
	mat.Diffuse.r = (float)1.0;
	mat.Diffuse.g = (float)1.0;
	mat.Diffuse.b = (float)1.0;
	mat.Diffuse.a = (float)1.0;
	mat.Ambient.r = (float)1.0;
	mat.Ambient.g = (float)1.0;
	mat.Ambient.b = (float)1.0;
	mat.Specular.r = (float)0.0;
	mat.Specular.g = (float)0.0;
	mat.Specular.b = (float)0.0;
	mat.Power = (float)0.0;

	for (i = 0; i<Tloadheader->num_texture_files; i++)
	{
		Tloadheader->lpMat[i] = mat;

		if (!FSSetMaterial(&mat))
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

//
// this will create and set a brand new texture pointer
//

BOOL TloadTextureSurf( TLOADHEADER * Tloadheader , int n)
{
    LPTEXTURE lpSrcTexture = NULL;
    release_texture(Tloadheader->lpTexture[n]);
	//Tloadheader->lpTexture[n] = NULL;
	if ( !Tloadheader->PlaceHolder[ n ] )
		if( Tloadheader->MipMap[n] )
			FSCreateTexture(
				&lpSrcTexture, &Tloadheader->ImageFile[n][0],
				&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
				0, &Tloadheader->ColourKey[n]);
		else
			FSCreateTexture(
				&lpSrcTexture, &Tloadheader->ImageFile[n][0],
				&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
				1, &Tloadheader->ColourKey[n]);

	Tloadheader->lpTexture[n] = lpSrcTexture;
	lpSrcTexture = NULL;
	return TRUE;
}

//
// this will either create a new texture pointer if it doesn't exist
// or reload a new surface while keeping the pointer the same
//

BOOL TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16 n )
{
	// create only one mipmap level (original texture only)
	int numMips = 1;

	// we only work on place holders
	if( !Tloadheader->PlaceHolderFile[n] || !Tloadheader->PlaceHolderFile[n][0] )
		return FALSE;

	// let d3d generate mipmaps for us that are needed
	if(Tloadheader->MipMap[n])
		numMips = 0;

	// for some reason level pic would not show up properly if you let update_texture_from_file create the texture for us
	if(!Tloadheader->lpTexture[n])
		FSCreateTexture(
			&Tloadheader->lpTexture[n], &Tloadheader->PlaceHolderFile[n][0],
			&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
			numMips, &Tloadheader->ColourKey[n]);
	else
		update_texture_from_file(
			Tloadheader->lpTexture[n], &Tloadheader->PlaceHolderFile[n][0],
			&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
			numMips, &Tloadheader->ColourKey[n]);

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
    release_texture(Tloadheader->lpTexture[n]);
	if ( Tloadheader->PlaceHolder[ n ] )
	{
		free( Tloadheader->PlaceHolderFile[ n ] );
		Tloadheader->PlaceHolderFile[ n ] = NULL;
	}
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
    }

	InitTload( Tloadheader  );
  
}

BOOL TloadAllTextures(TLOADHEADER * Tloadheader)
{
    int i;

    for (i = 0; i < Tloadheader->num_texture_files; i++)
	{
        if(!TloadTextureSurf( Tloadheader, i ))
		{
			goto exit_with_error;
		}
	}

    return TRUE;

exit_with_error:
    for ( ; i < Tloadheader->num_texture_files; i++) {
        TloadReleaseTexture( Tloadheader , i);
    }
    return FALSE;
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
			
	DebugPrintf("Added Texture: %s\n",Name);

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

// Allows consolodating commonly used texture files
// use texture in level folder as the ultimate source
// if it doesn't exist then look for it in data/textures
// or fall back to default.png
// also pkupsa?.bmp is hard coded to map to titana.png as the old game did
// size of destination should always be 256, path should end up at least 256-1
void GetLevelTexturePath( char * destination, char * file, char * level )
{
	char path[256];
	char new_file[256];

	// pkups pkup[sa] is really titana (old game hard coded it that way)
	if( !_stricmp( "pkups.bmp", file ) || !_stricmp( "pkupsa.bmp", file ) )
	{
		strncpy(destination,"data\\textures\\titana.png",256);
		return;
	}

	// convert file extension to new standard
	Change_Ext( file, new_file, ".PNG" );

	// check for level folder override
	sprintf(path,"data\\levels\\%s\\textures\\%s",level,new_file);
	if( File_Exists( path ) )
	{
		strncpy(destination,path,256);
		return;
	}

	// default to data/textures
	sprintf(path,"data\\textures\\%s",new_file);
	if( File_Exists( path ) )
	{
		strncpy(destination,path,256);
		return;
	}

	// we can't find it so set it to default for safety
	strncpy(file,"data\\textures\\defaults.png",sizeof(destination));
	DebugPrintf("ERROR: GetTexturePath() could not find %s\n",new_file);
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

#endif