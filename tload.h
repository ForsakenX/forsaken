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
	bool				Scale[MAXTPAGESPERTLOAD];		// Can I Be Scaled....???
	u_int16_t				CurScale[MAXTPAGESPERTLOAD];	// 0 normal...1 X half 2 X and Y half..
	bool				ColourKey[MAXTPAGESPERTLOAD];	// 0 not colour keyed
    char                ImageFile[MAXTPAGESPERTLOAD][256]; /* files */
    LPTEXTURE			lpTexture[MAXTPAGESPERTLOAD]; /* texture objs */
	bool				MipMap[MAXTPAGESPERTLOAD];
	bool				PlaceHolder[MAXTPAGESPERTLOAD];	// is the texture a placeholder ( for subsequent dynamic loading of textures? )
	char				*PlaceHolderFile[MAXTPAGESPERTLOAD];	// current full path of file occupying placeholder ( needed to restore surface )
	int					LOD[MAXTPAGESPERTLOAD];
}TLOADHEADER;

/*
 * fn prototypes
 */
bool InitTload( TLOADHEADER * Tloadheader  );
bool Tload( TLOADHEADER * Tloadheader );
bool TloadGetTextureHandle(TLOADHEADER * Tloadheader , int n);
void TloadReleaseTexture(TLOADHEADER * Tloadheader, int n);
void ReleaseTloadheader( TLOADHEADER * Tloadheader );

bool TloadAllTextures(TLOADHEADER * Tloadheader);

int16_t	FindTexture( TLOADHEADER * Tloadheader , char * Name );

void GetLevelTexturePath( char * destination, char * file, char * level );
int16_t	AddTexture( TLOADHEADER * Tloadheader , char * Name , u_int16_t ColourKey , bool Scale , bool MipMap, int16_t xsize, int16_t ysize );
int16_t	FindTexture( TLOADHEADER * Tloadheader , char * Name );

bool TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16_t n );

extern bool use_original_textures;

#endif	//TLOAD_INCLUDED

#endif
