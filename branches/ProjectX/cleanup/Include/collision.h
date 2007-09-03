/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 30 $
 *
 * $Header: /PcProjectX/Include/collision.h 30    12/02/98 14:09 Collinsd $
 *
 * $Log: /PcProjectX/Include/collision.h $
 * 
 * 30    12/02/98 14:09 Collinsd
 * Fixed targeting of bosses by homing missiles.
 * 
 * 29    7/02/98 13:23 Collinsd
 * Added polygonal collision to enemies.
 * 
 * 28    5/02/98 18:13 Collinsd
 * Added enemy component collision ( Disabled ).
 * 
 * 27    29/12/97 15:58 Oliverc
 * Added new version of ObjectCollide() which does no bouncing
 * 
 * 26    29/12/97 10:42 Collinsd
 * Pickups now use new collision
 * 
 * 25    29/12/97 9:27 Oliverc
 * Added cut-down collision routine QCollide() for use with miscellaneous
 * objects (eg pickups)
 * 
 * 24    20/11/97 8:29 Collinsd
 * Pickups now effected by BGObjects.
 * 
 * 23    5/11/97 20:09 Collinsd
 * Added more crushing/pushing code.
 * 
 * 22    5/11/97 9:59 Oliverc
 * Add new collision function WouldObjectCollide()
 * 
 * 21    10/17/97 11:48a Phillipd
 * 
 * 20    15/08/97 16:13 Collinsd
 * Started bgobjects moving ships.  External Forces bug now fixed.
 * 
 * 19    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 18    13/06/97 16:04 Collinsd
 * Taken out old bgobjects
 * 
 * 17    8/06/97 17:06 Collinsd
 * Done more work on BGObjects/Doors
 * 
 * 16    13-01-97 12:19p Collinsd
 * Doors now cannot be opened by various rays.
 * 
 * 15    12-01-97 6:20p Collinsd
 * Optimised BGObject collision.
 * 
 * 14    10-01-97 3:54p Collinsd
 * Missile/Primary weapons no longer open doors before collision.
 * 
 * 13    10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 12    6-01-97 9:11a Collinsd
 * 
 * 11    15/12/96 14:45 Oliverc
 * Fixed MAXCOLGROUPS != MAXGROUPS bug
 * 
 * 10    5/12/96 16:17 Oliverc
 * Integrated new portal format (portalpolys grouped into logical portals
 * and converted into preprojected collisionpolys), BUT bug in portal
 * transition routine remains to be fixed
 * 
 * 9     22/11/96 9:39 Oliverc
 * 
 * 8     7/10/96 17:15 Oliverc
 * New background collision map now used
 * allowing use of new simplified ray<->poly intersection routine (using
 * pre-projected pre-expanded collision polys)
 * 
 * 7     9/26/96 3:43p Phillipd
 * 
 * 6     18/09/96 10:41 Oliverc
 * Added alternate background collision routine which only checks one
 * group at a time
 * 
 * 5     7/22/96 5:49p Phillipd
 * 
 * 4     19/07/96 12:34 Oliverc
 * Changed ship <-> background collision routine
 * to track movement of ship through portals
 * 
 * 3     7/09/96 12:52p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *
 *  File:       collision.h
 *  Content:    collision include file
 *
 *
 ***************************************************************************/

#ifndef COLLISION_INCLUDED
#define COLLISION_INCLUDED

#include "d3dmain.h"
#include "d3ddemo.h"
#include "typedefs.h"
#include "file.h"
#include "exechand.h"
#include "d3dappi.h"
#include "new3d.h"
#include "mload.h"
#include <time.h>
#include "compobjects.h"
#include "BGObjects.h"

/*
 * defines
 */
#define MULTI_RAY_COLLISION
//#undef MULTI_RAY_COLLISION

#define MAXCOLGROUPS MAXGROUPS
//#define COLLISION_FUDGE (0.001F)
#define SCALE_FUDGE (1.015F)
#define COLLISION_FUDGE (10.0F*GLOBAL_SCALE)

/*
 * structures
 */



