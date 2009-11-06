
/*==========================================================================
 *
 *
 *  File:       Secondary.h
 *  Content:    Secondary include file
 *
 *
 ***************************************************************************/

#ifndef SECONDARY_INCLUDED
#define SECONDARY_INCLUDED

/*===================================================================
	Defines
===================================================================*/
//#define	SINT_PEACEFROG_CHEAT	1

#define	MIS_ACCELERATE		0		// Missile Sequences
#define	MIS_DROP			1		
#define	MIS_STRAIGHT		2		
#define	MIS_HOMING			3		
#define	MIS_GRAVITY			4		
#define	MIS_SPIRALING		5
#define	MIS_RETRACTTENTACLE	6
									
#define	MISMOVE_STRAIGHT	0		// Movement types
#define	MISMOVE_HOMING		1

#define	SEC_MISSILE			0		// Secondary Weapon Type
#define	SEC_MINE			1
#define	SEC_XMISSILE		2

#define	MAXSECONDARYWEAPONBULLETS	256
#define MAXSECONDARYWEAPONS			11

#define MAXENEMYSECONDARYWEAPONS	6
#define	TOTALSECONDARYWEAPONS	( MAXSECONDARYWEAPONS + 2 + MAXENEMYSECONDARYWEAPONS )

#define	MAXSCATTERED		9

#define	MUGMISSILE				0		// Straight
#define	SOLARISMISSILE			1		// Homing
#define	THIEFMISSILE			2		// Straight ( Grab best weapon )
#define	SCATTERMISSILE			3		// Scatter ( Grab all weapons )
#define	GRAVGONMISSILE			4		// Straight ( Gravity effecting )
#define	MULTIPLEMISSILE			5		// Multiple Rocket Launcher
#define	TITANSTARMISSILE		6		// Titan Star ( Create shrapnel on contact )
#define	PURGEMINE				7		// Purge Mine ( Proximity detonated )
#define	PINEMINE				8		// Pine Mine ( Aim and fire )
#define	QUANTUMMINE				9		// Quantum Mine ( Proximity detonated )
#define	SPIDERMINE				10		// Spider Mine ( Proximity detonated, Moving )
#define	PINEMISSILE				11		// Pine Mine Missile
#define	TITANSTARSHRAPNEL		12		// Titan Star Shrapnel
#define	ENEMYSPIRALMISSILE		13		// Straight Spiralling destructable missile
#define	ENEMYHOMINGMISSILE		14		// Homing destructable missile
#define	ENEMYBLUEHOMINGMISSILE	15		// Homing destructable missile ( BLUE )
#define	ENEMYFIREBALL			16		// FireBall
#define	ENEMYTENTACLE			17		// Tentacle
#define	ENEMYDEPTHCHARGE		18		// DepthCharge

#define	NO_SECONDARY		128		// No Secondary weapon to speak of....

#define	COS_CONE			-2.0F	//0.7071F
#define	MAX_ENTRYS			10
#define	MINE_RADIUS			( 256.0F * GLOBAL_SCALE )	// Mine Radius
#define	MINE_TRIGGER_RADIUS	( 800.0F * GLOBAL_SCALE )	// Detonation Radius

#define PURGE_SHOCKWAVE		4.0F
#define PINE_SHOCKWAVE		4.0F
#define	QUANTUM_SHOCKWAVE	20.0F
#define	SPIDER_SHOCKWAVE	4.0F
#define MISSILE_SHOCKWAVE	( 512 * GLOBAL_SCALE )		// Missile Shockwave ( Actual size )
#define	TITANSTAR_SHOCKWAVE	15.0F
#define	TENTACLE_SHOCKWAVE	10.0F
#define DEPTHCHARGE_SHOCKWAVE 10.0F

#define	MAXOLDPOS			10
#define	MAXTENTOLDPOS		20
#define	NUMSCATTERPOINTS	3

#define	SEC_LIGHTSIZE		( 1536.0F * GLOBAL_SCALE )

#define	MAXMISSEDMINES		100
#define	MM_TYPE_KILL		0
#define	MM_TYPE_INIT		1

#define	SECFLAGS_Nothing	0
#define	SECFLAGS_CanShoot	1		// Can be shot.
#define	SECFLAGS_CanTarget	2		// Can be targeted
#define	SECFLAGS_NoIncoming	4		// Dont Play Incoming

#define	SPIRAL_RADIUS		( 100 * GLOBAL_SCALE )

/*===================================================================
	Structures
===================================================================*/
typedef	struct OLDPOS {

	VECTOR		Pos;			// Old Position
	VECTOR		Dir;			// Old Direction
	uint8		R;				// Red
	uint8		G;				// Green
	uint8		B;				// Blue

} OLDPOS;

typedef struct SECONDARYWEAPONATTRIB {

	uint16	SecType;			// Missile/Mine
	uint16	State;				// Starting State
	uint16	MoveType;			// Movement Type ( Straight/Homeing )
	float	StartSpeed;			// how fast do I move to start with
	float	SpeedInc;			// how fast do I Accelerate
	float	Speed;				// how fast do I move
	float	Size;				// how big is my collide sphere
	float	TurnSpeed;			// how quickly can I turn
	uint16	FireDelay;			// how fast can I fire
	uint16	ViewCone;			// how wide is my view
	uint16	ModelNum;			// if 3D model which one....
	float	lightsize;			// how big is the light
	float	r;					// what color does it emit....
	float	g;					// what color does it emit....
	float	b;					// what color does it emit....
	float	Shield;				// How much shield do I have....
	float	Damage;				// How much Damage do I do.....
	BOOL	Lensflare;			// Lensflare......
	float	ColRadius;			// how big is my collide sphere
	uint16	Flags;				// Various Flags;

} SECONDARYWEAPONATTRIB;

typedef struct SECONDARYWEAPONBULLET {
	BOOL	Used;

	uint16	Next;				// link list.....	
	uint16	Prev;				// link list.....
	struct SECONDARYWEAPONBULLET * NextInGroup;// Next in same group ....
	struct SECONDARYWEAPONBULLET * PrevInGroup;// Previous in same group ....
	uint16	Flags;				// Flags
	uint16	Index;				// Index
	uint16	SecType;			// Missile/Mine
	uint16	Type;				// which type of bullet am I
	uint16	MoveType;			// Movement Type ( Straight/Homeing )
	uint16	State;				// which State of bullet am I
	uint16	OwnerType;			// Owner Type
	uint16	Owner;				// who fired me...
	uint16	ID;					// Id from that ship..
	int8	Weapon;				// Weapon that fired me..
	BOOL	Lensflare;			// Do Lensflare
	float	LifeCount;			// how long have i lived.....
	float	LifeSpan;			// how long can i live
	float	SpeedInc;			// how fast do I accelerate...
	float	SpeedWanted;		// how fast do I want to move...
	float	Speed;				// how fast do I move
	float	DropCount;			// Count to drop for..
	VECTOR	DropVector;			// Drop Direction
	float	Size;				// how big is my collide sphere
	VECTOR	Pos;				// where am I
	VECTOR	Offset;
	VECTOR	StartPos;			// where was i when initialised
	VECTOR	StartDir;			// where was i looking when initialised
	MATRIX	StartMat;		
	VECTOR	ColStart;			// where I was when started going straight.
	float	ColDist;			// Length from Collision to ColStart.
	uint16	ColFlag;			// have I checked if im going to collide with something
	uint16	ColGroup;			// Collision Group
	VERT	ColPoint;			// where am I going to collide
	NORMAL	ColPointNormal;		// if I reflect use this when I do
	uint16	GroupImIn;			// which group am I in...
	uint16	ModelNum;			// if Im a 3D model then I need a model description...
	uint16	ModelIndex;			// if Im a 3D model then I need a model description...
	MATRIX	Mat;				// and a Matrix for when Im displayed...
	uint16	fmpoly;				// if Im 2D then I need a Face Me Poly..
	int16	numfmpolys;			// Number of attached fmpolys
	uint16	poly;				// first attached poly
	int16	numpolys;			// Number of attached polys
	float	xsize;				// x size..
	float	ysize;				// y size..
	uint16	light;				// if I produce Light then I Need A Light
	float	lightsize;			// how big is the light
	float	r;					// what color does it emit....
	float	g;					// what color does it emit....
	float	b;					// what color does it emit....
	float	TurnSpeed;			// how quickly can I turn
	float	ViewCone;			// Visibility cone
	uint16	TargetType;			// Ship i'm going for
	uint16	Target;				// Ship i'm going for
	QUAT	DirQuat;			// Direction Quaternion
	VECTOR	DirVector;			// Forward Vector
	VECTOR	UpVector;			// Up Vector
	float	Shield;				// Shield
	float	Damage;				// Damage to do.
	uint16	Ammo;				// Ammo
	int16	NumBounces;			// Bounce Count
	int16	NumOldPos;			// Number of old positions stored.
	float	RetractPos;			// retract pos
	float	ColRadius;			// how big is my collide sphere
	float	Interval;			// Scatter Capture Interval/smoke trail interval
	float	Time;				// Interval left
	OLDPOS	OldPos[ MAXTENTOLDPOS ];// Old positions array;
	float	FramelagAddition;

} SECONDARYWEAPONBULLET;		// sizes and colours of 2d polys and Lights are set up depoending on type...

typedef	struct ENTRY {

	struct ENTRY	*	Prev;	// Previous Entry ( Used / Free )
	struct ENTRY	*	Next;	// Next Entry ( Used / Free )

} ENTRY;

