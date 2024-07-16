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

#include "main.h"
#include "tload.h"
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

extern	bool	TexturesEnabled;

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
	Output		:		bool false/true
===================================================================*/
bool InitTload( TLOADHEADER * Tloadheader  )
{
	int i;
	memset( Tloadheader, 0, sizeof(TLOADHEADER) );
	Tloadheader->num_texture_files = 0;
	for( i = 0 ; i < MAXTPAGESPERTLOAD ; i++ )
	{
		Tloadheader->lpTexture[i]     = NULL; // texture
		Tloadheader->CurScale[i]      = 0;	  // handle
		Tloadheader->Scale[i]		  = false;// Should it scale??

		Tloadheader->MipMap[i]		  = false;// Should it have Mip Maps
		Tloadheader->PlaceHolder[i]	  = false;// Is it a placeholder ( for subsequent dynamicly loaded textures )
		Tloadheader->LOD[i]			  = false;// How many Levels of Mip Map

		Tloadheader->Xsize[ i ] = 0;		
		Tloadheader->Ysize[ i ] = 0;
		
		Tloadheader->PlaceHolderFile[ i ] = NULL;
	}
	return	true;
}


/*===================================================================
	Procedure	:		Load All textures associated with a level
	Input		:		TLOADHEADER *
	Output		:		bool false/true
===================================================================*/
bool Tload( TLOADHEADER * Tloadheader  )
{
	int	i,e;
	int LeastScaledThatCanbe;
	int LeastScaledThatCanbeScale;

	build_gamma_table( Gamma );
 
	// Tloadheader is not valid until everything has been done..
	Tloadheader->state = false;

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
		if ( TloadAllTextures( Tloadheader ) != true)
		{
			Msg( "TLoadAllTextures() Failed\n" );
			return false;
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
	Tloadheader->state = true;

	return( true );
}
	
//
// this will create and set a brand new texture pointer
//

bool TloadTextureSurf( TLOADHEADER * Tloadheader , int n)
{
	LPTEXTURE lpSrcTexture = NULL;
	release_texture(Tloadheader->lpTexture[n]);
	Tloadheader->lpTexture[n] = NULL;
	if ( !Tloadheader->PlaceHolder[ n ] )
	{
		if( Tloadheader->MipMap[n] )
		{
			FSCreateTexture(
				&lpSrcTexture, &Tloadheader->ImageFile[n][0],
				&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
				0, &Tloadheader->ColourKey[n]);
		}
		else
		{
			FSCreateTexture(
				&lpSrcTexture, &Tloadheader->ImageFile[n][0],
				&Tloadheader->Xsize[n], &Tloadheader->Ysize[n],
				1, &Tloadheader->ColourKey[n]);
		}
	}
	Tloadheader->lpTexture[n] = lpSrcTexture;
	lpSrcTexture = NULL;

	return true;
}

//
// this will either create a new texture pointer if it doesn't exist
// or reload a new surface while keeping the pointer the same
//

bool TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16_t n )
{
	// create only one mipmap level (original texture only)
	int numMips = 1;

	// we only work on place holders
	if( !Tloadheader->PlaceHolderFile[n] || !Tloadheader->PlaceHolderFile[n][0] )
		return false;

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

	return true;
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
	Tloadheader->lpTexture[n] = NULL;
	if ( Tloadheader->PlaceHolder[ n ] )
	{
		free( Tloadheader->PlaceHolderFile[ n ] );
		Tloadheader->PlaceHolderFile[ n ] = NULL;
	}
}

/*
 * ReleaseTloadheader
 * Release all texture surfaces and texture interfaces
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
    InitTload( Tloadheader );
}

bool TloadAllTextures(TLOADHEADER * Tloadheader)
{
    int i;

    for (i = 0; i < Tloadheader->num_texture_files; i++)
	{
        if(!TloadTextureSurf( Tloadheader, i ))
		{
			goto exit_with_error;
		}
	}

    return true;

exit_with_error:
    for ( ; i < Tloadheader->num_texture_files; i++) {
        TloadReleaseTexture( Tloadheader , i);
    }
    return false;
}

/*===================================================================
	Procedure	:		Find a texture from a tloadheader Ignores the Path
	Input		:		TLOADHEADER * , char * Name
	Output		:		-1 if no match found otherwise number of texture
===================================================================*/

int16_t	FindTexture( TLOADHEADER * Tloadheader , char * Name )
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

		if( strcasecmp( pnt1, pnt2 ) == 0 )
			return i;

//		if( strcasecmp( (char*) Name, (char*) &Tloadheader->ImageFile[i] ) == 0 )
//			return i;
	}
	return -1;
}
/*===================================================================
	Procedure	:		Add a texture to a tloadheader
	Input		:		TLOADHEADER * , char * Name , ColourKey
	Output		:		-1 if too many tpages
===================================================================*/

int16_t	AddTexture( TLOADHEADER * Tloadheader , char * Name , u_int16_t ColourKey  , bool Scale , bool MipMap, int16_t xsize, int16_t ysize )
{
	int16_t i;
	char * NamePnt;

	// are we just loading placeholder?
	if ( !Name[ 0 ] )
	{
	 	i = Tloadheader->num_texture_files;
		Tloadheader->ColourKey[i] = ColourKey;
		Tloadheader->Scale[i] = false;		// cannot allow scaling of placeholder!
		Tloadheader->MipMap[i] = MipMap;
		Tloadheader->ImageFile[i][0] = 0;
		Tloadheader->PlaceHolder[i] = true;
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
		Tloadheader->PlaceHolder[i] = false;
	Tloadheader->num_texture_files++;
	return i;
}

// Allows consolodating commonly used texture files
// use texture in level folder as the ultimate source
// if it doesn't exist then look for it in data/textures
// or fall back to default.png
// also pkupsa?.bmp is hard coded to map to titana.png as the old game did
// size of destination should always be 256, path should end up at least 256-1
bool use_original_textures = false;
void GetLevelTexturePath( char * destination, char * file, char * level )
{
	char path[256];
	char new_file[256];

	// pkups pkup[sa] is really titana (old game hard coded it that way)
	if( !strcasecmp( "pkups.bmp", file ) || !strcasecmp( "pkupsa.bmp", file ) )
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

	// use old textures if requested and available
	sprintf(path,"data\\textures\\original\\%s",new_file);
	if( use_original_textures && File_Exists( path ) )
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
