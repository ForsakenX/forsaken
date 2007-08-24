/*
 * a i d o g f i g h t . c
 *
 * AIMODE_DOGFIGHT
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 35 $
 *
 * $Header: /PcProjectX/Ai/AIDogfight.c 35    3/28/98 10:06a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIDogfight.c $
 * 
 * 35    3/28/98 10:06a Phillipd
 * 
 * 34    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 33    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 32    2/03/98 4:29p Phillipd
 * Dans modifications to enemies. ( CHECKED IN BY DAVE )
 * 
 * 31    2/02/98 10:20a Phillipd
 * 
 * 30    1/31/98 3:24p Phillipd
 * 
 * 29    1/27/98 2:58p Phillipd
 * 
 * 28    1/24/98 5:10p Phillipd
 * 
 * 27    1/23/98 9:57a Phillipd
 * 
 * 26    1/22/98 12:06p Phillipd
 * 
 * 25    5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 24    26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 23    11/21/97 10:56a Phillipd
 * Max Kills to end a level....
 * 
 * 22    11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 21    11/15/97 1:09p Phillipd
 * 
 * 20    11/12/97 5:00p Phillipd
 * 
 * 19    11/11/97 11:00a Phillipd
 * 
 * 18    10/11/97 19:16 Collinsd
 * Added Bodge to fix mappers fuckup.
 * 
 * 17    10/24/97 5:05p Phillipd
 * Enemies version 2....
 * 
 * 16    10/21/97 5:07p Phillipd
 * 
 * 15    9/16/97 2:19p Phillipd
 * 
 * 14    9/12/97 11:12a Phillipd
 * 
 * 13    9/10/97 9:54a Phillipd
 * 
 * 12    9/03/97 3:50p Phillipd
 * 
 * 11    9/02/97 11:30a Phillipd
 * 
 * 10    9/02/97 8:52a Phillipd
 * 
 * 9     9/01/97 12:17p Phillipd
 * 
 * 8     8/31/97 3:35p Phillipd
 * 
 * 7     8/31/97 1:05p Phillipd
 * 
 * 6     8/29/97 12:27p Phillipd
 * 
 * 5     8/27/97 3:42p Phillipd
 * 
 * 4     8/27/97 9:20a Phillipd
 * 
 * 3     8/22/97 10:16a Phillipd
 * 
 * 2     8/19/97 12:59p Phillipd
 * 
 * 1     8/18/97 10:32a Phillipd
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
extern	FIREPOS	BigGeekFirePos2;
extern	FIREPOS	BigGeekFirePos3;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR Dogfighting...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_DOGFIGHT( register ENEMY * Enemy )
{
	float	Dist;
	OBJECT * TObject;
	OBJECT * SObject;
	VECTOR		NewPos;
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	float	MinRange;
	float	MaxRange;
	float	Range;
	BYTE	Weapon;
	FIREPOS *	FirePosPnt;
	VECTOR	FireOffset;
	int i;

	SObject = &Enemy->Object;

	// Is it time to think???

	AI_THINK( Enemy , FALSE , FALSE);

	TObject = (OBJECT*) Enemy->TShip;

	if( !TObject )
	{
		AI_SetFOLLOWPATH( Enemy );
		return;
	}
	
	Enemy->Timer -= framelag;
	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;

	Enemy->PrimaryFireTimer -= framelag;
	if( Enemy->PrimaryFireTimer < 0.0F )
		Enemy->PrimaryFireTimer = 0.0F;
	Enemy->SecondaryFireTimer -= framelag;
	if( Enemy->SecondaryFireTimer < 0.0F )
		Enemy->SecondaryFireTimer = 0.0F;

	//------------------------------------------------------------------------------
	// Aim at target
	//------------------------------------------------------------------------------
	if( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )
	{
		AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move , &Enemy->Object.Pos , TObject , &NewPos ,   PrimaryWeaponAttribs[EnemyTypes[Enemy->Type].PrimaryWeaponType].Speed[0] );
	}else{
		NewPos = TObject->Pos;
	}
	AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &NewPos );


	
	if( Enemy->AIFlags & ( AI_ICANSEEPLAYER + AI_ICANHEARPLAYER ) )
	{
		Enemy->TNode = NULL;
		if( !(Enemy->Object.Flags & SHIP_Scattered ) && (Enemy->AIFlags & AI_ICANSEEPLAYER) && !(Enemy->AIFlags & AI_FRIENDLYFIRE) )
		{
			ApplyMatrix( &SObject->Mat, &Forward, &TempVector );
			ApplyMatrix( &SObject->Mat, &SlideUp, &TempUpVector );
			
			if( (Enemy->PrimaryFireTimer == 0.0F) && ( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )  )
			{
				Enemy->PrimaryFireTimer = EnemyTypes[Enemy->Type].PrimaryFireRate + (float) Random_Range( (uint16) EnemyTypes[Enemy->Type].PrimaryFireRate );
				
				Weapon = BodgePrimaryWeapon( EnemyTypes[Enemy->Type].PrimaryWeaponType, Enemy->PickupHeld );
				
				EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
					Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
					EnemyTypes[Enemy->Type].PowerLevel, (EnemyTypes[Enemy->Type].PowerLevel +1) * 33.0F, FALSE, NULL );
			}

			if( (Enemy->SecondaryFireTimer == 0.0F) &&
				( EnemyTypes[Enemy->Type].SecondaryWeaponType != NO_SECONDARY )	&&
				( !Enemy->Object.Animating || Enemy->Type != ENEMY_Boss_LittleGeek) )
			{

				if( ( Enemy->Type != ENEMY_Boss_BigGeek ) || ( AimData.Angle.y < 45.0F && AimData.Angle.y > -45.0F ) )
				{
					Enemy->SecondaryFireTimer = EnemyTypes[Enemy->Type].SecondaryFireRate + (float) Random_Range( (uint16) EnemyTypes[Enemy->Type].SecondaryFireRate );
					
					FirePosPnt = EnemyTypes[Enemy->Type].FirePoints;
					ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );

					if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_RANDSECONDARYFIREPOS )
					{
						i = Random_Range( FirePosPnt->NumPoints );
						ApplyMatrix( &Enemy->Object.Mat, &FirePosPnt->Dir[i], &TempVector );
						ApplyMatrix( &Enemy->Object.Mat , &FirePosPnt->Points[i] , &FireOffset );
						InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
							&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
							&TempOffset, EnemyTypes[Enemy->Type].SecondaryWeaponType, FALSE );
					}else{
						for( i = 0 ; i < FirePosPnt->NumPoints ; i++ )
						{
							ApplyMatrix( &Enemy->Object.Mat, &FirePosPnt->Dir[i], &TempVector );
							ApplyMatrix( &Enemy->Object.Mat , &FirePosPnt->Points[i] , &FireOffset );
							
							InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
								&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
								&TempOffset, EnemyTypes[Enemy->Type].SecondaryWeaponType, FALSE );
						}
					}
				}else{
					// Big Geek Should Fire Depth Charges.....
					Enemy->SecondaryFireTimer = EnemyTypes[Enemy->Type].SecondaryFireRate;
				
					if( AimData.Angle.y > 0.0F )
						FirePosPnt = &BigGeekFirePos3;
					else
						FirePosPnt = &BigGeekFirePos2;
	
					ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );

					i = Random_Range( FirePosPnt->NumPoints );
					ApplyMatrix( &Enemy->Object.Mat, &FirePosPnt->Dir[i], &TempVector );
					ApplyMatrix( &Enemy->Object.Mat , &FirePosPnt->Points[i] , &FireOffset );
					InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
						&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
						&TempOffset, ENEMYDEPTHCHARGE, FALSE );
				
				}
			}
		}
	}else{
		AI_SetMOVETOTARGET( Enemy );
		return;
	}
	

	
	if( (Enemy->Object.Flags & SHIP_Scattered ) || ((EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_RETREAT) && ( Enemy->Object.Shield <= (EnemyTypes[Enemy->Type].Shield * 0.15 ) ) ) )
	{
		// we should get out of hear..
		AI_SetRETREAT( Enemy );
		return;
	}
	
	AI_UPDATEGUNS( Enemy );
	
	
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

	
	if( Enemy->AvoidTimer )
	{
		if( Enemy->AIFlags & AI_MINEAVOID )
		{
			Enemy->AvoidTimer = 0.0F;
			Enemy->AvoidType = 0;
			AI_SetKILLMINE( Enemy );
			return;
		}else{
			Enemy->AIMoveFlags = Enemy->AvoidType;
		}
	}else{
		MinRange = EnemyTypes[Enemy->Type].MinRange;
		MaxRange = EnemyTypes[Enemy->Type].MaxRange;
		Range = MaxRange - MinRange;
		
		Dist = DistanceVector2Vector( &Enemy->Object.Pos , &TObject->Pos );

		if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_WEAPONKNOWLEDGE ) && ( Enemy->AIFlags & AI_PLAYERHASFIREDPRIMARY ) && PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].MinDistance )
		{
			if( MinRange > PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].MinDistance )
			{
				MaxRange = PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].MinDistance;
				MinRange = MaxRange - Range;
				if( MinRange < SHIP_RADIUS )
					MinRange = SHIP_RADIUS;
			}
			else if( MinRange < PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].MinDistance )
			{
				MinRange = PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].MinDistance;
				MaxRange = MinRange + Range;
			}
		}
		if ( Dist < MinRange )
		{
			Enemy->AIMoveFlags |= AI_CONTROL_BACK;
		}
		else if ( Dist > MaxRange )
		{
			Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
		}
		else if( (AimData.Angle.x < -15.0F) || (AimData.Angle.x > 15.0F) ||
			(AimData.Angle.y < -15.0F) || (AimData.Angle.y > 15.0F) )
			Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
	}
	Enemy->AI_Angle = AimData.Angle;
	
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif


