/*
 * Secondary.c
 *
 * Missile Routines
 *
 * Project X, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 117 $
 *
 * $Header: /PcProjectX/Include/secondary.h 117   3/07/98 10:17 Collinsd $
 *
 * $Log: /PcProjectX/Include/secondary.h $
 * 
 * 117   3/07/98 10:17 Collinsd
 * Disabled Peacefrog cheat.
 * 
 * 116   2/07/98 14:10 Collinsd
 * Temp Cheat to wind up peacefrom and sint.
 * 
 * 115   26/06/98 10:55 Collinsd
 * Server Pickups and weapons works pretty well, death mode pickups thrown
 * out, as well as scattered.  Quitting/Crash players throw out all
 * pickups.
 * 
 * 114   8/06/98 15:46 Collinsd
 * Done more on Server mode and Fixed trojax ammo usage
 * 
 * 113   5/04/98 19:49 Collinsd
 * Added GlobalFramelagAddition.
 * 
 * 112   3/04/98 16:37 Collinsd
 * Done titan shrapnel as one message,
 * 
 * 111   12/03/98 21:52 Collinsd
 * Added cheatmodes for primary and seconadry weapons.
 * 
 * 110   9/03/98 16:22 Collinsd
 * Mines selectable for dropping as well.
 * 
 * 109   7/03/98 19:10 Collinsd
 * Added new message for olly ( TeamGoals ) Also added pos and group to
 * explodesecondary.
 * 
 * 108   6/03/98 12:10 Collinsd
 * Taken out drop pickup code.
 * 
 * 107   16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 106   4/02/98 19:22 Collinsd
 * Fixed pine mines targeting wrong enemies.  Also fixed pine mines not
 * shooting players in a multiplayer.
 * 
 * 105   30/01/98 13:48 Collinsd
 * Added Depth Charge.
 * 
 * 104   30/01/98 10:58 Collinsd
 * Added Correction code for laser direction
 * 
 * 103   29/01/98 12:06 Collinsd
 * Added fireball, blue photon, tentacle and changed spotfx laser.
 * 
 * 102   1/23/98 9:57a Phillipd
 * 
 * 101   19/01/98 9:09 Collinsd
 * Crushed pickups now regernerate.
 * 
 * 100   17/01/98 12:46 Collinsd
 * Hopefully fixed seconday link list corrupt error.
 * 
 * 99    1/16/98 3:59p Phillipd
 * 
 * 98    15/01/98 10:15 Collinsd
 * Hopefully fixed link list fuckup.  Pickups can only be generated once.
 * Started of fixing mine bug.  Updated MX, MXA and Spotfx formats.
 * 
 * 97    13/01/98 17:37 Collinsd
 * Added back pyrolite fuel,  Changes secondary to try to get rid of any
 * chance of any link list corruption.
 * 
 * 96    13/01/98 9:39 Collinsd
 * Added debug to show untriggered enemies.  Started Enemy Poly Colisions.
 * 
 * 95    3/01/98 19:14 Collinsd
 * Added Splash for secondary weapons
 * 
 * 94    2/01/98 17:52 Collinsd
 * Destroy at end better.
 * 
 * 93    11/12/97 15:53 Collinsd
 * Fixed enemy mines that didn't hurt you.  Also fixed enemy homing
 * missile.
 * 
 * 92    10/12/97 14:19 Collinsd
 * Fixed secondary collision.
 * 
 * 91    4/10/97 12:53 Collinsd
 * Fixed level change. ( Preload Enemies ).
 * 
 * 90    29/09/97 11:35 Collinsd
 * New bgobject stuff enabled. Pickups/Secondary and Enemies now have
 * group link lists.  Flags for secondary bullets being targeted and hit
 * now added.
 * 
 * 89    25/09/97 16:58 Collinsd
 * Added group link list to pickups/secondary added more code for
 * bgobjects.
 * 
 * 88    12/09/97 11:17 Collinsd
 * Mines now tell enemies where they are and collide when close enough
 * 
 * 87    1/09/97 11:37 Collinsd
 * Enemies target ships and ships target enemies/mines when homing.
 * 
 * 86    27/08/97 15:38 Collinsd
 * Taken out old secondary weapon stuff
 * 
 * 85    27/08/97 15:02 Collinsd
 * 
 * 84    13/08/97 15:09 Collinsd
 * Changed framelag trails.
 * 
 * 83    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 82    24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 81    21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 80    13/06/97 16:04 Collinsd
 * Taken out old bgobjects
 * 
 * 79    11-03-97 4:57p Collinsd
 * Changed titan star missile.
 * 
 * 78    30-01-97 3:58p Collinsd
 * 
 * 77    10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 76    6-01-97 11:09a Collinsd
 * All references to timer now replaced with titanstar.
 * 
 * 75    6-01-97 9:11a Collinsd
 * 
 * 74    3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 73    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 72    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 71    22-12-96 8:53p Collinsd
 * Added invisible shockwave to all missiles. hitting walls.
 * 
 * 70    22-12-96 7:27p Collinsd
 * Increased model count.  Decreased scatter viewcone.  Lensflare now
 * works properly on scatter missiles.  Names are printed after the
 * endscene.
 * 
 * 69    21-12-96 5:24p Collinsd
 * Added drop primary/drop secondary weapons.
 * 
 * 68    12/17/96 5:28p Collinsd
 * Added team stuff.
 * 
 * 67    12/14/96 10:44p Collinsd
 * When someone quits the game all pickups scattered. and dissapear under
 * host control.
 * 
 * 66    12/13/96 7:17p Collinsd
 * Added target for targeting computer. ( Disabled at the moment though ).
 * 
 * 65    12/09/96 5:25p Collinsd
 * Started smoke when hull damaged ( still need positions )
 * 
 * 64    12/06/96 4:03p Collinsd
 * Changed timer
 * 
 * 63    12/04/96 2:22p Collinsd
 * Pine Mine missile now works.
 * 
 * 62    25/11/96 12:16 Collinsd
 * Missiles now collide less when moving straight.
 * 
 * 61    22/11/96 12:44 Collinsd
 * Added function to check proximity of mines when regenerating.
 * 
 * 60    15/11/96 16:49 Collinsd
 * Mines now sent across when joining. Also fixed bug in pickup sending.
 * 
 * 59    14/11/96 10:12 Collinsd
 * Added golden powerpod and best mine function.
 * 
 * 58    13/11/96 15:46 Collinsd
 * Added golden powerpod and ljay.
 * 
 * 57    6/11/96 18:42 Collinsd
 * Tidied up secondary weapons.
 * 
 * 56    6/11/96 14:40 Collinsd
 * purge mine now bobs.
 * 
 * 55    31/10/96 16:30 Collinsd
 * Primary/Secondary now included ship updates.  Stealth regeneration
 * fixed
 * 
 * 54    30/10/96 14:34 Collinsd
 * Added stealthmode.
 * 
 * 53    29/10/96 14:49 Collinsd
 * Added big explosion flash.
 * 
 * 52    29/10/96 11:59 Collinsd
 * Added new trail for scatter.
 * 
 * 51    27/10/96 18:38 Collinsd
 * Optimised loads of shit.
 * 
 * 50    26/10/96 18:04 Collinsd
 * Added recoil vector to ihityou.
 * 
 * 49    24/10/96 11:47 Collinsd
 * Added total verts/poly for POLY's and added player
 * name display.
 * 
 * 48    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 47    19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 46    10/10/96 4:18p Phillipd
 * removed mesh.h..no longer used..
 * 
 * 45    7/10/96 13:52 Collinsd
 * Added new smoke trails
 * 
 * 44    23/09/96 14:20 Collinsd
 * Started spider mine.
 * 
 * 43    23/09/96 10:34 Collinsd
 * Pine mine fire pine missile ( Limited to 10 ).  Multiple rocket
 * launcher regeneration bug fixed
 * 
 * 42    20/09/96 17:13 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 41    17/09/96 13:58 Collinsd
 * Added autoselection of primary and secondary weapons.
 * Also you can pickup anything if F12 enabled.
 * 
 * 40    15/09/96 15:43 Collinsd
 * 
 * 39    15/09/96 14:48 Collinsd
 * Scaled primary weapons, changed defauls ammo levels.
 * 
 * 38    14/09/96 16:25 Collinsd
 * Added shockwave to ship death.
 * 
 * 37    14/09/96 15:40 Collinsd
 * Added shield to mines, and damage to missiles.
 * Also when you die your weapons and ammo are dropped
 * 
 * 36    13/09/96 17:04 Collinsd
 * Added quantun fireball mine.
 * 
 * 35    12/09/96 17:52 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 34    11/09/96 19:00 Collinsd
 * Added tests for RAMP emulation ( Although may not work)
 * Also fixed gravity effect to be dependent on GLOBAL_SCALE
 * 
 * 33    9/09/96 20:42 Collinsd
 * No longer used GroupPolyCol or WhichGroupImIn.
 * 
 * 32    7/09/96 17:08 Collinsd
 * Added purge mine and done more on pine mine.
 * 
 * 31    6/09/96 16:20 Collinsd
 * Added extra life pickup, and started pine mine.
 * 
 * 30    5/09/96 15:08 Collinsd
 * Added timer missile.
 * 
 * 29    5/09/96 10:45 Collinsd
 * Fixed next prim/sec weapons selection. 
 * 
 * 28    4/09/96 20:01 Collinsd
 * Scatter missile and thief missile now work!
 * 
 * 27    23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 26    8/21/96 5:49p Phillipd
 * 
 * 25    7/08/96 10:32 Collinsd
 * Added pickup collision with background
 * 
 * 24    2/08/96 16:13 Collinsd
 * Fixed primary weapon net loop bug.  Oooops...
 * 
 * 23    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 22    7/24/96 2:42p Phillipd
 * 
 * 21    21/07/96 14:21 Collinsd
 * Added drop vector to dplay and secondary weapons.
 * 
 * 20    21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 19    17/07/96 15:28 Collinsd
 * Missiles/2DPolys altered to work better
 * 
 * 18    16/07/96 16:05 Collinsd
 * Missiles redone, and bug fixed with collision.
 * 
 * 17    7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 16    15/07/96 14:30 Collinsd
 * Transparency & Translucency work (With hardware)
 * 
 * 15    11/07/96 12:50 Collinsd
 * Drop missiles started
 * 
 * 14    10/07/96 17:10 Collinsd
 * Smoke trails added to missiles, explosions improved.
 * 
 * 13    6/07/96 20:49 Collinsd
 * FmPolys now caluate position of explosion constantly.
 * 
 * 12    6/07/96 16:36 Collinsd
 * Explosion moved away from wall, toward camera.
 * 
 * 11    4/07/96 17:03 Collinsd
 * 
 * 10    4/07/96 16:09 Collinsd
 * 
 * 9     4/07/96 10:34 Collinsd
 * 
 * 8     7/04/96 9:49a Phillipd
 * 
 * 7     2/07/96 16:26 Collinsd
 * Straight/Homing Missile to ship collision (Sphere to Point )
 * 
 * 6     2/07/96 15:20 Collinsd
 * 
 * 5     28/06/96 17:35 Collinsd
 * 
 * 4     28/06/96 10:05 Collinsd
 * 
 * 3     26/06/96 15:55 Collinsd
 * Added homing missiles ( As 2D Sprites )
 * 
 * 2     26/06/96 12:14 Collinsd
 * 
 * 1     25/06/96 15:07 Collinsd
 * 
 */

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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Defines
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Prototypes
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
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
void	EnableSecondaryLights( void );
void	DisableSecondaryLights( void );
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
void	DispEnhancedCrosshair( void );
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


