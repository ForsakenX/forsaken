
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

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

// Externals
extern	float framelag;
extern	AIMDATA AimData;
extern	VECTOR	Forward;
extern	VECTOR	SlideUp;
void CreateSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
extern	bool ShowNode;
/*===================================================================
	Procedure	:	AIR Move to a Target
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR_MOVETOTARGET( register ENEMY * Enemy )
{
	OBJECT * TObject;
	OBJECT * SObject;
	NODE * TNode;
	NODE * Node;
	NODE * NewNode;

	SObject = &Enemy->Object;

	// Is it time to think???

	AI_THINK( Enemy , false , false);

	TObject = (OBJECT*) Enemy->TShip;

	if( !TObject )
	{
		AI_SetFOLLOWPATH( Enemy );
		return;
	}
	
	Enemy->Timer -= framelag;

	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;
	
	if( Enemy->AIFlags & AI_ICANSEEPLAYER )
	{
		AI_SetDOGFIGHT( Enemy );
		return;
	}

	if( !Enemy->TNode )
	{
		Enemy->TNode = Enemy->Object.NearestNode;
		Enemy->Timer = EnemyTypes[Enemy->Type].Behave.BlindSearchTime;	// How long I Search if I lose sight of target
	}
	
	AI_UPDATEGUNS( Enemy );
	// Aim at target
	if( Enemy->TNode )
	{
		if ( Enemy->Timer == 0.0F )
		{
			// I Only have a certian amount of time to see my target again..If that time expires go back to following...
			AI_SetFOLLOWPATH( Enemy );
			return;
		}

		TNode = (NODE*) Enemy->TNode;

		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &TNode->Pos );


		if( !( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ICANTPITCH ) )
		{
			Enemy->AIMoveFlags |= AimData.Flags;
			if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_DONTSTOPANDTURN ) || ( (AimData.Angle.x < 15.0F) && (AimData.Angle.x > -15.0F) &&	(AimData.Angle.y < 15.0F) && (AimData.Angle.y > -15.0F) ) )
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
		}else{
			//This enemy cant look up or down so it has to move up and down to compensate....
			if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_DONTSTOPANDTURN ) || ( (AimData.Angle.x < 15.0F) && (AimData.Angle.x > -15.0F) &&	(AimData.Angle.y < 15.0F) && (AimData.Angle.y > -15.0F) && (AimData.Angle.y < 3.0F) && (AimData.Angle.y > -3.0F)) )
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
			Enemy->AIMoveFlags |= ( AimData.Flags & ( AI_CONTROL_TURNLEFT + AI_CONTROL_TURNRIGHT ) );
			if( AimData.Flags & AI_CONTROL_TURNUP )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_UP;
			}else if( AimData.Flags & AI_CONTROL_TURNDOWN )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_DOWN;
			}

		}
		
		if( DistanceVector2Vector( &Enemy->Object.Pos , &TNode->Pos ) < 64.0F )
		{
			// Make my target my nearest automatically.....This might cause problems.....But it fixes others
			if( !( Enemy->Object.NodeNetwork & TNode->NetMask ) )
			{
				// The new node is not on my network...
				Node = Enemy->Object.NearestNode;
				if( Node->NetMask & Enemy->Object.NodeNetwork )
				{
					// But my old 1 was so remember it...
					Enemy->Object.LastNearestNode = Enemy->Object.NearestNode;
				}
			}
			Enemy->Object.NearestNode = TNode;
			// If I can leave my network to follow a target then do so if I have to...
			if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_LEAVENETWORK )
				NewNode = (void*) WhichNode( 1 , Enemy->Object.NearestNode , TObject->NearestNode );
			else
				NewNode = (void*) WhichNode( Enemy->Object.NodeNetwork , Enemy->Object.NearestNode , TObject->NearestNode );


			if( NewNode == Enemy->TNode && NewNode == Enemy->Object.NearestNode &&
				Enemy->TNode == Enemy->Object.NearestNode && (Enemy->Timer > ( 10.0F * 60.0F )) )
			{
				Enemy->Timer = 10.0F * 60.0F;
			}
			else
			{
				if( !NewNode )
				{
					// couldnt find a node that will take me to my target so go back to my nearest...
					Enemy->TNode = Enemy->Object.NearestNode;
	//			}else if( AI_ClearLOS( &Enemy->Object.Pos,Enemy->Object.Group, &NewNode->Pos ) )
				}else if( AI_ClearLOSNonZero( &Enemy->Object, &NewNode->Pos , EnemyTypes[Enemy->Type].Radius  ) )
				{
					// I can see a closer node so target it instead...
					Enemy->TNode = NewNode;
				}else{
					// cant see the ideal node so just keep going to my previous target...
					// So go back to following a path...
					Enemy->Timer = 0.0F;
				}
			}
		}
	}else{
		Enemy->TNode = Enemy->Object.NearestNode;
	}
	Enemy->AI_Angle = AimData.Angle;


	TNode = Enemy->Object.NearestNode;
//	TNode = Enemy->TNode;
	if( ShowNode && TNode )
		CreateSparks( &TNode->Pos, &SlideUp, TNode->Group );


}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

