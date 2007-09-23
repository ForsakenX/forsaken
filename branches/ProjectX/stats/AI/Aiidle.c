
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
extern	float	framelag;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	AIR Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_AIR_IDLE( register ENEMY * Enemy )
{
	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;

      
		if ( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
		return;         
	}         
	// Switch to AIMODE_SCAN
	Enemy->Object.AI_Mode = AIMODE_SCAN;
	Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (uint16) RESET_SCAN_TIME );
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	TURRET Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_TURRET_IDLE( register ENEMY * Enemy )
{
	if( Enemy->Object.Animating )
	{
		return;
	}
	
	if( Enemy->Object.CurAnimSeq != TURRETSEQ_Closed )
		SetCurAnimSeq( TURRETSEQ_Closed, &Enemy->Object );
	
	AI_UPDATEGUNS( Enemy );

	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if ( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
		return;         
	}         
	// Switch to AIMODE_SCAN

	AI_THINK( Enemy , TRUE , TRUE );

	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		Enemy->Object.AI_Mode = AIMODE_SCAN;
		Enemy->Timer  =	RESET_SCAN_TIME;
	}else{
		Enemy->Timer  =	RESET_IDLE_TIME * 2.0F;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	CRAWL Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AI_CRAWL_IDLE( register ENEMY * Enemy )
{
	if( Enemy->Object.Animating )
	{
		return;
	}

	if ( Enemy->Timer )
	{
		Enemy->Timer -= framelag;
		if ( Enemy->Timer < 0.0F )
			Enemy->Timer = 0.0F;
		return;         
	}         
	// Switch to AIMODE_SCAN
	Enemy->Object.AI_Mode = AIMODE_SCAN;
	Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (uint16) RESET_SCAN_TIME );
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif

