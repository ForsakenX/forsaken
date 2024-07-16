/*===================================================================
	Debugging display routines collision skin
===================================================================*/
#include <stdio.h>
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "lines.h"
#include "primary.h"
#include "skin.h"
#include "util.h"

#define USE_BSP_COLOURS

/*===================================================================
	Externals
===================================================================*/
extern	bool		DebugInfo;
extern	MLOADHEADER Mloadheader;
extern	u_int16_t		NumGroupsVisible;
extern	u_int16_t		GroupsVisible[MAXGROUPS];
extern	BYTE		WhoIAm;

extern	DWORD		CurrentSrcBlend;
extern	DWORD		CurrentDestBlend;
extern	DWORD		CurrentTextureBlend;

extern	LINE		Lines[ MAXLINES ];
extern	CAMERA		CurrentCamera;
extern	bool		NodeCube;
extern	bool		OldNodeCube;
extern	bool		NodeCubeType;

extern	int			Depth;
extern	BSP_NODE *	BSP_Nodes[ 256 ];
extern	bool		Inside;
extern	float		CollisionRadius;
extern	bool		ShowColZones;

extern	MODEL		Models[MAXNUMOFMODELS];
extern	MATRIX		MATRIX_Identity;

#define	BOXSIZE		96.0F
#define	NODECUBE_Branch	false
#define	NODECUBE_Array	true

/*===================================================================
	Globals variables and structures
===================================================================*/
	static	LVERTEX		PortalVerts[ 310 * 3 ];
	static	TRIANGLE		PortalTris[ 310 ];
	static	int16_t			PortalVertCount;
	static	int16_t			PortalTriCount;
//	D3DEXECUTEBUFFERDESC	Portal_debdesc;
//	D3DEXECUTEDATA			Portal_d3dexdata;
//	LPDIRECT3DEXECUTEBUFFER Portal_Execs[ MAXPORTALEXECS ];
	RENDEROBJECT			Portal_Execs[ MAXPORTALEXECS ];
	int16_t					Num_Portal_Execs = 0;
	int16_t					ShowPortal = 0;

	static	LVERTEX		SkinVerts[ 310 * 3 ];
	static	TRIANGLE		SkinTris[ 310 ];
	static	int16_t			SkinVertCount;
	static	int16_t			SkinTriCount;
//	D3DEXECUTEBUFFERDESC	Skin_debdesc;
//	D3DEXECUTEDATA			Skin_d3dexdata;
//	LPDIRECT3DEXECUTEBUFFER Skin_Execs[ MAXSKINEXECS ];
	RENDEROBJECT			Skin_Execs[ MAXSKINEXECS ];
	int16_t					Num_Skin_Execs = 0;
	int16_t					ShowSkin = 0;
	bool					ShowTrigZones = false;
	bool					ShowEFZones = false;
	bool					ShowTeleports = false;

	static int16_t			NumNodeCubeLines = 0;
	u_int16_t					NodeCubeLines[ MAXLINES ];

	VECTOR					TempVerts[ 64 ];
	u_int16_t					SphereZones[ MAXNUMOFMODELS ];
	int16_t					NumSphereZones = 0;


