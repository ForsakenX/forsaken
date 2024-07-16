

/*===================================================================
		Include Files...	
===================================================================*/

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "bsp.h"
#include "file.h"

#include "util.h"


#define BSP_VERSION_NUMBER	(1)
#define BSP_PORTAL_VERSION_NUMBER	(1)

#define BSP_ONLY

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif



/*===================================================================
		Externals ...
===================================================================*/
extern	int	Depth;
extern MLOADHEADER Mloadheader;

extern bool AmIOutsideGroup( MLOADHEADER * m, VECTOR * EndPos, u_int16_t EndGroup );

/*===================================================================
		Globals ...
===================================================================*/
bool FindCollision( BSP_NODE * node_ptr, VECTOR * start_point_ptr, VECTOR * end_point_ptr );

BSP_HEADER Bsp_Header[ 2 ];
float ColRad;
																   
BSP_NODE *	BSP_Nodes[256];
BSP_PORTAL_HEADER Bsp_Portal_Header = { false, 0, {} };


bool Bsp_Duplicate( BSP_HEADER *src, BSP_HEADER *dup )
{
	int j;
	BSP_TREE *srctree, *duptree;

	dup->State = src->State;
	dup->NumGroups = src->NumGroups;
	for ( j = 0; j < src->NumGroups; j++ )
	{
		srctree = &src->Bsp_Tree[ j ];
		duptree = &dup->Bsp_Tree[ j ];
		duptree->NumNodes = srctree->NumNodes;
		duptree->Root = (BSP_NODE *) calloc( srctree->NumNodes, sizeof( BSP_NODE ) );
		if ( !duptree->Root )
			return false;
		memmove( duptree->Root, srctree->Root, srctree->NumNodes * sizeof( BSP_NODE ) );//memcpy
	}
	return true;
}


bool Bsp_Identical( BSP_HEADER *b1, BSP_HEADER *b2 )
{
	int j;
	BSP_TREE *b1tree, *b2tree;

	if ( b1->State != b2->State )
		return false;
	if ( b1->NumGroups != b2->NumGroups )
		return false;
	for ( j = 0; j < b1->NumGroups; j++ )
	{
		b1tree = &b1->Bsp_Tree[ j ];
		b2tree = &b2->Bsp_Tree[ j ];
		if ( b1tree->NumNodes != b2tree->NumNodes )
			return false;
		if ( memcmp( b1tree->Root, b2tree->Root, b1tree->NumNodes * sizeof( BSP_NODE ) ) )
			return false;
	}
	return true;
}


static bool BSP_Loadtree( BSP_TREE *t, char **Buffer )
{
	BSP_RAWNODE * Raw;
	BSP_NODE * New;
	int16_t		*	int16_tpnt;
	int16_t		e;

	// get the number of nodes and move to next pointer
	int16_tpnt = ( int16_t * ) *Buffer;
	t->NumNodes = *int16_tpnt++;
	*Buffer = (char * ) int16_tpnt;
	
	// initialize the root node and set aside enough space
	// set aside size of N BSP_NODE's
	t->Root = (BSP_NODE * ) calloc( t->NumNodes , sizeof( BSP_NODE ) );
	if ( !t->Root )
		return false;
	
	// cast buffer to BSP_RAWNODE for extracting values
	Raw = (BSP_RAWNODE *) *Buffer;

	// initialize first node
	New = t->Root;
	New->Parent = NULL;
	
	// pack on node heirachy
	for( e = 0 ; e < t->NumNodes ; e++ )
	{
		New->Normal = Raw->Normal;
		New->Offset = Raw->Offset;
		New->Colour = Raw->Colour;
		if( !Raw->Front ) New->Front = NULL;
		else{
			New->Front = t->Root + Raw->Front;
			New->Front->Parent = New;
		}
		if( !Raw->Back ) New->Back = NULL;
		else{
			New->Back  = t->Root + Raw->Back;
			New->Back->Parent = New;
		}
		New++;
		Raw++;
	}

	// rest of data is garbage
	*Buffer = (char*) Raw;

	return true;
}


