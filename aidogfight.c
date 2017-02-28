
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
extern	FIREPOS	BigGeekFirePos2;
extern	FIREPOS	BigGeekFirePos3;
/*===================================================================
	Procedure	:	AIR Dogfighting...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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
				Enemy->PrimaryFireTimer = EnemyTypes[Enemy->Type].PrimaryFireRate + (float) Random_Range( (u_int16_t) EnemyTypes[Enemy->Type].PrimaryFireRate );
				
				Weapon = BodgePrimaryWeapon( EnemyTypes[Enemy->Type].PrimaryWeaponType, Enemy->PickupHeld );
				
				EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Weapon,
					Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
					EnemyTypes[Enemy->Type].PowerLevel, (EnemyTypes[Enemy->Type].PowerLevel +1) * 33.0F, false, NULL );
			}

			if( (Enemy->SecondaryFireTimer == 0.0F) &&
				( EnemyTypes[Enemy->Type].SecondaryWeaponType != NO_SECONDARY )	&&
				( !Enemy->Object.Animating || Enemy->Type != ENEMY_Boss_LittleGeek) )
			{

				if( ( Enemy->Type != ENEMY_Boss_BigGeek ) || ( AimData.Angle.y < 45.0F && AimData.Angle.y > -45.0F ) )
				{
					Enemy->SecondaryFireTimer = EnemyTypes[Enemy->Type].SecondaryFireRate + (float) Random_Range( (u_int16_t) EnemyTypes[Enemy->Type].SecondaryFireRate );
					
					FirePosPnt = EnemyTypes[Enemy->Type].FirePoints;
					ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );

					if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_RANDSECONDARYFIREPOS )
					{
						i = Random_Range( FirePosPnt->NumPoints );
						ApplyMatrix( &Enemy->Object.Mat, &FirePosPnt->Dir[i], &TempVector );
						ApplyMatrix( &Enemy->Object.Mat , &FirePosPnt->Points[i] , &FireOffset );
						InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
							&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
							&TempOffset, EnemyTypes[Enemy->Type].SecondaryWeaponType, false );
					}else{
						for( i = 0 ; i < FirePosPnt->NumPoints ; i++ )
						{
							ApplyMatrix( &Enemy->Object.Mat, &FirePosPnt->Dir[i], &TempVector );
							ApplyMatrix( &Enemy->Object.Mat , &FirePosPnt->Points[i] , &FireOffset );
							
							InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
								&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
								&TempOffset, EnemyTypes[Enemy->Type].SecondaryWeaponType, false );
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
						&TempOffset, ENEMYDEPTHCHARGE, false );
				
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


