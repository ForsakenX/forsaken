#ifdef LUA_BOT
#include "botai_main.h"
#include "botai_path.h"

extern bool ReverseNetwork;
extern int CurrentNode;
extern VECTOR FollowTargetPos;
extern int FollowTargetGroup;

VECTOR BOTAI_NodeNetwork(int node)
{
	if( strcmp( ShortLevelNames[LevelNum], "alpha") == 0 )
		return BOTAI_NodeNetworkAlpha(node);
	else if( strcmp( ShortLevelNames[LevelNum], "fourball") == 0 )
		return BOTAI_NodeNetworkFourball(node);
	else //if( strcmp( ShortLevelNames[LevelNum], "ship") == 0 )
		return BOTAI_NodeNetworkShip(node);
}

#define SHIP_NUM_BASEMENT_NODES 7
#define SHIP_NUM_NODES 34
#define SHIP_NUM_NODES_MAIN 23
#define SHIP_SHAFT_NODES 2
#define SHIP_TOP_BASEMENT_NODE 12
VECTOR BOTAI_NodeNetworkShip(int node)
{
	VECTOR NodePos;

	switch(node)
	{
		// bridge
		case  0: NodePos.x = -2800.0F; NodePos.y =  -430.0F; NodePos.z = 4090.0F; break;
		case  1: case 22: NodePos.x = -1440.0F; NodePos.y =  -530.0F; NodePos.z = 4150.0F; break;
		case  2: case 21: NodePos.x = -1070.0F; NodePos.y =  -530.0F; NodePos.z = 4820.0F; break;
		case  3: case 20: NodePos.x =  -980.0F; NodePos.y =  -570.0F; NodePos.z = 5970.0F; break;
		case  4: NodePos.x =   740.0F; NodePos.y =  -530.0F; NodePos.z = 5820.0F; break;
		case  5: NodePos.x =   400.0F; NodePos.y =  -680.0F; NodePos.z = 5080.0F; break;
		case  6: NodePos.x =  2650.0F; NodePos.y =  -680.0F; NodePos.z = 3700.0F; break;
				 // red room
		case  7: NodePos.x =  2600.0F; NodePos.y =  -600.0F; NodePos.z = 1760.0F; break;
		case  8: NodePos.x =   850.0F; NodePos.y = -2200.0F; NodePos.z = 1800.0F; break;
		case  9: NodePos.x =   650.0F; NodePos.y = -2210.0F; NodePos.z = 1980.0F; break;
		case 10: NodePos.x =  -235.0F; NodePos.y = -2150.0F; NodePos.z = 2160.0F; break;
		case 11: NodePos.x =  1180.0F; NodePos.y = -2200.0F; NodePos.z = 2380.0F; break;
				 // top of basement
		case 12: NodePos.x =  1600.0F; NodePos.y = -2200.0F; NodePos.z = 3810.0F; break;
		case 13: NodePos.x =   280.0F; NodePos.y = -2200.0F; NodePos.z = 3920.0F; break;
		case 14: NodePos.x =  -150.0F; NodePos.y = -2200.0F; NodePos.z = 5840.0F; break;
		case 15: NodePos.x = -1755.0F; NodePos.y = -2200.0F; NodePos.z = 5740.0F; break;
				 // bottom of slope
		case 16: NodePos.x = -2480.0F; NodePos.y = -2160.0F; NodePos.z = 4090.0F; break;
		case 17: NodePos.x = -1825.0F; NodePos.y = -1860.0F; NodePos.z = 4080.0F; break;
				 // top of slope
		case 18: NodePos.x =   500.0F; NodePos.y =  -590.0F; NodePos.z = 4150.0F; break;
		case 19: NodePos.x =  -230.0F; NodePos.y =  -680.0F; NodePos.z = 5960.0F; break;

				 // basement
		case 30: NodePos.x =  1500.0F; NodePos.y = -2150.0F; NodePos.z = 5030.0F; break;
		case 29: NodePos.x =  1410.0F; NodePos.y = -4490.0F; NodePos.z = 4950.0F; break;
		case 28: NodePos.x =  1670.0F; NodePos.y = -4510.0F; NodePos.z = 4780.0F; break;
		case 27: NodePos.x =  1800.0F; NodePos.y = -4340.0F; NodePos.z = 3790.0F; break;
		case 26: NodePos.x =   930.0F; NodePos.y = -4240.0F; NodePos.z = 4980.0F; break;
		case 25: NodePos.x =  1450.0F; NodePos.y = -4270.0F; NodePos.z = 5550.0F; break;
		case 24: NodePos.x =  2210.0F; NodePos.y = -4330.0F; NodePos.z = 5270.0F; break;

				 // bottom of vertical shaft
		case 32: NodePos.x =  2850.0F; NodePos.y = -2220.0F; NodePos.z = 1820.0F; break;
		case 33: NodePos.x =  2820.0F; NodePos.y = -2220.0F; NodePos.z = 3880.0F; break;

		default: NodePos.x = 0.0F; NodePos.y = 0.0F; NodePos.z = 0.0F; break;
	}

	return NodePos;
}

