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

num_texture_files : u_int16_t
texture_file_name[num_texture_files] : '\0' separated strings
num_groups : u_int16_t
{
	num_exec_lists : u_int16_t
	{
	  exec_type : u_int16_t // 0 = entirely opaque, 1 = contains transparencies
	  num_vertices : u_int16_t
	  vertex(x,y,z,reserved,colour,specular,tu,tv)[num_vertices] : x, y, z float, others u_int32_t
	  num_texture_groups : u_int16_t
	  {
	    texture_type : u_int16_t // 0 = normal, 1 = environment mapped
		start_vertex : u_int16_t
		num_vertices : u_int16_t
		texture_no : u_int16_t
		num_triangles : u_int16_t
		triangles(v0,v1,v2,pad16,nx,ny,nz)[num_triangles] : v? u_int16_t, pad16 u_int16_t, n? float
	  }[num_texture_groups]
	}[num_exec_lists]
}[num_groups]
mxtype : u_int16_t // always 2 for mxa format
num_frames : u_int16_t // number of animation frames
{
	{
		{
			{
				num_anim_vertices : u_int16_t // number of animating vertices for this texture group
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
#include "main.h"
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
#include "util.h"
#include "oct2.h"

/*===================================================================
		Defines
===================================================================*/
#define	MXA_VERSION_NUMBER	2

//#define FIX_MXA_UV
#undef FIX_MXA_UV

/*===================================================================
		Externals...	
===================================================================*/

extern void FixUV( LPTRIANGLE Tri, LPLVERTEX Vert, u_int16_t Tpage, LPOLDLVERTEX Orig_Vert );

extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern	float	framelag;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	int16_t	BikeModels[ MAXBIKETYPES ];
extern	RENDERMATRIX view;
extern	RENDERMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	RENDERMATRIX  TempWorld;

/*===================================================================
		Globals...	
===================================================================*/
MXALOADHEADER MxaModelHeaders[MAXMXAMODELHEADERS];

static void
FixUV_MXA( u_int16_t vertices, MXAVERT *MXA_Vert, LPLVERTEX Vert, LPLVERTEX Orig_Vert )
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
bool Mxaload( char * Filename, MXALOADHEADER * Mxaloadheader, bool StoreTriangles  )
{

	char		*	Buffer;
	char		*	tempBuffer;
	int16_t		*	Int16Pnt;
	u_int32_t		*	Uint32Pnt;
	u_int16_t		*	Uint16Pnt;
	float		*	FloatPnt;
	MFACE		*	MFacePnt;
	TRIANGLE		FacePnt; // was a pointer
	LPTRIANGLE	TempFacePnt; ZERO_STACK_MEM(TempFacePnt);
	u_int16_t			exec_type;			// the type of execute buffer
	u_int16_t			texture_type;		// the type of texture...0 normal  1 env
	u_int16_t			num_vertices;		// overall number of verts
	u_int16_t			num_texture_groups;// number of triangle groups
	u_int16_t			num_triangles;		// number of triangles in group
	u_int16_t			group_vertex_start; // where in the vert list to start processing
	u_int16_t			group_vertex_num;	// and how many to do...
	LPOLDLVERTEX lpLVERTEX2;
	LPLVERTEX	lpLVERTEX;
	LPLVERTEX	lpBufStart = NULL;
	WORD			*lpIndices = NULL;
	NORMAL		*lpNormals = NULL;
	int				ibIndex = 0;
	u_int16_t			frame;
	u_int16_t			texgroup;
	u_int16_t			num_anim_vertices;
	int				i,e;
	u_int16_t			tpage = 0;
	int				execbuf;
	int				group;
	u_int16_t			ExecSize;

	u_int16_t			NumSpotFX;
	PVSPOTFX *		SpotFXPtr;
	u_int16_t			NumFirePoints;
	PVFIREPOINT *	FirePointPtr;
	u_int32_t			Colour;
	int				colourkey = 0;
#if	MXA_VERSION_NUMBER == 2
	int16_t			c;
	int8_t		*	Int8Pnt;
	int8_t			SFXFilename[ 128 ];
#endif

	int numTriangles = 0;
	int triangleCount = 0;
	int indexOffset = 0;
	int tempInt;

	// Mxaloadheader is not valid until everything has been done..
	Mxaloadheader->state = false;
	Mxaloadheader->WantedFrame = 0;
	Mxaloadheader->CurrentFrame = 0;
	Mxaloadheader->Interp = 0.0F;
	Mxaloadheader->Time = 0.0F;

	DebugPrintf("MxaLoad\n");
	
	Buffer = Mxaloadheader->Buffer;
	if( Buffer == NULL)
	{
		Msg( "Mxaload() Unable to allocate buffer in %s\n", Filename );
		return false;
	}
	

	/*	get the number of groups	*/
	Uint16Pnt = (u_int16_t *) Buffer;
	Mxaloadheader->num_groups = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		
	if ( Mxaloadheader->num_groups > MAXGROUPS )
	{
		Msg( "Mxaload() num_groups > MAXGROUPS\n", Filename );
		return false;
	}

	for( group=0 ; group<Mxaloadheader->num_groups; group++)
	{
		
		/*	get the number of execbufs in this group	*/
		Uint16Pnt = (u_int16_t *) Buffer;
		Mxaloadheader->Group[group].num_execbufs = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;		

		if ( Mxaloadheader->Group[group].num_execbufs > MAXEXECBUFSPERGROUP )
		{
			Msg( "Mxaload() num_execbufs > MAXEXECBUFSPERGROUP\n", Filename );
			return false;
		}
		
		for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
		{
			Uint16Pnt = (u_int16_t *) Buffer;

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

			lpLVERTEX2 = (LPOLDLVERTEX ) Buffer;

			//vertexArray = (LVERTEX*)malloc(sizeof(LVERTEX) * num_vertices);

			/*	create a vertex buffer	*/
			if (!FSCreateVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], num_vertices))
			{
				return false;
			}

			DebugPrintf("created buffer to hold :%d verts\n", num_vertices);

			/*	lock the vertex buffer	*/
			if (!(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpLVERTEX)))
			{
				Msg( "Mxload() lock failed in %s\n", Filename );
				return false;
			}

			lpBufStart = lpLVERTEX;

			/*	copy the vertex data into the execute buffer	*/
			for ( i=0 ; i<num_vertices; i++)
			{

				LPOLDLVERTEX old = lpLVERTEX2;

				lpLVERTEX[i].x = old->x;
				lpLVERTEX[i].y = old->y;
				lpLVERTEX[i].z = old->z;
				lpLVERTEX[i].tu = old->tu;
				lpLVERTEX[i].tv = old->tv;
				lpLVERTEX[i].color = old->color;

#if !ACTUAL_TRANS
				//lpLVERTEX->color |= 0xFF000000;
				lpLVERTEX[i].color |= 0xFF000000;
#endif
				// nothing used specular so we removed it from LVERTEX
				// lpLVERTEX->specular = lpLVERTEX2->specular;
//				lpLVERTEX->dwReserved = 0;
				lpLVERTEX2++;
			}
			/* bjd - allows us to retrieve copies of the original vertices in the new format! */
			Mxaloadheader->Group[group].originalVerts[execbuf] = malloc(sizeof(LVERTEX) * num_vertices);
			memmove(Mxaloadheader->Group[group].originalVerts[execbuf], &lpLVERTEX[0], sizeof(LVERTEX) * num_vertices);//memcpy
	
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
							   , 0, true , 0, lpPointer);
*/
			
			Buffer = (char *) lpLVERTEX2;
			
			Uint16Pnt = (u_int16_t *) Buffer;
			num_texture_groups = *Uint16Pnt++;    
			Buffer = (char *) Uint16Pnt;		
			Mxaloadheader->Group[group].num_texture_groups[execbuf] = num_texture_groups;

			/* find out how many vertexes we'll need to load into our vertex buffer */
			triangleCount = 0;
			tempInt = 0;
			tempBuffer = Buffer;

			for ( i=0 ; i<num_texture_groups; i++)
			{
				u_int16_t *temp = (u_int16_t *) tempBuffer;
				
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
			if (!FSCreateIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], triangleCount * 3))
			{
				return false;
			}

			DebugPrintf("created index buffer to hold :%d incidices\n", triangleCount * 3);

			/*	lock the index buffer	*/
			if (!(FSLockIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpIndices)))
			{
				Msg( "Mxload() lock failed in %s\n", Filename );
				return false;
			}

			if (!FSCreateNormalBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], triangleCount))
			{
				DebugPrintf("Mxload() failed to create normal buffer in %s\n", Filename);
				return false;
			}
			if (!(FSLockNormalBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpNormals)))
			{
				Msg( "Mxload() normal lock failed in %s\n", Filename );
				return false;
			}

			ibIndex = 0;
			indexOffset = 0;

			for ( i=0 ; i<num_texture_groups; i++)
			{
				Uint16Pnt = (u_int16_t *) Buffer;
				texture_type = *Uint16Pnt++;
				group_vertex_start = *Uint16Pnt++;
				group_vertex_num = *Uint16Pnt++;
				tpage = *Uint16Pnt++;
				num_triangles = *Uint16Pnt++;    
				Buffer = (char *) Uint16Pnt;		
	
				Mxaloadheader->Group[group].texture_group_vert_off[execbuf][i] = (u_int32_t) (group_vertex_start*sizeof(LVERTEX));

				MFacePnt = (MFACE *) Buffer;

				/* bjd - CHECK - might need to be start of buffer */
				TempFacePnt = (LPTRIANGLE ) /*lpLVERTEX*/lpIndices;

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

					lpNormals->nx = MFacePnt->nx;
					lpNormals->ny = MFacePnt->ny;
					lpNormals->nz = MFacePnt->nz;

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
					//lpLVERTEX+=3;
					//FacePnt++;
					MFacePnt++;
					lpNormals++;
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
				
					memmove(Mxaloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (TRIANGLE) * num_triangles);//memcpy
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
			
				memmove(Mxaloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (TRIANGLE) * /*num_triangles*/triangleCount);//memcpy
			}
			else
			{
				Mxaloadheader->Group[group].num_polys_per_execbuf[execbuf] = 0;
				Mxaloadheader->Group[group].poly_ptr[execbuf] = NULL;			
			}

			if (!(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxaload() unlock failed in %s\n", Filename );
				return false ;
			}

			/*	unlock the index buffer	*/
			if (!(FSUnlockIndexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() ib unlock failed in %s\n", Filename );
				return false ;
			}

			if (!(FSUnlockNormalBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() normal unlock failed in %s\n", Filename );
				return false ;
			}
/*
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numVerts = num_vertices;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numTriangles = num_triangles;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].startVert = 0;
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = Tloadheader.lpTexture[Mxaloadheader->TloadIndex[tpage]];
			Mxaloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = Tloadheader.ColourKey[Mxaloadheader->TloadIndex[tpage]];
*/

			Mxaloadheader->Group[group].renderObject[execbuf].numTextureGroups = num_texture_groups;

			//free(vertexArray);
			//vertexArray = NULL;
		}
	}
				
	Uint16Pnt = (u_int16_t *) Buffer;
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
				return false;
			}
			if ( !( Mxaloadheader->frame_pnts = (MXAVERTFRAME *) calloc( Mxaloadheader->num_frames, sizeof( MXAVERTFRAME ) ) ) )
			{
				Msg( "Mxaload() calloc(num_frames=%d) failed in %s\n", Mxaloadheader->num_frames, Filename );
				return false;
			}
			
			for( frame=0 ; frame<Mxaloadheader->num_frames; frame++)
			{
				for( group=0 ; group<Mxaloadheader->num_groups; group++)
				{
					for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
					{
						if (!(FSLockVertexBuffer(&Mxaloadheader->Group[ group ].renderObject[execbuf], &lpLVERTEX)))
						{
							Msg( "Mxaload : Lock VertexBuffer failed\n" );
							return false;
						}

						for( texgroup=0; texgroup < Mxaloadheader->Group[group].num_texture_groups[execbuf] ; texgroup++)
						{
							Uint16Pnt = (u_int16_t *) Buffer;
							num_anim_vertices = *Uint16Pnt++;
							Buffer = (char * ) Uint16Pnt;
							Mxaloadheader->num_anim_vertices[frame][group][execbuf][texgroup] = num_anim_vertices;
							Mxaloadheader->frame_pnts[frame][group][execbuf][texgroup] = (MXAVERT*) Buffer;
							lpLVERTEX = 	( LPLVERTEX ) ( lpLVERTEX +( (u_int32_t) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							lpLVERTEX += 8;
							//lpLVERTEX2 =	( LPOLDLVERTEX ) ( (char*) Mxaloadheader->Group[group].org_vertpnt[execbuf] +( (u_int32_t) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
							//lpLVERTEX2 += 8;
#ifdef FIX_MXA_UV
							FixUV_MXA( Mxaloadheader->num_anim_vertices[frame][group][execbuf][texgroup],
								Mxaloadheader->frame_pnts[frame][group][execbuf][texgroup],
								lpLVERTEX, lpLVERTEX2 );
#endif
							Buffer += ( num_anim_vertices * sizeof( MXAVERT ) );
						}

						if (!(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
						{
							Msg( "Mxaload : Unlock VertexBuffer failed\n" );
							return false ;
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
	Uint16Pnt = (u_int16_t *) Buffer;
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
				return( false );
			}

			PVPtr = Mxaloadheader->PVs;

			for( i = 0; i < NumPVs; i++ )
			{
				PVPtr->Type = *Uint16Pnt++;				// Type ( u_int16_t )
				PVPtr->ID = *Uint16Pnt++;				// ID ( u_int16_t )
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
				Uint16Pnt = (u_int16_t *) FloatPnt;
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
				return( false );
			}

			FirePointPtr = Mxaloadheader->FirePoints;

			for( i = 0; i < NumFirePoints; i++ )
			{
				FirePointPtr->ID = *Uint16Pnt++;				// ID ( u_int16_t )
				FloatPnt = (float *) Uint16Pnt;
#ifdef ARM
				memcpy(&FirePointPtr->Pos, FloatPnt, 4*3);				// Pos ( 3 floats )
				FloatPnt+=3;
				memcpy(&FirePointPtr->Dir, FloatPnt, 3*4);				// Dir ( 3 floats )
				FloatPnt+=3;
				memcpy(&FirePointPtr->Up, FloatPnt, 3*4);				// Up ( 3 floats )
				FloatPnt+=3;
#else
				FirePointPtr->Pos.x = *FloatPnt++;				// Pos ( 3 floats )
				FirePointPtr->Pos.y = *FloatPnt++;
				FirePointPtr->Pos.z = *FloatPnt++;
				FirePointPtr->Dir.x = *FloatPnt++;				// Dir ( 3 floats )
				FirePointPtr->Dir.y = *FloatPnt++;
				FirePointPtr->Dir.z = *FloatPnt++;
				FirePointPtr->Up.x = *FloatPnt++;				// Up ( 3 floats )
				FirePointPtr->Up.y = *FloatPnt++;
				FirePointPtr->Up.z = *FloatPnt++;
#endif
				Uint16Pnt = (u_int16_t *) FloatPnt;
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
				return( false );
			}

			SpotFXPtr = Mxaloadheader->SpotFX;

			for( i = 0; i < NumSpotFX; i++ )
			{
				SpotFXPtr->Type = *Uint16Pnt++;				// Type ( u_int16_t )
				FloatPnt = (float *) Uint16Pnt;
#ifdef ARM
				memcpy(&SpotFXPtr->Pos, FloatPnt, 4*3);
				FloatPnt+=3;
				memcpy(&SpotFXPtr->DirVector.x, FloatPnt, 4*3);
				FloatPnt+=3;
				memcpy(&SpotFXPtr->UpVector, FloatPnt, 4*3);
				FloatPnt+=3;
				memcpy(&SpotFXPtr->StartDelay, FloatPnt++, 4); SpotFXPtr->StartDelay*= ANIM_SECOND;	// Start Delay
				memcpy(&SpotFXPtr->ActiveDelay, FloatPnt++,4); SpotFXPtr->ActiveDelay*= ANIM_SECOND;	// Active Delay
				memcpy(&SpotFXPtr->InactiveDelay, FloatPnt++, 4); SpotFXPtr->InactiveDelay*= ANIM_SECOND;	// Inactive Delay
#else
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
#endif
				Int16Pnt = (int16_t *) FloatPnt;
				SpotFXPtr->Primary = (int8_t) *Int16Pnt++;	// Primary ( int16_t )
				SpotFXPtr->Secondary = (int8_t) *Int16Pnt++;	// Secondary ( int16_t )
				Uint32Pnt = (u_int32_t *) Int16Pnt;
				Colour = *Uint32Pnt++;
#if	MXA_VERSION_NUMBER == 2
//				Int16Pnt = (int16_t *) Uint32Pnt;
//				SpotFXPtr->SoundFX = *Int16Pnt++;
//				Uint16Pnt = (u_int16_t *) Int16Pnt;

				Int8Pnt = (int8_t *) Uint32Pnt;

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
#ifdef ARM
				memcpy(&SpotFXPtr->SoundFXVolume, FloatPnt++, 4);
				memcpy(&SpotFXPtr->SoundFXSpeed, FloatPnt++, 4);
#else
				SpotFXPtr->SoundFXVolume = *FloatPnt++;
				SpotFXPtr->SoundFXSpeed = *FloatPnt++;
#endif
				Uint16Pnt = (u_int16_t *) FloatPnt;
#else
				Uint16Pnt = (u_int16_t *) Uint32Pnt;
#endif

				SpotFXPtr->Red = (u_int8_t) ( ( Colour >> 16 ) & 255 );
				SpotFXPtr->Green = (u_int8_t) ( ( Colour >> 8 ) & 255 );
				SpotFXPtr->Blue = (u_int8_t) ( Colour & 255 );
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
	Mxaloadheader->state = true;
	return( true );
}
#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:		Execute all group buffers for a Mxaloadheader
	Input		;		MXALOADHEADER *
	Output		:		FLASE/true
===================================================================*/

bool ExecuteMxaloadHeader( MXALOADHEADER * Mxaloadheader, u_int16_t in_group  )
{
	int i;
	int group;
	RENDERMATRIX Matrix;
	if (Mxaloadheader->state == true )
	{
		for ( group=0 ; group<Mxaloadheader->num_groups ; group++)
		{
			for ( i=0 ; i<Mxaloadheader->Group[group].num_execbufs; i++)
			{
				if( Mxaloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
				{
					if (!FSGetWorld(&Matrix))
					{
						return false;
					}
					AddTransExe( &Matrix , &Mxaloadheader->Group[group].renderObject[i] , 0, (u_int16_t) -1, in_group, Mxaloadheader->Group[ group ].num_verts_per_execbuf[i] );
				}
				else
				{
					if (!draw_object(&Mxaloadheader->Group[group].renderObject[i]))
					{
						return false;
					}
				}
			}
		}
	}
	return true;
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

	Mxaloadheader->state = false;
}




/*===================================================================
	Procedure	:		Pre - Load .Mxa File
	Input		:		char	*	Filename , MXALOADHEADER *
	Output		:		Nothing
===================================================================*/
extern	int16_t		LevelNum;
bool PreMxaload( char * Filename, MXALOADHEADER * Mxaloadheaders, int header_num, bool LevelSpecific )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	u_int16_t		*	Uint16Pnt;
	int			i;
	char	*	FileNamePnt;
	MXALOADHEADER * Mxaloadheader;
	u_int32_t		*	Uint32Pnt;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;
	int8_t			TempFilename[ 256 ];

	Mxaloadheader = &Mxaloadheaders[header_num];

	// Mxaloadheader is not valid until everything has been done..
	Mxaloadheader->state = false;
	Mxaloadheader->Buffer = NULL;

	File_Size = Get_File_Size( Filename );	

	if( !File_Size )
	{
		Msg( "PreMxaload() File %s not found\n", Filename );
		return( false );
	}

	Buffer = calloc( 1, File_Size + 32 );

	if( Buffer == NULL )
	{
		Msg( "PreMxaload() Unable to allocate buffer in %s\n", Filename );
		return( false );
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
		Msg( "PreMxaload() Error reading %s\n", Filename );
		return( false );
	}

	Mxaloadheader->OrgAddr = Buffer;

	Uint32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MXA_VERSION_NUMBER  ) )
	{
		Msg( "PreMxaload() Incompatible model( .MXA ) file %s", Filename );
		return( false );
	}

	Uint16Pnt = (u_int16_t *) Buffer;

	Mxaloadheader->num_texture_files = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		

	if ( Mxaloadheader->num_texture_files > MAXTPAGESPERMXALOAD )
	{
		Msg( "PreMxaload() Too many textures in mxamodel\n%s\n", Filename );
		return false;
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
				Mxaloadheader->TloadIndex[i] = AddTexture( &Tloadheader , &TempFilename[0] , true , true ,false, 0, 0 );
			
			if( Mxaloadheader->TloadIndex[i] == -1 )
			{
				Msg( "PreMxaload() Too many TPages\n" );
				return false;
			}
		}
	}

	Mxaloadheader->Buffer = Buffer;

	return true;
}

/*===================================================================
	Procedure	:		Interp between frames for MXA Anim...
	Input		:		MXALOADHEADER * Mxaloadheader 
				:		int FrameFrom, int FrameTo , float Interp
	Output		:		FLASE/true
===================================================================*/
bool	InterpFrames( MXALOADHEADER * Mxaloadheader , int FromFrame, int ToFrame , float Interp )
{
    LPLVERTEX	lpBufStart = NULL;
	LPLVERTEX	lpLVERTEX;
	LPLVERTEX	lpLVERTEX2;
	MXAVERT *		FromVert;
	MXAVERT *		ToVert;
	u_int16_t			texgroup;
	u_int16_t			num_anim_vertices;
	int execbuf;
	int group;
	COLOR_RGBA * color;
	COLOR_RGBA * from_color;
	COLOR_RGBA * to_color;

	if ( Mxaloadheader->num_frames <= 1 ) return true;

	for( group=0 ; group<Mxaloadheader->num_groups; group++)
	{
		for( execbuf=0 ; execbuf<Mxaloadheader->Group[group].num_execbufs; execbuf++)
		{
			if (!(FSLockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf], &lpBufStart)))
			{
				return false;
			}
		
			for( texgroup=0; texgroup < Mxaloadheader->Group[group].num_texture_groups[execbuf] ; texgroup++)
			{
				lpLVERTEX = 	( LPLVERTEX ) ( (char*) lpBufStart+( (u_int32_t) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
				lpLVERTEX += 8;

				lpLVERTEX2 = ( LPLVERTEX ) ( (char*) Mxaloadheader->Group[group].originalVerts[execbuf] + ( (u_int32_t) Mxaloadheader->Group[group].texture_group_vert_off[execbuf][texgroup]));
				lpLVERTEX2 += 8;
				
				FromVert = ( MXAVERT * ) Mxaloadheader->frame_pnts[FromFrame][group][execbuf][texgroup];
				ToVert = ( MXAVERT * ) Mxaloadheader->frame_pnts[ToFrame][group][execbuf][texgroup];

				for( num_anim_vertices = 0 ; num_anim_vertices <  Mxaloadheader->num_anim_vertices[FromFrame][group][execbuf][texgroup] ; num_anim_vertices++ )
				{
#ifdef ARM
					MXAVERT fromVert, toVert;
					memcpy(&fromVert, FromVert, sizeof(MXAVERT));
					memcpy(&toVert, ToVert, sizeof(MXAVERT));		// To have them correctly alligned for ARM...
#endif
					if ( FromVert->flags & MXA_ANIM_POS )
					{
#ifdef ARM
						lpLVERTEX->x = fromVert.x + ( toVert.x - fromVert.x ) * Interp;
						lpLVERTEX->y = fromVert.y + ( toVert.y - fromVert.y ) * Interp;
						lpLVERTEX->z = fromVert.z + ( toVert.z - fromVert.z ) * Interp;
#else
						lpLVERTEX->x = FromVert->x + ( ToVert->x - FromVert->x ) * Interp;
						lpLVERTEX->y = FromVert->y + ( ToVert->y - FromVert->y ) * Interp;
						lpLVERTEX->z = FromVert->z + ( ToVert->z - FromVert->z ) * Interp;
#endif
					}
					if ( FromVert->flags & MXA_ANIM_RGB )
					{
						color = (COLOR_RGBA *) &lpLVERTEX2->color;


						from_color = (COLOR_RGBA *) &FromVert->color;
						to_color = (COLOR_RGBA *) &ToVert->color;
						color->r = from_color->r + (int8_t) floor( ( to_color->r - from_color->r ) * Interp );
						color->g = from_color->g + (int8_t) floor( ( to_color->g - from_color->g ) * Interp );
						color->b = from_color->b + (int8_t) floor( ( to_color->b - from_color->b ) * Interp );
#ifdef DOES_NOT_WORK_FOR_BLASTER
						color->a = from_color->a + (int8_t) floor( ( to_color->a - from_color->a ) * Interp );
#endif
					}
					if ( FromVert->flags & MXA_ANIM_UV )
					{
#ifdef ARM
						lpLVERTEX->tu = fromVert.tu + ( toVert.tu - fromVert.tu ) * Interp;
						lpLVERTEX->tv = fromVert.tv + ( toVert.tv - fromVert.tv ) * Interp;
#else
						lpLVERTEX->tu = fromVert->tu + ( toVert->tu - fromVert->tu ) * Interp;
						lpLVERTEX->tv = fromVert->tv + ( toVert->tv - fromVert->tv ) * Interp;
#endif
					}

					lpLVERTEX++;
					lpLVERTEX2++;
					FromVert++;
					ToVert++;
				}
			}
#if 0
			/*	unlock the execute buffer	*/
			if ( Mxaloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxaloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return false ;
#endif
			if (!(FSUnlockVertexBuffer(&Mxaloadheader->Group[group].renderObject[execbuf])))
			{
				return false;
			}
		}
	}
	
	return true;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

