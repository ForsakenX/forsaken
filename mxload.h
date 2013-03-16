/*==========================================================================
 *
 *
 *  File:       mxload.h
 *  Content:    Mxload include file
 *
 *
 ***************************************************************************/

#ifndef MXLOAD_INCLUDED
#define MXLOAD_INCLUDED

#include "main.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "tload.h"
#include "mload.h"
#include "transexe.h"

/*
 * defines
 */
#define	MAXMXGROUPS 1
#define MAXMXATEXTUREGROUPSPER 8
#define	MAXTPAGESPERMXLOAD		8

enum {

	PV_FirePoint,		// 0
	PV_SpotFX,			// 1

};

 /*
 * structures
 */

typedef struct PVFIREPOINT{

	u_int16_t		ID;
	VECTOR		Pos;
	VECTOR		Dir;
	VECTOR		Up;

} PVFIREPOINT;

typedef struct PVSPOTFX {

	u_int16_t		Type;
	u_int16_t		Group;
	int8_t		Primary;
	int8_t		Secondary;
	u_int8_t		Red;
	u_int8_t		Green;
	u_int8_t		Blue;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	float		StartDelay;
	float		ActiveDelay;
	float		InactiveDelay;
	int16_t		SoundFX;
	float		SoundFXVolume;
	float		SoundFXSpeed;

} PVSPOTFX;


typedef struct MXGROUP{
	u_int16_t		num_execbufs;
	u_int16_t		exec_type[MAXEXECBUFSPERGROUP];
	u_int16_t		num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
//	COLOR *	org_colors[MAXEXECBUFSPERGROUP];
	u_int16_t	num_polys_per_execbuf[MAXEXECBUFSPERGROUP];
	u_int16_t	num_texture_groups[MAXMXATEXTUREGROUPSPER];
//	LPDIRECT3DEXECUTEBUFFER	lpExBuf[MAXEXECBUFSPERGROUP];
	RENDEROBJECT renderObject[MAXMXATEXTUREGROUPSPER];
	LPLVERTEX originalVerts[MAXEXECBUFSPERGROUP];

	LPTRIANGLE poly_ptr	[MAXEXECBUFSPERGROUP];
	u_int32_t		texture_group_vert_off[MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];
	char		name[16];
	u_int16_t		num_animating_polys[MAXEXECBUFSPERGROUP];
	POLYANIM *	polyanim[MAXEXECBUFSPERGROUP];

}MXGROUP;


typedef struct MXLOADHEADER{
	int					state;								// false if not loaded properly...
	u_int32_t				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
	char		*		OrgAddr;
	char		*		Buffer;
	u_int16_t				num_texture_files;
	u_int16_t				num_groups;
	MXGROUP				Group[MAXMXGROUPS];
	int16_t				TloadIndex[MAXTPAGESPERMXLOAD];		// which texture in the Tloadheader....
    char                ImageFile[MAXTPAGESPERMXLOAD][32];	// Texture Files....
	ANIMDATA			AnimData;
	u_int16_t				NumFirePoints;						// Number of FirePoints
	PVFIREPOINT	*		FirePoints;							// FirePoint Stuff
	u_int16_t				NumSpotFX;							// Number of SpotFX
	PVSPOTFX	*		SpotFX;								// SpotFX Stuff
	int					LOD;
	VECTOR				Center;								// Center Pos
	VECTOR				Sizes;								// X,Y,Z Sizes
}MXLOADHEADER;

/*
 * fn prototypes
 */
bool PreMxload( char * Filename, MXLOADHEADER * Mxloadheader , bool Panel, bool LevelSpecific );
bool Mxload( char * Filename, MXLOADHEADER * Mxloadheader , bool Panel, bool StoreTriangles );
bool ExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, u_int16_t Model );

void ReleaseMxloadheader( MXLOADHEADER * Mxloadheader );

bool ExecuteSingleGroupMxloadHeader( MXLOADHEADER * Mxloadheader, u_int16_t group  );
bool ReleaseMxModels();
bool ReallyExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, u_int16_t Model );

void ModelTextureAnimation( MXLOADHEADER * Mxloadheader  );
bool RestoreColourMxloadHeader( MXLOADHEADER * Mxloadheader1 );
#endif


