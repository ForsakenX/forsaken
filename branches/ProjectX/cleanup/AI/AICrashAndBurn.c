/*
 * a i c r a s h a n d b u r n . c
 *
 * AIMODE_DEATH_CRASHANDBURN
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 11 $
 *
 * $Header: /PcProjectX/Ai/AICrashAndBurn.c 11    3/19/98 5:17p Phillipd $
 *
 * $Log: /PcProjectX/Ai/AICrashAndBurn.c $
 * 
 * 11    3/19/98 5:17p Phillipd
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
 * 4     11/22/97 1:23p Phillipd
 * 
 * 3     11/21/97 10:56a Phillipd
 * Max Kills to end a level....
 * 
 * 2     11/20/97 12:39p Phillipd
 * 
 * 1     11/20/97 12:05p Phillipd
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
void CreateSpotFXBurning( VECTOR * Pos, VECTOR * Dir, uint16 Group );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR Crash and burn...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_DEATH_CRASHANDBURN( register ENEMY * Enemy )
{
	OBJECT * SObject;
	VECTOR		NewDir = { 0.0F, 1.0F, 0.0F };
	VECTOR		NewPos;

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
	Enemy->Object.ExternalForce.x -= Mloadheader.Group[ Enemy->Object.Group ].up.x * 0.55F * framelag;
	Enemy->Object.ExternalForce.y -= Mloadheader.Group[ Enemy->Object.Group ].up.y * 0.55F * framelag;
	Enemy->Object.ExternalForce.z -= Mloadheader.Group[ Enemy->Object.Group ].up.z * 0.55F * framelag;

	NewPos = Enemy->Object.Pos;

	NewPos.x -= Mloadheader.Group[ Enemy->Object.Group ].up.x * (EnemyTypes[Enemy->Type].Radius * 0.75F);
	NewPos.y -= Mloadheader.Group[ Enemy->Object.Group ].up.y * (EnemyTypes[Enemy->Type].Radius * 0.75F);
	NewPos.z -= Mloadheader.Group[ Enemy->Object.Group ].up.z * (EnemyTypes[Enemy->Type].Radius * 0.75F);

	AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &NewPos );
	Enemy->AI_Angle = AimData.Angle;
	Enemy->AIMoveFlags |= AimData.Flags;

	CreateSpotFXBurning( &NewPos, &Mloadheader.Group[ Enemy->Object.Group ].up, Enemy->Object.Group );

}
#ifdef OPT_ON
#pragma optimize( "", off )
#endif






