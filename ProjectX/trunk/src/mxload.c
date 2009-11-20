/*==========================================================================
 *
 *  File: Mxload.c
 *	loads in a bin....
 *  extracts the names of the ppm's and creates a suface and material...
 *  make an execution list per group of....
 *  make a list of D3DVERTEX's.....and TRIANGLE's....
***************************************************************************/

/*
Execution list format  (*.mx):

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
mxtype : uint16 // always 0 for mx format
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
#include "mxload.h"
#include "models.h"

#include "sfx.h"
#include "spotfx.h"
#include "xmem.h"
#include "util.h"

/*===================================================================
		Defines
===================================================================*/
#define	MX_VERSION_NUMBER	2

/*===================================================================
		Externals...
===================================================================*/

extern	uint32				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
extern	DWORD				CurrentSrcBlend;
extern	DWORD				CurrentDestBlend;
extern	DWORD				CurrentTextureBlend;

extern	TLOADHEADER Tloadheader;
extern	MODEL	Models[MAXNUMOFMODELS];

extern	BOOL	DrawPanel;
extern	GLOBALSHIP	Ships[MAX_PLAYERS];
extern	float	framelag;

/*===================================================================
		Globals...
===================================================================*/

#define	NUMOFPOLYSPER3DPANELITEM 2
POLYANIM * Panel3DPolyAnims_Primary[NUMOFPOLYSPER3DPANELITEM];
POLYANIM * Panel3DPolyAnims_Secondary[NUMOFPOLYSPER3DPANELITEM];
POLYANIM * Panel3DPolyAnims_Mine[NUMOFPOLYSPER3DPANELITEM];
POLYANIM * Panel3DPolyAnims_MineNum[NUMOFPOLYSPER3DPANELITEM];

void Check3DPanelPolyAnim(POLYANIM * PolyAnim );

extern void FixUV( LPTRIANGLE Tri, LPLVERTEX Vert, uint16 Tpage, LPLVERTEX Orig_Vert );
extern void FixUV_Anim( POLYANIM *PolyAnim, LPLVERTEX Vert, LPLVERTEX Orig_Vert );

/*===================================================================
		Function from title.c not #included due to dependences...
===================================================================*/
void TintDisc(uint16 Model);

//uint8 Red = 128;
//uint8 Green = 192;
//uint8 Blue = 192+64;

/*===================================================================
	Procedure	:		Load .Mx File
	Input		:		char	*	Filename , MXLOADHEADER *
	Output		:		Nothing
===================================================================*/
BOOL Mxload( char * Filename, MXLOADHEADER * Mxloadheader , BOOL Panel, BOOL StoreTriangles )
{

	char		*	Buffer;
	char		*	tempBuffer;
	int16		*	Int16Pnt;
	uint8		*	Uint8Pnt;
	uint16		*	Uint16Pnt;
	uint32		*	Uint32Pnt;
	uint16		*	Uint16Pnt2;
	float		*	FloatPnt;
	MFACE		*	MFacePnt;
//	LPTRIANGLE	FacePnt;
	TRIANGLE		FacePnt; // was a pointer
	LPTRIANGLE	TempFacePnt;
	uint16			exec_type = 0;			// the type of execute buffer
	uint16			texture_type = 0;		// the type of texture...0 normal  1 env
	uint16			num_vertices = 0;		// overall number of verts
	uint16			num_texture_groups = 0; // number of triangle groups
	uint16			num_triangles = 0;		// number of triangles in group
	uint16			group_vertex_start = 0; // where in the vert list to start processing
	uint16			group_vertex_num = 0;	// and how many to do...
	LPOLDLVERTEX lpLVERTEX2 = NULL;
	LPLVERTEX	lpLVERTEX = NULL;
	LPLVERTEX	lpBufStart = NULL;
	WORD			*lpIndices = NULL;
	int				ibIndex = 0;
	int				i,e,o;
	uint16			tpage;
	int				execbuf;
	int				group;
	uint16			ExecSize;
	uint16			animation,frames,vertices;
	POLYANIM	*	PolyAnim;
	TANIMUV		*	TanimUV;
	int			*	intPnt;
	uint16			num_animations;
	uint16			maxloops;
	uint16			animsize;
	int				r,g,b,a;
	COLOR		color;
	uint16			NumSpotFX;
	PVSPOTFX	*	SpotFXPtr;
	uint16			NumFirePoints;
	PVFIREPOINT *	FirePointPtr;
	uint32			Colour;
	int				colourkey = 0;

#if	MX_VERSION_NUMBER == 2
	int16			c;
	int8		*	Int8Pnt;
	int8			SFXFilename[ 128 ];
#endif

	int numTriangles = 0;
	int triangleCount = 0;
	int indexOffset = 0;
	int tempInt;

	DebugPrintf("Mxload - %s\n",Filename);

	// Mxloadheader is not valid until everything has been done..
	Mxloadheader->state = FALSE;

	if( Panel == TRUE )
	{
		if( DrawPanel == FALSE )
		{
			return TRUE;
		}
	}

	if( Panel )
	{
		// clear out the 3d Panel PolyAnim Data....
		for( i = 0 ; i < NUMOFPOLYSPER3DPANELITEM ; i++ )
		{
			Panel3DPolyAnims_Primary[i]=NULL;
			Panel3DPolyAnims_Secondary[i]=NULL;
			Panel3DPolyAnims_Mine[i]=NULL;
			Panel3DPolyAnims_MineNum[i]=NULL;
		}
	}

	Buffer = Mxloadheader->Buffer;
	if( Buffer == NULL)
	{
		Msg( "Mxload() Buffer was not allocated in %s\n", Filename );
		return FALSE;
	}

	/*	get the number of groups	*/
	Uint16Pnt = (uint16 *) Buffer;
	Mxloadheader->num_groups = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;

	if ( Mxloadheader->num_groups > MAXGROUPS )
	{
		Msg( "Mxload() Num groups > MAXGROUPS in %s\n", Filename );
		return FALSE;
	}

	for( group=0 ; group<Mxloadheader->num_groups; group++)
	{

		/*	get the number of execbufs in this group	*/
		Uint16Pnt = (uint16 *) Buffer;
		Mxloadheader->Group[group].num_execbufs = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;

		if ( Mxloadheader->Group[group].num_execbufs > MAXEXECBUFSPERGROUP )
		{
			Msg( "Mxload() numexecbufs > MAXEXECBUFSPERGROUP in %s\n", Filename );
			return FALSE;
		}

		for( execbuf=0 ; execbuf<Mxloadheader->Group[group].num_execbufs; execbuf++)
		{
			num_texture_groups = 0;
			Uint16Pnt = (uint16 *) Buffer;

			ExecSize = 	*Uint16Pnt++;
			ExecSize += 1024;

			/*	get the type of execution buffer	*/
			exec_type = *Uint16Pnt++;

			/*	get the number of verts in execution buffer	*/
			num_vertices =	*Uint16Pnt++;
			Buffer = (char *) Uint16Pnt;

			/*	record how what type of exec buffer	*/
			Mxloadheader->Group[group].exec_type[execbuf] = exec_type;

			/*	record how many verts there are in the exec buffer	*/
			Mxloadheader->Group[group].num_verts_per_execbuf[execbuf] = num_vertices;

			lpLVERTEX2 = (LPOLDLVERTEX ) Buffer;

			/*	create a vertex buffer	*/
			if (FAILED(FSCreateVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf], num_vertices)))
			{
				Msg( "Mxload : MakeExecBuffer Failed\n" );
				return FALSE;
			}

			DebugPrintf("created buffer to hold :%d verts\n", num_vertices);

			/*	lock the vertex buffer	*/
			if (FAILED(FSLockVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf], &lpLVERTEX)))
			{
				Msg( "Mxload() lock failed in %s\n", Filename );
				return FALSE;
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

				color = old->color;
				a = (color>>24)&255;
				r = (color>>16)&255;
				g = (color>>8)&255;
				b = color&255;

				/* bjd curr driver = 0 use to be software mode
				if(render_info.CurrDriver == 0) // is it or ramp mode..
				{
						a = 128;
				}
				else
				*/
				{
#if ACTUAL_TRANS
						a = 128;
#else
						a = 255;
#endif
				}

				color = RGBA_MAKE( r , g , b , a  );
				lpLVERTEX[i].color = color;

				lpLVERTEX[i].specular = RGB_MAKE( 0 , 0 , 0 );
				lpLVERTEX2->color = color;

				lpLVERTEX2++;
			}

			/* bjd - allows us to retrieve copies of the original vertices in the new format! */
			Mxloadheader->Group[group].originalVerts[execbuf] = malloc(sizeof(LVERTEX) * num_vertices);
			memcpy(Mxloadheader->Group[group].originalVerts[execbuf], &lpLVERTEX[0], sizeof(LVERTEX) * num_vertices);

			//	Stuff to clip if bounding box is off screen
/* bjd - CHECK
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

			if( Panel )
			{
				OP_STATE_RENDER( 1, lpPointer);
			    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE, lpPointer );
			}
*/

			Buffer = (char *) lpLVERTEX2;

			Uint16Pnt = (uint16 *) Buffer;
			num_texture_groups = *Uint16Pnt++;
			DebugPrintf("texture groups: %d\n", num_texture_groups);
			Buffer = (char *) Uint16Pnt;
			Mxloadheader->Group[group].num_texture_groups[execbuf] = num_texture_groups;


			triangleCount = 0;
			tempInt = 0;
			tempBuffer = Buffer;

			/* find out how many vertexes we'll need to load into our vertex buffer */
			for ( i=0 ; i<num_texture_groups; i++)
			{
				uint16 *temp = (uint16 *) tempBuffer;

				temp += 4;

				numTriangles = *temp++;

				tempBuffer = (char*) temp;
				MFacePnt = (MFACE *) tempBuffer;

				MFacePnt += numTriangles;
				tempBuffer = (char *) MFacePnt;
				triangleCount += numTriangles;
			}

			/*	create an index buffer	*/
			if (FAILED(FSCreateIndexBuffer(&Mxloadheader->Group[group].renderObject[execbuf], triangleCount * 3)))
			{
				return FALSE;
			}

			DebugPrintf("created index buffer to hold :%d incidices\n", triangleCount * 3);

			/*	lock the index buffer	*/
			if (FAILED(FSLockIndexBuffer(&Mxloadheader->Group[group].renderObject[execbuf], &lpIndices)))
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
				DebugPrintf("trinagles %d\n", num_triangles);
				Buffer = (char *) Uint16Pnt;

				Mxloadheader->Group[group].texture_group_vert_off[execbuf][i] = (uint32) (group_vertex_start*sizeof(LVERTEX));
