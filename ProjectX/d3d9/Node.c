
/*
.NOD file
	int32 Num of nodes...
	{NODE
		int16 Group
		VECTOR Pos
		float Radius
		uint32 NetMask...Which networks im in...
		int16 numoflinks..
		{LINK
			int Link num inside this network....
		}
	}
 */

/*===================================================================
		Include File...	
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "Node.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "file.h"
#include "triggers.h"
#include "models.h"
#include "enemies.h"
#include "mload.h"
#include <d3d9.h>
#include "d3dappi.h"
#include "AI.h"
#include "lines.h"

#include "collision.h"
#include "XMem.h"

/*===================================================================
		Defines
===================================================================*/
#define	MAXNODES	768
#define	NOD_VERSION_NUMBER	1

/*===================================================================
		Externals...	
===================================================================*/
extern	LINE			Lines[ MAXLINES ];
extern	ENEMY	*	FirstEnemyUsed;

/*===================================================================
		Globals...	
===================================================================*/

NODENETWORKHEADER	NodeNetworkHeader;
void SetNetworkDistance( void );
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;


float	* NodeArray = NULL;

NODE *	NodeInGroup[MAXGROUPS];


#define MAX_EXCEPTIONS	(20)
#define MAX_EXCEPTION_TYPES	(6)


int acos_exceptions = 0;
int sqrt_exceptions = 0;
int pow_exceptions = 0;
int unhandled_exceptions = 0;
int total_exceptions = 0;
int type_exceptions[ MAX_EXCEPTION_TYPES ];
int unknown_exceptions = 0;
char * MathErrStrings[] = { 
	"Domain",
	"Singularity",
	"Overflow",
	"Underflow",
	"Total Loss of significance",
	"Partial Loss of significance",
};

#ifdef DEBUG_ON
int     __cdecl _matherr(struct _exception * except)
//int _matherr( struct _exception *except )
{
	static int ignore = 0;

	total_exceptions++;
	if ( except->type > 0 && except->type <= MAX_EXCEPTION_TYPES )
		type_exceptions[ except->type - 1 ]++;
	else
		unknown_exceptions++;
	switch( except->type )
	{
		case _DOMAIN:
			if( !_stricmp( except->name, "acos" ) )
			{
				if( ( except->arg1 < -1.0 ) && ( except->arg1 > -1.0001 ) )
				{
					except->retval = PI;
					acos_exceptions++;
					return( 1 );
				}

				if( ( except->arg1 > 1.0 ) && ( except->arg1 < 1.0001 ) )
				{
					except->retval = 0;
					acos_exceptions++;
					return( 1 );
				}
			}
			else if ( !_stricmp( except->name, "sqrt" ) )
			{
				if ( except->arg1 < 0.0 )
				{
					sqrt_exceptions++;
					return 0; // not handled, needs fixing...
				}
			}
	  		break;

		case _SING:
		case _OVERFLOW:
		case _PLOSS:
		case _TLOSS:
			break;
		case _UNDERFLOW:
			if ( !_stricmp( except->name, "pow" ) )
			{
				if ( except->arg1 < 1.0 && except->arg2 > 0.0 )
				{
					pow_exceptions++;
					return 0; // not handled, but don't care
				}
			}
			break;

		default:
			break;
	}

	unhandled_exceptions++;
	DebugPrintf( "_matherr(#%d) %s( %lf, %lf ) = %lf %s\n",
		unhandled_exceptions,
		except->name, except->arg1, except->arg2,
		except->retval, MathErrStrings[ except->type - 1 ] );
	if ( !ignore )
	{
		if ( Msg( "MathErr() %s( %lf, %lf ) = %lf %s\n", 
					except->name, except->arg1, except->arg2, except->retval, 
					MathErrStrings[ except->type - 1 ] ) )
			ignore = 1;
	}

	return 0;
}
#endif

int DebugMathErrors( void )
{
	int j;

	if ( !total_exceptions )
		return 0;
	DebugPrintf( "Math Exception Summary\n" );
	DebugPrintf( "======================\n" );
	for ( j = 0; j < MAX_EXCEPTION_TYPES; j++ )
	{
		if ( type_exceptions[ j ] )
		{
			DebugPrintf( "%d %s\n",
				type_exceptions[ j ], MathErrStrings[ j ] );
		}
	}
	DebugPrintf( "===\n" );
	DebugPrintf( "%d total (%d unknown)\n",
		total_exceptions, unknown_exceptions );
	DebugPrintf( "(%d acos, %d sqrt, %d pow, %d unhandled)\n",
		acos_exceptions, sqrt_exceptions, pow_exceptions, unhandled_exceptions );
	return total_exceptions;
}



/*===================================================================
	Procedure	:		Load .Nod File
	Input		:		char	*	Filename 
	Output		:		Nothing
===================================================================*/
BOOL Nodeload( char * Filename )
{
	char		*	Buffer;
	char		*	OrgBuffer;
	int32		*	int32Pnt;
	uint32		*	uint32Pnt;
	int16		*	int16Pnt;
	float		*	FloatPnt;
	int			e,o,i;
	long			File_Size;
	long			Read_Size;
	NODE		*	NodePnt;
	uint32		MagicNumber;
	uint32		VersionNumber;
   	uint16		TempGroup;
	NORMAL		TempNormal;
	VECTOR		TempPos_New;
	VECTOR		Move_Off = { 0.0F , -MaxColDistance , 0.0F };
	BOOL		LegalGroup;


	for( i = 0 ; i < MAXGROUPS ; i ++ )
	{
		NodeInGroup[i] = NULL;
	}
	
	NodeNetworkHeader.State = FALSE;


	File_Size = Get_File_Size( Filename );	

	if( !File_Size )
	{
		// Doesnt Matter.....
		return TRUE;
	}
	Buffer = calloc( 1, File_Size );
	OrgBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Nodeload failed to Allocate buffer for %s failed\n", Filename );
		return FALSE;
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
		Msg( "Nodeload Error reading file %s\n", Filename );
		return FALSE;
	}

	uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *uint32Pnt++;
	VersionNumber = *uint32Pnt++;
	Buffer = (char *) uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != NOD_VERSION_NUMBER  ) )
	{
		Msg( "NodeLoad() Incompatible node( .NOD ) file %s", Filename );
		return( FALSE );
	}

	int32Pnt = (int32*) Buffer;
	NodeNetworkHeader.NumOfNodes = *int32Pnt++;

	if( NodeNetworkHeader.NumOfNodes >= MAXNODES )
	{
		Msg( "You have to many Nodes!!! %d is far to many... \n", NodeNetworkHeader.NumOfNodes);
		return FALSE;
	}

	NodeArray = (float*) malloc( NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof(float) );

	if(!NodeArray)
	{
		Msg( "Nodeload failed NodeArray malloc in %s \n", Filename );
		return FALSE;

	}
	
	NodeNetworkHeader.FirstNode = (NODE*) calloc( NodeNetworkHeader.NumOfNodes, sizeof(NODE) );
	
	
	NodePnt = NodeNetworkHeader.FirstNode;
	if( !NodePnt )
	{
		Msg( "Nodeload failed allocate Node Pointer in %s \n", Filename );
		return FALSE;
	}
	Buffer = (char*) int32Pnt;
	
	for( e = 0 ; e < NodeNetworkHeader.NumOfNodes ; e++ )
	{
		int16Pnt = (int16*) Buffer;
		NodePnt->Group = *int16Pnt++;
		FloatPnt = (float*) int16Pnt;
		
		NodePnt->NodeNum = e;
		
		NodePnt->Pos.x = *FloatPnt++;
		NodePnt->Pos.y = *FloatPnt++;
		NodePnt->Pos.z = *FloatPnt++;
		NodePnt->Radius = *FloatPnt++;
		
		uint32Pnt = (uint32*) FloatPnt;
		NodePnt->NetMask = *uint32Pnt++;				// which networks this node is in...

//		if( !(NodePnt->NetMask&1) )
//		{
//			Msg( "Nodeload failed Node %d is not in Network 1\n", e );
//		}
		
		int16Pnt = (int16*) uint32Pnt;
		NodePnt->Flags = (uint16) *int16Pnt++;
		NodePnt->NumOfLinks = *int16Pnt++;
		
		if( NodePnt->NumOfLinks > MAXLINKSPERNODE )
		{
			Msg( "NoadLoad failed too many Nodelinks in network , Node %d \n", e );
			return FALSE;
		}
		if( !NodePnt->NumOfLinks )
		{
			DebugPrintf("No Links for Node %d\n" , e );
		}
		int32Pnt = (int32*) int16Pnt;
		for( o = 0 ; o < NodePnt->NumOfLinks ; o++ )
		{
			
			NodePnt->NodeLink[o] = NodeNetworkHeader.FirstNode + *int32Pnt++;
		}

		NodePnt->SolidPos = NodePnt->Pos;
		// we need a Point on the background that is below the Node...
		
		LegalGroup = FALSE;
		
		if( !PointInsideSkin( &NodePnt->Pos, NodePnt->Group ) )
		{
			Msg( "NoadLoad failed Node %d is in WRONG Group (%s)\n",
				e, Mloadheader.Group[ NodePnt->Group ].name );
			for( i = 0 ; i < Mloadheader.num_groups ; i++ )
			{
				if( PointInsideSkin( &NodePnt->Pos, (uint16) i ) )
				{
					NodePnt->Group = i;
					Msg( "Node %d Should be in %s, FIX IT NOW!\n",
						e, Mloadheader.Group[ i ].name );
					LegalGroup = TRUE;
					break;
				}
			}
		}else{
			LegalGroup = TRUE;
		}

		if( LegalGroup )
		{
			if(	BackgroundCollide( &MCloadheadert0, &Mloadheader, &NodePnt->Pos, NodePnt->Group, &Move_Off,
				&NodePnt->SolidPos , &TempGroup, &TempNormal, &TempPos_New, FALSE, NULL ) )
			{
#ifdef DEBUG
				VECTOR move;
				float move_len;

				move.x = NodePnt->SolidPos.x - NodePnt->Pos.x;
				move.y = NodePnt->SolidPos.y - NodePnt->Pos.y;
				move.z = NodePnt->SolidPos.z - NodePnt->Pos.z;
				move_len = VectorLength( &move );
				if ( move_len > VectorLength( &Move_Off ) )
					Msg( "NodeLoad: move_len = %f , Node %d\n", move_len, e);
				if ( TempGroup != NodePnt->Group )
					Msg( "NodeLoad: ground group =%hd != node group = %hd, Node %d\n", TempGroup, NodePnt->Group, e );
#endif
				NodePnt->SolidPos.y += 75.0F;
			}else{
				Msg( "NodeLoad failed Couldnt find a Solid collide , Node %d in %s\n",
					e, Mloadheader.Group[ NodePnt->Group ].name );
				//			return FALSE;
			}
		}else{
			Msg( "Node %d Is not in any Legal Group..This is Very Bad...FIX IT!\n", e );
		}
		
		NodePnt->LegalGroup = LegalGroup;
		
		NodePnt->NextNodeInGroup = NodeInGroup[NodePnt->Group];
		NodeInGroup[NodePnt->Group] = NodePnt;
		
		NodePnt++;
		Buffer = (char*) int32Pnt;
	}
