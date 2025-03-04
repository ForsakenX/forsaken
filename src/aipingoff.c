
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
/*===================================================================
	Procedure	:	AIR Ping Off...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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