#define ALPHA_NUM_NODES_MAIN 14
#define ALPHA_TOP_OF_PIT_NODE 8
VECTOR BOTAI_NodeNetworkAlpha(int node)
{
	VECTOR NodePos;

	switch(node)
	{
		case  0: NodePos.x =  3670.0F; NodePos.y =   -40.0F; NodePos.z = -3125.0F; break; // generator room
		case  1: NodePos.x =  3670.0F; NodePos.y =   530.0F; NodePos.z = -3148.0F; break;
		case  2: NodePos.x =  3610.0F; NodePos.y =   555.0F; NodePos.z = -1835.0F; break; // generator passage
		case  3: NodePos.x =  2030.0F; NodePos.y =   545.0F; NodePos.z = -1034.0F; break;
		case  4: NodePos.x =  2082.0F; NodePos.y =   470.0F; NodePos.z = -2860.0F; break; // light room
		case  5: NodePos.x =  1970.0F; NodePos.y =  -305.0F; NodePos.z = -2810.0F; break; // stone room
		case  6: NodePos.x =  2608.0F; NodePos.y =  -330.0F; NodePos.z = -2780.0F; break; 
		case  7: NodePos.x =  2580.0F; NodePos.y =  -800.0F; NodePos.z = -2770.0F; break; 
		case  8: NodePos.x =  2055.0F; NodePos.y =  -725.0F; NodePos.z = -2795.0F; break; // connects to lower pit
		case  9: NodePos.x = -1202.0F; NodePos.y =  -770.0F; NodePos.z = -2825.0F; break; // lava corridor
		case 10: NodePos.x = -1280.0F; NodePos.y =   520.0F; NodePos.z = -2800.0F; break; // lower chambers entrance
		case 11: NodePos.x =    10.0F; NodePos.y =   475.0F; NodePos.z = -2833.0F; break; // rocks (cont to light rm)
		case 12: NodePos.x =    10.0F; NodePos.y =   520.0F; NodePos.z =	13.0F; break; // fire room
		case 13: NodePos.x =    14.0F; NodePos.y =  2170.0F; NodePos.z =	10.0F; break; // upper chambers entrance

				 // lower pit
		case 16: NodePos.x =  3119.0F; NodePos.y = -1515.0F; NodePos.z = -3555.0F; break; 
		case 17: NodePos.x =  3125.0F; NodePos.y = -1542.0F; NodePos.z = -2078.0F; break; 
		case 18: NodePos.x =   978.0F; NodePos.y = -1520.0F; NodePos.z = -3555.0F; break; 
		case 19: NodePos.x =   965.0F; NodePos.y = -1527.0F; NodePos.z = -2085.0F; break; 

		default: NodePos.x = 0.0F; NodePos.y = 0.0F; NodePos.z = 0.0F; break;
	}

	return NodePos;
}