/*===================================================================
	Procedure	:		Initialise Skin ExecList Table
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitSkinExecs( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
//		Skin_Execs[ Count ] = NULL;
		FSReleaseRenderObject(&Skin_Execs[ Count ]);
	}

	Num_Skin_Execs = 0;
}

/*===================================================================
	Procedure	:		Modify Collision Skin Execution Buffer
	Input		:		MLOADHEADER	*	Mloadheader
				:		int16_t			Number of Groups Visible
	Output		:		Nothing
===================================================================*/
bool CreateSkinExecList( MCLOADHEADER * MCloadheader, int16_t NumVisible )
{
	COLOR		color;
	COLOR		skincolor;
	COLOR		patchcolor;
	COLOR		backpatchcolor;
	MCFACE		*	FacePtr;
	int16_t			g;
	int16_t			Count;
	int16_t			FaceCnt;
	u_int16_t			Group;
	VERT			Verts[ 4 ];
	float			x=0, y=0, z=0;
	int16_t			Vnum;
	int16_t			VertCount;
	VECTOR			Vector1;
	VECTOR			Vector2;
	VECTOR			Normal;
	VECTOR			C;
	float			I;
	int32_t			Shade = 0;
	float			PlaneOffset;

	skincolor = RGBA_MAKE( 0, 0, 192, 128 );
	patchcolor = RGBA_MAKE( 0, 192, 0, 128 );
	backpatchcolor = RGBA_MAKE( 192, 0, 0, 128 );

	switch( ShowSkin )
	{
		case 0: 
			return true;
			break;
		case 2:
		case 4:
		case 6:
			NumVisible = 1;
			break;
		case 1:
		case 3:
		case 5:
		default:
			break;
	}

	if ( ShowSkin == 5 )
	{
		skincolor = RGBA_MAKE( 0, 0, 192, 128 );
		patchcolor = RGBA_MAKE( 0, 192, 0, 128 );
		backpatchcolor = RGBA_MAKE( 192, 0, 0, 128 );
	}
	else if ( ShowSkin == 6 )
	{
		skincolor = RGBA_MAKE( 0, 0, 0, 128 );
		patchcolor = RGBA_MAKE( 0, 192, 0, 128 );
		backpatchcolor = RGBA_MAKE( 192, 0, 0, 128 );
	}



	SkinVertCount = 0;
	SkinTriCount = 0;

	for( g = 0; g < NumVisible; g++ )
	{
		Group = GroupsVisible[ g ];

		FaceCnt = MCloadheader->num_of_faces_in_group[ Group ];
		FacePtr = MCloadheader->GroupFacePnt[ Group ];
	
		for( Count = 0; Count < FaceCnt; Count++, FacePtr++ )
		{
			Vnum = (int16_t) ( ( FacePtr->type & 1 ) + 3 );

			color = skincolor;
			if ( FacePtr->type & 0x200000L )
				color = patchcolor;
			if ( FacePtr->type & 0x800000L )
			{
				if ( !DebugInfo )
					continue; // ignore backfacing patch collision polys unless debugging
				color = backpatchcolor;
			}
			for( VertCount = 0; VertCount < Vnum; VertCount++ )
			{
				switch( FacePtr->type & 6 )
				{
					case 0:
						y = FacePtr->v[ VertCount ].u;
						z = FacePtr->v[ VertCount ].v;
						x = -( FacePtr->D + ( y * FacePtr->ny + z * FacePtr->nz ) ) / FacePtr->nx;
						break;
					case 2:
						x = FacePtr->v[ VertCount ].u;
						z = FacePtr->v[ VertCount ].v;
						y = -( FacePtr->D + ( x * FacePtr->nx + z * FacePtr->nz ) ) / FacePtr->ny;
						break;
					case 4:
						x = FacePtr->v[ VertCount ].u;
						y = FacePtr->v[ VertCount ].v;
						z = -( FacePtr->D + ( x * FacePtr->nx + y * FacePtr->ny ) ) / FacePtr->nz;
						break;
					default:
						break;

				}
				Verts[ VertCount ].x = x;
				Verts[ VertCount ].y = y;
				Verts[ VertCount ].z = z;
			}

			if( ShowSkin == 5 )
			{
				Vector1.x = ( Verts[ 0 ].x - Verts[ 1 ].x );
				Vector1.y = ( Verts[ 0 ].y - Verts[ 1 ].y );
				Vector1.z = ( Verts[ 0 ].z - Verts[ 1 ].z );
				Vector2.x = ( Verts[ 2 ].x - Verts[ 1 ].x );
				Vector2.y = ( Verts[ 2 ].y - Verts[ 1 ].y );
				Vector2.z = ( Verts[ 2 ].z - Verts[ 1 ].z );
				CrossProduct( &Vector1, &Vector2, &Normal );
				NormaliseVector( &Normal );
				PlaneOffset = -DotProduct( (VECTOR *) &Verts[ 0 ], &Normal );
				I = ( Normal.x + Normal.y + Normal.z + PlaneOffset );
				*(float *) &Shade = I;
				srand( Shade );
				C.x = (float) rand() / RAND_MAX - 0.5F;
				C.y = (float) rand() / RAND_MAX - 0.5F;
				C.z = (float) rand() / RAND_MAX - 0.5F;
				NormaliseVector( &C );
				C.x = 0.5F + 0.5F * C.x;
				C.y = 0.5F + 0.5F * C.y;
				C.z = 0.5F + 0.5F * C.z;
				color = RGB_MAKE( (int32_t) floor(C.x * 255), (int32_t) floor(C.y * 255), (int32_t) floor(C.z * 255) );
			}

			SkinVerts[ SkinVertCount ].x = Verts[ 0 ].x;
			SkinVerts[ SkinVertCount ].y = Verts[ 0 ].y;
			SkinVerts[ SkinVertCount ].z = Verts[ 0 ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;

			SkinVerts[ SkinVertCount ].x = Verts[ 1 ].x;
			SkinVerts[ SkinVertCount ].y = Verts[ 1 ].y;
			SkinVerts[ SkinVertCount ].z = Verts[ 1 ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;

			SkinVerts[ SkinVertCount ].x = Verts[ 2 ].x;
			SkinVerts[ SkinVertCount ].y = Verts[ 2 ].y;
			SkinVerts[ SkinVertCount ].z = Verts[ 2 ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;

			SkinTris[ SkinTriCount ].v1 = SkinVertCount - 3;
			SkinTris[ SkinTriCount ].v2 = SkinVertCount - 2;
			SkinTris[ SkinTriCount ].v3 = SkinVertCount - 1;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;

			if( FacePtr->type & 1 )
			{
//				SkinTris[ SkinTriCount-1 ].wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );

				SkinVerts[ SkinVertCount ].x = Verts[ 2 ].x;
				SkinVerts[ SkinVertCount ].y = Verts[ 2 ].y;
				SkinVerts[ SkinVertCount ].z = Verts[ 2 ].z;
				SkinVerts[ SkinVertCount ].tu = 0.0F;
				SkinVerts[ SkinVertCount ].tv = 0.0F;
				SkinVerts[ SkinVertCount ].color = color;
//				SkinVerts[ SkinVertCount ].dwReserved = 0;
				SkinVertCount++;
	
				SkinVerts[ SkinVertCount ].x = Verts[ 3 ].x;
				SkinVerts[ SkinVertCount ].y = Verts[ 3 ].y;
				SkinVerts[ SkinVertCount ].z = Verts[ 3 ].z;
				SkinVerts[ SkinVertCount ].tu = 0.0F;
				SkinVerts[ SkinVertCount ].tv = 0.0F;
				SkinVerts[ SkinVertCount ].color = color;
//				SkinVerts[ SkinVertCount ].dwReserved = 0;
				SkinVertCount++;
	
				SkinVerts[ SkinVertCount ].x = Verts[ 0 ].x;
				SkinVerts[ SkinVertCount ].y = Verts[ 0 ].y;
				SkinVerts[ SkinVertCount ].z = Verts[ 0 ].z;
				SkinVerts[ SkinVertCount ].tu = 0.0F;
				SkinVerts[ SkinVertCount ].tv = 0.0F;
				SkinVerts[ SkinVertCount ].color = color;
//				SkinVerts[ SkinVertCount ].dwReserved = 0;
				SkinVertCount++;
	
				SkinTris[ SkinTriCount ].v1 = SkinVertCount - 3;
				SkinTris[ SkinTriCount ].v2 = SkinVertCount - 2;
				SkinTris[ SkinTriCount ].v3 = SkinVertCount - 1;
//				SkinTris[ SkinTriCount ].wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				SkinTriCount++;
			}

			if( SkinTriCount > MAX_TRIS_PER_EXEC )
			{
				if( MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount ) == false ) return false;
				SkinVertCount = 0;
				SkinTriCount = 0;
			}
		}
	}

	if( SkinTriCount )
	{
		if( MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount ) == false ) return false;
		SkinVertCount = 0;
		SkinTriCount = 0;
	}
	return true;
}

/*===================================================================
	Procedure	:		Create New Execute Buffer
	Input		:		Nothing
	Output		:		bool		True/False
===================================================================*/
void DisplayBSPNode( BSP_NODE * Node )
{
#ifdef BSP
	int16_t			Count;
	int16_t			Vnum;
	VECTOR			TLPos;
	VECTOR			BRPos;
	COLOR		color;
	COLOR		tempcolor;
	float			D;
	BSP_NODE	*	TempNode;
	float			NumNodes = 0.0F;
	int16_t			NodeNum;
#endif

	SkinVertCount = 0;
	SkinTriCount = 0;

	ReleaseSkinExecs();
	KillNodeCubeLines();
	KillBoxLines();
	KillAllSphereZones();

#ifdef BSP
	if( NodeCube ) DisplayNewBSPNode( Node );
	if( !OldNodeCube ) return;

	CreateLineBox();

	switch( NodeCubeType )
	{
		case NODECUBE_Branch:
			TempNode = Node;

			while( TempNode )
			{
				TempNode = TempNode->Parent;
				NumNodes += 1.0F;
			}
		
			while( Node )
			{
				D = ( DotProduct( &CurrentCamera.Pos, &Node->Normal ) + Node->Offset );
				if( D < 0 )
				{
					tempcolor = Node->Colour;
					color = ( ( ( ( tempcolor & 0xFF000000 ) >> 1 ) & 0xFF000000 ) |
							  ( ( ( tempcolor & 0x00FF0000 ) >> 1 ) & 0x00FF0000 ) |
							  ( ( ( tempcolor & 0x0000FF00 ) >> 1 ) & 0x0000FF00 ) |
							    ( ( tempcolor & 0x000000FF ) >> 1 ) );
				}
				else
				{
					color = Node->Colour;
				}
		
				TLPos.x = ( Ships[ WhoIAm ].Object.Pos.x - BOXSIZE );
				TLPos.y = ( Ships[ WhoIAm ].Object.Pos.y - BOXSIZE );
				TLPos.z = ( Ships[ WhoIAm ].Object.Pos.z - BOXSIZE );
			
				BRPos.x = ( Ships[ WhoIAm ].Object.Pos.x + BOXSIZE );
				BRPos.y = ( Ships[ WhoIAm ].Object.Pos.y + BOXSIZE );
				BRPos.z = ( Ships[ WhoIAm ].Object.Pos.z + BOXSIZE );
		
				Vnum = PlaneToCubeIntersectionShape( &TLPos, &BRPos, &Node->Normal, Node->Offset, &TempVerts[0] );
			
				for( Count = 0; Count < Vnum; Count++ )
				{
					SkinVerts[ SkinVertCount ].x = TempVerts[ Count ].x;
					SkinVerts[ SkinVertCount ].y = TempVerts[ Count ].y;
					SkinVerts[ SkinVertCount ].z = TempVerts[ Count ].z;
					SkinVerts[ SkinVertCount ].tu = 0.0F;
					SkinVerts[ SkinVertCount ].tv = 0.0F;
					SkinVerts[ SkinVertCount ].color = color;
//					SkinVerts[ SkinVertCount ].dwReserved = 0;
					SkinVertCount++;
				}
			
				for( Count = 0; Count < ( Vnum - 2 ); Count++ )
				{
					SkinTris[ SkinTriCount ].v1 = 0;
					SkinTris[ SkinTriCount ].v2 = Count+1;
					SkinTris[ SkinTriCount ].v3 = Count+2;
//					SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					SkinTriCount++;
		
					SkinTris[ SkinTriCount ].v3 = 0;
					SkinTris[ SkinTriCount ].v2 = Count+1;
					SkinTris[ SkinTriCount ].v1 = Count+2;
//					SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					SkinTriCount++;
				}
			
				if( SkinTriCount )
				{
					MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount );
					SkinVertCount = 0;
					SkinTriCount = 0;
				}
		
				Node = Node->Parent;
				NumNodes--;
			}
			break;

		case NODECUBE_Array:
			for( NodeNum = 0; NodeNum < Depth; NodeNum++ )
			{
				if(!BSP_Nodes[NodeNum])
					continue;

				D = ( DotProduct( &CurrentCamera.Pos, &BSP_Nodes[ NodeNum ]->Normal ) + BSP_Nodes[ NodeNum ]->Offset );

				if( D < 0 )
				{
					tempcolor = BSP_Nodes[ NodeNum ]->Colour;
					color = ( ( ( ( tempcolor & 0xFF000000 ) >> 1 ) & 0xFF000000 ) |
							  ( ( ( tempcolor & 0x00FF0000 ) >> 1 ) & 0x00FF0000 ) |
							  ( ( ( tempcolor & 0x0000FF00 ) >> 1 ) & 0x0000FF00 ) |
							    ( ( tempcolor & 0x000000FF ) >> 1 ) );
				}
				else
				{
					color = BSP_Nodes[ NodeNum ]->Colour;
				}

				TLPos.x = ( Ships[ WhoIAm ].Object.Pos.x - BOXSIZE );
				TLPos.y = ( Ships[ WhoIAm ].Object.Pos.y - BOXSIZE );
				TLPos.z = ( Ships[ WhoIAm ].Object.Pos.z - BOXSIZE );
			
				BRPos.x = ( Ships[ WhoIAm ].Object.Pos.x + BOXSIZE );
				BRPos.y = ( Ships[ WhoIAm ].Object.Pos.y + BOXSIZE );
				BRPos.z = ( Ships[ WhoIAm ].Object.Pos.z + BOXSIZE );
		
				Vnum = PlaneToCubeIntersectionShape( &TLPos, &BRPos, &BSP_Nodes[ NodeNum ]->Normal, BSP_Nodes[ NodeNum ]->Offset, &TempVerts[0] );
			
				for( Count = 0; Count < Vnum; Count++ )
				{
					SkinVerts[ SkinVertCount ].x = TempVerts[ Count ].x;
					SkinVerts[ SkinVertCount ].y = TempVerts[ Count ].y;
					SkinVerts[ SkinVertCount ].z = TempVerts[ Count ].z;
					SkinVerts[ SkinVertCount ].tu = 0.0F;
					SkinVerts[ SkinVertCount ].tv = 0.0F;
					SkinVerts[ SkinVertCount ].color = color;
//					SkinVerts[ SkinVertCount ].dwReserved = 0;
					SkinVertCount++;
				}
			
				for( Count = 0; Count < ( Vnum - 2 ); Count++ )
				{
					SkinTris[ SkinTriCount ].v1 = 0;
					SkinTris[ SkinTriCount ].v2 = Count+1;
					SkinTris[ SkinTriCount ].v3 = Count+2;
//					SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					SkinTriCount++;
		
					SkinTris[ SkinTriCount ].v3 = 0;
					SkinTris[ SkinTriCount ].v2 = Count+1;
					SkinTris[ SkinTriCount ].v1 = Count+2;
//					SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					SkinTriCount++;
				}
			
				if( SkinTriCount )
				{
					MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount );
					SkinVertCount = 0;
					SkinTriCount = 0;
				}
			}
			break;
	}
