/*==========================================================================
 *
 *  File: Mxaload.c
 *	loads in a bin....
 *  extracts the names of the ppm's and creates a suface and material...
 *  make an execution list per group of....
 *  make a list of D3DVERTEX's.....and D3DTRIANGLE's....
***************************************************************************/

/*
Execution list format with animated vertices (*.mxa):

num_texture_files : uint16
texture_file_name[num_texture_files] : '\0' separated strings
num_groups : uint16
{
	num_exec_lists : uint16
	{
	  exec_type : uint16 // 0 = entirely opaque, 1 = contains transparencies
	  num_vertices : uint16
	  vertex(x,y,z,reserved,colour,specular,tu,tv)[num_vertices] : x, y, z float, others uint32
	  num_texture_groups : uint16
	  {
	    texture_type : uint16 // 0 = normal, 1 = environment mapped
		start_vertex : uint16
		num_vertices : uint16
		texture_no : uint16
		num_triangles : uint16
		triangles(v0,v1,v2,pad16,nx,ny,nz)[num_triangles] : v? uint16, pad16 uint16, n? float
	  }[num_texture_groups]
	}[num_exec_lists]
}[num_groups]
mxtype : uint16 // always 2 for mxa format
num_frames : uint16 // number of animation frames
{
	{
		{
			{
				num_anim_vertices : uint16 // number of animating vertices for this texture group
				vertex(x, y, z)[num_anim_vertices] : all float
			}[num_texture_groups]
		}[num_exec_lists]
	}[num_groups]
}[num_frames]
*/
/*===================================================================
		Include File...	
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "triggers.h"
#include "pickups.h"
#include "mxload.h"
#include "mxaload.h"
#include "magic.h"
#include "sfx.h"
#include "spotfx.h"
#include "XMem.h"

/*===================================================================
		Defines
===================================================================*/
#define	MXA_VERSION_NUMBER	2

//#define FIX_MXA_UV
#undef FIX_MXA_UV

/*===================================================================
		Externals...	
===================================================================*/

void DebugPrintf( char *fmt, ... );
extern void FixUV( LPD3DTRIANGLE Tri, LPD3DLVERTEX Vert, uint16 Tpage, LPD3DLVERTEX Orig_Vert );

extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern	D3DMATRIXHANDLE hWorld;
extern	BOOL	UsedStippledAlpha;
extern	float	framelag;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	BOOL	CanCullFlag;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	int16	BikeModels[ MAXBIKETYPES ];
extern	D3DMATRIXHANDLE hView;
extern	D3DMATRIX view;
extern	D3DMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	D3DMATRIXHANDLE hWorld;
extern	D3DMATRIX  TempWorld;	
extern	BOOL	CanCullFlag;

int16		NewLevelNum;

/*===================================================================
		Globals...	
===================================================================*/
MXALOADHEADER MxaModelHeaders[MAXMXAMODELHEADERS];
#if 0
BOOL	WaterEnable = FALSE;
#define WATERLEVEL	(1)
#define WATERTPAGE	(2)
#define MESHX (32)
#define MESHY (32)
#define WATER (31)
#define WATER_CELLSIZE	(64.0F)
#define WATER_IMPACT	(-2.0F)
#define	ROOTTWO  (1.0F / (float)sqrt(2.0F))
#define	SPECIALROOTTWO (1.0F / (4.0F+ROOTTWO*4.0F))

VECTOR	WaterPos = { 28.5F * 256.0F, -430.0F , (1.0F * 256.0F) };	// Biodome in a dome...
VECTOR	WaterNormal = { 0.0F, 1.0F, 0.0F };
int16	WaterGroup = 0;

float	Fc[MESHX][MESHY];
float	V[MESHX][MESHY];
float	FinalV[MESHX][MESHY];

LPDIRECT3DEXECUTEBUFFER WaterExecbuf = NULL;
#endif


static void
FixUV_MXA( uint16 vertices, MXAVERT *MXA_Vert, LPD3DLVERTEX Vert, LPD3DLVERTEX Orig_Vert )
{
	int vnum;

	for ( vnum = 0; vnum < vertices; vnum++ )
	{
		MXA_Vert[ vnum ].tu += Vert[ vnum ].tu - Orig_Vert[ vnum ].tu;
		MXA_Vert[ vnum ].tv += Vert[ vnum ].tv - Orig_Vert[ vnum ].tv;
	}
}

/*===================================================================
	Procedure	:		Load .Mx File
	Input		:		char	*	Filename , MXALOADHEADER *
	Output		:		Nothing
===================================================================*/
BOOL Mxaload( char * Filename, MXALOADHEADER * Mxaloadheader, BOOL StoreTriangles  )
{
//	D3DEXECUTEDATA			d3dExData;
//	D3DEXECUTEBUFFERDESC	debDesc;

	char		*	Buffer;
	int16		*	Int16Pnt;
	uint32		*	Uint32Pnt;
	uint16		*	Uint16Pnt;
	float		*	FloatPnt;
	MFACE		*	MFacePnt;
	LPD3DTRIANGLE	FacePnt;
	LPD3DTRIANGLE	TempFacePnt;
	uint16			exec_type;			// the type of execute buffer
	uint16			texture_type;		// the type of texture...0 normal  1 env
	uint16			num_vertices;		// overall number of verts
	uint16			num_texture_groups;// number of triangle groups
	uint16			num_triangles;		// number of triangles in group
	uint16			group_vertex_start; // where in the vert list to start processing
	uint16			group_vertex_num;	// and how many to do...
	LPD3DLVERTEX   	lpD3DLVERTEX2;
	LPD3DLVERTEX	lpD3DLVERTEX;
	uint16			frame;
	uint16			texgroup;
	uint16			num_anim_vertices;
	int				i,e;
	uint16			tpage;
	int				execbuf;
	int				group;
	uint16			ExecSize;

	uint16			NumSpotFX;
	PVSPOTFX *		SpotFXPtr;
	uint16			NumFirePoints;
	PVFIREPOINT *	FirePointPtr;
	uint32			Colour;
	int				colourkey = 0;
#if	MXA_VERSION_NUMBER == 2
	int16			c;
	int8		*	Int8Pnt;
	int8			SFXFilename[ 128 ];
#endif

    LPVOID lpBufStart = NULL;
	LPVOID lpInsStart = NULL;
	LPVOID lpPointer = NULL;

	// Mxaloadheader is not valid until everything has been done..
	Mxaloadheader->state = FALSE;
	Mxaloadheader->WantedFrame = 0;
	Mxaloadheader->CurrentFrame = 0;
	Mxaloadheader->Interp = 0.0F;
	Mxaloadheader->Time = 0.0F;
	
	Buffer = Mxaloadheader->Buffer;
	if( Buffer == NULL)
	{
		Msg( "Mxaload() Unable to allocate buffer in %s\n", Filename );
		return FALSE;
	}
	

	/*	get the number of groups	*/
	Uint16Pnt = (uint16 *) Buffer;
	Mxaloadheader->num_groups = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		

	if ( Mxaloadheader->num_groups > MAXGROUPS )
	{
		Msg( "Mxaload() num_groups > MAXGROUPS\n", Filename );
		return FALSE;
	}

	for( group=0 ; group<Mxaloadheader->num_groups; group++)
	{
		
		/*	get the number of execbufs in this group	*/
		Uint16Pnt = (uint16 *) Buffer;
		Mxaloadheader->Group[group].num_execbufs = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;		
	
		if ( Mxaloadheader->Group[group].num_execbufs > MAXEXECBUFSPERGROUP )
		{
			Msg( "Mxaload() num_execbufs > MAXEXECBUFSPERGROUP\n", Filename );
			return FALSE;
		}
		
		for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
		{
			Uint16Pnt = (uint16 *) Buffer;

			ExecSize = 	*Uint16Pnt++;
			ExecSize += 512;

			/*	get the type of execution buffer	*/
			exec_type = *Uint16Pnt++;
			/*	get the number of verts in execution buffer	*/
			num_vertices =	*Uint16Pnt++;     
			Buffer = (char *) Uint16Pnt;		
	
			/*	record how what type of exec buffer	*/
			Mxaloadheader->Group[group].exec_type[execbuf] = exec_type;

			/*	record how many verts there are in the exec buffer	*/
			Mxaloadheader->Group[group].num_verts_per_execbuf[execbuf] = num_vertices;


			/*	create an execution buffer	*/
#if 0	
			if (MakeExecuteBuffer( &debDesc, /*d3dappi.lpD3DDevice,*/ &Mxaloadheader->Group[group].lpExBuf[execbuf] , ExecSize ) != TRUE ) // bjd
			{	
				Msg( "Mxaload() MakeExecuteBuffer failed in %s\n", Filename );
				return FALSE;
			}
	
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
#endif
			if (FAILED(FSCreateVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], num_vertices)))
			{
				Msg( "Mxaload() CreateVertexBuffer failed in %s\n", Filename );
				return FALSE;
			}

			/*	lock the execute buffer	*/
//			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( Mxaloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
/*
			if (FSLockExecuteBuffer(Mxaloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
			{
				Msg( "Mxaload() lock failed in %s\n", Filename );
				return FALSE ;
			}
*/
			if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], lpBufStart)))
			{
				Msg( "Mxaload() lock failed in %s\n", Filename );
				return FALSE ;
			}

