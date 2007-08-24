/*
 * k i l l m i n e . c
 *
 * AIMODE_KILLMINE
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 10 $
 *
 * $Header: /PcProjectX/Ai/AIKillMine.c 10    3/28/98 10:06a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIKillMine.c $
 * 
 * 10    3/28/98 10:06a Phillipd
 * 
 * 9     3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 8     21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 7     5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 6     26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 5     11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 4     11/11/97 11:00a Phillipd
 * 
 * 3     10/11/97 19:16 Collinsd
 * Added Bodge to fix mappers fuckup.
 * 
 * 2     9/16/97 2:19p Phillipd
 * 
 * 1     9/15/97 10:20a Phillipd
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
extern	SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR KillMine
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_KILLMINE( register ENEMY * Enemy )
{
	OBJECT * SObject;
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	BYTE		Weapon;

	SObject = &Enemy->Object;

	// Is it time to think???

	AI_THINK( Enemy , FALSE , FALSE);

	Enemy->Timer -= framelag;
	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;

	Enemy->PrimaryFireTimer -= framelag;
	if( Enemy->PrimaryFireTimer < 0.0F )
		Enemy->PrimaryFireTimer = 0.0F;

	if( !(Enemy->AIFlags &AI_MINEAVOID) )
	{
		Enemy->AIFlags &= ~(AI_MINEAVOID + AI_MINEISCLOSE);
		AI_SetFOLLOWPATH( Enemy );
		return;
	}
	
	Enemy->AIFlags &= ~AI_MINEAVOID;

	
	AI_UPDATEGUNS( Enemy );
	// Aim at target

	AI_AimAtTarget( &SObject->InvMat , &SObject->Pos, &Enemy->MinePos );
	
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
	if( Enemy->AvoidTimer && Enemy->AvoidType )
	{
		Enemy->AIMoveFlags = Enemy->AvoidType;
	}else{
		Enemy->AvoidTimer = 0.0F;
		ApplyMatrix( &SObject->Mat, &Forward, &TempVector );
		ApplyMatrix( &SObject->Mat, &SlideUp, &TempUpVector );
		  
		if( !(Enemy->Object.Flags & SHIP_Scattered ) && (Enemy->PrimaryFireTimer == 0.0F) && ( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )  )
		{
			Enemy->PrimaryFireTimer = EnemyTypes[Enemy->Type].PrimaryFireRate + (float) Random_Range( (uint16) EnemyTypes[Enemy->Type].PrimaryFireRate );

			Weapon = BodgePrimaryWeapon( EnemyTypes[Enemy->Type].PrimaryWeaponType, Enemy->PickupHeld );

			EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
				Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
				EnemyTypes[Enemy->Type].PowerLevel, 0.0F, FALSE, NULL );
		}
	}
	Enemy->AI_Angle = AimData.Angle;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

