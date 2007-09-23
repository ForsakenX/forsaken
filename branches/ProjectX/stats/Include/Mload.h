/*==========================================================================
 *
 *
 *  File:       Mload.h
 *  Content:    Mload include file
 *
 *
 ***************************************************************************/

#ifndef MLOAD_INCLUDED
#define MLOAD_INCLUDED

#include "d3dmain.h"
#include "d3ddemo.h"
#include "typedefs.h"
#include "file.h"
#include "exechand.h"
#include "d3dappi.h"
#include "new3d.h"
#include "Tload.h"
#include "Transexe.h"

/*
 * defines
 */
//#define	CELLSIZE ( 2048.0F * GLOBAL_SCALE )
#define	CELLSIZE ( 256.0F )
#define	BLOCKSIZE		( 1024.0F * GLOBAL_SCALE )

#define MAXTPAGESPERMLOAD 8
#define MAXGROUPS			128
#define MAXEXECBUFSPERGROUP 16
#define MAXPORTALSPERGROUP	64
#define MAXVERTSPERPORTAL	32
#define MAXPOLYSPERPORTAL	32
#define MAXVERTSPERPORTALPOLY	4
#define MAXGROUPSVISIBLEPERPORTAL 64
#define MAXSTARTPOSITIONS	MAXGROUPS
#define HASTRANSPARENCIES	0x001
#define HASENVIROMENTMAP	0x002


#define MAXPOLYANIMSPEREXECBUF 64
#define MAXANIMSEQS		128
#define	MAXANIMLOOPSTACK 8
#define TEXTUREANIM_Stop	0
#define TEXTUREANIM_Go		1


#define TEXANIM_COMMAND		(0x8000) // (top bit set if command otherwise frame number)
#define TEXANIM_DURATION	(0x8001) // default frame duration
#define TEXANIM_DURATION1	(0x8002) // next frame duration only
#define TEXANIM_LOOPINIT	(0x8003) // loop var, number of repeats
#define TEXANIM_LOOPEND		(0x8004) // loop var, start of loop
#define TEXANIM_END			(0xFFFF) // (end of animation)

#define	LEVEL_SPECIFIC			TRUE
#define	NOT_LEVEL_SPECIFIC		FALSE

/*
 * structures
 */

typedef struct  {
	float u, v;
} PROJPOINT;



typedef struct MCFACE {
	uint32 type;
	float nx;
	float ny;
	float nz;
	float D;
	PROJPOINT v[ 4 ];
} MCFACE;


typedef struct VERTEXCELL{
	uint16	num_verts_in_cell;
	uint16	start_vert_in_cell;
}VERTEXCELL;


typedef struct STARTPOS{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	Up;
	uint16	Group;
}STARTPOS;

typedef struct _GAMESTARTPOS{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	Up;
	uint16	Group;
	uint16	NextInGroup;
}GAMESTARTPOS;

typedef struct PORTALPOLY
{
	uint16 num_vertices_in_poly;
	VERT   Verts[MAXVERTSPERPORTALPOLY];
}PORTALPOLY;


typedef struct VISTREE VISTREE;

typedef struct PORTAL PORTAL;


typedef struct VISTREE
{
	PORTAL *portal;
	uint16 group;
	uint16 num_visible;
	VISTREE *visible;
}VISTREE;



typedef struct PORTAL{
	uint16 num_vertices_in_portal;
	VERT   Verts[MAXVERTSPERPORTAL];
	uint16 num_polys_in_portal;
	MCFACE Poly[MAXPOLYSPERPORTAL];
	VISTREE visible;
}PORTAL;


typedef struct  {
	float u, v;
} TANIMUV;


typedef struct  {
	uint16	State;
	uint16	animation;
	float	CurrentTime;
	float	MasterTime;
	uint16	CurrentOffset;
	uint16	currentframe;	// what we are displaying now...
	uint16	newframe;		// what we want ti display..
	uint16	frames;			// number of frames
	uint16	vertices;		// number of verts...
	uint16	loopstack[MAXANIMLOOPSTACK];
	int	*	vert;
	TANIMUV	* UVs;
	char	Tag[32];
	int16	WhenStoppedTriggerMod;
} POLYANIM;

