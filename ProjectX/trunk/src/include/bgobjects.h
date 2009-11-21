/*===================================================================
	File	:	bgobjects.h
	Content	:	BGObjects.c include file
===================================================================*/
#ifndef BGOBJECTS_INCLUDED
#define BGOBJECTS_INCLUDED

/*===================================================================
	Defines
===================================================================*/
#define	MAXBGOBJECTS				256

#define	BGOTYPE_Static				0
#define	BGOTYPE_Door				1
#define	BGOTYPE_Looping_Anim		2
#define	BGOTYPE_MultipleStep_Anim	3
#define	BGOTYPE_OneOff_Anim			4
#define	BGOTYPE_Sequences			5

#define	DOOR_CLOSED					0
#define	DOOR_OPENING				1
#define	DOOR_OPEN					2
#define	DOOR_CLOSING				3
#define	DOOR_REVERSEOPENING			4
#define	DOOR_REVERSECLOSING			5

#define	WHEN_PlayerBump				1
#define	WHEN_PlayerShot				2
#define	WHEN_EnemyBump				4
#define	WHEN_EnemyShot				8

#define	BUMP						0
#define	SHOT						1

#define	BGOGENTYPE_Immediate		0
#define	BGOGENTYPE_AfterTime		1
#define	BGOGENTYPE_Triggered		2

#define	LOOPANIM_WaitForDelay		0
#define	LOOPANIM_WaitForTrigger		1
#define	LOOPANIM_Animating			2

#define	ONEOFFANIM_WaitForDelay		0
#define	ONEOFFANIM_WaitForTrigger	1
#define	ONEOFFANIM_Animating		2

#define	DOORSFXTYPE_Generic			0
#define	DOORSFXTYPE_Stone			1
#define	DOORSFXTYPE_ForceField		2
#define	DOORSFXTYPE_Lift			3
#define	DOORSFXTYPE_Smoke			4
#define	DOORSFXTYPE_Electric1		5
#define	DOORSFXTYPE_Electric2		6
#define	DOORSFXTYPE_Electric3		7
#define	DOORSFXTYPE_Electric4		8

/*===================================================================
	Structures
===================================================================*/
typedef struct BGOBJECT {

	int16		State;
	uint16		Type;
	uint16		Group;
	uint16		ModelIndex;
	VECTOR		StartPos;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	VECTOR		RightVector;
	VECTOR		MoveOff;
	uint16		CrushCount;
	float		Frame;
	MATRIX		Matrix;
	MATRIX		InvMatrix;
	MATRIX		ColMatrix;
	MATRIX		InvColMatrix;
	MATRIX		ScaleMatrix;
	MATRIX		InvScaleMatrix;
	float		Offset;
	float		OffsetInc;
	float		MaxOffset;
	float		Timer;
	float		Width;
	float		Height;
	float		Shield;
	int16		NumChildren;
	struct COMP_OBJ	* Children;

	float		OldOldTime;
	float		OldTime;
	float		Time;
	float		OverallTime;
	float		MidTime;
	float		AnimSpeed;

	float		StateChangeDelay;

	int16		OpenTrigMod;		// Trigger Mod pointer index
	int16		CloseTrigMod;		// Trigger Mod pointer index
	struct TRIGGERMOD * OpenTrigModPtr;		// Trigger Mod Array
	struct TRIGGERMOD * CloseTrigModPtr;		// Trigger Mod Array
	int16		ShotTrigMod;		// Trigger Mod pointer index
	int16		BumpTrigMod;		// Trigger Mod pointer index
	int16		EndTrigMod;			// Trigger Mod pointer index
	struct TRIGGERMOD * ShotTrigModPtr;		// Trigger Mod Array
	struct TRIGGERMOD * BumpTrigModPtr;		// Trigger Mod Array
	struct TRIGGERMOD * EndTrigModPtr;		// Trigger Mod Array
	int16		Locked;
	int16		OpenedBy;
	int16		DestroyAtEnd;
	int32		PickupNeeded;
	int16		GenType;
	float		Delay;

	float		WantedTime;
	float		DamageTime;
	float		StepInterval;
	float		TimePerDamageUnit;
	float		DamagePerInterval;

	VECTOR		ColTopLeft;
	VECTOR		ColBottomRight;
	VECTOR		ColCenter;
	float		ColRadius;

	VECTOR		TopLeft;
	VECTOR		BottomRight;
	VECTOR		Center;
	float		Radius;
	uint16		Index;
	int16		UpdateCount;

	int16		DoorSfxType;
	uint32		SoundFX_ID;

	struct	BGOBJECT	*	PrevUsed;
	struct	BGOBJECT	*	NextUsed;
	struct	BGOBJECT	*	PrevFree;
	struct	BGOBJECT	*	NextFree;

} BGOBJECT;

