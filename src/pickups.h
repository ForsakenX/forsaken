/*==========================================================================
 *  File:       P i c k u p s . h
 *  Content:    Pickups include file
 ***************************************************************************/
#ifndef PICKUPS_INCLUDED
#define PICKUPS_INCLUDED

#include "main.h"
/*===================================================================
	Defines
===================================================================*/

#ifdef DEBUG_ON
#define DEBUG_PICKUPS			1
#else
#define DEBUG_PICKUPS			0
#endif

#define	PIC_VERSION_NUMBER	1
								
#define	MAXPICKUPS				256
#define MAXPICKUPTYPES			46
#define	PICKUP_RADIUS			( 562.5F * GLOBAL_SCALE )
#define	PICKUPLIGHT_RADIUS		( 1536.0F * GLOBAL_SCALE )
#define	PICKUP_SPEED			( 40.0F * GLOBAL_SCALE )
#define	PICKUP_DEC				( 0.20F * GLOBAL_SCALE )
#define	COLOUR_SCALE			0.5F
#define	PICKUP_MODEL_RADIUS		( 310.0F * GLOBAL_SCALE )
								
#define	MAXMISSEDPICKUPS		256
#define	MPU_TYPE_KILL			0
#define	MPU_TYPE_INIT			1
								
#define	INVULNERABILITY_TIME	( 30.0F * 60.0F )

#define	MAXFAILEDKILLS			100
#define	FAILEDKILLTIMEOUT		( 15.0F * 60.0F )

enum {							// Pickup Mode

	PICKUPMODE_Appear,
	PICKUPMODE_Normal,
	PICKUPMODE_Disappear,
};

enum {							// Pickup Kill Modes

	PICKUPKILL_Immediate,
	PICKUPKILL_Disappear,
};

enum {

	PICKUP_Trojax,				// 0 Primary Weapons
	PICKUP_Pyrolite,			// 1
	PICKUP_Transpulse,			// 2
	PICKUP_SussGun,				// 3
	PICKUP_Laser,				// 4
	PICKUP_Mug,					// 5 Secondary weapons
	PICKUP_Mugs,				// 6
	PICKUP_Heatseaker,			// 7
	PICKUP_HeatseakerPickup,	// 8
	PICKUP_Thief,				// 9
	PICKUP_Scatter,				// 10
	PICKUP_Gravgon,				// 11
	PICKUP_Launcher,			// 12
	PICKUP_TitanStar,			// 13
	PICKUP_PurgePickup,			// 14
	PICKUP_PinePickup,			// 15
	PICKUP_QuantumPickup,		// 16
	PICKUP_SpiderPod,			// 17
	PICKUP_Parasite,			// 18
	PICKUP_Flare,				// 19
	PICKUP_GeneralAmmo,			// 20 Ammo
	PICKUP_PyroliteAmmo,		// 21
	PICKUP_SussGunAmmo,			// 22
	PICKUP_PowerPod,			// 23 Extras
	PICKUP_Shield,				// 24
	PICKUP_Inv,					// 25
	PICKUP_ExtraLife,			// 26
	PICKUP_Computer,			// 27
	PICKUP_Smoke,				// 28
	PICKUP_Nitro,				// 29
	PICKUP_Goggles,				// 30
	PICKUP_Gold,				// 31
	PICKUP_Mantle,				// 32
	PICKUP_Crystal,				// 33
	PICKUP_Orb,					// 34
	PICKUP_GoldenPowerPod,		// 35
	PICKUP_DNA,					// 36
	PICKUP_SkeletonKey,			// 37
	PICKUP_Bomb,				// 38
	PICKUP_GoldFigure,			// 39
	PICKUP_Flag,				// 40
	PICKUP_Bounty,				// 41
	PICKUP_Flag1,				// 42
	PICKUP_Flag2,				// 43
	PICKUP_Flag3,				// 44
	PICKUP_Flag4,				// 45
};

/*===================================================================
	Regeneration point defines
===================================================================*/
enum {							// Pickup Regeneration Status

	PU_REGENSTAT_Free,
	PU_REGENSTAT_Used,
	PU_REGENSTAT_TimeGen,
	PU_REGENSTAT_TrigGen,
};

enum {

	PU_GENTYPE_Initialised,		// Initialised immediatly
	PU_GENTYPE_Time,			// Initialised after time
	PU_GENTYPE_Trigger,			// Initialised by trigger
};

