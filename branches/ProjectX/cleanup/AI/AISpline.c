/*
 * a i s p l i n e . c
 *
 * AISPLINE AIMODE_FOLLOWPATH
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 12 $
 *
 * $Header: /PcProjectX/Ai/AISpline.c 12    3/05/98 9:29a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AISpline.c $
 * 
 * 12    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 11    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 10    11/27/97 6:39p Phillipd
 * 
 * 9     11/27/97 12:25p Phillipd
 * 
 * 8     11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 7     11/15/97 1:09p Phillipd
 * 
 * 6     10/30/97 3:40p Phillipd
 * 
 * 5     10/30/97 3:17p Phillipd
 * 
 * 4     10/30/97 9:30a Phillipd
 * Bikes with different mods started...
 * 
 * 3     10/29/97 12:25p Phillipd
 * 
 * 2     10/25/97 3:49p Phillipd
 * 
 * 1     10/25/97 1:32p Phillipd
 * 
 *
 */
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <dplay.h>
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "Node.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "ships.h"
#include "text.h"
#include "triggers.h"
#include "sphere.h"
#include "pickups.h"
#include "enemies.h"
#include "AI.h"
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
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AI Spline Movement....
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
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
	

	AI_THINK( Enemy , TRUE , TRUE);

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
			Enemy->PrimaryFireTimer  =	RESET_VALIDATE_TIME + (float) Random_Range( (uint16) RESET_VALIDATE_TIME );
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