static bool BSP_LoadPortal( BSP_PORTAL *p, char **Buffer )
{
	int16_t	*	int16_tpnt;
	float	*	floatpnt;

	int16_tpnt = (int16_t *) *Buffer;
	p->group = *int16_tpnt++;
	floatpnt = (float *) int16_tpnt;
	p->normal.x = *floatpnt++;
	p->normal.y = *floatpnt++;
	p->normal.z = *floatpnt++;
	p->offset = *floatpnt++;
	*Buffer = (char *) floatpnt;

	return BSP_Loadtree( &p->bsp, Buffer );
}


static bool BSP_LoadPortalGroup( BSP_PORTAL_GROUP *pg, char **Buffer )
{
	int16_t		*	int16_tpnt;
	int				i;

	int16_tpnt = (int16_t *) *Buffer;
	pg->portals = *int16_tpnt++;
	*Buffer = (char *) int16_tpnt;

	pg->portal = (BSP_PORTAL *) calloc( pg->portals, sizeof( BSP_PORTAL ) );
	for ( i = 0; i < pg->portals; i++ )
	{
		if ( !BSP_LoadPortal( &pg->portal[ i ], Buffer ) )
		{
			return false;
		}
	}

	return true;
}


static bool BSP_LoadPortals( char *fname )
{
	char	Filename[ 256 ];
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int16_t		*	int16_tpnt;
	int16_t			i;
	u_int32_t		*	Uint32Pnt;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;

	Bsp_Portal_Header.state = false;

	Change_Ext( fname, Filename, ".PBS" );

	File_Size = Get_File_Size( Filename );	
	if( !File_Size )
	{
		Msg( "Bsp_Portalload() no PBS file %s", Filename );
		return false;
	}

	Buffer = malloc( File_Size );
	OrgBuffer = Buffer;

	if( Buffer == NULL ) return false;

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size ) return false;

	Uint32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != BSP_PORTAL_VERSION_NUMBER  ) )
	{
		Msg( "Bsp_Portalload() Incompatible PBS file %s", Filename );
		return( false );
	}
	int16_tpnt = ( int16_t * ) Buffer;
	Bsp_Portal_Header.groups = *int16_tpnt++;
	Buffer = (char * ) int16_tpnt;

	for ( i = 0; i < Bsp_Portal_Header.groups; i++ )
	{
		if ( !BSP_LoadPortalGroup( &Bsp_Portal_Header.group[ i ], &Buffer ) )
		{
			return false;
		}
	}

	free( OrgBuffer );

	Bsp_Portal_Header.state = true;

	return true;
}

/*===================================================================
	Procedure	:		Load .Bsp File
	Input		:		char	*	Filename
	Output		:		bool	true/false
===================================================================*/
bool Bspload( char * Filename, BSP_HEADER *Bsp_Header )
{
#ifdef BSP
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int16_t		*	int16_tpnt;
	int16_t			i;
	u_int32_t		*	Uint32Pnt;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;

	Bsp_Header->State = false;

	File_Size = Get_File_Size( Filename );	
	if( !File_Size )
	{
		Msg( "Bspload() no BSP file %s", Filename );
		return false;
	}

	Buffer = malloc( File_Size );

	if( Buffer == NULL ) return false;

	OrgBuffer = Buffer;

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size ) return false;

	Uint32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != BSP_VERSION_NUMBER  ) )
	{
		Msg( "Bspload() Incompatible BSP file %s", Filename );
		return( false );
	}
	int16_tpnt = ( int16_t * ) Buffer;
	Bsp_Header->NumGroups = *int16_tpnt++;
	Buffer = (char * ) int16_tpnt;

	for( i = 0 ; i < Bsp_Header->NumGroups ; i++ )
	{
		if ( !BSP_Loadtree( &Bsp_Header->Bsp_Tree[ i ], &Buffer ) )
		{
			return false;
		}
	}
	free( OrgBuffer );

#endif
	Bsp_Header->State = true;

#ifdef BSP_ONLY
	if ( !BSP_LoadPortals( Filename ) )
		return false;
#else
	BSP_LoadPortals( Filename );