enum {

	PU_REGENTYPE_Random,		// Random each time
	PU_REGENTYPE_Const,			// Same each time
	PU_REGENTYPE_Onceonly,		// Once only ( Never used again )
};


/*===================================================================
	Structures
===================================================================*/
typedef struct PICKUPATTRIB {

	u_int16_t ModelType;	// if 3D model which one....
	u_int16_t Light;			// do I produce Light
	float	Lightsize;		// how big is the light
	float	R;					// what color does it emit....
	float	G;					// what color does it emit....
	float	B;					// what color does it emit....

} PICKUPATTRIB;

typedef struct PICKUP {

	u_int16_t	Next;				// link list.....	
	u_int16_t	Prev;				// link list.....
	struct PICKUP * NextInGroup;		// Next in same group ....
	struct PICKUP *	PrevInGroup;	// Previous in same group ....
	u_int16_t	Index;
	u_int16_t	Type;				// which type of bullet am I
	u_int16_t	Owner;			// Owner ID
	u_int16_t	ID;				// ID
	int16_t	Mode;				// Mode Pickup is currently in ...
	float	LifeCount;			// How long do i live ...
	float	PickupCount;		// Cannot pickup unless zero...
	VECTOR	Rot;				// Rotation..
	VECTOR	Pos;				// where am I
	VECTOR	Dir;				// where am I looking
	float	Speed;				// how fast am I moving...
	float	ExternalSpeed;	// Speed created by external force
	u_int16_t	Group;			// which group am I in...
	u_int16_t	ModelType;		// if Im a 3D model then I need a model type
	u_int16_t	ModelNum;		// Index into models...
	MATRIX	Mat;				// and a Matrix for when Im displayed...
	u_int16_t	Fmpoly;			// if Im 2D then I need a Face Me Poly..
	float	Xsize;				// x size..
	float	Ysize;				// y size..
	u_int16_t	Light;				// if I produce Light then I Need A Light
	float	Lightsize;			// how big is the light
	float	R;						// what color does it emit....
	float	G;						// what color does it emit....
	float	B;						// what color does it emit....
	QUAT	DirQuat;				// Direction Quaternion
	VECTOR	DirVector;		// Forward Vector
	VECTOR	UpVector;		// Up Vector
	int16_t	RegenSlot;			// Slot from original pickup file ( -1 anywhere )
	u_int16_t	TriggerMod;		// Trigger Mod pointer index
	TRIGGERMOD * TriggerModPtr;	// Trigger Mod Array

	VECTOR	ColStart;			// where I was when started going straight.
	float	ColDist;			// Length from Collision to ColStart.
	u_int16_t	ColFlag;			// have I checked if im going to collide with something
	u_int16_t	ColGroup;			// Collision Group
	VERT	ColPoint;			// where am I going to collide
	NORMAL	ColPointNormal;		// if I reflect use this when I do
	bool	CouldNotPickup;		// Could not pickup last time collided

	float	PickupTime;			// Life count so far for pickup

} PICKUP;

typedef struct REGENPOINT {

	u_int16_t	GenType;			// Generation Type
	u_int16_t	RegenType;			// Regeneration Type
	float	GenDelay;			// Generation Delay ( 60hz )
	float	Lifespan;			// Lifespan of pickup ( 60hz )
	float	Wait;				// Global count
	int16_t	Status;				// Status of slot
	int16_t	Group;				// Group to generate pickup
	VECTOR	Pos;				// Position to generate pickup
	u_int16_t	Type;				// Type of pickup
	u_int16_t	TriggerMod;			// Trigger Mod Index
	u_int16_t	PickupIndex;		// Pickup Index
	u_int16_t	PickupID;			// Pickup ID

} REGENPOINT;

typedef struct QUEDPICKUP {

	int16_t	Type;
	struct	QUEDPICKUP	*	PrevUsed;
	struct	QUEDPICKUP	*	NextUsed;
	struct	QUEDPICKUP	*	PrevFree;
	struct	QUEDPICKUP	*	NextFree;

} QUEDPICKUP;

typedef struct REGENPICKUPINFO {

	int16_t	Required;
	int16_t	Num;

} REGENPICKUPINFO;

typedef struct FREESLOTINFO {

	int16_t	Slot;
	float	Distance;

} FREESLOTINFO;

