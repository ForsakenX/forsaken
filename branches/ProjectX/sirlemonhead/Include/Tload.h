#if 1 //bjd
/*==========================================================================
 *
 *
 *  File:       Tload.h
 *  Content:    Tload include file
 *
 *
 ***************************************************************************/

#ifndef TLOAD_INCLUDED
#define TLOAD_INCLUDED

#include "d3dmain.h"
#include "d3ddemo.h"
#include "typedefs.h"
#include "file.h"
#include "exechand.h"
#include "d3dappi.h"
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
typedef struct TLOADNAME{
	char	Name[32];
	uint16	ColourKey;
}TLOADNAME;



typedef struct TLOADHEADER{
	uint16	CurrentBPP;
	DWORD	VidMemBefore;
	DWORD	VidMemEstimate;
	DWORD	VidMemAfter;
	int		state;
	uint16 num_texture_files;
	uint16				Xsize[MAXTPAGESPERTLOAD];		// the Xsize now
	uint16				Ysize[MAXTPAGESPERTLOAD];		// the Ysize now
	BOOL				Scale[MAXTPAGESPERTLOAD];		// Can I Be Scaled....???
	uint16				CurScale[MAXTPAGESPERTLOAD];	// 0 normal...1 X half 2 X and Y half..
	uint16				ColourKey[MAXTPAGESPERTLOAD];	// 0 not colour keyed
	DWORD				SizeInVidMem[MAXTPAGESPERTLOAD]; // Calculated size in video memory....
    char                ImageFile[MAXTPAGESPERTLOAD][256]; /* files */
//    LPDIRECTDRAWSURFACE lpTextureSurf[MAXTPAGESPERTLOAD]; /* surfaces */
    LPDIRECT3DTEXTURE9  lpTexture[MAXTPAGESPERTLOAD]; /* texture objs */
	D3DMATERIAL9		lpMat[MAXTPAGESPERTLOAD];
/*
    D3DTEXTUREHANDLE    hTex[MAXTPAGESPERTLOAD]; // handles
	LPDIRECT3DMATERIAL	lpMat[MAXTPAGESPERTLOAD];
	D3DMATERIALHANDLE	hMat[MAXTPAGESPERTLOAD];
*/
    BOOL                bTexturesInVideo[MAXTPAGESPERTLOAD]; /* are textures in video memory? */
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
//LPDIRECTDRAWSURFACE TloadSurface(LPDIRECTDRAW lpDD, LPCSTR lpName,
//                   LPDDSURFACEDESC lpFormat, DWORD memoryflag);

BOOL
TloadGetStats( TLOADHEADER * Tloadheader , int i ,LPCSTR lpName , uint16 * Width , uint16 * Height );
int16	FindTexture( TLOADHEADER * Tloadheader , char * Name );
//LPDIRECTDRAWSURFACE CreateTextureSurf(LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpFormat, DWORD memoryflag, DWORD dwWidth, DWORD dwHeight);
//LPDIRECTDRAWSURFACE TloadSurfaceScale( LPDIRECTDRAW lpDD, LPCSTR lpName,
 //                  LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale );

int16	AddTexture( TLOADHEADER * Tloadheader , char * Name , uint16 ColourKey , BOOL Scale , BOOL MipMap, int16 xsize, int16 ysize );
int16	FindTexture( TLOADHEADER * Tloadheader , char * Name );
BOOL
TloadCheckForLostSurfaces(TLOADHEADER * Tloadheader);
BOOL TloadReloadTextureSurf(TLOADHEADER * Tloadheader , int16 n);

BOOL TloadBlankTextureSurf(TLOADHEADER * Tloadheader , int16 n);

//LPDIRECTDRAWSURFACE TloadSurfaceScale8BitPrimary( LPDIRECTDRAW lpDD, LPCSTR lpName,
//                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale );
//BOOL InitCopyDDSurfaceToTextureSurfaces ( LPDIRECTDRAWSURFACE lpDDS_source, LPDIRECTDRAWSURFACE lpDDS_dest1, LPDIRECTDRAWSURFACE lpDDS_dest2 );
//BOOL CopyDDSurfaceToTextureSurfaces ( LPDIRECTDRAWSURFACE lpDDS_source, LPDIRECTDRAWSURFACE lpDDS_dest1, LPDIRECTDRAWSURFACE lpDDS_dest2 );
BOOL TloadReloadPlaceHolder( TLOADHEADER *Tloadheader, int16 n );

#endif	//TLOAD_INCLUDED

#endif