VECTOR BOTAI_NodeNetworkFourball(int node)
{
	VECTOR NodePos;

	switch(node)
	{
		// upper level
		case  0: NodePos.x = -416.0F; NodePos.y =   925.0F; NodePos.z =    17.0F; break; // blb01 (1, 3, 4)
		case  1: NodePos.x = -450.0F; NodePos.y =   925.0F; NodePos.z =  1381.0F; break; // isit01 (0, 2, 5)
		case  2: NodePos.x = 1020.0F; NodePos.y =   865.0F; NodePos.z =  1420.0F; break; // trb01 (1, 3, 6)
		case  3: NodePos.x = 1085.0F; NodePos.y =   820.0F; NodePos.z =   -32.0F; break; // la01 (2, 0, 7)

				 // mid level 
		case  4: NodePos.x = -477.0F; NodePos.y =  -528.0F; NodePos.z =    17.0F; break; // object06 (0, 5, 8)
		case  5: NodePos.x = -440.0F; NodePos.y =  -540.0F; NodePos.z =  1381.0F; break; // jgkl01 (1, 4, 10)
		case  6: NodePos.x = 1020.0F; NodePos.y =  -585.0F; NodePos.z =  1420.0F; break; // dyuio001 (2, 10, 15)
		case  7: NodePos.x = 1080.0F; NodePos.y =  -540.0F; NodePos.z =   -32.0F; break; // dyuio001 (3, 8, 15)
		case  8: NodePos.x =  235.0F; NodePos.y =  -535.0F; NodePos.z =    78.0F; break; // object01 (7, 4, 9)
		case  9: NodePos.x =  195.0F; NodePos.y =  -535.0F; NodePos.z =   877.0F; break; // object02 (8, 18)
		case 10: NodePos.x =  210.0F; NodePos.y =  -505.0F; NodePos.z =  1495.0F; break; // bigat01 (5, 6, 11)
		case 11: NodePos.x =  200.0F; NodePos.y =  -435.0F; NodePos.z =   598.0F; break; // bigat01 (10, 12)

				 // ground level (pillars)
		case 12: NodePos.x =  330.0F; NodePos.y = -2835.0F; NodePos.z =   476.0F; break; // dui01 (11, 13. 16, 20)
		case 13: NodePos.x = 2560.0F; NodePos.y = -2195.0F; NodePos.z =   525.0F; break; // longee01 (12, 14)
		case 14: NodePos.x = 2052.0F; NodePos.y =  -685.0F; NodePos.z =   645.0F; break; // dyui001 (13, 15)
		case 15: NodePos.x = 1107.0F; NodePos.y =  -600.0F; NodePos.z =   625.0F; break; // dyui001 (14, 7, 6)
		case 16: NodePos.x =  360.0F; NodePos.y = -2710.0F; NodePos.z =  2445.0F; break; // opu01 (12, 17)

				 // tunnel
		case 17: NodePos.x =  250.0F; NodePos.y = -1910.0F; NodePos.z =  2465.0F; break; // opu01 (16, 18)
		case 18: NodePos.x =  250.0F; NodePos.y = -1915.0F; NodePos.z =   916.0F; break; // object04 (17, 9, 19)
		case 19: NodePos.x =  195.0F; NodePos.y = -1990.0F; NodePos.z = -1141.0F; break; // object03 (18, 20)
		case 20: NodePos.x =  200.0F; NodePos.y = -2905.0F; NodePos.z = -1151.0F; break; // object03 (19, 12)

		default: NodePos.x = 0.0F; NodePos.y = 0.0F; NodePos.z = 0.0F; break;
	}

	return NodePos;
}

void BOTAI_GetNextNode()
{
	if( strcmp( ShortLevelNames[LevelNum], "alpha") == 0 )
		BOTAI_GetNextNodeAlpha();
	else if( strcmp( ShortLevelNames[LevelNum], "fourball") == 0 )
		BOTAI_GetNextNodeFourball();
	else if( strcmp( ShortLevelNames[LevelNum], "ship") == 0 )
		BOTAI_GetNextNodeShip();
}