#endif

	return true;
}
/*===================================================================
	Procedure	:		Free up the memory calloced by bspload..
	Input		:		Nothing
	Output		:		bool	true/false
===================================================================*/
void Bspfree( void )
{
#ifdef BSP
	int			i;
	int			bsp_num;

	for ( bsp_num = 0; bsp_num < MAX_BSP_MODELS; bsp_num++ )
	{
		if ( Bsp_Header[ bsp_num ].State )
		{
			for( i = 0 ; i < Bsp_Header[ bsp_num ].NumGroups ; i++ )
			{
				if( Bsp_Header[ bsp_num ].Bsp_Tree[i].Root )
				{
					free(Bsp_Header[ bsp_num ].Bsp_Tree[i].Root);
					Bsp_Header[ bsp_num ].Bsp_Tree[i].Root = NULL;
				}
			}
			Bsp_Header[ bsp_num ].NumGroups = 0;
			Bsp_Header[ bsp_num ].State = false;
		}
	}

	if ( Bsp_Portal_Header.state )
	{
		int					j;
		BSP_PORTAL_GROUP	*pg;
		BSP_PORTAL			*bp;

		for ( i = 0; i < Bsp_Portal_Header.groups; i++ )
		{
			pg = &Bsp_Portal_Header.group[ i ];
			for ( j = 0; j < pg->portals; j++ )
			{
				bp = &pg->portal[ j ];
				if ( bp->bsp.Root )
					free( bp->bsp.Root );
				bp->bsp.Root = NULL;
			}
			if ( pg->portal )
				free( pg->portal );
			pg->portal = NULL;
			pg->portals = 0;
		}
		Bsp_Portal_Header.state = false;
	}
#endif
}


#define POINT_TO_PLANE( P, N ) ( (P)->x * (N)->Normal.x + (P)->y * (N)->Normal.y + (P)->z * (N)->Normal.z + ( (N)->Offset) )

static VECTOR RayPos;
static VECTOR RayDir;
static float RayLength;
float	Trick;
bool Collided;
float CollideDist;
VECTOR CollidePoint;
BSP_NODE *CollideNode;
BSP_NODE *OldCollideNode;
VECTOR OldCollidePoint;
float CollisionRadius = 0.0F;

bool RayCollide( BSP_HEADER *Bsp_Header, VECTOR *StartPos, VECTOR *Dir, VECTOR *ImpactPoint, VECTOR *ImpactNormal, float *ImpactOffset , u_int16_t group)
{
	bool collided;
	VECTOR EndPos;

 	RayLength = VectorLength( Dir );
	CollideNode = NULL;
	RayPos = *StartPos;
	RayDir = *Dir;
	EndPos.x = StartPos->x + Dir->x;
	EndPos.y = StartPos->y + Dir->y;
	EndPos.z = StartPos->z + Dir->z;
	Depth = 0;

	collided = FindCollision( Bsp_Header->Bsp_Tree[ group % Bsp_Header->NumGroups ].Root, StartPos, &EndPos );
	
	if ( collided && CollideNode )
	{
		OldCollideNode = CollideNode;
		OldCollidePoint = CollidePoint;
		*ImpactPoint = CollidePoint;
   		*ImpactNormal = CollideNode->Normal;
   		*ImpactOffset = CollideNode->Offset - CollisionRadius;

#if 0	// Shouldnt need this if all opposite co-planear faces are not used as splitting planes...
		Trick = ( ImpactNormal->x * Dir->x + ImpactNormal->y * Dir->y + ImpactNormal->z * Dir->z ) / RayLength;
		if ( Trick > 0.001F )
   		{
   			ImpactNormal->x = -ImpactNormal->x;
   			ImpactNormal->y = -ImpactNormal->y;
   			ImpactNormal->z = -ImpactNormal->z;
   			*ImpactOffset = -*ImpactOffset;
   		}
#endif
   		return true;
	}
	else
	{
		return false;
	}
}


