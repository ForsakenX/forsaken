
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

extern	VECTOR	Forward;
extern	VECTOR	SlideUp;
extern	PRIMARYWEAPONATTRIB	PrimaryWeaponAttribs[];
extern	SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[];

extern	AIMDATA			AimData;
extern	ENEMY	*	FirstEnemyUsed;
extern	VECTOR Forward;
extern	float	Difficulty;
extern	float	DifficlutyTab[3];
extern	float	FleshMorphTimer;


void (* AI_AIR_Mode[ ])( ENEMY * Enemy ) = {
	NULL,
	AI_AIR_FOLLOWPATH,								// Follow Path..
	AI_AIR_IDLE,
   	AI_AIR_SCAN,
	AI_AIR_MOVETOTARGET,
	NULL,											//FIREATTARGET,
	AI_AIR_DOGFIGHT,								//AIMODE_DOGFIGHT
	AI_AIR_KILLMINE,								//AIMODE_KILLMINE
	AI_AIR_RETREAT,									//AIMODE_RETREAT
	AI_AIR_FORMATION,								//AIMODE_FORMATION
	AI_AIR_DEATH_CRASHANDBURN,						//AIMODE_DEATH_CRASHNBURN
	AI_AIR_DEATH_PINGOFF,							//AIMODE_DEATH_PINGOFF
	AI_AIR_DEATH_PSYCHO,							//AIMODE_DEATH_PSYCHO
};
void (* AI_FLESHMORPH_Mode[ ])( ENEMY * Enemy ) = {
	AI_FLESHMORPH_IDLE,
	AI_FLESHMORPH_MOVEFORWARDS,
	AI_FLESHMORPH_STOPFORWARD,
	AI_FLESHMORPH_MOVEBACKWARDS,
	AI_FLESHMORPH_STOPBACKWARD,
};


void (* AI_TURRET_Mode[ ])( ENEMY * Enemy ) = {
	NULL,
	NULL,											// Follow Path..
	AI_TURRET_IDLE,
   	AI_TURRET_SCAN,
	NULL,											//MOVETOTARGET,
	AI_TURRET_FIREATTARGET,
	NULL,											//AIMODE_DOGFIGHT
	NULL,											//AIMODE_KILLMINE
	NULL,											//AIMODE_RETREAT
	NULL,											//AIMODE_FORMATION
	NULL,											//AIMODE_DEATH_CRASHNBURN
	NULL,											//AIMODE_DEATH_PINGOFF
};

void (* AI_CRAWL_Mode[ ])( ENEMY * Enemy ) = {
	NULL,
	AI_CRAWL_FOLLOWPATH,							// Follow Path..
	AI_CRAWL_IDLE,
   	AI_CRAWL_SCAN,
	NULL,											//MOVETOTARGET,
	NULL,											//FIREATTARGET,
	NULL,											//AIMODE_DOGFIGHT
	NULL,											//AIMODE_KILLMINE
	NULL,											//AIMODE_RETREAT
	NULL,											//AIMODE_FORMATION
	NULL,											//AIMODE_DEATH_CRASHNBURN
	NULL,											//AIMODE_DEATH_PINGOFF
};
void (* AI_SPLINE_Mode[ ])( ENEMY * Enemy ) = {
	AI_SPLINE_FOLLOWPATH,//NULL,
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_FOLLOWPATH
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_IDLE
   	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_SCAN
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_MOVETOTARGET
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_FIREATTARGET
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_DOGFIGHT
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_KILLMINE
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_RETREAT
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_FORMATION
	AI_SPLINE_FOLLOWPATH,//NULL,											//AIMODE_DEATH_CRASHNBURN
};

void (* AI_EXOGENON_Mode[ ])( ENEMY * Enemy ) = {
	AI_EXOGENON_IDLE,
	AI_EXOGENON_MOVEDOWN,
	AI_EXOGENON_FIRE,
	AI_EXOGENON_MOVEUP,
	AI_EXOGENON_SCAN,
	AI_EXOGENON_SWEEP,
};

