/*==========================================================================
 *
 *
 *  File:       Mxload.h
 *  Content:    Mxload include file
 *
 *
 ***************************************************************************/

#ifndef MXLOAD_INCLUDED
#define MXLOAD_INCLUDED

#include <d3d9.h>
#include "typedefs.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "Tload.h"
#include "Mload.h"
#include "Transexe.h"

/*
 * defines
 */
#define	MAXMXGROUPS 1
#define MAXTEXTUREGROUPSPEREXECLIST 8
#define	MAXTPAGESPERMXLOAD		8

enum {

	PV_FirePoint,		// 0
	PV_SpotFX,			// 1

};

 /*
 * structures
 */

typedef struct PVFIREPOINT{

	uint16		ID;
	VECTOR		Pos;
	VECTOR		Dir;
	VECTOR		Up;

} PVFIREPOINT;

typedef struct PVSPOTFX {

	uint16		Type;
	uint16		Group;
	int8		Primary;
	int8		Secondary;
	uint8		Red;
	uint8		Green;
	uint8		Blue;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	float		StartDelay;
	float		ActiveDelay;
	float		InactiveDelay;
	int16		SoundFX;
	float		SoundFXVolume;
	float		SoundFXSpeed;

} PVSPOTFX;


typedef struct MXGROUP{
	uint16		num_execbufs;
	uint16		exec_type[MAXEXECBUFSPERGROUP];
	uint16		num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
//	D3DCOLOR *	org_colors[MAXEXECBUFSPERGROUP];
	uint16	num_polys_per_execbuf[MAXEXECBUFSPERGROUP];
	uint16	num_texture_groups[MAXTEXTUREGROUPSPEREXECLIST];
//	LPDIRECT3DEXECUTEBUFFER	lpExBuf[MAXEXECBUFSPERGROUP];
	RENDEROBJECT renderObject[MAXTEXTUREGROUPSPEREXECLIST];
	LPD3DLVERTEX originalVerts[MAXEXECBUFSPERGROUP];

	LPD3DTRIANGLE poly_ptr	[MAXEXECBUFSPERGROUP];
	uint32		texture_group_vert_off[MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];
	char		name[16];
	uint16		num_animating_polys[MAXEXECBUFSPERGROUP];
	POLYANIM *	polyanim[MAXEXECBUFSPERGROUP];

}MXGROUP;


typedef struct MXLOADHEADER{
	int					state;								// FALSE if not loaded properly...
	uint32				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
	char		*		OrgAddr;
	char		*		Buffer;
	uint16				num_texture_files;
	uint16				num_groups;
	MXGROUP				Group[MAXMXGROUPS];
	int16				TloadIndex[MAXTPAGESPERMXLOAD];		// which texture in the Tloadheader....
    char                ImageFile[MAXTPAGESPERMXLOAD][32];	// Texture Files....
	ANIMDATA			AnimData;
	uint16				NumFirePoints;						// Number of FirePoints
	PVFIREPOINT	*		FirePoints;							// FirePoint Stuff
	uint16				NumSpotFX;							// Number of SpotFX
	PVSPOTFX	*		SpotFX;								// SpotFX Stuff
	int					LOD;
	VECTOR				Center;								// Center Pos
	VECTOR				Sizes;								// X,Y,Z Sizes
}MXLOADHEADER;

/*
 * fn prototypes
 */
BOOL PreMxload( char * Filename, MXLOADHEADER * Mxloadheader , BOOL Panel, BOOL LevelSpecific );
BOOL Mxload( char * Filename, MXLOADHEADER * Mxloadheader , BOOL Panel, BOOL StoreTriangles );
BOOL ExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, uint16 Model );

void ReleaseMxloadheader( MXLOADHEADER * Mxloadheader );

BOOL ExecuteSingleGroupMxloadHeader( MXLOADHEADER * Mxloadheader, uint16 group  );
BOOL ReleaseMxModels();
BOOL ReallyExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, uint16 Model );

void ModelTextureAnimation( MXLOADHEADER * Mxloadheader  );
BOOL RestoreColourMxloadHeader( MXLOADHEADER * Mxloadheader1 );
#endif


