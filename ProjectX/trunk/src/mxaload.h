/*==========================================================================
 *
 *
 *  File:       mxaload.h
 *  Content:    Mxaload include file
 *
 *
 ***************************************************************************/

#ifndef MXALOAD_INCLUDED
#define MXALOAD_INCLUDED

/*===================================================================
	Includes
===================================================================*/

#include "main.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "tload.h"
#include "mload.h"
#include "transexe.h"
//#include "pickups.h"

/*===================================================================
	Defines
===================================================================*/
#define MAX_MXAGROUPS			1
#define	MAX_ANIM_FRAMES			16 //1
#define MAXMXATEXTUREGROUPSPER 8
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
	COLOR color;
	COLOR specular;
	float tu;
	float tv;
	int32_t flags;
} MXAVERT;


typedef struct COLOR_RGBA {
	u_int8_t b;
	u_int8_t g;
	u_int8_t r;
	u_int8_t a;
} COLOR_RGBA;

typedef struct MXAGROUP{
	u_int16_t	num_execbufs;
	u_int16_t	exec_type[MAXEXECBUFSPERGROUP];
	u_int16_t	num_verts_per_execbuf[MAXEXECBUFSPERGROUP];
	u_int16_t	num_polys_per_execbuf[MAXEXECBUFSPERGROUP];
	u_int16_t	num_texture_groups[MAXMXATEXTUREGROUPSPER];
	RENDEROBJECT renderObject[MAXEXECBUFSPERGROUP];
	LPLVERTEX originalVerts[MAXEXECBUFSPERGROUP];
	LPTRIANGLE poly_ptr	[MAXEXECBUFSPERGROUP];
	u_int32_t	texture_group_vert_off[MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];
	char	name[16];
}MXAGROUP;


typedef u_int16_t MXAVERTCOUNT[MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];		// points to the begining of each set of verts...	
typedef	MXAVERT	*MXAVERTFRAME[MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];		// points to the begining of each set of verts...	

typedef struct MXALOADHEADER{

	int					state;								// false if not loaded properly...
	int					CurrentFrame;
	int					WantedFrame;
	float				Interp;								// how close to the wanted frame....
	float				Time;
	char		*		OrgAddr;
	char		*		Buffer;
	u_int16_t				num_texture_files;
	u_int16_t				num_groups;
	MXAGROUP			Group[MAX_MXAGROUPS];
	int16_t				TloadIndex[MAXTPAGESPERMXALOAD];		// which texture in the Tloadheader....
    char                ImageFile[MAXTPAGESPERMXALOAD][32];	// Texture Files....
	u_int16_t				num_frames;
#if 1
	MXAVERTCOUNT *		num_anim_vertices;// points to the begining of each set of verts...	
	MXAVERTFRAME *		frame_pnts;		  // points to the begining of each set of verts...	
#else
	u_int16_t				num_anim_vertices[MAX_ANIM_FRAMES][MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];		// points to the begining of each set of verts...	
	MXAVERT		*		frame_pnts[MAX_ANIM_FRAMES][MAX_MXAGROUPS][MAXEXECBUFSPERGROUP][MAXMXATEXTUREGROUPSPER];		// points to the begining of each set of verts...	
#endif
	int					AllocateTPage;
	u_int16_t				NumFirePoints;							// Number of FirePoints
	PVFIREPOINT	*		FirePoints;								// FirePoints
	u_int16_t				NumSpotFX;								// Number of SpotFX
	PVSPOTFX	*		SpotFX;									// SpotFX
	VECTOR				Center;									// Center Pos
	VECTOR				Sizes;									// X,Y,Z Sizes

}MXALOADHEADER;

/*===================================================================
	Prototypes
===================================================================*/
bool PreMxaload( char * Filename, MXALOADHEADER * Mxaloadheaders, int header_num, bool LevelSpecific );
bool Mxaload( char * Filename, MXALOADHEADER * Mxaloadheader, bool StoreTriangles  );
bool ExecuteMxaloadHeader( MXALOADHEADER * Mxaloadheader, u_int16_t group );
void ReleaseMxaloadheader( MXALOADHEADER * Mxaloadheader );
bool ExecuteSingleGroupMxaloadHeader( MXALOADHEADER * Mxaloadheader, u_int16_t group  );
bool PreInitMxaModels( char * NamePnt);
bool InitMxaModels( char * NamePnt);
bool ReleaseMxaModels();
bool InterpFrames( MXALOADHEADER * Mxaloadheader , int FromFrame, int ToFrame , float Interp );

#endif