//			lpBufStart = debDesc.lpData;
			lpPointer = lpBufStart;
	
			lpD3DLVERTEX2 = (LPD3DLVERTEX ) Buffer;
			lpD3DLVERTEX = (LPD3DLVERTEX ) lpPointer;
			Mxaloadheader->Group[group].org_vertpnt[execbuf] = lpD3DLVERTEX2;

			/*	copy the vertex data into the execute buffer	*/
			for ( i=0 ; i<num_vertices; i++)
			{
				lpD3DLVERTEX->x = lpD3DLVERTEX2->x;
				lpD3DLVERTEX->y = lpD3DLVERTEX2->y;
				lpD3DLVERTEX->z = lpD3DLVERTEX2->z;
				lpD3DLVERTEX->tu = lpD3DLVERTEX2->tu;
				lpD3DLVERTEX->tv = lpD3DLVERTEX2->tv;

				lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
#if !ACTUAL_TRANS
				lpD3DLVERTEX->color |= 0xFF000000;
#endif
//				lpD3DLVERTEX->specular = lpD3DLVERTEX2->specular;
				lpD3DLVERTEX->specular = RGB_MAKE( 0 , 0 , 0 );
//				lpD3DLVERTEX->dwReserved = 0;
				lpD3DLVERTEX++;
				lpD3DLVERTEX2++;
			}

 			lpPointer = (void * )  lpD3DLVERTEX;			

			lpInsStart = lpPointer;
	
/* bjd - TODO
			//	Stuff to clip if bounding box is off screen
			OP_SET_STATUS( D3DSETSTATUS_STATUS,  D3DSTATUS_CLIPINTERSECTIONFRONT |
							   D3DSTATUS_CLIPINTERSECTIONTOP |
							   D3DSTATUS_CLIPINTERSECTIONBOTTOM |
							   D3DSTATUS_CLIPINTERSECTIONLEFT |
							   D3DSTATUS_CLIPINTERSECTIONRIGHT, 0, 0, 0, 0, lpPointer);
			OP_PROCESS_VERTICES(1, lpPointer);
				PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, 0, 8, lpPointer);
			OP_BRANCH_FORWARD( D3DSTATUS_CLIPINTERSECTIONFRONT |
							   D3DSTATUS_CLIPINTERSECTIONTOP |
							   D3DSTATUS_CLIPINTERSECTIONBOTTOM |
							   D3DSTATUS_CLIPINTERSECTIONLEFT |
							   D3DSTATUS_CLIPINTERSECTIONRIGHT
							   , 0, TRUE , 0, lpPointer);
*/
			
			Buffer = (char *) lpD3DLVERTEX2;
			
			Uint16Pnt = (uint16 *) Buffer;
			num_texture_groups = *Uint16Pnt++;    
			Buffer = (char *) Uint16Pnt;		
			Mxaloadheader->Group[group].num_texture_groups[execbuf] = num_texture_groups;			

			for ( i=0 ; i<num_texture_groups; i++)
			{
				Uint16Pnt = (uint16 *) Buffer;
				texture_type = *Uint16Pnt++;
				group_vertex_start = *Uint16Pnt++;
				group_vertex_num = *Uint16Pnt++;
				tpage = *Uint16Pnt++;
				num_triangles = *Uint16Pnt++;    
				Buffer = (char *) Uint16Pnt;		
	
				Mxaloadheader->Group[group].texture_group_vert_off[execbuf][i] = (uint32) (group_vertex_start*sizeof(D3DLVERTEX));
/* bjd - TODO
				OP_STATE_LIGHT(1, lpPointer);
				    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[Mxaloadheader->TloadIndex[tpage]], lpPointer);
				OP_PROCESS_VERTICES(1, lpPointer);
					PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, group_vertex_start+8, group_vertex_num, lpPointer);
				OP_STATE_RENDER(1, lpPointer);
				    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[Mxaloadheader->TloadIndex[tpage]], lpPointer);
			    if (QWORD_ALIGNED(lpPointer))
					OP_NOP(lpPointer);

				OP_TRIANGLE_LIST( (short) num_triangles, lpPointer);
*/

				MFacePnt = (MFACE *) Buffer;
				FacePnt = (LPD3DTRIANGLE ) lpPointer;
				TempFacePnt = FacePnt;

				/*	copy the faces data into the execute buffer	*/
				for( e=0; e<num_triangles; e++)
				{
					FacePnt->v1 = MFacePnt->v1;
					FacePnt->v2 = MFacePnt->v2;
					FacePnt->v3 = MFacePnt->v3;
					if ( MFacePnt->pad & 1 )
					{
						// colourkey triangle found
						colourkey++;
						MFacePnt->pad &= ~1;
					}
//					FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					FacePnt->wFlags = MFacePnt->pad;
#ifdef FIX_MXA_UV
					FixUV( FacePnt, lpBufStart, tpage, Mxaloadheader->Group[group].org_vertpnt[execbuf] );
#endif
					FacePnt++;
					MFacePnt++;
				}
				lpPointer = (LPVOID) FacePnt;
				Buffer = (char *) MFacePnt;

				if (StoreTriangles)
				{	Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = num_triangles;			
					Mxaloadheader->Group[group].poly_ptr[execbuf] = (LPD3DTRIANGLE)malloc( sizeof (D3DTRIANGLE) * num_triangles);			
				
					memcpy(Mxaloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (D3DTRIANGLE) * num_triangles);				

				}
				else
				{
					Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = 0;
					Mxaloadheader->Group[group].poly_ptr[execbuf] = NULL;			
				}
			}
			
