
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
extern	float	framelag;

/*===================================================================
	Procedure	:	AIR Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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
	Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (u_int16_t) RESET_SCAN_TIME );
}
/*===================================================================
	Procedure	:	TURRET Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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

	AI_THINK( Enemy , true , true );

	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		Enemy->Object.AI_Mode = AIMODE_SCAN;
		Enemy->Timer  =	RESET_SCAN_TIME;
	}else{
		Enemy->Timer  =	RESET_IDLE_TIME * 2.0F;
	}
}

/*===================================================================
	Procedure	:	CRAWL Idle
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
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
	Enemy->Timer  =	RESET_SCAN_TIME + (float) Random_Range( (u_int16_t) RESET_SCAN_TIME );
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif

