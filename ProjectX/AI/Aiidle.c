/*
 * a i i d l e . c
 *
 * AIMODE_IDLE
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 16 $
 *
 * $Header: /PcProjectX/Ai/Aiidle.c 16    3/05/98 9:29a Phillipd $
 *
 * $Log: /PcProjectX/Ai/Aiidle.c $
 * 
 * 16    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 15    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 14    11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 13    11/11/97 4:05p Phillipd
 * 
 * 12    11/11/97 11:00a Phillipd
 * 
 * 11    9/01/97 12:17p Phillipd
 * 
 * 10    8/17/97 1:17p Phillipd
 * Enemies coming along....
 * 
 * 9     8/15/97 4:24p Phillipd
 * enemies coming along
 * 
 * 8     7/21/97 10:28a Phillipd
 * 
 * 7     7/18/97 10:17a Phillipd
 * 
 * 6     17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 5     7/09/97 9:23a Phillipd
 * 
 * 4     8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 3     8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 2     7/07/97 2:12p Phillipd
 * More Ai...Gun Stuff
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