#endif
}

/*===================================================================
	Procedure	:		Display Node Volume
	Input		:		BSP_NODE	*	Node
	Output		:		Nothing
===================================================================*/
void DisplayNewBSPNode( BSP_NODE * Node )
{
#ifdef BSP
	int16_t			Count;
	int16_t			Vnum;
	COLOR		color;
	BSP_NODE	*	TempNode;
	BSP_NODE	*	StartNode;
	int				Side;
	u_int16_t			line;

	Side = -1;
	StartNode = Node;

	while( Node )
	{
#ifdef USE_BSP_COLOURS
		color = Node->Colour | 0x80000000L;
#else
		color = RGBA_MAKE( 128, 0, 0, 128 );
#endif

		Vnum = GimmeNodeVertices( Node, StartNode, Side, &TempVerts[ 0 ] );
	
		for( Count = 0; Count < Vnum; Count++ )
		{
			line = FindFreeLine();						// Line attached

			if( line != (u_int16_t ) -1 )
			{
				if( Count == ( Vnum - 1 ) )
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ 0 ];
				}
				else
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ Count + 1 ];
				}
					
				Lines[ line ].StartCol.R = 255;
				Lines[ line ].StartCol.G = 255;
				Lines[ line ].StartCol.B = 255;
				Lines[ line ].EndCol.R = 255;
				Lines[ line ].EndCol.G = 255;
				Lines[ line ].EndCol.B = 255;
				Lines[ line ].StartTrans = 255;
				Lines[ line ].EndTrans = 255;
				Lines[ line ].Group = Ships[ WhoIAm ].Object.Group;
				NodeCubeLines[ NumNodeCubeLines ] = line;
				NumNodeCubeLines++;
			}
		}

		for( Count = 0; Count < Vnum; Count++ )
		{
			SkinVerts[ SkinVertCount ].x = TempVerts[ Count ].x;
			SkinVerts[ SkinVertCount ].y = TempVerts[ Count ].y;
			SkinVerts[ SkinVertCount ].z = TempVerts[ Count ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;
		}
	
		for( Count = 0; Count < ( Vnum - 2 ); Count++ )
		{
			SkinTris[ SkinTriCount ].v1 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v3 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
	
			SkinTris[ SkinTriCount ].v3 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v1 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
		}
	
		if( SkinTriCount )
		{
			MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount );
			SkinVertCount = 0;
			SkinTriCount = 0;
		}
	
		TempNode = Node;
		Node = TempNode->Parent;
	}
#endif
}

/*===================================================================
	Procedure	:		Kill all Sphere Zones
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void KillAllSphereZones( void )
{
	int16_t	Count;

	for( Count = 0; Count < NumSphereZones; Count++ )
	{
		if( SphereZones[ Count ] != (u_int16_t) -1 )
		{
			KillUsedModel( SphereZones[ Count ] );
			SphereZones[ Count ] = (u_int16_t) -1;
		}
	}

	NumSphereZones = 0;
}

/*===================================================================
	Procedure	:		Display sphere zone
	Input		:		VECTOR		*	Pos
				:		float			Radius
				:		u_int16_t			Group
				:		u_int8_t			Red
				:		u_int8_t			Green
				:		u_int8_t			Blue
				:		u_int8_t			Trans
	Output		:		Nothing
===================================================================*/
void DisplaySphereZone( VECTOR * Pos, float Radius, u_int16_t Group,
					   u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans )
{
	float	Scale;
	u_int16_t	Model;

	Model =	FindFreeModel();

	if( Model != (u_int16_t ) -1 )
	{
		Scale = ( ( Radius * 2.0F ) / ( 512.0F * GLOBAL_SCALE ) );

		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = *Pos;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_SphereZone;
		Models[ Model ].MaxScale = Scale;
		Models[ Model ].Scale = Scale;
		Models[ Model ].Group = Group;
		Models[ Model ].Red = (int) Red;
		Models[ Model ].Green = (int) Green;
		Models[ Model ].Blue = (int) Blue;
		Models[ Model ].LifeCount = 1000000.0F;

		SphereZones[ NumSphereZones ] = Model;
		NumSphereZones++;
	}
}

/*===================================================================
	Procedure	:		Display Zone Direction
	Input		:		VECTOR		*	Pos
				:		VECTOR		*	Dir
				:		u_int16_t			Group
	Output		:		Nothing
===================================================================*/
void DisplayZoneDirection( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	Model;
	QUAT	TempQuat;
	VECTOR	TempDir;

	TempDir.x = -Dir->x;
	TempDir.y = -Dir->y;
	TempDir.z = -Dir->z;

	Model =	FindFreeModel();

	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_TitanStar;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = *Pos;
		QuatFromVector2( &TempDir, &TempQuat );
		QuatToMatrix( &TempQuat, &Models[ Model ].Mat );
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Nothing;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = 1000000.0F;

		SphereZones[ NumSphereZones ] = Model;
		NumSphereZones++;
	}
}

/*===================================================================
	Procedure	:		Display Direction and Up
	Input		:		VECTOR		*	Pos
				:		VECTOR		*	Dir
				:		VECTOR		*	Up
				:		u_int16_t			Group
	Output		:		Nothing
===================================================================*/
void DisplayDirAndUp( VECTOR * Pos, VECTOR * Dir, VECTOR *Up, u_int16_t Group )
{
	u_int16_t	Model;
	QUAT	TempQuat;
	VECTOR	TempDir;

	TempDir.x = -Dir->x;
	TempDir.y = -Dir->y;
	TempDir.z = -Dir->z;

	Model =	FindFreeModel();

	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_Heatseaker;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = *Pos;
		QuatFromDirAndUp( Dir, Up, &TempQuat );
		QuatToMatrix( &TempQuat, &Models[ Model ].Mat );
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Nothing;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = 1000000.0F;

		SphereZones[ NumSphereZones ] = Model;
		NumSphereZones++;
	}
}

/*===================================================================
	Procedure	:		Display Trigger Zone Volume
	Input		:		TRIGGER_ZONE *	Zone Sides
				:		int16_t			NumSides
				:		u_int16_t			Group
	Output		:		Nothing
===================================================================*/
void DisplayTriggerZone( TRIGGER_ZONE * Sides, int16_t NumSides, u_int16_t Group,
						 u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans )
{
	int16_t			Count;
	int16_t			Vnum;
	COLOR		color;
	TRIGGER_ZONE *	StartSide;
	int				Side;
	u_int16_t			line;
	int16_t			SideCount;

	Side = -1;
	StartSide = Sides;

	for( SideCount = 0; SideCount < NumSides; SideCount++ )
	{
		color = RGBA_MAKE( Red, Green, Blue, Trans ); //0, 64, 0, 128 );

		Vnum = GimmeTriggerZoneVertices( Sides, StartSide, NumSides, Side, &TempVerts[ 0 ] );
	
		for( Count = 0; Count < Vnum; Count++ )
		{
			line = FindFreeLine();						// Line attached

			if( line != (u_int16_t ) -1 )
			{
				if( Count == ( Vnum - 1 ) )
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ 0 ];
				}
				else
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ Count + 1 ];
				}
					
				Lines[ line ].StartCol.R = 255;
				Lines[ line ].StartCol.G = 255;
				Lines[ line ].StartCol.B = 255;
				Lines[ line ].EndCol.R = 255;
				Lines[ line ].EndCol.G = 255;
				Lines[ line ].EndCol.B = 255;
				Lines[ line ].StartTrans = 255;
				Lines[ line ].EndTrans = 255;
				Lines[ line ].Group = Group;
				NodeCubeLines[ NumNodeCubeLines ] = line;
				NumNodeCubeLines++;
			}
		}

		for( Count = 0; Count < Vnum; Count++ )
		{
			SkinVerts[ SkinVertCount ].x = TempVerts[ Count ].x;
			SkinVerts[ SkinVertCount ].y = TempVerts[ Count ].y;
			SkinVerts[ SkinVertCount ].z = TempVerts[ Count ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;
		}
	
		for( Count = 0; Count < ( Vnum - 2 ); Count++ )
		{
			SkinTris[ SkinTriCount ].v1 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v3 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
	
			SkinTris[ SkinTriCount ].v3 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v1 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
		}
	
		if( SkinTriCount )
		{
			MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount );
			SkinVertCount = 0;
			SkinTriCount = 0;
		}
	
		Sides++;
	}
}

/*===================================================================
	Procedure	:		Display ColZone Volume
	Input		:		ZONESIDE	*	Zone Sides
				:		int16_t			NumSides
				:		u_int16_t			Group
				:		int16_t			Sensitive
	Output		:		Nothing
===================================================================*/
void DisplayColZone( ZONESIDE * Sides, int16_t NumSides, u_int16_t Group, int16_t Sensitive )
{
	int16_t			Count;
	int16_t			Vnum;
	COLOR		color;
	ZONESIDE	*	StartSide;
	int				Side;
	u_int16_t			line;
	int16_t			SideCount;

	if( !ShowColZones || !NumSides || !Sides ) return;

	Side = -1;
	StartSide = Sides;

	for( SideCount = 0; SideCount < NumSides; SideCount++ )
	{
		if( Sensitive )
		{
			color = RGBA_MAKE( 64, 0, 0, 128 ); 
		}
		else
		{
			if( Sides->Sensitive ) color = RGBA_MAKE( 64, 0, 0, 128 ); 
			else color = RGBA_MAKE( 0, 64, 0, 128 );
		}

		Vnum = GimmeColZoneVertices( Sides, StartSide, NumSides, Side, &TempVerts[ 0 ] );
	
		for( Count = 0; Count < Vnum; Count++ )
		{
			line = FindFreeLine();						// Line attached

			if( line != (u_int16_t ) -1 )
			{
				if( Count == ( Vnum - 1 ) )
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ 0 ];
				}
				else
				{
					Lines[ line ].StartPos = TempVerts[ Count ];
					Lines[ line ].EndPos = TempVerts[ Count + 1 ];
				}
					
				Lines[ line ].StartCol.R = 255;
				Lines[ line ].StartCol.G = 255;
				Lines[ line ].StartCol.B = 255;
				Lines[ line ].EndCol.R = 255;
				Lines[ line ].EndCol.G = 255;
				Lines[ line ].EndCol.B = 255;
				Lines[ line ].StartTrans = 255;
				Lines[ line ].EndTrans = 255;
				Lines[ line ].Group = Group;
				NodeCubeLines[ NumNodeCubeLines ] = line;
				NumNodeCubeLines++;
			}
		}

		for( Count = 0; Count < Vnum; Count++ )
		{
			SkinVerts[ SkinVertCount ].x = TempVerts[ Count ].x;
			SkinVerts[ SkinVertCount ].y = TempVerts[ Count ].y;
			SkinVerts[ SkinVertCount ].z = TempVerts[ Count ].z;
			SkinVerts[ SkinVertCount ].tu = 0.0F;
			SkinVerts[ SkinVertCount ].tv = 0.0F;
			SkinVerts[ SkinVertCount ].color = color;
//			SkinVerts[ SkinVertCount ].dwReserved = 0;
			SkinVertCount++;
		}
	
		for( Count = 0; Count < ( Vnum - 2 ); Count++ )
		{
			SkinTris[ SkinTriCount ].v1 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v3 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
	
			SkinTris[ SkinTriCount ].v3 = 0;
			SkinTris[ SkinTriCount ].v2 = Count+1;
			SkinTris[ SkinTriCount ].v1 = Count+2;
//			SkinTris[ SkinTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			SkinTriCount++;
		}
	
		if( SkinTriCount )
		{
			MakeNewSkinExec( &SkinVerts[ 0 ], &SkinTris[ 0 ], SkinVertCount, SkinTriCount );
			SkinVertCount = 0;
			SkinTriCount = 0;
		}
	
		Sides++;
	}
}

/*===================================================================
	Procedure	:		Kill Node Cube Lines
	Input		:		BSP_NODE	*	Node
	Output		:		Nothing
===================================================================*/
void KillNodeCubeLines( void )
{
	int16_t	Count;

	for( Count = 0; Count < NumNodeCubeLines; Count++ )
	{
		if( NodeCubeLines[ Count ] != (u_int16_t) -1 )
		{
			KillUsedLine( NodeCubeLines[ Count ] );
			NodeCubeLines[ Count ] = (u_int16_t) -1;
		}
	}

	NumNodeCubeLines = 0;
}

/*===================================================================
	Procedure	:		Create New Execute Buffer
	Input		:		Nothing
	Output		:		bool		True/False
===================================================================*/
bool MakeNewSkinExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16_t	NumVerts, int16_t NumTris )
{
    LPLVERTEX	lpBufStart = NULL;//, lpInsStart, lpPointer;

	if( Num_Skin_Execs >= MAXSKINEXECS )
	{
		DebugPrintf( "Skin Execs Overflow\n" );
		return false;
	}

	if (!FSCreateVertexBuffer(&Skin_Execs[ Num_Skin_Execs ], NumVerts))
	{
		return false;
	}
   
	if (!(FSLockVertexBuffer(&Skin_Execs[ Num_Skin_Execs ], &lpBufStart)))
	{
		return false;
	}
   		
//   	lpBufStart = Skin_debdesc.lpData;
//   	lpPointer = lpBufStart;
   		
	memmove(lpBufStart, Verts, sizeof(LVERTEX) * NumVerts);//memcpy
   
//    lpInsStart = lpPointer;
   
   	if( ( ShowSkin == 3  ) || ( ShowSkin == 4 ) )
   	{
/* bjd - CHECK
   	   	OP_STATE_RENDER( 1, lpPointer);
   			STATE_DATA( D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME, lpPointer );
*/
   	}
   	else
   	{
		if( ShowSkin == 5 )
		{
/* bjd - CHECK
	   	   	OP_STATE_RENDER( 8, lpPointer);
				STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE, lpPointer );
				STATE_DATA( D3DRENDERSTATE_BLENDENABLE, false, lpPointer );
				STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, true, lpPointer );
				STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , false, lpPointer );
				STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE, lpPointer );
				STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPU, false, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPV, false, lpPointer );
*/
		}
   	}
/* bjd - CHECK
    OP_PROCESS_VERTICES( 1, lpPointer );
        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, 0, NumVerts, lpPointer );
    OP_STATE_RENDER( 1, lpPointer );
        STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer);
    OP_TRIANGLE_LIST( NumTris, lpPointer );
        TRIANGLE_LIST_DATA( Tris, NumTris, lpPointer );		// Copy Triangles to Execute Buffer
*/

//   	OP_EXIT( lpPointer );
   
//   	Skin_Execs[ Num_Skin_Execs ]->lpVtbl->Unlock( Skin_Execs[ Num_Skin_Execs ] );
	if (!(FSUnlockVertexBuffer(&Skin_Execs[ Num_Skin_Execs ])))
	{
		return false;
	}
/*		
   	memset( &Skin_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
   	Skin_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
   	Skin_d3dexdata.dwVertexCount = NumVerts;
   	Skin_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
   	Skin_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
   	if( ( Skin_Execs[ Num_Skin_Execs ]->lpVtbl->SetExecuteData( Skin_Execs[ Num_Skin_Execs ], &Skin_d3dexdata ) ) != D3D_OK) return false;
*/
	Num_Skin_Execs++;

	return true;
}

