
/*
.NOD file
	int32_t Num of nodes...
	{NODE
		int16_t Group
		VECTOR Pos
		float Radius
		u_int32_t NetMask...Which networks im in...
		int16_t numoflinks..
		{LINK
			int Link num inside this network....
		}
	}
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
#include "node.h"
#include "networking.h"
#include "file.h"
#include "triggers.h"
#include "models.h"
#include "enemies.h"
#include "mload.h"
#include "render.h"
#include "ai.h"
#include "lines.h"

#include "collision.h"

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

/*===================================================================
	Procedure	:		Load .Nod File
	Input		:		char	*	Filename 
	Output		:		Nothing
===================================================================*/
bool Nodeload( char * Filename )
{
	char		*	Buffer;
	char		*	OrgBuffer;
	int32_t		*	int32Pnt;
	u_int32_t		*	u_int32Pnt;
	int16_t		*	int16Pnt;
	float		*	FloatPnt;
	int			e,o,i;
	long			File_Size;
	long			Read_Size;
	NODE		*	NodePnt;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
   	u_int16_t		TempGroup;
	NORMAL		TempNormal;
	VECTOR		TempPos_New;
	VECTOR		Move_Off = { 0.0F , -MaxColDistance , 0.0F };
	bool		LegalGroup;


	for( i = 0 ; i < MAXGROUPS ; i ++ )
	{
		NodeInGroup[i] = NULL;
	}
	
	NodeNetworkHeader.State = false;


	File_Size = Get_File_Size( Filename );	

	if( !File_Size )
	{
		// Doesnt Matter.....
		return true;
	}
	Buffer = calloc( 1, File_Size );
	OrgBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Nodeload failed to Allocate buffer for %s failed\n", Filename );
		return false;
	}

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
	{
		Msg( "Nodeload Error reading file %s\n", Filename );
		return false;
	}

	u_int32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *u_int32Pnt++;
	VersionNumber = *u_int32Pnt++;
	Buffer = (char *) u_int32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != NOD_VERSION_NUMBER  ) )
	{
		Msg( "NodeLoad() Incompatible node( .NOD ) file %s", Filename );
		return( false );
	}

	int32Pnt = (int32_t*) Buffer;
	NodeNetworkHeader.NumOfNodes = *int32Pnt++;

	if( NodeNetworkHeader.NumOfNodes >= MAXNODES )
	{
		Msg( "You have to many Nodes!!! %d is far to many... \n", NodeNetworkHeader.NumOfNodes);
		return false;
	}

	NodeArray = (float*) malloc( NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof(float) );

	if(!NodeArray)
	{
		Msg( "Nodeload failed NodeArray malloc in %s \n", Filename );
		return false;

	}
	
	NodeNetworkHeader.FirstNode = (NODE*) calloc( NodeNetworkHeader.NumOfNodes, sizeof(NODE) );
	
	
	NodePnt = NodeNetworkHeader.FirstNode;
	if( !NodePnt )
	{
		Msg( "Nodeload failed allocate Node Pointer in %s \n", Filename );
		return false;
	}
	Buffer = (char*) int32Pnt;
	
	for( e = 0 ; e < NodeNetworkHeader.NumOfNodes ; e++ )
	{
		int16Pnt = (int16_t*) Buffer;
		NodePnt->Group = *int16Pnt++;
		FloatPnt = (float*) int16Pnt;
		
		NodePnt->NodeNum = e;
		
		NodePnt->Pos.x = *FloatPnt++;
		NodePnt->Pos.y = *FloatPnt++;
		NodePnt->Pos.z = *FloatPnt++;
		NodePnt->Radius = *FloatPnt++;
		
		u_int32Pnt = (u_int32_t*) FloatPnt;
		NodePnt->NetMask = *u_int32Pnt++;				// which networks this node is in...

//		if( !(NodePnt->NetMask&1) )
//		{
//			Msg( "Nodeload failed Node %d is not in Network 1\n", e );
//		}
		
		int16Pnt = (int16_t*) u_int32Pnt;
		NodePnt->Flags = (u_int16_t) *int16Pnt++;
		NodePnt->NumOfLinks = *int16Pnt++;
		
		if( NodePnt->NumOfLinks > MAXLINKSPERNODE )
		{
			Msg( "NoadLoad failed too many Nodelinks in network , Node %d \n", e );
			return false;
		}
		if( !NodePnt->NumOfLinks )
		{
			DebugPrintf("No Links for Node %d\n" , e );
		}
		int32Pnt = (int32_t*) int16Pnt;
		for( o = 0 ; o < NodePnt->NumOfLinks ; o++ )
		{
			
			NodePnt->NodeLink[o] = NodeNetworkHeader.FirstNode + *int32Pnt++;
		}

		NodePnt->SolidPos = NodePnt->Pos;
		// we need a Point on the background that is below the Node...
		
		LegalGroup = false;
		
		if( !PointInsideSkin( &NodePnt->Pos, NodePnt->Group ) )
		{
			DebugPrintf( "NoadLoad failed Node %d is in WRONG Group (%s)\n", e, Mloadheader.Group[ NodePnt->Group ].name );
			for( i = 0 ; i < Mloadheader.num_groups ; i++ )
			{
				if( PointInsideSkin( &NodePnt->Pos, (u_int16_t) i ) )
				{
					NodePnt->Group = i;
					DebugPrintf( "Node %d Should be in %s, FIX IT NOW!\n", e, Mloadheader.Group[ i ].name );
					LegalGroup = true;
					break;
				}
			}
		}else{
			LegalGroup = true;
		}

		if( LegalGroup )
		{
			if(	BackgroundCollide( &MCloadheadert0, &Mloadheader, &NodePnt->Pos, NodePnt->Group, &Move_Off,
				&NodePnt->SolidPos , &TempGroup, &TempNormal, &TempPos_New, false, NULL ) )
			{
#ifdef DEBUG_ON
				VECTOR move;
				float move_len;

				move.x = NodePnt->SolidPos.x - NodePnt->Pos.x;
				move.y = NodePnt->SolidPos.y - NodePnt->Pos.y;
				move.z = NodePnt->SolidPos.z - NodePnt->Pos.z;
				move_len = VectorLength( &move );
				if ( move_len > VectorLength( &Move_Off ) )
					DebugPrintf( "NodeLoad: move_len = %f , Node %d\n", move_len, e);
				if ( TempGroup != NodePnt->Group )
					DebugPrintf( "NodeLoad: ground group =%hd != node group = %hd, Node %d\n", TempGroup, NodePnt->Group, e );
#endif
				NodePnt->SolidPos.y += 75.0F;
			}else{
				DebugPrintf( "NodeLoad failed Couldnt find a Solid collide , Node %d in %s\n", e, Mloadheader.Group[ NodePnt->Group ].name );
				//return false;
			}
		}else{
			DebugPrintf( "Node %d Is not in any Legal Group..This is Very Bad...FIX IT!\n", e );
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
			return false;
		}
		// copy pre-calculated node distances over
		memmove( NodeArray2, Buffer, NodeNetworkHeader.NumOfNodes * NodeNetworkHeader.NumOfNodes * sizeof( float ) );

		NodeNetworkHeader.State = true;
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
	
	NodeNetworkHeader.State = true;

#ifndef LOAD_PRECALC_NODE_DISTANCE_TABLE
	SetNetworkDistance();
#endif
	
	return true;
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
		NodeNetworkHeader.State = false;
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
	u_int32_t NetMask = 1;
	u_int16_t	line;
	bool	Mutual;
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
			InitOneEnemy( ENEMY_GENTYPE_Initialised, &NodePnt->SolidPos, &Dir, &Up, NodePnt->Group, Enemy->ModelNum, -1 ,ENEMY_Swarm ,0,-1,(u_int16_t) -1 , 0.0F);
		}

//		if( (NodePnt->NetMask&NetMask) && !NodePnt->LegalGroup)
		if( (NodePnt->NetMask&NetMask) )
		{
			for( i = 0 ; i < NodePnt->NumOfLinks ; i++ )
			{
				NodeLink = NodePnt->NodeLink[i];
				if( NodeLink->NetMask & NetMask )
				{
					Mutual = false;

					for( e = 0 ; e < NodeLink->NumOfLinks ; e ++ )
					{
						if( NodePnt == NodeLink->NodeLink[e] )
							Mutual = true;
					}
					

					line = FindFreeLine();						// Line attached
					if( line != (u_int16_t ) -1 )
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
void ShowNodeNetwork( u_int32_t NetMask )
{
	int o,i,e;
	NODE * NodePnt;
	NODE * NodeLink;
	u_int16_t	line;
	bool	Mutual;
	bool	CanSee;

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
					Mutual = false;

					for( e = 0 ; e < NodeLink->NumOfLinks ; e ++ )
					{
						if( NodePnt == NodeLink->NodeLink[e] )
							Mutual = true;
					}
					
					CanSee = AI_ClearLOSNonZeroNonObject( &NodePnt->Pos, NodePnt->Group, &NodeLink->Pos, SHIP_RADIUS );
					
					line = FindFreeLine();						// Line attached
					if( line != (u_int16_t ) -1 )
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
				:		int32_t	Network
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
				:		int32_t	Network
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
	Input		:		int16_t Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to get to my destination quickly..
===================================================================*/
NODE * WhichNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo )
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
	Input		:		int16_t Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to get to my destination quickly..
===================================================================*/
NODE * WhichRetreatNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo )
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
	Input		:		int16_t Network..Which Network...
				:		NODE * NodeFrom ...Which node im close to..
				:		NODE * NodeTo...Which node im trying to get to...
	Output		:		NODE * Which node to go to for a different path..
===================================================================*/
NODE * ChooseAlternateNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo )
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







NODE * FindSuitableSplineNode( u_int32_t Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 )
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
NODE * FindSuitableSplineNodeRandom( u_int32_t Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 )
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
			if( !Random_Range((u_int16_t) (NodeFrom->NumOfLinks / 2) ) )
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
				:		u_int16_t	*	Group
				:		VECTOR	*	TopLeft
				:		VECTOR	*	BottomRight
	Output		:		float		Distance ( -1 None )
===================================================================*/
float ReturnClosestNode( VECTOR * Pos, VECTOR * NearestNodePos, u_int16_t * Group, VECTOR * TopLeft, VECTOR * BottomRight )
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