#ifdef LOAD_PRECALC_NODE_DISTANCE_TABLE
#ifndef CHECK_NODE_DISTANCES
	// copy pre-calculated node distances over
	memmove( NodeArray, Buffer, NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof( float ) );
#else
	{
		float * NodeArray2 = NULL;
		int j, errs = 0;

		NodeArray2 = (float*) malloc( NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof(float) );
		if(!NodeArray2)
		{
			Msg( "Nodeload failed NodeArray2 malloc in %s \n", Filename );
			return FALSE;
		}
		// copy pre-calculated node distances over
		memmove( NodeArray2, Buffer, NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof( float ) );

		NodeNetworkHeader.State = TRUE;
		SetNetworkDistance();
		for ( j = 0; j < NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes; j++ )
		{
			if ( fabs( NodeArray[ j ] - NodeArray2[ j ] ) > 0.1F )
			{
				errs++;
			}
		}
		if ( errs )
		{
			Msg( "%d node array errors found in %d nodes\n",
				errs, NodeNetworkHeader.NumOfNodes );
		}
		free( NodeArray2 );
	}
#endif
#endif

	// All Node Networks have been loaded...
	free(OrgBuffer);
	
	NodeNetworkHeader.State = TRUE;

#ifndef LOAD_PRECALC_NODE_DISTANCE_TABLE
	SetNetworkDistance();
