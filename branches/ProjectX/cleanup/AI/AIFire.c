/*
 * a i m o v e t . c
 *
 * AIMODE_FIREATTARGET
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 46 $
 *
 * $Header: /PcProjectX/Ai/AIFire.c 46    3/28/98 10:06a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIFire.c $
 * 
 * 46    3/28/98 10:06a Phillipd
 * 
 * 45    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 44    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 43    1/28/98 5:15p Phillipd
 * 
 * 42    1/28/98 9:49a Phillipd
 * 
 * 41    1/13/98 3:06p Phillipd
 * Turret bug fixes......
 * 
 * 40    1/09/98 2:15p Phillipd
 * 
 * 39    1/02/98 11:13a Phillipd
 * 
 * 38    12/11/97 4:04p Phillipd
 * 
 * 37    5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 36    12/03/97 4:37p Phillipd
 * 
 * 35    11/29/97 4:36p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 34    11/27/97 12:25p Phillipd
 * 
 * 33    26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 32    11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 31    11/12/97 5:00p Phillipd
 * 
 * 30    11/11/97 11:00a Phillipd
 * 
 * 29    10/11/97 19:16 Collinsd
 * Added Bodge to fix mappers fuckup.
 * 
 * 28    7/11/97 14:51 Collinsd
 * Fixed enemy gun fire points.
 * 
 * 27    11/03/97 6:39p Phillipd
 * 
 * 26    10/21/97 5:07p Phillipd
 * 
 * 25    9/10/97 9:54a Phillipd
 * 
 * 24    9/01/97 12:17p Phillipd
 * 
 * 23    8/31/97 3:35p Phillipd
 * 
 * 22    31/08/97 13:24 Collinsd
 * Added aiming pos, firing pos and laser works with enemies now.
 * 
 * 21    8/29/97 12:27p Phillipd
 * 
 * 20    27/08/97 15:02 Collinsd
 * 
 * 19    8/17/97 2:58p Phillipd
 * 
 * 18    8/17/97 1:33p Phillipd
 * 
 * 17    8/17/97 1:17p Phillipd
 * Enemies coming along....
 * 
 * 16    8/15/97 4:24p Phillipd
 * enemies coming along
 * 
 * 15    24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 14    21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 13    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 12    7/14/97 5:07p Phillipd
 * 
 * 11    7/12/97 3:34p Phillipd
 * 
 * 10    10/07/97 17:42 Collinsd
 * 
 * 9     7/10/97 12:05p Phillipd
 * 
 * 8     7/10/97 11:35a Phillipd
 * 
 * 7     7/09/97 9:23a Phillipd
 * 
 * 6     8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 4     8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 3     7/08/97 12:51p Phillipd
 * 
 * 2     7/07/97 2:12p Phillipd
 * More Ai...Gun Stuff
 * 
 * 1     7/07/97 10:52a Phillipd
 */
