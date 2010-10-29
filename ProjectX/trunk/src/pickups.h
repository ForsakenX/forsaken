/*==========================================================================
 *  File:       P i c k u p s . h
 *  Content:    Pickups include file
 ***************************************************************************/
#ifndef PICKUPS_INCLUDED
#define PICKUPS_INCLUDED

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

	uint16 ModelType;	// if 3D model which one....
	uint16 Light;			// do I produce Light
	float	Lightsize;		// how big is the light
	float	R;					// what color does it emit....
	float	G;					// what color does it emit....
	float	B;					// what color does it emit....

} PICKUPATTRIB;

typedef struct PICKUP {

	uint16	Next;				// link list.....	
	uint16	Prev;				// link list.....
	struct PICKUP * NextInGroup;		// Next in same group ....
	struct PICKUP *	PrevInGroup;	// Previous in same group ....
	uint16	Index;
	uint16	Type;				// which type of bullet am I
	uint16	Owner;			// Owner ID
	uint16	ID;				// ID
	int16	Mode;				// Mode Pickup is currently in ...
	float	LifeCount;			// How long do i live ...
	float	PickupCount;		// Cannot pickup unless zero...
	VECTOR	Rot;				// Rotation..
	VECTOR	Pos;				// where am I
	VECTOR	Dir;				// where am I looking
	float	Speed;				// how fast am I moving...
	float	ExternalSpeed;	// Speed created by external force
	uint16	Group;			// which group am I in...
	uint16	ModelType;		// if Im a 3D model then I need a model type
	uint16	ModelNum;		// Index into models...
	MATRIX	Mat;				// and a Matrix for when Im displayed...
	uint16	Fmpoly;			// if Im 2D then I need a Face Me Poly..
	float	Xsize;				// x size..
	float	Ysize;				// y size..
	uint16	Light;				// if I produce Light then I Need A Light
	float	Lightsize;			// how big is the light
	float	R;						// what color does it emit....
	float	G;						// what color does it emit....
	float	B;						// what color does it emit....
	QUAT	DirQuat;				// Direction Quaternion
	VECTOR	DirVector;		// Forward Vector
	VECTOR	UpVector;		// Up Vector
	int16	RegenSlot;			// Slot from original pickup file ( -1 anywhere )
	uint16	TriggerMod;		// Trigger Mod pointer index
	TRIGGERMOD * TriggerModPtr;	// Trigger Mod Array

	VECTOR	ColStart;			// where I was when started going straight.
	float	ColDist;			// Length from Collision to ColStart.
	uint16	ColFlag;			// have I checked if im going to collide with something
	uint16	ColGroup;			// Collision Group
	VERT	ColPoint;			// where am I going to collide
	NORMAL	ColPointNormal;		// if I reflect use this when I do
	BOOL	CouldNotPickup;		// Could not pickup last time collided

	float	PickupTime;			// Life count so far for pickup

} PICKUP;

typedef struct REGENPOINT {

	uint16	GenType;			// Generation Type
	uint16	RegenType;			// Regeneration Type
	float	GenDelay;			// Generation Delay ( 60hz )
	float	Lifespan;			// Lifespan of pickup ( 60hz )
	float	Wait;				// Global count
	int16	Status;				// Status of slot
	int16	Group;				// Group to generate pickup
	VECTOR	Pos;				// Position to generate pickup
	uint16	Type;				// Type of pickup
	uint16	TriggerMod;			// Trigger Mod Index
	uint16	PickupIndex;		// Pickup Index
	uint16	PickupID;			// Pickup ID

} REGENPOINT;

typedef struct QUEDPICKUP {

	int16	Type;
	struct	QUEDPICKUP	*	PrevUsed;
	struct	QUEDPICKUP	*	NextUsed;
	struct	QUEDPICKUP	*	PrevFree;
	struct	QUEDPICKUP	*	NextFree;

} QUEDPICKUP;

typedef struct REGENPICKUPINFO {

	int16	Required;
	int16	Num;

} REGENPICKUPINFO;