#endif
	
	return TRUE;
}



/*===================================================================
	Procedure	:		Release NodeHeader..
	Input		:		NOTHING
	Output		:		Nothing
===================================================================*/
void NodeRelease( void)
{
	if( NodeNetworkHeader.State )
	{
		free( NodeNetworkHeader.FirstNode );
		NodeNetworkHeader.FirstNode = NULL;
		NodeNetworkHeader.State = FALSE;
	}
	if( NodeArray )
	{
		free( NodeArray );
		NodeArray = NULL;
	}
}

/*===================================================================
	Procedure	:		Put Enmies At Node....And draw lines for a Network
	Input		:		NOTHING
	Output		:		Nothing
===================================================================*/
ENEMY * PutEnemiesAtNodes(void)
{
	int o,i,e;
	NODE * NodePnt;
	NODE * NodeLink;
	VECTOR Dir = { 0.0F , 0.0F , 1.0F };
	VECTOR Up = { 0.0F , 1.0F , 0.0F };
	uint32 NetMask = 1;
	uint16	line;
	BOOL	Mutual;
	int	NumOfEnemies = 0;
	ENEMY * Enemy;

	Enemy = FirstEnemyUsed;

	return NULL;

	if( !NodeNetworkHeader.State )
		return NULL;

	if( !NodeNetworkHeader.State )
		return NULL;


	NodePnt = NodeNetworkHeader.FirstNode;

	for( o = 0 ; o < NodeNetworkHeader.NumOfNodes ; o++ )
	{

//		InitOneEnemy( ENEMY_GENTYPE_Initialised, &NodePnt->Pos, &Dir, &Up, NodePnt->Group, MODEL_Swarm, -1 ,0 ,0);
		if( NumOfEnemies )
		{
			NumOfEnemies--;
			InitOneEnemy( ENEMY_GENTYPE_Initialised, &NodePnt->SolidPos, &Dir, &Up, NodePnt->Group, Enemy->ModelNum, -1 ,ENEMY_Swarm ,0,-1,(uint16) -1 , 0.0F);
		}

//		if( (NodePnt->NetMask&NetMask) && !NodePnt->LegalGroup)
		if( (NodePnt->NetMask&NetMask) )
		{
			for( i = 0 ; i < NodePnt->NumOfLinks ; i++ )
			{
				NodeLink = NodePnt->NodeLink[i];
				if( NodeLink->NetMask & NetMask )
				{
					Mutual = FALSE;

					for( e = 0 ; e < NodeLink->NumOfLinks ; e ++ )
					{
						if( NodePnt == NodeLink->NodeLink[e] )
							Mutual = TRUE;
					}
					

					line = FindFreeLine();						// Line attached
					if( line != (uint16 ) -1 )
					{
						Lines[ line ].StartPos = NodePnt->Pos;
						Lines[ line ].EndPos = NodeLink->Pos;
						if( Mutual )
						{

							Lines[ line ].StartCol.R = 255;
							Lines[ line ].StartCol.G = 255;
							Lines[ line ].StartCol.B = 255;
							Lines[ line ].EndCol.R = 255;
							Lines[ line ].EndCol.G = 64;
							Lines[ line ].EndCol.B = 64;
						}else{
							Lines[ line ].StartCol.R = 0;
							Lines[ line ].StartCol.G = 0;
							Lines[ line ].StartCol.B = 255;
							Lines[ line ].EndCol.R = 0;
							Lines[ line ].EndCol.G = 0;
							Lines[ line ].EndCol.B = 64;
						}
						Lines[ line ].StartTrans = 255;
						Lines[ line ].EndTrans = 255;
						Lines[ line ].Group = NodePnt->Group;
					}



				}
			}
		}
		NodePnt++;

	}

	NodePnt = NodeNetworkHeader.FirstNode;

	return NULL;
}




