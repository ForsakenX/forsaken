/*===================================================================
	File	:	compobjects.h
	Content	:	CompObjects.c include file
===================================================================*/
#ifndef COMPOBJECTS_INCLUDED
#define COMPOBJECTS_INCLUDED

#include "main.h"
#include <stdio.h>

/*===================================================================
	Defines
===================================================================*/
#define TRANS_TRANSLATE				0
#define TRANS_ROTATE				1
#define TRANS_MORPH					2
#define TRANS_PROPERTY				3

#define	PROPTYPE_Unknown			-1				// Unknown
#define	PROPTYPE_State				0				// Property Types
#define	PROPTYPE_SFX				1

#define	PROPSTATE_Opening			0				// States
#define	PROPSTATE_Open				1
#define	PROPSTATE_Closing			2
#define	PROPSTATE_Closed			3
#define	PROPSTATE_UserControl		4
#define	PROPSTATE_Invulnerable		5
#define	PROPSTATE_Destroy			6
#define	PROPSTATE_DisableSpotFX		7

#define	COMPFLAG_Enable				1

#define	ANIM_SECOND					60.0F

#define	MAX_USER_CONT_COMPS			10
/*===================================================================
	Structures
===================================================================*/
typedef struct ANIM_SEQ {

	float	StartTime;			// Model Filename or Component Filename ( .MX/.MXA or .COB )
	float	EndTime;			// -1 None ( Not used so far )

} ANIM_SEQ;

typedef struct COMP_OBJ {

	int16_t		Flags;
	int16_t		ID;
	VECTOR		DisplayPos;
	VECTOR		OffPos;
	VECTOR		Dir;
	VECTOR		Up;
	VECTOR		RotPoint;
	VECTOR		RotAxis;
	float		RotAngle;
	MATRIX		InitMatrix;
	MATRIX		LocalMatrix;
	MATRIX		DisplayMatrix;
	MATRIX		InvDisplayMatrix;

	int16_t		Frame;
	int16_t		InterpFrame;
	float		InterpTime;

	float		OldOldTime;
	VECTOR		OldOldDisplayPos;
	MATRIX		OldOldDisplayMatrix;
	MATRIX		OldOldInvDisplayMatrix;

	float		OldTime;
	VECTOR		OldDisplayPos;
	MATRIX		OldDisplayMatrix;
	MATRIX		OldInvDisplayMatrix;

	bool		UserControl;

	VECTOR		UserAxis;
	VECTOR		UserAxisPoint;
	float		UserAngle;

	int16_t		NumChildren;
	u_int16_t		ModelIndex;
	struct COMP_OBJ	 * Children;
	int16_t		NumTrans;
	struct ANI_TRANS * Trans;

	int16_t		NumColZones;
	struct ANI_ZONES * ColZones;

} COMP_OBJ;

/*===================================================================
	Animation Types
===================================================================*/
typedef struct ANI_TRANS {

	int16_t	Type;
	float	TimeStart;
	float	TimeDuration;
	void *	Data;

} ANI_TRANS;

typedef struct ANI_TRANSLATE {

	VECTOR	Trans;
	int16_t	Local;

} ANI_TRANSLATE;

typedef struct ANI_ROTATE {

	VECTOR	Axis;
	VECTOR	Origin;
	float	Angle;
	int16_t	Local;

} ANI_ROTATE;

typedef struct ANI_MORPH {

	int16_t	Frame;

} ANI_MORPH;

typedef struct ANI_PROPERTY {

	int16_t	Type;
	int16_t	Value;

} ANI_PROPERTY;

/*===================================================================
	Collision Zone Types
===================================================================*/
typedef struct ZONESIDE {

	VECTOR	Normal;
	float	PlaneOffset;
	int16_t	Sensitive;
	float	Damage;

} ZONESIDE;

typedef struct ANI_ZONES {

	int16_t	Type;
	int16_t	Sensitive;
	float	Damage;
	void *	Data;

} ANI_ZONES;

typedef struct ANI_ZONE_SPHERE {

	VECTOR	Center;
	VECTOR	HalfSize;

} ANI_ZONE_SPHERE;

typedef struct ANI_ZONE_BOX {

	VECTOR	Center;
	VECTOR	HalfSize;
	u_int16_t	NumSides;
	ZONESIDE * Sides;

} ANI_ZONE_BOX;

typedef struct ANI_ZONE_POLYGONAL {

	VECTOR	Center;
	VECTOR	HalfSize;
	u_int16_t	NumSides;
	ZONESIDE * Sides;

} ANI_ZONE_POLYGONAL;

/*===================================================================
	Prototypes
===================================================================*/
bool PreLoadCompObj( int8_t * Filename, u_int16_t * BaseModel, bool LevelSpecific );
COMP_OBJ * LoadCompObj( int8_t * Filename, VECTOR * Pos, VECTOR * Dir, u_int16_t Group,
					    float * OverallTime, float * MidTime, u_int16_t * BaseModel,
						u_int16_t OwnerType, u_int16_t OwnerID );
FILE * LoadCompObjChildren( FILE * fp, COMP_OBJ * Comp, int16_t NumComp,
						    VECTOR * Pos, VECTOR * Dir, u_int16_t Group,
							float * Time, float * MidTime, u_int16_t * BaseModel, u_int16_t OwnerType, u_int16_t OwnerID );
void FreeCompObjChildren( COMP_OBJ * Children, int16_t NumChildren );
void UpdateCompObjChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix, VECTOR * ParentPos, float Time, u_int16_t Group, VECTOR * Pos );
COMP_OBJ * GetCompObjAddress( int16_t ID, int16_t NumChildren, COMP_OBJ * Children );
void SetCompObjModelsState( COMP_OBJ * Children, int16_t NumChildren, bool Visible );
void UndoCompObjAnim( COMP_OBJ * Children, int16_t NumChildren );
void ShowCompObjColZones( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Group );
bool RayToColZone( VECTOR * StartPos, VECTOR * EndPos, ZONESIDE * StartSide, int16_t StartNumSides, VECTOR * IntPoint, ZONESIDE ** IntSide, float Radius );
bool PointInside( VECTOR * Pos, ZONESIDE * Sides, int16_t NumSides, float Radius, int16_t Side );
bool GetCompObjAxis( COMP_OBJ * Comp );
void GetCompObjBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16_t NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight );
void GetCompObjBoundingBoxChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight );
void UpdateCompObjColours( COMP_OBJ * Children, int16_t NumChildren, int Red, int Green, int Blue );
void UpdateCompObjClipGroup( COMP_OBJ * Children, int16_t NumChildren, u_int16_t ClipGroup );
void MaximizeBoundingBox( VECTOR * TopLeft, VECTOR * BottomRight );
void SetCompObjRealLighting( COMP_OBJ * Children, int16_t NumChildren, float Radius );
void ClearCompObjRealLighting( COMP_OBJ * Children, int16_t NumChildren );
bool AmIInvulnerable( COMP_OBJ * Children, int16_t NumChildren, float Time );
void UpdateCompObjFlags( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Flags );
FILE * SaveAllCompObj( FILE * fp, COMP_OBJ * Children, int16_t NumChildren );
FILE * LoadAllCompObj( FILE * fp, COMP_OBJ * Children, int16_t NumChildren );

void GetCompObjColBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16_t NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight );
void GetCompObjColBoundingBoxChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight );
void GetCompObjColZoneBoundingBox( COMP_OBJ * Children, MATRIX * DisplayMatrix, VECTOR * DisplayPos,
								   VECTOR * TopLeft, VECTOR * BottomRight );
void GetZoneBoundingBox( VECTOR * Center, VECTOR * HalfSize, MATRIX * Matrix, VECTOR * Pos,
						 VECTOR * TopLeft, VECTOR * BottomRight );
void AddVertToBoundingBox( VECTOR * Vert, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft, VECTOR * BottomRight );
void SetStealthOffset( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Offset );
void SetCompObjStealth( COMP_OBJ * Children, int16_t NumChildren );
void SetCompObjNonStealth( COMP_OBJ * Children, int16_t NumChildren );

#endif


