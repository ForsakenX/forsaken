
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "node.h"
#include "networking.h"
#include "ships.h"
#include "text.h"
#include "triggers.h"
#include "sphere.h"
#include "pickups.h"
#include "enemies.h"
#include "ai.h"
#include "secondary.h"
#include "primary.h"
#include "lines.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

// Externals
extern	float framelag;
extern	AIMDATA AimData;
extern	VECTOR	Forward;
extern	VECTOR	SlideUp;
extern	LINE			Lines[ MAXLINES ];
extern	NODENETWORKHEADER	NodeNetworkHeader;
extern	MLOADHEADER	Mloadheader;
/*===================================================================
	Procedure	:	AI Spline Movement....
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SPLINE_FOLLOWPATH( register ENEMY * Enemy )
{
	NODE * TNode;
	NODE * Node1;
	NODE * Node2;
	NODE * Node3;
	NODE * Node4;
	VECTOR	OldPos;
	VECTOR	MoveOffset;
	float	Distance;
	float	WantedDistance;
	float	Time;
	float	Tstep;
	VECTOR	TempPos;
	int Count;

	OldPos = Enemy->Object.Pos;

	if( !Enemy->SplineNode1 )
	{
		//We have to start
		TNode = Enemy->Object.NearestNode;

		Enemy->SplineNode1 = (void*) TNode;
		Enemy->SplineNode2 = (void*) TNode;
		Node1 = (NODE*)Enemy->SplineNode1;
		Node2 = (NODE*)Enemy->SplineNode2;
		Enemy->SplineNode3 = (void*) FindSuitableSplineNode( Enemy->Object.NodeNetwork ,Node2 , Node1 , Node2 , NULL , NULL );
		Node3 = (NODE*)Enemy->SplineNode3;
		Enemy->SplineNode4 = (void*) FindSuitableSplineNode( Enemy->Object.NodeNetwork ,Node3 , Node1 , Node2 , Node3 , NULL );

	}
	Node1 = (NODE*)Enemy->SplineNode1;
	Node2 = (NODE*)Enemy->SplineNode2;
	Node3 = (NODE*)Enemy->SplineNode3;
	Node4 = (NODE*)Enemy->SplineNode4;


	WantedDistance = (EnemyTypes[Enemy->Type].MaxMoveRate*0.65F) * framelag;

	Tstep = ( WantedDistance / DistanceVector2Vector( &Node2->Pos , &Node3->Pos ) ) * 0.1F;

	Distance = DistanceVector2Vector( &Node2->Pos , &Node3->Pos );
	Time = (Distance / (EnemyTypes[Enemy->Type].MaxMoveRate * 0.65F) );
	
	Distance = 0.0F;
	Count = 0;
	do
	{
		Enemy->Timer += Tstep;

		if( Enemy->Timer >= 1.0F )
		{
			Enemy->SplineNode1 = Enemy->SplineNode2;
			Enemy->SplineNode2 = Enemy->SplineNode3;
			Enemy->SplineNode3 = Enemy->SplineNode4;
			Node1 = (NODE*)Enemy->SplineNode1;
			Node2 = (NODE*)Enemy->SplineNode2;
			Node3 = (NODE*)Enemy->SplineNode3;
			
			Enemy->Timer = ((Enemy->Timer - 1.0F) * Time);
			Distance = DistanceVector2Vector( &Node2->Pos , &Node3->Pos );
			Time = (Distance / (EnemyTypes[Enemy->Type].MaxMoveRate*0.65F) );
			Enemy->Timer /= Time;
			
			Enemy->SplineNode4 = (void*) FindSuitableSplineNodeRandom( Enemy->Object.NodeNetwork ,Node3 , Node1 , Node2 , Node3 , NULL );
			Node4 = (NODE*)Enemy->SplineNode4;
			Tstep = ( WantedDistance / DistanceVector2Vector( &Node2->Pos , &Node3->Pos ) ) * 0.1F;

			if( Node3 && (Node3->Flags&NODE_TERMINATE) )
			{
				KillUsedEnemy( Enemy );
				return;
			}
		}
	
		
		TempPos = Enemy->Object.Pos;

		spline(&Enemy->Object.Pos, Enemy->Timer, &Node1->Pos, &Node2->Pos, &Node3->Pos, &Node4->Pos);

		Distance += DistanceVector2Vector( &Enemy->Object.Pos , &TempPos);
		Count++;

	}while( ( Distance < WantedDistance ) && (Count < 100) );
	

	AI_THINK( Enemy , true , true);

	if( (Enemy->AIFlags & AI_ANYPLAYERINRANGE) )
	{
		Enemy->PrimaryFireTimer -= framelag;
		if( Enemy->PrimaryFireTimer < 0.0F )
			Enemy->PrimaryFireTimer = 0.0F;
		
		if(Enemy->PrimaryFireTimer == 0.0F)
		{
			if( !Enemy->TShip )
			{
				AI_DO_SCAN( Enemy );
			}
			Enemy->PrimaryFireTimer  =	RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
		}
	}
	AI_UPDATEGUNS( Enemy );

	MoveOffset.x = Enemy->Object.Pos.x - OldPos.x;
	MoveOffset.y = Enemy->Object.Pos.y - OldPos.y;
	MoveOffset.z = Enemy->Object.Pos.z - OldPos.z;

	Enemy->Object.Group = MoveGroup( &Mloadheader, &OldPos, Enemy->Object.Group, &MoveOffset );


}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif









