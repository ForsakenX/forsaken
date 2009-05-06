
#include	<stdio.h>
#include <math.h>
#include <malloc.h>
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "Node.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
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
extern	int		Exogenon_Num_StartPos;
extern	VECTOR	Exogenon_StartPos[6];
void ExogenonAim( ENEMY * Enemy );


VECTOR	ExogenonSweepAngle;
uint32	ExogenonSweepDir;
void	ExogenonFireLeftRight( ENEMY * Enemy );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_IDLE( register ENEMY * Enemy )
{
	int i;
	int startpos = 0;
	float	Distance = 0.0F;
	AI_THINK( Enemy , TRUE , TRUE );

	if( Enemy->Object.CurAnimSeq != EXOGENONSEQ_Stop_Up )
	{
		SetCurAnimSeq( EXOGENONSEQ_Stop_Up, &Enemy->Object );
		return;
	}
	
	Enemy->Object.Pos = Exogenon_StartPos[0];
	Enemy->Object.Pos.y += 256.0F * 10.0F;
	if( !(Enemy->AIFlags & AI_ANYPLAYERINRANGE) )
		return;

	if( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if( Enemy->Timer <= 0.0F )
		{
			Enemy->Timer = 0.0F;
		}
		return;
	}

	startpos = Random_Range( (uint16) Exogenon_Num_StartPos);
	i = Exogenon_Num_StartPos;

	while( Distance < (SHIP_RADIUS * 3.0F ) && i >= 0 )
	{
		Enemy->Object.Pos = Exogenon_StartPos[startpos];
		startpos++;
		if( startpos >= Exogenon_Num_StartPos )
			startpos = 0;
		Distance = DistanceVector2Vector( &Enemy->Object.Pos,&Ships[WhoIAm].Object.Pos );
		i--;
	}

	
	SetCurAnimSeq( EXOGENONSEQ_Move_Down, &Enemy->Object );
	Enemy->Object.AI_Mode = AIMODE_EXOGENON_MOVEDOWN;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Move Down
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_MOVEDOWN( register ENEMY * Enemy )
{
	VECTOR	TempUpVector;
	VECTOR	FireDir;

	BOOL	ClearLos = TRUE;
	VECTOR	TempOffset = { 0.0F, 0.0F, 180.0F };
	ExogenonAim( Enemy );

	Enemy->Object.AnimSpeed = 1.5F;
	if( Enemy->Object.Animating )
	{
		return;
	}

	if( !(ClearLos = AI_ClearLOSNonZeroNonObject( &Enemy->Object.Pos,Enemy->Object.Group, &Ships[WhoIAm].Object.Pos , SHIP_RADIUS))
		|| (Random_Range(10) > 5 ) )
	{
		Enemy->Object.AI_Mode = AIMODE_EXOGENON_MOVEUP;
		SetCurAnimSeq( EXOGENONSEQ_Move_Up, &Enemy->Object );

		if( ClearLos )
		{
			ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );
			ApplyMatrix( &Enemy->Object.Mat, &Forward, &FireDir );
			// Fire a homing missile Forward...
			InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
				&Enemy->Object.Pos, &TempOffset, &FireDir, &TempUpVector,
				&TempOffset, ENEMYBLUEHOMINGMISSILE, FALSE );
		}
		ExogenonFireLeftRight( Enemy );
	}else{
		Enemy->Object.AI_Mode = AIMODE_EXOGENON_SCAN;
		SetCurAnimSeq( EXOGENONSEQ_Idle, &Enemy->Object );
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Scan
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_SCAN( register ENEMY * Enemy )
{
	ExogenonAim( Enemy );

	Enemy->Object.AnimSpeed = 2.0F;
	if( Enemy->Object.Animating )
	{
		return;
	}
	if( !AI_ClearLOS( &Enemy->Object.Pos,Enemy->Object.Group, &Ships[WhoIAm].Object.Pos ) )
	{
		Enemy->Object.AI_Mode = AIMODE_EXOGENON_MOVEUP;
		SetCurAnimSeq( EXOGENONSEQ_Move_Up, &Enemy->Object );
		ExogenonFireLeftRight( Enemy );
		return;
	}
	
	Enemy->Object.AI_Mode = AIMODE_EXOGENON_FIRE;
	SetCurAnimSeq( EXOGENONSEQ_Fire, &Enemy->Object );
	Enemy->Timer = 60.0F * 2.5F;

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Fire
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_FIRE( register ENEMY * Enemy )
{
	VECTOR		TempVector;
	VECTOR		TempUpVector;
	VECTOR		TempOffset = { 0.0F, 0.0F, 180.0F };
	float		Distance;
	

	Enemy->Object.AnimSpeed = 1.0F;
	
	if( Enemy->Timer )
	{
		Enemy->Timer -= framelag;

		if( Enemy->Timer <= 0.0F )
		{
			// This is where the main Weapon would fire....
			Enemy->Timer = 0.0F;

			if( EnemyTypes[Enemy->Type].PrimaryWeaponType != NO_PRIMARY )
			{
				ApplyMatrix( &Enemy->Object.Mat, &Forward, &TempVector );
				Distance = DistanceVector2Vector( &Ships[WhoIAm].Object.Pos ,&Enemy->Object.Pos );

				TempVector.x *= Distance;
				TempVector.y *= Distance;
				TempVector.z *= Distance;
				TempVector.y -= Enemy->Object.Pos.y - Ships[WhoIAm].Object.Pos.y;
				NormaliseVector(&TempVector);

				ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );
				
				EnemyFirePrimary( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, EnemyTypes[Enemy->Type].PrimaryWeaponType,
					Enemy->Object.Group, &Enemy->Object.Pos, &TempOffset, &TempVector, &TempUpVector,
					EnemyTypes[Enemy->Type].PowerLevel, (EnemyTypes[Enemy->Type].PowerLevel +1) * 33.0F, FALSE, NULL );
			}

			Enemy->Object.AI_Mode = AIMODE_EXOGENON_SWEEP;
			ExogenonAim( Enemy );
			ExogenonSweepDir = Enemy->AIMoveFlags;
			ExogenonSweepAngle = Enemy->AI_Angle;
			Enemy->Timer = 60.0F * 1.0F;
		}
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Move Up
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_MOVEUP( register ENEMY * Enemy )
{
	Enemy->Object.AnimSpeed = 2.0F;
	if( Enemy->Object.Animating )
	{
		return;
	}
	
	Enemy->Object.AI_Mode = AIMODE_EXOGENON_IDLE;
	Enemy->Timer = 60.0F * ( 0.5F + Random_Range_Float( 0.5F ) );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Aim
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ExogenonAim( ENEMY * Enemy )
{
	AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &Ships[WhoIAm].Object.Pos );
	Enemy->AIMoveFlags |= AimData.Flags;
	Enemy->AI_Angle = AimData.Angle;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Sweep
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_EXOGENON_SWEEP( register ENEMY * Enemy )
{
	if( Enemy->Timer )
	{
		Enemy->Timer -= framelag;

		if( Enemy->Timer <= 0.0F )
		{
			Enemy->Timer = 0.0F;
		}
		Enemy->AIMoveFlags = ExogenonSweepDir;
		Enemy->AI_Angle = ExogenonSweepAngle;
		ExogenonAim( Enemy );

	}
	
	if( Enemy->Object.Animating )
	{
		return;
	}
	Enemy->Object.AI_Mode = AIMODE_EXOGENON_MOVEUP;
	SetCurAnimSeq( EXOGENONSEQ_Move_Up, &Enemy->Object );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Exogenon Sweep
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	ExogenonFireLeftRight( ENEMY * Enemy )
{
	VECTOR	FireDir;
	VECTOR	TempUpVector;
	VECTOR	TempOffset;

	ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempUpVector );

	ApplyMatrix( &Enemy->Object.Mat, &SlideLeft, &FireDir );
	// Fire a homing missile left
	TempOffset.x = FireDir.x * ( 1200.0F * GLOBAL_SCALE );
	TempOffset.y = FireDir.y * ( 1200.0F * GLOBAL_SCALE );
	TempOffset.z = FireDir.z * ( 1200.0F * GLOBAL_SCALE );

	InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
		&Enemy->Object.Pos, &TempOffset, &FireDir, &TempUpVector,
		&TempOffset, ENEMYBLUEHOMINGMISSILE, FALSE );

	ApplyMatrix( &Enemy->Object.Mat, &SlideRight, &FireDir );
	TempOffset.x = FireDir.x * ( 1200.0F * GLOBAL_SCALE );
	TempOffset.y = FireDir.y * ( 1200.0F * GLOBAL_SCALE );
	TempOffset.z = FireDir.z * ( 1200.0F * GLOBAL_SCALE );
	// Fire a homing missile right
	InitOneSecBull( OWNER_ENEMY, Enemy->Index, ++Enemy->BulletID, Enemy->Object.Group,
		&Enemy->Object.Pos, &TempOffset, &FireDir, &TempUpVector,
		&TempOffset, ENEMYBLUEHOMINGMISSILE, FALSE );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

