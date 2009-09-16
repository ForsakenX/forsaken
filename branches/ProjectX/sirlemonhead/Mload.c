/*==========================================================================
 *
 *  File: Mload.c
 *	loads in a bin....
 *  extracts the names of the ppm's and creates a suface and material...
 *  make an execution list per group of....
 *  make a list of D3DVERTEX's.....and D3DTRIANGLE's....
***************************************************************************/

/*
Execution list format with visibility info and vertex index (*.mxv):

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
	
		num_animating_polys : uint16
		{
		  animation : uint16
		  frames : uint16
		  vertices : uint16
		  vertex[ vertices ] : uint16
		  {
		     {
		  	tu, tv : float
		     }[ vertices ]
		  }[ frames ]
		}[ num_animating_polys ]

	}[num_exec_lists]
}[num_groups]
num_animations : uint16
{
	frames : uint16
	maxloops : uint16
	animsize : uint16
	animdata[ animsize ] : uint16
}[ num_animations ]
mxvflag : uint16 // always 1 for mxv format
{
	group_name : '\0' separated string
	group_center(x, y, z) : all float
	group_half_size(x, y, z) : all float
	num_portals_in_group : uint16
	{
		num_vertices_in_portal : uint16
		vertex(x, y, z)[num_vertices_in_portal] : all float
		num_groups_visible_from_portal : uint16
		groups_visible_from_portal[num_groups_visible_from_portal] : uint16
	}[num_portals_in_group]
}[num_groups]
mxviflag : uint16 // always 1 for mxv format with vertex index
{
	num_exec_lists : uint16
	{
		cell_origin(x, y, z) : all float
		xcells, ycells, zcells : all uint16
		num_vertex_indices : uint16
		vertex_index[num_vertex_indices] : uint16 // points into exec buffer vertex[]
		vertex_cell(num_verts_in_cell : uint16
			start_vert_in_cell : uint16)[num_vertex_cells] // points into vertex_index[]
	}[num_exec_lists]
}[num_groups]
num_start_points : uint16
{
	x, y, z : float
	in_group : uint16
}[num_start_points]

*/




/*===================================================================
		Include File...	
===================================================================*/
#include "typedefs.h"
#include "main.h"
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "Local.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "mload.h"
#include "lights.h"
#include "triggers.h"
#include "magic.h"
#include "Xmem.h"

/*===================================================================
		Externals...	
===================================================================*/
extern	BOOL SWMonoChrome;
extern	TRIGGERMOD	*	TrigMods;
extern	PALETTEENTRY ppe[256];
extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern  D3DMATRIXHANDLE hWorld;
extern	BOOL	UsedStippledAlpha;
extern	MLOADHEADER Mloadheader;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	float framelag;
extern	uint32	AnimOncePerFrame;	// used for stuff that is displayed more than once in a single frame..

void DebugPrintf( char *fmt, ... );
BOOL FindGroupConnections( MLOADHEADER *m );
BOOL ReadGroupConnections( MLOADHEADER *m, char **pbuf );
void FreeGroupConnections( void );
void ReadSoundInfo( MLOADHEADER *m, char **pbuf );

/*===================================================================
		Defines
===================================================================*/
#define	MXV_VERSION_NUMBER	3

#define UV_FUDGE	(0.5F)
#define SAME_UV		(2.0F)

/*===================================================================
		Globals...	
===================================================================*/
float	UV_Fix = UV_FUDGE;
BOOL	AllWires = FALSE;
uint16	num_start_positions = 0;
uint16	last_start_position = 0;
GAMESTARTPOS		StartPositions[MAXSTARTPOSITIONS];			// pos and group info...

uint16	FirstStartPositionInGroup[MAXGROUPS];


float	SoundInfo[MAXGROUPS][MAXGROUPS];
BOOL	TempGroups[MAXGROUPS];
void InitSoundInfo( MLOADHEADER * Mloadheader );
uint16	GroupTris[ MAXGROUPS ];

/*===================================================================
	Procedure	:		Dont Know....
	Input		:		Who can tell...
	Output		:		int93???
===================================================================*/
static BOOL
read_visible( MLOADHEADER * Mloadheader, VISTREE *v, uint16 group, uint16 **Uint16PntPtr )
{
	BOOL ok;
	uint16 *ptr;
	uint16 vnum;

	ok = TRUE;
	ptr = *Uint16PntPtr;
	if ( *ptr < MAXPORTALSPERGROUP ) // nb cannot check against Mloadheader->Group[group].num_portals yet...
	{
		v->portal = &Mloadheader->Group[group].Portal[*ptr++];
		v->group = *ptr++;
		if ( v->group < Mloadheader->num_groups )
		{
			v->num_visible = *ptr++;
			if ( v->num_visible )
			{
				v->visible = (VISTREE *) calloc( v->num_visible, sizeof( VISTREE ) );
				if ( v->visible )
				{
					for ( vnum = 0; vnum < v->num_visible; vnum++ )
					{
						if ( !read_visible( Mloadheader, &v->visible[ vnum ], v->group, &ptr ) )
						{
							ok = FALSE;
							break;
						}
					}
				}
				else
					ok = FALSE;
			}
			else
				v->visible = NULL;
		}
		else
			ok = FALSE;
	}
	else
		ok = FALSE;
	*Uint16PntPtr = ptr;
	return ok;
}
/*===================================================================
	Procedure	:		Build a Gamma Correction table 
	Input		:		double GammaValue
	Output		:		Nothing
===================================================================*/
BYTE  Tab[256];
void BuildGammaCorrect( double GammaValue )
{
	double k;
	int i;

	// recover in release build
	if (GammaValue <= 0)
	    GammaValue = 1.0;
	
	k = 255.0/pow(255.0, 1.0/GammaValue);
	
	for (i = 0; i <= 255; i++)
	{
	    Tab[i] = (BYTE)(k*(pow((double)i, 1.0/GammaValue)));
	}
};

void
FixUV( LPD3DTRIANGLE Tri, LPD3DLVERTEX Vert, uint16 Tpage, LPD3DLVERTEX Orig_Vert )
{
	static LPD3DLVERTEX TriVert[ 3 ], Orig_TriVert[ 3 ];
	static float u[ 3 ], v[ 3 ];
	float du, dv;
	int j, k;
	int zu, zv, uz, vz;
	float su, sv;
	int Xsize, Ysize;

	Xsize = Tloadheader.Xsize[Tpage] / ( 1 << Tloadheader.CurScale[Tpage] );
	Ysize = Tloadheader.Ysize[Tpage] / ( 1 << Tloadheader.CurScale[Tpage] );

/* bjd - TODO - store D3D9 caps in the struct and check that
	if( d3dappi.Driver[d3dappi.CurrDriver].bSquareOnly )
	{
		if( Xsize != Ysize )
		{
			if( Xsize > Ysize )
				Xsize = Ysize;
			if( Ysize > Xsize )
				Ysize = Xsize;
		}
	}
*/
	u[ 0 ] = Vert[ Tri->v1 ].tu;
	u[ 1 ] = Vert[ Tri->v2 ].tu;
	u[ 2 ] = Vert[ Tri->v3 ].tu;
	v[ 0 ] = Vert[ Tri->v1 ].tv;
	v[ 1 ] = Vert[ Tri->v2 ].tv;
	v[ 2 ] = Vert[ Tri->v3 ].tv;
	zu = zv = 0;
	for ( j = 0; j < 3; j++ )
	{
		k = ( j + 1 ) % 3;
		du = u[ k ] - u[ j ];
		dv = v[ k ] - v[ j ];
		if ( fabs( du * Xsize ) < SAME_UV )
		{
			uz = j;
			zu++;
		}
		if ( fabs( dv * Ysize ) < SAME_UV )
		{
			vz = j;
			zv++;
		}
	}
	if ( zu != 1 || zv != 1 )
		return; // not a square texture
	TriVert[ 0 ] = &Vert[ Tri->v1 ];
	TriVert[ 1 ] = &Vert[ Tri->v2 ];
	TriVert[ 2 ] = &Vert[ Tri->v3 ];

	Orig_TriVert[ 0 ] = &Orig_Vert[ Tri->v1 ];
	Orig_TriVert[ 1 ] = &Orig_Vert[ Tri->v2 ];
	Orig_TriVert[ 2 ] = &Orig_Vert[ Tri->v3 ];

	du = ( UV_Fix / Xsize );
	dv = ( UV_Fix / Ysize );
	su = du * ( ( TriVert[ ( uz + 2 ) % 3 ]->tu > TriVert[ uz ]->tu ) ? 1 : -1 );
	sv = dv * ( ( TriVert[ ( vz + 2 ) % 3 ]->tv > TriVert[ vz ]->tv ) ? 1 : -1 );
	if ( fabs( TriVert[ uz ]->tu + su - Orig_TriVert[ uz ]->tu ) <= du )
		TriVert[ uz ]->tu += su;
	k = ( uz + 1 ) % 3;
	if ( fabs( TriVert[ k ]->tu + su - Orig_TriVert[ k ]->tu ) <= du )
		TriVert[ k ]->tu += su;
	if ( fabs( TriVert[ vz ]->tv + sv - Orig_TriVert[ vz ]->tv ) <= dv )
		TriVert[ vz ]->tv += sv;
	k = ( vz + 1 ) % 3;
	if ( fabs( TriVert[ k ]->tv + sv - Orig_TriVert[ k ]->tv ) <= dv )
		TriVert[ k ]->tv += sv;
}