typedef struct FREESLOTINFO {

	int16	Slot;
	float	Distance;

} FREESLOTINFO;

typedef struct FAILEDKILL {

	int16	Next;				// link list.....	
	int16	Prev;				// link list.....
	float	Life;
	uint16	Owner;
	uint16	ID;
	int16	Style;
	uint16	NewOwner;

} FAILEDKILL;

/*===================================================================
	Prototypes
===================================================================*/
void	InitPickups( void );
uint16	FindFreePickup( void );
void	KillUsedPickup( uint16 i );
void	ProcessPickups( void );
void	CleanUpPickup( uint16 i );
uint16	InitOnePickup( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type,
					   uint16 Owner, uint16 ID, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod );
uint16	InitJoinPickup( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type,
					   uint16 Owner, uint16 ID, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod );
void	DropPickup( int16 Type );
void	KillPickup( uint16 Owner, uint16 ID, int16 Style );
BOOL	CollectPickup( uint16 i );
void	CheckPickup( void );
void	SavePickupsPositions( void );
BOOL	LoadPickupsPositions( void );
void	RegeneratePickups( void );
void	RegenerateQuedPickups( void );
BOOL	RegeneratePickup( uint16 Type );
void	GenPickupList( uint16 Ship, SHORTPICKUP * PickupSlots, BYTE * NumPickups, BYTE Section );
void	RegenPickupList( SHORTPICKUP * Slots, BYTE Num );
void	GenRegenSlotList( uint16 Ship, SHORTREGENSLOT * RegenSlots, BYTE * NumRegenSlots, BYTE Section );
void	RegenRegenSlotList( SHORTREGENSLOT * Slots, BYTE Num );
void	KillAllPickups( void );
void	NewSecondaryWeapon( int16 Weapon );
void	NewPrimaryWeapon( int16 Weapon );
void	CopyPickups( uint16 Player );
void	AddPickupToRegen( uint16 Type );
uint16	InitSlotPickup( uint16 Slot );
void	CopyRegenSlots( uint16 Player );
uint16	FindClosestPickup( void );
uint16	FindClosestShip( void );
void	SetupPickupGroups( void );
void	AddPickupToGroup( uint16 i, uint16 Group );
void	RemovePickupFromGroup( uint16 i, uint16 Group );
void	MovePickupToGroup( uint16 i, uint16 OldGroup, uint16 NewGroup );
void	ClearPickupsGot( void );
BOOL	FilterPickup( uint16 PickupType );
void	PackPickupInfo( uint32 *packed );
void	UnpackPickupInfo( uint32 *packed );
FILE * SaveAllPickups( FILE * fp );
FILE * LoadAllPickups( FILE * fp );
void KillAllPickupsOfType( uint16 Type, int16 Style );

void InitQuedPickups( void );
BOOL AddPickupToRegenQue( int16 Type );
void FreeQuedPickup( QUEDPICKUP * QuedPickup );
void ProcessQuedPickups( void );
void GetFreeSlots( int16 Type, int16 * NumRandomSlots, FREESLOTINFO * RandomSlots,
				   int16 * NumConstantSlots, FREESLOTINFO * ConstantSlots );
BOOL AddPrimaryToRegenQue( int8 Weapon );
BOOL AddSecondaryToRegenQue( int8 Weapon );
void PickupModelValid( void );
void CheckPickupAllPlayers( void );
BOOL ActuallyCollectPickup( uint16 i );
BOOL PretendCollectPickup( uint16 i );
void CorrectForExtraOrMissingPickups( void );
void CountMinesInLevel( void );
void KillAllPickupsOfTypeAndSend( uint16 Type, int16 Style );

void InitFailedKillSlots( void );
BOOL AddFailedKillToQue( uint16 Owner, uint16 ID, int16 Style, uint16 NewOwner);
int16 FindFreeFailedKillSlot( void );
void ReleaseFailedKillSlot( int16 i );
void ProcessFailedKills( void );
uint16 FindPickup( uint16 Owner, uint16 ID );

#endif