void BOTAI_GetNextNodeFourball()
{
	int random;
	if( !ReverseNetwork )
	{
		switch( CurrentNode )
		{
			case 0: CurrentNode = 1; break;
			case 1: CurrentNode = 5; break;
			case 2: CurrentNode = 1; break;
			case 3: CurrentNode = 2; break;
			case 4: CurrentNode = 0; break;
			case 5: CurrentNode = 10; break;
			case 6: CurrentNode = 2; break;
			case 7: CurrentNode = 3; break;
			case 8: CurrentNode = 4; break;
			case 9: CurrentNode = 8; break;
			case 10: CurrentNode = 11; break;
			case 11: CurrentNode = 12; break;
			case 12:
					 random = Random_Range(3);
					 if(random == 0) CurrentNode = 20;
					 else if(random == 1) CurrentNode = 13;
					 else CurrentNode = 16;
					 break;
			case 13: CurrentNode = 14; break;
			case 14: CurrentNode = 15; break;
			case 15: CurrentNode = 6; break;
			case 16: CurrentNode = 17; break;
			case 17: CurrentNode = 18; break;
			case 18: CurrentNode = 9; break;
			case 19: CurrentNode = 18; break;
			case 20: CurrentNode = 19; break;
		}
	}
	else
	{
		switch( CurrentNode )
		{
			case 0: CurrentNode = 4; break;
			case 1: CurrentNode = 0; break;
			case 2: CurrentNode = 1; break;
			case 3: CurrentNode = 2; break;
			case 4: CurrentNode = 8; break;
			case 5: CurrentNode = 1; break;
			case 6: CurrentNode = 2; break;
			case 7: CurrentNode = 15; break;
			case 8:
					random = Random_Range(2);
					if(random == 0) CurrentNode = 9;
					else CurrentNode = 7;
					break;
			case 9: CurrentNode = 18; break;
			case 10: CurrentNode = 5; break;
			case 11: CurrentNode = 10; break;
			case 12: CurrentNode = 11; break;
			case 13: CurrentNode = 12; break;
			case 14: CurrentNode = 13; break;
			case 15: CurrentNode = 14; break;
			case 16: CurrentNode = 12; break;
			case 17: CurrentNode = 16; break;
			case 18: 
					 random = Random_Range(2);
					 if(random == 0) CurrentNode = 19;
					 else CurrentNode = 17;
					 break;
			case 19: CurrentNode = 20; break;
			case 20: CurrentNode = 12; break;
		}
	}
}

void BOTAI_GetNextNodeAlpha()
{
	// random choice to go left or right at the rocks
	if( CurrentNode == 11 && ReverseNetwork )
	{
		if(Random_Range(2) > 0)
			CurrentNode = 4;
		else
			CurrentNode--;
		return;
	}

	// random choice to go down or straight at the light room
	if( CurrentNode == 4 && !ReverseNetwork )
	{
		if(Random_Range(2) > 0)
			CurrentNode = 11;
		else
			CurrentNode++;
		return;
	}

	CurrentNode++;

	// following the main network in reverse
	if( ReverseNetwork )
		CurrentNode -= 2;

	// in the lower pit
	if( CurrentNode > ALPHA_NUM_NODES_MAIN )
		CurrentNode = ALPHA_TOP_OF_PIT_NODE;	

	// looping over the node network
	else if( CurrentNode > ALPHA_NUM_NODES_MAIN-1 )
	{
		ReverseNetwork = true;
		CurrentNode = ALPHA_NUM_NODES_MAIN-1;
	}
	else if( CurrentNode < 0 )
	{
		ReverseNetwork = false;
		CurrentNode = 0;
	}
}

void BOTAI_GetNextNodeShip()
{
	CurrentNode++;

	// however, in the basement or shaft
	if( CurrentNode > SHIP_NUM_NODES_MAIN )
	{
		if( CurrentNode == SHIP_NUM_NODES || CurrentNode == SHIP_NUM_NODES-SHIP_SHAFT_NODES-1 )
			CurrentNode = SHIP_TOP_BASEMENT_NODE; // get back on main line
	}
	// following the main network in reverse
	else if(ReverseNetwork)
	{
		// loop to start
		if(CurrentNode <= 1)
			CurrentNode = SHIP_NUM_NODES_MAIN-1;
		// next node
		else
			CurrentNode -= 2;
	}

	// looping over the node network
	else if(CurrentNode > SHIP_NUM_NODES_MAIN-1)
		CurrentNode = 0;
}

