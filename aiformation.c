
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
extern	PRIMARYWEAPONATTRIB	PrimaryWeaponAttribs[];
extern	SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[];
/*===================================================================
	Procedure	:	AIR Formation Flying
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR_FORMATION( register ENEMY * Enemy )
{
	float	Dist;
	OBJECT * TObject;
	OBJECT * SObject;
	VECTOR		NewPos;
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	VECTOR		Offset;
	VECTOR		TargetPos;
	VECTOR		TempDir;
	ENEMY * LinkEnemy;

	SObject = &Enemy->Object;
	// Is it time to think???
	AI_THINK( Enemy , false ,false);

	LinkEnemy = Enemy->FormationLink;

	if( !LinkEnemy || ( LinkEnemy->Object.AI_Mode == AIMODE_RETREAT ) )
	{
		if( Enemy->AIFlags & ( AI_ICANSEEPLAYER + AI_ICANHEARPLAYER ) )
		{
			AI_SetDOGFIGHT( Enemy );
			return;
		}else{
			AI_SetFOLLOWPATH( Enemy );
			return;
		}
	}

	TObject = (OBJECT*) Enemy->TShip;

	
	Enemy->Timer -= framelag;
	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;

	Enemy->PrimaryFireTimer -= framelag;
	if( Enemy->PrimaryFireTimer < 0.0F )
		Enemy->PrimaryFireTimer = 0.0F;
	Enemy->SecondaryFireTimer -= framelag;
	if( Enemy->SecondaryFireTimer < 0.0F )
		Enemy->SecondaryFireTimer = 0.0F;

	if( Enemy->AIFlags & ( AI_ICANSEEPLAYER + AI_ICANHEARPLAYER ) )
	{
		Enemy->TNode = NULL;
		if( !(Enemy->Object.Flags & SHIP_Scattered ) && (Enemy->AIFlags & AI_ICANSEEPLAYER) && !(Enemy->AIFlags & AI_FRIENDLYFIRE) )
		{
			ApplyMatrix( &SObject->Mat, &Forward, &TempVector );
			ApplyMatrix( &SObject->Mat, &SlideUp, &TempUpVector );

			if( (Enemy->PrimaryFireTimer == 0.0F) && ( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )  )
			{
				Enemy->PrimaryFireTimer = EnemyTypes[Enemy->Type].PrimaryFireRate + (float) Random_Range( (u_int16_t) EnemyTypes[Enemy->Type].PrimaryFireRate );
				EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, EnemyTypes[Enemy->Type].PrimaryWeaponType,
					Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
					EnemyTypes[Enemy->Type].PowerLevel, 0.0F, false, NULL );
			}
		}
	}

	if( (Enemy->Object.Flags & SHIP_Scattered ) || ( (EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_RETREAT) && ( Enemy->Object.Shield <= (EnemyTypes[Enemy->Type].Shield * 0.15 ) ) ) )
	{
		// we should get out of hear..
		Enemy->FormationLink = NULL;
		AI_SetRETREAT( Enemy );
		return;
	}
	
	AI_UPDATEGUNS( Enemy );
	// Aim at target

	if( TObject && ( Enemy->AIFlags & AI_ICANSEEPLAYER ) )
	{
		if( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )
		{
			AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move , &Enemy->Object.Pos , TObject , &NewPos ,   PrimaryWeaponAttribs[EnemyTypes[Enemy->Type].PrimaryWeaponType].Speed[0] );
		}else{
			NewPos = TObject->Pos;
		}
		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &NewPos );
		
		if( !( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ICANTPITCH ) )
		{
			Enemy->AIMoveFlags |= AimData.Flags;
		}else{
			//This enemy cant look up or down so it has to move up and down to compensate....
			Enemy->AIMoveFlags |= ( AimData.Flags & ( AI_CONTROL_TURNLEFT + AI_CONTROL_TURNRIGHT ) );
			if( AimData.Flags & AI_CONTROL_TURNUP )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_UP;
			}else if( AimData.Flags & AI_CONTROL_TURNDOWN )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_DOWN;
			}
			
		}
		
		Enemy->AI_Angle = AimData.Angle;
	}else{
		ApplyMatrix( &LinkEnemy->Object.Mat, &Forward, &TempDir );
		TempDir.x *= 1024.0F * GLOBAL_SCALE * 5.0F;
		TempDir.y *= 1024.0F * GLOBAL_SCALE * 5.0F;
		TempDir.z *= 1024.0F * GLOBAL_SCALE * 5.0F;
		NewPos.x = LinkEnemy->Object.Pos.x + TempDir.x;
		NewPos.y = LinkEnemy->Object.Pos.y + TempDir.y;
		NewPos.z = LinkEnemy->Object.Pos.z + TempDir.z;

		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &NewPos );
		if( !( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ICANTPITCH ) )
		{
			Enemy->AIMoveFlags |= AimData.Flags;
		}else{
			//This enemy cant look up or down so it has to move up and down to compensate....
			Enemy->AIMoveFlags |= ( AimData.Flags & ( AI_CONTROL_TURNLEFT + AI_CONTROL_TURNRIGHT ) );
			if( AimData.Flags & AI_CONTROL_TURNUP )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_UP;
			}else if( AimData.Flags & AI_CONTROL_TURNDOWN )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_DOWN;
			}
			
		}
		
		Enemy->AI_Angle = AimData.Angle;
	}

	if( Enemy->AvoidTimer )
	{
		Enemy->AIMoveFlags = Enemy->AvoidType;
	}else{

		Offset = Enemy->FormationOffset;
		ApplyMatrix( &LinkEnemy->Object.Mat, &Offset, &TargetPos );
		TargetPos.x += LinkEnemy->Object.Pos.x;
		TargetPos.y += LinkEnemy->Object.Pos.y;
		TargetPos.z += LinkEnemy->Object.Pos.z;

		Dist = DistanceVector2Vector( &TargetPos , &Enemy->Object.Pos);

		if( Dist > (EnemyTypes[Enemy->Type].Radius * 0.75F) )
		{
			Offset.x = TargetPos.x - Enemy->Object.Pos.x;
			Offset.y = TargetPos.y - Enemy->Object.Pos.y;
			Offset.z = TargetPos.z - Enemy->Object.Pos.z;

			ApplyMatrix( &Enemy->Object.InvMat, &Offset, &TargetPos );

			if( TargetPos.x < -(EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_LEFT;
			}
			if( TargetPos.x > (EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_RIGHT;
			}
			if( TargetPos.y < -(EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_DOWN;
			}
			if( TargetPos.y > (EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_UP;
			}
			if( TargetPos.z < -(EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_BACK;
			}
			if( TargetPos.z > (EnemyTypes[Enemy->Type].Radius * 0.25F) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
			}

		}
	}
}
/*===================================================================
	Procedure	:	AIR Formation Flying
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_LITTLEGEEK_FORMATION( register ENEMY * Enemy )
{
	OBJECT * SObject;
	VECTOR		Offset;
	VECTOR		TargetPos;
	ENEMY * LinkEnemy;
	SObject = &Enemy->Object;
	// Is it time to think???
	AI_THINK( Enemy , false ,false);

	LinkEnemy = Enemy->FormationLink;

	if( !LinkEnemy || ( LinkEnemy->Object.Shield <= (EnemyTypes[LinkEnemy->Type].Shield * 0.5F ) )  )
	{
		Enemy->Object.ControlType = ENEMY_CONTROLTYPE_FLY_AI;
		EnemyTypes[ENEMY_Boss_LittleGeek].Radius = 800 * GLOBAL_SCALE;
		EnemyTypes[ENEMY_Boss_BigGeek].Radius = 900.0F * GLOBAL_SCALE;
		AI_SetDOGFIGHT( Enemy );
		SetCurAnimSeq( LITTLEGEEKSEQ_Open, &Enemy->Object );
		Enemy->FormationLink = NULL;
		return;
	}

	SetCurAnimSeq( LITTLEGEEKSEQ_Stop, &Enemy->Object );

	Offset = Enemy->FormationOffset;
	ApplyMatrix( &LinkEnemy->Object.Mat, &Offset, &TargetPos );
	TargetPos.x += LinkEnemy->Object.Pos.x;
	TargetPos.y += LinkEnemy->Object.Pos.y;
	TargetPos.z += LinkEnemy->Object.Pos.z;

	Enemy->Object.Pos = TargetPos;

	Enemy->Object.Quat = LinkEnemy->Object.Quat;
	Enemy->Object.Mat = LinkEnemy->Object.Mat;
	Enemy->Object.InvMat = LinkEnemy->Object.InvMat;
	Enemy->Object.FinalMat = LinkEnemy->Object.FinalMat;
	Enemy->Object.FinalInvMat = LinkEnemy->Object.FinalInvMat;
	EnemyTypes[ENEMY_Boss_LittleGeek].Radius = 0.0F;

}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif

