/*==========================================================================
 *
 *
 *  File:       Mxaload.h
 *  Content:    Mxaload include file
 *
 *
 ***************************************************************************/

#ifndef MXALOAD_INCLUDED
#define MXALOAD_INCLUDED

/*===================================================================
	Includes
===================================================================*/

#include <d3d9.h>
#include "typedefs.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "Tload.h"
#include "Mload.h"
#include "Transexe.h"
//#include "pickups.h"

/*===================================================================
	Defines
===================================================================*/
#define MAX_MXAGROUPS			1
#define	MAX_ANIM_FRAMES			16 //1
#define MAXTEXTUREGROUPSPEREXECLIST 8
#define	MAXTPAGESPERMXALOAD		8
#define	MAXMXAMODELHEADERS		1024

#define	MAXBIKETYPES		17

#define MXA_ANIM_POS		(1 << 0)
#define MXA_ANIM_RGB		(1 << 1)
#define MXA_ANIM_UV			(1 << 2)

/*===================================================================
	Structures
===================================================================*/
typedef struct MXAVERT {
	float x;
	float y;
	float z;
	D3DCOLOR color;
	D3DCOLOR specular;
	float tu;
	float tv;
	int32 flags;
} MXAVERT;


typedef struct D3DCOLOR_RGBA {
	uint8 b;
	uint8 g;
	uint8 r;
	uint8 a;
} D3DCOLOR_RGBA;

typedef struct MXAGROUP{
	uint16	num_execbufs;
	uint16	exec_type[MAXEXECBUFSPERGROUP];
	uint16	num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
	uint16	num_polys_per_execbuf[MAXEXECBUFSPERGROUP];
	uint16	num_texture_groups[MAXTEXTUREGROUPSPEREXECLIST];
//	LPDIRECT3DEXECUTEBUFFER	lpExBuf[MAXEXECBUFSPERGROUP];
	RENDEROBJECT renderObject[MAXEXECBUFSPERGROUP];
	LPD3DLVERTEX originalVerts[MAXEXECBUFSPERGROUP];
	LPD3DTRIANGLE poly_ptr	[MAXEXECBUFSPERGROUP];
	uint32	texture_group_vert_off[MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];
	char	name[16];
}MXAGROUP;


typedef uint16 MXAVERTCOUNT[MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];		// points to the begining of each set of verts...	
typedef	MXAVERT	*MXAVERTFRAME[MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];		// points to the begining of each set of verts...	

typedef struct MXALOADHEADER{

	int					state;								// FALSE if not loaded properly...
	int					CurrentFrame;
	int					WantedFrame;
	float				Interp;								// how close to the wanted frame....
	float				Time;
	char		*		OrgAddr;
	char		*		Buffer;
	uint16				num_texture_files;
	uint16				num_groups;
	MXAGROUP			Group[MAX_MXAGROUPS];
	int16				TloadIndex[MAXTPAGESPERMXALOAD];		// which texture in the Tloadheader....
    char                ImageFile[MAXTPAGESPERMXALOAD][32];	// Texture Files....
	uint16				num_frames;
#if 1
	MXAVERTCOUNT *		num_anim_vertices;// points to the begining of each set of verts...	
	MXAVERTFRAME *		frame_pnts;		  // points to the begining of each set of verts...	
#else
	uint16				num_anim_vertices[MAX_ANIM_FRAMES][MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];		// points to the begining of each set of verts...	
	MXAVERT		*		frame_pnts[MAX_ANIM_FRAMES][MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXTEXTUREGROUPSPEREXECLIST];		// points to the begining of each set of verts...	
#endif
	int					AllocateTPage;
	uint16				NumFirePoints;							// Number of FirePoints
	PVFIREPOINT	*		FirePoints;								// FirePoints
	uint16				NumSpotFX;								// Number of SpotFX
	PVSPOTFX	*		SpotFX;									// SpotFX
	VECTOR				Center;									// Center Pos
	VECTOR				Sizes;									// X,Y,Z Sizes

}MXALOADHEADER;

/*===================================================================
	Prototypes
===================================================================*/
BOOL PreMxaload( char * Filename, MXALOADHEADER * Mxaloadheaders, int header_num, BOOL LevelSpecific );
BOOL Mxaload( char * Filename, MXALOADHEADER * Mxaloadheader, BOOL StoreTriangles  );
BOOL ExecuteMxaloadHeader( MXALOADHEADER * Mxaloadheader, uint16 group );
void ReleaseMxaloadheader( MXALOADHEADER * Mxaloadheader );
BOOL ExecuteSingleGroupMxaloadHeader( MXALOADHEADER * Mxaloadheader, uint16 group  );
BOOL PreInitMxaModels( char * NamePnt);
BOOL InitMxaModels( char * NamePnt);
BOOL ReleaseMxaModels();
BOOL InterpFrames( MXALOADHEADER * Mxaloadheader , int FromFrame, int ToFrame , float Interp );

#endif