/*===================================================================
	Procedure	:		Release Skin Execs
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void ReleaseSkinExecs( void )
{
	int16_t	Count;

	for( Count = 0; Count < Num_Skin_Execs; Count++ )
	{
//		if( Skin_Execs[ Count ] != NULL )
		{
			FSReleaseRenderObject(&Skin_Execs[ Count ]);
//			Skin_Execs[ Count ] = NULL;
	 	}
	}

	Num_Skin_Execs = 0;
}


/*===================================================================
	Procedure	:		Modify Collision Portal Execution Buffer
	Input		:		MLOADHEADER	*	Mloadheader
				:		int16_t			Number of Groups Visible
	Output		:		Nothing
===================================================================*/
bool CreatePortalExecList( MLOADHEADER * Mloadheader, int16_t NumVisible )
{
	COLOR		color;
	int16_t			g;
	int16_t			Count, Count2;
	u_int16_t			VertCount;
	u_int16_t			TriCount;
	int16_t			PortalCnt;
	int16_t			PolyCount;
	PORTAL		*	PortalPtr;
	MCFACE		*	PolyPtr;
	VECTOR			Verts[ 4 ];
	float			x=0, y=0, z=0;

	PortalVertCount = 0;
	PortalTriCount = 0;
	ReleasePortalExecs();

	switch( ShowPortal )
	{
		case 0: 
			return true;
			break;
		case 2:
		case 4:
			NumVisible = 1;
			break;
		case 1:
		case 3:
		default:
			break;
	}

	color = RGBA_MAKE( 192, 0, 0, 128 );

	for( g = 0; g < NumVisible; g++ )
	{
		PortalCnt = Mloadheader->Group[ GroupsVisible[ g ] ].num_portals;
		PortalPtr = Mloadheader->Group[ GroupsVisible[ g ] ].Portal;
	
		for( Count = 0; Count < PortalCnt; Count++ )
		{
			PolyPtr = PortalPtr->Poly;
			for ( PolyCount = 0; PolyCount < PortalPtr->num_polys_in_portal; PolyCount++ )
			{
				VertCount = (int16_t) ( ( PolyPtr->type & 1 ) + 3 );

				for( Count2 = 0; Count2 < VertCount; Count2++ )
				{
					switch( PolyPtr->type & 6 )
					{
						case 0:
							y = PolyPtr->v[ Count2 ].u;
							z = PolyPtr->v[ Count2 ].v;
							x = -( PolyPtr->D + ( y * PolyPtr->ny + z * PolyPtr->nz ) ) / PolyPtr->nx;
							break;
						case 2:
							x = PolyPtr->v[ Count2 ].u;
							z = PolyPtr->v[ Count2 ].v;
							y = -( PolyPtr->D + ( x * PolyPtr->nx + z * PolyPtr->nz ) ) / PolyPtr->ny;
							break;
						case 4:
							x = PolyPtr->v[ Count2 ].u;
							y = PolyPtr->v[ Count2 ].v;
							z = -( PolyPtr->D + ( x * PolyPtr->nx + y * PolyPtr->ny ) ) / PolyPtr->nz;
							break;
						default:
							break;

					}
					Verts[ Count2 ].x = x;
					Verts[ Count2 ].y = y;
					Verts[ Count2 ].z = z;
				}

				TriCount = ( VertCount - 2 );

				for( Count2 = 0; Count2 < VertCount; Count2++ )
				{
					PortalVerts[ PortalVertCount + Count2 ].x = Verts[ Count2 ].x;
					PortalVerts[ PortalVertCount + Count2 ].y = Verts[ Count2 ].y;
					PortalVerts[ PortalVertCount + Count2 ].z = Verts[ Count2 ].z;
					PortalVerts[ PortalVertCount + Count2 ].tu = 0.0F;
					PortalVerts[ PortalVertCount + Count2 ].tv = 0.0F;
					PortalVerts[ PortalVertCount + Count2 ].color = color;
//					PortalVerts[ PortalVertCount + Count2 ].dwReserved = 0;
				}

				for( Count2 = 0; Count2 < TriCount; Count2++ )				// Count Tris
				{
					PortalTris[ PortalTriCount ].v1 = PortalVertCount;
					PortalTris[ PortalTriCount ].v2 = PortalVertCount + Count2 + 1;
					PortalTris[ PortalTriCount ].v3 = PortalVertCount + Count2 + 2;
//					PortalTris[ PortalTriCount ].wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					PortalTriCount++;
				}

				PortalVertCount += VertCount;

				if( PortalTriCount > MAX_TRIS_PER_EXEC )
				{
					if( MakeNewPortalExec( &PortalVerts[ 0 ], &PortalTris[ 0 ], PortalVertCount, PortalTriCount ) == false ) return false;
					PortalVertCount = 0;
					PortalTriCount = 0;
				}

				PolyPtr++;
			}

			PortalPtr++;
		}
	}

	if( PortalTriCount )
	{
		if( MakeNewPortalExec( &PortalVerts[ 0 ], &PortalTris[ 0 ], PortalVertCount, PortalTriCount ) == false ) return false;
		PortalVertCount = 0;
		PortalTriCount = 0;
	}

	return true;
}

/*===================================================================
	Procedure	:		Create New Execute Buffer for portal polys
	Input		:		Nothing
	Output		:		bool		True/False
===================================================================*/
bool MakeNewPortalExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16_t NumVerts, int16_t NumTris )
{
    LPLVERTEX	lpBufStart = NULL;//, lpInsStart, lpPointer;

	if( Num_Portal_Execs >= MAXPORTALEXECS )
	{
		DebugPrintf( "Portal Execs Overflow\n" );
		return false;
	}

	if (!FSCreateVertexBuffer(&Portal_Execs[ Num_Portal_Execs ], NumVerts))
	{
		return false;
	}

	if (!(FSLockVertexBuffer(&Portal_Execs[ Num_Portal_Execs ], &lpBufStart)))
	{
		return false;
	}
   		
//   	lpBufStart = Portal_debdesc.lpData;
//   	lpPointer = lpBufStart;
   		
	memmove(lpBufStart, Verts, sizeof(LVERTEX) * NumVerts);//memcpy
   
//    lpInsStart = lpPointer;

/* bjd - CHECK   
    OP_PROCESS_VERTICES( 1, lpPointer );
        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, 0, NumVerts, lpPointer );
    OP_STATE_RENDER( 1, lpPointer );
        STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer);
    OP_TRIANGLE_LIST( NumTris, lpPointer );
        TRIANGLE_LIST_DATA( Tris, NumTris, lpPointer );		// Copy Triangles to Execute Buffer
*/		
   
//   	OP_EXIT( lpPointer );
   
//   	Portal_Execs[ Num_Portal_Execs ]->lpVtbl->Unlock( Portal_Execs[ Num_Portal_Execs ] );

	if (!(FSUnlockVertexBuffer(&Portal_Execs[ Num_Portal_Execs ])))
	{
		return false;
	}
/*   		
   	memset( &Portal_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
   	Portal_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
   	Portal_d3dexdata.dwVertexCount = NumVerts;
   	Portal_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
   	Portal_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
   	if( ( Portal_Execs[ Num_Portal_Execs ]->lpVtbl->SetExecuteData( Portal_Execs[ Num_Portal_Execs ], &Portal_d3dexdata ) ) != D3D_OK) return false;
*/
	Num_Portal_Execs++;

	return true;
}

/*===================================================================
	Procedure	:		Initialise Portal ExecList Table
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitPortalExecs( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		FSReleaseRenderObject(&Portal_Execs[ Count ]);
//		Portal_Execs[ Count ] = NULL;
	}

	Num_Portal_Execs = 0;
}

/*===================================================================
	Procedure	:		Release Portal Execs
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void ReleasePortalExecs( void )
{
	int16_t	Count;

	for( Count = 0; Count < Num_Portal_Execs; Count++ )
	{
//		if( Portal_Execs[ Count ] != NULL )
		{
			FSReleaseRenderObject(&Portal_Execs[ Count ]);
//			Portal_Execs[ Count ] = NULL;
	 	}
	}

	Num_Portal_Execs = 0;
}

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/

struct
{
	float	x, y, z;
	bool	flag;

} PointList[ 12 ];

#define	SET_POINT( idx, x, y, z )	{ PointList[ idx ].x = x; PointList[ idx ].y = y; PointList[ idx ].z = z; PointList[ idx ].flag = true; }

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------*\

	       -------10-------
	     /                 /
	    / |               / |
	   5  9              6  |
	  /   |             /   |
	 /                 /    |
	 --------1--------      11
	|                 |     |
	|     |           |     |
	|     |           |     |
	0      -------8-  |  -- 
	|    /            2    /
	|   4             |   /
	|  /              |  7
	| /               | /
	|                 |/
	 ---------3-------


	+y
	|  +z
	| /
	|/
	 ----- +x

\*---------------------------------------------------------------------------------------------------------------------------*/