void (* AI_LITTLEGEEK_Mode[ ])( ENEMY * Enemy ) = {
	NULL,
	NULL,								// Follow Path..
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	AI_LITTLEGEEK_FORMATION,								//AIMODE_FORMATION
	NULL,
	NULL,
	NULL,
};



PRIMARYWEAPONKNOWLEDGE PrimaryKnowledge[] = {
	{// Pulsar
		0.0F,						// distance at which the weapon becomes less effective...
		10.0F,						// How accurate does the player have to be to hit me....
	},
	{// Trojax
		(3*1024*GLOBAL_SCALE),		// distance at which the weapon becomes less effective...
		13.0F,						// How accurate does the player have to be to hit me.... 
	},
	{// Pyrolite
		0.0F,		// distance at which the weapon becomes less effective...
		10.0F,						// How accurate does the player have to be to hit me.... 
	},
	{// Transpulse
		(3*1024*GLOBAL_SCALE),		// distance at which the weapon becomes less effective...
		12.0F,						// How accurate does the player have to be to hit me.... 
	},
	{// Suss gun
		(7*1024*GLOBAL_SCALE),		// distance at which the weapon becomes less effective...
		20.0F,						// How accurate does the player have to be to hit me.... 
	},
	{// Laser
		(2*1024*GLOBAL_SCALE),		// distance at which the weapon becomes less effective...
		10.0F,						// How accurate does the player have to be to hit me.... 
	},
	{// Orbit pulsar
		0.0F,						// distance at which the weapon becomes less effective...
		10.0F,						// How accurate does the player have to be to hit me.... 
	},
};

