
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
extern	MLOADHEADER Mloadheader;
void CreateSpotFXBurning( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );

/*===================================================================
	Procedure	:	AIR Crash and burn...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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