//			OP_EXIT(lpPointer);

#if 0
			/*	unlock the execute buffer	*/
			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxaloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
			{
				Msg( "Mxaload() unlock failed in %s\n", Filename );
				return FALSE ;
			}
#endif
			if (FAILED(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxaload() unlock failed in %s\n", Filename );
				return FALSE ;
			}
#if 0
			/*	set the data for the execute buffer	*/
			memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
			d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
			d3dExData.dwVertexCount = (num_vertices);
			d3dExData.dwInstructionOffset = (ULONG) ((char *)lpInsStart - (char *)lpBufStart);
			d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->SetExecuteData(Mxaloadheader->Group[group].lpExBuf[execbuf], &d3dExData) != D3D_OK)
			{
				Msg( "Mxaload() SetExecuteDate failed in %s\n", Filename );
				return FALSE;
			}
#endif
			Mxaloadheader->Group[group].renderObject[execbuf].startVert = 0;
			Mxaloadheader->Group[group].renderObject[execbuf].numVerts = num_vertices;
			Mxaloadheader->Group[group].renderObject[execbuf].texture = NULL;
		}
	}
				

	Uint16Pnt = (uint16 *) Buffer;
	// is there any vispoly info.....
	if ( *Uint16Pnt++ == 2)				// 2 means .mxa format
	{
/*===================================================================
 *		Particular Stuff Related Anims	
===================================================================*/
		Mxaloadheader->num_frames = *Uint16Pnt++;

		Buffer = (char * ) Uint16Pnt;

		if ( Mxaloadheader->num_frames > 1 )
		{
			if ( !( Mxaloadheader->num_anim_vertices = (MXAVERTCOUNT *) calloc( Mxaloadheader->num_frames, sizeof( MXAVERTCOUNT ) ) ) )
			{
				Msg( "Mxaload() calloc(num_frames=%d) failed in %s\n", Mxaloadheader->num_frames, Filename );
				return FALSE;
			}
			if ( !( Mxaloadheader->frame_pnts = (MXAVERTFRAME *) calloc( Mxaloadheader->num_frames, sizeof( MXAVERTFRAME ) ) ) )
			{
				Msg( "Mxaload() calloc(num_frames=%d) failed in %s\n", Mxaloadheader->num_frames, Filename );
				return FALSE;
			}
			
			for( frame=0 ; frame<Mxaloadheader->num_frames; frame++)
			{
				for( group=0 ; group<Mxaloadheader->num_groups; group++)
				{
					for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
					{
/*
						memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
						debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/
//						if( Mxaloadheader->Group[ group ].lpExBuf[ execbuf ]->lpVtbl->Lock(
//							Mxaloadheader->Group[ group ].lpExBuf[ execbuf ], &debDesc ) != D3D_OK ) // bjd
/*
						if (FSLockExecuteBuffer(Mxaloadheader->Group[ group ].lpExBuf[ execbuf ], &debDesc ) != D3D_OK )
						{
							Msg( "Mxaload : Lock ExecBuffer failed\n" );
							return FALSE;
						}
*/
						if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[ group ].renderObject[execbuf], lpD3DLVERTEX)))
						{
							Msg( "Mxaload : Lock VertexBuffer failed\n" );
							return FALSE;
						}

						for( texgroup=0; texgroup < Mxaloadheader->Group[group].num_texture_groups[execbuf] ; texgroup++)
						{
							Uint16Pnt = (uint16 *) Buffer;
							num_anim_vertices = *Uint16Pnt++;
							Buffer = (char * ) Uint16Pnt;
							Mxaloadheader->num_anim_vertices[frame][group][execbuf][texgroup] = num_anim_vertices;
							Mxaloadheader->frame_pnts[frame][group][execbuf][texgroup] = (MXAVERT*) Buffer;
							lpD3DLVERTEX = 	( LPD3DLVERTEX ) ( /*(char*) debDesc.lpData*/lpD3DLVERTEX +( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							lpD3DLVERTEX += 8;
							lpD3DLVERTEX2 =	( LPD3DLVERTEX ) ( (char*) Mxaloadheader->Group[group].org_vertpnt[execbuf] +( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							lpD3DLVERTEX2 += 8;
#ifdef FIX_MXA_UV
							FixUV_MXA( Mxaloadheader->num_anim_vertices[frame][group][execbuf][texgroup],
								Mxaloadheader->frame_pnts[frame][group][execbuf][texgroup],
								lpD3DLVERTEX, lpD3DLVERTEX2 );
#endif
							Buffer += ( num_anim_vertices * sizeof( MXAVERT ) );
						}
/*
						if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxaloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
						{
							Msg( "Mxaload : Unlock ExecBuffer failed\n" );
							return FALSE ;
						}
*/						
						if (FAILED(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
						{
							Msg( "Mxaload : Unlock VertexBuffer failed\n" );
							return FALSE ;
						}
					}
				}
			}
		}
		else
		{
			Mxaloadheader->num_frames = 0;
			Mxaloadheader->num_anim_vertices = NULL;
			Mxaloadheader->frame_pnts = NULL;
		}
	}
	
/*===================================================================
	Point Direction Data
===================================================================*/
	Uint16Pnt = (uint16 *) Buffer;
#if 0
	if( *Uint16Pnt++ )
	{
		NumPVs = *Uint16Pnt++;

		if( NumPVs )
		{
			Mxaloadheader->NumPVs = NumPVs;
			Mxaloadheader->PVs = (PV *) malloc( NumPVs * sizeof( PV ) );

			if( !Mxaloadheader->PVs )
			{
				Mxaloadheader->NumPVs = 0;
				return( FALSE );
			}

			PVPtr = Mxaloadheader->PVs;

			for( i = 0; i < NumPVs; i++ )
			{
				PVPtr->Type = *Uint16Pnt++;				// Type ( uint16 )
				PVPtr->ID = *Uint16Pnt++;				// ID ( uint16 )
				FloatPnt = (float *) Uint16Pnt;
				PVPtr->Pos.x = *FloatPnt++;				// Pos ( 3 floats )
				PVPtr->Pos.y = *FloatPnt++;
				PVPtr->Pos.z = *FloatPnt++;
				PVPtr->Dir.x = *FloatPnt++;				// Dir ( 3 floats )
				PVPtr->Dir.y = *FloatPnt++;
				PVPtr->Dir.z = *FloatPnt++;
				PVPtr->Up.x = *FloatPnt++;				// Up ( 3 floats )
				PVPtr->Up.y = *FloatPnt++;
				PVPtr->Up.z = *FloatPnt++;
				PVPtr->OldPos = PVPtr->Pos;
				Uint16Pnt = (uint16 *) FloatPnt;
				PVPtr++;
			}
		}
		else
		{
			Mxaloadheader->PVs = NULL;
			Mxaloadheader->NumPVs = 0;
		}
		Buffer = (char *) Uint16Pnt;
	}
	else
	{
		Mxaloadheader->NumPVs = 0;
		Mxaloadheader->PVs = NULL;
		Buffer = (char *) ( Uint16Pnt + 1 );
	}
#else
	if( *Uint16Pnt++ )
	{
		NumFirePoints = *Uint16Pnt++;

		if( NumFirePoints )
		{
			Mxaloadheader->NumFirePoints = NumFirePoints;
			Mxaloadheader->FirePoints = (PVFIREPOINT *) malloc( NumFirePoints * sizeof( PVFIREPOINT ) );

			if( !Mxaloadheader->FirePoints )
			{
				Mxaloadheader->NumFirePoints = 0;
				return( FALSE );
			}

			FirePointPtr = Mxaloadheader->FirePoints;

			for( i = 0; i < NumFirePoints; i++ )
			{
				FirePointPtr->ID = *Uint16Pnt++;				// ID ( uint16 )
				FloatPnt = (float *) Uint16Pnt;
				FirePointPtr->Pos.x = *FloatPnt++;				// Pos ( 3 floats )
				FirePointPtr->Pos.y = *FloatPnt++;
				FirePointPtr->Pos.z = *FloatPnt++;
				FirePointPtr->Dir.x = *FloatPnt++;				// Dir ( 3 floats )
				FirePointPtr->Dir.y = *FloatPnt++;
				FirePointPtr->Dir.z = *FloatPnt++;
				FirePointPtr->Up.x = *FloatPnt++;				// Up ( 3 floats )
				FirePointPtr->Up.y = *FloatPnt++;
				FirePointPtr->Up.z = *FloatPnt++;
				Uint16Pnt = (uint16 *) FloatPnt;
				FirePointPtr++;
			}
		}
		else
		{
			Mxaloadheader->FirePoints = NULL;
			Mxaloadheader->NumFirePoints = 0;
		}

		NumSpotFX = *Uint16Pnt++;

		if( NumSpotFX )
		{
			Mxaloadheader->NumSpotFX = NumSpotFX;
			Mxaloadheader->SpotFX = (PVSPOTFX *) malloc( NumSpotFX * sizeof( PVSPOTFX ) );

			if( !Mxaloadheader->SpotFX )
			{
				Mxaloadheader->NumSpotFX = 0;
				return( FALSE );
			}

			SpotFXPtr = Mxaloadheader->SpotFX;

			for( i = 0; i < NumSpotFX; i++ )
			{
				SpotFXPtr->Type = *Uint16Pnt++;				// Type ( uint16 )
				FloatPnt = (float *) Uint16Pnt;
				SpotFXPtr->Pos.x = *FloatPnt++;				// Pos ( 3 floats )
				SpotFXPtr->Pos.y = *FloatPnt++;
				SpotFXPtr->Pos.z = *FloatPnt++;
				SpotFXPtr->DirVector.x = *FloatPnt++;		// Dir ( 3 floats )
				SpotFXPtr->DirVector.y = *FloatPnt++;
				SpotFXPtr->DirVector.z = *FloatPnt++;
				SpotFXPtr->UpVector.x = *FloatPnt++;		// Up ( 3 floats )
				SpotFXPtr->UpVector.y = *FloatPnt++;
				SpotFXPtr->UpVector.z = *FloatPnt++;
				SpotFXPtr->StartDelay = ( *FloatPnt++ * ANIM_SECOND );	// Start Delay
				SpotFXPtr->ActiveDelay = ( *FloatPnt++ * ANIM_SECOND );	// Active Delay
				SpotFXPtr->InactiveDelay = ( *FloatPnt++ * ANIM_SECOND );	// Inactive Delay
				Int16Pnt = (int16 *) FloatPnt;
				SpotFXPtr->Primary = (int8) *Int16Pnt++;	// Primary ( int16 )
				SpotFXPtr->Secondary = (int8) *Int16Pnt++;	// Secondary ( int16 )
				Uint32Pnt = (uint32 *) Int16Pnt;
				Colour = *Uint32Pnt++;

#if	MXA_VERSION_NUMBER == 2
//				Int16Pnt = (int16 *) Uint32Pnt;
//				SpotFXPtr->SoundFX = *Int16Pnt++;
//				Uint16Pnt = (uint16 *) Int16Pnt;

				Int8Pnt = (int8 *) Uint32Pnt;

				c = 0;
				do
				{
					SFXFilename[ c ] = *Int8Pnt++;
					c++;
				}
				while( SFXFilename[ ( c - 1 ) ] != 0 );
				
				if( SpotFXPtr->Type == SPOTFX_SoundFX )
				{
					if( ( SpotFXPtr->ActiveDelay == 0.0F ) && ( SpotFXPtr->InactiveDelay == 0.0F ) )
					{
						Msg( "PointDirSpotfx (SoundFX) has on and off time of 0" );
						SpotFXPtr->SoundFX = -1;
					}
					else
					{
						SpotFXPtr->SoundFX = ReturnSFXIndex( &SFXFilename[ 0 ] );
					}
				}
				else
				{
					SpotFXPtr->SoundFX = -1;
				}

				FloatPnt = (float *) Int8Pnt;
				SpotFXPtr->SoundFXVolume = *FloatPnt++;
				SpotFXPtr->SoundFXSpeed = *FloatPnt++;
				Uint16Pnt = (uint16 *) FloatPnt;
#else
				Uint16Pnt = (uint16 *) Uint32Pnt;
#endif

				SpotFXPtr->Red = (uint8) ( ( Colour >> 16 ) & 255 );
				SpotFXPtr->Green = (uint8) ( ( Colour >> 8 ) & 255 );
				SpotFXPtr->Blue = (uint8) ( Colour & 255 );
				SpotFXPtr++;
			}
		}
		else
		{
			Mxaloadheader->SpotFX = NULL;
			Mxaloadheader->NumSpotFX = 0;
		}

		Buffer = (char *) Uint16Pnt;
	}
	else
	{
		Mxaloadheader->NumFirePoints = 0;
		Mxaloadheader->FirePoints = NULL;
		Mxaloadheader->NumSpotFX = 0;
		Mxaloadheader->SpotFX = NULL;
		Buffer = (char *) ( Uint16Pnt + 1 );
	}
#endif

	if ( colourkey )
		DebugPrintf( "Mxaload: %d colourkey triangles found\n", colourkey );
	// Mxaloadheader is valid and can be executed...
	Mxaloadheader->state = TRUE;
	return( TRUE );
}
#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:		Execute all group buffers for a Mxaloadheader
	Input		;		MXALOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/

BOOL ExecuteMxaloadHeader( MXALOADHEADER * Mxaloadheader, uint16 in_group  )
{
	int i;
	int group;
	D3DMATRIX Matrix;
	if (Mxaloadheader->state == TRUE )
	{
		for ( group=0 ; group<Mxaloadheader->num_groups ; group++)
		{
			for ( i=0 ; i<Mxaloadheader->Group[group].num_execbufs; i++)
			{
				// if its a Transparent Execute Buffer then dont display it add it to the Transexe list
//				if( ((Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES) != 0) && ( UsedStippledAlpha == FALSE)  )
				if( Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
				{
//					if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
					if (FAILED(FSGetMatrix(D3DTS_WORLD, &Matrix)))
					{
						return FALSE;
					}
					AddTransExe( &Matrix , /*Mxaloadheader->Group[group].lpExBuf[i]*/&Mxaloadheader->Group[group].renderObject[i] , 0, (uint16) -1, in_group, Mxaloadheader->Group[ group ].num_verts_per_execbuf[i] );
				}
				else
				{
/*
					if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, Mxaloadheader->Group[group].lpExBuf[i], d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
						return FALSE;
*/					if (FAILED(FSDrawVertexBuffer(&Mxaloadheader->Group[group].renderObject[i])))
					{
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}
/*===================================================================
	Procedure	:		Execute one group for an Mxaloadheader
	Input		;		MXALOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/

#if 0
BOOL ExecuteSingleGroupMxaloadHeader( MXALOADHEADER * Mxaloadheader, uint16 group  )
{
	uint16 i;
	D3DMATRIX Matrix;
	
	if (Mxaloadheader->state == TRUE )
	{
		for ( i=0 ; i<Mxaloadheader->Group[group].num_execbufs; i++)
		{
			// if its a Transparent Execute Buffer then dont diplay it add it to the Transexe list
//			if( ((Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES) != 0) && ( UsedStippledAlpha == FALSE)  )
			if( Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
			{
				if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
				AddTransExe( &Matrix , Mxaloadheader->Group[group].lpExBuf[i] , 0, (uint16) -1, Mxaloadheader->Group[ group ].num_verts_per_execbuf[i] );

			}else{
				if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, Mxaloadheader->Group[group].lpExBuf[i], d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
					return FALSE;
			}
		}
	}
	return TRUE;
}
#endif

/*
 * ReleaseMxaloadheader
 * Release Execute buffers
 */
void
ReleaseMxaloadheader( MXALOADHEADER * Mxaloadheader )
{
    int i;
    int group;

	if ( !Mxaloadheader->state )
		return;
    for (group = 0; group < Mxaloadheader->num_groups; group++)
	{
	    for (i = 0; i < Mxaloadheader->Group[group].num_execbufs; i++)
		{
//			XRELEASE(Mxaloadheader->Group[group].lpExBuf[i]);
			FSReleaseRenderObject(&Mxaloadheader->Group[group].renderObject[i]);

//			free(Mxaloadheader->Group[group].org_vertpnt[i]);
	
			if (Mxaloadheader->Group[group].poly_ptr[i])
			{
				free(Mxaloadheader->Group[group].poly_ptr[i]);
				Mxaloadheader->Group[group].poly_ptr[i] = NULL;
			}
		}
	}

	if ( Mxaloadheader->OrgAddr )
	{
		free( Mxaloadheader->OrgAddr );
		Mxaloadheader->OrgAddr = NULL;
	}
	
	if( Mxaloadheader->SpotFX && Mxaloadheader->NumSpotFX )
	{
		free( Mxaloadheader->SpotFX );
		Mxaloadheader->SpotFX = NULL;
		Mxaloadheader->NumSpotFX = 0;
	}

	if( Mxaloadheader->FirePoints && Mxaloadheader->NumFirePoints )
	{
		free( Mxaloadheader->FirePoints );
		Mxaloadheader->FirePoints = NULL;
		Mxaloadheader->NumFirePoints = 0;
	}

	if ( Mxaloadheader->num_frames > 1 )
	{
		if ( Mxaloadheader->num_anim_vertices )
			free( Mxaloadheader->num_anim_vertices );
		Mxaloadheader->num_anim_vertices = NULL;

		if ( Mxaloadheader->frame_pnts )
			free ( Mxaloadheader->frame_pnts );
		Mxaloadheader->frame_pnts = NULL;
	}

	Mxaloadheader->state = FALSE;
}




/*===================================================================
	Procedure	:		Pre - Load .Mxa File
	Input		:		char	*	Filename , MXALOADHEADER *
	Output		:		Nothing
===================================================================*/
BOOL PreMxaload( char * Filename, MXALOADHEADER * Mxaloadheaders, int header_num, BOOL LevelSpecific )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	uint16		*	Uint16Pnt;
	int			i;
	char	*	FileNamePnt;
	MXALOADHEADER * Mxaloadheader;
	uint32		*	Uint32Pnt;
	uint32			MagicNumber;
	uint32			VersionNumber;
	int8			TempFilename[ 256 ];

	Mxaloadheader = &Mxaloadheaders[header_num];

	// Mxaloadheader is not valid until everything has been done..
	Mxaloadheader->state = FALSE;
	Mxaloadheader->Buffer = NULL;

	File_Size = Get_File_Size( Filename );	

	if( !File_Size )
	{
		Msg( "PreMxaload() File %s not found\n", Filename );
		return( FALSE );
	}

	Buffer = calloc( 1, File_Size + 32 );

	if( Buffer == NULL )
	{
		Msg( "PreMxaload() Unable to allocate buffer in %s\n", Filename );
		return( FALSE );
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
		Msg( "PreMxaload() Error reading %s\n", Filename );
		return( FALSE );
	}

	Mxaloadheader->OrgAddr = Buffer;

	Uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MXA_VERSION_NUMBER  ) )
	{
		Msg( "PreMxaload() Incompatible model( .MXA ) file %s", Filename );
		return( FALSE );
	}

	Uint16Pnt = (uint16 *) Buffer;

	Mxaloadheader->num_texture_files = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		

	if ( Mxaloadheader->num_texture_files > MAXTPAGESPERMXALOAD )
	{
		Msg( "PreMxaload() Too many textures in mxamodel\n%s\n", Filename );
		return FALSE;
	}
	
	if ( Mxaloadheader->num_texture_files !=0)
	{
		/*	put the filenames in the tex struct	*/
		for( i = 0;	 i < Mxaloadheader->num_texture_files; i++)
		{
			FileNamePnt = (char *) &Mxaloadheader->ImageFile[i];
			while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );

			if( LevelSpecific )
			{
				sprintf( &TempFilename[ 0 ], "data\\levels\\%s\\textures\\%s", &ShortLevelNames[ LevelNum ][ 0 ], &Mxaloadheader->ImageFile[i] );
			}
			else
			{
				sprintf( &TempFilename[ 0 ], "data\\textures\\%s", &Mxaloadheader->ImageFile[i] );
			}

			if( !_stricmp( "default.ppm", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\default.ppm" );
			if( !_stricmp( "pkups.ppm", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "pkupsa.ppm", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "default.bmp", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\default.ppm" );
			if( !_stricmp( "pkups.bmp", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "pkupsa.bmp", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "titana.bmp", &Mxaloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );

			if (Mxaloadheader->AllocateTPage & LOAD_TPAGES_SYSMEM)
			{
					if ( header_num > 0 )
					{
						Mxaloadheader->TloadIndex[i] = Mxaloadheaders[header_num - 1].TloadIndex[i];	// use same tpage as last model
						strcpy (SystemMemTPages[ CurrentSysTexture ].FileName, &TempFilename[0]);	// store name of tpage
					
						Mxaloadheader->SysTloadIndex[i] = CurrentSysTexture;
						SystemMemTPages[ CurrentSysTexture ].VidTPageIndex = Mxaloadheader->TloadIndex[i];
//bjd						SystemMemTPages[ CurrentSysTexture ].lpSrcTextureSurf = NULL;
						CurrentSysTexture++;

						if (CurrentSysTexture == MAXSYSTEMMEMTPAGES)
						{
							Msg("2dTextures.c: Load_Off_File() Max system mem t-pages exceeded\n");
							exit(1);
						}
					}
			}

			if (Mxaloadheader->AllocateTPage & LOAD_TPAGES_VIDMEM)
				Mxaloadheader->TloadIndex[i] = AddTexture( &Tloadheader , &TempFilename[0] , TRUE , TRUE ,FALSE, 0, 0 );		// dont colourkey
			
			if( Mxaloadheader->TloadIndex[i] == -1 )
			{
				Msg( "PreMxaload() Too many TPages\n" );
				return FALSE;
			}
		}
	
	}

	Mxaloadheader->Buffer = Buffer;

	return TRUE;
}

/*===================================================================
	Procedure	:		Interp between frames for MXA Anim...
	Input		:		MXALOADHEADER * Mxaloadheader 
				:		int FrameFrom, int FrameTo , float Interp
	Output		:		FLASE/TRUE
===================================================================*/
BOOL	InterpFrames( MXALOADHEADER * Mxaloadheader , int FromFrame, int ToFrame , float Interp )
{
//	D3DEXECUTEBUFFERDESC	debDesc;
    LPVOID lpBufStart = NULL;
	LPD3DLVERTEX	lpD3DLVERTEX;
	LPD3DLVERTEX	lpD3DLVERTEX2;
	MXAVERT *		FromVert;
	MXAVERT *		ToVert;
	uint16			texgroup;
	uint16			num_anim_vertices;
	int execbuf;
	int group;
	D3DCOLOR_RGBA * color;
	D3DCOLOR_RGBA * from_color;
	D3DCOLOR_RGBA * to_color;

	if ( Mxaloadheader->num_frames <= 1 ) return TRUE;

	for( group=0 ; group<Mxaloadheader->num_groups; group++)
	{
		for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
		{
/*
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
			/*	lock the execute buffer	*/
//			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( Mxaloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK) // bjd
//			if (FSLockExecuteBuffer(Mxaloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
//				return FALSE ;
			if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], lpBufStart)))
			{
				return FALSE;
			}
		
//			lpBufStart = debDesc.lpData;
		
			for( texgroup=0; texgroup < Mxaloadheader->Group[group].num_texture_groups[execbuf] ; texgroup++)
			{
				lpD3DLVERTEX = 	( LPD3DLVERTEX ) ( (char*) lpBufStart+( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
				lpD3DLVERTEX += 8;

				lpD3DLVERTEX2 = ( LPD3DLVERTEX ) ( (char*) Mxaloadheader->Group[group].org_vertpnt[execbuf] + ( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
				lpD3DLVERTEX2 += 8;
				
				FromVert = ( MXAVERT * ) Mxaloadheader->frame_pnts[FromFrame][group][execbuf][texgroup];
				ToVert = ( MXAVERT * ) Mxaloadheader->frame_pnts[ToFrame][group][execbuf][texgroup];

				for( num_anim_vertices = 0 ; num_anim_vertices <  Mxaloadheader->num_anim_vertices[FromFrame][group][execbuf][texgroup] ; num_anim_vertices++ )
				{
					if ( FromVert->flags & MXA_ANIM_POS )
					{
						lpD3DLVERTEX->x = FromVert->x + ( ToVert->x - FromVert->x ) * Interp;
						lpD3DLVERTEX->y = FromVert->y + ( ToVert->y - FromVert->y ) * Interp;
						lpD3DLVERTEX->z = FromVert->z + ( ToVert->z - FromVert->z ) * Interp;
					}
					if ( FromVert->flags & MXA_ANIM_RGB )
					{
						color = (D3DCOLOR_RGBA *) &lpD3DLVERTEX2->color;


						from_color = (D3DCOLOR_RGBA *) &FromVert->color;
						to_color = (D3DCOLOR_RGBA *) &ToVert->color;
						color->r = from_color->r + (int8) floor( ( to_color->r - from_color->r ) * Interp );
						color->g = from_color->g + (int8) floor( ( to_color->g - from_color->g ) * Interp );
						color->b = from_color->b + (int8) floor( ( to_color->b - from_color->b ) * Interp );
#ifdef DOES_NOT_WORK_FOR_BLASTER
						color->a = from_color->a + (int8) floor( ( to_color->a - from_color->a ) * Interp );
#endif
					}
					if ( FromVert->flags & MXA_ANIM_UV )
					{
						lpD3DLVERTEX->tu = FromVert->tu + ( ToVert->tu - FromVert->tu ) * Interp;
						lpD3DLVERTEX->tv = FromVert->tv + ( ToVert->tv - FromVert->tv ) * Interp;
					}

					lpD3DLVERTEX++;
					lpD3DLVERTEX2++;
					FromVert++;
					ToVert++;
				}
			}
#if 0
			/*	unlock the execute buffer	*/
			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxaloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return FALSE ;
#endif
			if (FAILED(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}


#if 0
/*===================================================================
	Procedure	:		Do the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
/*===================================================================
	Procedure	:		Do the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
float	WaterTime = 0.0F;
void WaterMesh()
{
	int x,y;
 	D3DEXECUTEBUFFERDESC	debDesc;
	LPD3DLVERTEX	lpD3DLVERTEX;
    LPVOID lpBufStart;
	int col;
	float dx, dy;
	float u, v;
	float * floatpnt;
	float temp;
	float	Tick,Damp;
	float Fn;

	if( !WaterEnable || NewLevelNum != WATERLEVEL )
		return;

	Damp = (float)pow( 1.0 - 0.01, framelag * framelag);
	Tick = 0.04F * framelag;

	WaterTime-=framelag;

	if(  WaterTime< 0.0F )
	{
		V[Random_Range(WATER)][Random_Range(WATER)] = -(2.0F + (float) Random_Range(2));
//		V[MESHX/2][MESHY/2] = -64.0F;
//		V[MESHX/2-1][MESHY/2] = -64.0F;
//		V[MESHX/2-1][MESHY/2-1] = -64.0F;
//		V[MESHX/2][MESHY/2-1] = -64.0F;
		WaterTime = 2.0F;// + (float) Random_Range(8);
//		WaterTime = 300;
	}

	
	memcpy( &Fc[0][0] , &FinalV[0][0] , sizeof(Fc) );

	for (x = 1; x< WATER; x++)
	{
		for (y=1; y<WATER; y++)
		{
			floatpnt = &Fc[x-1][y-1];
			temp = *floatpnt++ * ROOTTWO;
			temp += *floatpnt++;
			temp += *floatpnt * ROOTTWO;
			floatpnt += MESHX-2;
			
			temp += *floatpnt++;
			floatpnt++;
			temp += *floatpnt;
			
			floatpnt += MESHX-2;
			
			temp += *floatpnt++ * ROOTTWO;
			temp += *floatpnt++;
			temp += *floatpnt * ROOTTWO;

			Fn = temp * SPECIALROOTTWO;
	
//			Fn [x][y]=(roottwo*Fc[x-1][y-1]) + Fc[x][y-1] + (roottwo*Fc[x+1][y-1])+
//				Fc[x-1][y]    +   0.0F*Fc[x][y] +   Fc[x+1][y]   +
//				(roottwo*Fc[x-1][y+1]) + Fc[x][y+1] + (roottwo*Fc[x+1][y+1]) ;
//			Fn [x][y] = Fn [x][y]/(4.0F+roottwo*4.0F);

			V[x][y] += Tick * (Fn - Fc[x][y])  /* Accel */;
			V[x][y] *= Damp; /* Damping */
			FinalV[x][y] += V[x][y] * framelag;
//			FinalV[x][y] += Tick * V[x][y];
//			V[x][y] += TICK * (Fn[x][y] - Fc[x][y]) /* Accel */;
//			V[x][y] *= DAMP; /* Damping */
//			FinalV[x][y] += TICK * V[x][y];

		}
	}


	memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
	debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	
	/*	lock the execute buffer	*/
	if ( WaterExecbuf->lpVtbl->Lock( WaterExecbuf, &debDesc ) != D3D_OK)
		return;
	lpBufStart = debDesc.lpData;
	
	lpD3DLVERTEX = 	( LPD3DLVERTEX )lpBufStart;
	
	
	for( x = 0 ; x < MESHX ; x++ )
	{
		for( y = 0 ; y < MESHY ; y++ )
		{
			lpD3DLVERTEX->x = -( (MESHX * WATER_CELLSIZE ) * 0.5F ) + (x*WATER_CELLSIZE);
			lpD3DLVERTEX->z = -( (MESHY * WATER_CELLSIZE ) * 0.5F ) + (y*WATER_CELLSIZE);
			lpD3DLVERTEX->y = FinalV[x][y];

//			dx = FinalV[x][y+1] + FinalV[x][y-1] + 0.25F * ( FinalV[x-1][y-1] + FinalV[x-1][y+1] + FinalV[x+1][y-1] + FinalV[x+1][y+1] ) - 3 * FinalV[x][y];
//			dy = FinalV[x-1][y] + FinalV[x+1][y] + 0.25F * ( FinalV[x-1][y-1] + FinalV[x-1][y+1] + FinalV[x+1][y-1] + FinalV[x+1][y+1] ) - 3 * FinalV[x][y];

			if( x > 0 && x < MESHX-1 && y > 0 && y < MESHY-1 )
			{
				dx = FinalV[x+1][y] - FinalV[x-1][y] + 0.25F * ( FinalV[x+1][y-1] + FinalV[x+1][y+1] - FinalV[x-1][y-1] - FinalV[x-1][y+1] );
				dy = FinalV[x][y+1] - FinalV[x][y-1] + 0.25F * ( FinalV[x+1][y+1] + FinalV[x-1][y+1] - FinalV[x+1][y-1] - FinalV[x-1][y-1] );
			}else{
				dx = 0.0F;
				dy = 0.0F;
			}
			u = 0.5F * (float) ( x + dx * 0.025F) / MESHX;
			v = 0.5F * (float) ( y + dy * 0.025F) / MESHY;
			lpD3DLVERTEX->tu = u;
			lpD3DLVERTEX->tv = v;
//			col = (int)FinalV[x][y];
//			col += 128;
			col = 192 + (int) ( dy * 8 );
			if( col > 255 )
				col = 255;
			if( col < 0 )
				col = 0;
//			lpD3DLVERTEX->color = RGBA_MAKE( col , col , col , 128);
			lpD3DLVERTEX->color = RGBA_MAKE( col >> 1 , col >> 1 , col , 128);
			
			
			lpD3DLVERTEX++;
		}
	}
	/*	unlock the execute buffer	*/
	if ( WaterExecbuf->lpVtbl->Unlock( WaterExecbuf ) != D3D_OK)
		return;
	
}


/*===================================================================
	Procedure	:		Release the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void ReleaseWaterMesh()
{
	if( WaterExecbuf )
		XRELEASE(WaterExecbuf);
}
/*===================================================================
	Procedure	:		Display the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void DisplayWaterMesh()
{
	LPDIRECTDRAW lpDD = d3dapp->lpDD;
	LPDIRECT3D lpD3D = d3dapp->lpD3D;
	LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
	LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;

	if( !WaterEnable || NewLevelNum != WATERLEVEL )
		return;

	TempWorld._11 = MATRIX_Identity._11;
	TempWorld._12 = MATRIX_Identity._12;
	TempWorld._13 = MATRIX_Identity._13;
	TempWorld._14 = MATRIX_Identity._14;
	TempWorld._21 = MATRIX_Identity._21;
	TempWorld._22 = MATRIX_Identity._22;
	TempWorld._23 = MATRIX_Identity._23;
	TempWorld._24 = MATRIX_Identity._24;
	TempWorld._31 = MATRIX_Identity._31;
	TempWorld._32 = MATRIX_Identity._32;
	TempWorld._33 = MATRIX_Identity._33;
	TempWorld._34 = MATRIX_Identity._34;
	TempWorld._41 = WaterPos.x;
	TempWorld._42 = WaterPos.y;
	TempWorld._43 = WaterPos.z;
	TempWorld._44 = MATRIX_Identity._44;
	if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &TempWorld) != D3D_OK)
		return;
	/*	Execute it	*/
	if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, WaterExecbuf, d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
		return;
	if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &identity) != D3D_OK)
		return;
}
/*===================================================================
	Procedure	:		Init the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitWaterMesh()
{
	int x,y;
	D3DEXECUTEDATA			d3dExData;
	D3DEXECUTEBUFFERDESC	debDesc;
	LPD3DTRIANGLE	FacePnt;
	LPD3DLVERTEX	lpD3DLVERTEX;
	int			i;

    LPVOID lpBufStart, lpInsStart, lpPointer;

	for (x = 0; x< MESHX; x++)
	{
		for (y = 0; y< MESHY; y++)
		{
			V[x][y] = 0.0F;
			FinalV[x][y] = 0.0F;

		}

	}
 
	V[MESHX/2][MESHY/2] = 8.0F;
	V[MESHX/2-1][MESHY/2] = 8.0F;
	V[MESHX/2-1][MESHY/2-1] = 8.0F;
	V[MESHX/2][MESHY/2-1] = 8.0F;
	
	WaterTime = 320.0F;

	if (MakeExecuteBuffer( &debDesc, d3dappi.lpD3DDevice , &WaterExecbuf , 65535 ) != TRUE )
		return;


	memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
	debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	
	/*	lock the execute buffer	*/
	if ( WaterExecbuf->lpVtbl->Lock( WaterExecbuf, &debDesc ) != D3D_OK)
		return;
	lpBufStart = debDesc.lpData;
	lpPointer = lpBufStart;
	lpD3DLVERTEX = (LPD3DLVERTEX ) lpPointer;

	
	for ( i=0 ; i<MESHX*MESHY; i++)
	{
		lpD3DLVERTEX->x = 0.0F;
		lpD3DLVERTEX->y = 0.0F;
		lpD3DLVERTEX->z = 0.0F;
		lpD3DLVERTEX->tu = 0.0F;
		lpD3DLVERTEX->tv = 0.0F;
		lpD3DLVERTEX->color = RGBA_MAKE(128,128,128,128);
		lpD3DLVERTEX->specular = RGBA_MAKE(128,128,128,255);
		lpD3DLVERTEX->dwReserved = 0;
		lpD3DLVERTEX++;
	}
	lpPointer = (void * )  lpD3DLVERTEX;			
	lpInsStart = lpPointer;
	OP_STATE_LIGHT(1, lpPointer);
		STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[WATERTPAGE], lpPointer);
	OP_PROCESS_VERTICES(1, lpPointer);
		PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, 0, MESHX*MESHY, lpPointer);
	OP_STATE_RENDER(1, lpPointer);
		STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[WATERTPAGE], lpPointer);
	if (QWORD_ALIGNED(lpPointer))
		OP_NOP(lpPointer);
	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE, lpPointer );
	}
	OP_TRIANGLE_LIST( (short) (MESHX-1)*(MESHY-1)*2, lpPointer);
				


	FacePnt = (LPD3DTRIANGLE ) lpPointer;
	/*	copy the faces data into the execute buffer	*/

	for( x = 0 ; x < MESHX-1 ; x++ )
	{
		for( y = 0 ; y < MESHY-1 ; y++ )
		{
			i = x*MESHY + y;
			FacePnt->v1 = i+MESHY+1;
			FacePnt->v2 = i+MESHY;
			FacePnt->v3 = i;
			FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			FacePnt++;
			FacePnt->v1 = i+1;
			FacePnt->v2 = i+MESHY+1;
			FacePnt->v3 = i;
			FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			FacePnt++;
		}
	}
	lpPointer = (LPVOID) FacePnt;
	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW, lpPointer );
	}
	OP_EXIT(lpPointer);
			
	/*	unlock the execute buffer	*/
	if ( WaterExecbuf->lpVtbl->Unlock( WaterExecbuf ) != D3D_OK)
		return;

	/*	set the data for the execute buffer	*/
	memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
	d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
	d3dExData.dwVertexCount = (MESHX*MESHY);
	d3dExData.dwInstructionOffset = (ULONG) ((char *)lpInsStart - (char *)lpBufStart);
	d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
	if ( WaterExecbuf->lpVtbl->SetExecuteData(WaterExecbuf, &d3dExData) != D3D_OK)
		return;
}