/*===================================================================
	Prototypes
===================================================================*/
void	InitSecBulls(void);
uint16	FindFreeSecBull(void);
void	KillUsedSecBull( uint16 i );
void	FireSecondary(void);
void	ProcessSecondaryBullets( void );
void	CleanUpSecBull( uint16 i );
uint16	InitOneSecBull( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon, BOOL NetFlag );
BOOL	PointToSphere(	VERT * TargPos, float TargRadius, VERT * BulPos );
void	CreateExplosion( VECTOR * Pos, uint16 Group );
void	CreateSmallExplosion( VECTOR * Pos, uint16 Group );
void	CreateSingleExplosion( VECTOR * Pos, uint16 Group );
void	CreateShipExplosion( VECTOR * Pos, uint16 Group );
void	CreateTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateBloodTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateBloodSplat( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateGravgonTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSolarisTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNewTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	InitLinkList( void );
ENTRY *	FindFreeLink( void );
void	FreeLink( ENTRY * Entry );
void	DispSecAmmoLevels( void );
void	CreateGravgonField( uint16 i );
void	CreateShockwave( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, float ShockwaveSize, BYTE Weapon );
void	ExplodeSecondary( VECTOR * Pos, uint16 Group, uint16 OwnerType, uint16 Owner, uint16 ID, float ShockwaveSize );
BOOL	ProcessMines( uint16 i );
BOOL	ProcessMissiles( uint16 i, uint16 * NextMissile );
void	KillOwnersSecBulls( uint16 Owner );
void	ScatterWeapons( VECTOR * Dir, int16 MaxPickupsAllowed );
int16	GenerateSecondaryPickups( int16 NumAllowed );
int16	GetCurSecAmmo( void );
void	SecondarySelectControl( void );
uint16	GetBestMine( void );
void	DropMine( void );
uint16	TargetClosestShip( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos );
BOOL	StillGotTarget( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 TargetType, uint16 Target, float ViewConeCos );
void	CreateScatterTrail( uint16 i );
void	CreateBigFlash( VECTOR * Pos, uint16 Group, float Size );
void	CreateMuzzleFlash( VECTOR * Pos, uint16 Group, float Size );
void	FireSecondaryWeapons( uint8 Ship );
void	DestroySecondary( uint16 i, VECTOR * Int_Point );
void	GenMineList( uint16 Ship, SHORTMINE * MineSlots, BYTE * NumMines, BYTE Section );
void	RegenMineList( SHORTMINE * Slots, BYTE Num );
void	CopyMines( uint16 Player );
void	RegenerateQuedMines( void );
void	KillOwnerIDSecBulls( uint16 OwnerType, uint16 Owner, uint16 ID );
BOOL	CheckForMines( VECTOR * Pos );
void	SyncMines( void );
void	DoDamagedEffects( uint16 i );

void	GetFinalMatrix( MATRIX * FinalMatrix );
void	TargetingComputer( uint16 ClosestShip, uint16 ClosestEnemy );
void	HeatseakerTarget( uint16 ClosestShip, uint16 ClosestEnemy );
void	HUDNames( void );
void	DispHUDNames( void );
void	DropSecondary( void );
void	CreateTitanStarShrapnel( uint16 i, VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSmokeRing( VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, uint16 Group );
uint16	TargetClosestEnemy( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos );
uint16	TargetClosestSecondary( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos );
BOOL	CheckProximityToShips( VECTOR * Pos, uint16 Group, float Radius, uint16 OwnerType, uint16 Owner );
BOOL	CheckProximityToEnemies( VECTOR * Pos, uint16 Group, float Radius, uint16 OwnerType, uint16 Owner );
uint16	GetClosestEnemy( VECTOR * Pos, uint16 Group, float * Radius, uint16 OwnerType, uint16 Owner );
void	CreateBigExplosion( VECTOR * Pos, uint16 Group );
void	CreateSplash( VECTOR * Pos, VECTOR * DirVector, uint16 Group );

void	SetupSecBullGroups( void );
void	AddSecBullToGroup( uint16 i, uint16 Group );
void	RemoveSecBullFromGroup( uint16 i, uint16 Group );
void	MoveSecBullToGroup( uint16 i, uint16 OldGroup, uint16 NewGroup );
void	DispInGameText( VECTOR * Pos, int8 * Text );
void	CreateFireBall( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateTentacle( uint16 i );
uint16	GetSecondaryBulletID( uint16 OwnerType, uint16 Owner );
FILE * SaveSecBulls( FILE * fp );
FILE * LoadSecBulls( FILE * fp );
uint8 GetNextValidSeconadaryWeapon( void );
uint8 GetPrevValidSecondaryWeapon( void );
int8 ConvSecToNormWeapon( int8 Secondary );
void ToggleSecondaryWeapons( int8 Secondary );

void ScatterWeaponsForShip( uint16 Ship, VECTOR * Dir, int16 MaxPickupsAllowed );
int16 HostGenerateSecondaryPickups( uint16 Ship, int16 NumAllowed );
void ShortScatterWeapons( void );

#endif // SECONDARY_INCLUDED