int BOTAI_GetNodeGroup(int node)
{
	int Group = 0;
	VECTOR Pos = BOTAI_NodeNetwork(node);
	for(Group = 0; Group <Bsp_Header[0].NumGroups; Group++)
	{
		if( !AmIOutsideGroup( &Mloadheader, &Pos, Group ) )
			return Group;
	}
	return Group;
}

int BOTAI_GetNearestNode(OBJECT * TObj)
{
	int i;
	float dist;
	float shortest = BIGDISTANCE;
	int node = -1;
	VECTOR tmp;

	// for each node in the network
	for(i=0; i<50; i++)
	{
		tmp = BOTAI_NodeNetwork(i);

		if(tmp.x == 0.0F && tmp.y == 0.0F && tmp.z == 0.0F)
			continue;

		// that the given object has a line of sight to
		if( BOTAI_ClearLOS( &TObj->Pos, TObj->Group, &tmp ))
		{
			dist = DistanceVector2Vector(&TObj->Pos, &tmp);
			// find the shortest
			if(dist < shortest)
			{
				shortest = dist;
				node = i;
			}
		}
	}

	return node;
}

void BOTAI_FollowNodeNetwork( bool SlideOnly, bool FollowingTarget )
{
	VECTOR ThisTargetPos;
	VECTOR TempNodePos;
	int i;
	float dist;
	float shortest = BIGDISTANCE;

	// following a target find the node to move to
	if( FollowingTarget )	
	{
		CurrentNode = -1;
		// is there a node that both i and the target have a line of sight to? 
		for(i=0; i<SHIP_NUM_NODES; i++)
		{
			TempNodePos = BOTAI_NodeNetwork(i);
			if( BOTAI_ClearLOS( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &TempNodePos ) )
			{
				if( BOTAI_ClearLOS(&FollowTargetPos, FollowTargetGroup, &TempNodePos) )
				{
					CurrentNode = i;
					DebugPrintf("found node both i and target can see: %d \n", CurrentNode);
					break;
				}
			}
		}

		// otherwise find the closest node to the target that i can see 
		// change this later to search the node paths - probably the A* algorithm
		if( CurrentNode < 0 )
		{
			for(i=0; i<SHIP_NUM_NODES; i++)
			{
				TempNodePos = BOTAI_NodeNetwork(i);
				if( BOTAI_ClearLOS( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &TempNodePos ) )
				{
					dist = DistanceVector2Vector(&FollowTargetPos, &TempNodePos);
					if(dist < shortest)
					{
						shortest = dist;
						CurrentNode = i;
					}
				}
			}
			DebugPrintf("using closest node to target that i can see: %d\n", CurrentNode);
		}
	}

	// if we don't know where we're going, use the closest node 
	if( CurrentNode < 0 )
	{
		CurrentNode = BOTAI_GetNearestNode( &Ships[WhoIAm].Object );
		DebugPrintf("i don't know where i'm going, so moving to nearest node: %d\n", CurrentNode);
	}

	// get the node position
	ThisTargetPos = BOTAI_NodeNetwork( CurrentNode );

	if( FollowingTarget )
		DebugPrintf("following target to node %d\n", CurrentNode);
	else
		DebugPrintf("moving to node %d\n", CurrentNode);

	// try and get to the current node
	if( !SlideOnly )
	{
		// using normal movement
		if( !BOTAI_MoveToTarget( &ThisTargetPos ) )
			return;
	}
	// using slide only
	else if( !BOTAI_SlideToTarget( &ThisTargetPos ) )
		return;

	// -- reached current node now set the next node
	DebugPrintf("reached current node, looking for next...\n");

	// roaming without target
	if( !FollowingTarget )
		BOTAI_GetNextNode();
}
#endif // LUA_BOT
