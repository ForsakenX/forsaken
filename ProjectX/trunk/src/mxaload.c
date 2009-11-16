/*==========================================================================
 *
 *  File: Mxaload.c
 *	loads in a bin....
 *  extracts the names of the ppm's and creates a suface and material...
 *  make an execution list per group of....
 *  make a list of D3DVERTEX's.....and TRIANGLE's....
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

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "triggers.h"
#include "pickups.h"
#include "mxload.h"
#include "mxaload.h"

#include "sfx.h"
#include "spotfx.h"
#include "xmem.h"
#include "util.h"

/*===================================================================
		Defines
===================================================================*/
#define	MXA_VERSION_NUMBER	2

//#define FIX_MXA_UV
#undef FIX_MXA_UV

/*===================================================================
		Externals...	
===================================================================*/

extern void FixUV( LPTRIANGLE Tri, LPLVERTEX Vert, uint16 Tpage, LPOLDLVERTEX Orig_Vert );

extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern	float	framelag;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	int16	BikeModels[ MAXBIKETYPES ];
extern	D3DMATRIX view;
extern	D3DMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	D3DMATRIX  TempWorld;

int16		NewLevelNum;

/*===================================================================
		Globals...	
===================================================================*/
MXALOADHEADER MxaModelHeaders[MAXMXAMODELHEADERS];