int GetNextSide( int side )
{
	int new_side;

	new_side = -1;

	switch( side )
	{
    	case 11:
			     if( PointList[  2 ].flag )	new_side =  2;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[  7 ].flag )	new_side =  7;
			else if( PointList[  8 ].flag )	new_side =  8;
			else if( PointList[  9 ].flag )	new_side =  9;
			else if( PointList[ 10 ].flag )	new_side = 10;
			break;
    	case 10:
			     if( PointList[  9 ].flag )	new_side =  9;
			else if( PointList[  8 ].flag )	new_side =  8;
			else if( PointList[ 11 ].flag )	new_side = 11;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[  1 ].flag )	new_side =  1;
			break;
    	case 9:
			     if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  8 ].flag )	new_side =  8;
			else if( PointList[ 10 ].flag )	new_side = 10;
			else if( PointList[ 11 ].flag )	new_side = 11;
			break;
    	case 8:
			     if( PointList[  9 ].flag )	new_side =  9;
			else if( PointList[ 10 ].flag )	new_side = 10;
			else if( PointList[ 11 ].flag )	new_side = 11;
			else if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  7 ].flag )	new_side =  7;
			break;
    	case 7:
			     if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  8 ].flag )	new_side =  8;
			else if( PointList[  2 ].flag )	new_side =  2;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[ 11 ].flag )	new_side = 11;
			break;
    	case 6:
			     if( PointList[  1 ].flag )	new_side =  1;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[ 10 ].flag )	new_side = 10;
			else if( PointList[  2 ].flag )	new_side =  2;
			else if( PointList[  7 ].flag )	new_side =  7;
			else if( PointList[ 11 ].flag )	new_side = 11;
			break;
    	case 5:
			     if( PointList[  1 ].flag )	new_side =  1;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[ 10 ].flag )	new_side = 10;
			else if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  9 ].flag )	new_side =  9;
			break;
    	case 4:
			     if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[  9 ].flag )	new_side =  9;
			else if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  7 ].flag )	new_side =  7;
			else if( PointList[  8 ].flag )	new_side =  8;
			break;
		case 3:
			     if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  8 ].flag )	new_side =  8;
			else if( PointList[  7 ].flag )	new_side =  7;
			else if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  1 ].flag )	new_side =  1;
			else if( PointList[  2 ].flag )	new_side =  2;
			break;
		case 0:
			     if( PointList[  1 ].flag )	new_side =  1;
			else if( PointList[  2 ].flag )	new_side =  2;
			else if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  4 ].flag )	new_side =  4;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[  9 ].flag )	new_side =  9;
			break;
    	case 1:
			     if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  2 ].flag )	new_side =  2;
			else if( PointList[  5 ].flag )	new_side =  5;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[ 10 ].flag )	new_side = 10;
			break;
    	case 2:
			     if( PointList[  1 ].flag )	new_side =  1;
			else if( PointList[  0 ].flag )	new_side =  0;
			else if( PointList[  3 ].flag )	new_side =  3;
			else if( PointList[  6 ].flag )	new_side =  6;
			else if( PointList[  7 ].flag )	new_side =  7;
			else if( PointList[ 11 ].flag )	new_side = 11;
			break;
	}
	return new_side;
}


int PlaneToCubeIntersectionShape( VECTOR * min_ptr, VECTOR * max_ptr, VECTOR * vec_ptr, float offset, VECTOR * result_ptr )
{
	// Variables.

	float	x, y, z;
	int		i, count;
	int		side;
	
	// Code.

	for( i = 0; i < 12; i++ )
	{
		PointList[i].flag = false;
	}

	if( vec_ptr->y != 0 )
	{
		x = min_ptr->x;
		z = min_ptr->z;
		y = -( vec_ptr->x * x + vec_ptr->z * z + offset ) / vec_ptr->y;
		if( y < max_ptr->y && y > min_ptr->y )	SET_POINT( 0, x, y, z );

		x = max_ptr->x;
		z = min_ptr->z;
		y = -( vec_ptr->x * x + vec_ptr->z * z + offset ) / vec_ptr->y;
		if( y < max_ptr->y && y > min_ptr->y )	SET_POINT( 2, x, y, z );

		x = min_ptr->x;
		z = max_ptr->z;
		y = -( vec_ptr->x * x + vec_ptr->z * z + offset ) / vec_ptr->y;
		if( y < max_ptr->y && y > min_ptr->y )	SET_POINT( 9, x, y, z );

		x = max_ptr->x;
		z = max_ptr->z;
		y = -( vec_ptr->x * x + vec_ptr->z * z + offset ) / vec_ptr->y;
		if( y < max_ptr->y && y > min_ptr->y )	SET_POINT( 11, x, y, z );
	}

	if( vec_ptr->z != 0 )
	{
		x = min_ptr->x;
		y = min_ptr->y;
		z = -( vec_ptr->x * x + vec_ptr->y * y + offset ) / vec_ptr->z;
		if( z < max_ptr->z && z > min_ptr->z )	SET_POINT( 4, x, y, z );

		x = max_ptr->x;
		y = min_ptr->y;
		z = -( vec_ptr->x * x + vec_ptr->y * y + offset ) / vec_ptr->z;
		if( z < max_ptr->z && z > min_ptr->z )	SET_POINT( 7, x, y, z );

		x = min_ptr->x;
		y = max_ptr->y;
		z = -( vec_ptr->x * x + vec_ptr->y * y + offset ) / vec_ptr->z;
		if( z < max_ptr->z && z > min_ptr->z )	SET_POINT( 5, x, y, z );

		x = max_ptr->x;
		y = max_ptr->y;
		z = -( vec_ptr->x * x + vec_ptr->y * y + offset ) / vec_ptr->z;
		if( z < max_ptr->z && z > min_ptr->z )	SET_POINT( 6, x, y, z );
	}

	if( vec_ptr->x != 0 )
	{
		z = min_ptr->z;
		y = min_ptr->y;
		x = -( vec_ptr->z * z + vec_ptr->y * y + offset ) / vec_ptr->x;
		if( x < max_ptr->x && x > min_ptr->x )	SET_POINT( 3, x, y, z );

		z = max_ptr->z;
		y = min_ptr->y;
		x = -( vec_ptr->z * z + vec_ptr->y * y + offset ) / vec_ptr->x;
		if( x < max_ptr->x && x > min_ptr->x )	SET_POINT( 8, x, y, z );

		z = min_ptr->z;
		y = max_ptr->y;
		x = -( vec_ptr->z * z + vec_ptr->y * y + offset ) / vec_ptr->x;
		if( x < max_ptr->x && x > min_ptr->x )	SET_POINT( 1, x, y, z );

		z = max_ptr->z;
		y = max_ptr->y;
		x = -( vec_ptr->z * z + vec_ptr->y * y + offset ) / vec_ptr->x;
		if( x < max_ptr->x && x > min_ptr->x )	SET_POINT( 10, x, y, z );
	}

	// Write the points to the output, in order from smallest to largest x.

	// Find first side.

	side = -1;
	for( i = 0; i < 12; i++ )
	{
		if( PointList[ i ].flag )
		{
			side = i;
			break;
		}
	}

	count = 0;
	while( side != -1 )
	{
		result_ptr[ count ].x = PointList[ side ].x;
		result_ptr[ count ].y = PointList[ side ].y;
		result_ptr[ count ].z = PointList[ side ].z;
		PointList[ side ].flag = false;
		count++;

		side = GetNextSide( side );
	}

	return count;
}

u_int16_t	BoxLines[ 12 ] = { 0xffff, 0xffff, 0xffff, 0xffff,
						   0xffff, 0xffff, 0xffff, 0xffff,
						   0xffff, 0xffff, 0xffff, 0xffff };

VECTOR	BoxOffsets[ 24 ] = {
   	{ -BOXSIZE, -BOXSIZE, -BOXSIZE },
   	{  BOXSIZE, -BOXSIZE, -BOXSIZE },
   	{  BOXSIZE, -BOXSIZE, -BOXSIZE },
   	{  BOXSIZE,  BOXSIZE, -BOXSIZE },
   	{  BOXSIZE,  BOXSIZE, -BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE, -BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE, -BOXSIZE },
   	{ -BOXSIZE, -BOXSIZE, -BOXSIZE },
   						  
   	{ -BOXSIZE, -BOXSIZE,  BOXSIZE },
   	{  BOXSIZE, -BOXSIZE,  BOXSIZE },
   	{  BOXSIZE, -BOXSIZE,  BOXSIZE },
   	{  BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{  BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{ -BOXSIZE, -BOXSIZE,  BOXSIZE },

   	{ -BOXSIZE, -BOXSIZE,  BOXSIZE },
   	{ -BOXSIZE, -BOXSIZE, -BOXSIZE },
   	{  BOXSIZE, -BOXSIZE,  BOXSIZE },
   	{  BOXSIZE, -BOXSIZE, -BOXSIZE },
   	{  BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{  BOXSIZE,  BOXSIZE, -BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE,  BOXSIZE },
   	{ -BOXSIZE,  BOXSIZE, -BOXSIZE },
};

void CreateLineBox( void )
{ 
	int16_t	Count;
	u_int16_t	line;
	VECTOR	StartPos;
	VECTOR	EndPos;

	for( Count = 0; Count < 12; Count++ )
	{
		line = FindFreeLine();						// Line attached

		BoxLines[ Count ] = line;

		if( line != (u_int16_t ) -1 )
		{
			StartPos.x = ( Ships[ WhoIAm ].Object.Pos.x + BoxOffsets[ Count * 2 ].x );
			StartPos.y = ( Ships[ WhoIAm ].Object.Pos.y + BoxOffsets[ Count * 2 ].y );
			StartPos.z = ( Ships[ WhoIAm ].Object.Pos.z + BoxOffsets[ Count * 2 ].z );
			EndPos.x = ( Ships[ WhoIAm ].Object.Pos.x + BoxOffsets[ ( Count * 2 ) + 1 ].x );
			EndPos.y = ( Ships[ WhoIAm ].Object.Pos.y + BoxOffsets[ ( Count * 2 ) + 1 ].y );
			EndPos.z = ( Ships[ WhoIAm ].Object.Pos.z + BoxOffsets[ ( Count * 2 ) + 1 ].z );

			Lines[ line ].StartPos = StartPos;
			Lines[ line ].EndPos = EndPos;
			Lines[ line ].StartCol.R = 255;
			Lines[ line ].StartCol.G = 255;
			Lines[ line ].StartCol.B = 255;
			Lines[ line ].EndCol.R = 255;
			Lines[ line ].EndCol.G = 255;
			Lines[ line ].EndCol.B = 255;
			Lines[ line ].StartTrans = 255;
			Lines[ line ].EndTrans = 255;
			Lines[ line ].Group = Ships[ WhoIAm ].Object.Group;
		}
	}
}

void KillBoxLines( void )
{ 
	int16_t	Count;

	for( Count = 0; Count < 12; Count++ )
	{
		if( BoxLines[ Count ] != (u_int16_t) -1 )
		{
			KillUsedLine( BoxLines[ Count ] );
			BoxLines[ Count ] = (u_int16_t) -1;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/

int GetMajorAxis( VECTOR * vec_ptr )
{
	if( fabs( vec_ptr->x ) > fabs( vec_ptr->y ) )
	{
		if( fabs( vec_ptr->x ) > fabs( vec_ptr->z ) )
			return 0;
		else
			return 2;
	}
	else
	{
		if( fabs( vec_ptr->y ) > fabs( vec_ptr->z ) )
			return 1;
		else
			return 2;
	}
}

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/

typedef	struct Pin	Pin;

struct Pin
{
	Pin		* next_ptr;
	float	x, y, z;
	float	dist;
};

#define FAR_AWAY 64000.0F
#define	MAX_PINS	256

Pin	PinBuf[ MAX_PINS ];

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/

int GimmeNodeVertices( BSP_NODE * plane_ptr, BSP_NODE * space_ptr, int side, VECTOR * out_ptr )
{
	// Variables.

	Pin		* first_free_pin_ptr;
	Pin		* first_pin_ptr;

	Pin		* pin_ptr, * next_pin_ptr, * new_pin_ptr;
	Pin		** prev_pin_ptr_ptr;
	int		count;
	float	back_portion, front_portion;
	int		i;
	float	x,y,z;

	// Code.

	first_free_pin_ptr = NULL;
	first_pin_ptr = NULL;

	// Prepare PinBuf;

	for( i = 0; i < MAX_PINS; i++ )
	{
		PinBuf[ i ].next_ptr = first_free_pin_ptr;
		first_free_pin_ptr = &PinBuf[ i ];
	}

	// Create initial poly by clipping with four bounding planes.

	switch( GetMajorAxis( &plane_ptr->Normal ) )
	{
		case 0: // major X
			y = -FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y = -FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 1: // major Y
			x = -FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.y;
 			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->Offset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 2: // major Z
			x = -FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->Offset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

 			x =  FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->Offset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->Offset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->Offset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;
	}

	// Got the initial poly. Do the intersection business.

	while( NULL != space_ptr )
	{
		if( space_ptr != plane_ptr )
		{

			// Calculate distance from each pin to the intersecting plane.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				pin_ptr->dist = side * (	pin_ptr->x * space_ptr->Normal.x +
											pin_ptr->y * space_ptr->Normal.y +
											pin_ptr->z * space_ptr->Normal.z +
														 space_ptr->Offset );
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Create intersection points.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
				if( NULL == next_pin_ptr )
				{
					next_pin_ptr = first_pin_ptr;
				}
	
				// We've got a pair of points. See if there is an intersection.
	
				if( ( pin_ptr->dist < 0 && next_pin_ptr->dist >= 0 ) || ( pin_ptr->dist >= 0 && next_pin_ptr->dist < 0 ) )
				{
					// Grab new structure.
	
					if( NULL != first_free_pin_ptr )
					{
						new_pin_ptr = first_free_pin_ptr;
						first_free_pin_ptr = first_free_pin_ptr->next_ptr;
	
						// Set up new structure.
	
						front_portion = pin_ptr->dist / ( pin_ptr->dist - next_pin_ptr->dist );
						back_portion = 1 - front_portion;
	
						new_pin_ptr->x = next_pin_ptr->x * front_portion + pin_ptr->x * back_portion;
						new_pin_ptr->y = next_pin_ptr->y * front_portion + pin_ptr->y * back_portion;
						new_pin_ptr->z = next_pin_ptr->z * front_portion + pin_ptr->z * back_portion;
						new_pin_ptr->dist = 0.0F;
	
						// Link it into the list.
	
						new_pin_ptr->next_ptr = pin_ptr->next_ptr;
						pin_ptr->next_ptr = new_pin_ptr;
	
						// Ensure the new point is skipped.
	
						pin_ptr = new_pin_ptr;
					}
				}
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Remove dead points.
	
			pin_ptr = first_pin_ptr;
			prev_pin_ptr_ptr = &first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
	
				if( pin_ptr->dist < 0 )
				{
					// Remove from list.
	
					*prev_pin_ptr_ptr = pin_ptr->next_ptr;
	
					// Add to free list.
	
					pin_ptr->next_ptr = first_free_pin_ptr;
					first_free_pin_ptr = pin_ptr;
				}
				else
				{
					// Keep in list.
	
					prev_pin_ptr_ptr = &pin_ptr->next_ptr;
				}
	
				pin_ptr = next_pin_ptr;
			}
		}

		// Go to Parent plane.

		if( NULL != space_ptr->Parent )
		{
			if( space_ptr == space_ptr->Parent->Front )
				side = +1;
			else
				side = -1;
		}
		space_ptr = space_ptr->Parent;
	}

	count = 0;

	pin_ptr = first_pin_ptr;
	while( NULL != pin_ptr )
	{
		out_ptr->x = pin_ptr->x;
		out_ptr->y = pin_ptr->y;
		out_ptr->z = pin_ptr->z;

		out_ptr++;
		count++;

		pin_ptr = pin_ptr->next_ptr;
	}

	return count;
}

/*---------------------------------------------------------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------------------------------------------------------*/

/*===================================================================
	Procedure	:		Get Plane Intersected with Zone Vertices
	Input		:		TRIGGER_ZONE *	Plane to intersect
				:		TRIGGER_ZONE *	Space Ptr
				:		int16_t			Num Sides in Space
				:		int16_t			Side
				:		VECTOR		*	Vertices to output
	Output		:		bool			True/False
===================================================================*/
int GimmeTriggerZoneVertices( TRIGGER_ZONE * plane_ptr, TRIGGER_ZONE * space_ptr, int NumSides, int side, VECTOR * out_ptr )
{
	Pin		* first_free_pin_ptr;
	Pin		* first_pin_ptr;

	Pin		* pin_ptr, * next_pin_ptr, * new_pin_ptr;
	Pin		** prev_pin_ptr_ptr;
	int		count;
	float	back_portion, front_portion;
	int		i;
	float	x,y,z;
	int		SCount;

	first_free_pin_ptr = NULL;
	first_pin_ptr = NULL;

	// Prepare PinBuf;

	for( i = 0; i < MAX_PINS; i++ )
	{
		PinBuf[ i ].next_ptr = first_free_pin_ptr;
		first_free_pin_ptr = &PinBuf[ i ];
	}

	// Create initial poly by clipping with four bounding planes.

	switch( GetMajorAxis( &plane_ptr->normal ) )
	{
		case 0: // major X
			y = -FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->normal.y * y + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->normal.y * y + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->normal.y * y + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y = -FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->normal.y * y + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 1: // major Y
			x = -FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->normal.x * x + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.y;
 			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->normal.x * x + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->normal.x * x + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->normal.x * x + plane_ptr->normal.z * z + plane_ptr->offset ) / plane_ptr->normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 2: // major Z
			x = -FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->normal.x * x + plane_ptr->normal.y * y + plane_ptr->offset ) / plane_ptr->normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

 			x =  FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->normal.x * x + plane_ptr->normal.y * y + plane_ptr->offset ) / plane_ptr->normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->normal.x * x + plane_ptr->normal.y * y + plane_ptr->offset ) / plane_ptr->normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->normal.x * x + plane_ptr->normal.y * y + plane_ptr->offset ) / plane_ptr->normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;
	}

	// Got the initial poly. Do the intersection business.

	for( SCount = 0; SCount < NumSides; SCount++ )
	{
		if( space_ptr != plane_ptr )
		{

			// Calculate distance from each pin to the intersecting plane.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				pin_ptr->dist = side * (	pin_ptr->x * space_ptr->normal.x +
											pin_ptr->y * space_ptr->normal.y +
											pin_ptr->z * space_ptr->normal.z +
														 space_ptr->offset );
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Create intersection points.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
				if( NULL == next_pin_ptr )
				{
					next_pin_ptr = first_pin_ptr;
				}
	
				// We've got a pair of points. See if there is an intersection.
	
				if( ( pin_ptr->dist < 0 && next_pin_ptr->dist >= 0 ) || ( pin_ptr->dist >= 0 && next_pin_ptr->dist < 0 ) )
				{
					// Grab new structure.
	
					if( NULL != first_free_pin_ptr )
					{
						new_pin_ptr = first_free_pin_ptr;
						first_free_pin_ptr = first_free_pin_ptr->next_ptr;
	
						// Set up new structure.
	
						front_portion = pin_ptr->dist / ( pin_ptr->dist - next_pin_ptr->dist );
						back_portion = 1 - front_portion;
	
						new_pin_ptr->x = next_pin_ptr->x * front_portion + pin_ptr->x * back_portion;
						new_pin_ptr->y = next_pin_ptr->y * front_portion + pin_ptr->y * back_portion;
						new_pin_ptr->z = next_pin_ptr->z * front_portion + pin_ptr->z * back_portion;
						new_pin_ptr->dist = 0.0F;
	
						// Link it into the list.
	
						new_pin_ptr->next_ptr = pin_ptr->next_ptr;
						pin_ptr->next_ptr = new_pin_ptr;
	
						// Ensure the new point is skipped.
	
						pin_ptr = new_pin_ptr;
					}
				}
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Remove dead points.
	
			pin_ptr = first_pin_ptr;
			prev_pin_ptr_ptr = &first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
	
				if( pin_ptr->dist < 0 )
				{
					// Remove from list.
	
					*prev_pin_ptr_ptr = pin_ptr->next_ptr;
	
					// Add to free list.
	
					pin_ptr->next_ptr = first_free_pin_ptr;
					first_free_pin_ptr = pin_ptr;
				}
				else
				{
					// Keep in list.
	
					prev_pin_ptr_ptr = &pin_ptr->next_ptr;
				}
	
				pin_ptr = next_pin_ptr;
			}
		}

		space_ptr++;
	}

	count = 0;

	pin_ptr = first_pin_ptr;
	while( NULL != pin_ptr )
	{
		out_ptr->x = pin_ptr->x;
		out_ptr->y = pin_ptr->y;
		out_ptr->z = pin_ptr->z;

		out_ptr++;
		count++;

		pin_ptr = pin_ptr->next_ptr;
	}

	return count;
}

/*===================================================================
	Procedure	:		Get Plane Intersected with Zone Vertices
	Input		:		ZONESIDE	*	Plane to intersect
				:		ZONESIDE	*	Space Ptr
				:		int16_t			Num Sides in Space
				:		int16_t			Side
				:		VECTOR		*	Vertices to output
	Output		:		bool			True/False
===================================================================*/
int GimmeColZoneVertices( ZONESIDE * plane_ptr, ZONESIDE * space_ptr, int NumSides, int side, VECTOR * out_ptr )
{
	Pin		* first_free_pin_ptr;
	Pin		* first_pin_ptr;

	Pin		* pin_ptr, * next_pin_ptr, * new_pin_ptr;
	Pin		** prev_pin_ptr_ptr;
	int		count;
	float	back_portion, front_portion;
	int		i;
	float	x,y,z;
	int		SCount;

	first_free_pin_ptr = NULL;
	first_pin_ptr = NULL;

	// Prepare PinBuf;

	for( i = 0; i < MAX_PINS; i++ )
	{
		PinBuf[ i ].next_ptr = first_free_pin_ptr;
		first_free_pin_ptr = &PinBuf[ i ];
	}

	// Create initial poly by clipping with four bounding planes.

	switch( GetMajorAxis( &plane_ptr->Normal ) )
	{
		case 0: // major X
			y = -FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z = -FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y =  FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			y = -FAR_AWAY;
			z =  FAR_AWAY;
			x = -( plane_ptr->Normal.y * y + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.x;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 1: // major Y
			x = -FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.y;
 			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z = -FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			z =  FAR_AWAY;
			y = -( plane_ptr->Normal.x * x + plane_ptr->Normal.z * z + plane_ptr->PlaneOffset ) / plane_ptr->Normal.y;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;

		case 2: // major Z
			x = -FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->PlaneOffset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

 			x =  FAR_AWAY;
			y = -FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->PlaneOffset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x =  FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->PlaneOffset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			x = -FAR_AWAY;
			y =  FAR_AWAY;
			z = -( plane_ptr->Normal.x * x + plane_ptr->Normal.y * y + plane_ptr->PlaneOffset ) / plane_ptr->Normal.z;
			pin_ptr = first_free_pin_ptr;
			first_free_pin_ptr = first_free_pin_ptr->next_ptr;
			pin_ptr->x = x;
			pin_ptr->y = y;
			pin_ptr->z = z;
			pin_ptr->next_ptr = first_pin_ptr;
			first_pin_ptr = pin_ptr;

			break;
	}

	// Got the initial poly. Do the intersection business.

	for( SCount = 0; SCount < NumSides; SCount++ )
	{
		if( space_ptr != plane_ptr )
		{

			// Calculate distance from each pin to the intersecting plane.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				pin_ptr->dist = side * (	pin_ptr->x * space_ptr->Normal.x +
											pin_ptr->y * space_ptr->Normal.y +
											pin_ptr->z * space_ptr->Normal.z +
														 space_ptr->PlaneOffset );
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Create intersection points.
	
			pin_ptr = first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
				if( NULL == next_pin_ptr )
				{
					next_pin_ptr = first_pin_ptr;
				}
	
				// We've got a pair of points. See if there is an intersection.
	
				if( ( pin_ptr->dist < 0 && next_pin_ptr->dist >= 0 ) || ( pin_ptr->dist >= 0 && next_pin_ptr->dist < 0 ) )
				{
					// Grab new structure.
	
					if( NULL != first_free_pin_ptr )
					{
						new_pin_ptr = first_free_pin_ptr;
						first_free_pin_ptr = first_free_pin_ptr->next_ptr;
	
						// Set up new structure.
	
						front_portion = pin_ptr->dist / ( pin_ptr->dist - next_pin_ptr->dist );
						back_portion = 1 - front_portion;
	
						new_pin_ptr->x = next_pin_ptr->x * front_portion + pin_ptr->x * back_portion;
						new_pin_ptr->y = next_pin_ptr->y * front_portion + pin_ptr->y * back_portion;
						new_pin_ptr->z = next_pin_ptr->z * front_portion + pin_ptr->z * back_portion;
						new_pin_ptr->dist = 0.0F;
	
						// Link it into the list.
	
						new_pin_ptr->next_ptr = pin_ptr->next_ptr;
						pin_ptr->next_ptr = new_pin_ptr;
	
						// Ensure the new point is skipped.
	
						pin_ptr = new_pin_ptr;
					}
				}
	
				pin_ptr = pin_ptr->next_ptr;
			}
	
			// Remove dead points.
	
			pin_ptr = first_pin_ptr;
			prev_pin_ptr_ptr = &first_pin_ptr;
			while( NULL != pin_ptr )
			{
				next_pin_ptr = pin_ptr->next_ptr;
	
				if( pin_ptr->dist < 0 )
				{
					// Remove from list.
	
					*prev_pin_ptr_ptr = pin_ptr->next_ptr;
	
					// Add to free list.
	
					pin_ptr->next_ptr = first_free_pin_ptr;
					first_free_pin_ptr = pin_ptr;
				}
				else
				{
					// Keep in list.
	
					prev_pin_ptr_ptr = &pin_ptr->next_ptr;
				}
	
				pin_ptr = next_pin_ptr;
			}
		}

		// Go to Parent plane.

		space_ptr++;
	}

	count = 0;

	pin_ptr = first_pin_ptr;
	while( NULL != pin_ptr )
	{
		out_ptr->x = pin_ptr->x;
		out_ptr->y = pin_ptr->y;
		out_ptr->z = pin_ptr->z;

		out_ptr++;
		count++;

		pin_ptr = pin_ptr->next_ptr;
	}

	return count;
}

