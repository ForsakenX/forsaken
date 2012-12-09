

/*==========================================================================
 *
 *
 *  File:       primary.h
 *  Content:    primary include file
 *
 *
 ***************************************************************************/

#ifndef PRIMARY_INCLUDED
#define PRIMARY_INCLUDED

/*===================================================================
	Defines
===================================================================*/
#define	MAXPRIMARYWEAPONBULLETS	256
#define MAXPRIMARYWEAPONS		6

#define	MAXENEMYPRIMARYWEAPONS	10
#define	TOTALPRIMARYWEAPONS		( MAXPRIMARYWEAPONS + 1 + MAXENEMYPRIMARYWEAPONS )

#define	MAXPOWERLEVELS			3

#define	MAXGENERALAMMO		2000.0F
#define	MAXPYROLITEAMMO		3000.0F
#define	MAXSUSSGUNAMMO		3000.0F
#define	MAXMULTIPLEAMMO		1000.0F

#define	MINIMUM_TOTAL_AMMO	100.0F

#define	GENERALAMMOPERPOD	500.0F
#define	PYROLITEAMMOPERPOD	500.0F
#define	SUSSGUNAMMOPERPOD	500.0F

#define	PULSAR				0
#define	TROJAX				1
#define	PYROLITE_RIFLE		2
#define	TRANSPULSE_CANNON	3
#define	SUSS_GUN			4
#define	LASER				5
#define	ORBITPULSAR			6
#define	NME_BULLET1			7
#define	NME_PULSAR			8
#define	NME_TROJAX			9
#define	NME_PYROLITE		10
#define	NME_TRANSPULSE		11
#define	NME_SUSS_GUN		12
#define	NME_LASER			13
#define	NME_LIGHTNING		14
#define	FLAMES				15
#define	NME_POWERLASER		16

#define	NO_PRIMARY			128

#define	ORBITPULSAR_FMSIZE	( ( 2.0F * GLOBAL_SCALE ) * 0.5F )

#define	PULSAR_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	PULSAR_FMSIZE		( ( 5.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILSTART	( ( 40.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILEND		( ( 20.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILPOWERMUL	( ( 240.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILADDMIN	( ( 376.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 0.5F )

#define	TROJAX_COLRADIUS	( ( 512.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_SIZE			( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPSIZE		( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPMAXSIZE	( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPINC		( ( 0.333F * GLOBAL_SCALE ) * 1.0F )
  
#define	PYROLITE_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	PYROLITE_POWERMUL	( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	PYROLITE_ADDMIN		( ( 0.75F * GLOBAL_SCALE ) * 1.0F )

#define	TRANSPULSE_COLRADIUS ( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_POWERMUL	( ( 2.4F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_ADDMIN	( ( 4.2F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_POWERMUL		( ( 3.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_ADDMIN			( ( 6.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_LENGTH			( ( 12.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_SPARKSIZE ( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_INC	( ( 0.333F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_MINSIZE	( ( 1.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_MAXSIZE	( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_FLARE_SIZE ( ( 10.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_BOUNCES	( 1 )
#define	TRANSPULSE_VIEWCONE	( 160.0F / 2.0F )

#define	ARCTRAIL_POWERMUL	( ( 360.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARCTRAIL_ADDMIN		( ( 580.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_POWERMUL	( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_ADDMIN		( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_FLARESIZE	( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUNLIFE_POWERMUL ( 10.0F )
#define	SUSSGUNLIFE_ADDMIN	 ( 10.0F )

#define	LASER_COLRADIUS		( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_WIDTH_POWERMUL ( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_WIDTH_ADDMIN	( ( 24.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_TEMP_POWERMUL ( 0.5F )
#define	LASER_TEMP_ADDMIN	( 0.5F )

#define	NMEBUL1_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	NMEBUL1_FMSIZE		( ( 5.0F * GLOBAL_SCALE ) * 2.0F )
#define	NMEBUL1_TAILSTART	( ( 40.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILEND		( ( 20.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILPOWERMUL ( ( 240.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILADDMIN	( ( 376.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 0.5F )

#define	SPARKSIZE			( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	SHIP_EXPLO_SIZE		( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	NORM_EXPLO_SIZE		( ( 16.0F * GLOBAL_SCALE ) * 1.0F )

#define	SMOKE_TRAIL_SIZE	( ( 2.0F * GLOBAL_SCALE ) * 1.0F )
#define	GRAVGON_TRAIL_SIZE	( ( 7.0F * GLOBAL_SCALE ) * 1.0F )
#define	GRAVGON_TRAIL_INC	( ( 0.4F * GLOBAL_SCALE ) * 1.0F )
#define	SOLARIS_TRAIL_SIZE	( ( 7.0F * GLOBAL_SCALE ) * 1.0F )
#define	NEW_TRAIL_SIZE		( ( 7.0F * GLOBAL_SCALE ) * 1.0F )

