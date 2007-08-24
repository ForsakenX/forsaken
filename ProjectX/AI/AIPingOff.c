/*
 * a i p i n g o f f . c
 *
 * AIMODE_DEATH_PINGOFF
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 10 $
 *
 * $Header: /PcProjectX/Ai/AIPingOff.c 10    3/05/98 9:29a Phillipd $
 *
 * $Log: /PcProjectX/Ai/AIPingOff.c $
 * 
 * 10    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 9     21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 8     11/26/97 10:44a Phillipd
 * 
 * 7     11/25/97 4:13p Phillipd
 * 
 * 6     11/25/97 3:13p Phillipd
 * 
 * 5     11/25/97 2:37p Phillipd
 * 
 * 4     11/24/97 3:43p Phillipd
 * 
 * 3     11/22/97 2:11p Phillipd
 * 
 * 2     11/22/97 1:23p Phillipd
 * 
 * 1     11/22/97 1:23p Phillipd
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
extern	MLOADHEADER Mloadheader;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR Ping Off...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_DEATH_PINGOFF( register ENEMY * Enemy )
{
	OBJECT * SObject;
	VECTOR		NewDir = { 0.0F, 1.0F, 0.0F };

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
	
	Enemy->Object.ExternalForce.x += (Enemy->LastDamageDir.x * (EnemyTypes[Enemy->Type].MoveRateAccell * Enemy->LastDamageSpeed) * framelag) * ( ( 1.0F/PINGOFFTIME )*Enemy->Timer);
	Enemy->Object.ExternalForce.y += (Enemy->LastDamageDir.y * (EnemyTypes[Enemy->Type].MoveRateAccell * Enemy->LastDamageSpeed) * framelag) * ( ( 1.0F/PINGOFFTIME )*Enemy->Timer);
	Enemy->Object.ExternalForce.z += (Enemy->LastDamageDir.z * (EnemyTypes[Enemy->Type].MoveRateAccell * Enemy->LastDamageSpeed) * framelag) * ( ( 1.0F/PINGOFFTIME )*Enemy->Timer);

	Enemy->Object.ExternalForce.x -= Mloadheader.Group[ Enemy->Object.Group ].up.x * 0.25F * framelag;
	Enemy->Object.ExternalForce.y -= Mloadheader.Group[ Enemy->Object.Group ].up.y * 0.25F * framelag;
	Enemy->Object.ExternalForce.z -= Mloadheader.Group[ Enemy->Object.Group ].up.z * 0.25F * framelag;

//	ObjectRotateExternal( &Enemy->Object, &Enemy->LastDamagePos , &Enemy->LastDamageImpactPos,
//						  &Enemy->LastDamageDir, D2R( (15.0F ) * framelag ) , EnemyTypes[Enemy->Type].Radius );

	ObjectRotateExternal( &Enemy->Object, &Enemy->LastDamagePos , &Enemy->LastDamageImpactPos,
						  &Enemy->LastDamageDir, D2R( -Enemy->LastDamage * framelag ) , EnemyTypes[Enemy->Type].Radius );

}





#ifdef OPT_ON
#pragma optimize( "", off )
#endif