void
FixUV_Anim( POLYANIM *PolyAnim, LPD3DLVERTEX Vert, LPD3DLVERTEX Orig_Vert )
{
	int vnum, frame, vi;
	TANIMUV *UV;

	UV = PolyAnim->UVs;
	for ( frame = 0; frame < PolyAnim->frames; frame++ )
	{
		for ( vnum = 0; vnum < PolyAnim->vertices; vnum++ )
		{
			vi = PolyAnim->vert[ vnum ];
			UV->u += Vert[ vi ].tu - Orig_Vert[ vi ].tu;
			UV->v += Vert[ vi ].tv - Orig_Vert[ vi ].tv;
			UV++;
		}
	}
}


/*===================================================================
	Procedure	:		Load .Mxv File
	Input		:		char	*	Filename , MLOADHEADER *
	Output		:		Nothing
===================================================================*/
BOOL Mload( char * Filename, MLOADHEADER * Mloadheader  )
{
	char		*	FileNamePnt;
	char		*	Buffer;
	char		*	tempBuffer;
	uint16		*	Uint16Pnt;
	uint16		*	Uint16Pnt2;
	int16		*	int16Pnt;
	float		*	FloatPnt;
	MFACE		*	MFacePnt;
	VERT		*	VertPnt;
	VERTEXCELL  *	VertexCellPnt;
	D3DTRIANGLE		FacePnt; // was a pointer
	LPD3DTRIANGLE	TempFacePnt;
	uint16			exec_type;			// the type of execute buffer
	uint16			texture_type;			// the type of texture...0 normal  1 env
	uint16			num_vertices;		// overall number of verts
	uint16			num_triangle_groups;// number of triangle groups
	uint16			num_triangles;		// number of triangles in group
	uint16			group_vertex_start; // where in the vert list to start processing
	uint16			group_vertex_num;	// and how many to do...
	uint16			verts;
	uint16			portal;
	uint16			ExecSize;
	LPOLDD3DLVERTEX lpD3DLVERTEX2 = NULL;
	LPD3DLVERTEX	lpD3DLVERTEX;
	LPD3DLVERTEX	lpBufStart = NULL;
	WORD			*lpIndices = NULL;
	int				indexOffset = 0;
	int				ibIndex = 0;
	int				numTriangles = 0;
	int				tempInt = 0;
	int				triangleCount = 0;
	STARTPOS	  *	StartPosPnt;
	int			i,e,o;
	uint16 tpage;
	int execbuf;
	int group;
	int r,g,b,a;
	D3DCOLOR color;
	float		*	up;
	uint16	animation,frames,vertices;
	POLYANIM * PolyAnim;
	TANIMUV * TanimUV;
	int * intPnt;
	uint16	num_animations;
	uint16	maxloops;
	uint16	animsize;
	uint16	NumColours;
	uint32	TempColour;
	uint32	*	Uint32Pnt;
	TEXTUREANIMINFOINDEX * TAnimInfoIndexPnt;
	uint16	polyanim;
	uint16	initstate;
	int16	whenstoppedtriggermod;
	D3DCOLOR *ambient;
	int colourkey = 0;
	char buf[100];


	OutputDebugString("Mload - ");
	OutputDebugString(Filename);
	OutputDebugString("\n");

	if (strcmp("data\\levels\\vol2\\vol2.mxv", Filename) == 0)
	{
		OutputDebugString("break here...\n");
	}

	// Mloadheader is not valid until everything has been done..
	Mloadheader->state = FALSE;

	BuildGammaCorrect( 1.0 );
	
	Buffer = Mloadheader->Buffer;
	if( Buffer == NULL)
	{
		Msg( "Mload : Error Allocating Buffer\n" );
		return FALSE;
	}

	/*	get the number of groups	*/
	Uint16Pnt = (uint16 *) Buffer;
	Mloadheader->num_groups = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		

	if ( Mloadheader->num_groups > MAXGROUPS )
	{
		Msg( "Mload : NumGroups > MAXGROUPS\n" );
		return FALSE;
	}

	for( group = 0; group < Mloadheader->num_groups; group++)
	{
		GroupTris[ group ] = 0;
		/*	get the number of execbufs in this group	*/
		Uint16Pnt = (uint16 *) Buffer;
		Mloadheader->Group[group].num_execbufs = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;		
	
		if ( Mloadheader->Group[group].num_execbufs > MAXEXECBUFSPERGROUP )
		{
			Msg( "Mload : NumExecBufs > MAXEXECBUFSPERGROUP\n" );
			return FALSE;
		}
		
		for( execbuf = 0; execbuf < Mloadheader->Group[group].num_execbufs; execbuf++)
		{
			Mloadheader->Group[group].num_animating_polys[execbuf] = 0;
			
			int16Pnt = (int16 *) Buffer;
			ExecSize = 	*int16Pnt++;
			ExecSize += 512;
			Buffer = (char *) int16Pnt;		
			
			Uint16Pnt = (uint16 *) Buffer;

			/*	get the type of execution buffer	*/
			exec_type = *Uint16Pnt++;
			/*	get the number of verts in execution buffer	*/
			num_vertices =	*Uint16Pnt++;     
			Buffer = (char *) Uint16Pnt;		
	
			/*	record how what type of exec buffer	*/
			Mloadheader->Group[group].exec_type[execbuf] = exec_type;

			/*	record how many verts there are in the exec buffer	*/
			Mloadheader->Group[group].num_verts_per_execbuf[execbuf] = num_vertices;

			lpD3DLVERTEX2 = (LPOLDD3DLVERTEX ) Buffer;

			if (FAILED(FSCreateVertexBuffer(&Mloadheader->Group[group].renderObject[execbuf], num_vertices)))
			{
				Msg( "Mload : MakeExecBuffer Failed\n" );
				return FALSE;
			}

			sprintf(buf, "created buffer to hold :%d verts\n", num_vertices);
			OutputDebugString(buf);

			if (FAILED(FSLockVertexBuffer(&Mloadheader->Group[group].renderObject[execbuf], &lpD3DLVERTEX)))
			{
				Msg( "Mload() lock failed in %s\n", Filename );
				return FALSE;
			}

			lpBufStart = lpD3DLVERTEX;

			/*	copy the vertex data into the execute buffer	*/
			for ( i=0 ; i<num_vertices; i++)
			{
				OLDD3DLVERTEX *old = lpD3DLVERTEX2;

				lpD3DLVERTEX[i].x = old->x;
				lpD3DLVERTEX[i].y = old->y;
				lpD3DLVERTEX[i].z = old->z;
				lpD3DLVERTEX[i].tu = old->tu;
				lpD3DLVERTEX[i].tv = old->tv;

				color = old->color;
				a = (color>>24)&255;
				r = (color>>16)&255;
				g = (color>>8)&255;
				b = color&255;

				if(d3dapp->CurrDriver == 0) // is it or ramp mode..
				{
					if( ( exec_type&HASTRANSPARENCIES )  && ( UsedStippledAlpha ) )	// if transparencies and alpha stipple
					{
						a = 128;
					}else{
						if ( SWMonoChrome )
						{
							r = (r+g+b) / 3;
							g = r;
							b = g;
						}
						a = 128;
					}
				}else{
					if( ( exec_type&HASTRANSPARENCIES )  && ( UsedStippledAlpha  ) )	// if transparencies and alpha stipple
					{
						r = Tab[r];
						g = Tab[g];
						b = Tab[b];
						a = 64;
					}else{
						r = Tab[r];
						g = Tab[g];
						b = Tab[b];
#if ACTUAL_TRANS
						a = 128;
#else
						a = 255;
#endif
					}
				}

				// right here you could override texture coloring if you wanted
				color = RGBA_MAKE( r , g , b , a  );
				lpD3DLVERTEX[i].color = color;
				lpD3DLVERTEX2->color = color;

// bjd - CHECK				lpD3DLVERTEX2->dwReserved |= 0xff000000;

				// right here you could set the specular value
				// they seemed to have turned it off anyway
				// testing shows that it appears to do nothing
				//lpD3DLVERTEX->specular = lpD3DLVERTEX2->specular;
				lpD3DLVERTEX[i].specular = RGB_MAKE( 0 , 0 , 0 );
//				lpD3DLVERTEX++;
				lpD3DLVERTEX2++;
			}

			/* bjd - allows us to retrieve copies of the original vertices in the new format! */
			Mloadheader->Group[group].originalVerts[execbuf] = malloc(sizeof(D3DLVERTEX) * num_vertices);
			memcpy(Mloadheader->Group[group].originalVerts[execbuf], &lpD3DLVERTEX[0], sizeof(D3DLVERTEX) * num_vertices);
	
			Buffer = (char *) lpD3DLVERTEX2;
			
			Uint16Pnt = (uint16 *) Buffer;
			num_triangle_groups = *Uint16Pnt++;    
			Buffer = (char *) Uint16Pnt;

			triangleCount = 0;
			tempInt = 0;
			tempBuffer = Buffer;

			/* find out how many vertexes we'll need to load into our vertex buffer */
			for ( i=0 ; i<num_triangle_groups; i++)
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
			if (FAILED(FSCreateIndexBuffer(&Mloadheader->Group[group].renderObject[execbuf], triangleCount * 3)))
			{
				return FALSE;
			}

			/*	lock the index buffer	*/
			if (FAILED(FSLockIndexBuffer(&Mloadheader->Group[group].renderObject[execbuf], &lpIndices)))
			{
				Msg( "Mload() lock failed in %s\n", Filename );
				return FALSE;
			}

			ibIndex = 0;
			indexOffset = 0;
			
			for ( i=0 ; i<num_triangle_groups; i++)
			{
				Uint16Pnt = (uint16 *) Buffer;
				texture_type = *Uint16Pnt++;
				group_vertex_start = *Uint16Pnt++;
				group_vertex_num = *Uint16Pnt++;
				tpage = *Uint16Pnt++;
				num_triangles = *Uint16Pnt++;    
				Buffer = (char *) Uint16Pnt;
				
				GroupTris[ group ] += num_triangles;
	
/* bjd - TODO?
				OP_STATE_LIGHT(1, lpPointer);
				    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[Mloadheader->TloadIndex[tpage]], lpPointer);
				OP_PROCESS_VERTICES(1, lpPointer);
					PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, group_vertex_start, group_vertex_num, lpPointer);
				OP_STATE_RENDER(1, lpPointer);
					STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[Mloadheader->TloadIndex[tpage]], lpPointer);

			    if (QWORD_ALIGNED(lpPointer))
					OP_NOP(lpPointer);
					
				OP_TRIANGLE_LIST( (short) num_triangles, lpPointer);
*/
				MFacePnt = (MFACE *) Buffer;
				TempFacePnt = (LPD3DTRIANGLE ) lpIndices;
	
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
/*
					FacePnt->v1 = MFacePnt->v1;
					FacePnt->v2 = MFacePnt->v2;
					FacePnt->v3 = MFacePnt->v3;
*/
					if ( MFacePnt->pad & 1 )
					{
						// colourkey triangle found
						colourkey++;
						MFacePnt->pad &= ~1;
					}
					if ( AllWires )
					{
//bjd - CHECK						FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					}
					else
					{
//						FacePnt->wFlags = MFacePnt->pad;
					}

					FixUV( &FacePnt, lpBufStart, tpage, /*Mloadheader->Group[group].org_vertpnt[execbuf]*/ Mloadheader->Group[group].originalVerts[execbuf] );
					//FacePnt++;
					//MFacePnt++;
					MFacePnt++;
					tempInt+=3;
				}
				//lpPointer = (LPVOID) FacePnt;
				Buffer = (char *) MFacePnt;
			}

			Mloadheader->Group[group].renderObject[execbuf].textureGroups[i].numVerts = num_vertices;
			Mloadheader->Group[group].renderObject[execbuf].textureGroups[i].numTriangles = num_triangles;

			/* keep track of our offset into our index buffer */
			Mloadheader->Group[ group ].renderObject[execbuf].textureGroups[i].startIndex = indexOffset;
			indexOffset += num_triangles * 3;

			Mloadheader->Group[group].renderObject[execbuf].textureGroups[i].texture = NULL;

			Mloadheader->Group[group].polyanim[execbuf] = NULL;
			Mloadheader->Group[group].num_animating_polys[execbuf] = 0;
			
			Mloadheader->Group[group].renderObject[execbuf].numTextureGroups++;


			// this go here?
			/*	unlock the vertex buffer	*/
			if (FAILED(FSUnlockVertexBuffer(&Mloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() vb unlock failed in %s\n", Filename );
				return FALSE ;
			}

			/*	unlock the index buffer	*/
			if (FAILED(FSUnlockIndexBuffer(&Mloadheader->Group[group].renderObject[execbuf])))
			{
				Msg( "Mxload() ib unlock failed in %s\n", Filename );
				return FALSE ;
			}
		}
	}
				
	
	Uint16Pnt = (uint16 *) Buffer;
	// is there any vispoly info.....
	if ( *Uint16Pnt != 0)
	{
		uint16 pnum, vnum;

		if ( *Uint16Pnt != 2 )
		{
			Msg( "Mload : Not in new recursive format\n" );
			return FALSE; // error if not in new recursive group/portal visibility tree format
		}

		Uint16Pnt++;
/*===================================================================
 *		Particular Stuff Related To Visiploys	
===================================================================*/
		Buffer = ( char * ) Uint16Pnt;

		for( group=0 ; group<Mloadheader->num_groups; group++)
		{
			/*	copy over the groups Name	*/
			FileNamePnt = (char*) &Mloadheader->Group[group].name;
			while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );
	

			// Make sure there are no start points initially in this group..
			Mloadheader->Group[group].StartPosInThisGroup = (uint16) -1;

			VertPnt = ( VERT* )	Buffer;
			/*	get the center position of the group	*/
			Mloadheader->Group[group].center = *VertPnt++;
			/*	get the size of the group	*/
			Mloadheader->Group[group].half_size = *VertPnt++;
			Buffer = ( char * ) VertPnt;

			Uint16Pnt = (uint16 *) Buffer;
			/*	get the number of portals in the group	*/
			Mloadheader->Group[group].num_portals = *Uint16Pnt++;    
			Buffer = (char *) Uint16Pnt;		
			if(Mloadheader->Group[group].num_portals > MAXPORTALSPERGROUP)
			{
				Msg( "Mload : NumPortals > MAXPORTALSPERGROUP\n" );
				return FALSE;    
			}
			
			Mloadheader->Group[group].Portal = (PORTAL*) malloc( Mloadheader->Group[group].num_portals * sizeof(PORTAL) );
			
			for( portal = 0 ; portal < Mloadheader->Group[group].num_portals ; portal ++ )
			{
				Uint16Pnt = (uint16 *) Buffer;
				/*	get the number of Verts in the Portal	*/
				Mloadheader->Group[group].Portal[portal].num_vertices_in_portal = *Uint16Pnt++;
				Buffer = (char *) Uint16Pnt;		

				if(Mloadheader->Group[group].Portal[portal].num_vertices_in_portal > MAXVERTSPERPORTAL)
				{
					Msg( "Mload : More than MAXVERTSPERPORTAL\n" );
					return FALSE;  
				}
	
				VertPnt = ( VERT* )	Buffer;
				for( verts = 0 ; verts < Mloadheader->Group[group].Portal[portal].num_vertices_in_portal ; verts ++ )
				{
					/*	get the Verts in the Portal	*/
					Mloadheader->Group[group].Portal[portal].Verts[verts] = *VertPnt++;
				}
				Buffer = (char *) VertPnt;		
					
				Uint16Pnt = (uint16 *) Buffer;
				// get the polys in the portal
				Mloadheader->Group[group].Portal[portal].num_polys_in_portal = *Uint16Pnt++;
				if ( Mloadheader->Group[group].Portal[portal].num_polys_in_portal > MAXPOLYSPERPORTAL )
				{
					Msg( "Mload : More than MAXPOLYSPERPORTAL\n" );
					return FALSE;
				}

				for ( pnum = 0; pnum < Mloadheader->Group[group].Portal[portal].num_polys_in_portal; pnum++ )
				{
#if 1
					Mloadheader->Group[group].Portal[portal].Poly[pnum] = * (MCFACE *) Uint16Pnt;
					Uint16Pnt = (uint16 *) ( ( (MCFACE *) Uint16Pnt) + 1 );
#else
					Mloadheader->Group[group].Portal[portal].Poly[pnum].num_vertices_in_poly = *Uint16Pnt++;
					if ( Mloadheader->Group[group].Portal[portal].Poly[pnum].num_vertices_in_poly > MAXVERTSPERPORTALPOLY )
					{
						Msg( "Mload : More than MAXVERTSPERPORTALPOLY\n" );
						return FALSE;
					}
					for ( verts = 0; verts < Mloadheader->Group[group].Portal[portal].Poly[pnum].num_vertices_in_poly; verts++ )
					{
						vnum = *Uint16Pnt++;
						if ( vnum >= Mloadheader->Group[group].Portal[portal].num_vertices_in_portal )
						{
							Msg( "Mload : Different num of verts in portal\n" );
							return FALSE;
						}
						Mloadheader->Group[group].Portal[portal].Poly[pnum].Verts[verts] = Mloadheader->Group[group].Portal[portal].Verts[vnum];
					}
#endif
				}
	
				Buffer = (char *) Uint16Pnt;		
			}
		
		}		
		// get the recursive group/portal visibility tree
		for( group=0 ; group<Mloadheader->num_groups; group++)
		{
			for( portal = 0 ; portal < Mloadheader->Group[group].num_portals ; portal ++ )
			{
				Mloadheader->Group[group].Portal[portal].visible.portal = &Mloadheader->Group[group].Portal[portal];
				Mloadheader->Group[group].Portal[portal].visible.group = *Uint16Pnt++;
				if ( Mloadheader->Group[group].Portal[portal].visible.group >= Mloadheader->num_groups )
				{
					Msg( "Mload : Group > Num groups\n" );
					return FALSE;
				}
				Mloadheader->Group[group].Portal[portal].visible.num_visible = *Uint16Pnt++;
				if ( Mloadheader->Group[group].Portal[portal].visible.num_visible > 0 )
				{
					Mloadheader->Group[group].Portal[portal].visible.visible =
						(VISTREE *) calloc( Mloadheader->Group[group].Portal[portal].visible.num_visible, sizeof( VISTREE ) );
					if ( !Mloadheader->Group[group].Portal[portal].visible.visible )
					{
						Msg( "Mload : no visible portal\n" );
						return FALSE;
					}

					for ( vnum = 0; vnum < Mloadheader->Group[group].Portal[portal].visible.num_visible; vnum++ )
					{
						if ( !read_visible( Mloadheader, &Mloadheader->Group[group].Portal[portal].visible.visible[vnum], 
								Mloadheader->Group[group].Portal[portal].visible.group, &Uint16Pnt ) )
						{
							Msg( "Mload : Read visible failed\n" );
							return FALSE;
						}
					}
				}
				else
					Mloadheader->Group[group].Portal[portal].visible.visible = NULL;
			}
		}
		Buffer = (char *) Uint16Pnt;		
	
/*===================================================================
 *		End Of Visiploy Stuff	
===================================================================*/

#if MXV_VERSION_NUMBER >= 3
		if ( !ReadGroupConnections( Mloadheader, &Buffer ) )
			return FALSE;
		ReadSoundInfo( Mloadheader, &Buffer );
#endif

/*===================================================================
 *		Start of Cell Index Stuff	
===================================================================*/
		Uint16Pnt = (uint16 *) Buffer;
		// is there any cell index info.....
		if ( *Uint16Pnt++ != 0)
		{
			Buffer = (char *) Uint16Pnt;		

			FloatPnt = ( float * ) Buffer;
			Mloadheader->CellSize = 1.0F / *FloatPnt++;
			Buffer = (char * ) FloatPnt;
	
			for( group=0 ; group<Mloadheader->num_groups; group++)
			{
				Uint16Pnt = (uint16 *) Buffer;
				Uint16Pnt++; //dont need the number of exec buffs......
				Buffer = (char *) Uint16Pnt;		
				for( execbuf=0 ; execbuf<Mloadheader->Group[group].num_execbufs; execbuf++)
				{
					FloatPnt = (float * ) Buffer;
					Mloadheader->Group[group].cell_origin[execbuf].x = *FloatPnt++;
					Mloadheader->Group[group].cell_origin[execbuf].y = *FloatPnt++;
					Mloadheader->Group[group].cell_origin[execbuf].z = *FloatPnt++;
					Buffer = (char * ) FloatPnt;
					Uint16Pnt = (uint16 *) Buffer;
					Mloadheader->Group[group].xcells[execbuf] = *Uint16Pnt++;
					Mloadheader->Group[group].ycells[execbuf] = *Uint16Pnt++;
					Mloadheader->Group[group].zcells[execbuf] = *Uint16Pnt++;
					Mloadheader->Group[group].numofcells[execbuf] =	Mloadheader->Group[group].xcells[execbuf] *
																	Mloadheader->Group[group].ycells[execbuf] *
																	Mloadheader->Group[group].zcells[execbuf];


#if MXV_VERSION_NUMBER >= 2
					if ( !execbuf )
						Mloadheader->Group[group].colour_cell_pnt[0] = (D3DCOLOR*) malloc( Mloadheader->Group[group].numofcells[execbuf] * sizeof(D3DCOLOR) );
					else
						Mloadheader->Group[group].colour_cell_pnt[execbuf] = NULL;
					if( !Mloadheader->Group[group].colour_cell_pnt[0] )
					{
						Msg( "Mload : Couldnt allocate enough memory for the cell colour info\n" );
						return FALSE;
					}
#else
					Mloadheader->Group[group].colour_cell_pnt[execbuf] = (D3DCOLOR*) malloc( Mloadheader->Group[group].numofcells[execbuf] * sizeof(D3DCOLOR) );
					if( !Mloadheader->Group[group].colour_cell_pnt[execbuf] )
					{
						Msg( "Mload : Couldnt allocate enough memory for the cell colour info\n" );
						return FALSE;
					}
#endif
					Mloadheader->Group[group].num_vertex_indices[execbuf] = *Uint16Pnt++;

					Mloadheader->Group[group].vertex_index_pnt[execbuf] = Uint16Pnt;

					Uint16Pnt += Mloadheader->Group[group].num_vertex_indices[execbuf];

					VertexCellPnt  = (VERTEXCELL * ) Uint16Pnt;

					Mloadheader->Group[group].vertex_cell_pnt[execbuf] = VertexCellPnt;

					VertexCellPnt += Mloadheader->Group[group].numofcells[execbuf];

					Buffer = (char *) VertexCellPnt;
					
				}
#if MXV_VERSION_NUMBER >= 2
				ambient = (D3DCOLOR *) Buffer;
				for ( i = 0; i < Mloadheader->Group[group].numofcells[0]; i++ )
				{
					Mloadheader->Group[group].colour_cell_pnt[0][i] = *ambient++;
				}
				Buffer = (char *) ambient;
#endif
			}
		
		}
/*===================================================================
 *		Start of Start Pos Stuff
===================================================================*/
		Uint16Pnt = (uint16 *) Buffer;
		num_start_positions = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;		

		for( i = 0 ; i < MAXGROUPS ; i++ )
		{
			FirstStartPositionInGroup[i] = (uint16) -1;
		}
		StartPosPnt = (STARTPOS * ) Buffer;
		for( i = 0 ; i < num_start_positions ; i++ )
		{
			StartPositions[i].Pos.x = StartPosPnt->Pos.x;
			StartPositions[i].Pos.y = StartPosPnt->Pos.y;
			StartPositions[i].Pos.z = StartPosPnt->Pos.z;
			StartPositions[i].Dir.x = StartPosPnt->Dir.x;
			StartPositions[i].Dir.y = StartPosPnt->Dir.y;
			StartPositions[i].Dir.z = StartPosPnt->Dir.z;
			StartPositions[i].Up.x = StartPosPnt->Up.x;
			StartPositions[i].Up.y = StartPosPnt->Up.y;
			StartPositions[i].Up.z = StartPosPnt->Up.z;
			StartPositions[i].Group = StartPosPnt->Group;
			//Make a note in each group of which Start point is in there...
			Mloadheader->Group[StartPositions[i].Group].StartPosInThisGroup = i;
			if( FirstStartPositionInGroup[StartPositions[i].Group] == (uint16) -1 )
			{
				StartPositions[i].NextInGroup = (uint16) -1;
				FirstStartPositionInGroup[StartPositions[i].Group] = i;
			}else{
				StartPositions[i].NextInGroup = FirstStartPositionInGroup[StartPositions[i].Group];
				FirstStartPositionInGroup[StartPositions[i].Group] = i;
			}
			StartPosPnt++;
		}
		Buffer = (char *) StartPosPnt;		

/*===================================================================
 *		Start of group up vector Stuff
===================================================================*/
		Uint16Pnt = (uint16 *) Buffer;
		// is there any up vector info.....
		if ( *Uint16Pnt++ != 0)
		{
			Buffer = (char *) Uint16Pnt;
			up = (float *) Buffer;
			for ( i = 0; i < Mloadheader->num_groups; i++ )
			{
				Mloadheader->Group[ i ].up.x = *up++;
				Mloadheader->Group[ i ].up.y = *up++;
				Mloadheader->Group[ i ].up.z = *up++;
			}
			Buffer = (char *) up;
		}
		else
		{
			// no up vector info -> use default (0,1,0)
			for ( i = 0; i < Mloadheader->num_groups; i++ )
			{
				Mloadheader->Group[ i ].up.x = 0.0F;
				Mloadheader->Group[ i ].up.y = 1.0F;
				Mloadheader->Group[ i ].up.z = 0.0F;
			}
		}

	}

	// Background Animation Stuff......
	Uint16Pnt = (uint16 *) Buffer;
	num_animations = *Uint16Pnt++;
	Mloadheader->AnimData.num_animations = num_animations;
	Buffer = (char *) Uint16Pnt;
	
	if( num_animations )
	{
		for( i = 0 ; i < num_animations ; i++ )
		{
			frames = *Uint16Pnt++;
			maxloops = *Uint16Pnt++;
			animsize = *Uint16Pnt++;
			Mloadheader->AnimData.AnimSeq[i] = (uint16 *) malloc( animsize * sizeof( uint16 ) );
			Uint16Pnt2 = Mloadheader->AnimData.AnimSeq[i];
			for( e = 0 ; e < animsize ; e++ )
			{
				*Uint16Pnt2++ = *Uint16Pnt++;
			}
		}
		Buffer = ( char * ) Uint16Pnt;
		
		for( group=0 ; group<Mloadheader->num_groups; group++)
		{
			for( execbuf=0 ; execbuf<Mloadheader->Group[group].num_execbufs; execbuf++)
			{
				// Animating background polys......		
				Uint16Pnt = (uint16 *) Buffer;
				Mloadheader->Group[group].num_animating_polys[execbuf] = *Uint16Pnt++;
				Buffer = ( char * ) Uint16Pnt;
				if( Mloadheader->Group[group].num_animating_polys[execbuf] != 0 )
				{
					Mloadheader->Group[group].polyanim[execbuf] = malloc( Mloadheader->Group[group].num_animating_polys[execbuf] * sizeof( POLYANIM ) );
		
		
					PolyAnim = Mloadheader->Group[group].polyanim[execbuf];
					if( !PolyAnim )
					{
						Msg( "Mload : No PolyAnim\n" );
						return FALSE;
					}
/*
					memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
					debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/
//					if( Mloadheader->Group[ group ].lpExBuf[ execbuf ]->lpVtbl->Lock(
//						Mloadheader->Group[ group ].lpExBuf[ execbuf ], &debDesc ) != D3D_OK ) // bjd
/*
					if (FSLockExecuteBuffer(Mloadheader->Group[ group ].lpExBuf[ execbuf ], &debDesc ) != D3D_OK )
					{
						Msg( "Mload : Lock ExecBuffer failed\n" );
						return FALSE;
					}
*/
					if (FAILED(FSLockVertexBuffer(&Mloadheader->Group[ group ].renderObject[execbuf], &lpD3DLVERTEX)))
					{
						Msg( "Mload : Lock VertexBuffer failed\n" );
						return FALSE;
					}
//					lpD3DLVERTEX = (LPD3DLVERTEX ) debDesc.lpData;

					for( i = 0 ; i < Mloadheader->Group[group].num_animating_polys[execbuf] ; i++ )
					{
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
							Msg( "Mload : PolyAnim vert is null\n" );
							return FALSE;
						}
						PolyAnim->UVs  = (TANIMUV *) malloc( vertices * frames * sizeof(TANIMUV) );
						if( !PolyAnim->UVs)
						{
							Msg( "Mload : PolyAnim uv is null\n" );
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
						FixUV_Anim( PolyAnim, lpD3DLVERTEX, /*Mloadheader->Group[group].org_vertpnt[execbuf]*/ Mloadheader->Group[group].originalVerts[execbuf] );
						PolyAnim++;
					}

					if (FAILED(FSUnlockVertexBuffer(&Mloadheader->Group[group].renderObject[execbuf])))
					{
						Msg( "Mload : Unlock VertexBuffer failed\n" );
						return FALSE;
					}
				}
			}
		}
	}

	Uint16Pnt = (uint16 *) Buffer;
	NumColours = *Uint16Pnt++;

	if( NumColours )
	{
		Buffer = (char *) Uint16Pnt;
		Uint32Pnt = (uint32 *) Buffer;

		for( group=0; group < Mloadheader->num_groups; group++ )
		{
			TempColour = *Uint32Pnt++;
			Mloadheader->Group[group].BGClear_Flag	= (int16) ( ( TempColour >> 24 ) & 255 );
			Mloadheader->Group[group].BGClear_Red	= ( ( ( (float) ( ( TempColour >> 16 ) & 255 ) ) ) / 255.0F );
			Mloadheader->Group[group].BGClear_Green = ( ( ( (float) ( ( TempColour >> 8 ) & 255 ) ) ) / 255.0F );
			Mloadheader->Group[group].BGClear_Blue	= ( ( ( (float) ( ( TempColour >> 0 ) & 255 ) ) ) / 255.0F );
		}

		Buffer = ( char * ) Uint32Pnt;
	}
	else
	{
		for( group=0; group < Mloadheader->num_groups; group++ )
		{
			Mloadheader->Group[group].BGClear_Flag = 0;
			Mloadheader->Group[group].BGClear_Red	= 0.0F;
			Mloadheader->Group[group].BGClear_Green = 0.0F;
			Mloadheader->Group[group].BGClear_Blue	= 0.0F;
		}
		Buffer = ( char * ) Uint16Pnt;
	}

	
	// Background Poly Info Index Animation Stuff......
	Mloadheader->AnimData.TAnimInfoIndex = NULL;
	Mloadheader->AnimData.num_animating_polys = 0;
	Uint16Pnt = (uint16 *) Buffer;
	if( *Uint16Pnt++)
	{
		Mloadheader->AnimData.num_animating_polys = *Uint16Pnt++;
		Mloadheader->AnimData.TAnimInfoIndex = ( TEXTUREANIMINFOINDEX * ) malloc( Mloadheader->AnimData.num_animating_polys * sizeof(TEXTUREANIMINFOINDEX) );
	   
		TAnimInfoIndexPnt = Mloadheader->AnimData.TAnimInfoIndex;
		if( !TAnimInfoIndexPnt )
		{
			Msg( "Mload : Texture Animation Index failed mallco\n" );
			return FALSE;
		}
		for( i = 0 ; i < Mloadheader->AnimData.num_animating_polys ; i++ )
		{
			group = *Uint16Pnt++;
			execbuf = *Uint16Pnt++;
			polyanim = *Uint16Pnt++;
			initstate = *Uint16Pnt++;
			whenstoppedtriggermod = (int16) *Uint16Pnt++;

			PolyAnim = Mloadheader->Group[group].polyanim[execbuf];
			PolyAnim += polyanim;
			PolyAnim->State = initstate;
			PolyAnim->WhenStoppedTriggerMod = whenstoppedtriggermod;


			TAnimInfoIndexPnt->Group = group;
			TAnimInfoIndexPnt->Execbuf = execbuf;
			TAnimInfoIndexPnt->PolyAnim = polyanim;
			TAnimInfoIndexPnt->InitState = initstate;
			TAnimInfoIndexPnt++;
		}
	}
	Buffer = (char *) Uint16Pnt;

	DebugPrintf( "Mload: %d colourkey triangles found\n", colourkey );
	// Mloadheader is valid and can be executed...
	Mloadheader->state = TRUE;

#if MXV_VERSION_NUMBER < 3
	if ( !FindGroupConnections( Mloadheader ) )
		return FALSE;
#endif

	return( TRUE );
}

/*===================================================================
	Procedure	:		Execute all group buffers for a Mloadheader
	Input		;		MLOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/

BOOL ExecuteMloadHeader( MLOADHEADER * Mloadheader  )
{
	int i;
	int group;
	D3DMATRIX Matrix;
	if (Mloadheader->state == TRUE )
	{
		for ( group=0 ; group<Mloadheader->num_groups ; group++)
		{
			for ( i=0 ; i<Mloadheader->Group[group].num_execbufs; i++)
			{
				// if its a Transparent Execute Buffer then dont display it add it to the Transexe list
//				if( ((Mloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES) != 0) && ( UsedStippledAlpha == FALSE)  )
				if( Mloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
				{
	//				if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
					if (FAILED(FSGetMatrix(D3DTS_WORLD, &Matrix)))
					{
						return FALSE;
					}
					AddTransExe( &Matrix , /*Mloadheader->Group[group].lpExBuf[i]*/&Mloadheader->Group[group].renderObject[i] , 0, (uint16) -1, (uint16) group, Mloadheader->Group[ group ].num_verts_per_execbuf[i] );
				}
				else
				{
/*
					if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, Mloadheader->Group[group].lpExBuf[i], d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
						return FALSE;
*/
					if (FAILED(FSDrawVertexBuffer(&Mloadheader->Group[group].renderObject[i])))
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
	Procedure	:		Execute one group for an Mloadheader
	Input		;		MLOADHEADER *
	Output		:		FLASE/TRUE
===================================================================*/

// bjd - this function seems to draw static geometry?

BOOL ExecuteSingleGroupMloadHeader( MLOADHEADER * Mloadheader, uint16 group  )
{
	uint16 i;
	D3DMATRIX Matrix;
	
	if (Mloadheader->state == TRUE )
	{
		for ( i=0 ; i<Mloadheader->Group[group].num_execbufs; i++)
		{
			// if its a Transparent Execute Buffer then dont diplay it add it to the Transexe list
//			if( ((Mloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES) != 0) && ( UsedStippledAlpha == FALSE)  )
			if( Mloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
			{
				//if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
				if (FAILED(FSGetMatrix(D3DTS_WORLD, &Matrix)))
				{
					return FALSE;
				}
				AddTransExe( &Matrix , &Mloadheader->Group[group].renderObject[i], 0, (uint16) -1, group, Mloadheader->Group[ group ].num_verts_per_execbuf[i] );

			}
			else
			{
				if (FAILED(FSDrawVertexBuffer(&Mloadheader->Group[group].renderObject[i])))
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}


void ReleaseVisible( VISTREE *v )
{
	int i;

	if ( v->visible )
	{
		for ( i = 0; i < v->num_visible; i++ )
		{
			ReleaseVisible( &v->visible[ i ] );
		}
		free( v->visible );
		v->visible = NULL;
	}
}


/*
 * ReleaseMloadheader
 * Release Execute buffers
 */
void
ReleaseMloadheader( MLOADHEADER * Mloadheader )
{
    int i,e;
    int group;
	POLYANIM * PolyAnim;

	if ( !Mloadheader->state )
		return;

	FreeGroupConnections();

    for (group = 0; group < Mloadheader->num_groups; group++)
	{

		for ( i = 0; i < Mloadheader->Group[ group ].num_portals; i++ )
		{
			ReleaseVisible( &Mloadheader->Group[ group ].Portal[ i ].visible );
		}

		free( Mloadheader->Group[group].Portal );

		for (i = 0; i < Mloadheader->Group[group].num_execbufs; i++)
		{
//			XRELEASE(Mloadheader->Group[group].lpExBuf[i]);
			FSReleaseRenderObject(&(Mloadheader->Group[group].renderObject[i]));

			if ( Mloadheader->Group[group].colour_cell_pnt[i] )
				free( Mloadheader->Group[group].colour_cell_pnt[i] );
			Mloadheader->Group[group].colour_cell_pnt[i] = NULL;

			PolyAnim = Mloadheader->Group[group].polyanim[i];

			
			for( e = 0 ; e < Mloadheader->Group[group].num_animating_polys[i] ; e++ )
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

			if (Mloadheader->Group[group].polyanim[i])
		   		free(Mloadheader->Group[group].polyanim[i]);
			
			Mloadheader->Group[group].polyanim[i] = NULL;
		}
	}
	for( i = 0 ; i < Mloadheader->AnimData.num_animations ; i++ )
	{
		free(Mloadheader->AnimData.AnimSeq[i]);
		Mloadheader->AnimData.AnimSeq[i] = NULL;
	}

	if( Mloadheader->AnimData.TAnimInfoIndex )
	{
		free(Mloadheader->AnimData.TAnimInfoIndex);
		Mloadheader->AnimData.TAnimInfoIndex = NULL;
	}
	
	if ( Mloadheader->OrgAddr )
	{
		free( Mloadheader->OrgAddr );
		Mloadheader->OrgAddr = NULL;
	}

	Mloadheader->state = FALSE;
}


#ifdef UNSCRAMBLE

#define CODE_LENGTH		(977)
#define CODEKEY			"+>*\\?<~:$^`£=&|,¬"

static void Unscramble( char *buf, long size, char *fname )
{
	char *fname_end;
	static char codeword[ CODE_LENGTH ], *codekey = CODEKEY;
	int codenum;
	char *ck, *cn;

	fname_end = strrchr( fname, '\\' );
	if ( fname_end && *++fname_end )
		fname = fname_end;
	fname_end = strrchr( fname, '.' );
	if ( !fname_end )
		fname_end = fname + strlen( fname );

	// set up code table
	memset( codeword, 0, sizeof( codeword ) );
	for ( ck = codekey, cn = fname, codenum = 0; codenum < sizeof( codeword ); codenum++ )
	{
		codeword[ codenum ] = *ck++ ^ *cn++ ^ ( ( codenum + 113 ) % 29 );
		if ( !codeword[ codenum ] )
			codeword[ codenum ] = ( codenum + 61 ) % 253;
		if ( !*ck )
			ck = codekey;
		if ( cn == fname_end )
			cn = fname;
	}
	codeword[ sizeof( codeword ) - 1 ] = 0;

	// decode data
	for ( codenum = 0; size--; )
	{
		*buf++ ^= codeword[ codenum++ ];
		if ( !codeword[ codenum ] )
			codenum = 0;
	}
}

#endif // UNSCRAMBLE


/*===================================================================
	Procedure	:		Pre - Load .Mx File
	Input		:		char	*	Filename , MLOADHEADER *
	Output		:		Nothing
===================================================================*/
BOOL PreMload( char * Filename, MLOADHEADER * Mloadheader  )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	uint16		*	Uint16Pnt;
	uint32		*	Uint32Pnt;
	int				i;
	char		*	FileNamePnt;
	uint32			MagicNumber;
	uint32			VersionNumber;
	int8			TempFilename[ 256 ];

	// Mloadheader is not valid until everything has been done..
	Mloadheader->state = FALSE;
	Mloadheader->Buffer = NULL;

	File_Size = Get_File_Size( Filename );
	
	if ( !File_Size )
	{
	 	Msg( "PreMLoad : File_Size is zero\n" );
		return FALSE;
	}

	Buffer = calloc( 1, File_Size + sizeof( int ) );

	if( Buffer == NULL )
	{
	 	Msg( "PreMLoad : Unable to allocate buffer\n" );
		return( FALSE );
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
	 	Msg( "PreMLoad : Didn't read all of file\n" );
		return( FALSE );
	}

	Mloadheader->OrgAddr = Buffer;

#ifdef UNSCRAMBLE
	if ( _stricmp( Filename, "data\\levels\\accworld\\accworld.mxv" ) &&
		 _stricmp( Filename, "data\\levels\\probeworld\\probeworld.mxv" ) )
		Unscramble( Buffer, Read_Size, Filename );
#endif

	Uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MXV_VERSION_NUMBER  ) )
	{
		Msg( "PreMload() Incompatible level( .MXV ) file %s", Filename );
		return( FALSE );
	}

	Uint16Pnt = (uint16 *) Buffer;

	Mloadheader->num_texture_files = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		

	if ( Mloadheader->num_texture_files > MAXTPAGESPERMLOAD )
	{
	 	Msg( "PreMLoad : Too many texture pages\n" );
		return FALSE;
	}
	
	if ( Mloadheader->num_texture_files !=0)
	{
		/*	put the filenames in the tex struct	*/
		for( i = 0;	 i < Mloadheader->num_texture_files; i++)
		{
			FileNamePnt = (char *) &Mloadheader->ImageFile[i];
			while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );

			sprintf( &TempFilename[ 0 ], "data\\levels\\%s\\textures\\%s", &ShortLevelNames[ LevelNum ][ 0 ], (char * ) &Mloadheader->ImageFile[i][0] );

			if( !_stricmp( "default.ppm", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\default.ppm" );
			if( !_stricmp( "pkups.ppm", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "pkupsa.ppm", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "default.bmp", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\default.ppm" );
			if( !_stricmp( "pkups.bmp", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "pkupsa.bmp", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );
			if( !_stricmp( "titana.bmp", &Mloadheader->ImageFile[i][0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\titana.ppm" );

			Mloadheader->TloadIndex[i] = AddTexture( &Tloadheader , &TempFilename[ 0 ], TRUE , FALSE , TRUE, 0, 0 );		// do colourkey	dont scale do MipMap

			if( Mloadheader->TloadIndex[i] == -1 )
			{
			 	Msg( "PreMLoad : Too many TPages\n" );
				return( FALSE );
			}
		}
	
	}

	Mloadheader->Buffer = Buffer;
	return( TRUE );
}

/*===================================================================
	Procedure	:		Handle All Animation For an Mloadheader
	Input		:		MLOADHEADER *
	Output		:		Nothing
===================================================================*/
void BackGroundTextureAnimation( MLOADHEADER * Mloadheader , uint16 group )
{
	uint16		*	Uint16Pnt;
	int	i;
	int	execbuf;
	POLYANIM * PolyAnim;
	uint16 * AnimData;
	float	ExtraTime;
	
	if( AnimOncePerFrame == Mloadheader->Group[group].AnimOncePerFrame )
		return;
	Mloadheader->Group[group].AnimOncePerFrame = AnimOncePerFrame;
	
	for( execbuf=0 ; execbuf<Mloadheader->Group[group].num_execbufs; execbuf++)
	{
		PolyAnim = Mloadheader->Group[group].polyanim[execbuf];
		
		for( i = 0 ; i < Mloadheader->Group[group].num_animating_polys[execbuf] ; i++ )
		{
			
			if( ( PolyAnim->State == TEXTUREANIM_Go ) &&
				( PolyAnim->CurrentOffset != (uint16) -1 ) &&
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
					AnimData = ( Mloadheader->AnimData.AnimSeq[PolyAnim->animation] + PolyAnim->CurrentOffset );
					Uint16Pnt = HandleAnimCommands( PolyAnim , AnimData , Mloadheader->AnimData.AnimSeq[PolyAnim->animation] );
					if ( Uint16Pnt )
						PolyAnim->CurrentOffset = Uint16Pnt - Mloadheader->AnimData.AnimSeq[PolyAnim->animation];
					else
					{
						// animation has reached the end of its sequence
						PolyAnim->CurrentOffset = (uint16) -1;
						if ( PolyAnim->WhenStoppedTriggerMod != -1 )
							ApplyTriggerMod( &TrigMods[ PolyAnim->WhenStoppedTriggerMod ] );
					}
				}
				
			}
			PolyAnim++;
		}
	}
}
/*===================================================================
	Procedure	:		Handle All Animation Commands
	Input		:		MLOADHEADER *
	Output		:		Nothing
===================================================================*/
uint16 * HandleAnimCommands( POLYANIM * PolyAnim , uint16 * AnimData , uint16 * OrgAnimAdr)
{
	uint16 Frame;
	uint16 LoopDepth;
	while( ( Frame = *AnimData++ ) >= TEXANIM_COMMAND )
	{
		switch( Frame )
		{
		case TEXANIM_COMMAND:
			break;
		case TEXANIM_DURATION:
			PolyAnim->MasterTime = (float) (*AnimData++) * 0.06F;
			PolyAnim->CurrentTime = PolyAnim->MasterTime;
			break;
		case TEXANIM_DURATION1:
			PolyAnim->CurrentTime = (float) (*AnimData++) * 0.06F;
			break;
		case TEXANIM_LOOPINIT:
			LoopDepth = *AnimData++;
			PolyAnim->loopstack[LoopDepth] = *AnimData++;
			break;
		case TEXANIM_LOOPEND:
			LoopDepth = *AnimData++;
			if( (PolyAnim->loopstack[LoopDepth] == 0xffff ) || ( (PolyAnim->loopstack[LoopDepth] -= 1 ) != 0 ) )
			{
				AnimData = OrgAnimAdr + *AnimData;
			}else{
 				AnimData++;
			}
			break;
		case TEXANIM_END:
			PolyAnim->State = TEXTUREANIM_Stop;
			return NULL;
			break;
		}
	}
	PolyAnim->newframe = Frame;
 	return AnimData;
}





/*===================================================================
	Procedure	:		Is GroupVisible recursive...
	Input		:		VISTREE * vistree , group to..
	Output		:		Nothing
===================================================================*/
float IsGroupVisibleSoundInfoRecursive( MLOADHEADER * Mloadheader ,VISTREE * visible , uint16 group2 , uint16 old_group , float Distance )
{
	int i;
	float	Distance2;
	float Temp;
	Temp = DistanceVector2Vector( (VECTOR*) &Mloadheader->Group[old_group].center , (VECTOR*) &Mloadheader->Group[visible->group].center );

	Distance += ( Temp / 2.0F );

	TempGroups[visible->group] = TRUE;

	if( visible->group == group2)
		return Distance;

	for( i = 0 ; i < visible->num_visible ; i++ )
	{
		if( (Distance2 = IsGroupVisibleSoundInfoRecursive( Mloadheader , &visible->visible[i] , group2 , visible->group , Distance ) ) != -1.0F )
			return Distance2;
	}
	return -1.0F;
}

/*===================================================================
	Procedure	:		Is GroupVisible...
	Input		:		MLOADHEADER * , group in , group to..
	Output		:		Nothing
===================================================================*/
float IsGroupVisibleSoundInfo( MLOADHEADER * Mloadheader , uint16 group , uint16 group2 )
{
	int i;
	PORTAL  * Portal;
	VISTREE * visible;
	float Distance;
	float Min;

	Portal = Mloadheader->Group[group].Portal;


	Min = 100000.0F;
	for( i = 0 ; i < Mloadheader->Group[group].num_portals ; i++ )
	{
		visible = &Portal->visible;

		while( visible )
		{
			if( (Distance = IsGroupVisibleSoundInfoRecursive( Mloadheader , visible , group2 , group , 0.0F)) != -1.0F )
			{
			TempGroups[visible->group] = TRUE;
				if( Distance < Min )
				{
					Min = Distance;
				}
			}
			visible = visible->visible;
		}
			
		Portal++;
	}
	if( Min != 100000.0F )
	{
		return Min;
	}
	return -1.0F;
}
/*===================================================================
	Procedure	:		Is GroupVisible...
	Input		:		MLOADHEADER * , group in , group to..
	Output		:		Nothing
===================================================================*/
float IsGroupVisibleSoundInfo2( MLOADHEADER * Mloadheader , uint16 group , uint16 group2 )
{
	uint16 i;
	float Distance;
	float Min;
	BOOL TempGroups2[MAXGROUPS];
	for( i = 0 ; i < Mloadheader->num_groups ; i++ )
	{
		TempGroups2[i] = TempGroups[i];
	}

	Min = 100000.0F;
	for( i = 0 ; i < Mloadheader->num_groups ; i++ )
	{
		if( TempGroups2[i] )
		{
			if( ( Distance = IsGroupVisibleSoundInfo( Mloadheader , i , group2 ) )!= -1.0F )
			{
				if( Distance < Min )
				{
					Min = Distance;
				}
			}

		}
	}
		
	if( Min != 100000.0F )
	{
		return Min;
	}
	return -1.0F;
}

/*===================================================================
	Procedure	:		Is GroupVisible...
	Input		:		MLOADHEADER * , group in , group to..
	Output		:		Nothing
===================================================================*/
float IsGroupVisibleSoundInfo3( MLOADHEADER * Mloadheader , uint16 group , uint16 group2 )
{
	uint16 i;
	float Distance;
	float Min;

	Min = 100000.0F;

	for( i = 0 ; i < Mloadheader->num_groups ; i++ )
	{
		if( (i != group) && (i != group2) && (SoundInfo[group][i] >= 1.0F) )
		{
			memset( &TempGroups , 0 , sizeof( TempGroups ) );
			Distance = IsGroupVisibleSoundInfo( Mloadheader , i , group2 );
			{
				if( Distance == -1.0F )
				{
					Distance = IsGroupVisibleSoundInfo2( Mloadheader , i , group2 );
				}
			}
			if( Distance > 1.0F )
			{
				Distance += SoundInfo[group][i];

				if( Distance < Min )
				{
					Min = Distance;
				}
			}
		}
	}
		
	if( Min != 100000.0F )
	{
		return Min;
	}
	return -1.0F;
}


/*===================================================================
	Procedure	:		Set up Sound Info...
	Input		:		MLOADHEADER *
	Output		:		Nothing
===================================================================*/
void InitSoundInfo( MLOADHEADER * Mloadheader )
{
#if MXV_VERSION_NUMBER <= 2
	uint16 i,e;
	// Find any groups that a group can see and any groups that are connected to those groups...
	for( i = 0 ; i < Mloadheader->num_groups ; i++ )
	{
		for( e = 0 ; e < Mloadheader->num_groups ; e++ )
		{
			SoundInfo[i][e] = 0.0F;
			if( i != e )
			{
				memset( &TempGroups , 0 , sizeof( TempGroups ) );
				if( IsGroupVisibleSoundInfo( Mloadheader , i , e ) == -1.0F )
				{
					SoundInfo[i][e] = IsGroupVisibleSoundInfo2( Mloadheader , i , e );
				}
			}
		}
	}

	// Find any groups that can see ANY of the allready cassified Groups....
	for( i = 0 ; i < Mloadheader->num_groups ; i++ )
	{
		for( e = 0 ; e < Mloadheader->num_groups ; e++ )
		{
			if( i != e )
			{
   				if( SoundInfo[i][e] == -1.0F )
   				{
   					SoundInfo[i][e] = IsGroupVisibleSoundInfo3( Mloadheader , i , e );
				}
			}
		}
	}
#endif
}


void ReadSoundInfo( MLOADHEADER *m, char **pbuf )
{
	float *buf;
	int j, k;

	buf = (float *) *pbuf;
	for ( j = 0; j < m->num_groups; j++ )
	{
		for ( k = 0; k < m->num_groups; k++ )
		{
			SoundInfo[ j ][ k ] = *buf++;
		}
	}

	*pbuf = (char *) buf;
}

/*===================================================================
	Procedure	:		Trigger Background Animation to change state...
	Input		:		uint16 * Data
				:		first uint16 is the Polyanim index number...
				:		the second uint16 is the state to change that anim to
	Output		:		Nothing
===================================================================*/
void TriggerBackgroundAnimationGo( uint16 * Data )
{
	POLYANIM * PolyAnim;
	TEXTUREANIMINFOINDEX * TAnimInfoIndexPnt;

	TAnimInfoIndexPnt = Mloadheader.AnimData.TAnimInfoIndex;
	if( !TAnimInfoIndexPnt )
		return;
	TAnimInfoIndexPnt += *Data;
	PolyAnim = Mloadheader.Group[TAnimInfoIndexPnt->Group].polyanim[TAnimInfoIndexPnt->Execbuf];
	PolyAnim += TAnimInfoIndexPnt->PolyAnim;
	PolyAnim->State = TEXTUREANIM_Go;
}
/*===================================================================
	Procedure	:		Trigger Background Animation to change state...
	Input		:		uint16 * Data
				:		first uint16 is the Polyanim index number...
				:		the second uint16 is the state to change that anim to
	Output		:		Nothing
===================================================================*/
void TriggerBackgroundAnimationStop( uint16 * Data )
{
	POLYANIM * PolyAnim;
	TEXTUREANIMINFOINDEX * TAnimInfoIndexPnt;

	TAnimInfoIndexPnt = Mloadheader.AnimData.TAnimInfoIndex;
	if( !TAnimInfoIndexPnt )
		return;
	TAnimInfoIndexPnt += *Data;
	PolyAnim = Mloadheader.Group[TAnimInfoIndexPnt->Group].polyanim[TAnimInfoIndexPnt->Execbuf];
	PolyAnim += TAnimInfoIndexPnt->PolyAnim;
	PolyAnim->State = TEXTUREANIM_Stop;
}



FILE *LoadTextureAnimations( FILE *f )
{
	uint16 group;
	int	execbuf;
	int	i;
	POLYANIM * PolyAnim;

	for ( group = 0; group < Mloadheader.num_groups; group++ )
	{
		for( execbuf=0 ; execbuf<Mloadheader.Group[group].num_execbufs; execbuf++)
		{
			PolyAnim = Mloadheader.Group[group].polyanim[execbuf];
			
			for( i = 0 ; i < Mloadheader.Group[group].num_animating_polys[execbuf] ; i++ )
			{
				fread( &PolyAnim->State, sizeof( PolyAnim->State ), 1, f );
				fread( &PolyAnim->MasterTime, sizeof( PolyAnim->MasterTime ), 1, f );
				fread( &PolyAnim->CurrentTime, sizeof( PolyAnim->CurrentTime ), 1, f );
				fread( &PolyAnim->CurrentOffset, sizeof( PolyAnim->CurrentOffset ), 1, f );
				fread( &PolyAnim->newframe, sizeof( PolyAnim->newframe ), 1, f );
				fread( PolyAnim->loopstack, sizeof( PolyAnim->loopstack ), 1, f );
				PolyAnim++;
			}
		}
	}
	return f;
}



FILE *SaveTextureAnimations( FILE *f )
{
	uint16 group;
	int	execbuf;
	int	i;
	POLYANIM * PolyAnim;

	for ( group = 0; group < Mloadheader.num_groups; group++ )
	{
		for( execbuf=0 ; execbuf<Mloadheader.Group[group].num_execbufs; execbuf++)
		{
			PolyAnim = Mloadheader.Group[group].polyanim[execbuf];
			
			for( i = 0 ; i < Mloadheader.Group[group].num_animating_polys[execbuf] ; i++ )
			{
				fwrite( &PolyAnim->State, sizeof( PolyAnim->State ), 1, f );
				fwrite( &PolyAnim->MasterTime, sizeof( PolyAnim->MasterTime ), 1, f );
				fwrite( &PolyAnim->CurrentTime, sizeof( PolyAnim->CurrentTime ), 1, f );
				fwrite( &PolyAnim->CurrentOffset, sizeof( PolyAnim->CurrentOffset ), 1, f );
				fwrite( &PolyAnim->newframe, sizeof( PolyAnim->newframe ), 1, f );
				fwrite( PolyAnim->loopstack, sizeof( PolyAnim->loopstack ), 1, f );
				PolyAnim++;
			}
		}
	}
	return f;
}