/*===================================================================
	Procedure	:		Collide the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
BOOL WaterCollide( VECTOR *Origin, VECTOR *Offset, VECTOR *CollidePos , uint16 Damage)
{
	float	WaterOffset;
	float	Div;
	float	Num;
	float	t;
	VECTOR	Delta;
	float	WaterX;
	float	WaterY;
	int		IWaterX;
	int		IWaterY;
	float	dx;
	float	dy;
	float	Impact;
	float	NewDamage;


	if( !WaterEnable || NewLevelNum != WATERLEVEL )
		return FALSE;
	
	WaterOffset = -DotProduct( &WaterNormal, &WaterPos );

/*===================================================================
	Calculate T
===================================================================*/
	Div = ( Offset->x * WaterNormal.x) + 
		  ( Offset->y * WaterNormal.y) + 
		  ( Offset->z * WaterNormal.z);
	
//	Div = ColDotProduct( &ODir , (NORMAL *) &WaterNormal->x );
	
//	if( Div >= 0.0F ) return FALSE;		/* Reject, Parallel */
	if( Div == 0.0F ) return FALSE;		/* Reject, Parallel */


	Num = ( ( Origin->x * WaterNormal.x ) +
		    ( Origin->y * WaterNormal.y ) +
		    ( Origin->z * WaterNormal.z ) ) + WaterOffset ; 
		
