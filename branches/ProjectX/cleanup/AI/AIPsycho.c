/*
 * a i p s y c h o . c
 *
 * AIMODE_DEATH_PSYCHO
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 9 $
 *
 * $Header: /PcProjectX/Ai/AIPsycho.c 9     3/28/98 10:06a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIPsycho.c $
 * 
 * 9     3/28/98 10:06a Phillipd
 * 
 * 8     21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 7     5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 6     5/12/97 15:57 Collinsd
 * Uses EnemyFirePrimary
 * 
 * 5     11/26/97 10:44a Phillipd
 * 
 * 4     11/25/97 4:13p Phillipd
 * 
 * 3     11/25/97 3:13p Phillipd
 * 
 * 2     11/25/97 2:37p Phillipd
 * 
 * 1     11/25/97 2:37p Phillipd
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

// Externals
extern	float framelag;
extern	AIMDATA AimData;
extern	VECTOR	Forward;
extern	VECTOR	SlideUp;
extern	PRIMARYWEAPONATTRIB	PrimaryWeaponAttribs[];
extern	SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[];
extern	MLOADHEADER Mloadheader;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR Crash and burn...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_DEATH_PSYCHO( register ENEMY * Enemy )
{
	OBJECT * SObject;
	VECTOR		NewDir = { 0.0F, 1.0F, 0.0F };
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	BYTE	Weapon;

	SObject = &Enemy->Object;
	Enemy->AIFlags |= AI_ANYPLAYERINRANGE;

	Enemy->Timer -= framelag;
	if( (Enemy->Timer < 0.0F) || (Enemy->AIMoveFlags & AI_CONTROL_COLLISION) )
	{
		Enemy->Timer = 0.0F;
		// Time to Die properly
		CreateNewExplosion( &Enemy->Object.Pos, &NewDir, Enemy->Object.Group );
		KillUsedEnemy( Enemy );
		return;
	}
	Enemy->Object.Angle.y += (3.0F - ( 3.0F * ( ( 1.0F / ( 1.5F * 60.0F ) ) * Enemy->Timer ) ) ) * framelag;
	Enemy->Object.Angle.x += (0.5F - ( 0.5F * ( ( 1.0F / ( 1.5F * 60.0F ) ) * Enemy->Timer ) ) ) * framelag;

	Enemy->PrimaryFireTimer -= framelag;
	if( Enemy->PrimaryFireTimer < 0.0F )
		Enemy->PrimaryFireTimer = 0.0F;

	if( !(Enemy->Object.Flags & SHIP_Scattered) )
	{
		ApplyMatrix( &SObject->Mat, &Forward, &TempVector );
		ApplyMatrix( &SObject->Mat, &SlideUp, &TempUpVector );
		
		if( (Enemy->PrimaryFireTimer == 0.0F) && ( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )  )
		{
			Enemy->PrimaryFireTimer = EnemyTypes[Enemy->Type].PrimaryFireRate * 0.5F;
			
			Weapon = BodgePrimaryWeapon( EnemyTypes[Enemy->Type].PrimaryWeaponType, Enemy->PickupHeld );
		
			EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
				Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
				EnemyTypes[Enemy->Type].PowerLevel, (EnemyTypes[Enemy->Type].PowerLevel +1) * 33.0F, FALSE, NULL );
		}
	}
	
}





