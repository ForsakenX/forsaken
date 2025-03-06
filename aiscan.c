
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

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

// Externals
extern	float framelag;
/*===================================================================
	Procedure	:	AIR Scan for a Target
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_AIR_SCAN( register ENEMY * Enemy )
{
	OBJECT * TObject;
	float dist;

	// Scan for target
	Tinfo->Flags = 0;
	SET_TARGET_PLAYERS;

	AI_GetDistToNearestTarget( Enemy );


	if( Tinfo->TFlags == _TARGET_PLAYERS )
	{
  		TObject = Tinfo->TObject;
		dist = DistanceVector2Vector( &Enemy->Object.Pos , &Tinfo->Pos ) + SoundInfo[Enemy->Object.Group][TObject->Group];
		
		if( AI_InViewCone( &Enemy->Object.Pos , &Enemy->Object.Mat , &TObject->Pos, Enemy->Viewcone ) || ( (dist <= EnemyTypes[Enemy->Type].Behave.HearRange) && TObject->Noise ) )
		{
			if( AI_ClearLOS( &Enemy->Object.Pos , Enemy->Object.Group , &TObject->Pos ) )
			{
				Enemy->TShip = Tinfo->TObject;
				AI_SetMOVETOTARGET( Enemy );
				return;
			}else{
				if(  Enemy->TShip && TObject->NearestNode )
				{
					Enemy->TShip = Tinfo->TObject;
					Enemy->TNode = (void*) WhichNode( Enemy->Object.NodeNetwork , Enemy->Object.NearestNode , TObject->NearestNode );
					AI_SetMOVETOTARGET( Enemy );
					return;
				}
			}
		}
	}

	// Switch to AIMODE_IDLE
	AI_SetIDLE( Enemy );
   
}

/*===================================================================
	Procedure	:	AI for a turret while its SCANNING..
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void AI_TURRET_SCAN( register ENEMY * Enemy )
{
	OBJECT * TObject;

	Enemy->Timer -= framelag;
	if( Enemy->Object.Animating )
	{
		return;
	}
 	// Scan for target
	if ( Enemy->Timer <= 0.0F )
	{
		AI_THINK( Enemy , true , true );
		if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
		{
			SET_TARGET_PLAYERS;
			
			AI_GetDistToNearestTarget( Enemy );
			
			switch( Tinfo->TFlags )
			{
			case _TARGET_PLAYERS:
				
				TObject = Tinfo->TObject;
				if( AI_InViewCone( &Enemy->Object.Pos ,&Enemy->Object.Mat , &TObject->Pos, Enemy->Viewcone ) )
				{
					if( AI_ClearLOS( &Enemy->Object.Pos, Enemy->Object.Group , &TObject->Pos ) )
					{
						Enemy->AIFlags |= AI_ICANSEEPLAYER;
						SetCurAnimSeq( TURRETSEQ_Opening, &Enemy->Object );
						Enemy->TShip = TObject;
						AI_SetFIREATTARGET( Enemy );
					}
				}
				break;
			default:
				// Switch to AIMODE_IDLE
				Enemy->Object.AI_Mode = AIMODE_IDLE;
				Enemy->Timer  =	RESET_IDLE_TIME;
				break;
				
			}
		}else{
			Enemy->Object.AI_Mode = AIMODE_IDLE;
			Enemy->Timer  =	RESET_IDLE_TIME;
		}
	}
}

/*===================================================================
	Procedure	:	AI for a Crawling Enemy while its SCANNING..
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void AI_CRAWL_SCAN( register ENEMY * Enemy )
{

	if( Enemy->Object.Animating )
	{
		return;
	}
	
	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;

      
		if ( Enemy->Timer < 0.0F )
		{
			//------------------------------------------------------------------------------
			// Switch to AIMODE_IDLE
			//------------------------------------------------------------------------------
			Enemy->Object.AI_Mode = AIMODE_IDLE;
			Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (u_int16_t) RESET_SCAN_TIME );
			return;         
		}
	}         


}


/*===================================================================
	Procedure	:	AI Do a Sacn and find a target...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_DO_SCAN( register ENEMY * Enemy )
{
	OBJECT * TObject;
	// Scan for target
	Tinfo->Pos.x 	= Enemy->Object.Pos.x;
	Tinfo->Pos.y 	= Enemy->Object.Pos.y;
	Tinfo->Pos.z 	= Enemy->Object.Pos.z;
	Tinfo->Group 	= Enemy->Object.Group;
	Tinfo->Nearest = BIGDISTANCE;
	Tinfo->TFlags = 0;
	Tinfo->SObject	= &Enemy->Object;
	Tinfo->TObject	= NULL;

	SET_TARGET_PLAYERS;

	AI_GetDistToNearestTarget( Enemy );

	if( Tinfo->TFlags & _TARGET_PLAYERS )
	{
  		TObject = Tinfo->TObject;
		if( AI_InViewCone( &Enemy->Object.Pos , &Enemy->Object.Mat , &TObject->Pos, Enemy->Viewcone ) || ( ( Enemy->LastDistance <= EnemyTypes[Enemy->Type].Behave.HearRange) && TObject->Noise ) )
		{
			if( AI_ClearLOS( &Enemy->Object.Pos , Enemy->Object.Group , &TObject->Pos ) )
			{
				Enemy->TShip = Tinfo->TObject;
			}
		}
	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