#include	<stdio.h>
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
#include "compobjects.h"
#include "enemies.h"
#include "AI.h"
#include "Primary.h"
#include "Secondary.h"

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
extern	PRIMARYWEAPONBULLET	PrimBulls[MAXPRIMARYWEAPONBULLETS];

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	TURRET Fire At Target..
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_TURRET_FIREATTARGET( register ENEMY * Enemy )
{
	OBJECT * TObject;

	TObject = (OBJECT*) Enemy->TShip;


	if( (Enemy->Type != ENEMY_MissileTurret) )
	{
		if( Enemy->Object.Animating && !(Enemy->Object.CurAnimSeq == TURRETSEQ_Fire) )
		{
			return;
		}
	}else{
		if( Enemy->Object.Animating && (Enemy->Object.CurAnimSeq == TURRETSEQ_Opening ) )
		{

		}else{

			if( !Enemy->Object.Animating )
			{
				SetCurAnimSeq( TURRETSEQ_Open, &Enemy->Object );
			}
		}
	}
	
	
	
	AI_THINK( Enemy , TRUE , TRUE );


	// Is it time to validate target ?
	if ( Enemy->Timer == 0.0F )
	{
		if( !( Enemy->AIFlags & AI_ANYPLAYERINRANGE ) || !AI_ClearLOS( &Enemy->Object.Pos ,Enemy->Object.Group , &TObject->Pos ) )
		{
			Enemy->AIFlags &= ~AI_ICANSEEPLAYER;
			SetCurAnimSeq( TURRETSEQ_Closing, &Enemy->Object );
			AI_SetSCAN( Enemy );
			return;
		}
		Enemy->Timer  =	RESET_VALIDATE_TIME + (float) Random_Range( (uint16) RESET_VALIDATE_TIME );

	}else{
		Enemy->Timer -= framelag;
		if( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
	}

	Enemy->AIFlags |= AI_ICANSEEPLAYER;
	AI_UPDATEGUNS( Enemy );
}



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Update an Enemies Guns...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_UPDATEGUNS( register ENEMY * Enemy )
{
	OBJECT * TObject;
	GUNOBJECT * GObject;
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	VECTOR		NewPos;
	VECTOR		FireOffset;
	FIREPOS *	FirePosPnt;
	VECTOR		AimOffset;
	uint16		i;
	BYTE		Weapon;

	TObject = (OBJECT*) Enemy->TShip;

	if( !TObject || TObject->Mode != NORMAL_MODE )
		return;
	
	GObject = Enemy->Object.FirstGun;

	
	while( GObject )
	{
		
//		if( TObject && ( (Enemy->AIFlags & AI_ICANSEEPLAYER) || (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_TURRET_AI) || (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_SPLINE) ) )
		if( TObject && ( (Enemy->AIFlags & AI_ICANSEEPLAYER) || (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_SPLINE) ) )
		{
			FirePosPnt = EnemyTypes[Enemy->Type].GunFirePoints[GObject->GunNum>>1];
			ApplyMatrix( &GObject->Mat , &FirePosPnt->Points[GObject->FirePosCount] , &FireOffset );
			FireOffset.x += GObject->FirePos.x;
			FireOffset.y += GObject->FirePos.y;
			FireOffset.z += GObject->FirePos.z;
			FireOffset.x -= Enemy->Object.Pos.x;
			FireOffset.y -= Enemy->Object.Pos.y;
			FireOffset.z -= Enemy->Object.Pos.z;
			
			ApplyMatrix( &GObject->Mat, EnemyTypes[ Enemy->Type ].GunAimPos[GObject->GunNum>>1], &AimOffset );
			AimOffset.x += Enemy->Object.Pos.x;
			AimOffset.y += Enemy->Object.Pos.y;
			AimOffset.z += Enemy->Object.Pos.z;
			
			if( !GunTypes[GObject->Type].BurstMasterCount )
			{
				// Normal gun that just fires when it can..
				
				if( GunTypes[GObject->Type].PrimarySecondary )
				{
					AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , SecondaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed);
				}else{
					AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , PrimaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed[GunTypes[GObject->Type].PowerLevel]);
				}
				
				
				AI_AimAtTarget( &GObject->InvMat , &AimOffset, &NewPos );
				GObject->AIMoveFlags |= AimData.Flags;
				GObject->AI_Angle = AimData.Angle;

				if( GObject->Type == GUN_MetaTankMain )
				{
					if( (GObject->ReloadTime <= (0.5F*60.0F) ) && !Enemy->Object.Animating )
					{
						SetCurAnimSeq( 0, &Enemy->Object );
					}
				}
				if( GObject->ReloadTime <= 0.0F && ( (( ( GObject->AI_Angle.x < GunTypes[GObject->Type].BurstAngle && GObject->AI_Angle.x > -GunTypes[GObject->Type].BurstAngle )
					&& ( GObject->AI_Angle.y < GunTypes[GObject->Type].BurstAngle && GObject->AI_Angle.y > -GunTypes[GObject->Type].BurstAngle ) )  ) || GObject->Type == GUN_MetaTankMain ) )
				{
					ApplyMatrix( &GObject->Mat, &Forward, &TempVector );
					ApplyMatrix( &GObject->Mat, &SlideUp, &TempUpVector );
					
					if( !(Enemy->Object.Flags & SHIP_Scattered ) && (!GunTypes[GObject->Type].Range || ( DistanceVector2Vector( &TObject->Pos , &Enemy->Object.Pos) < GunTypes[GObject->Type].Range ) ))
					{

						if(Enemy->Object.ControlType == ENEMY_CONTROLTYPE_TURRET_AI)
						{
							if( !Enemy->Object.Animating && (Enemy->Type != ENEMY_MissileTurret) )
							{
								SetCurAnimSeq( TURRETSEQ_Fire, &Enemy->Object );
							}
						}
						
						if( GunTypes[GObject->Type].PrimarySecondary )
						{
							InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
								&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
								&TempOffset, GunTypes[GObject->Type].WeaponType, FALSE );
							
						}else{
							Weapon = BodgePrimaryWeapon( GunTypes[GObject->Type].WeaponType, Enemy->PickupHeld );

							i = EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
										Enemy->Object.Group, &Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
										GunTypes[GObject->Type].PowerLevel, (GunTypes[GObject->Type].PowerLevel +1) * 33.0F, FALSE, GObject );
							if( i != (uint16) -1 )
							{
								PrimBulls[i].FirePoint = GObject->FirePosCount;
							}
						}
						GObject->FirePosCount++;
						if( GObject->FirePosCount >= FirePosPnt->NumPoints )
							GObject->FirePosCount = 0;
						
						
						GObject->ReloadTime =GunTypes[GObject->Type].ReloadTime;
					}
				}
			}else{
				// A Gun That Does a Burst...
				if( GObject->BurstCount )
				{
					
					if( GunTypes[GObject->Type].PrimarySecondary )
					{
						AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , SecondaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed);
					}else{
						AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , PrimaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed[GunTypes[GObject->Type].PowerLevel]);
					}
					AI_AimAtTarget( &GObject->InvMat , &AimOffset, &NewPos );
					GObject->AIMoveFlags |= AimData.Flags;
					GObject->AIMoveFlags &= ~(AI_CONTROL_TURNLEFT + AI_CONTROL_TURNRIGHT );
					GObject->AI_Angle = AimData.Angle;
					if( GObject->BurstStartSign	< 0.0F )
					{
						GObject->AIMoveFlags |= AI_CONTROL_TURNLEFT;
					}else{
						GObject->AIMoveFlags |= AI_CONTROL_TURNRIGHT;
					}
					if( !(Enemy->Object.Flags & SHIP_Scattered ) &&GObject->ReloadTime <= 0.0F )
					{
						ApplyMatrix( &GObject->Mat, &Forward, &TempVector );
						ApplyMatrix( &GObject->Mat, &SlideUp, &TempUpVector );
						
						if(Enemy->Object.ControlType == ENEMY_CONTROLTYPE_TURRET_AI)
						{
							if( !Enemy->Object.Animating && (Enemy->Type != ENEMY_MissileTurret) )
							{
								SetCurAnimSeq( TURRETSEQ_Fire, &Enemy->Object );
							}
						}

						if( GunTypes[GObject->Type].PrimarySecondary )
						{
							InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
								&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
								&TempOffset, GunTypes[GObject->Type].WeaponType, FALSE );
							
						}else{
							Weapon = BodgePrimaryWeapon( GunTypes[GObject->Type].WeaponType, Enemy->PickupHeld );

							i = EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
										Enemy->Object.Group, &Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
										GunTypes[GObject->Type].PowerLevel, (GunTypes[GObject->Type].PowerLevel +1) * 33.0F, FALSE, GObject );
							if( i != (uint16) -1 )
							{
								PrimBulls[i].FirePoint = GObject->FirePosCount;
							}
						}
						
						GObject->FirePosCount++;
						if( GObject->FirePosCount >= FirePosPnt->NumPoints )
							GObject->FirePosCount = 0;
						
						GObject->ReloadTime = GunTypes[GObject->Type].ReloadTime;
						GObject->BurstCount--;
					}
				}
				
				if( !GObject->BurstCount && GunTypes[GObject->Type].BurstMasterCount )
				{
					
					if( GunTypes[GObject->Type].PrimarySecondary )
					{
						AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , SecondaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed);
					}else{
						AI_LookAhead( EnemyTypes[Enemy->Type].Behave.Anticipate_Move ,&AimOffset, TObject , &NewPos , PrimaryWeaponAttribs[GunTypes[GObject->Type].WeaponType].Speed[GunTypes[GObject->Type].PowerLevel]);
					}
					
					AI_AimAtTarget( &GObject->InvMat , &AimOffset, &NewPos );
					GObject->AIMoveFlags |= AimData.Flags;
					GObject->AI_Angle = AimData.Angle;
					if( GObject->BurstTime == 0.0F )
					{
						if( (GObject->AI_Angle.y <= GunTypes[GObject->Type].BurstAngle) && (GObject->AI_Angle.y >= -GunTypes[GObject->Type].BurstAngle) )
						{
							GObject->BurstCount = GunTypes[GObject->Type].BurstMasterCount;
							GObject->BurstTime = GunTypes[GObject->Type].BurstMasterTime;
							GObject->BurstStartSign = GObject->AI_Angle.y;
						}
					}
				}
				
			}
		}
		GObject = GObject->Next;
	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