static void
FixUV_MXA( uint16 vertices, MXAVERT *MXA_Vert, LPLVERTEX Vert, LPLVERTEX Orig_Vert )
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

	char		*	Buffer;
	char		*	tempBuffer;
	int16		*	Int16Pnt;
	uint32		*	Uint32Pnt;
	uint16		*	Uint16Pnt;
	float		*	FloatPnt;
	MFACE		*	MFacePnt;
	TRIANGLE		FacePnt; // was a pointer
	LPTRIANGLE	TempFacePnt;
	uint16			exec_type;			// the type of execute buffer
	uint16			texture_type;		// the type of texture...0 normal  1 env
	uint16			num_vertices;		// overall number of verts
	uint16			num_texture_groups;// number of triangle groups
	uint16			num_triangles;		// number of triangles in group
	uint16			group_vertex_start; // where in the vert list to start processing
	uint16			group_vertex_num;	// and how many to do...
	LPOLDLVERTEX lpD3DLVERTEX2;
	LPLVERTEX	lpD3DLVERTEX;
	LPLVERTEX	lpBufStart = NULL;
	WORD			*lpIndices = NULL;
	int				ibIndex = 0;
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

	int numTriangles = 0;
	int triangleCount = 0;
	int indexOffset = 0;
	int tempInt;

	// Mxaloadheader is not valid until everything has been done..
	Mxaloadheader->state = FALSE;
	Mxaloadheader->WantedFrame = 0;
	Mxaloadheader->CurrentFrame = 0;
	Mxaloadheader->Interp = 0.0F;
	Mxaloadheader->Time = 0.0F;

	DebugPrintf("MxaLoad\n");
	
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

			lpD3DLVERTEX2 = (LPOLDLVERTEX ) Buffer;

			//vertexArray = (LVERTEX*)malloc(sizeof(LVERTEX) * num_vertices);

			/*	create a vertex buffer	*/
			if (FAILED(FSCreateVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], num_vertices)))
			{
				return FALSE;
			}

			DebugPrintf("created buffer to hold :%d verts\n", num_vertices);

			/*	lock the vertex buffer	*/
			if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpD3DLVERTEX)))
			{
				Msg( "Mxload() lock failed in %s\n", Filename );
				return FALSE;
			}

			lpBufStart = lpD3DLVERTEX;

			/*	copy the vertex data into the execute buffer	*/
			for ( i=0 ; i<num_vertices; i++)
			{

				LPOLDLVERTEX old = lpD3DLVERTEX2;

				lpD3DLVERTEX[i].x = old->x;
				lpD3DLVERTEX[i].y = old->y;
				lpD3DLVERTEX[i].z = old->z;
				lpD3DLVERTEX[i].tu = old->tu;
				lpD3DLVERTEX[i].tv = old->tv;
				lpD3DLVERTEX[i].color = old->color;

#if !ACTUAL_TRANS
				//lpD3DLVERTEX->color |= 0xFF000000;
				lpD3DLVERTEX[i].color |= 0xFF000000;
#endif
//				lpD3DLVERTEX->specular = lpD3DLVERTEX2->specular;
				//lpD3DLVERTEX->specular = RGB_MAKE( 0 , 0 , 0 );
				lpD3DLVERTEX[i].specular = RGB_MAKE( 0 , 0 , 0 );
//				lpD3DLVERTEX->dwReserved = 0;
				//lpD3DLVERTEX++;
				lpD3DLVERTEX2++;
			}

			/* bjd - allows us to retrieve copies of the original vertices in the new format! */
			Mxaloadheader->Group[group].originalVerts[execbuf] = malloc(sizeof(LVERTEX) * num_vertices);
			memcpy(Mxaloadheader->Group[group].originalVerts[execbuf], &lpD3DLVERTEX[0], sizeof(LVERTEX) * num_vertices);
	
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

			/* find out how many vertexes we'll need to load into our vertex buffer */
			triangleCount = 0;
			tempInt = 0;
			tempBuffer = Buffer;

			for ( i=0 ; i<num_texture_groups; i++)
			{
				uint16 *temp = (uint16 *) tempBuffer;
				
				/* skip over unwated stuff */
				temp += 4;

				numTriangles = *temp++;

				tempBuffer = (char*) temp;
				MFacePnt = (MFACE *) tempBuffer;

				MFacePnt += numTriangles;
				tempBuffer = (char *) MFacePnt; 
				triangleCount += numTriangles;
			}

			/*	create an index buffer	*/
			if (FAILED(FSCreateIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], triangleCount * 3)))
			{
				return FALSE;
			}

			DebugPrintf("created index buffer to hold :%d incidices\n", triangleCount * 3);

			/*	lock the index buffer	*/
			if (FAILED(FSLockIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpIndices)))
			{
				Msg( "Mxload() lock failed in %s\n", Filename );
				return FALSE;
			}

			ibIndex = 0;
			indexOffset = 0;

			for ( i=0 ; i<num_texture_groups; i++)
			{
				Uint16Pnt = (uint16 *) Buffer;
				texture_type = *Uint16Pnt++;
				group_vertex_start = *Uint16Pnt++;
				group_vertex_num = *Uint16Pnt++;
				tpage = *Uint16Pnt++;
				num_triangles = *Uint16Pnt++;    
				Buffer = (char *) Uint16Pnt;		
	
				Mxaloadheader->Group[group].texture_group_vert_off[execbuf][i] = (uint32) (group_vertex_start*sizeof(LVERTEX));

				MFacePnt = (MFACE *) Buffer;

				/* bjd - CHECK - might need to be start of buffer */
				TempFacePnt = (LPTRIANGLE ) /*lpD3DLVERTEX*/lpIndices;

				/*	copy the faces data into the execute buffer	*/
				for( e=0; e<num_triangles; e++)
				{
					FacePnt.v1 = MFacePnt->v1;
					FacePnt.v2 = MFacePnt->v2;
					FacePnt.v3 = MFacePnt->v3;

					lpIndices[ibIndex] = FacePnt.v1;
					ibIndex++;
					lpIndices[ibIndex] = FacePnt.v2;
					ibIndex++;
					lpIndices[ibIndex] = FacePnt.v3;
					ibIndex++;

					if ( MFacePnt->pad & 1 )
					{
						// colourkey triangle found
						colourkey++;
						MFacePnt->pad &= ~1;
					}
//					FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
//					FacePnt->wFlags = MFacePnt->pad;
#ifdef FIX_MXA_UV
					FixUV( &FacePnt, lpBufStart, tpage, Mxaloadheader->Group[group].originalVerts[execbuf] );
#endif
					//lpD3DLVERTEX+=3;
					//FacePnt++;
					MFacePnt++;
					tempInt+=3;
				}

				DebugPrintf("put %d verts into VB\n", tempInt);

				Buffer = (char *) MFacePnt;

				// BJD - check this. correct value to place here?
				Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numVerts = num_vertices;//num_triangles * 3;
				Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numTriangles = num_triangles;

				/* keep track of our offset into our index buffer */
				Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].startIndex = indexOffset;
				indexOffset += num_triangles * 3;

				Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = Tloadheader.lpTexture[Mxaloadheader->TloadIndex[tpage]];
				Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = Tloadheader.ColourKey[Mxaloadheader->TloadIndex[tpage]];
/*
				if (StoreTriangles)
				{	Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = num_triangles;			
					Mxaloadheader->Group[group].poly_ptr[execbuf] = (LPTRIANGLE)malloc( sizeof (TRIANGLE) * num_triangles);			
				
					memcpy(Mxaloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (TRIANGLE) * num_triangles);
				}
				else
				{
					Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = 0;
					Mxaloadheader->Group[group].poly_ptr[execbuf] = NULL;			
				}
*/
			}

			if (StoreTriangles)
			{	Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = /*num_triangles*/triangleCount;
				Mxaloadheader->Group[group].poly_ptr[execbuf] = (LPTRIANGLE)malloc( sizeof (TRIANGLE) * /*num_triangles*/triangleCount);			
			
				memcpy(Mxaloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (TRIANGLE) * /*num_triangles*/triangleCount);
			}
			else
			{
				Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = 0;
				Mxaloadheader->Group[group].poly_ptr[execbuf] = NULL;			
			}

			if (FAILED(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxaload() unlock failed in %s\n", Filename );
				return FALSE ;
			}

			/*	unlock the index buffer	*/
			if (FAILED(FSUnlockIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() ib unlock failed in %s\n", Filename );
				return FALSE ;
			}
/*
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numVerts = num_vertices;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numTriangles = num_triangles;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].startVert = 0;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = Tloadheader.lpTexture[Mxaloadheader->TloadIndex[tpage]];
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = Tloadheader.ColourKey[Mxaloadheader->TloadIndex[tpage]];
*/
			Mxaloadheader->Group[ group ].renderObject[execbuf].material = Tloadheader.lpMat[Mxaloadheader->TloadIndex[tpage]];

			Mxaloadheader->Group[group].renderObject[execbuf].numTextureGroups = num_texture_groups;

			//free(vertexArray);
			//vertexArray = NULL;
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
						if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[ group ].renderObject[execbuf], &lpD3DLVERTEX)))
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
							lpD3DLVERTEX = 	( LPLVERTEX ) ( lpD3DLVERTEX +( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							lpD3DLVERTEX += 8;
							//lpD3DLVERTEX2 =	( LPOLDLVERTEX ) ( (char*) Mxaloadheader->Group[group].org_vertpnt[execbuf] +( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							//lpD3DLVERTEX2 += 8;
#ifdef FIX_MXA_UV
							FixUV_MXA( Mxaloadheader->num_anim_vertices[frame][group][execbuf][texgroup],
								Mxaloadheader->frame_pnts[frame][group][execbuf][texgroup],
								lpD3DLVERTEX, lpD3DLVERTEX2 );
#endif
							Buffer += ( num_anim_vertices * sizeof( MXAVERT ) );
						}

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
				if( Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
				{
					if (FAILED(FSGetMatrix(D3DTS_WORLD, &Matrix)))
					{
						return FALSE;
					}
					AddTransExe( &Matrix , &Mxaloadheader->Group[group].renderObject[i] , 0, (uint16) -1, in_group, Mxaloadheader->Group[ group ].num_verts_per_execbuf[i] );
				}
				else
				{
					if (FAILED(draw_object(&Mxaloadheader->Group[group].renderObject[i])))
					{
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

/*
 * ReleaseMxaloadheader
 * Release Execute buffers
 */

void ReleaseMxaloadheader( MXALOADHEADER * Mxaloadheader )
{
    int i;
    int group;

	if ( !Mxaloadheader->state )
		return;
    for (group = 0; group < Mxaloadheader->num_groups; group++)
	{
	    for (i = 0; i < Mxaloadheader->Group[group].num_execbufs; i++)
		{
			FSReleaseRenderObject(&Mxaloadheader->Group[group].renderObject[i]);
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
extern char  ShortLevelNames[MAXLEVELS][32];
extern	int16		LevelNum;
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
				GetLevelTexturePath( &TempFilename[ 0 ], &Mxaloadheader->ImageFile[i][0], &ShortLevelNames[ LevelNum ][ 0 ] );
			else
				sprintf( &TempFilename[ 0 ], "data\\textures\\%s", &Mxaloadheader->ImageFile[i][0] );

			if (Mxaloadheader->AllocateTPage & LOAD_TPAGES)
				Mxaloadheader->TloadIndex[i] = AddTexture( &Tloadheader , &TempFilename[0] , TRUE , TRUE ,FALSE, 0, 0 );
			
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
    LPLVERTEX	lpBufStart = NULL;
	LPLVERTEX	lpD3DLVERTEX;
	LPLVERTEX	lpD3DLVERTEX2;
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
			if (FAILED(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpBufStart)))
			{
				return FALSE;
			}
		
			for( texgroup=0; texgroup < Mxaloadheader->Group[group].num_texture_groups[execbuf] ; texgroup++)
			{
				lpD3DLVERTEX = 	( LPLVERTEX ) ( (char*) lpBufStart+( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
				lpD3DLVERTEX += 8;

				lpD3DLVERTEX2 = ( LPLVERTEX ) ( (char*) Mxaloadheader->Group[group].originalVerts[execbuf] + ( (uint32) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
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

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