/*===================================================================
	Procedure	:		draw lines for a Network
	Input		:		NOTHING
	Output		:		Nothing
===================================================================*/
void ShowNodeNetwork( uint32 NetMask )
{
	int o,i,e;
	NODE * NodePnt;
	NODE * NodeLink;
	VECTOR Dir = { 0.0F , 0.0F , 1.0F };
	VECTOR Up = { 0.0F , 1.0F , 0.0F };
	uint16	line;
	BOOL	Mutual;
	BOOL	CanSee;

	InitLines(); // kill all lines (hack hack hack)

	if( !NodeNetworkHeader.State )
		return;

	NodePnt = NodeNetworkHeader.FirstNode;

	for( o = 0 ; o < NodeNetworkHeader.NumOfNodes ; o++ )
	{

		if( (NodePnt->NetMask&NetMask) )
		{
			for( i = 0 ; i < NodePnt->NumOfLinks ; i++ )
			{
				NodeLink = NodePnt->NodeLink[i];
				if( NodeLink->NetMask & NetMask )
				{
					Mutual = FALSE;

					for( e = 0 ; e < NodeLink->NumOfLinks ; e ++ )
					{
						if( NodePnt == NodeLink->NodeLink[e] )
							Mutual = TRUE;
					}
					
					CanSee = AI_ClearLOSNonZeroNonObject( &NodePnt->Pos, NodePnt->Group, &NodeLink->Pos, SHIP_RADIUS );
					
					line = FindFreeLine();						// Line attached
					if( line != (uint16 ) -1 )
					{
						Lines[ line ].StartPos = NodePnt->Pos;
						Lines[ line ].EndPos = NodeLink->Pos;
						if( !CanSee )
						{
							Lines[ line ].StartCol.R = 0;
							Lines[ line ].StartCol.G = 255;
							Lines[ line ].StartCol.B = 0;
							Lines[ line ].EndCol.R = 0;
							Lines[ line ].EndCol.G = 255;
							Lines[ line ].EndCol.B = 0;

						}else if( Mutual )
						{

							Lines[ line ].StartCol.R = 255;
							Lines[ line ].StartCol.G = 255;
							Lines[ line ].StartCol.B = 255;
							Lines[ line ].EndCol.R = 255;
							Lines[ line ].EndCol.G = 64;
							Lines[ line ].EndCol.B = 64;
						}else{
							Lines[ line ].StartCol.R = 0;
							Lines[ line ].StartCol.G = 0;
							Lines[ line ].StartCol.B = 255;
							Lines[ line ].EndCol.R = 0;
							Lines[ line ].EndCol.G = 0;
							Lines[ line ].EndCol.B = 64;
						}
						Lines[ line ].StartTrans = 255;
						Lines[ line ].EndTrans = 255;
						Lines[ line ].Group = NodePnt->Group;
					}



				}
			}
		}
		NodePnt++;

	}
}





/*===================================================================
	Procedure	:		Recursive Node Dsistance Find.....
	Input		:		NODE * Node
				:		int32	Network
	Output		:		Nothing
===================================================================*/
NODE * Source;
float * NA;

