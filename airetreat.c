
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
/*===================================================================
	Procedure	:	AIR Retreat And run away
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR_RETREAT( register ENEMY * Enemy )
{
	OBJECT * TObject;
	OBJECT * SObject;
	NODE * TNode;
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
	
	if( !Enemy->TNode )
	{
		Enemy->TNode = Enemy->Object.NearestNode;
		Enemy->Timer = EnemyTypes[Enemy->Type].Behave.RetreatTime;	// How long I Run away for...
	}
	
	AI_UPDATEGUNS( Enemy );
	// Aim at target

	if( Enemy->TNode )
	{
		if ( Enemy->Timer == 0.0F )
		{
			// I Only have a certian amount of time to Run Away..
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
			Enemy->Object.NearestNode = TNode;


			if( !(Enemy->Object.NodeNetwork&TNode->NetMask) && Enemy->Object.LastNearestNode && !(EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_LEAVENETWORK) )
			{
				// The node Im Targetting is not on my network...Better try and find one...
				TNode = WhichNode( 1 , Enemy->Object.NearestNode , Enemy->Object.LastNearestNode );
				
				if( !TNode )
				{
					Enemy->Object.LastNearestNode = NULL;
					return;
				}
				
				if( !AI_ClearLOSNonZero( &Enemy->Object, &TNode->Pos , EnemyTypes[Enemy->Type].Radius  ) )
				{
					// cant see the ideal node so just keep going to my previous target...
				}else{
				Enemy->TNode = TNode;
				}
			}else{
				// If I can leave my network to follow a target then do so if I have to...
				if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_LEAVENETWORK )
					NewNode = (void*) WhichRetreatNode( 1 , Enemy->Object.NearestNode , TObject->NearestNode );
				else
					NewNode = (void*) WhichRetreatNode( Enemy->Object.NodeNetwork , Enemy->Object.NearestNode , TObject->NearestNode );

				if( !NewNode || !AI_ClearLOSNonZero( &Enemy->Object, &NewNode->Pos , EnemyTypes[Enemy->Type].Radius  ) ) 
				{
					// cant see the ideal node so just keep going to my previous target...
				}else{
					Enemy->TNode = NewNode;
				}
			}
		}
	}else{
		Enemy->TNode = Enemy->Object.NearestNode;

	}
	Enemy->AI_Angle = AimData.Angle;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

