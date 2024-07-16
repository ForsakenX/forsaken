
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
extern	SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	VECTOR	SlideUp;
/*===================================================================
	Procedure	:	AIR Follow Path
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR_FOLLOWPATH( register ENEMY * Enemy )
{
	OBJECT * SObject;
	NODE * TNode;
	VECTOR	TempVector = { 0.0F , 0.0F , 0.0F };
	VECTOR	TempUpVector;
	VECTOR	TempForwardVector;
	u_int16_t	MineIndex;

	SObject = &Enemy->Object;

	AI_THINK( Enemy , false , false );

	if( !(Enemy->AIFlags & AI_ANYPLAYERINRANGE) )
		return;

	if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ATTACK_ONSITE )
	{
		Tinfo->Flags = 0;
		SET_TARGET_PLAYERS;
		AI_GetDistToNearestTarget( Enemy );
		Enemy->TShip = Tinfo->TObject;
	}

	if( Enemy->TShip && ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ATTACK_ONSITE ) )
	{
		if( Enemy->AIFlags&AI_ICANSEEPLAYER )
		{
			AI_SetDOGFIGHT( Enemy );
			return;
		}else if( Enemy->AIFlags&AI_ICANHEARPLAYER )
		{
			AI_SetMOVETOTARGET( Enemy );
			return;
		}
	}


	if( (Enemy->AIFlags&AI_MINEAVOID) && (EnemyTypes[Enemy->Type].Behave.Flags&AI_BEHAVIOUR_ATTACKMINES) )
	{
		Enemy->AvoidTimer = 0.0F;
		Enemy->AvoidType = 0;
		AI_SetKILLMINE( Enemy );
		return;
	}
	
	
	if( TNode = (NODE*) Enemy->TNode )
	{
		if( !(Enemy->Object.NodeNetwork&TNode->NetMask) )
		{
			// The node Im Targetting is not on my network...Better try and find one...
			if( Enemy->Object.LastNearestNode )
			{
				TNode = (NODE*) ( Enemy->TNode = WhichNode( 1 , Enemy->Object.NearestNode , Enemy->Object.LastNearestNode ) );
				
				if( !TNode )
				{
					AI_SetSCAN( Enemy );
					return;
				}
				
				if( !AI_ClearLOSNonZero( &Enemy->Object, &TNode->Pos , EnemyTypes[Enemy->Type].Radius  ) )
				{
					// couldnt find a node that will take me to my target so go back to my nearest...
					TNode = (NODE*) ( Enemy->TNode = Enemy->Object.NearestNode );
				}
			}
		}

		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &TNode->Pos );
		Enemy->AI_Angle = AimData.Angle;

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
			Tinfo->Flags = 0;
			SET_TARGET_NODES;
			AI_GetDistToNearestTarget( Enemy );
			Enemy->TNode = Tinfo->TObject;
		}
		// MINES........
		if(	(Enemy->AIFlags&AI_MINEAVOID) && (EnemyTypes[Enemy->Type].Behave.Flags&AI_BEHAVIOUR_AVOIDMINES) )
		{
			// A Mine is close....And I can See it...
			TNode = ChooseAlternateNode( Enemy->Object.NodeNetwork , Enemy->Object.NearestNode , Enemy->TNode );
			Enemy->TNode = TNode;
			Enemy->AIFlags &= ~AI_MINEAVOID;
			Enemy->AIMoveFlags &= ~AI_CONTROL_FORWARD;
		}

		if( !Enemy->AvoidTimer)
		{
			TNode = Enemy->Object.NearestNode;
			if( (TNode->Flags&NODE_DROPMINES) && (EnemyTypes[Enemy->Type].Behave.Flags&AI_BEHAVIOUR_DROPMINES) ) 
			{
				Enemy->SecondaryFireTimer -= framelag;
				if( Enemy->SecondaryFireTimer < 0.0F )
					Enemy->SecondaryFireTimer = 0.0F;
				
				if( (Enemy->SecondaryFireTimer == 0.0F) )
				{
					Enemy->SecondaryFireTimer = EnemyTypes[Enemy->Type].SecondaryFireRate + (float) Random_Range( (u_int16_t) EnemyTypes[Enemy->Type].SecondaryFireRate );
					//This is where we Lay Mines....
					
					ApplyMatrix( &Enemy->Object.Mat, &Forward, &TempForwardVector );
					ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );
					
					MineIndex = InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
								&Enemy->Object.Pos, &TempVector, &TempForwardVector, &TempUpVector,
								&TempVector, EnemyTypes[Enemy->Type].SecondaryWeaponType, false );

					if( MineIndex != (u_int16_t) -1 )
					{
						SecBulls[MineIndex].LifeSpan = 10.0F * 60.0F;
					}
				}
			}
		}
	}else{
		// If no target node has been found yet go to the nearest one...
		Enemy->TNode = Enemy->Object.NearestNode;
	}
}
/*===================================================================
	Procedure	:	CRAWL Follow Path
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_CRAWL_FOLLOWPATH( register ENEMY * Enemy )
{
	OBJECT * SObject;
	NODE * TNode;

	SObject = &Enemy->Object;

	AI_THINK( Enemy , false , false);

	if( !(Enemy->AIFlags & AI_ANYPLAYERINRANGE) )
		return;

	Enemy->Timer -= framelag;

	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;
	
	if(Enemy->Timer == 0.0F)
	{
		if( !(Enemy->AIFlags & AI_ICANSEEPLAYER ))
		{
			if( !Enemy->TShip )
			{
				AI_DO_SCAN( Enemy );
			}
		}
		Enemy->Timer  =	RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
	}
	
	AI_UPDATEGUNS( Enemy );


	if( (Enemy->Type == ENEMY_Legz) || (Enemy->Type == ENEMY_LEADER_Legz) )
	{
		if( !Enemy->Object.Animating )
		{
			SetCurAnimSeq( 0, &Enemy->Object );
		}
		Enemy->Object.AnimSpeed = 0.5F + ( ( Enemy->Object.Speed.z / EnemyTypes[Enemy->Type].MaxMoveRate ) * 2.0F );
	}

	
	if( TNode = (NODE*) Enemy->TNode )
	{

		if( !(EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_NOTURN) )
		{
			AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &TNode->SolidPos );
			Enemy->AIMoveFlags |= AimData.Flags;
			Enemy->AI_Angle = AimData.Angle;

 
			if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_DONTSTOPANDTURN ) || ( (AimData.Angle.y < 10.0F) && (AimData.Angle.y > -10.0F) ) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
			}
		}else{
			Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
		}

		if(Enemy->PickNewNodeNow)
		{
			Enemy->PickNewNodeNow = false;

			Enemy->LastTNode = TNode;


			if( !Enemy->NextTNode )
			{
				Enemy->Object.NearestNode = TNode;
				Enemy->NextTNode = FindSuitableSplineNode( Enemy->Object.NodeNetwork, Enemy->Object.NearestNode , Enemy->Object.NearestNode , Enemy->LastTNode , Enemy->NextTNode , Enemy->TNode );
			}
			
			TNode = Enemy->NextTNode;
			Enemy->TNode = TNode;


			if( TNode )
			{
				Enemy->Object.NearestNode = TNode;
				Enemy->NextTNode = FindSuitableSplineNode( Enemy->Object.NodeNetwork, Enemy->Object.NearestNode , Enemy->Object.NearestNode , Enemy->LastTNode , Enemy->NextTNode , Enemy->TNode );
			}
			return;
		}
	}else{
		// If no target node has been found yet go to the nearest one...
		Enemy->TNode = Enemy->Object.NearestNode;
		Enemy->NextTNode = NULL;
	}


}
#ifdef OPT_ON
#pragma optimize( "", off )
#endif

