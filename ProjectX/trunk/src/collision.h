

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

#include "main.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "mload.h"
#include "object.h"
#include "compobjects.h"
#include "bgobjects.h"

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
	u_int16_t	num_of_groups;
	u_int16_t	num_of_faces_in_group[MAXCOLGROUPS];
	MCFACE	*GroupFacePnt[MAXCOLGROUPS];
}MCLOADHEADER;



/*
 * fn prototypes
 */

float ColDotProduct( VECTOR * a , NORMAL * b );
_Bool MCload( char * Filename , MCLOADHEADER * MCloadheader );
_Bool ColRayPolyIntersect( MCFACE *face );
_Bool RayPolyIntersect( float * P0 , float * P1 , float * P2 , float * P3 ,
	 				 VERT *  Point, NORMAL * FaceNormal , float D , float * TempDistance);
_Bool BackgroundCollide( MCLOADHEADER *c, MLOADHEADER *m,
					  VECTOR *StartPos, u_int16_t StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, u_int16_t *EndGroup,
					  NORMAL *FaceNormal, VECTOR *NewTarget, _Bool BGCol, BGOBJECT ** BGObject );
_Bool BackgroundCollideOneGroup( MCLOADHEADER *c, MLOADHEADER *m,
					  VECTOR *StartPos, u_int16_t StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, u_int16_t *EndGroup,
					  NORMAL *FaceNormal, VECTOR *NewTarget, _Bool BGCol, BGOBJECT ** BGObject );
u_int16_t MoveGroup( MLOADHEADER *m, VECTOR *StartPos, u_int16_t StartGroup, VECTOR *MoveOffset );
_Bool OneGroupPolyCol( MCLOADHEADER * MCloadheader ,MLOADHEADER * Mloadheader , u_int16_t group ,
					VECTOR * Pos, VECTOR * Dir  ,
					VECTOR * ImpactPoint , NORMAL  * FaceNormal , VECTOR * Pos_New, _Bool BGCol,
					BGOBJECT ** BGObject );
_Bool OneGroupPortalCol( MLOADHEADER * Mloadheader , u_int16_t group ,
						VECTOR * Pos, VECTOR * Dir  ,
						VECTOR * ImpactPoint , NORMAL * FaceNormal, u_int16_t * PortalHit,
						int collisionhint );
_Bool AmIOutsideGroup( MLOADHEADER * m, VECTOR * EndPos, u_int16_t EndGroup );
_Bool CheckBGObjectsCol( u_int16_t Group, float Distance, VECTOR * ImpactPoint, int collided,
						VECTOR * New_Pos, NORMAL * FaceNormal, BGOBJECT ** BGObject );

_Bool BGObjectCollideOneGroup( VECTOR *StartPos, u_int16_t StartGroup, VECTOR *MoveOffset, 
					  VECTOR *EndPos, u_int16_t *EndGroup, NORMAL *FaceNormal, VECTOR *NewTarget,
					  BGOBJECT ** BGObject );

_Bool OneGroupBGObjectCol( float Distance, int16_t Collided, u_int16_t Group, VECTOR * Pos, VECTOR * Dir  ,
					 VECTOR * ImpactPoint , NORMAL  * FaceNormal , VECTOR * Pos_New, BGOBJECT ** BGObject, float Radius );

_Bool CheckBGObjectCollision( VECTOR * Pos, BGOBJECT * Object, VECTOR * PushVector, float * DamagePtr, float Radius );
void CollideBGOToCompObjChildren( COMP_OBJ * Children, int16_t NumChildren );
void CollideBGOChildren( COMP_OBJ * Children, int16_t NumChildren );

_Bool ObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
_Bool WouldObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
_Bool QCollide( VECTOR *Start_Pos, u_int16_t Start_Group, VECTOR *Move_Off, float radius,
			  VECTOR *Impact_Point, u_int16_t *Impact_Group, NORMAL *Impact_Normal );
_Bool ObjectCollideNoBounce( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
_Bool CheckCompObjectCollision( VECTOR * Pos, COMP_OBJ * Comps, VECTOR * PushVector, float * DamagePtr, float Radius );
u_int16_t GetComEnemyHitInfo( VECTOR * IntPoint, NORMAL * IntNormal, float * IntDistance );
u_int16_t CheckAnyEnemyHit( void );

#endif // COLLISION_INCLUDED


