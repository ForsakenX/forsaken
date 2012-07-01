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
	u_int16_t num_texture_files;
	u_int16_t				Xsize[MAXTPAGESPERTLOAD];		// the Xsize now
	u_int16_t				Ysize[MAXTPAGESPERTLOAD];		// the Ysize now
	_Bool				Scale[MAXTPAGESPERTLOAD];		// Can I Be Scaled....???
	u_int16_t				CurScale[MAXTPAGESPERTLOAD];	// 0 normal...1 X half 2 X and Y half..
	_Bool				ColourKey[MAXTPAGESPERTLOAD];	// 0 not colour keyed
    char                ImageFile[MAXTPAGESPERTLOAD][256]; /* files */
    LPTEXTURE			lpTexture[MAXTPAGESPERTLOAD]; /* texture objs */
	_Bool				MipMap[MAXTPAGESPERTLOAD];
	_Bool				PlaceHolder[MAXTPAGESPERTLOAD];	// is the texture a placeholder ( for subsequent dynamic loading of textures? )
	char				*PlaceHolderFile[MAXTPAGESPERTLOAD];	// current full path of file occupying placeholder ( needed to restore surface )
	int					LOD[MAXTPAGESPERTLOAD];
}TLOADHEADER;

/*
 * fn prototypes
 */
_Bool InitTload( TLOADHEADER * Tloadheader  );
_Bool Tload( TLOADHEADER * Tloadheader );
_Bool TloadGetTextureHandle(TLOADHEADER * Tloadheader , int n);
void TloadReleaseTexture(TLOADHEADER * Tloadheader, int n);
void ReleaseTloadheader( TLOADHEADER * Tloadheader );

_Bool TloadAllTextures(TLOADHEADER * Tloadheader);

int16_t	FindTexture( TLOADHEADER * Tloadheader , char * Name );

void GetLevelTexturePath( char * destination, char * file, char * level );
int16_t	AddTexture( TLOADHEADER * Tloadheader , char * Name , u_int16_t ColourKey , _Bool Scale , _Bool MipMap, int16_t xsize, int16_t ysize );
int16_t	FindTexture( TLOADHEADER * Tloadheader , char * Name );

_Bool TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16_t n );

#endif	//TLOAD_INCLUDED

#endif
