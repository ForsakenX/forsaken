/*/*===================================================================
	File	:	SpotFX.h
	Content	:	SpotFX.c include file
===================================================================*/
#ifndef SPOTFX_INCLUDED
#define SPOTFX_INCLUDED

/*/*===================================================================
	Defines
===================================================================*/
#define	MAXSPOTFX	512

#define	SPOTFX_FLAGS_Nothing	0
#define	SPOTFX_FLAGS_Enabled	1
#define	SPOTFX_FLAGS_DoneOnce	2

typedef enum
{
	SPOTFX_Smoke,					// 0
	SPOTFX_Steam,					// 1
	SPOTFX_Sparks,					// 2
	SPOTFX_NmeTrail,				// 3
	SPOTFX_NmeGlow,					// 4
	SPOTFX_NmeVapourTrail,			// 5
	SPOTFX_Flame,					// 6
	SPOTFX_Explosion,				// 7
	SPOTFX_FirePrimary,				// 8
	SPOTFX_FireSecondary,			// 9
	SPOTFX_ElectricBeams,			// 10
	SPOTFX_GravgonTrail,			// 11
	SPOTFX_FireWall,				// 12
	SPOTFX_GravitySparks,			// 13
	SPOTFX_Shrapnel,				// 14
	SPOTFX_Bubbles,					// 15
	SPOTFX_Drip,					// 16
	SPOTFX_SoundFX,					// 17
	SPOTFX_BeardAfterburner,		// 18
	SPOTFX_BorgAfterburner,			// 19
	SPOTFX_ExcopAfterburner,		// 20
	SPOTFX_TruckerAfterburner,		// 21
	SPOTFX_NubiaAfterburner,		// 22
	SPOTFX_CerberoAfterburner,		// 23
	SPOTFX_FoetoidAfterburner,		// 24
	SPOTFX_FoetoidSmallAfterburner,	// 25
	SPOTFX_Hk5Afterburner,			// 26
	SPOTFX_JapBirdAfterburner,		// 27
	SPOTFX_JoAfterburner,			// 28
	SPOTFX_JoSmallAfterburner,		// 29
	SPOTFX_LaJayAfterburner,		// 30
	SPOTFX_MofistoAfterburner,		// 31
	SPOTFX_NutterAfterburner,		// 32
	SPOTFX_RhesusAfterburner,		// 33
	SPOTFX_SharkAfterburner,		// 34
	SPOTFX_SlickAfterburner,		// 35

};

enum {

	SPOTFX_GENTYPE_Initialised,		// Initialised immediatly
	SPOTFX_GENTYPE_Time,			// Initialised after time
	SPOTFX_GENTYPE_Trigger,			// Initialised by trigger
};

enum {

	SPOTFX_STATE_Active,
	SPOTFX_STATE_Inactive,
	SPOTFX_STATE_WaitingForDelay,
	SPOTFX_STATE_WaitingForTrigger,
};

/*/*===================================================================
	Structures
===================================================================*/
typedef struct SPOTFX {
	uint16		Type;
	uint16		Flags;
	uint16		State;
	uint16		Index;
	uint16		Group;
	uint16		Light;
	int16		SoundFX;
	float		SoundFXVolume;
	float		SoundFXSpeed;
	int16		SoundFXType;
	uint32		SoundFX_ID;
	int8		Primary;
	int8		Secondary;
	uint8		Red;
	uint8		Green;
	uint8		Blue;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	float		Delay;
	float		ActiveDelay;
	float		InactiveDelay;
	float		Time;
	float		TimeInterval;
	float		GenDelay;
	float		MaxHeight;
	struct	SPOTFX	*	PrevUsed;
	struct	SPOTFX	*	NextUsed;
	struct	SPOTFX	*	PrevFree;
	struct	SPOTFX	*	NextFree;
	struct	SPOTFX	*	NextInGroup;
	struct	SPOTFX	*	PrevInGroup;

} SPOTFX;

/*/*===================================================================
	Prototypes
===================================================================*/
void SetupSpotFX( void );
BOOL LoadSpotFX( void );
SPOTFX * InitOneSpotFX( uint16 GenType, float GenDelay, uint16 Type, uint16 Group,
					    VECTOR * Pos, VECTOR * Dir, VECTOR * Up, float ActiveDelay, float InactiveDelay,
						int32 Colour, int16 Primary, int16 Secondary, int16 SoundFX, float Volume, float Speed, int16 SFXType );
SPOTFX * FindFreeSpotFX( void );
void KillUsedSpotFX( SPOTFX * Object );
void ProcessSpotFX( void );
void EnableSpotFX( uint16 SpotFXIndex );
void DisableSpotFX( uint16 SpotFXIndex );
void CreateSpotFXSteam( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXFlame( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXSmoke( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXFlameSmoke( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXFireSecondary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, uint16 Group, uint8 Weapon, uint16 OwnerType, uint16 OwnerID );
void CreateSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, uint16 Group, uint8 Weapon, uint16 OwnerType, uint16 OwnerID );
void CreateSpotFXElectricBeams( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, uint16 Group );
void CreateSpotFXGravgonTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXNmeTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXNmeGlow( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXNmeVapourTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXRealSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXShrapnel( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXSmallShrapnel( VECTOR * Pos, VECTOR * Dir, float Speed, uint16 Group );
void CreateLargeShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16 Group, uint16 ModelNum );
void CreateSmallShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16 Group, uint16 ModelNum );
void CreateShrapnelTrail( VECTOR * Pos, VECTOR * Dir, float Size, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXFireWall( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, uint16 Group );
void CreateSpotFXBubbles( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue, float MaxHeight );
void CreateBubbleTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXDrip( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXDripSplash( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXBurning( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void CreateSpotFXBeardAfterburner( VECTOR * Pos, VECTOR * Dir, uint16 Group, float Speed );
void CreateSpotFXBorgAfterburner( VECTOR * Pos, VECTOR * Dir, uint16 Group, float Speed );
void SetupSpotFXGroups( void );
void AddSpotFXToGroup( SPOTFX * SpotFX, uint16 Group );
void RemoveSpotFXFromGroup( SPOTFX * SpotFX, uint16 Group );
void MoveSpotFXToGroup( SPOTFX * SpotFX, uint16 OldGroup, uint16 NewGroup );
void CreateNmeGenEffect( VECTOR * Pos, uint16 Group );
void CreateSpotFXSmallAfterburner( VECTOR * Pos, VECTOR * Dir, uint16 Group, float Speed );
void CreateSpotFXHk5Afterburner( VECTOR * Pos, VECTOR * Dir, uint16 Group, float Speed, uint8 Red, uint8 Green, uint8 Blue );
void CreateSpotFXRockShrapnel( VECTOR * Pos, VECTOR * Dir, uint16 Group );
FILE * SaveAllSpotFX( FILE * fp );
FILE * LoadAllSpotFX( FILE * fp );

#endif


