/*===================================================================
	File	:	SpotFX.h
	Content	:	SpotFX.c include file
===================================================================*/
#ifndef SPOTFX_INCLUDED
#define SPOTFX_INCLUDED

/*===================================================================
	Defines
===================================================================*/
#define	MAXSPOTFX	512

#define	SPOTFX_FLAGS_Nothing	0
#define	SPOTFX_FLAGS_Enabled	1
#define	SPOTFX_FLAGS_DoneOnce	2

enum
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

/*===================================================================
	Structures
===================================================================*/
typedef struct SPOTFX {
	u_int16_t		Type;
	u_int16_t		Flags;
	u_int16_t		State;
	u_int16_t		Index;
	u_int16_t		Group;
	u_int16_t		Light;
	int16_t		SoundFX;
	float		SoundFXVolume;
	float		SoundFXSpeed;
	int16_t		SoundFXType;
	u_int32_t		SoundFX_ID;
	int8_t		Primary;
	int8_t		Secondary;
	u_int8_t		Red;
	u_int8_t		Green;
	u_int8_t		Blue;
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

/*===================================================================
	Prototypes
===================================================================*/
void SetupSpotFX( void );
bool LoadSpotFX( void );
SPOTFX * InitOneSpotFX( u_int16_t GenType, float GenDelay, u_int16_t Type, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Dir, VECTOR * Up, float ActiveDelay, float InactiveDelay,
						int32_t Colour, int16_t Primary, int16_t Secondary, int16_t SoundFX, float Volume, float Speed, int16_t SFXType );
SPOTFX * FindFreeSpotFX( void );
void KillUsedSpotFX( SPOTFX * Object );
void ProcessSpotFX( void );
void EnableSpotFX( u_int16_t SpotFXIndex );
void DisableSpotFX( u_int16_t SpotFXIndex );
void CreateSpotFXSteam( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXFlame( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXSmoke( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXFlameSmoke( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXFireSecondary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group, u_int8_t Weapon, u_int16_t OwnerType, u_int16_t OwnerID );
void CreateSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group, u_int8_t Weapon, u_int16_t OwnerType, u_int16_t OwnerID );
void CreateSpotFXElectricBeams( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group );
void CreateSpotFXGravgonTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXNmeTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXNmeGlow( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXNmeVapourTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXRealSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXShrapnel( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXSmallShrapnel( VECTOR * Pos, VECTOR * Dir, float Speed, u_int16_t Group );
void CreateLargeShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16_t Group, u_int16_t ModelNum );
void CreateSmallShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16_t Group, u_int16_t ModelNum );
void CreateShrapnelTrail( VECTOR * Pos, VECTOR * Dir, float Size, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXFireWall( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group );
void CreateSpotFXBubbles( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue, float MaxHeight );
void CreateBubbleTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXDrip( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXDripSplash( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXBurning( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void CreateSpotFXBeardAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed );
void CreateSpotFXBorgAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed );
void SetupSpotFXGroups( void );
void AddSpotFXToGroup( SPOTFX * SpotFX, u_int16_t Group );
void RemoveSpotFXFromGroup( SPOTFX * SpotFX, u_int16_t Group );
void MoveSpotFXToGroup( SPOTFX * SpotFX, u_int16_t OldGroup, u_int16_t NewGroup );
void CreateNmeGenEffect( VECTOR * Pos, u_int16_t Group );
void CreateSpotFXSmallAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed );
void CreateSpotFXHk5Afterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void CreateSpotFXRockShrapnel( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
FILE * SaveAllSpotFX( FILE * fp );
FILE * LoadAllSpotFX( FILE * fp );

#endif