/*===================================================================
	Procedure	:	AIR AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR( register ENEMY * Enemy )
{
	// Process current AI mode ...
	if( AI_AIR_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_AIR_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	TURRET AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_TURRET( register ENEMY * Enemy )
{
	// Process current AI mode ...
	if( AI_TURRET_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_TURRET_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	SPLINE AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SPLINE( register ENEMY * Enemy )
{
	// Process current AI mode ...
	if( AI_SPLINE_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_SPLINE_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	CRAWL AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_CRAWL( register ENEMY * Enemy )
{
	// Process current AI mode ...

	if( AI_CRAWL_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_CRAWL_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	EXOGENON AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_EXOGENON( register ENEMY * Enemy )
{
	// Process current AI mode ...
	if( AI_EXOGENON_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_EXOGENON_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	LITTLEGEEK AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_LITTLEGEEK( register ENEMY * Enemy )
{
	// Process current AI mode ...
	if( AI_LITTLEGEEK_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_LITTLEGEEK_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	FLESHMORPH AI
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_FLESHMORPH( register ENEMY * Enemy )
{
	if(FleshMorphTimer > 0.0F)
	{
		FleshMorphTimer -= framelag;
		Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
		return;
	}
	// Process current AI mode ...
	if( AI_FLESHMORPH_Mode[ Enemy->Object.AI_Mode ] )
		( * AI_FLESHMORPH_Mode[ Enemy->Object.AI_Mode ] )(Enemy);		//go off and do his thing...
	
}
/*===================================================================
	Procedure	:	Enemy to Enemy Check Friendly Fire...
	Input		:	ENEMY * SEnemy
	Output		:	bool true/false...Yes you will hit an enemy if you fire..
===================================================================*/
bool Enemy2EnemyFriendlyFireCheck( ENEMY * SEnemy )
{
	ENEMY * TEnemy;
	VECTOR	Move_Dir;
	VECTOR	TempVector;

	TEnemy = FirstEnemyUsed;

	ApplyMatrix( &SEnemy->Object.Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
	NormaliseVector( &Move_Dir );
	while( TEnemy != NULL )
	{
		if( (TEnemy != SEnemy ) && ( TEnemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[SEnemy->Object.Group][TEnemy->Object.Group] )
			{
				if( DistanceVector2Vector( &TEnemy->Object.Pos , &SEnemy->Object.Pos ) < SEnemy->LastDistance )
				{
					// Two Enemies are within visible params....
					if( RaytoSphere2(&TEnemy->Object.Pos, EnemyTypes[TEnemy->Type].Radius , &SEnemy->Object.Pos, &Move_Dir , &TempVector , &TempVector ) )
					{
						return true;
					}
				}
			}
		}
		TEnemy = TEnemy->NextUsed;
	}

	return false;
}


/*===================================================================
	Procedure	:	AI Do a think and update how I perceive the world..
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void AI_THINK( register ENEMY * Enemy , bool OverideThinkTime , bool JustCheckPlayerRange )
{
	OBJECT * TObject;
	bool	InViewCone;
	bool	InHisViewCone;
	ENEMY * EnemyLink;
	int i;

	if( Enemy->FormationLink )
	{
		EnemyLink = Enemy->FormationLink;
		// If im linked I Target what my master Targets...
		if( EnemyLink->Alive )
		{
			Enemy->TShip = EnemyLink->TShip;
		}else{
			Enemy->FormationLink = NULL;
		}
	}
	if( JustCheckPlayerRange )
	{
		Enemy->AIFlags &= AI_MINEAVOID + AI_IHAVESEENPLAYER + AI_PLAYERISTARGETINGME + AI_PLAYERHASFIREDPRIMARY + AI_PLAYERHASFIREDSECONDARY;	// some flags need to be preserved!!!!
		// Are any Players in Range???
#ifdef COOPERATIVE
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( Ships[i].enable )
			{
				if( EnemyTypes[Enemy->Type].Behave.ThinkRange > (DistanceVector2Vector( &Enemy->Object.Pos , &Ships[i].Object.Pos) + SoundInfo[Enemy->Object.Group][Ships[i].Object.Group]) )
				{
					Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
					break;
				}
			}
		}
#else
		if( Ships[WhoIAm].enable )
		{
			if( EnemyTypes[Enemy->Type].Behave.ThinkRange > (DistanceVector2Vector( &Enemy->Object.Pos , &Ships[WhoIAm].Object.Pos) + SoundInfo[Enemy->Object.Group][Ships[WhoIAm].Object.Group]) )
			{
				Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
			}
		}
#endif
		if( Enemy->IveBeenHitTimer || GroupsAreVisible( Ships[WhoIAm].Object.Group, Enemy->Object.Group ) )
			Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
		return;
	}

	
	AI_AVOIDCHECK( Enemy );

	if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_CLOAK )
	{
		Enemy->CloakTimer -= framelag;
		if( Enemy->CloakTimer < 0.0F )
		{
			if( Enemy->Object.Flags & SHIP_Stealth )
			{
				// de cloak
				Enemy->Object.Flags &= ~SHIP_Stealth;
				Enemy->CloakTimer = 5.0F * 60.0F;
			}else{
				// cloak
				Enemy->Object.Flags |= SHIP_Stealth;
				Enemy->CloakTimer = 20.0F * 60.0F;
			}
		}
	}
	
	Enemy->ThinkTimer -= framelag;
	if(  !OverideThinkTime && Enemy->ThinkTimer > 0.0F )
		return;
	Enemy->ThinkTimer = EnemyTypes[Enemy->Type].Behave.ThinkTime;

	Enemy->AIFlags &= AI_MINEAVOID + AI_IHAVESEENPLAYER + AI_PLAYERISTARGETINGME + AI_PLAYERHASFIREDPRIMARY + AI_PLAYERHASFIREDSECONDARY;	// some flags need to be preserved!!!!

	// does this enemy ever think???
	if ( EnemyTypes[Enemy->Type].Behave.ThinkRange == 0.0F )
		return;

 	// Are any Players in Range???
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		if( Ships[i].enable )
		{
			if( EnemyTypes[Enemy->Type].Behave.ThinkRange > (DistanceVector2Vector( &Enemy->Object.Pos , &Ships[i].Object.Pos) + SoundInfo[Enemy->Object.Group][Ships[i].Object.Group]) )
			{
				Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
				break;
			}
		}
	}

	if( Enemy->IveBeenHitTimer || GroupsAreVisible( Ships[WhoIAm].Object.Group, Enemy->Object.Group ) )
		Enemy->AIFlags |= AI_ANYPLAYERINRANGE;
	
	if( Enemy->FormationLink )
	{
		// If im linked I know what my master knows...
		EnemyLink = Enemy->FormationLink;
		Enemy->AIFlags |= EnemyLink->AIFlags & ( AI_ANYPLAYERINRANGE + AI_PLAYERINRANGE + AI_ICANHEARPLAYER );
	}


	if( !(Enemy->AIFlags & AI_ANYPLAYERINRANGE ) )
		return;

	TObject = (OBJECT*) Enemy->TShip;

	if( TObject )
	{
		if( TObject->Mode != NORMAL_MODE )
		{
			Enemy->TShip = NULL;
			return;
		}

		Enemy->LastDistance	= DistanceVector2Vector( &Enemy->Object.Pos , &TObject->Pos) + SoundInfo[Enemy->Object.Group][TObject->Group];
		//See if target is within think range....
		if( (EnemyTypes[Enemy->Type].Behave.ThinkRange > Enemy->LastDistance) || Enemy->IveBeenHitTimer )
		{
			Enemy->AIFlags |= AI_PLAYERINRANGE;
		}else{
			// Target has gone too far away...
			return;
		}
		// see if im close enough to hear the target if it has made any noise...
		if( ( (Enemy->LastDistance <= (EnemyTypes[Enemy->Type].Behave.HearRange * 1.5F) ) && TObject->Noise )  )
			Enemy->AIFlags |= AI_ICANHEARPLAYER;
		
		if( Enemy->AIFlags & AI_ICANHEARPLAYER )
		{
			// if an enemy can hear a player he should be able to use that to see him.....
			InViewCone = true;
		}else{
			//See if Its within my viewcone..
			InViewCone = AI_InViewCone( &Enemy->Object.Pos, &Enemy->Object.Mat , &TObject->Pos, Enemy->Viewcone ) + ( (Enemy->AIFlags & AI_ICANSEEPLAYER) != 0 );
		}
		//See if Im in his viewcone..
		InHisViewCone = AI_InViewCone( &TObject->Pos, &TObject->Mat ,&Enemy->Object.Pos , (float) COSD( 90.0F * 0.5F ) );	// 90 degree...

		// If either of us could be seen then check for direct LOS...
		if( InViewCone || InHisViewCone )
		{

			if( AI_ClearLOS( &Enemy->Object.Pos ,Enemy->Object.Group, &TObject->Pos ) )
			{
				if( InViewCone && ( !(TObject->Flags & SHIP_Stealth) || (TObject->Flags & SHIP_Litup) ) )
				{
					Enemy->AIFlags |= AI_ICANSEEPLAYER;
					Enemy->AIFlags |= AI_IHAVESEENPLAYER;
				}
				if( InHisViewCone )
				{
					Enemy->AIFlags |= AI_PLAYERCANSEEME;
				}

				if( !(Enemy->AIFlags & ( AI_ICANSEEPLAYER + AI_IHAVESEENPLAYER + AI_ICANHEARPLAYER ) ) && Enemy->IveBeenHitTimer )
				{
					Enemy->AIFlags |= AI_ICANSEEPLAYER;
					Enemy->AIFlags |= AI_IHAVESEENPLAYER;
					Enemy->AIFlags |= AI_ICANHEARPLAYER;
				}
			}
		}else if( Enemy->IveBeenHitTimer && ( !(TObject->Flags & SHIP_Stealth) || (TObject->Flags & SHIP_Litup) ) )
		{
			// if I cant see the player in my viewcone but there is a los then I should go to full alert.... 
			Enemy->AIFlags |= AI_ICANSEEPLAYER;
			Enemy->AIFlags |= AI_IHAVESEENPLAYER;
			Enemy->AIFlags |= AI_ICANHEARPLAYER;
		}

		
		if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_NOFRIENDLYFIRE )
		{
			if( Enemy2EnemyFriendlyFireCheck( Enemy ) )
			{
				Enemy->AIFlags |= AI_FRIENDLYFIRE;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Check if a target is firing at me... And suggest something to do to avoid it....
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void AI_AVOIDCHECK( register ENEMY * Enemy )
{
	OBJECT * TObject;
	float	Cone;
	bool	InCone;
	GLOBALSHIP * TShip;
	float DistanceToMissile;
	float Range;

	if( Enemy->AvoidTimer )
	{
		Enemy->AvoidTimer -= framelag;
		if( Enemy->AvoidTimer < 0.0F )
			Enemy->AvoidTimer = 0.0F;
	}

	TObject = (OBJECT*) Enemy->TShip;


	if( (Enemy->AIFlags & (AI_TARGETED+AI_TARGETED_ONEOFF) ) && ( !Enemy->AvoidTimer ) )
	{
		if( AI_InViewCone( &Enemy->Object.Pos, &Enemy->Object.Mat , &Enemy->TargetingPos, Enemy->Viewcone ) )
		{
			// I can see the Missile
			DistanceToMissile = DistanceVector2Vector( &Enemy->Object.Pos , &Enemy->TargetingPos );
			if( (Enemy->AIFlags & AI_TARGETED_ONEOFF ) || ( (4.0F * 1024.0F * GLOBAL_SCALE) > DistanceToMissile ) )
			{
				// The Missile is to close ... take a chance and try amd avoid it
//				if( Random_Range( EnemyTypes[Enemy->Type].Behave.Avoid_Secondary ) )
				if( Random_Range_Float( Difficulty * EnemyTypes[Enemy->Type].Behave.Avoid_Secondary ) >=1.0F )
				{
					
					Enemy->AvoidTimer = 60.0F;
					Enemy->AvoidType = 0;

					if( Random_Range( 2 ) )
					{
						if( Enemy->AIMoveFlags & AI_CONTROL_NO_RIGHT )
							Enemy->AvoidType |= AVOID_LEFT;
						else
							Enemy->AvoidType |= AVOID_RIGHT;
					}else{
						if( Enemy->AIMoveFlags & AI_CONTROL_NO_LEFT )
							Enemy->AvoidType |= AVOID_RIGHT;
						else
							Enemy->AvoidType |= AVOID_LEFT;

					}
					if( !Random_Range( 3 ) )
					{
						if( Random_Range( 2 ) )
						{
							if( Enemy->AIMoveFlags & AI_CONTROL_NO_UP )
								Enemy->AvoidType |= AVOID_DOWN;
							else
								Enemy->AvoidType |= AVOID_UP;
						}else{
							if( Enemy->AIMoveFlags & AI_CONTROL_NO_DOWN )
								Enemy->AvoidType |= AVOID_UP;
							else
								Enemy->AvoidType |= AVOID_DOWN;
						}
					}
					// if the missile is to close then just slide left or right...
					if( (2.5F * 1024.0F * GLOBAL_SCALE) < DistanceToMissile )
					{
						Enemy->AvoidType |= AVOID_FORWARD;
					}
				}
			}
		}
	}

	if( (Enemy->AIFlags&AI_MINEISCLOSE) && (!Enemy->AvoidTimer) && (EnemyTypes[Enemy->Type].Behave.Flags&(AI_BEHAVIOUR_AVOIDMINES+AI_BEHAVIOUR_ATTACKMINES)) )
	{
		// A mine has told me im close so now I can react to that...Maybe...
		if( AI_InViewCone( &Enemy->Object.Pos, &Enemy->Object.Mat , &Enemy->MinePos, Enemy->Viewcone ) )
		{
			Enemy->AvoidTimer = 60.0F;
			Enemy->AvoidType = 0;
			// I can See the Mine......
			Enemy->AIFlags |= AI_MINEAVOID;
		}
	}
	
	Enemy->AIFlags &= ~(AI_PLAYERISTARGETINGME + AI_TARGETED + AI_MINEISCLOSE);

	// do I have a target??
	if( !TObject )
		return;
	if( (Enemy->AIFlags & AI_ICANSEEPLAYER) && ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_WEAPONKNOWLEDGE ) )
	{
		TShip = TObject->ID;
		if( TObject->Flags& SHIP_PrimFire )
		{
			// Player is firing primary at me....
			Enemy->AIFlags |= AI_PLAYERHASFIREDPRIMARY;
			Enemy->LastPrimaryFiredAtMe = TShip->Primary;
		}
		if( TObject->Flags& SHIP_SecFire )
		{
			// Player is firing secondary at me....
			Enemy->AIFlags |= AI_PLAYERHASFIREDSECONDARY;
			Enemy->LastSecondaryFiredAtMe = TShip->Secondary;
		}
	}
	// can the target see me??
	if( !( Enemy->AIFlags & AI_PLAYERCANSEEME ) )
		return;


	if( ( Enemy->AIFlags & AI_PLAYERHASFIREDPRIMARY ) && ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_WEAPONKNOWLEDGE ) )
	{
		// use primary knowledge to give enemy a better idea of when to try to avoid a bullet....
		Cone = PrimaryKnowledge[Enemy->LastPrimaryFiredAtMe].AccuracyAngle;
	}else{
		// cone starts off at 10 Degrees...
		Cone = 10.0F;
	}

	// at 4 blocks away the target needs to be within 10 degrees of center to be hit???
	Cone *= ( 1024.0F * 4.0F * GLOBAL_SCALE ) / Enemy->LastDistance;
	if( Cone > 90.0F )
		Cone = 90.0F;
	if( Cone < 1.0F )
		Cone = 1.0F;
	
	// is he targeting me....
	InCone = AI_InViewCone( &TObject->Pos, &TObject->Mat ,&Enemy->Object.Pos , (float) COSD( Cone * 0.5F ) );

	if( InCone )
	{
		Enemy->AIFlags |= AI_PLAYERISTARGETINGME;

		// Check to see if he is firing at me....
		if( TObject->Flags&( SHIP_PrimFire | SHIP_MulFire | SHIP_SecFire ) )
		{
			AI_AimAtTarget( &TObject->FinalInvMat , &TObject->Pos, &Enemy->Object.Pos );

			if( TObject->Flags&( SHIP_PrimFire | SHIP_MulFire ) )
			{
				Range = Difficulty * EnemyTypes[Enemy->Type].Behave.Avoid_Primary;

				if( TObject->Flags&( SHIP_SecFire ) )
				{
					Range += Difficulty * EnemyTypes[Enemy->Type].Behave.Avoid_Secondary;
				}
			}else
			{
				Range = Difficulty * EnemyTypes[Enemy->Type].Behave.Avoid_Secondary;
			}
			
			// its Firing at me.....woh...How can I best avoid it,,,			
			if( Random_Range_Float( Range ) >= 1.0F)
			{
				Enemy->AvoidTimer = 15.0F;
				Enemy->AvoidType = 0;
				
				
				if( AimData.Angle.y < 0.0F )
				{
					if( Enemy->AIMoveFlags & AI_CONTROL_NO_RIGHT )
						Enemy->AvoidType |= AVOID_LEFT;
					else
						Enemy->AvoidType |= AVOID_RIGHT;
				}
				if( AimData.Angle.y > 0.0F )
				{
					if( Enemy->AIMoveFlags & AI_CONTROL_NO_LEFT )
						Enemy->AvoidType |= AVOID_RIGHT;
					else
						Enemy->AvoidType |= AVOID_LEFT;
				}
				
				if( AimData.Angle.x < -1.0F )
				{
					if( Enemy->AIMoveFlags & AI_CONTROL_NO_UP )
						Enemy->AvoidType |= AVOID_DOWN;
					else
						Enemy->AvoidType |= AVOID_UP;
				}
				if( AimData.Angle.x > 1.0F )
				{
					if( Enemy->AIMoveFlags & AI_CONTROL_NO_DOWN )
						Enemy->AvoidType |= AVOID_UP;
					else
						Enemy->AvoidType |= AVOID_DOWN;
				}
			}
		}

		
	}

}

 

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