typedef struct FAILEDKILL {

	int16_t	Next;				// link list.....	
	int16_t	Prev;				// link list.....
	float	Life;
	u_int16_t	Owner;
	u_int16_t	ID;
	int16_t	Style;
	u_int16_t	NewOwner;

} FAILEDKILL;

/*===================================================================
	Prototypes
===================================================================*/
void	InitPickups( void );
u_int16_t	FindFreePickup( void );
void	KillUsedPickup( u_int16_t i );
void	ProcessPickups( void );
void	CleanUpPickup( u_int16_t i );
u_int16_t	InitOnePickup( VECTOR * Pos, u_int16_t Group, VECTOR * Dir, float Speed, int16_t Type,
					   u_int16_t Owner, u_int16_t ID, int16_t RegenSlot, bool Sparkle, float LifeCount, u_int16_t TriggerMod );
u_int16_t	InitJoinPickup( VECTOR * Pos, u_int16_t Group, VECTOR * Dir, float Speed, int16_t Type,
					   u_int16_t Owner, u_int16_t ID, int16_t RegenSlot, bool Sparkle, float LifeCount, u_int16_t TriggerMod );
void	DropPickup( int16_t Type );
void	KillPickup( u_int16_t Owner, u_int16_t ID, int16_t Style );
bool	CollectPickup( u_int16_t i );
void	CheckPickup( void );
void	SavePickupsPositions( void );
bool	LoadPickupsPositions( void );
void	RegeneratePickups( void );
void	RegenerateQuedPickups( void );
bool	RegeneratePickup( u_int16_t Type );
void	GenPickupList( u_int16_t Ship, SHORTPICKUP * PickupSlots, BYTE * NumPickups, BYTE Section );
void	RegenPickupList( SHORTPICKUP * Slots, BYTE Num );
void	GenRegenSlotList( u_int16_t Ship, SHORTREGENSLOT * RegenSlots, BYTE * NumRegenSlots, BYTE Section );
void	RegenRegenSlotList( SHORTREGENSLOT * Slots, BYTE Num );
void	KillAllPickups( void );
void	NewSecondaryWeapon( int16_t Weapon );
void	NewPrimaryWeapon( int16_t Weapon );
void	CopyPickups( u_int16_t Player );
void	AddPickupToRegen( u_int16_t Type );
u_int16_t	InitSlotPickup( u_int16_t Slot );
void	CopyRegenSlots( u_int16_t Player );
u_int16_t	FindClosestPickup( void );
u_int16_t	FindClosestShip( void );
void	SetupPickupGroups( void );
void	AddPickupToGroup( u_int16_t i, u_int16_t Group );
void	RemovePickupFromGroup( u_int16_t i, u_int16_t Group );
void	MovePickupToGroup( u_int16_t i, u_int16_t OldGroup, u_int16_t NewGroup );
void	ClearPickupsGot( void );
bool	FilterPickup( u_int16_t PickupType );
void	PackPickupInfo( u_int32_t *packed );
void	UnpackPickupInfo( u_int32_t *packed );
FILE * SaveAllPickups( FILE * fp );
FILE * LoadAllPickups( FILE * fp );
void KillAllPickupsOfType( u_int16_t Type, int16_t Style );

void InitQuedPickups( void );
bool AddPickupToRegenQue( int16_t Type );
void FreeQuedPickup( QUEDPICKUP * QuedPickup );
void ProcessQuedPickups( void );
void GetFreeSlots( int16_t Type, int16_t * NumRandomSlots, FREESLOTINFO * RandomSlots,
				   int16_t * NumConstantSlots, FREESLOTINFO * ConstantSlots );
bool AddPrimaryToRegenQue( int8_t Weapon );
bool AddSecondaryToRegenQue( int8_t Weapon );
void PickupModelValid( void );
void CheckPickupAllPlayers( void );
bool ActuallyCollectPickup( u_int16_t i );
bool PretendCollectPickup( u_int16_t i );
void CorrectForExtraOrMissingPickups( void );
void CountMinesInLevel( void );
void KillAllPickupsOfTypeAndSend( u_int16_t Type, int16_t Style );

void InitFailedKillSlots( void );
bool AddFailedKillToQue( u_int16_t Owner, u_int16_t ID, int16_t Style, u_int16_t NewOwner);
int16_t FindFreeFailedKillSlot( void );
void ReleaseFailedKillSlot( int16_t i );
void ProcessFailedKills( void );
u_int16_t FindPickup( u_int16_t Owner, u_int16_t ID );

#endif