/* bjd - CHECK
				OP_STATE_LIGHT(1, lpPointer);

				if( Mxloadheader->num_texture_files == 0 ) STATE_DATA(D3DLIGHTSTATE_MATERIAL, 0, lpPointer);
				else STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[Mxloadheader->TloadIndex[tpage]], lpPointer);

				OP_PROCESS_VERTICES(1, lpPointer);
					PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, group_vertex_start+8, group_vertex_num, lpPointer);
				OP_STATE_RENDER(1, lpPointer);

				if( Mxloadheader->num_texture_files == 0 ) STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer);
				else STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[Mxloadheader->TloadIndex[tpage]], lpPointer);

			    if (QWORD_ALIGNED(lpPointer))
					OP_NOP(lpPointer);

				OP_TRIANGLE_LIST( (short) num_triangles, lpPointer);
*/
				MFacePnt = (MFACE *) Buffer;
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

					if ( MFacePnt->pad & 1 )
					{
						// colourkey triangle found
						colourkey++;
						MFacePnt->pad &= ~1;
					}
//					FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					FixUV( &FacePnt, lpBufStart, tpage, Mxloadheader->Group[group].originalVerts[execbuf]);
//					FacePnt++;
					//lpLVERTEX+=3;
					MFacePnt++;
					tempInt+=3;
				}
				Buffer = (char *) MFacePnt;

				// BJD - check this. correct value to place here?
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numVerts = num_vertices;//num_triangles * 3;//num_triangles * 3;
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].numTriangles = num_triangles;

				/* keep track of our offset into our index buffer */
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].startIndex = indexOffset;
				indexOffset += num_triangles * 3;

				if( Mxloadheader->num_texture_files == 0 )
				{
					Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = NULL;
					Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = FALSE;
				}
				else
				{
					Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = Tloadheader.lpTexture[Mxloadheader->TloadIndex[tpage]];
					Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = Tloadheader.ColourKey[Mxloadheader->TloadIndex[tpage]];
				}
			}

			DebugPrintf("put %d indices into IB\n", tempInt);

			// bjd - probably should be part of the render object.
			if( Panel )
			{
				reset_cull();
			}

			if (StoreTriangles)
			{	Mxloadheader->Group[group].num_polys_per_execbuf[execbuf] = triangleCount;//num_triangles;
				Mxloadheader->Group[group].poly_ptr[execbuf] = (LPTRIANGLE)malloc( sizeof (TRIANGLE) * /*num_triangles*/triangleCount);

				memcpy(Mxloadheader->Group[group].poly_ptr[execbuf], TempFacePnt, sizeof (TRIANGLE) * /*num_triangles*/triangleCount);

			}
			else
			{
				Mxloadheader->Group[group].num_polys_per_execbuf[execbuf] = 0;
				Mxloadheader->Group[group].poly_ptr[execbuf] = NULL;
			}