#define	PLASER_COLRADIUS	( ( 200.0F * GLOBAL_SCALE ) * 1.0F )
#define	PLASER_WIDTH_POWERMUL ( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	PLASER_WIDTH_ADDMIN	( ( 48.0F * GLOBAL_SCALE ) * 1.0F )

#define	GRAVGON_SCALE_DEC	0.075F

#define	PICKUP_REGEN_SIZE	( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	BLOODSIZE			( ( 4.0F * GLOBAL_SCALE ) * 1.0F )

#define	FLAMES_COLRADIUS	( ( 500.0F * GLOBAL_SCALE ) * 1.0F )

#define	PULSAR2SHOTPOWERLEVEL	( 1 )

#define	COLTYPE_Point		0
#define	COLTYPE_Sphere		1
#define	COLTYPE_Trojax		2
#define	COLTYPE_Transpulse	3

#define	WEPTYPE_Primary		0
#define	WEPTYPE_Secondary	1
#define	WEPTYPE_Various		2

#define	OWNER_NOBODY		0
#define	OWNER_SHIP			1
#define	OWNER_ENEMY			2
#define	OWNER_BGOBJECT		3
#define	OWNER_MINE			4
#define	OWNER_MODELSPOTFX	5

#define	COLPERS_Forsaken	0					// Forsaken Collision ( shooter Perspective )
#define	COLPERS_Descent	1					// Descent Collision ( target Perspective )

/*===================================================================
	Structures
===================================================================*/
typedef struct PRIMARYWEAPONATTRIB {

	float		Speed[ MAXPOWERLEVELS ];		// how fast do I move
	float		LifeCount;						// how long can I last..
	u_int16_t		FireDelay[ MAXPOWERLEVELS ];	// how fast can I fire
	float		AmmoUsage[ MAXPOWERLEVELS ];	// how much ammo do i use per shot...
	float		Damage[ MAXPOWERLEVELS ];		// how much damage can i inflict...
	float		ColRadius[ MAXPOWERLEVELS ];	// Collision Radius...
	u_int16_t		ColType;						// Collision Type...
	float		lightsize;						// how big is the light
	float		r[ MAXPOWERLEVELS ];			// what color does it emit....
	float		g[ MAXPOWERLEVELS ];			// what color does it emit....
	float		b[ MAXPOWERLEVELS ];			// what color does it emit....
	int16_t		FmSeq;							// Faceme Sequence
	FRAME_INFO ** FmFrmInfo;					// Faceme FrameInfo

} PRIMARYWEAPONATTRIB;

typedef struct PRIMARYWEAPONBULLET {
	bool		Used;
	u_int16_t		Next;							// link list.....	
	u_int16_t		Prev;							// link list.....
	u_int16_t		Type;							// which type of bullet am I
	u_int16_t		OwnerType;						// who fired me...
	u_int16_t		Owner;							// who fired me...
	void *		EnemyGun;						// Only used by enemies.
	u_int16_t		ID;								// Id from that ship..
	int8_t		Weapon;							// Weapon type that fired me..
	int16_t		PowerLevel;						// Power level the weapon has..
	float		TrojPower;						// Trojax Power
	float		LifeCount;						// how long do I live.....
	float		Speed;							// how fast do I move
	float		ColRadius;						// how big is my collide sphere
	u_int16_t		ColType;						// what type of collision
	VECTOR		Offset;							// offset relative to ship that fired me...
	VECTOR		Pos;							// where am I
	VECTOR		Dir;							// where am i going
	VECTOR		LocalDir;						// Local Direction
	VECTOR		UpVector;						// UpVector
	VECTOR		ColStart;						// where I was when started going straight.
	float		ColDist;						// Length from Collision to ColStart.
	u_int16_t		ColFlag;						// have I checked if im going to collide with something
	VERT		ColPoint;						// where am I going to collide
	NORMAL		ColPointNormal;					// if I reflect use this when I do
	u_int16_t		ColGroup;						// where am I going to collide
	u_int16_t		GroupImIn;						// which group am I in...
	MATRIX		Mat;							// and a Matrix for when Im displayed...
	u_int16_t		line;							// attached line
	u_int16_t		fmpoly;							// if Im 2D then I need a Face Me Poly..
	int16_t		numfmpolys;						// Number of linked fmpolys
	u_int16_t		poly;							// first attached poly
	int16_t		numpolys;						// Number of attached polys
	u_int16_t		light;							// if I produce Light then I Need A Light
	float		lightsize;						// how big is the light
	float		r;								// what color does it emit....
	float		g;								// what color does it emit....
	float		b;								// what color does it emit....
	u_int16_t		Bounces;						// Number of bounces
	float		TimeInterval;
	float		TimeCount;
	int16_t		FirePoint;						// gun...
	int16_t		SpotFX;
	float		FramelagAddition;

} PRIMARYWEAPONBULLET;							// sizes and colours of 2d polys and Lights are set up depoending on type...


