
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
	u_int8_t		R;				// Red
	u_int8_t		G;				// Green
	u_int8_t		B;				// Blue

} OLDPOS;

typedef struct SECONDARYWEAPONATTRIB {

	u_int16_t	SecType;			// Missile/Mine
	u_int16_t	State;				// Starting State
	u_int16_t	MoveType;			// Movement Type ( Straight/Homeing )
	float	StartSpeed;			// how fast do I move to start with
	float	SpeedInc;			// how fast do I Accelerate
	float	Speed;				// how fast do I move
	float	Size;				// how big is my collide sphere
	float	TurnSpeed;			// how quickly can I turn
	u_int16_t	FireDelay;			// how fast can I fire
	u_int16_t	ViewCone;			// how wide is my view
	u_int16_t	ModelNum;			// if 3D model which one....
	float	lightsize;			// how big is the light
	float	r;					// what color does it emit....
	float	g;					// what color does it emit....
	float	b;					// what color does it emit....
	float	Shield;				// How much shield do I have....
	float	Damage;				// How much Damage do I do.....
	bool	Lensflare;			// Lensflare......
	float	ColRadius;			// how big is my collide sphere
	u_int16_t	Flags;				// Various Flags;

} SECONDARYWEAPONATTRIB;

typedef struct SECONDARYWEAPONBULLET {
	bool	Used;

	u_int16_t	Next;				// link list.....	
	u_int16_t	Prev;				// link list.....
	struct SECONDARYWEAPONBULLET * NextInGroup;// Next in same group ....
	struct SECONDARYWEAPONBULLET * PrevInGroup;// Previous in same group ....
	u_int16_t	Flags;				// Flags
	u_int16_t	Index;				// Index
	u_int16_t	SecType;			// Missile/Mine
	u_int16_t	Type;				// which type of bullet am I
	u_int16_t	MoveType;			// Movement Type ( Straight/Homeing )
	u_int16_t	State;				// which State of bullet am I
	u_int16_t	OwnerType;			// Owner Type
	u_int16_t	Owner;				// who fired me...
	u_int16_t	ID;					// Id from that ship..
	int8_t	Weapon;				// Weapon that fired me..
	bool	Lensflare;			// Do Lensflare
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
	u_int16_t	ColFlag;			// have I checked if im going to collide with something
	u_int16_t	ColGroup;			// Collision Group
	VERT	ColPoint;			// where am I going to collide
	NORMAL	ColPointNormal;		// if I reflect use this when I do
	u_int16_t	GroupImIn;			// which group am I in...
	u_int16_t	ModelNum;			// if Im a 3D model then I need a model description...
	u_int16_t	ModelIndex;			// if Im a 3D model then I need a model description...
	MATRIX	Mat;				// and a Matrix for when Im displayed...
	u_int16_t	fmpoly;				// if Im 2D then I need a Face Me Poly..
	int16_t	numfmpolys;			// Number of attached fmpolys
	u_int16_t	poly;				// first attached poly
	int16_t	numpolys;			// Number of attached polys
	float	xsize;				// x size..
	float	ysize;				// y size..
	u_int16_t	light;				// if I produce Light then I Need A Light
	float	lightsize;			// how big is the light
	float	r;					// what color does it emit....
	float	g;					// what color does it emit....
	float	b;					// what color does it emit....
	float	TurnSpeed;			// how quickly can I turn
	float	ViewCone;			// Visibility cone
	u_int16_t	TargetType;			// Ship i'm going for
	u_int16_t	Target;				// Ship i'm going for
	QUAT	DirQuat;			// Direction Quaternion
	VECTOR	DirVector;			// Forward Vector
	VECTOR	UpVector;			// Up Vector
	float	Shield;				// Shield
	float	Damage;				// Damage to do.
	u_int16_t	Ammo;				// Ammo
	int16_t	NumBounces;			// Bounce Count
	int16_t	NumOldPos;			// Number of old positions stored.
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
u_int16_t	FindFreeSecBull(void);
void	KillUsedSecBull( u_int16_t i );
void	FireSecondary(void);
void	ProcessSecondaryBullets( void );
void	CleanUpSecBull( u_int16_t i );
u_int16_t	InitOneSecBull( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon, bool NetFlag );
bool	PointToSphere(	VERT * TargPos, float TargRadius, VERT * BulPos );
void	CreateExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateSmallExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateSingleExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateShipExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateBloodTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateBloodSplat( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateGravgonTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateSolarisTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateNewTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	InitLinkList( void );
ENTRY *	FindFreeLink( void );
void	FreeLink( ENTRY * Entry );
void	DispSecAmmoLevels( void );
void	CreateGravgonField( u_int16_t i );
void	CreateShockwave( u_int16_t OwnerType, u_int16_t Owner, VECTOR * Pos, u_int16_t Group, float ShockwaveSize, BYTE Weapon );
void	ExplodeSecondary( VECTOR * Pos, u_int16_t Group, u_int16_t OwnerType, u_int16_t Owner, u_int16_t ID, float ShockwaveSize );
bool	ProcessMines( u_int16_t i );
bool	ProcessMissiles( u_int16_t i, u_int16_t * NextMissile );
void	KillOwnersSecBulls( u_int16_t Owner );
void	ScatterWeapons( VECTOR * Dir, int16_t MaxPickupsAllowed );
int16_t	GenerateSecondaryPickups( int16_t NumAllowed );
int16_t	GetCurSecAmmo( void );
void	SecondarySelectControl( void );
u_int16_t	GetBestMine( void );
void	DropMine( void );
u_int16_t	TargetClosestShip( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos );
bool	StillGotTarget( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t TargetType, u_int16_t Target, float ViewConeCos );
void	CreateScatterTrail( u_int16_t i );
void	CreateBigFlash( VECTOR * Pos, u_int16_t Group, float Size );
void	CreateMuzzleFlash( VECTOR * Pos, u_int16_t Group, float Size );
void	FireSecondaryWeapons( u_int8_t Ship );
void	DestroySecondary( u_int16_t i, VECTOR * Int_Point );
void	GenMineList( u_int16_t Ship, SHORTMINE * MineSlots, BYTE * NumMines, BYTE Section );
void	RegenMineList( SHORTMINE * Slots, BYTE Num );
void	CopyMines( u_int16_t Player );
void	RegenerateQuedMines( void );
void	KillOwnerIDSecBulls( u_int16_t OwnerType, u_int16_t Owner, u_int16_t ID );
bool	CheckForMines( VECTOR * Pos );
void	SyncMines( void );
void	DoDamagedEffects( u_int16_t i );

void	GetFinalMatrix( MATRIX * FinalMatrix );
void	TargetingComputer( u_int16_t ClosestShip, u_int16_t ClosestEnemy );
void	HeatseakerTarget( u_int16_t ClosestShip, u_int16_t ClosestEnemy );
void	HUDNames( void );
void	DispHUDNames( void );
void	DropSecondary( void );
void	CreateTitanStarShrapnel( u_int16_t i, VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateSmokeRing( VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, u_int16_t Group );
u_int16_t	TargetClosestEnemy( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos );
u_int16_t	TargetClosestSecondary( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos );
bool	CheckProximityToShips( VECTOR * Pos, u_int16_t Group, float Radius, u_int16_t OwnerType, u_int16_t Owner );
bool	CheckProximityToEnemies( VECTOR * Pos, u_int16_t Group, float Radius, u_int16_t OwnerType, u_int16_t Owner );
u_int16_t	GetClosestEnemy( VECTOR * Pos, u_int16_t Group, float * Radius, u_int16_t OwnerType, u_int16_t Owner );
void	CreateBigExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateSplash( VECTOR * Pos, VECTOR * DirVector, u_int16_t Group );

void	SetupSecBullGroups( void );
void	AddSecBullToGroup( u_int16_t i, u_int16_t Group );
void	RemoveSecBullFromGroup( u_int16_t i, u_int16_t Group );
void	MoveSecBullToGroup( u_int16_t i, u_int16_t OldGroup, u_int16_t NewGroup );
void	DispInGameText( VECTOR * Pos, int8_t * Text );
void	CreateFireBall( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateTentacle( u_int16_t i );
u_int16_t	GetSecondaryBulletID( u_int16_t OwnerType, u_int16_t Owner );
FILE * SaveSecBulls( FILE * fp );
FILE * LoadSecBulls( FILE * fp );
u_int8_t GetNextValidSeconadaryWeapon( void );
u_int8_t GetPrevValidSecondaryWeapon( void );
int8_t ConvSecToNormWeapon( int8_t Secondary );
void ToggleSecondaryWeapons( int8_t Secondary );

void ScatterWeaponsForShip( u_int16_t Ship, VECTOR * Dir, int16_t MaxPickupsAllowed );
int16_t HostGenerateSecondaryPickups( u_int16_t Ship, int16_t NumAllowed );

#endif // SECONDARY_INCLUDED