//			OP_STATE_RENDER( 1, lpPointer);
//				STATE_DATA(D3DRENDERSTATE_FOGENABLE, FALSE, lpPointer );
//			OP_EXIT(lpPointer);

			//free(vertexArray);
			//vertexArray = NULL;

			/*	unlock the vertex buffer	*/
			if (FAILED(FSUnlockVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() vb unlock failed in %s\n", Filename );
				return FALSE ;
			}

			/*	unlock the index buffer	*/
			if (FAILED(FSUnlockIndexBuffer(&Mxloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() ib unlock failed in %s\n", Filename );
				return FALSE ;
			}

			Mxloadheader->Group[group].renderObject[execbuf].numTextureGroups = num_texture_groups;


			/* update the renderObject */
//			Mxloadheader->Group[ group ].renderObject[execbuf].numVerts = num_vertices;
//			Mxloadheader->Group[ group ].renderObject[execbuf].numTriangles = triangleCount; //num_triangles;

			if( Mxloadheader->num_texture_files == 0 )
			{
				// ?
				//STATE_DATA(D3DLIGHTSTATE_MATERIAL, 0, lpPointer);
			}
			else
			{
				DebugPrintf("setting a material\n");
				Mxloadheader->Group[ group ].renderObject[execbuf].material = Tloadheader.lpMat[Mxloadheader->TloadIndex[tpage]];
				//STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[Mxloadheader->TloadIndex[tpage]], lpPointer);
			}
/*
			if( Mxloadheader->num_texture_files == 0 )
			{
				//Mxloadheader->Group[ group ].renderObject[execbuf].texture = NULL;
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = NULL;
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = FALSE;
			}
			else
			{
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].texture = Tloadheader.lpTexture[Mxloadheader->TloadIndex[tpage]];
				Mxloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].colourkey = Tloadheader.ColourKey[Mxloadheader->TloadIndex[tpage]];
				//STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[Mxloadheader->TloadIndex[tpage]], lpPointer);
			}
*/
		}
	}

	// Background Animation Stuff......
	Uint16Pnt = (uint16 *) Buffer;
	Uint16Pnt++;		// skip old mxv flag..
	num_animations = *Uint16Pnt++;
	Mxloadheader->AnimData.num_animations = num_animations;

	if( num_animations )
	{
		for( i = 0 ; i < num_animations ; i++ )
		{
			frames = *Uint16Pnt++;
			maxloops = *Uint16Pnt++;
			animsize = *Uint16Pnt++;
			Mxloadheader->AnimData.AnimSeq[i] = (uint16 *) malloc( animsize * sizeof( uint16 ) );
			Uint16Pnt2 = Mxloadheader->AnimData.AnimSeq[i];
			for( e = 0 ; e < animsize ; e++ )
			{
				*Uint16Pnt2++ = *Uint16Pnt++;
			}
		}
		Buffer = ( char * ) Uint16Pnt;

		for( group=0 ; group<Mxloadheader->num_groups; group++)
		{
			for( execbuf=0 ; execbuf<Mxloadheader->Group[group].num_execbufs; execbuf++)
			{
				// Animating background polys......
				Uint16Pnt = (uint16 *) Buffer;
				Mxloadheader->Group[group].num_animating_polys[execbuf] = *Uint16Pnt++;
				Buffer = ( char * ) Uint16Pnt;
				if( Mxloadheader->Group[group].num_animating_polys[execbuf] != 0 )
				{
					Mxloadheader->Group[group].polyanim[execbuf] = malloc( Mxloadheader->Group[group].num_animating_polys[execbuf] * sizeof( POLYANIM ) );


					PolyAnim = Mxloadheader->Group[group].polyanim[execbuf];
					if( !PolyAnim )
					{
						Msg( "Mxload() Polyanim failed in %s\n", Filename );
						return FALSE;
					}

					if (FAILED(FSLockVertexBuffer(&Mxloadheader->Group[ group ].renderObject[execbuf], &lpLVERTEX)))
					{
						Msg( "Mxload : Lock ExecBuffer failed\n" );
						return FALSE;
					}

					for( i = 0 ; i < Mxloadheader->Group[group].num_animating_polys[execbuf] ; i++ )
					{
						Uint8Pnt = (uint8 *) Buffer;
						for( e = 0 ; e < 32 ; e ++ )
						{
							PolyAnim->Tag[e] = *Uint8Pnt++;
						}
						Buffer = ( char * ) Uint8Pnt;

						Uint16Pnt = (uint16 *) Buffer;
						animation = *Uint16Pnt++;
						frames = *Uint16Pnt++;
						vertices = *Uint16Pnt++;


						PolyAnim->State = TEXTUREANIM_Go;
						PolyAnim->CurrentTime = 0.0F;
						PolyAnim->MasterTime = 0.0F;
						PolyAnim->CurrentOffset = 0;
						PolyAnim->currentframe = (uint16) -1;
						PolyAnim->newframe = 0;

						PolyAnim->animation =animation;
						PolyAnim->frames	=frames;
						PolyAnim->vertices	=vertices;

						PolyAnim->vert = NULL;
						PolyAnim->UVs = NULL;

						PolyAnim->vert = (int *) malloc( vertices * sizeof(int) );
						if( !PolyAnim->vert)
						{
							Msg( "Mxload() PolyAnim->Vert failed\n", Filename );
							return FALSE;
						}
						PolyAnim->UVs  = (TANIMUV *) malloc( vertices * frames * sizeof(TANIMUV) );
						if( !PolyAnim->UVs)
						{
							Msg( "Mxload() PolyAnim->UVs failed\n", Filename );
							return FALSE;
						}

						intPnt = PolyAnim->vert;
						TanimUV = PolyAnim->UVs;

						Buffer = ( char * ) Uint16Pnt;

						Uint16Pnt = (uint16 *) Buffer;
						for( e = 0 ; e < vertices  ; e++ )
						{
							*intPnt++ = *Uint16Pnt++;
						}
						Buffer = ( char * ) Uint16Pnt;

						FloatPnt = (float*) Buffer;
						for( o = 0 ; o < vertices*frames ; o++ )
						{
							TanimUV->u = *FloatPnt++;
							TanimUV->v = *FloatPnt++;
							TanimUV++;
						}
						Buffer = ( char * ) FloatPnt;

						FixUV_Anim( PolyAnim, lpLVERTEX, Mxloadheader->Group[group].originalVerts[execbuf] );

						if( Panel )
						{
							Check3DPanelPolyAnim( PolyAnim );
						}

						PolyAnim++;
					}

					if (FAILED(FSUnlockVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf])))
					{
						Msg( "Mxload : Unlock ExecBuffer failed\n" );
						return FALSE ;
					}
				}
			}
		}
	}
	else
	{
		Buffer = (char *) Uint16Pnt;
	}