void NodeRecurse( NODE * NodeFrom , NODE * NodeTo , float Distance )
{
	int i;

	if( NodeTo == Source )
		return;

	Distance += DistanceVector2Vector( &NodeFrom->Pos , &NodeTo->Pos );

	// Set the NA pointer to the place in the NodeArray we are interested in....
	NA = NodeArray;
	NA += ( Source->NodeNum * NodeNetworkHeader.NumOfNodes ) + NodeTo->NodeNum;
	
	if( ( *NA == -1.0F ) ||	( Distance < *NA ) )
	{
		*NA = Distance;
	}else{
		return;
	}
	for( i = 0 ; i < NodeTo->NumOfLinks ; i++ )
	{
		NodeRecurse( NodeTo , NodeTo->NodeLink[i] ,  Distance );
	}
}
/*===================================================================
	Procedure	:		Set Network weight form 1 Node...
	Input		:		NODE * Node
				:		int32	Network
	Output		:		Nothing
===================================================================*/
void SetNetworkDistance( void )
{
	int e,o,v;
	NODE * NodeFrom;
	float  * na;

	if( !NodeNetworkHeader.State )
		return;
							  
	NodeFrom = NodeNetworkHeader.FirstNode;
	
	for( e = 0 ; e < NodeNetworkHeader.NumOfNodes ; e++ )
	{
		for( o = 0 ; o < NodeNetworkHeader.NumOfNodes ; o++ )
		{

			na = NodeArray;
			na += ( e * NodeNetworkHeader.NumOfNodes ) + o;
			*na = -1.0F;
		}
		
		na = NodeArray;
		na += ( e * NodeNetworkHeader.NumOfNodes ) + e;
		*na = 0.0F;
	}
	
	
	for( e = 0 ; e < NodeNetworkHeader.NumOfNodes ; e++ )
	{
		Source =  NodeFrom;
		for( v = 0 ; v < NodeFrom->NumOfLinks ; v++ )
		{
			NodeRecurse( NodeFrom, NodeFrom->NodeLink[v], 0.0F );
		}
		NodeFrom++;
	}


	for( e = 0 ; e < 1 ; e++ )
	{
		for( o = 0 ; o < NodeNetworkHeader.NumOfNodes ; o++ )
		{
			na = NodeArray;
			na += ( e * NodeNetworkHeader.NumOfNodes ) + o;
			if( *na == -1.0F )
			{
				// a node is not connected to another node
//				DebugPrintf("Node %d Is not Connected to Node %d in any way...THIS IS BAD..FIX IT!\n", o , e  );
//				Msg( "Node %d Is not Connected to Node %d in any way...THIS IS BAD..FIX IT!\n", o , e );
			}
		}
	}
}


/*===================================================================
	Procedure	:		Find the nearest node for an object using its network number..
	Input		:		OBJECT * Object
	Output		:		Nothing
===================================================================*/
void FindNearestNode( OBJECT * Object )
{
	float Distance;
	int i;
	float Temp;
	NODE * Node;

	if( !NodeNetworkHeader.State )
		return;

	Distance = -1.0F;

	Node = NodeNetworkHeader.FirstNode;

	for( i = 0 ; i < NodeNetworkHeader.NumOfNodes; i ++ )
	{
		Temp = DistanceVector2Vector( (VECTOR*) &Node->Pos , (VECTOR*) &Object->Pos );

		if( (Object->NodeNetwork & Node->NetMask) && AI_ClearLOS( &Object->Pos, Object->Group , &Node->Pos ) && ( (Distance == -1.0F) || (Temp < Distance) ) )
		{
			Distance = Temp;
			Object->NearestNode = (void*) Node;
		}
		Node++;
	}
}


/*===================================================================
	Procedure	:		Update the nearest node for an object using its network number..
	Input		:		OBJECT * Object
	Output		:		Nothing
===================================================================*/

void UpdateNearestNode( OBJECT * Object )
{
	float Distance;
	int i;
	float Temp;
	NODE * Node;
	NODE * NodeLink;

	if( !NodeNetworkHeader.State )
		return;

	if( !Object->NearestNode )
		return;
	Node = (NODE *) Object->NearestNode;

	Distance = DistanceVector2Vector( (VECTOR*) &Node->Pos , (VECTOR*) &Object->Pos );
	
	for( i = 0 ; i < Node->NumOfLinks ; i ++ )
	{

		NodeLink = Node->NodeLink[i];
		
		Temp = DistanceVector2Vector( (VECTOR*) &NodeLink->Pos , (VECTOR*) &Object->Pos );
		
		if( ( Temp < Distance ) && AI_ClearLOS( &Object->Pos ,Object->Group, &NodeLink->Pos ) )
		{
			Distance = Temp;
			
			if( !( Object->NodeNetwork & NodeLink->NetMask ) )
			{
				// uh oh im leaving my node network......
				if( ( Object->NodeNetwork & Node->NetMask ) )
				{
					// remember which was the closest node on my network
					Object->LastNearestNode = (void*) Node;
				}
			}
			
			Object->NearestNode = (void*) NodeLink;
		}
	}
}

