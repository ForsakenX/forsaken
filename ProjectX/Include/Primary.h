

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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Defines
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct PRIMARYWEAPONATTRIB {

	float		Speed[ MAXPOWERLEVELS ];		// how fast do I move
	float		LifeCount;						// how long can I last..
	uint16		FireDelay[ MAXPOWERLEVELS ];	// how fast can I fire
	float		AmmoUsage[ MAXPOWERLEVELS ];	// how much ammo do i use per shot...
	float		Damage[ MAXPOWERLEVELS ];		// how much damage can i inflict...
	float		ColRadius[ MAXPOWERLEVELS ];	// Collision Radius...
	uint16		ColType;						// Collision Type...
	float		lightsize;						// how big is the light
	float		r[ MAXPOWERLEVELS ];			// what color does it emit....
	float		g[ MAXPOWERLEVELS ];			// what color does it emit....
	float		b[ MAXPOWERLEVELS ];			// what color does it emit....
	int16		FmSeq;							// Faceme Sequence
	FRAME_INFO ** FmFrmInfo;					// Faceme FrameInfo

} PRIMARYWEAPONATTRIB;

typedef struct PRIMARYWEAPONBULLET {
	BOOL		Used;
	uint16		Next;							// link list.....	
	uint16		Prev;							// link list.....
	uint16		Type;							// which type of bullet am I
	uint16		OwnerType;						// who fired me...
	uint16		Owner;							// who fired me...
	void *		EnemyGun;						// Only used by enemies.
	uint16		ID;								// Id from that ship..
	int8		Weapon;							// Weapon type that fired me..
	int16		PowerLevel;						// Power level the weapon has..
	float		TrojPower;						// Trojax Power
	float		LifeCount;						// how long do I live.....
	float		Speed;							// how fast do I move
	float		ColRadius;						// how big is my collide sphere
	uint16		ColType;						// what type of collision
	VECTOR		Offset;							// offset relative to ship that fired me...
	VECTOR		Pos;							// where am I
	VECTOR		Dir;							// where am i going
	VECTOR		LocalDir;						// Local Direction
	VECTOR		UpVector;						// UpVector
	VECTOR		ColStart;						// where I was when started going straight.
	float		ColDist;						// Length from Collision to ColStart.
	uint16		ColFlag;						// have I checked if im going to collide with something
	VERT		ColPoint;						// where am I going to collide
	NORMAL		ColPointNormal;					// if I reflect use this when I do
	uint16		ColGroup;						// where am I going to collide
	uint16		GroupImIn;						// which group am I in...
	MATRIX		Mat;							// and a Matrix for when Im displayed...
	uint16		line;							// attached line
	uint16		fmpoly;							// if Im 2D then I need a Face Me Poly..
	int16		numfmpolys;						// Number of linked fmpolys
	uint16		poly;							// first attached poly
	int16		numpolys;						// Number of attached polys
	uint16		light;							// if I produce Light then I Need A Light
	float		lightsize;						// how big is the light
	float		r;								// what color does it emit....
	float		g;								// what color does it emit....
	float		b;								// what color does it emit....
	uint16		Bounces;						// Number of bounces
	float		TimeInterval;
	float		TimeCount;
	int16		FirePoint;						// gun...
	int16		SpotFX;
	float		FramelagAddition;

} PRIMARYWEAPONBULLET;							// sizes and colours of 2d polys and Lights are set up depoending on type...


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Prototypes
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void	InitPrimBulls(void);
uint16	FindFreePrimBull(void);
void	KillUsedPrimBull( uint16 i );
void	FirePrimary(void);
void	ProcessPrimaryBullets( void );
void	CleanUpPrimBull( uint16 i, BOOL LightFlag  );
uint16 InitOnePrimBull( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag );
void	CreateTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group );
void	CreateNmeTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group );
void	CreatePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmeShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateFireExplosion( VECTOR * Pos, uint16 Group );
void	CreateNmeArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmeLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateNmePowerLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateNmeLightningPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateTransTrail( uint16 i );
void	DispPowerLevel( void );
void	RestoreAmmo( void );
void	RestoreWeapons( void );
void	InitAmmoUsed( void );
void	CreateShieldEffect( VECTOR * Pos, VECTOR * IntPos, VECTOR * IntPos2, uint16 ShipHit, int16 Num,
							 uint8 Red, uint8 Green, uint8 Blue );
uint16	CheckHitShip( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType );
uint16	CheckHitSecondary( VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType );
void	PrimarySelectControl( void );
void	CreateDebugLine( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void	EnablePrimaryLights( void );
void	DisablePrimaryLights( void );
void	FirePrimaryWeapons( uint8 Ship );
void	CreateColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSmallColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CheckCharging( uint16 Ship, uint32 OldFlags, uint32 NewFlags );
void	GivemeAllWeapons( void );
void	LoseAllWeapons( void );
void	DropPrimary( void );
void	GetDeathString( BYTE WeaponType, BYTE Weapon, char * DstString );
void	DropShield( void );
void	DropAmmo( void );
BOOL ChangeTranspulseDir( uint16 i, VECTOR * Pos, VECTOR * MoveOffset, VECTOR * Dir );
void	ChangePulsarDir( uint16 i, VECTOR * Dir );
void	ChangeTrojaxDir( uint16 i, VECTOR * Dir );
void	ChangeSussgunDir( uint16 i, VECTOR * Dir );
void	ChangeOrbitPulsarDir( uint16 i, VECTOR * Dir );
void	ReflectLaser( uint16 i, VECTOR * Dir );
void	GivemeSuperNashram( void );
void	LoseSuperNashram( void );
void	CreateInvEffect( uint16 ShipHit, int16 Num, uint8 Red, uint8 Green, uint8 Blue );
void	CreateSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateMushroomExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNewExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSteam( VECTOR * Pos, VECTOR * Dir, uint16 Group );
uint16	EnemyFirePrimary( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag, void * Gun );
void CreatePowerLaserSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void GetLaserLocalVector( uint16 i, VECTOR * LocalVector );
void CreateScaleExplosion( VECTOR * Pos, uint16 Group, float GlobalScale );
BOOL ValidGroupCollision( VECTOR * StartPos, uint16 StartGroup, VECTOR * IntPoint, VECTOR * ObjPos, uint16 ObjGroup );
int16 GetAmmoPickupToGen( void );
FILE * SavePrimBulls( FILE * fp );
FILE * LoadPrimBulls( FILE * fp );
uint8 GetNextValidPrimaryWeapon( void );
uint8 GetPrevValidPrimaryWeapon( void );
int8 ConvPrimToNormWeapon( int8 Primary );
void TogglePrimaryWeapons( int8 Primary );
void KillPrimBullsByOwner(  uint16 OwnerType, uint16 Owner );
void HostGivemeSuperNashram( uint16 Player );
void HostLoseSuperNashram( uint16 Player );
void HostRestoreWeapons( int16 Player );
void HostRestoreAmmo( int16 Player );
float GetPrimaryAmmoUsage( int16 Ship, int8 Weapon );

#endif // PRIMARY_INCLUDED


