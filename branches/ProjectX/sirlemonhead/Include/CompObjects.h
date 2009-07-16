/*/*===================================================================
	File	:	CompObjects.h
	Content	:	CompObjects.c include file
===================================================================*/
#ifndef COMPOBJECTS_INCLUDED
#define COMPOBJECTS_INCLUDED

/*/*===================================================================
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
/*/*===================================================================
	Structures
===================================================================*/
typedef struct ANIM_SEQ {

	float	StartTime;			// Model Filename or Component Filename ( .MX/.MXA or .COB )
	float	EndTime;			// -1 None ( Not used so far )

} ANIM_SEQ;

typedef struct COMP_OBJ {

	int16		Flags;
	int16		ID;
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

	int16		Frame;
	int16		InterpFrame;
	float		InterpTime;

	float		OldOldTime;
	VECTOR		OldOldDisplayPos;
	MATRIX		OldOldDisplayMatrix;
	MATRIX		OldOldInvDisplayMatrix;

	float		OldTime;
	VECTOR		OldDisplayPos;
	MATRIX		OldDisplayMatrix;
	MATRIX		OldInvDisplayMatrix;

	BOOL		UserControl;

	VECTOR		UserAxis;
	VECTOR		UserAxisPoint;
	float		UserAngle;

	int16		NumChildren;
	uint16		ModelIndex;
	struct COMP_OBJ	 * Children;
	int16		NumTrans;
	struct ANI_TRANS * Trans;

	int16		NumColZones;
	struct ANI_ZONES * ColZones;

} COMP_OBJ;

/*/*===================================================================
	Animation Types
===================================================================*/
typedef struct ANI_TRANS {

	int16	Type;
	float	TimeStart;
	float	TimeDuration;
	void *	Data;

} ANI_TRANS;

typedef struct ANI_TRANSLATE {

	VECTOR	Trans;
	int16	Local;

} ANI_TRANSLATE;

typedef struct ANI_ROTATE {

	VECTOR	Axis;
	VECTOR	Origin;
	float	Angle;
	int16	Local;

} ANI_ROTATE;

typedef struct ANI_MORPH {

	int16	Frame;

} ANI_MORPH;

typedef struct ANI_PROPERTY {

	int16	Type;
	int16	Value;

} ANI_PROPERTY;

/*/*===================================================================
	Collision Zone Types
===================================================================*/
typedef struct ZONESIDE {

	VECTOR	Normal;
	float	PlaneOffset;
	int16	Sensitive;
	float	Damage;

} ZONESIDE;

typedef struct ANI_ZONES {

	int16	Type;
	int16	Sensitive;
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
	uint16	NumSides;
	ZONESIDE * Sides;

} ANI_ZONE_BOX;

typedef struct ANI_ZONE_POLYGONAL {

	VECTOR	Center;
	VECTOR	HalfSize;
	uint16	NumSides;
	ZONESIDE * Sides;

} ANI_ZONE_POLYGONAL;

/*/*===================================================================
	Prototypes
===================================================================*/
BOOL PreLoadCompObj( int8 * Filename, uint16 * BaseModel, BOOL LevelSpecific );
COMP_OBJ * LoadCompObj( int8 * Filename, VECTOR * Pos, VECTOR * Dir, uint16 Group,
					    float * OverallTime, float * MidTime, uint16 * BaseModel,
						uint16 OwnerType, uint16 OwnerID );
FILE * LoadCompObjChildren( FILE * fp, COMP_OBJ * Comp, int16 NumComp,
						    VECTOR * Pos, VECTOR * Dir, uint16 Group,
							float * Time, float * MidTime, uint16 * BaseModel, uint16 OwnerType, uint16 OwnerID );
void FreeCompObjChildren( COMP_OBJ * Children, int16 NumChildren );
void UpdateCompObjChildren( COMP_OBJ * Children, int16 NumChildren, MATRIX * ParentMatrix, VECTOR * ParentPos, float Time, uint16 Group, VECTOR * Pos );
COMP_OBJ * GetCompObjAddress( int16 ID, int16 NumChildren, COMP_OBJ * Children );
void SetCompObjModelsState( COMP_OBJ * Children, int16 NumChildren, BOOL Visible );
void UndoCompObjAnim( COMP_OBJ * Children, int16 NumChildren );
void ShowCompObjColZones( COMP_OBJ * Children, int16 NumChildren, uint16 Group );
BOOL RayToColZone( VECTOR * StartPos, VECTOR * EndPos, ZONESIDE * StartSide, int16 StartNumSides, VECTOR * IntPoint, ZONESIDE ** IntSide, float Radius );
BOOL PointInside( VECTOR * Pos, ZONESIDE * Sides, int16 NumSides, float Radius, int16 Side );
BOOL GetCompObjAxis( COMP_OBJ * Comp );
void GetCompObjBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16 NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight );
void GetCompObjBoundingBoxChildren( COMP_OBJ * Children, int16 NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight );
void UpdateCompObjColours( COMP_OBJ * Children, int16 NumChildren, int Red, int Green, int Blue );
void UpdateCompObjClipGroup( COMP_OBJ * Children, int16 NumChildren, uint16 ClipGroup );
void MaximizeBoundingBox( VECTOR * TopLeft, VECTOR * BottomRight );
void SetCompObjRealLighting( COMP_OBJ * Children, int16 NumChildren, float Radius );
void ClearCompObjRealLighting( COMP_OBJ * Children, int16 NumChildren );
BOOL AmIInvulnerable( COMP_OBJ * Children, int16 NumChildren, float Time );
void UpdateCompObjFlags( COMP_OBJ * Children, int16 NumChildren, uint16 Flags );
FILE * SaveAllCompObj( FILE * fp, COMP_OBJ * Children, int16 NumChildren );
FILE * LoadAllCompObj( FILE * fp, COMP_OBJ * Children, int16 NumChildren );

void GetCompObjColBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16 NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight );
void GetCompObjColBoundingBoxChildren( COMP_OBJ * Children, int16 NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight );
void GetCompObjColZoneBoundingBox( COMP_OBJ * Children, MATRIX * DisplayMatrix, VECTOR * DisplayPos,
								   VECTOR * TopLeft, VECTOR * BottomRight );
void GetZoneBoundingBox( VECTOR * Center, VECTOR * HalfSize, MATRIX * Matrix, VECTOR * Pos,
						 VECTOR * TopLeft, VECTOR * BottomRight );
void AddVertToBoundingBox( VECTOR * Vert, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft, VECTOR * BottomRight );
void SetStealthOffset( COMP_OBJ * Children, int16 NumChildren, uint16 Offset );
void SetCompObjStealth( COMP_OBJ * Children, int16 NumChildren );
void SetCompObjNonStealth( COMP_OBJ * Children, int16 NumChildren );

#endif