/*===================================================================
	Procedure	:		Which Node to aim for to get to my target Node....
	Input		:		int16 Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to get to my destination quickly..
===================================================================*/
NODE * WhichNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo )
{
	float Distance,Temp;
	int i;
	NODE * NodeLink;
	NODE * NodeReturn;
	float * na;



	if( !NodeNetworkHeader.State )
		return NULL;

	if( (NodeFrom == NodeTo) || !NodeFrom || !NodeTo )
		return NodeFrom;
	
	Distance = -1.0F;
	NodeReturn = NULL;
  
	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];

		na = NodeArray;
		na += ( NodeLink->NodeNum * NodeNetworkHeader.NumOfNodes ) + NodeTo->NodeNum;
		Temp = *na;
		if( (Temp != -1.0F ) && ( (Distance == -1.0F) || (Temp < Distance ) ) )
		{
			if( NodeLink->NetMask & Network )
			{
				NodeReturn = NodeLink;
				Distance = Temp;
			}
			else
			{
				//if its not on my network but is closer then the one on my network is useless so dont use it..
//				NodeReturn = NULL;
//				Distance = Temp;
			}
		}

	}
	return NodeReturn;
}
/*===================================================================
	Procedure	:		Which Node to aim for to get to get away from my target....
	Input		:		int16 Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to get to my destination quickly..
===================================================================*/
NODE * WhichRetreatNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo )
{
	float Distance,Temp;
	int i;
	NODE * NodeLink;
	NODE * NodeReturn;
	float * na;


	if( !NodeNetworkHeader.State )
		return NULL;

	if( (NodeFrom == NodeTo) || !NodeFrom || !NodeTo )
		return NodeFrom;
	
	Distance = -1.0F;
	NodeReturn = NULL;
  
	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];

		na = NodeArray;
		na += ( NodeLink->NodeNum * NodeNetworkHeader.NumOfNodes ) + NodeTo->NodeNum;
		Temp = *na;
		if( (Distance == -1.0F) || (Temp > Distance ) )
		{
			if( NodeLink->NetMask & Network )
			{
				NodeReturn = NodeLink;
				Distance = Temp;
			}
			else
			{
				//if its not on my network but is closer then the one on my network is useless so dont use it..
				NodeReturn = NULL;
				Distance = Temp;
			}
		}

	}
	return NodeReturn;
}



/*===================================================================
	Procedure	:		An Alternate root is needed so find a different node if possible..
	Input		:		int16 Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to for a different path..
===================================================================*/
NODE * ChooseAlternateNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo )
{
	int i;
	NODE * NodeLink;

	if( !NodeNetworkHeader.State )
		return NULL;

	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];

		if( (NodeLink != NodeTo) && (NodeLink->NetMask&Network) )
		{
			return NodeLink;
		}
	}
	return NULL;
}







NODE * FindSuitableSplineNode( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 )
{

	int i;
	NODE * NodeLink;

	if( !NodeNetworkHeader.State )
		return NULL;

	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];
		if( (NodeLink != Node1 ) &&
			(NodeLink != Node2 ) &&
			(NodeLink != Node3 ) &&
			(NodeLink != Node4 ) &&
			(NodeLink->NetMask&Network) )
			return NodeLink;
	}
	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];
		if( NodeLink->NetMask&Network )
			return NodeLink;
	}
	return NodeFrom;
}
NODE * FindSuitableSplineNodeRandom( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 )
{

	int i;
	NODE * NodeLink;

	if( !NodeNetworkHeader.State )
		return NULL;

	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];
		if( (NodeLink != Node1 ) &&
			(NodeLink != Node2 ) &&
			(NodeLink != Node3 ) &&
			(NodeLink != Node4 ) &&
			(NodeLink->NetMask&Network) )
		{
			if( !Random_Range((uint16) (NodeFrom->NumOfLinks / 2) ) )
				return NodeLink;
		}
	}
	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];
		if( (NodeLink != Node1 ) &&
			(NodeLink != Node2 ) &&
			(NodeLink != Node3 ) &&
			(NodeLink != Node4 ) &&
			(NodeLink->NetMask&Network) )
		{
			return NodeLink;
		}
	}
	for( i = 0 ; i < NodeFrom->NumOfLinks ; i ++ )
	{
		NodeLink = NodeFrom->NodeLink[i];
		if( NodeLink->NetMask&Network )
			return NodeLink;
	}
	return NodeFrom;
}

