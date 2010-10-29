#if 1 //bjd
/*==========================================================================
 *
 *
 *  File:       tload.h
 *  Content:    Tload include file
 *
 *
 ***************************************************************************/

#ifndef TLOAD_INCLUDED
#define TLOAD_INCLUDED

#include "file.h"
#include "render.h"
#include "new3d.h"

/*
 * defines
 */
#define MAXTPAGESPERTLOAD 50
#define SCALEMAX 2

#define	DONTLOAD_TPAGES				0
#define	LOAD_TPAGES					1
#define	LOAD_TPAGES_PLACEHOLDER		4
#define	LOAD_TPAGES_FINISH			8

/*
 * structures
 */

typedef struct TLOADHEADER{
	int		state;
	uint16 num_texture_files;
	uint16				Xsize[MAXTPAGESPERTLOAD];		// the Xsize now
	uint16				Ysize[MAXTPAGESPERTLOAD];		// the Ysize now
	BOOL				Scale[MAXTPAGESPERTLOAD];		// Can I Be Scaled....???
	uint16				CurScale[MAXTPAGESPERTLOAD];	// 0 normal...1 X half 2 X and Y half..
	BOOL				ColourKey[MAXTPAGESPERTLOAD];	// 0 not colour keyed
    char                ImageFile[MAXTPAGESPERTLOAD][256]; /* files */
    LPTEXTURE			lpTexture[MAXTPAGESPERTLOAD]; /* texture objs */
	RENDERMATERIAL		lpMat[MAXTPAGESPERTLOAD];
	BOOL				MipMap[MAXTPAGESPERTLOAD];
	BOOL				PlaceHolder[MAXTPAGESPERTLOAD];	// is the texture a placeholder ( for subsequent dynamic loading of textures? )
	char				*PlaceHolderFile[MAXTPAGESPERTLOAD];	// current full path of file occupying placeholder ( needed to restore surface )
	int					LOD[MAXTPAGESPERTLOAD];
}TLOADHEADER;

/*
 * fn prototypes
 */
BOOL InitTload( TLOADHEADER * Tloadheader  );
BOOL Tload( TLOADHEADER * Tloadheader );
BOOL TloadCreateMaterials( TLOADHEADER * Tloadheader );
BOOL TloadGetTextureHandle(TLOADHEADER * Tloadheader , int n);
void TloadReleaseTexture(TLOADHEADER * Tloadheader, int n);
void ReleaseTloadheader( TLOADHEADER * Tloadheader );

BOOL TloadAllTextures(TLOADHEADER * Tloadheader);

int16	FindTexture( TLOADHEADER * Tloadheader , char * Name );

void GetLevelTexturePath( char * destination, char * file, char * level );
int16	AddTexture( TLOADHEADER * Tloadheader , char * Name , uint16 ColourKey , BOOL Scale , BOOL MipMap, int16 xsize, int16 ysize );
int16	FindTexture( TLOADHEADER * Tloadheader , char * Name );

BOOL TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16 n );

#endif	//TLOAD_INCLUDED

#endif