//	Num = ( D + ColDotProduct( &Origin , (NORMAL *) WaterNormal->x) );
	
	
	t = -( Num / Div );

/*===================================================================
	Do Polygon collision
===================================================================*/

	if( t < 0.0F ) return FALSE;		/* Intersection behind origin */
	if( t > 1.0F ) return FALSE;		/* Intersection Greater then ray length */

	CollidePos->x = ( Origin->x + ( Offset->x * t ) );
	CollidePos->y = ( Origin->y + ( Offset->y * t ) );
	CollidePos->z = ( Origin->z + ( Offset->z * t ) );

	Delta.x = CollidePos->x - WaterPos.x;
	Delta.y = CollidePos->y - WaterPos.y;
	Delta.z = CollidePos->z - WaterPos.z;

	// ideally, now we'd use the WaterNormal (and other parameters, such as water orientation in the plane)
	// to find out the correct WaterX and Y positions.  for now, though... [assuming WaterNormal is (0,1,0)]
	WaterX = ( Delta.x + ( (MESHX * WATER_CELLSIZE ) * 0.5F ) ) / WATER_CELLSIZE;
	WaterY = ( Delta.z + ( (MESHY * WATER_CELLSIZE ) * 0.5F ) ) / WATER_CELLSIZE;


	IWaterX = (int) floor( WaterX );
	IWaterY = (int) floor( WaterY );
	if ( IWaterX < 0 || IWaterX >= MESHX || IWaterY < 0 || IWaterY >= MESHY )
		return FALSE;

	NewDamage = Damage / 32.0F;
	if( NewDamage < 0.75F )
		NewDamage = 0.75F;
	
	Impact = WATER_IMPACT * NewDamage;
	if( Div >= 0.0F )
		Impact *= -0.25F;

	dx = 1.0F - (float) ( WaterX - floor( WaterX ) );
	dy = 1.0F - (float) ( WaterY - floor( WaterY ) );
	V[IWaterX][IWaterY] = Impact * ( dx + dy );
	if ( IWaterX + 1 < MESHX )
	{
		V[IWaterX+1][IWaterY] = Impact * ( ( 1 - dx ) + dy );
		if ( IWaterY + 1 < MESHY )
			V[IWaterX+1][IWaterY+1] = Impact * ( ( 1 - dx ) + ( 1 - dy ) );
	}
	if ( IWaterY + 1 < MESHY)
		V[IWaterX][IWaterY+1] = Impact * ( dx + ( 1 - dy ) );

	return TRUE;
}
#endif

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

