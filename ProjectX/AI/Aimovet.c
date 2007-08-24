/*
 * a i m o v e t . c
 *
 * AIMODE_MOVETOTARGET
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 33 $
 *
 * $Header: /PcProjectX/Ai/Aimovet.c 33    3/09/98 10:28a Phillipd $
 *
 * $Log: /PcProjectX/Ai/Aimovet.c $
 * 
 * 33    3/09/98 10:28a Phillipd
 * 
 * 32    3/05/98 9:29a Phillipd
 * added the OPT_ON stuff to all AI files....
 * 
 * 31    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 30    1/07/98 2:53p Phillipd
 * 
 * 29    1/03/98 1:35p Phillipd
 * 
 * 28    1/03/98 1:00p Phillipd
 * 
 * 27    1/02/98 5:27p Phillipd
 * 
 * 26    1/02/98 3:10p Phillipd
 * 
 * 25    1/02/98 3:09p Phillipd
 * 
 * 24    1/02/98 3:08p Phillipd
 * 
 * 23    12/04/97 4:55p Phillipd
 * 
 * 22    11/27/97 10:39a Phillipd
 * 
 * 21    11/19/97 6:50p Phillipd
 * Clean up
 * 
 * 20    11/17/97 10:13a Phillipd
 * Trig area V2...
 * 
 * 19    11/11/97 11:00a Phillipd
 * 
 * 18    10/22/97 12:14p Phillipd
 * 
 * 17    9/16/97 2:19p Phillipd
 * 
 * 16    9/03/97 3:50p Phillipd
 * 
 * 15    8/27/97 9:20a Phillipd
 * 
 * 14    8/19/97 12:59p Phillipd
 * 
 * 13    8/19/97 11:37a Phillipd
 * 
 * 12    8/17/97 2:58p Phillipd
 * 
 * 11    8/17/97 1:41p Phillipd
 * 
 * 10    8/17/97 1:17p Phillipd
 * Enemies coming along....
 * 
 * 9     8/15/97 4:24p Phillipd
 * enemies coming along
 * 
 * 8     7/31/97 9:09a Phillipd
 * 
 * 7     17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 6     10/07/97 17:42 Collinsd
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
void CreateSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
extern	BOOL ShowNode;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	AIR Move to a Target
	Input		:	ENEMY * Enemy
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void AI_AIR_MOVETOTARGET( register ENEMY * Enemy )
{
	OBJECT * TObject;
	OBJECT * SObject;
	NODE * TNode;
	NODE * Node;
	NODE * NewNode;
	VECTOR		TempOffset = { 0.0F, 0.0F, 0.0F };

	SObject = &Enemy->Object;

	// Is it time to think???

	AI_THINK( Enemy , FALSE , FALSE);

	TObject = (OBJECT*) Enemy->TShip;

	if( !TObject )
	{
		AI_SetFOLLOWPATH( Enemy );
		return;
	}
	
	Enemy->Timer -= framelag;

	if( Enemy->Timer < 0.0F )
		Enemy->Timer = 0.0F;
	
	if( Enemy->AIFlags & AI_ICANSEEPLAYER )
	{
		AI_SetDOGFIGHT( Enemy );
		return;
	}

	if( !Enemy->TNode )
	{
		Enemy->TNode = Enemy->Object.NearestNode;
		Enemy->Timer = EnemyTypes[Enemy->Type].Behave.BlindSearchTime;	// How long I Search if I lose sight of target
	}
	
	AI_UPDATEGUNS( Enemy );
	// Aim at target
	if( Enemy->TNode )
	{
		if ( Enemy->Timer == 0.0F )
		{
			// I Only have a certian amount of time to see my target again..If that time expires go back to following...
			AI_SetFOLLOWPATH( Enemy );
			return;
		}

		TNode = (NODE*) Enemy->TNode;

		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &TNode->Pos );


		if( !( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ICANTPITCH ) )
		{
			Enemy->AIMoveFlags |= AimData.Flags;
			if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_DONTSTOPANDTURN ) || ( (AimData.Angle.x < 15.0F) && (AimData.Angle.x > -15.0F) &&	(AimData.Angle.y < 15.0F) && (AimData.Angle.y > -15.0F) ) )
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
		}else{
			//This enemy cant look up or down so it has to move up and down to compensate....
			if( ( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_DONTSTOPANDTURN ) || ( (AimData.Angle.x < 15.0F) && (AimData.Angle.x > -15.0F) &&	(AimData.Angle.y < 15.0F) && (AimData.Angle.y > -15.0F) && (AimData.Angle.y < 3.0F) && (AimData.Angle.y > -3.0F)) )
				Enemy->AIMoveFlags |= AI_CONTROL_FORWARD;
			Enemy->AIMoveFlags |= ( AimData.Flags & ( AI_CONTROL_TURNLEFT + AI_CONTROL_TURNRIGHT ) );
			if( AimData.Flags & AI_CONTROL_TURNUP )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_UP;
			}else if( AimData.Flags & AI_CONTROL_TURNDOWN )
			{
				Enemy->AIMoveFlags |=  AI_CONTROL_DOWN;
			}

		}
		
		if( DistanceVector2Vector( &Enemy->Object.Pos , &TNode->Pos ) < 64.0F )
		{
			// Make my target my nearest automatically.....This might cause problems.....But it fixes others
			if( !( Enemy->Object.NodeNetwork & TNode->NetMask ) )
			{
				// The new node is not on my network...
				Node = Enemy->Object.NearestNode;
				if( Node->NetMask & Enemy->Object.NodeNetwork )
				{
					// But my old 1 was so remember it...
					Enemy->Object.LastNearestNode = Enemy->Object.NearestNode;
				}
			}
			Enemy->Object.NearestNode = TNode;
			// If I can leave my network to follow a target then do so if I have to...
			if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_LEAVENETWORK )
				NewNode = (void*) WhichNode( 1 , Enemy->Object.NearestNode , TObject->NearestNode );
			else
				NewNode = (void*) WhichNode( Enemy->Object.NodeNetwork , Enemy->Object.NearestNode , TObject->NearestNode );


			if( NewNode == Enemy->TNode && NewNode == Enemy->Object.NearestNode &&
				Enemy->TNode == Enemy->Object.NearestNode && (Enemy->Timer > ( 10.0F * 60.0F )) )
			{
				Enemy->Timer = 10.0F * 60.0F;
			}
			else
			{
				if( !NewNode )
				{
					// couldnt find a node that will take me to my target so go back to my nearest...
					Enemy->TNode = Enemy->Object.NearestNode;
	//			}else if( AI_ClearLOS( &Enemy->Object.Pos,Enemy->Object.Group, &NewNode->Pos ) )
				}else if( AI_ClearLOSNonZero( &Enemy->Object, &NewNode->Pos , EnemyTypes[Enemy->Type].Radius  ) )
				{
					// I can see a closer node so target it instead...
					Enemy->TNode = NewNode;
				}else{
					// cant see the ideal node so just keep going to my previous target...
					// So go back to following a path...
					Enemy->Timer = 0.0F;
				}
			}
		}
	}else{
		Enemy->TNode = Enemy->Object.NearestNode;
	}
	Enemy->AI_Angle = AimData.Angle;


	TNode = Enemy->Object.NearestNode;
//	TNode = Enemy->TNode;
	if( ShowNode && TNode )
		CreateSparks( &TNode->Pos, &SlideUp, TNode->Group );


}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