typedef struct BGO_FILE {

	char		Filename[ 256 ];
	uint16		BaseModel;
	uint16		Type;
	int16		OpenEvent;
	int16		CloseEvent;
	int16		ShotEvent;
	int16		BumpEvent;
	int16		EndEvent;
	int32		PickupNeeded;		// -1 if none needed
	int16		OpenedBy;
	int16		Locked;
	int16		DestroyAtEnd;
	int16		GenType;
	float		Delay;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Dir;
	VECTOR		Up;
	uint16		NumIntervals;
	float		DamagePerInterval;
	int16		DoorSfxType;

} BGO_FILE;

typedef struct BIKEINFO {

	char	*	Filename;
	uint16		BaseModel;
	uint16		StealthBaseModel;
	float		OverallTime;

} BIKEINFO;

typedef struct DOORTYPESFX {

	int16		StartOpeningSoundFX;
	int8	*	StartOpeningName;
	int16		OpeningSoundFX;
	int8	*	OpeningName;
	int16		StartOpenSoundFX;
	int8	*	StartOpenName;
	int16		OpenSoundFX;
	int8	*	OpenName;
	int16		StartClosingSoundFX;
	int8	*	StartClosingName;
	int16		ClosingSoundFX;
	int8	*	ClosingName;
	int16		StartClosedSoundFX;
	int8	*	StartClosedName;
	int16		ClosedSoundFX;
	int8	*	ClosedName;

} DOORTYPESFX;

/*===================================================================
	Prototypes
===================================================================*/
void InitBGObjects( void );
BGOBJECT * FindFreeBGObject( void );
void KillUsedBGObject( BGOBJECT * Object );
void ProcessBGObjects( BOOL Collide );
BGOBJECT * CreateBGObject( VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector,
						  uint16 Group, uint16 Model, float Frame, uint16 Func, uint16 Type );
void KillAllBGObjects( void );
void ChangeBGState( BGOBJECT * Object, uint16 OwnerType, uint16 Owner, int16 How, float Damage );
BOOL PreLoadBGOFiles( void );
BOOL LoadBGOFiles( void );
BGOBJECT * LoadBGObjectData( int8 * Filename, int16 Type, VECTOR * Pos, VECTOR * DirVector,
							 VECTOR * UpVector, uint16 Group, int16 OpenTrigMod, int16 CloseTrigMod,
							 int16 ShotTrigMod, int16 BumpTrigMod, int16 EndTrigMod, int16 OpenedBy,
							 int16 DestroyAtEnd, int16 GenType, float Delay, int16 Locked, int32 PickupNeeded,
							 uint16 * BaseModel, uint16 NumIntervals, float DamagePerInterval, int16 DoorSfxType );
void UpdateBGObject( uint16 BGObject, int16 State, float Time );
void UndoBGObjectAnim( BGOBJECT * Object );
void OpenDoor( uint16 Index );
void CloseDoor( uint16 Index );
void LockDoor( uint16 Index );
void UnlockDoor( uint16 Index );
void StartBGOAnim( uint16 Index );
void StopBGOAnim( uint16 Index );
void ShowAllColZones( uint16 Group );
void CreateBoundingBox( VECTOR * x1, VECTOR * x2, uint16 Group );
float CalcObjectRadius( VECTOR * Pos, VECTOR * TopLeft, VECTOR * BottomRight );
BOOL CheckBGObjectToEnemies( BGOBJECT * Object );
BOOL CheckBGObjectToPickups( BGOBJECT * Object );
BOOL CheckBGObjectToMines( BGOBJECT * Object );
void ExplodeChildren( COMP_OBJ * Children, int16 NumChildren );
FILE * SaveBGObjects( FILE * fp );
FILE * LoadBGObjects( FILE * fp );
BOOL PreLoadShips( void );
BOOL AllocateCompShip( uint16 Ship );
void ReloadNakedGirlsTextures( void );
void RequestDoorTypeSFX( int16 Type );
void StartOrChangeDoorSoundFX( BGOBJECT * Object );

BOOL PreLoadFlyGirl( void );
BOOL AllocateCompFlyGirl( void );
void UpdateFlyGirl( VECTOR * Pos, MATRIX * Matrix, BOOL Visible );
void ReleaseFlyGirl( void );

#endif