/*===================================================================
	Procedure	:		Special Update the nearest node for an object using its network number..
	Input		:		OBJECT * Object
	Output		:		Nothing
===================================================================*/

void UpdateNearestNodeSpecial( OBJECT * Object )
{
	float Distance;
	int i;
	float Temp;
	NODE * Node;
	NODE * NodeLink;

	if( !NodeNetworkHeader.State )
		return;

	Node = (NODE *) Object->NearestNode;
	if( !Node )
		return;
	
	Distance = DistanceVector2Vector( (VECTOR*) &Node->Pos , (VECTOR*) &Object->Pos );
	
	for( i = 0 ; i < Node->NumOfLinks ; i ++ )
	{
		NodeLink = Node->NodeLink[i];
		
		Temp = DistanceVector2Vector( (VECTOR*) &NodeLink->Pos , (VECTOR*) &Object->Pos );
		
		if( ( Object->NodeNetwork & NodeLink->NetMask ) &&
			( Object->Group != NodeLink->Group ) &&
			( Temp < Distance ) &&
			AI_ClearLOS( &Object->Pos ,Object->Group, &NodeLink->Pos ) )
		{
			Distance = Temp;
			Object->NearestNode = (void*) NodeLink;
		}
	}
	Node = (NODE *) NodeInGroup[Object->Group];
	while( Node )
	{
		if( (Object->NodeNetwork & Node->NetMask) && (Node != Object->NearestNode) )
		{
			Temp = DistanceVector2Vector( (VECTOR*) &Node->Pos , (VECTOR*) &Object->Pos );
			if( ( Temp < Distance ) && AI_ClearLOS( &Object->Pos ,Object->Group, &Node->Pos ) )
			{
				Distance = Temp;
				
				Object->NearestNode = (void*) Node;
			}
		}
		Node = Node->NextNodeInGroup;
	}
	Node = (NODE *) Object->NearestNode;

}

/*===================================================================
	Procedure	:		Return Pos of nearest node to a point
	Input		:		VECTOR	*	Pos
				:		uint16	*	Group
				:		VECTOR	*	TopLeft
				:		VECTOR	*	BottomRight
	Output		:		float		Distance ( -1 None )
===================================================================*/
float ReturnClosestNode( VECTOR * Pos, VECTOR * NearestNodePos, uint16 * Group, VECTOR * TopLeft, VECTOR * BottomRight )
{
	int			i;
	float		Temp;
	NODE	*	Node;
	NODE	*	NearestNode = NULL;
	float		Distance = -1.0F; 

	if( !NodeNetworkHeader.State )
		return Distance;

	Node = NodeNetworkHeader.FirstNode;

	for( i = 0 ; i < NodeNetworkHeader.NumOfNodes; i ++ )
	{
		if( !( ( Node->Pos.x > TopLeft->x ) && ( Node->Pos.x < BottomRight->x ) &&
			   ( Node->Pos.y > TopLeft->y ) && ( Node->Pos.y < BottomRight->y ) &&
			   ( Node->Pos.z > TopLeft->z ) && ( Node->Pos.z < BottomRight->z ) ) )
		{
			Temp = DistanceVector2Vector( (VECTOR*) &Node->Pos , Pos );

			if( Distance < 0.0F )
			{
				Distance = Temp;
				NearestNode = Node;
			}
			else
			{
				if( Temp < Distance )
				{
					Distance = Temp;
					NearestNode = Node;
				}
			}
		}
		Node++;
	}

	if( NearestNode )
	{
		NearestNodePos->x = NearestNode->Pos.x;
		NearestNodePos->y = NearestNode->Pos.y;
		NearestNodePos->z = NearestNode->Pos.z;
		*Group = NearestNode->Group;
	}
	return( Distance );
}