typedef struct MCLOADHEADER{
	int		state;
	char * Buffer;
	uint16	num_of_groups;
	uint16	num_of_faces_in_group[MAXCOLGROUPS];
	MCFACE	*GroupFacePnt[MAXCOLGROUPS];
}MCLOADHEADER;



/*
 * fn prototypes
 */

float ColDotProduct( VECTOR * a , NORMAL * b );
BOOL MCload( char * Filename , MCLOADHEADER * MCloadheader );
BOOL ColRayPolyIntersect( MCFACE *face );
BOOL RayPolyIntersect( float * P0 , float * P1 , float * P2 , float * P3 ,
	 				 VERT *  Point, NORMAL * FaceNormal , float D , float * TempDistance);
BOOL BackgroundCollide( MCLOADHEADER *c, MLOADHEADER *m,
					  VECTOR *StartPos, uint16 StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, uint16 *EndGroup,
					  NORMAL *FaceNormal, VECTOR *NewTarget, BOOL BGCol, BGOBJECT ** BGObject );
BOOL BackgroundCollideOneGroup( MCLOADHEADER *c, MLOADHEADER *m,
					  VECTOR *StartPos, uint16 StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, uint16 *EndGroup,
					  NORMAL *FaceNormal, VECTOR *NewTarget, BOOL BGCol, BGOBJECT ** BGObject );
uint16 MoveGroup( MLOADHEADER *m, VECTOR *StartPos, uint16 StartGroup, VECTOR *MoveOffset );
BOOL OneGroupPolyCol( MCLOADHEADER * MCloadheader ,MLOADHEADER * Mloadheader , uint16 group ,
					VECTOR * Pos, VECTOR * Dir  ,
					VECTOR * ImpactPoint , NORMAL  * FaceNormal , VECTOR * Pos_New, BOOL BGCol,
					BGOBJECT ** BGObject );
BOOL OneGroupPortalCol( MLOADHEADER * Mloadheader , uint16 group ,
						VECTOR * Pos, VECTOR * Dir  ,
						VECTOR * ImpactPoint , NORMAL * FaceNormal, uint16 * PortalHit,
						int collisionhint );
BOOL AmIOutsideGroup( MLOADHEADER * m, VECTOR * EndPos, uint16 EndGroup );
BOOL CheckBGObjectsCol( uint16 Group, float Distance, VECTOR * ImpactPoint, int collided,
						VECTOR * New_Pos, NORMAL * FaceNormal, BGOBJECT ** BGObject );

BOOL BGObjectCollideOneGroup( VECTOR *StartPos, uint16 StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, uint16 *EndGroup, NORMAL *FaceNormal, VECTOR *NewTarget,
					  BGOBJECT ** BGObject );

BOOL OneGroupBGObjectCol( float Distance, int16 Collided, uint16 Group, VECTOR * Pos, VECTOR * Dir  ,
					 VECTOR * ImpactPoint , NORMAL  * FaceNormal , VECTOR * Pos_New, BGOBJECT ** BGObject, float Radius );

BOOL CheckBGObjectCollision( VECTOR * Pos, BGOBJECT * Object, VECTOR * PushVector, float * DamagePtr, float Radius );
void CollideBGOToCompObjChildren( COMP_OBJ * Children, int16 NumChildren );
void CollideBGOChildren( COMP_OBJ * Children, int16 NumChildren );

BOOL ObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
BOOL WouldObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
BOOL QCollide( VECTOR *Start_Pos, uint16 Start_Group, VECTOR *Move_Off, float radius,
			  VECTOR *Impact_Point, uint16 *Impact_Group, NORMAL *Impact_Normal );
BOOL ObjectCollideNoBounce( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
BOOL CheckCompObjectCollision( VECTOR * Pos, COMP_OBJ * Comps, VECTOR * PushVector, float * DamagePtr, float Radius );
uint16 GetComEnemyHitInfo( VECTOR * IntPoint, NORMAL * IntNormal, float * IntDistance );
uint16 CheckAnyEnemyHit( void );

#endif // COLLISION_INCLUDED