/*===================================================================
	Point Direction Data
===================================================================*/
	Uint16Pnt = (uint16 *) Buffer;

	if( *Uint16Pnt++ )
	{
		NumFirePoints = *Uint16Pnt++;

		if( NumFirePoints )
		{
			Mxloadheader->NumFirePoints = NumFirePoints;
			Mxloadheader->FirePoints = (PVFIREPOINT *) malloc( NumFirePoints * sizeof( PVFIREPOINT ) );

			if( !Mxloadheader->FirePoints )
			{
				Mxloadheader->NumFirePoints = 0;
				return( FALSE );
			}

			FirePointPtr = Mxloadheader->FirePoints;

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
			Mxloadheader->FirePoints = NULL;
			Mxloadheader->NumFirePoints = 0;
		}

		NumSpotFX = *Uint16Pnt++;

		if( NumSpotFX )
		{
			Mxloadheader->NumSpotFX = NumSpotFX;
			Mxloadheader->SpotFX = (PVSPOTFX *) malloc( NumSpotFX * sizeof( PVSPOTFX ) );

			if( !Mxloadheader->SpotFX )
			{
				Mxloadheader->NumSpotFX = 0;
				return( FALSE );
			}

			SpotFXPtr = Mxloadheader->SpotFX;

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

#if	MX_VERSION_NUMBER == 2
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
			Mxloadheader->SpotFX = NULL;
			Mxloadheader->NumSpotFX = 0;
		}

		Buffer = (char *) Uint16Pnt;
	}
	else
	{
		Mxloadheader->NumFirePoints = 0;
		Mxloadheader->FirePoints = NULL;
		Mxloadheader->NumSpotFX = 0;
		Mxloadheader->SpotFX = NULL;
		Buffer = (char *) ( Uint16Pnt + 1 );
	}

	if ( colourkey )
		DebugPrintf( "Mxload: %d colourkey triangles found\n", colourkey );
	// Mxloadheader is valid and can be executed...
	Mxloadheader->state = TRUE;
	return( TRUE );
}
/*===================================================================
	Procedure	:		Execute all group buffers for a Mxloadheader
	Input		;		MXLOADHEADER *
				:		uint16		Models[] Index
	Output		:		FLASE/TRUE
===================================================================*/

BOOL ExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, uint16 Model  )
{
	int		i,e,f;
	int		group;
	RENDERMATRIX Matrix;
	BOOL	Display;
    LPLVERTEX	lpPointer = NULL;
	LPLVERTEX	lpLVERTEX = NULL;
	POLYANIM * PolyAnim;
	uint32 * uint32Pnt;
	TANIMUV * TanimUV;
	BOOL	VertBufferLocked;

	ModelTextureAnimation( Mxloadheader );

	if (Mxloadheader->state == TRUE )
	{
		for ( group=0 ; group<Mxloadheader->num_groups ; group++)
		{
			for ( i=0 ; i<Mxloadheader->Group[group].num_execbufs; i++)
			{
				VertBufferLocked = FALSE;

				if( Mxloadheader->Group[group].num_animating_polys[i] && Mxloadheader->AnimData.num_animations )
				{

					PolyAnim = Mxloadheader->Group[group].polyanim[i];

					for( f = 0 ; f < Mxloadheader->Group[group].num_animating_polys[i] ; f++ )
					{

						if( PolyAnim->currentframe != PolyAnim->newframe )
						{
							// something has changed....
							if( !VertBufferLocked )
							{
								if (FAILED(FSLockVertexBuffer(&Mxloadheader->Group[group].renderObject[i], &lpPointer)))
								{
									return FALSE;
								}

								VertBufferLocked = TRUE;
							}

							uint32Pnt = PolyAnim->vert;
							for( e = 0 ; e < PolyAnim->vertices ; e++ )
							{

								lpLVERTEX = lpPointer+ *uint32Pnt++;
								TanimUV = PolyAnim->UVs;
								TanimUV += e + (PolyAnim->vertices * PolyAnim->newframe);
								lpLVERTEX->tu = TanimUV->u;
								lpLVERTEX->tv = TanimUV->v;
							}
							PolyAnim->currentframe = PolyAnim->newframe;
						}
						PolyAnim++;
					}
				}
				if( VertBufferLocked )
				{
					// unlock it..
					if (FAILED(FSUnlockVertexBuffer(&Mxloadheader->Group[group].renderObject[i])))
					{
						return FALSE;
					}
					VertBufferLocked = FALSE;
				}

				if( Mxloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
				{

					if( Model != (uint16) -1 )
					{
						if( ( Models[ Model ].Flags & MODFLAG_Light ) )
						{
							if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
						}
#if 1
						switch( Models[ Model ].Func )
						{
							case MODFUNC_ScaleDonut:
								ProcessModel2( Mxloadheader, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );

							default:
								break;
						}
#endif
					}

					if (!FSGetWorld(&Matrix))
					{
						return FALSE;
					}

					if( Models[ Model ].Flags & MODFLAG_UseClipGroup )
					{
						AddTransExe( &Matrix , &Mxloadheader->Group[group].renderObject[i] , 0, Model, Models[ Model ].ClipGroup, Mxloadheader->Group[ group ].num_verts_per_execbuf[i] );
					}
					else
					{
						AddTransExe( &Matrix , &Mxloadheader->Group[group].renderObject[i] , 0, Model, Models[ Model ].Group, Mxloadheader->Group[ group ].num_verts_per_execbuf[i] );
					}
				}
				else
				{
					Display = TRUE;

					if( Model != (uint16) -1 )
					{
						if( ( Models[ Model ].Flags & MODFLAG_Light ) )
						{
							if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
						}

						switch( Models[ Model ].Func )
						{
							case MODFUNC_Explode:
								ProcessModel( Mxloadheader, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
								break;

							case MODFUNC_Scale:
							case MODFUNC_Scale2:
								ProcessModel( Mxloadheader, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
								break;
							case MODFUNC_Regen:
								ProcessModel( Mxloadheader, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
								break;

							case MODFUNC_OrbitPulsar:
								if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
								{
									if( !LightModel( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
								}
								break;

							case MODFUNC_ScaleDonut:
//								ProcessModel( Mxloadheader, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );
								break;

							case MODFUNC_RestoreColours:
								RestoreColourMxloadHeader(Mxloadheader );
								break;

							case MODFUNC_Nothing:
							default:
								break;
						}
					}

					if( Display )
					{
						if (FAILED(draw_object(&Mxloadheader->Group[group].renderObject[i])))
						{
							return FALSE;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

/*
 * ReleaseMxloadheader
 * Release Execute buffers
 */
void
ReleaseMxloadheader( MXLOADHEADER * Mxloadheader )
{
    int i,e;
    int group;
	POLYANIM * PolyAnim;

	if ( !Mxloadheader->state )
		return;

    for (group = 0; group < Mxloadheader->num_groups; group++)
	{
	    for (i = 0; i < Mxloadheader->Group[group].num_execbufs; i++)
		{
//		   	free(Mxloadheader->Group[group].org_colors[i]);
//		   	Mxloadheader->Group[group].org_colors[i]= NULL;

			FSReleaseRenderObject(&Mxloadheader->Group[group].renderObject[i]);

			if (Mxloadheader->Group[group].poly_ptr[i])
			{
				free(Mxloadheader->Group[group].poly_ptr[i]);
				Mxloadheader->Group[group].poly_ptr[i] = NULL;
			}

			PolyAnim = Mxloadheader->Group[group].polyanim[i];

			if( PolyAnim )
			{
				for( e = 0 ; e < Mxloadheader->Group[group].num_animating_polys[i] ; e++ )
				{

				   	if( PolyAnim )
				   	{
				   		if( PolyAnim->vert )
				   		{
				   			free( PolyAnim->vert );
				   			PolyAnim->vert = NULL;
				   		}
				   		if( PolyAnim->UVs )
				   		{
				   			free( PolyAnim->UVs );
				   			PolyAnim->UVs = NULL;
				   		}
				   	}
					PolyAnim++;
				}
		   		free(Mxloadheader->Group[group].polyanim[i]);
				Mxloadheader->Group[group].polyanim[i] = NULL;
			}
		}
	}

	for( i = 0 ; i < Mxloadheader->AnimData.num_animations ; i++ )
	{
		free(Mxloadheader->AnimData.AnimSeq[i]);
		Mxloadheader->AnimData.AnimSeq[i] = NULL;
	}

	if( Mxloadheader->SpotFX && Mxloadheader->NumSpotFX )
	{
		free( Mxloadheader->SpotFX );
		Mxloadheader->SpotFX = NULL;
		Mxloadheader->NumSpotFX = 0;
	}

	if( Mxloadheader->FirePoints && Mxloadheader->NumFirePoints )
	{
		free( Mxloadheader->FirePoints );
		Mxloadheader->FirePoints = NULL;
		Mxloadheader->NumFirePoints = 0;
	}

	if ( Mxloadheader->OrgAddr )
	{
		free( Mxloadheader->OrgAddr );
		Mxloadheader->OrgAddr = NULL;
	}

	Mxloadheader->state = FALSE;
}




/*===================================================================
	Procedure	:		Pre - Load .Mx File
	Input		:		char	*	Filename , MXLOADHEADER *
	Output		:		Nothing
===================================================================*/
extern char  ShortLevelNames[MAXLEVELS][32];
extern	int16		LevelNum;
BOOL PreMxload( char * Filename, MXLOADHEADER * Mxloadheader , BOOL Panel, BOOL LevelSpecific )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	uint16		*	Uint16Pnt;
	int			i;
	char	*	FileNamePnt;
	uint32		*	Uint32Pnt;
	uint32			MagicNumber;
	uint32			VersionNumber;
	int8			TempFilename[ 256 ];

	// Mxloadheader is not valid until everything has been done..
	Mxloadheader->state = FALSE;
	Mxloadheader->Buffer = NULL;


	if( Panel == TRUE )
	{
		if( DrawPanel == FALSE )
		{
			return TRUE;
		}
	}


	File_Size = Get_File_Size( Filename );

	if( !File_Size )
	{
		Msg( "PreMxload() File %s does not exist\n", Filename );
		return( FALSE );
	}

	Buffer = calloc( 1, File_Size + 32 );

	if( Buffer == NULL )
	{
		Msg( "PreMxload() Allocate buffer for %s failed\n", Filename );
		return( FALSE );
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
		Msg( "PreMxload() Error reading file %s\n", Filename );
		return( FALSE );
	}

	Mxloadheader->OrgAddr = Buffer;

	Uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MX_VERSION_NUMBER  ) )
	{
		Msg( "PreMxload() Incompatible model ( .MX ) file %s", Filename );
		return( FALSE );
	}

	Uint16Pnt = (uint16 *) Buffer;

	Mxloadheader->num_texture_files = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;

	if ( Mxloadheader->num_texture_files > MAXTPAGESPERMXLOAD )
	{
		Msg( "PreMxload() Too many textures in %s\n", Filename );
		return FALSE;
	}

	if ( Mxloadheader->num_texture_files !=0)
	{
		/*	put the filenames in the tex struct	*/
		for( i = 0;	 i < Mxloadheader->num_texture_files; i++)
		{
			FileNamePnt = (char *) &Mxloadheader->ImageFile[i];
			while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );

			if( LevelSpecific )
				GetLevelTexturePath( &TempFilename[ 0 ], &Mxloadheader->ImageFile[i][0], &ShortLevelNames[ LevelNum ][ 0 ] );
			else
				sprintf( &TempFilename[ 0 ], "data\\textures\\%s", &Mxloadheader->ImageFile[i][0] );

			Mxloadheader->TloadIndex[i] = AddTexture( &Tloadheader , &TempFilename[ 0 ], (uint16) (Panel ^ TRUE) , Panel ^ TRUE, FALSE, 0, 0 );
			if( Mxloadheader->TloadIndex[i] == -1 )
			{
				Msg( "PreMxload() Too many TPages\n" );
				return FALSE;
			}
		}

	}

	Mxloadheader->Buffer = Buffer;
	return( TRUE );
}



/*===================================================================
	Procedure	:		Execute all group buffers for a Mxloadheader
	Input		;		MXLOADHEADER *
				:		uint16		Models[] Index
	Output		:		FLASE/TRUE
===================================================================*/
BOOL ReallyExecuteMxloadHeader( MXLOADHEADER * Mxloadheader, uint16 Model )
{
	int			i;
	int			group;

	if (Mxloadheader->state == TRUE )
	{
		for ( group=0 ; group<Mxloadheader->num_groups ; group++)
		{
			for ( i=0 ; i<Mxloadheader->Group[group].num_execbufs; i++)
			{
				if (FAILED(draw_object(&Mxloadheader->Group[group].renderObject[i])))
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}



/*===================================================================
	Procedure	:		Handle All Animation For an Mloadheader
	Input		:		MLOADHEADER *
	Output		:		Nothing
===================================================================*/
void ModelTextureAnimation( MXLOADHEADER * Mxloadheader  )
{
	uint16		*	Uint16Pnt;
	int	i;
	int	execbuf;
	int	group;
	POLYANIM * PolyAnim;
	uint16 * AnimData;
	float	ExtraTime;
	if( !Mxloadheader->AnimData.num_animations )
		return;
	if( AnimOncePerFrame == Mxloadheader->AnimOncePerFrame )
		return;
	Mxloadheader->AnimOncePerFrame = AnimOncePerFrame;


	for( group=0 ; group<Mxloadheader->num_groups; group++)
	{
		for( execbuf=0 ; execbuf<Mxloadheader->Group[group].num_execbufs; execbuf++)
		{
			PolyAnim = Mxloadheader->Group[group].polyanim[execbuf];

			for( i = 0 ; i < Mxloadheader->Group[group].num_animating_polys[execbuf] ; i++ )
			{

				if( ( PolyAnim->State == TEXTUREANIM_Go ) &&
					( ( PolyAnim->CurrentTime -= framelag ) <= 0.0F ) )
				{
					ExtraTime = PolyAnim->CurrentTime;
					// time to tickover...
					while( (PolyAnim->State==TEXTUREANIM_Go) && (PolyAnim->CurrentTime<0.0F) )
					{
						// Take account of the overflow just incase that means another frame need to be displayed....
						PolyAnim->CurrentTime = PolyAnim->MasterTime + ExtraTime;
						ExtraTime += PolyAnim->MasterTime;
						if( ExtraTime > 0.0F )
							ExtraTime = 0.0F;
						AnimData = ( Mxloadheader->AnimData.AnimSeq[PolyAnim->animation] + PolyAnim->CurrentOffset );
						Uint16Pnt = HandleAnimCommands( PolyAnim , AnimData , Mxloadheader->AnimData.AnimSeq[PolyAnim->animation] );
						PolyAnim->CurrentOffset = Uint16Pnt - Mxloadheader->AnimData.AnimSeq[PolyAnim->animation];
					}

				}
   				PolyAnim++;
			}
		}
	}
}

/*===================================================================
	Procedure	:		Restore the coolours for a Mxloadheader..
	Input		;		MXLOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/

BOOL RestoreColourMxloadHeader( MXLOADHEADER * Mxloadheader1 )
{
	int		i,e;
	int		group;
//	D3DEXECUTEBUFFERDESC	debDesc1;
	LPLVERTEX	lpD3DLVERTEX1 = NULL;
//	COLOR * ColourPnt;
	LPLVERTEX			VertPtr;

	if( !Mxloadheader1->state )
		return FALSE;
	for ( group=0 ; group<Mxloadheader1->num_groups ; group++)
	{
		for ( i=0 ; i<Mxloadheader1->Group[group].num_execbufs; i++)
		{
//			memset(&debDesc1, 0, sizeof(D3DEXECUTEBUFFERDESC));
//			debDesc1.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			// lock the execute buffer
//			if ( Mxloadheader1->Group[group].lpExBuf[i]->lpVtbl->Lock( Mxloadheader1->Group[group].lpExBuf[i], &debDesc1 ) != D3D_OK) // bjd
//			if (FSLockExecuteBuffer(Mxloadheader1->Group[group].lpExBuf[i], &debDesc1 ) != D3D_OK)
//				return FALSE;
			if (FAILED(FSLockVertexBuffer(&Mxloadheader1->Group[group].renderObject[i], &lpD3DLVERTEX1)))
			{
				return FALSE;
			}

//			lpD3DLVERTEX1 = (LPLVERTEX) debDesc1.lpData;

			//ColourPnt = Mxloadheader1->Group[group].org_colors[i];
			VertPtr = Mxloadheader1->Group[group].originalVerts[i];

			for( e = 0 ; e < Mxloadheader1->Group[group].num_verts_per_execbuf[i] ; e++ )
			{
				//lpD3DLVERTEX1->color = *ColourPnt++;
				lpD3DLVERTEX1->color = VertPtr->color;
				VertPtr++;
				lpD3DLVERTEX1++;
			}

//			if ( Mxloadheader1->Group[group].lpExBuf[i]->lpVtbl->Unlock( Mxloadheader1->Group[group].lpExBuf[i] ) != D3D_OK)
//				return FALSE;
			if (FAILED(FSUnlockVertexBuffer(&Mxloadheader1->Group[group].renderObject[i])))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
/*===================================================================
	Procedure	:		CheckFor 3d Panel PolyAnims..
	Input		;		POLYANIM * PolyAnim
	Output		:		NOTHING
===================================================================*/
void Check3DPanelPolyAnim(POLYANIM * PolyAnim )
{
	int j;
	// Make a note of any polyanims that are to do with the 3D Panel.....
	if( _stricmp( "Primary" , (char*) &PolyAnim->Tag[0] ) == 0 )
	{
		for( j = 0 ; j < NUMOFPOLYSPER3DPANELITEM ; j++ )
		{
			if( Panel3DPolyAnims_Primary[j] == NULL )
				Panel3DPolyAnims_Primary[j] = PolyAnim;
		}
	}
	else if( _stricmp( "Secondary" , (char*) &PolyAnim->Tag[0] ) == 0 )
	{
		for( j = 0 ; j < NUMOFPOLYSPER3DPANELITEM ; j++ )
		{
			if( Panel3DPolyAnims_Secondary[j] == NULL )
				Panel3DPolyAnims_Secondary[j] = PolyAnim;
		}
	}
	else if( _stricmp( "Mine" , (char*) &PolyAnim->Tag[0] ) == 0 )
	{
		for( j = 0 ; j < NUMOFPOLYSPER3DPANELITEM ; j++ )
		{
			if( Panel3DPolyAnims_Mine[j] == NULL )
				Panel3DPolyAnims_Mine[j] = PolyAnim;
		}

	}
	else if( _stricmp( "MineNum" , (char*) &PolyAnim->Tag[0] ) == 0 )
	{
		for( j = 0 ; j < NUMOFPOLYSPER3DPANELITEM ; j++ )
		{
			if( Panel3DPolyAnims_MineNum[j] == NULL )
				Panel3DPolyAnims_MineNum[j] = PolyAnim;
		}
	}
}