/*===================================================================
	Prototypes
===================================================================*/
void	InitPrimBulls(void);
u_int16_t	FindFreePrimBull(void);
void	KillUsedPrimBull( u_int16_t i );
void	FirePrimary(void);
void	ProcessPrimaryBullets( void );
void	CleanUpPrimBull( u_int16_t i, bool LightFlag  );
u_int16_t InitOnePrimBull( u_int16_t OwnerType, u_int16_t OwnerID, u_int16_t BulletID, int8_t Weapon,
						u_int16_t Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16_t PowerLevel, float PLevel, bool NetFlag );
void	CreateTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t FmPoly, int16_t Type, u_int16_t Group );
void	CreateNmeTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t FmPoly, int16_t Type, u_int16_t Group );
void	CreatePulsarExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateNmePulsarExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateNmeShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateFireExplosion( VECTOR * Pos, u_int16_t Group );
void	CreateNmeArcExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateArcExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateNmeLaserPulse( u_int16_t i, float Distance, VECTOR * Dir, u_int16_t Group, u_int16_t EndGroup );
void	CreateNmePowerLaserPulse( u_int16_t i, float Distance, VECTOR * Dir, u_int16_t Group, u_int16_t EndGroup );
void	CreateNmeLightningPulse( u_int16_t i, float Distance, VECTOR * Dir, u_int16_t Group, u_int16_t EndGroup );
void	CreateLaserPulse( u_int16_t i, float Distance, VECTOR * Dir, u_int16_t Group, u_int16_t EndGroup );
void	CreateTransTrail( u_int16_t i );
void	DispPowerLevel( void );
void	RestoreAmmo( void );
void	RestoreWeapons( void );
void	InitAmmoUsed( void );
void	CreateShieldEffect( VECTOR * Pos, VECTOR * IntPos, VECTOR * IntPos2, u_int16_t ShipHit, int16_t Num,
							 u_int8_t Red, u_int8_t Green, u_int8_t Blue );
u_int16_t	CheckHitShip( u_int16_t OwnerType, u_int16_t Owner, VECTOR * Pos, u_int16_t Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, u_int16_t ColType );
u_int16_t	CheckHitSecondary( VECTOR * Pos, u_int16_t Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, u_int16_t ColType );
void	PrimarySelectControl( void );
void	CreateDebugLine( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void	FirePrimaryWeapons( u_int8_t Ship );
void	CreateColSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateSmallColSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CheckCharging( u_int16_t Ship, u_int32_t OldFlags, u_int32_t NewFlags );
void	GivemeAllWeapons( void );
void	LoseAllWeapons( void );
void	DropPrimary( void );
void	GetDeathString( BYTE WeaponType, BYTE Weapon, char * DstString );
void	DropShield( void );
void	DropAmmo( void );
bool ChangeTranspulseDir( u_int16_t i, VECTOR * Pos, VECTOR * MoveOffset, VECTOR * Dir );
void	ChangePulsarDir( u_int16_t i, VECTOR * Dir );
void	ChangeTrojaxDir( u_int16_t i, VECTOR * Dir );
void	ChangeSussgunDir( u_int16_t i, VECTOR * Dir );
void	ChangeOrbitPulsarDir( u_int16_t i, VECTOR * Dir );
void	ReflectLaser( u_int16_t i, VECTOR * Dir );
void	GivemeSuperNashram( void );
void	LoseSuperNashram( void );
void	CreateInvEffect( u_int16_t ShipHit, int16_t Num, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void	CreateSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateMushroomExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateNewExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
void	CreateSteam( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
u_int16_t	EnemyFirePrimary( u_int16_t OwnerType, u_int16_t OwnerID, u_int16_t BulletID, int8_t Weapon,
						u_int16_t Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16_t PowerLevel, float PLevel, bool NetFlag, void * Gun );
void CreatePowerLaserSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue );
void GetLaserLocalVector( u_int16_t i, VECTOR * LocalVector );
void CreateScaleExplosion( VECTOR * Pos, u_int16_t Group, float GlobalScale );
bool ValidGroupCollision( VECTOR * StartPos, u_int16_t StartGroup, VECTOR * IntPoint, VECTOR * ObjPos, u_int16_t ObjGroup );
int16_t GetAmmoPickupToGen( void );
FILE * SavePrimBulls( FILE * fp );
FILE * LoadPrimBulls( FILE * fp );
u_int8_t GetNextValidPrimaryWeapon( void );
u_int8_t GetPrevValidPrimaryWeapon( void );
int8_t ConvPrimToNormWeapon( int8_t Primary );
void TogglePrimaryWeapons( int8_t Primary );
void KillPrimBullsByOwner(  u_int16_t OwnerType, u_int16_t Owner );
void HostGivemeSuperNashram( u_int16_t Player );
void HostLoseSuperNashram( u_int16_t Player );
void HostRestoreWeapons( int16_t Player );
void HostRestoreAmmo( int16_t Player );
float GetPrimaryAmmoUsage( int16_t Ship, int8_t Weapon );

#endif // PRIMARY_INCLUDED