typedef struct _TEXTUREANIMINFOINDEX{
	uint16	Group;
	uint16	Execbuf;
	uint16	PolyAnim;
	uint16	InitState;
}TEXTUREANIMINFOINDEX;


typedef struct  {
	uint16	num_animating_polys;
TEXTUREANIMINFOINDEX * TAnimInfoIndex;
	uint16	num_animations;
	uint16 * AnimSeq[MAXANIMSEQS];
}ANIMDATA;


typedef struct GROUP{
	uint16	num_execbufs;
	uint16	exec_type[MAXEXECBUFSPERGROUP];
	uint16	num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
	LPDIRECT3DEXECUTEBUFFER	lpExBuf[MAXEXECBUFSPERGROUP];
	LPD3DLVERTEX org_vertpnt[MAXEXECBUFSPERGROUP];
	char	name[32];
	VERT	center;
	VERT	half_size;
	VECTOR	up;
	uint16	num_portals;
	PORTAL  * Portal;
	int16	BGClear_Flag;
	float	BGClear_Red;
	float	BGClear_Green;
	float	BGClear_Blue;

	VECTOR	cell_origin[MAXEXECBUFSPERGROUP];
	uint16	xcells[MAXEXECBUFSPERGROUP];
	uint16	ycells[MAXEXECBUFSPERGROUP];
	uint16	zcells[MAXEXECBUFSPERGROUP];
	uint16	numofcells[MAXEXECBUFSPERGROUP];
	uint16	num_vertex_indices[MAXEXECBUFSPERGROUP];
	uint16 * vertex_index_pnt[MAXEXECBUFSPERGROUP];
	VERTEXCELL * vertex_cell_pnt[MAXEXECBUFSPERGROUP];
	D3DCOLOR * colour_cell_pnt[MAXEXECBUFSPERGROUP];

	uint16	num_animating_polys[MAXEXECBUFSPERGROUP];

	POLYANIM *polyanim[MAXEXECBUFSPERGROUP];
	uint16	StartPosInThisGroup;
	uint32				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
}GROUP;


typedef struct MLOADHEADER{
	int					state;								// FALSE if not loaded properly...
	char		*		OrgAddr;
	char		*		Buffer;
	uint16				num_texture_files;
	uint16				num_groups;
	GROUP				Group[MAXGROUPS];
	int16				TloadIndex[MAXTPAGESPERMLOAD];		// which texture in the Tloadheader....
    char                ImageFile[MAXTPAGESPERMLOAD][32];	// Texture Files....
	float				CellSize;
	ANIMDATA			AnimData;
}MLOADHEADER;



typedef struct MVERT {
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float tu;
	float tv;
} MVERT;

typedef struct MFACE {
	uint16 v1;
	uint16 v2;
	uint16 v3;
	uint16 pad;
	float nx;
	float ny;
	float nz;
} MFACE;


typedef struct VISPOLVERTEX {
	float	x;
	float	y;
	float	z;
	DWORD	dwFlags;

#ifdef SOFTWARE_ENABLE
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Chris Walsh's Code
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
	float	wx,wy;	// cw: In screen space.
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#endif

} VISPOLVERTEX;


/*
 * fn prototypes
 */
BOOL PreMload( char * Filename, MLOADHEADER * Mloadheader  );
BOOL Mload( char * Filename, MLOADHEADER * Mloadheader );
BOOL ExecuteMloadHeader( MLOADHEADER * Mloadheader  );
void ReleaseMloadheader( MLOADHEADER * Mloadheader );

BOOL ExecuteSingleGroupMloadHeader( MLOADHEADER * Mloadheader, uint16 group  );
void BackGroundTextureAnimation( MLOADHEADER * Mloadheader , uint16 group );
uint16 * HandleAnimCommands( POLYANIM * PolyAnim , uint16 * AnimData , uint16 * OrgAnimAdr);

void TriggerBackgroundAnimationGo( uint16 * Data );
void TriggerBackgroundAnimationStop( uint16 * Data );

extern FILE *LoadTextureAnimations( FILE *f );
extern FILE *SaveTextureAnimations( FILE *f );

#endif


