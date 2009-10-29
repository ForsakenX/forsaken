

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

#include <d3d9.h>
#include "typedefs.h"
#include "file.h"
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


