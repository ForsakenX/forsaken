/*
 * a i f l e s h m o r p h . c
 *
 * AIFLESHMORPH
 *
 * Copyright (c) 1998 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 11 $
 *
 * $Header: /PcProjectX/Ai/AIFleshmorph.c 11    3/28/98 10:06a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIFleshmorph.c $
 * 
 * 11    3/28/98 10:06a Phillipd
 * 
 * 10    3/19/98 5:05p Phillipd
 * 
 * 9     3/09/98 4:19p Phillipd
 * 
 * 8     3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 7     2/26/98 4:50p Phillipd
 * 
 * 6     2/25/98 3:18p Phillipd
 * 
 * 5     2/25/98 10:50a Phillipd
 * 
 * 4     2/23/98 4:46p Phillipd
 * 
 * 3     21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 2     2/03/98 5:38p Phillipd
 * 
 * 1     2/03/98 5:36p Phillipd
 * 
 * 
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
extern	VECTOR	SlideLeft;
extern	VECTOR	SlideRight;
extern	PRIMARYWEAPONATTRIB	PrimaryWeaponAttribs[];
extern	SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[];
extern	PRIMARYWEAPONBULLET	PrimBulls[MAXPRIMARYWEAPONBULLETS];

// Globals
void AI_FLESHMORPH_RANDOMFIREBALL( register ENEMY * Enemy );

VECTOR	FleshmorphPos;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_FLESHMORPH_IDLE( register ENEMY * Enemy )
{
	OBJECT * TObject;


	Enemy->Object.Pos = FleshmorphPos;

	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if ( Enemy->Timer < 0.0F )
		{
			Enemy->Timer = 0.0F;
		}
		return;         
	}         
 	// Scan for target
	AI_THINK( Enemy , TRUE , TRUE );
	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		SetCurAnimSeq( FLESHMORPHSEQ_Stop, &Enemy->Object );
		SET_TARGET_PLAYERS;
		
		AI_GetDistToNearestTarget( Enemy );
		
		if( Tinfo->TFlags == _TARGET_PLAYERS )
		{
			TObject = Tinfo->TObject;
			if( AI_InViewCone( &Enemy->Object.Pos ,&Enemy->Object.Mat , &TObject->Pos, Enemy->Viewcone ) )
			{
				if( AI_ClearLOS( &Enemy->Object.Pos, Enemy->Object.Group , &TObject->Pos ) )
				{
					Enemy->AIFlags |= AI_ICANSEEPLAYER;
					Enemy->TShip = TObject;
					Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_MOVEFORWARDS;
					Enemy->Timer  =	ONE_SECOND * 16.0F;
					return;
				}
			}
		}
	}
	Enemy->Timer  =	RESET_IDLE_TIME;

}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph MoveForward
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_FLESHMORPH_MOVEFORWARDS( register ENEMY * Enemy )
{

	if( !Enemy->Object.Animating )
	{
		SetCurAnimSeq( FLESHMORPHSEQ_Pulsate, &Enemy->Object );
	}
	
	AI_FLESHMORPH_RANDOMFIREBALL( Enemy );
	if( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if( Enemy->Timer < 0.0F )
		{
			Enemy->Timer = 0.0F;
		}else{
			if( ( ( (int)(Enemy->Timer / ONE_SECOND ) ) & 3 ) >= 1 )
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
		}
		return;         
	}         
	Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_STOPFORWARD;
	Enemy->Timer  =	ONE_SECOND * 15.0F;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph StopForward
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_FLESHMORPH_STOPFORWARD( register ENEMY * Enemy )
{
//	OBJECT * TObject;
	OBJECT * SObject;
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	BYTE	Weapon;
	FIREPOS *	FirePosPnt;
	VECTOR	FireOffset;
	int i;

	if( !Enemy->Object.Animating )
	{
		SetCurAnimSeq( FLESHMORPHSEQ_Pulsate, &Enemy->Object );
	}

	SObject = &Enemy->Object;

	if ( !Enemy->Timer )
	{
		Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_MOVEBACKWARDS;
		Enemy->Timer  =	ONE_SECOND * 16.0F;
		return;
	}
	Enemy->Timer -= framelag;
	if ( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;
	// Is it time to think???
	Enemy->PrimaryFireTimer -= framelag;
	if( Enemy->PrimaryFireTimer < 0.0F )
		Enemy->PrimaryFireTimer = 0.0F;
	Enemy->SecondaryFireTimer -= framelag;
	if( Enemy->SecondaryFireTimer < 0.0F )
		Enemy->SecondaryFireTimer = 0.0F;

//	if( Enemy->AIFlags & ( AI_ICANSEEPLAYER + AI_ICANHEARPLAYER ) )
	{
//		if( !(Enemy->Object.Flags & SHIP_Scattered ) && (Enemy->AIFlags & AI_ICANSEEPLAYER) && !(Enemy->AIFlags & AI_FRIENDLYFIRE) )
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
				( EnemyTypes[Enemy->Type].SecondaryWeaponType != NO_SECONDARY )	)
			{
				Enemy->SecondaryFireTimer = EnemyTypes[Enemy->Type].SecondaryFireRate;
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
			}
		}
	}


	
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph MoveBackwards
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_FLESHMORPH_MOVEBACKWARDS( register ENEMY * Enemy )
{
	AI_FLESHMORPH_RANDOMFIREBALL( Enemy );
	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if ( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
		else{
			if( (((int) (Enemy->Timer / ONE_SECOND )) & 3) >= 1 )
				Enemy->AIMoveFlags |= AI_CONTROL_BACK;
			if( !Enemy->Object.Animating )
			{
				SetCurAnimSeq( FLESHMORPHSEQ_Pulsate, &Enemy->Object );
			}
		}
		return;         
	}         
	if( Enemy->Object.Animating )
	{
		return;
	}
	SetCurAnimSeq( FLESHMORPHSEQ_Attack, &Enemy->Object );
	Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_STOPBACKWARD;
	Enemy->Timer  =	ONE_SECOND * 5.0F;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph StopBackward
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_FLESHMORPH_STOPBACKWARD( register ENEMY * Enemy )
{
	if( !Enemy->Object.Animating )
	{
		SetCurAnimSeq( FLESHMORPHSEQ_Pulsate, &Enemy->Object );
	}

	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if ( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
		return;         
	}         

	Enemy->Object.Pos = FleshmorphPos;
	AI_THINK( Enemy , TRUE , TRUE );
	if( !(Enemy->AIFlags & AI_ANYPLAYERINRANGE) )
	{
		Enemy->Timer = RESET_IDLE_TIME;
		Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_IDLE;
		return;
	}
	Enemy->Object.AI_Mode = AIMODE_FLESHMORPH_MOVEFORWARDS;
	Enemy->Timer  =	ONE_SECOND * 16.0F;
}





/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fleshmorph Random Fireball move...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
VECTOR	FireDirs[4] = { { -0.7F , 0.0F , 0.3F },
						{ -0.3F , 0.0F , 0.7F },
						{  0.3F , 0.0F , 0.7F },
						{  0.7F , 0.0F , 0.3F }
};
VECTOR	FireOffs[4] = { { (-800.0F * GLOBAL_SCALE), (1400.0F * GLOBAL_SCALE), (600.0F * GLOBAL_SCALE) },
						{ (-800.0F * GLOBAL_SCALE), (1400.0F * GLOBAL_SCALE), (600.0F * GLOBAL_SCALE) },
						{ (-800.0F * GLOBAL_SCALE), (1400.0F * GLOBAL_SCALE), (600.0F * GLOBAL_SCALE) },
						{ (-800.0F * GLOBAL_SCALE), (1400.0F * GLOBAL_SCALE), (600.0F * GLOBAL_SCALE) }
};



void AI_FLESHMORPH_RANDOMFIREBALL( register ENEMY * Enemy )
{
	VECTOR		TempVector;
	VECTOR		TempUpVector = { 0.0F, 1.0F, 0.0F };
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };
	VECTOR		FireOffset = { (-400.0F * GLOBAL_SCALE), (1200.0F * GLOBAL_SCALE), (600.0F * GLOBAL_SCALE) };
	VECTOR		Dir;
	int8		Weapon;
	int i , Num;

	Enemy->SecondaryFireTimer -= framelag;
	if( Enemy->SecondaryFireTimer < 0.0F )
		Enemy->SecondaryFireTimer = 0.0F;

	if( Enemy->SecondaryFireTimer == 0.0F)
	{
		Enemy->SecondaryFireTimer = (ONE_SECOND * 1.5F) + Random_Range_Float(ONE_SECOND * 3.0F);

		if( Random_Range( 20 ) > 5 )
		{
			Weapon = ENEMYBLUEHOMINGMISSILE;
			Num = 4;
		}else{
			Weapon = ENEMYFIREBALL;
			Num = 9;
			Dir.x = Ships[WhoIAm].Object.Pos.x - Enemy->Object.Pos.x;
			Dir.y = Ships[WhoIAm].Object.Pos.y - Enemy->Object.Pos.y;
			Dir.z = Ships[WhoIAm].Object.Pos.z - Enemy->Object.Pos.z;
			NormaliseVector( &Dir );
		}

		for( i = 0 ; i < Num ; i++ )
		{
			if( Weapon == ENEMYBLUEHOMINGMISSILE )
			{
				TempVector = FireDirs[i];
				FireOffset = FireOffs[i];
			}else{
				TempVector.x = Dir.x + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		TempVector.y = Dir.y + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		TempVector.z = Dir.z + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
				NormaliseVector( &TempVector );
			}

			InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
				&Enemy->Object.Pos, &FireOffset, &TempVector, &TempUpVector,
				&TempOffset, Weapon, FALSE );
		}
	}
	
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

