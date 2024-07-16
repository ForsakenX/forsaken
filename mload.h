/*==========================================================================
 *
 *
 *  File:       mload.h
 *  Content:    Mload include file
 *
 *
 ***************************************************************************/

#ifndef MLOAD_INCLUDED
#define MLOAD_INCLUDED

#include "main.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "tload.h"
#include "transexe.h"

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

#define	LEVEL_SPECIFIC			true
#define	NOT_LEVEL_SPECIFIC		false

/*
 * structures
 */

typedef struct  {
	float u, v;
} PROJPOINT;



typedef struct MCFACE {
	u_int32_t type;
	float nx;
	float ny;
	float nz;
	float D;
	PROJPOINT v[ 4 ];
} MCFACE;


typedef struct VERTEXCELL{
	u_int16_t	num_verts_in_cell;
	u_int16_t	start_vert_in_cell;
}VERTEXCELL;


typedef struct STARTPOS{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	Up;
	u_int16_t	Group;
}STARTPOS;

typedef struct _GAMESTARTPOS{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	Up;
	u_int16_t	Group;
	u_int16_t	NextInGroup;
}GAMESTARTPOS;

typedef struct PORTALPOLY
{
	u_int16_t num_vertices_in_poly;
	VERT   Verts[MAXVERTSPERPORTALPOLY];
}PORTALPOLY;


typedef struct VISTREE VISTREE;

typedef struct PORTAL PORTAL;


struct VISTREE
{
	PORTAL *portal;
	u_int16_t group;
	u_int16_t num_visible;
	VISTREE *visible;
};



struct PORTAL{
	u_int16_t num_vertices_in_portal;
	VERT   Verts[MAXVERTSPERPORTAL];
	u_int16_t num_polys_in_portal;
	MCFACE Poly[MAXPOLYSPERPORTAL];
	VISTREE visible;
	VERT centroid;
};


typedef struct  {
	float u, v;
} TANIMUV;


typedef struct  {
	u_int16_t	State;
	u_int16_t	animation;
	float	CurrentTime;
	float	MasterTime;
	u_int16_t	CurrentOffset;
	u_int16_t	currentframe;	// what we are displaying now...
	u_int16_t	newframe;		// what we want ti display..
	u_int16_t	frames;			// number of frames
	u_int16_t	vertices;		// number of verts...
	u_int16_t	loopstack[MAXANIMLOOPSTACK];
	int	*	vert;
	TANIMUV	* UVs;
	char	Tag[32];
	int16_t	WhenStoppedTriggerMod;
} POLYANIM;

typedef struct _TEXTUREANIMINFOINDEX{
	u_int16_t	Group;
	u_int16_t	Execbuf;
	u_int16_t	PolyAnim;
	u_int16_t	InitState;
}TEXTUREANIMINFOINDEX;


typedef struct  {
	u_int16_t	num_animating_polys;
TEXTUREANIMINFOINDEX * TAnimInfoIndex;
	u_int16_t	num_animations;
	u_int16_t * AnimSeq[MAXANIMSEQS];
}ANIMDATA;


typedef struct LVLGROUP{
	u_int16_t	num_execbufs;
	u_int16_t	exec_type[MAXEXECBUFSPERGROUP];
	u_int16_t	num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
	LEVELRENDEROBJECT			renderObject[MAXEXECBUFSPERGROUP];
	LPLVERTEX originalVerts[MAXEXECBUFSPERGROUP];
	char	name[32];
	VERT	center;
	VERT	half_size;
	VECTOR	up;
	u_int16_t	num_portals;
	PORTAL  * Portal;
	int16_t	BGClear_Flag;
	float	BGClear_Red;
	float	BGClear_Green;
	float	BGClear_Blue;

	VECTOR	cell_origin[MAXEXECBUFSPERGROUP];
	u_int16_t	xcells[MAXEXECBUFSPERGROUP];
	u_int16_t	ycells[MAXEXECBUFSPERGROUP];
	u_int16_t	zcells[MAXEXECBUFSPERGROUP];
	u_int16_t	numofcells[MAXEXECBUFSPERGROUP];
	u_int16_t	num_vertex_indices[MAXEXECBUFSPERGROUP];
	u_int16_t * vertex_index_pnt[MAXEXECBUFSPERGROUP];
	VERTEXCELL * vertex_cell_pnt[MAXEXECBUFSPERGROUP];
	COLOR * colour_cell_pnt[MAXEXECBUFSPERGROUP];

	u_int16_t	num_animating_polys[MAXEXECBUFSPERGROUP];

	POLYANIM *polyanim[MAXEXECBUFSPERGROUP];
	u_int16_t	StartPosInThisGroup;
	u_int32_t				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
}LVLGROUP;


typedef struct MLOADHEADER{
	int					state;								// false if not loaded properly...
	char		*		OrgAddr;
	char		*		Buffer;
	char		*		testBuffer;
	u_int16_t				num_texture_files;
	u_int16_t				num_groups;
	LVLGROUP				Group[MAXGROUPS];
	int16_t				TloadIndex[MAXTPAGESPERMLOAD];		// which texture in the Tloadheader....
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
	u_int16_t v1;
	u_int16_t v2;
	u_int16_t v3;
	u_int16_t pad;
	float nx;
	float ny;
	float nz;
} MFACE;


typedef struct VISPOLVERTEX {
	float	x;
	float	y;
	float	z;
	DWORD	dwFlags;

} VISPOLVERTEX;


/*
 * fn prototypes
 */
bool PreMload( char * Filename, MLOADHEADER * Mloadheader  );
bool Mload( char * Filename, MLOADHEADER * Mloadheader );
bool ExecuteMloadHeader( MLOADHEADER * Mloadheader  );
void ReleaseMloadheader( MLOADHEADER * Mloadheader );

bool ExecuteSingleGroupMloadHeader( MLOADHEADER * Mloadheader, u_int16_t group  );
void BackGroundTextureAnimation( MLOADHEADER * Mloadheader , u_int16_t group );
u_int16_t * HandleAnimCommands( POLYANIM * PolyAnim , u_int16_t * AnimData , u_int16_t * OrgAnimAdr);

void TriggerBackgroundAnimationGo( u_int16_t * Data );
void TriggerBackgroundAnimationStop( u_int16_t * Data );

extern FILE *LoadTextureAnimations( FILE *f );
extern FILE *SaveTextureAnimations( FILE *f );

#endif


