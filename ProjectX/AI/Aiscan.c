/*
 * a i s c a n . c
 *
 * AIMODE_SCAN
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 24 $
 *
 * $Header: /PcProjectX/Ai/Aiscan.c 24    3/05/98 9:29a Phillipd $
 *
 * $Log: /PcProjectX/Ai/Aiscan.c $
 * 
 * 24    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 23    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 22    1/22/98 12:06p Phillipd
 * 
 * 21    1/13/98 3:06p Phillipd
 * Turret bug fixes......
 * 
 * 20    1/07/98 2:53p Phillipd
 * 
 * 19    11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 18    11/12/97 5:00p Phillipd
 * 
 * 17    11/11/97 4:05p Phillipd
 * 
 * 16    11/11/97 11:00a Phillipd
 * 
 * 15    9/01/97 12:17p Phillipd
 * 
 * 14    8/19/97 12:59p Phillipd
 * 
 * 13    8/17/97 2:58p Phillipd
 * 
 * 12    8/17/97 1:17p Phillipd
 * Enemies coming along....
 * 
 * 11    8/15/97 4:24p Phillipd
 * enemies coming along
 * 
 * 10    7/31/97 9:09a Phillipd
 * 
 * 9     7/21/97 10:28a Phillipd
 * 
 * 8     7/18/97 10:17a Phillipd
 * 
 * 7     17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 6     7/09/97 9:23a Phillipd
 * 
 * 5     8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 4     8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 3     7/07/97 2:12p Phillipd
 * More Ai...Gun Stuff
 * 
 * 2     7/03/97 10:11a Phillipd
 * More Ai and node
 * 
 * 1     6/30/97 10:34a Phillipd
 * 
 * 1     6/30/97 10:33a Phillipd
 * 
 * 1     6/30/97 10:32a Phillipd
 * 
 * 1     6/30/97 10:29a Phillipd
 *
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

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

// Externals
extern	float framelag;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	AIR Scan for a Target
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	AI for a turret while its SCANNING..
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
		AI_THINK( Enemy , TRUE , TRUE );
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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	AI for a Crawling Enemy while its SCANNING..
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
			Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (uint16) RESET_SCAN_TIME );
			return;         
		}
	}         


}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	AI Do a Sacn and find a target...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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