/*===================================================================
	Procedure	:		Define if a ray hits a solid..
	Input		:		BSP_NODE * Start node
						VECTOR * Start Position
						VECTOR * end Position
	Output		:		bool
===================================================================*/
bool FindCollision( BSP_NODE * node_ptr, VECTOR * start_point_ptr, VECTOR * end_point_ptr )
{
	float		d1, d2;
	VECTOR		intersection_point;
	BSP_NODE	* near_node_ptr;
	BSP_NODE	* far_node_ptr;
	float div;
	float distance2plane;
	bool	side;

	Depth++;

start:

	// BUG: some reason bully got a null pointer here on either start_point_ptr or node_ptr
	// was a recursive call from the very last line of this function

/*
	Thread 0 (crashed)
 0  ProjectX_1.13.1000.exe!FindCollision [bsp.c : 449 + 0x8]
    eip = 0x0041d5db   esp = 0x0012f704   ebp = 0x0012f74c   ebx = 0x00000001
    esi = 0x00000002   edi = 0x00000a28   eax = 0x00120124   ecx = 0x0012f79c
    edx = 0x2dbd85a5   efl = 0x00210206
 1  ProjectX_1.13.1000.exe!FindCollision [bsp.c : 567 + 0x10]
    eip = 0x0041da5c   esp = 0x0012f754   ebp = 0x0012f7a8
 2  ProjectX_1.13.1000.exe!RayCollide [bsp.c : 401 + 0x1f]
    eip = 0x0041d50f   esp = 0x0012f7b0   ebp = 0x0012f7cc
 3  ProjectX_1.13.1000.exe!OneGroupPolyCol [collision.c : 1408 + 0x24]
    eip = 0x0041e9c8   esp = 0x0012f7d4   ebp = 0x0012f86c
 4  ProjectX_1.13.1000.exe!BackgroundCollide [collision.c : 697 + 0x30]
    eip = 0x0041e213   esp = 0x0012f874   ebp = 0x0012f950
 5  ProjectX_1.13.1000.exe!ObjectCollide [collision.c : 3204 + 0x50]
    eip = 0x00420896   esp = 0x0012f958   ebp = 0x0012fae4
 6  ProjectX_1.13.1000.exe!ProcessShips [ships.c : 1078 + 0x1a]
    eip = 0x004dc5c8   esp = 0x0012faec   ebp = 0x0012fcc4
 7  ProjectX_1.13.1000.exe!MainRoutines [oct2.c : 5397 + 0x4]
    eip = 0x0047a326   esp = 0x0012fccc   ebp = 0x0012fccc
 8  ProjectX_1.13.1000.exe!MainGame [oct2.c : 5596 + 0x4]
    eip = 0x0047a7c7   esp = 0x0012fcd4   ebp = 0x0012fd40
 9  ProjectX_1.13.1000.exe!RenderScene [oct2.c : 3956 + 0xc]
    eip = 0x004778be   esp = 0x0012fd48   ebp = 0x0012fef0
10  ProjectX_1.13.1000.exe!RenderLoop [d3dmain.cpp : 1207 + 0x17]
    eip = 0x00431a86   esp = 0x0012fef8   ebp = 0x0012ff00
11  ProjectX_1.13.1000.exe!WinMain [d3dmain.cpp : 232 + 0x4]
    eip = 0x0043077a   esp = 0x0012ff08   ebp = 0x0012ff28
12  ProjectX_1.13.1000.exe!__tmainCRTStartup [crt0.c : 324 + 0x1b]
    eip = 0x005417e5   esp = 0x0012ff30   ebp = 0x0012ffc0
13  kernel32.dll + 0x16fe6
    eip = 0x7c816fe7   esp = 0x0012ffc8   ebp = 0x0012fff0

*/

	if( !start_point_ptr || !node_ptr || !end_point_ptr ) return false;

	d1 = POINT_TO_PLANE( start_point_ptr, node_ptr ) - CollisionRadius;
	d2 = POINT_TO_PLANE(   end_point_ptr, node_ptr ) - CollisionRadius;

    if( d1 < TOLER && d1 > -TOLER ) d1 = 0.0F;
    if( d2 < TOLER && d2 > -TOLER )
	{
		if( d1 == 0 )
		{
			// d1 + d2 = 0 Parallel ray....
			BSP_NODE *Back_CollideNode;
			VECTOR Back_CollidePoint;

			if ( node_ptr->Back && node_ptr->Front )
			{
				if ( FindCollision( node_ptr->Back, start_point_ptr, end_point_ptr ) )
				{
					Back_CollideNode = CollideNode;
					Back_CollidePoint = CollidePoint;
					if ( FindCollision( node_ptr->Front, start_point_ptr, end_point_ptr ) )
					{
						VECTOR dv;

						dv.x = Back_CollidePoint.x - start_point_ptr->x;
						dv.y = Back_CollidePoint.y - start_point_ptr->y;
						dv.z = Back_CollidePoint.z - start_point_ptr->z;
						d1 = VectorLength( &dv );
						dv.x = CollidePoint.x - start_point_ptr->x;
						dv.y = CollidePoint.y - start_point_ptr->y;
						dv.z = CollidePoint.z - start_point_ptr->z;
						d2 = VectorLength( &dv );
						if ( d1 < d2 )
						{
							CollideNode = Back_CollideNode;
							CollidePoint = Back_CollidePoint;
						}
					}
					else
					{
						CollideNode = Back_CollideNode;
						CollidePoint = Back_CollidePoint;
					}
					return true;
				}
				else
				{
					return FindCollision( node_ptr->Front, start_point_ptr, end_point_ptr );
				}
			}
			else if ( node_ptr->Back )
			{
				return FindCollision( node_ptr->Back, start_point_ptr, end_point_ptr );
			}
			else if ( node_ptr->Front )
			{
				return FindCollision( node_ptr->Front, start_point_ptr, end_point_ptr );
			}
			else // oh shit...what do we do now???
			{
				return false;
			}

			return true;
		}
		d2 = 0.0F;
	}

	if( (d1 < -CollisionRadius) && (d2 < -CollisionRadius) )
	{
		if( node_ptr = node_ptr->Back )
		{
			goto start;
		}
		// Entire segment inside a solid.
		return true;
	}
	if( (d1 >= CollisionRadius) && (d2 >= CollisionRadius) )
	{
		if( node_ptr = node_ptr->Front )
		{
			goto start;
		}
		return false;
	}
   	// We intersect the Plane...


	div = ( RayDir.x * node_ptr->Normal.x) + 
   		  ( RayDir.y * node_ptr->Normal.y) + 
   		  ( RayDir.z * node_ptr->Normal.z);
	distance2plane = POINT_TO_PLANE( &RayPos , node_ptr ) - CollisionRadius;

   	distance2plane = distance2plane / div;
   	intersection_point.x = RayPos.x - ( RayDir.x * distance2plane );
   	intersection_point.y = RayPos.y - ( RayDir.y * distance2plane );
   	intersection_point.z = RayPos.z - ( RayDir.z * distance2plane );
   	
	if( side = d1 < 0 )
   	{
   		near_node_ptr = node_ptr->Back;
   		far_node_ptr = node_ptr->Front;
   	}else{
   		near_node_ptr = node_ptr->Front;
   		far_node_ptr = node_ptr->Back;
   	}

   	if( ( !near_node_ptr && ( side ) ) ||
   		( near_node_ptr && FindCollision( near_node_ptr, start_point_ptr, &intersection_point ) ) )
   	{
   		return true;
   	}
	CollideNode = node_ptr;
	CollidePoint = intersection_point;
	if ( !far_node_ptr )
	{
		if ( side )
			return false;
		return true;
	}
	return FindCollision( far_node_ptr, &intersection_point, end_point_ptr );
}





/*===================================================================
	Procedure	:		Define if a point is inside or outside
	Input		:		VECTOR * Pos , Node *node
	Output		:		bool
===================================================================*/

bool PISDistRecursive( VECTOR *Pos, BSP_NODE *node)
{
	float d;

start:
	// go down the BSP tree
	d = node->Normal.x * Pos->x + node->Normal.y * Pos->y + node->Normal.z * Pos->z + node->Offset;
	if ( d > TOLER  )
	{ // definitely in front of plane
	 	if ( node = node->Front )
	 	{
	 		goto start;
	 	}
	 	else
	 	{
	 		return true;
	 	}
	}
	if ( d < -TOLER )
	{ // definitely behind plane
		if ( node = node->Back )
		{
			goto start;
		}
		OldCollideNode = node;
		return false;
	}
	// somewhere in between plane +/- TOLER (tricky case)
	if ( !node->Front || PISDistRecursive( Pos, node->Front ) )
		return true;
	if ( node->Back && PISDistRecursive( Pos, node->Back ) )
		return true;
	return false;
}



bool PointInsideSkin( VECTOR *Pos, u_int16_t Group )
{
	if( Bsp_Header[ 0 ].State )
	{
		return PISDistRecursive( Pos, Bsp_Header[0].Bsp_Tree[ Group % Bsp_Header[0].NumGroups ].Root );
	}
	return !AmIOutsideGroup( &Mloadheader, Pos, Group );
 }




#ifdef OPT_ON
#pragma optimize( "", off )
#endif
