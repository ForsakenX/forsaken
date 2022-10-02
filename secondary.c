

/*===================================================================
*	s e c o n d . c
*	All routines to do with Secondary weapons...
===================================================================*/
#include <stdio.h>
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "screenpolys.h"
#include "polys.h"
#include "ships.h"
#include "mload.h"
#include "collision.h"
#include "lights.h"
#include "2dpolys.h"
#include "models.h"
#include "sphere.h"
#include "controls.h"
#include "visi.h"
#include "sfx.h"
#include "triggers.h"
#include "pickups.h"
#include "enemies.h"
#include "secondary.h"
#include "primary.h"
#include "title.h"
#include "text.h"
#include "spotfx.h"
#include "ai.h"
#include "water.h"
#include "local.h"
#include "util.h"
#include "timer.h"

#define	SCATTER_TEST	0

#define	MISSILE_SPEED	( MAXMOVESPEED * 1.6F )			// Ship Speed * 1.6

#define TITANBITS_SEND  1

#include "localtitle.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Externs
===================================================================*/
extern SLIDER BikeCompSpeechSlider;
extern bool CaptureTheFlag;
extern bool CTF;
extern bool BountyHunt;
extern int8_t TeamFlagPickup[ MAX_TEAMS ];
extern u_int32_t TeamFlagMask[ MAX_TEAMS ];
extern char *TeamName[ MAX_TEAMS ];
extern int16_t			PickupsGot[ MAXPICKUPTYPES ];
extern	FRAME_INFO	*	Flare_Header;
extern	FRAME_INFO	*	Trojax_Exp_Header;
extern	FRAME_INFO	*	Pulsar_Exp_Header;
extern	FRAME_INFO	*	Bits_Header;
extern	SHIPCONTROL		control;
extern	RENDERMATRIX		view;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
//extern	D3DTEXTUREHANDLE bTex;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	XLIGHT			XLights[MAXXLIGHTS];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	POLY			Polys[MAXPOLYS];
extern	MODEL			Models[MAXNUMOFMODELS];
extern	float			framelag;
extern	BYTE			WhoIAm;
extern	bool            bSoundEnabled;
extern	FRAME_INFO	*	Exp_Header;
extern	FRAME_INFO	*	Exp2_Header;
extern	FRAME_INFO	*	GravgonTrail_Header;
extern	FRAME_INFO	*	SolarisTrail_Header;
extern	FRAME_INFO	*	NewTrail_Header;
extern	FRAME_INFO	*	Blood_Header;
extern	FRAME_INFO	*	BloodSplat_Header;
extern	FRAME_INFO	*	BloodDribble_Header;
extern	FRAME_INFO	*	Laser_Header;
extern	FRAME_INFO	*	ScatterTrail_Header;
extern	FRAME_INFO	*	GreyFlare_Header;
extern	FRAME_INFO	*	BigFlash_Header;
extern	FRAME_INFO	*	Target_Header;
extern	FRAME_INFO	*	LensFlare_Header;
extern	FRAME_INFO	*	BlueSpark_Header;
extern	FRAME_INFO	*	Circle_Header;
extern	FRAME_INFO	*	Tentacle_Header;
extern	MATRIX			MATRIX_Identity;
extern	int16_t			PrimaryWeaponsGot[ MAXPRIMARYWEAPONS ];
extern	USERCONFIG	*	player_config;
extern	bool            IsHost;
extern	float			NitroFuel;
extern	float			NitroFuelUsed;
extern	int16_t			NumStealths;
extern	int16_t			NumOrbs;
extern	u_int16_t			CameraMissile;
extern	int				MissileCameraActive;
extern	MATRIX			ProjMatrix;
extern	CAMERA			CurrentCamera;
extern	MISSEDMINEMSG	MissedMines[ MAXMISSEDMINES ];
extern	SHORTMINE		MissedInitMines[ MAXMISSEDMINES ];
extern	SHORTKILLMINE	MissedKillMines[ MAXMISSEDMINES ];
extern	int16_t			NumMissedMines;
extern	int16_t			NumMissedInitMines;
extern	int16_t			NumMissedKillMines;
extern	int16_t			NumPowerPods;
extern	BYTE			GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
																	// this tells the drones what status the host thinks hes in..

extern	SCRPOLY			ScrPolys[ MAXNUMOFSCRPOLYS ];
extern	PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern	bool			TeamGame;
extern	BYTE			TeamNumber[MAX_PLAYERS];
extern	bool			DebugInfo;
extern	bool			GodMode;
extern	u_int16_t			IsGroupVisible[MAXGROUPS];
extern	bool			PickupInvulnerability;
extern	int16_t			NumInvuls;
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	ENEMY	*		FirstEnemyUsed;
extern	ENEMY			Enemies[ MAXENEMIES ];
extern	ENEMY_TYPES		EnemyTypes[ MAX_ENEMY_TYPES ];
extern	int				no_collision;		// disables player ship-to-background collisions
extern	int				outside_map;
extern	float			NmeDamageModifier;
extern	int32_t			ColPerspective;
extern	bool			CaptureTheFlag;
extern	bool			BountyHunt;
extern	float			GlobalFramelagAddition;
extern	bool			PlayDemo;
extern	int16_t	Host_PrimaryWeaponsGot[ MAX_PLAYERS ][ MAXPRIMARYWEAPONS ];
extern	int16_t	Host_SecondaryWeaponsGot[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	float	Host_GeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmo[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_GeneralAmmoUsed;
extern	float	Host_PyroliteAmmoUsed;
extern	float	Host_SussGunAmmoUsed;
extern	int16_t	Host_SecondaryAmmo[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	int16_t	Host_SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	int8_t	Host_SecWeaponsGot[ MAX_PLAYERS ][ MAXPICKUPS ];
extern	float	Host_NitroFuel[ MAX_PLAYERS ];
extern	float	Host_NitroFuelUsed;
extern	int16_t	Host_PowerLevel[ MAX_PLAYERS ];
extern	BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
extern	BYTE	Host_CopyOfNumOfOrbitals[ MAX_PLAYERS ];
extern	float	Host_CopyOfOrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	int16_t	Host_CopyOfPowerLevel[ MAX_PLAYERS ];
extern	float	Host_CopyOfNitroFuel[ MAX_PLAYERS ];
extern	float	Host_CopyOfGeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfSussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfPyroliteAmmo[ MAX_PLAYERS ];
extern	int16_t	Host_PickupsGot[ MAX_PLAYERS ][ MAXPICKUPTYPES ];
extern  u_int32_t  Host_Flags[ MAX_PLAYERS ];
extern	int16_t	NumSuperNashrams;
extern	PICKUP	Pickups[ MAXPICKUPS ];
extern	float	RegenDelay;

extern render_info_t render_info;

// custom colour messages (Title.c)
extern int PickupMessageColour;
// player selected for watch mode - used to display their missile camera (Title.c)
extern SLIDER WatchPlayerSelect;
// player selected in demo mode - used to display their missile camera (Title.c)
extern SLIDER DemoEyesSelect;
extern bool ShowPlayerHealthAboveBikes;
extern bool ScaleFontPlayerHealthAboveBikes;
// (networking.c)
extern SHIPHEALTHMSG PlayerHealths[ MAX_PLAYERS+1 ];
extern u_int8_t ShipHealthColour[ MAX_PLAYERS+1 ];

/*===================================================================
	Globals
===================================================================*/
/*
char *SecondaryDescription[MAXSECONDARYWEAPONS] = {

	"Mug",
	"Solaris",
	"Thief",
	"Scatter",
	"Gravgon",
	"MFRL",
	"Titan",
	"Purge Mine",
	"Pine Mine",
	"Quantum Mine",
	"Spider Mine"

};
*/
char *SecondaryDescription[MAXSECONDARYWEAPONS] = {
	LT_MENU_NEW_SecondaryWeaponKeys17 /*"mug missile:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys18 /*"solaris missile:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys29 /*"Thief"*/,
	LT_MENU_NEW_SecondaryWeaponKeys5 /*"scatter missile"*/,
	LT_MENU_NEW_SecondaryWeaponKeys6 /*"gravgon missile"*/,
	LT_MENU_NEW_SecondaryWeaponKeys21 /*"mfrl:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys22 /*"Titan Missile:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys23 /*"purge mine:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys24 /*"pine mine:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys25 /*"quantum mine:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys30 /*"spider mine"*/,
};

// is weapon valid as far as order processing & keydefs concerned?
bool SecondaryValid[ MAXSECONDARYWEAPONS ] =
{

	true,	// mug
	true,	// solaris
	true,	// thief
	true,	// scatter
	true,	// gravgon
	true,	// mfrl
	true,	// titan
	true,	// purge
	true,	// pine
	true,	// quantum
	true,	// spider

};

ENTRY		EntryList[ MAX_ENTRYS ];
ENTRY	*	FirstFree = &EntryList[ 0 ];
ENTRY	*	FirstUsed = NULL;

SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
SHORTMINE	MinesCopy[ MAX_PLAYERS ][ MAXSECONDARYWEAPONBULLETS ];
u_int16_t		FirstSecBullUsed;
u_int16_t		FirstSecBullFree;
float		SecondaryFireDelay = 0.0F;

int16_t		SecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
int8_t		SecWeaponsGot[ MAXPICKUPS ];
int16_t		SecAmmoUsed[ MAXSECONDARYWEAPONS ];
int16_t		SecondaryAmmo[ MAXSECONDARYWEAPONS ];
bool		TargetComputerOn = false;

int16_t		NumSecBullsPerGroup[ MAXGROUPS ];
SECONDARYWEAPONBULLET *	SecBullGroups[ MAXGROUPS ];

bool		ImTargeted = false;
float		TargetedDelay = 0.0F;
float		MissileDistance = 0.0F;
u_int16_t		MissileNum;

bool		ShowNamesAnyway = false;

u_int16_t		GlobalSecBullsID = 0;

int8_t	*	DebugSecStrings[] = {

			"Mug Missile",					// Secondary Weapons
			"Heatseeker Missile",
			"Thief Missile",
			"Scatter Missile",
			"Gravgon Missile",
			"Rocket Launcher",
			"TitanStar Missile",
			"Purge Mine",
			"Pine Mine",
			"Quantum Mine",
			"Spider Mine",
			"Pine Mine Missile",
			"TitanStar Shrapnel",
			"Enemy Spiral",
			"Photon Missile",
			"Blue Photon Missile",
			"FireBall",
};

int16_t SecondarySfxLookup [ MAXSECONDARYWEAPONS ] = 
{
	SFX_Select_MugMissile,
	SFX_Select_SolarisMissile,
	SFX_DontHaveThat, // no sfx for thief missile
	SFX_Select_ScatterMissile,
	SFX_Select_GravgonMissile,
	SFX_Select_MFRL,
	SFX_Select_TitanStarMissile,
	SFX_Select_PurgeMine,
	SFX_Select_PineMine,
	SFX_Select_QuantumMine,
	SFX_BIKECOMP_SP,
};

CAMERA		TempCamera;

int8_t SecondaryToFireLookup[ MAXSECONDARYWEAPONS ] = {

	MUGMISSILE,
	SOLARISMISSILE,
	THIEFMISSILE,
	SCATTERMISSILE,
	GRAVGONMISSILE,
	MULTIPLEMISSILE,
	TITANSTARMISSILE,
	PURGEMINE,
	PINEMINE,
	QUANTUMMINE,
	SPIDERMINE,
};

bool	SecondaryWeaponCheat = false;

#ifdef SINT_PEACEFROG_CHEAT
int16_t	TargetingWeaponCheat = -1;
#endif

SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[ TOTALSECONDARYWEAPONS ] = {

/*===================================================================
	0 Mug Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Mug,		// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	1 Solaris Heatseeker Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_HOMING,	// u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		1.5F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		120,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Heatseaker, // u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	2 Thief Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_HOMING, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		3.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
//		MODEL_Thief,	// u_int16_t	ModelNum;			// if 3D model which one....
		MODEL_Eyeball,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	3 Scatter Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_HOMING, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#if SCATTER_TEST
		3.0F,			// float	TurnSpeed			// how fase can I turn
#else
		2.0F,			// float	TurnSpeed			// how fase can I turn
#endif
		30,				// u_int16_t	FireDelay			// how fast can I fire
#if SCATTER_TEST
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
#else
		180,			// u_int16_t	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_Scatter,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		true,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	4 Gravgon Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F ,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Gravgon,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	5 Multiple Missile Launcher
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_DROP,		//u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		MISSILE_SPEED,	// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 2.0F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		1,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_PineMissile,	// u_int16_t	ModelNum;			// if 3D model which one....
		0.0F,			// float	lightsize;	// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		192.0F,			// float	g;					// what color does it emit....
		0.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		32.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	6 TitanStar Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
#ifdef SINT_PEACEFROG_CHEAT
		MISMOVE_HOMING,	// u_int16_t	Move Type			// Straight/Homing
#else
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
#endif
		MISSILE_SPEED * 0.75F,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED * 0.75F,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#ifdef SINT_PEACEFROG_CHEAT
		2.0F,			// float	TurnSpeed			// how fase can I turn
#else
		0.0F,			// float	TurnSpeed			// how fase can I turn
#endif
		30,				// u_int16_t	FireDelay			// how fast can I fire
#ifdef SINT_PEACEFROG_CHEAT
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
#else
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_TitanStar,// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE * 1.5F,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		256.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	7 Purge Mine
===================================================================*/
	{
		SEC_MINE,		// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Purge,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		1.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},

/*===================================================================
	8 Pine Mine
===================================================================*/
	{
		SEC_MINE,		// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.75F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Pine,		// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		400.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*===================================================================
	9 Quantum Mine
===================================================================*/
	{
		SEC_MINE,		// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Quantum,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		32.0F,			// float	r;					// what color does it emit....
		32.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		40.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*===================================================================
	10 Spider Mine
===================================================================*/
	{
		SEC_MINE,		// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
//		MODEL_Spider,	// u_int16_t	ModelNum;			// if 3D model which one....
		MODEL_Eyeball,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		32.0F,			// float	r;					// what color does it emit....
		32.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		40.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*===================================================================
	11 Pine Mine Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		MISSILE_SPEED,	// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 2.0F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_PineMissile,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		20.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},
/*===================================================================
	12 Titan Star Shapnel Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
#ifdef SINT_PEACEFROG_CHEAT
		MISMOVE_HOMING,	// u_int16_t	Move Type			// Straight/Homing
#else
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
#endif
		( MISSILE_SPEED * 1.25F ),	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 1.25F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#ifdef SINT_PEACEFROG_CHEAT
		3.5F,			// float	TurnSpeed			// how fast can I turn
#else
		0.0F,			// float	TurnSpeed			// how fast can I turn
#endif
		30,				// u_int16_t	FireDelay			// how fast can I fire
#ifdef SINT_PEACEFROG_CHEAT
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
#else
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_TitanStarShrapnel,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		20.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		SHIP_RADIUS,	// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},

/*===================================================================
	13 Enemy Spiraling Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_SPIRALING,	// u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.75F ),	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.75F ),	// float	StartSpeed;			// how fast do I move to start with
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		0,				// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_EnemySpiralMissile, // u_int16_t	ModelNum;	// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		1.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot,						 		// Initial Flags
	},
/*===================================================================
	14 Enemy Homing Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_HOMING,	// u_int16_t	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		(u_int16_t) -1,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot, // u_int16_t Flags				// Initial Flags
	},
/*===================================================================
	15 Enemy Blue Homing Missile
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_HOMING,	// u_int16_t	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		(u_int16_t) -1,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		128.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot, // u_int16_t Flags				// Initial Flags
	},
/*===================================================================
	16 Enemy FireBall
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	// u_int16_t	State				// Starting state
		MISMOVE_STRAIGHT, // u_int16_t	Move Type			// Straight/Homing
		( MISSILE_SPEED * 1.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 1.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		(u_int16_t) -1,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		85.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // u_int16_t Flags				// Initial Flags
	},
/*===================================================================
	17 Enemy Tentacle
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_STRAIGHT,	//u_int16_t	State				// Starting state
		MISMOVE_HOMING, // u_int16_t	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Eyeball,	// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_NoIncoming, // u_int16_t Flags				// Initial Flags
	},
/*===================================================================
	18 DepthCharge
===================================================================*/
	{
		SEC_MISSILE,	// u_int16_t	SecType				// Missile/Mine
		MIS_GRAVITY,	//u_int16_t	State				// Starting state
		MISMOVE_HOMING, // u_int16_t	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.16F ),	// float	StartSpeed;		// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.16F ),	// float	Speed;	// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// u_int16_t	FireDelay			// how fast can I fire
		360,			// u_int16_t	ViewCone			// View Cone ( Degrees )
		MODEL_Purge,		// u_int16_t	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		false,			// bool		Lensflare			// Lensflare
		( SHIP_RADIUS * 0.5F ),	// float	ColRadius	// Collision Radius
		SECFLAGS_NoIncoming, // u_int16_t Flags			// Initial Flags
	},
};

/*===================================================================
	Procedure	:	Set up And Init all SecBulls
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void	InitSecBulls(void)
{
	u_int16_t	i;

	FirstSecBullUsed = (u_int16_t) -1;
	FirstSecBullFree = 0;

	SetupSecBullGroups();

	for( i = 0 ; i < MAXSECONDARYWEAPONBULLETS ; i++ )
	{
		SecBulls[ i ].Used = false;
		SecBulls[ i ].Next = i + 1;
		SecBulls[ i ].Prev = (u_int16_t) -1;
		SecBulls[ i ].NextInGroup = NULL;
		SecBulls[ i ].PrevInGroup = NULL;
		SecBulls[ i ].State = MIS_STRAIGHT;
		SecBulls[ i ].Flags = SECFLAGS_Nothing;
		SecBulls[ i ].Index = i;
		SecBulls[ i ].Type = (u_int16_t) -1;
		SecBulls[ i ].SecType = SEC_MISSILE;
		SecBulls[ i ].DropCount = 0.0F;
		SecBulls[ i ].MoveType = MISMOVE_STRAIGHT;
		SecBulls[ i ].Owner = (u_int16_t) -1;
		SecBulls[ i ].LifeCount = 0.0F;
		SecBulls[ i ].ColFlag = 0;
		SecBulls[ i ].GroupImIn = (u_int16_t) -1;
		SecBulls[ i ].ModelNum = (u_int16_t) -1;
		SecBulls[ i ].ModelIndex = (u_int16_t) -1;
		SecBulls[ i ].fmpoly = (u_int16_t) -1;
		SecBulls[ i ].numfmpolys = 0;
		SecBulls[ i ].poly = (u_int16_t) -1;
		SecBulls[ i ].numpolys = 0;
		SecBulls[ i ].light = (u_int16_t) -1;
		SecBulls[ i ].Target = (u_int16_t) -1;
		SecBulls[ i ].TargetType = (u_int16_t) -1;
		SecBulls[ i ].SpeedWanted = 32.0F;
		SecBulls[ i ].SpeedInc = 32.0F;
		SecBulls[ i ].Speed = 32.0F;
		SecBulls[ i ].TurnSpeed = 0.0F;
		SecBulls[ i ].ViewCone = 0.0F;
		SecBulls[ i ].DirVector = Forward;
		SecBulls[ i ].UpVector = SlideUp;
		SecBulls[ i ].DropVector = SlideDown;
		SecBulls[ i ].NumOldPos = 0;
		QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &SecBulls[ i ].DirVector );
		QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
	}
	SecBulls[MAXSECONDARYWEAPONBULLETS-1].Next = (u_int16_t) -1;
}
/*===================================================================
	Procedure	:	Find a free SecBull and move it from the free list to
					the used list
	Input		:	nothing
	Output		:	u_int16_t number of SecBull free....
===================================================================*/
u_int16_t	FindFreeSecBull(void)
{
	u_int16_t i;

	i = FirstSecBullFree;
	
	if ( i == (u_int16_t) -1)
		return i;

	if( SecBulls[i].Used )
	{
		// This Secondary Bullet has been Used before....
		Msg( "%s Bullet has been Used more than once\n", DebugSecStrings[SecBulls[i].Type] );
	}
	
	
	SecBulls[i].Prev = FirstSecBullUsed;
	if ( FirstSecBullUsed != (u_int16_t) -1)
	{
		SecBulls[FirstSecBullUsed].Next = i;
	}
	FirstSecBullUsed = i;
	FirstSecBullFree = SecBulls[i].Next;
	SecBulls[i].Used = true;
	return i ;
}

/*===================================================================
	Procedure	:	Kill a used SecBull and move it from the used list to
					the free list
	Input		:	u_int16_t number of SecBull free....
	Output		:	nothing
===================================================================*/
void	KillUsedSecBull( u_int16_t i )
{
	u_int16_t	its_prev;
	u_int16_t	its_next;
	

	if( !SecBulls[i].Used )
	{
		// This Secondary Bullet has been Freed before....
		Msg( "%s Bullet has been Freed more than once\n", DebugSecStrings[SecBulls[i].Type] );
	}
	
	its_prev = SecBulls[i].Prev;
	its_next = SecBulls[i].Next;

	if ( i == FirstSecBullUsed )
		FirstSecBullUsed = SecBulls[i].Prev;

	if( its_prev != (u_int16_t) -1)
		SecBulls[its_prev].Next = its_next;

	if( its_next != (u_int16_t) -1)
		SecBulls[its_next].Prev = its_prev;

	SecBulls[i].Prev = (u_int16_t) -1;
	SecBulls[i].Next = FirstSecBullFree;
	FirstSecBullFree	= i;
	SecBulls[i].Used = false;

}

/*===================================================================
	Procedure	:	Check if a new bullet is due....if it is init it
	Input		:	nothing
	Output		:	nothing
===================================================================*/
static	int16_t	Toggle = 0;

void FireSecondary( void )
{
static int16_t	OnceOnlyFlag = 0;
	VECTOR	UpVector;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	DropVector;
	VECTOR	recoil;
	int		firing;

	firing = 0;

#if SCATTER_TEST
	int16_t	Count;
#endif
	RegenerateQuedMines();

	if( WhoIAm == MAX_PLAYERS ) return;

	if( control.drop_secondary )
	{
		DropSecondary();
	}

	if( control.select_secondary )
	{
		if( SecondaryWeaponsGot[ ( control.select_secondary - 1 ) ] )
		{
			if( SecondaryAmmo[ ( control.select_secondary - 1 ) ] )
			{
				if( !OnceOnlyFlag )
				{
					if( SecondaryWeaponCheat )
					{
						if( Ships[ WhoIAm ].Secondary == ( control.select_secondary - 1 ) )
						{
							ToggleSecondaryWeapons( Ships[ WhoIAm ].Secondary );
						}
					}

					OnceOnlyFlag = 1;
					Ships[ WhoIAm ].Secondary = ( control.select_secondary - 1 );
					if( BikeCompSpeechSlider.value )
					{
						PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
					}else
					{
						PlaySfx( SFX_WeaponSelect, 1.0F );
					}
				}
			}
		}
		else
		{
			if( !OnceOnlyFlag )
			{
				OnceOnlyFlag = 1;
				if( !bSoundEnabled ) AddColourMessageToQue( PickupMessageColour, YOU_DONT_HAVE_THAT_WEAPON );
				PlaySfx( SFX_DontHaveThat, 1.0F );
			}
		}
	}
	else
	{
		OnceOnlyFlag = 0;
	}

	if( control.select_next_secondary )
	{
		Ships[ WhoIAm ].Secondary = GetNextValidSeconadaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	if( control.select_prev_secondary )
	{
		Ships[ WhoIAm ].Secondary = GetPrevValidSecondaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	SecondarySelectControl();

	if( control.fire_mine )
	{
		DropMine();
		SecondaryFireDelay -= framelag;													// Added because mines could
		return;																			// be dropped the same time as
																						// firing secondary missiles
	}

	if ( SecondaryFireDelay <= 0.0F )
	{
		if( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE )
		{
			if( control.fire_secondary )
			{
				SecondaryFireDelay += (float) SecondaryWeaponAttribs[ SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ] ].FireDelay;
	
				if( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] )
				{
					firing = 1;
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
	
					switch( Ships[WhoIAm].Secondary )
					{
						case PURGEMINE:
						case PINEMINE:
						case SPIDERMINE:
						case QUANTUMMINE:
							break;

						default:
							Ships[ WhoIAm ].Object.Flags |= SHIP_SecFire;
							if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_SecToggle;
							else Ships[ WhoIAm ].Object.Flags &= ~SHIP_SecToggle;
							Ships[ WhoIAm ].SecID = Ships[ WhoIAm ].SecBullIdCount + 1;
							Ships[ WhoIAm ].SecWeapon = Ships[WhoIAm].Secondary;
							break;
					}

					switch( SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ] )
					{
/*===================================================================
				Fire bottom Left, then Right, then Left, then...
===================================================================*/
						case MUGMISSILE:
							if( Toggle == 0 )
							{
								Toggle = 1;
								DropVector.x = -0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
							else
							{
								Toggle = 0;
								DropVector.x = +0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							break;
	
/*===================================================================
				Fire bottom Left and Right together
===================================================================*/
						case SOLARISMISSILE:
							if( Toggle == 0 )
							{
								Toggle = 1;
								DropVector.x = -0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
							else
							{
								Toggle = 0;
								DropVector.x = +0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
	
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							break;
	
/*===================================================================
				Fire from bottom center at random trajectories
===================================================================*/
						case MULTIPLEMISSILE:
							DropVector.x = ( ( ( (float) Random_Range( 512 ) ) / 640.0F ) - 0.4F );
							DropVector.y = -( ( ( (float) Random_Range( 512 ) ) / 1024.0F ) + 1.0F );
							DropVector.z = ( ( (float) Random_Range( 512 ) ) / 512.0F );
							NormaliseVector( &DropVector );
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
						case THIEFMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
						case SCATTERMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
#if SCATTER_TEST
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos = Ships[ WhoIAm ].Object.Pos;

							for( Count = 0; Count < 8; Count++ )
							{
								Dir.x = (float) sin( D2R( ( 360.0F / 8.0F ) * Count ) );
								Dir.y = (float) cos( D2R( ( 360.0F / 8.0F ) * Count ) );
								Dir.z = 0.0F;
								UpVector.x = 0.0F;
								UpVector.y = 0.0F;
								UpVector.z = -1.0F;
								ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Dir, &Dir );
								ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &UpVector, &UpVector );
								InitOneSecBull( WhoIAm, Ships[WhoIAm].Object.Group, Pos, Dir, UpVector, DropVector, ++Ships[ WhoIAm ].SecBullIdCount, Ships[ WhoIAm ].Secondary, false );
							}
#else
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
#endif
							break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
						case GRAVGONMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
						case TITANSTARMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
			
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, false );
							recoil.x = 0.0F;
							recoil.y = 0.0F;
							recoil.z = -( 20.0F * GLOBAL_SCALE );
							ForceInternalOneOff( WhoIAm, &recoil );
							break;
	
/*===================================================================
				Drop Behind
===================================================================*/
						case PURGEMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, true );
							break;
	
/*===================================================================
				Drop Behind
===================================================================*/
						case PINEMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, true );
							break;
	
/*===================================================================
				Drop Behind
===================================================================*/
						case QUANTUMMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, true );
							break;
	
/*===================================================================
				Drop Behind
===================================================================*/
						case SPIDERMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, true );
							break;

/*===================================================================
				Spiral Missile
===================================================================*/
						case ENEMYSPIRALMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], false );
							break;
/*===================================================================
				Photon Missile
===================================================================*/
						case ENEMYHOMINGMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], false );
							break;
/*===================================================================
				Blue Photon Missile
===================================================================*/
						case ENEMYBLUEHOMINGMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], false );
							break;
/*===================================================================
				Blue Photon Missile
===================================================================*/
						case ENEMYFIREBALL:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], false );
							break;
					}
				}
			}
			else
			{
				if( SecondaryFireDelay < 0.0F ) SecondaryFireDelay = 0.0F;
			}
		}
   }
   else
   {
		SecondaryFireDelay -= framelag;
   }
}

/*===================================================================
	Procedure	:	Clean up and kill a Sec Bull
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void CleanUpSecBull( u_int16_t i )
{
	u_int16_t	light;
	u_int16_t	fmpoly;
	u_int16_t	nextfmpoly;
	u_int16_t	model;
	u_int16_t	poly;
	u_int16_t	nextpoly;
	int16_t	Count;

	light = SecBulls[i].light;
	fmpoly = SecBulls[i].fmpoly;
	poly = SecBulls[i].poly;
	model = SecBulls[i].ModelIndex;

	RemoveSecBullFromGroup( i, SecBulls[i].GroupImIn );

	if ( light != (u_int16_t) -1 )
	{
		SetLightDie(SecBulls[i].light);					// Kill Light
		SecBulls[i].light = (u_int16_t) -1;
	}

	for( Count = 0; Count < SecBulls[i].numfmpolys; Count++ )
	{
		nextfmpoly = FmPolys[ fmpoly ].Prev;
		if( fmpoly != (u_int16_t) -1 ) KillUsedFmPoly( fmpoly );
		fmpoly = nextfmpoly;
	}
	SecBulls[i].numfmpolys = 0;
	SecBulls[i].fmpoly = (u_int16_t) -1;

	if ( model != (u_int16_t) -1 )
	{
		KillUsedModel( model );							// Kill Model
		SecBulls[i].ModelIndex = (u_int16_t) -1;
	}

	for( Count = 0; Count < SecBulls[i].numpolys; Count++ )
	{
		nextpoly = Polys[ poly ].Prev;
		if( poly != (u_int16_t) -1 ) KillUsedPoly( poly );
		poly = nextpoly;
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (u_int16_t) -1;

	SecBulls[i].LifeCount = 0.0F;
	if( CameraMissile == i ) MissileCameraActive = 0;

	KillUsedSecBull( i );								// Kill Bullet
}

/*===================================================================
	Procedure	:	Init 1 Secondary Bullet..Mainly for networking...
	Input		:	u_int16_t		OwnerType
				:	u_int16_t		Owner
				:	u_int16_t		Bullet ID
				:	u_int16_t		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	Offset
				:	VECTOR	*	Dir
				:	VECTOR	*	DropDir
				:	int8_t		Weapon
				:	bool		NetFlag ( Send to everyone? )
	Output		:	u_int16_t		SecBull Index ( -1 nothing )
===================================================================*/
u_int16_t	InitOneSecBull( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon, bool NetFlag )
{
	u_int16_t	i;
	u_int16_t	light;
	u_int16_t	fmpoly;
	u_int16_t	model;
	int16_t	Count;
	int Player; // used for watch or demo mode

	// normal missile camera or watch mode camera
	if(Ships[ WhoIAm ].Object.Mode != DEMO_MODE)
		Player = WatchPlayerSelect.value;
	// demo missile camera
	else 
		Player = DemoEyesSelect.value;
	
	
	if( !VectorLength( Dir ) )
	{
		return( (u_int16_t) -1 );
	}

/*
	switch( Weapon )
	{
		case MUGMISSILE:
		case ENEMYSPIRALMISSILE:
		case ENEMYHOMINGMISSILE:
		case ENEMYBLUEHOMINGMISSILE:
		case ENEMYFIREBALL:
		case ENEMYDEPTHCHARGE:
		case TITANSTARSHRAPNEL:
		case THIEFMISSILE:
		case MULTIPLEMISSILE:
		case ENEMYTENTACLE:
			if( MyGameStatus != STATUS_SinglePlayer )
			{
				if( !IsGroupVisible[ Group ] && !VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Group, NULL ) && !outside_map )
					return( (u_int16_t) -1 );
			}
			break;

		case SOLARISMISSILE:
		case SCATTERMISSILE:
		case TITANSTARMISSILE:
		case GRAVGONMISSILE:
		case PURGEMINE:
		case PINEMINE:
		case QUANTUMMINE:
		case SPIDERMINE:
		case PINEMISSILE:
			break;

		default:
			Weapon = -1;
			break;
	}
*/

	i = FindFreeSecBull();

	if( i != (u_int16_t) -1 )
	{
		if( ( OwnerType == OWNER_SHIP ) && ( Owner == Player/*WhoIAm*/ ) )
		{
			CameraMissile = i;
			MissileCameraActive = 1;
		}

		SecBulls[i].DropVector = *DropDir;
		SecBulls[i].DirVector = *Dir;
		SecBulls[i].UpVector = *UpVector;

		QuatFromDirAndUp( &SecBulls[ i ].DirVector, &SecBulls[ i ].UpVector, &SecBulls[ i ].DirQuat );
		QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );							// Create Dir Matrix

		SecBulls[i].Type = Weapon;
		SecBulls[i].Flags = SecondaryWeaponAttribs[ Weapon ].Flags;
		SecBulls[i].SecType = SecondaryWeaponAttribs[ Weapon ].SecType;
		SecBulls[i].MoveType = SecondaryWeaponAttribs[ Weapon ].MoveType;
		SecBulls[i].ColRadius = SecondaryWeaponAttribs[ Weapon ].ColRadius;
		SecBulls[i].DropCount = 2.0F;
		SecBulls[i].TargetType = (u_int16_t) -1;
		SecBulls[i].Target = (u_int16_t) -1;
		SecBulls[i].Pos.x = ( Pos->x + Offset->x );
		SecBulls[i].Pos.y = ( Pos->y + Offset->y );
		SecBulls[i].Pos.z = ( Pos->z + Offset->z );
		SecBulls[i].Offset = *Offset;
		SecBulls[i].StartPos = SecBulls[i].Pos;
		SecBulls[i].StartDir = SecBulls[i].DirVector;
		SecBulls[i].StartMat = SecBulls[i].Mat;
		SecBulls[i].OwnerType = OwnerType;
		SecBulls[i].Owner = Owner;
		SecBulls[i].Weapon = Weapon;
		SecBulls[i].ID = BulletID;
		SecBulls[i].State = SecondaryWeaponAttribs[ Weapon ].State;
		SecBulls[i].SpeedInc = SecondaryWeaponAttribs[ Weapon ].SpeedInc;
		SecBulls[i].SpeedWanted = SecondaryWeaponAttribs[ Weapon ].Speed;
		SecBulls[i].Speed = SecondaryWeaponAttribs[ Weapon ].StartSpeed;
		SecBulls[i].TurnSpeed = SecondaryWeaponAttribs[ Weapon ].TurnSpeed;
		SecBulls[i].ViewCone = (float) COSD( SecondaryWeaponAttribs[ Weapon ].ViewCone / 2 );
		SecBulls[i].LifeCount = 0.0F;
		SecBulls[i].LifeSpan = 0.0F;
		SecBulls[i].ModelNum = SecondaryWeaponAttribs[ Weapon ].ModelNum;
		SecBulls[i].xsize = 32.0F;
		SecBulls[i].ysize = 32.0F;
		SecBulls[i].ColFlag = 0;
		SecBulls[i].ColPoint.x = 0.0F;
		SecBulls[i].ColPoint.y = 0.0F;
		SecBulls[i].ColPoint.z = 0.0F;
		SecBulls[i].GroupImIn = Group;
		SecBulls[i].Shield = SecondaryWeaponAttribs[ Weapon ].Shield;
		SecBulls[i].Damage = SecondaryWeaponAttribs[ Weapon ].Damage;
		SecBulls[i].Ammo = 0;
		SecBulls[i].NumOldPos = 0;
		SecBulls[i].fmpoly = (u_int16_t) -1;
		SecBulls[i].numfmpolys = 0;
		SecBulls[i].numpolys = 0;
		SecBulls[i].Time = 0.0F;
		SecBulls[i].Interval = (float) 1.0F;
		SecBulls[i].Lensflare = SecondaryWeaponAttribs[ Weapon ].Lensflare;
		SecBulls[i].NumBounces = 0;
		SecBulls[i].RetractPos = 0.0F;
		SecBulls[i].FramelagAddition = 0.0F;

		if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) )
		{
			if( SecBulls[i].SecType != SEC_XMISSILE )
			{
				if( !GodMode )
				{
					int8_t TempWeapon = ConvSecToNormWeapon( Weapon );
					if( TempWeapon < MAXSECONDARYWEAPONS )
					{
						if( SecBulls[i].SecType != SEC_MINE ) SecAmmoUsed[ TempWeapon ]++;
						if( SecondaryAmmo[ TempWeapon ] ) SecondaryAmmo[ TempWeapon ]--;
						if( !SecondaryAmmo[ TempWeapon ] ) SecondaryWeaponsGot[ TempWeapon ] = 0;
					}
				}
			}
		}

		SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, Pos, Group, Offset );

		AddSecBullToGroup( i, SecBulls[i].GroupImIn );

		switch( Weapon )
		{
			case TITANSTARSHRAPNEL:
				SecBulls[i].LifeSpan = (float) 600.0F;
				break;

			case THIEFMISSILE:
				SecBulls[i].LifeSpan = 600.0F;
				break;

			case ENEMYTENTACLE:
				SecBulls[i].LifeSpan = 200.0F;
				break;

			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case SCATTERMISSILE:
			case GRAVGONMISSILE:
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case TITANSTARMISSILE:
				PlayPannedSfx( SFX_TitanLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 1.0F );
				break;

			case MULTIPLEMISSILE:
				//PlayPannedSfx( SFX_MFRLLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				PlayPannedSfxWithVolModify(SFX_MFRLLaunch, SecBulls[i].GroupImIn, &SecBulls[i].Pos, 0.0F, 0.85F );
				break;

			case PINEMISSILE:
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case PURGEMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case PINEMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				SecBulls[i].Ammo = 10;
				break;

			case QUANTUMMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case SPIDERMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case ENEMYFIREBALL:
				SecBulls[i].NumBounces = 2;
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case ENEMYDEPTHCHARGE:
				QuatFromDirAndUp( &Forward, &SlideUp, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );			// Create Dir Matrix

				SecBulls[i].LifeSpan = ( ( ( (float) Random_Range( 10000 ) ) / 5000.0F ) + 2.0F ) * 60.0F;
				SecBulls[i].DirVector.x *= SecBulls[i].Speed;
				SecBulls[i].DirVector.y *= SecBulls[i].Speed;
				SecBulls[i].DirVector.z *= SecBulls[i].Speed;
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;
		}

		light = (u_int16_t) -1;
		fmpoly = (u_int16_t) -1;
		model = (u_int16_t) -1;

		if( SecondaryWeaponAttribs[ Weapon ].lightsize != 0.0F )
		{
			light = FindFreeXLight();
			if( light != (u_int16_t ) -1 )
			{
				switch( Weapon )
				{
					case QUANTUMMINE:
						ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &XLights[ light ].Dir ); /* Calc Direction Vector */
						NormaliseVector( &XLights[ light ].Dir );
						XLights[ light ].Type = SPOT_LIGHT;
						XLights[ light ].Pos = SecBulls[i].Pos;
						XLights[ light ].CosArc = (float)cos(D2R(30));
						XLights[ light ].Size = ( 4096.0F * GLOBAL_SCALE ); //PickupAttribs[ Type ].Lightsize;
						XLights[ light ].SizeCount = 0.0F;
						XLights[ light ].r = SecondaryWeaponAttribs[ Weapon ].r;
						XLights[ light ].g = SecondaryWeaponAttribs[ Weapon ].g;
						XLights[ light ].b = SecondaryWeaponAttribs[ Weapon ].b;
						XLights[ light ].Group = SecBulls[i].GroupImIn;
						break;

					default:
						XLights[light].Pos = SecBulls[i].Pos;
						XLights[light].Size = SecondaryWeaponAttribs[ Weapon ].lightsize;
						XLights[light].SizeCount = 0.0F;
						XLights[light].r = SecondaryWeaponAttribs[ Weapon ].r;
						XLights[light].g = SecondaryWeaponAttribs[ Weapon ].g;
						XLights[light].b = SecondaryWeaponAttribs[ Weapon ].b;
						XLights[light].Group = SecBulls[i].GroupImIn;
						break;
				}
			}
		}
		SecBulls[i].light = light;

		if( SecBulls[i].ModelNum != (u_int16_t) -1 )
		{
			model =	FindFreeModel();

			if( model != (u_int16_t ) -1 )
			{
				Models[model].Owner = SecBulls[i].Owner;
				Models[model].OwnerType = SecBulls[i].OwnerType;
				Models[model].ModelNum = SecBulls[ i ].ModelNum;
				Models[model].Type = MODTYPE_Missile;
				Models[model].Flags = MODFLAG_Clip;
				Models[model].Visible = true;
				Models[model].Pos = SecBulls[ i ].Pos;
				Models[model].Mat = SecBulls[ i ].Mat;
				MatrixTranspose( &SecBulls[ i ].Mat, &Models[model].InvMat );
				Models[model].Func = MODFUNC_Nothing;
			}
		}
		SecBulls[i].ModelIndex = model;

		switch( Weapon )
		{
			case ENEMYTENTACLE:
				SecBulls[i].Interval = 8.0F;
				break;

			case SCATTERMISSILE:
				SecBulls[i].Interval = 8.0F;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (u_int16_t ) -1 )
   				{
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
   					FmPolys[ fmpoly ].Frame = 0.0F;
#if ACTUAL_TRANS
					FmPolys[ fmpoly ].Trans = 128;
#else
					FmPolys[ fmpoly ].Trans = 255;
#endif
					FmPolys[ fmpoly ].R = 64;
					FmPolys[ fmpoly ].G = 64;
					FmPolys[ fmpoly ].B = 255;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = ( 10.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].ysize = ( 10.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

			case ENEMYHOMINGMISSILE:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (u_int16_t ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 2.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 255;
					FmPolys[ fmpoly ].G = 128;
					FmPolys[ fmpoly ].B = 64;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &LensFlare_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 12.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				for( Count = 0; Count < 10; Count++ )
				{
					fmpoly = FindFreeFmPoly();					// Faceme polygon attached

					if( fmpoly != (u_int16_t ) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 10000.0F;
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_PHOTON;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].AnimSpeed = 1.0F;
						FmPolys[ fmpoly ].R = 64;
						FmPolys[ fmpoly ].G = 64;
						FmPolys[ fmpoly ].B = 64;
   						FmPolys[ fmpoly ].Trans = 255;
						FmPolys[ fmpoly ].Frm_Info = &BigFlash_Header;
						FmPolys[ fmpoly ].DirVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						NormaliseVector( &FmPolys[ fmpoly ].DirVector );
						NormaliseVector( &FmPolys[ fmpoly ].UpVector );
   						FmPolys[ fmpoly ].Flags = FM_FLAG_RELATIVE;
						FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
						FmPolys[ fmpoly ].RotSpeed = ( ( (float) Random_Range( 10000 ) ) / 2500.0F ) + 2.0F;
						FmPolys[ fmpoly ].xsize = ( 2.0F * GLOBAL_SCALE );
						FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   						FmPolys[ fmpoly ].Dir.x = 0.0F;
   						FmPolys[ fmpoly ].Dir.y = 0.0F;
   						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].UpSpeed = 0.0F;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						SecBulls[i].numfmpolys++;
						SecBulls[i].fmpoly = fmpoly;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
				}
				break;

			case ENEMYBLUEHOMINGMISSILE:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (u_int16_t ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 2.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 96;
					FmPolys[ fmpoly ].G = 96;
					FmPolys[ fmpoly ].B = 255;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &LensFlare_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 12.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				for( Count = 0; Count < 10; Count++ )
				{
					fmpoly = FindFreeFmPoly();					// Faceme polygon attached

					if( fmpoly != (u_int16_t ) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 10000.0F;
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_BLUEPHOTON;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].AnimSpeed = 1.0F;
						FmPolys[ fmpoly ].R = 128;
						FmPolys[ fmpoly ].G = 128;
						FmPolys[ fmpoly ].B = 128;
   						FmPolys[ fmpoly ].Trans = 255;
						FmPolys[ fmpoly ].Frm_Info = &BlueSpark_Header;
						FmPolys[ fmpoly ].DirVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						NormaliseVector( &FmPolys[ fmpoly ].DirVector );
						NormaliseVector( &FmPolys[ fmpoly ].UpVector );
   						FmPolys[ fmpoly ].Flags = FM_FLAG_RELATIVE;
						FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
						FmPolys[ fmpoly ].RotSpeed = ( ( (float) Random_Range( 10000 ) ) / 2500.0F ) + 2.0F;
						FmPolys[ fmpoly ].xsize = ( 4.0F * GLOBAL_SCALE );
						FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   						FmPolys[ fmpoly ].Dir.x = 0.0F;
   						FmPolys[ fmpoly ].Dir.y = 0.0F;
   						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].UpSpeed = 0.0F;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						SecBulls[i].numfmpolys++;
						SecBulls[i].fmpoly = fmpoly;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
				}
				break;

			case ENEMYFIREBALL:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (u_int16_t ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 255;
					FmPolys[ fmpoly ].G = 128;
					FmPolys[ fmpoly ].B = 128;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &Circle_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 8.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 8.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

			case TITANSTARSHRAPNEL:
			case THIEFMISSILE:
			case ENEMYSPIRALMISSILE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case MULTIPLEMISSILE:
			case GRAVGONMISSILE:
			case TITANSTARMISSILE:
			case PINEMISSILE:
			case PURGEMINE:
			case PINEMINE:
			case QUANTUMMINE:
			case SPIDERMINE:
			case ENEMYDEPTHCHARGE:
			default:
				break;
		}

		if( NetFlag )
		{
			SecBullPosDirSend( OwnerType, Owner, BulletID, Group, Pos, Offset, Dir, UpVector,
						DropDir, Weapon );
		}
	}
	return i;
}

/*===================================================================
	Procedure	:	Check Point to Sphere collision
	Input		:	VERT	*	Target Position
				:	float		Target Radius
				:	VERT	*	Point
	Output		:	bool		true/false
===================================================================*/
bool PointToSphere(	VERT * TargPos, float TargRadius, VERT * BulPos )
{
	float	d;
	float	xd, yd, zd;

	xd = ( TargPos->x - BulPos->x );
	yd = ( TargPos->y - BulPos->y );
	zd = ( TargPos->z - BulPos->z );
	d = ( xd * xd ) + ( yd * yd ) + ( zd * zd );
	if( d < 0.0F ) d = -d;
	if( d <= ( TargRadius * TargRadius ) ) return true;

	return false;
}

/*===================================================================
	Procedure	:	Process Secondary Bullets
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessSecondaryBullets( void )
{
	u_int16_t		i;
	u_int16_t		NextMissile;
	MATRIX		TempMat;
	u_int16_t		fmpoly;
	u_int16_t		nextfmpoly;
	int16_t		Count;
	float		NewFramelag;

	i = FirstSecBullUsed;

	if( ImTargeted )
	{
		TargetedDelay -= framelag;
		if( TargetedDelay <= 0.0F )
		{
			if( !( SecBulls[ MissileNum ].Flags & SECFLAGS_NoIncoming ) )
			{
				PlayPannedSfx( SFX_Lockon, SecBulls[ MissileNum ].GroupImIn , &SecBulls[ MissileNum ].Pos, 0.0F );
			}
			TargetedDelay += ( ( MissileDistance / ( 1024.0F * GLOBAL_SCALE ) ) * 2.0F );
		}
	}
	else
	{
		TargetedDelay = 0.0F;
	}

	ImTargeted = false;
	MissileDistance = 32767.0F;

	while( i != (u_int16_t) -1 )
	{
		NextMissile = SecBulls[ i ].Prev;											/* Next Missile */

		NewFramelag = ( framelag + SecBulls[i].FramelagAddition );

		SecBulls[ i ].LifeCount += NewFramelag; //framelag;

		switch( SecBulls[ i ].SecType )
		{
			case SEC_MISSILE:
			case SEC_XMISSILE:
				if( !ProcessMissiles( i, &NextMissile ) )
				{
					i = NextMissile;
					continue;
				}
				break;

			case SEC_MINE:
				if( !ProcessMines( i ) )
				{
					i = NextMissile;
					continue;
				}
				break;

			default:
				i = NextMissile;
				continue;
				break;
		}

/*===================================================================
			Update Attached Objects
===================================================================*/
		if( i != (u_int16_t) -1 )
		{
			if( SecBulls[i].light != (u_int16_t) -1 )									/* Light Attached? */
			{																		
				XLights[SecBulls[i].light].Pos = SecBulls[i].Pos;					/* Update Light Position */
				XLights[SecBulls[i].light].Group = SecBulls[i].GroupImIn;			/* Update Light Group */
				if( XLights[ SecBulls[i].light ].Type == SPOT_LIGHT )
				{
					MatrixTranspose( &SecBulls[i].Mat, &TempMat );
					ApplyMatrix( &TempMat, &Forward, &XLights[ SecBulls[i].light ].Dir );
					NormaliseVector( &XLights[ SecBulls[i].light ].Dir );
				}
			}																		

			if( SecBulls[i].fmpoly != (u_int16_t) -1 )
			{
				fmpoly = SecBulls[i].fmpoly;

				for( Count = 0; Count < SecBulls[i].numfmpolys; Count++ )
				{
					nextfmpoly = FmPolys[ fmpoly ].Prev;
			
					if ( fmpoly != (u_int16_t) -1 )
					{
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						if( ( SecBulls[i].Weapon == SCATTERMISSILE ) ||
							( SecBulls[i].Weapon == ENEMYTENTACLE ) )
						{
							FmPolys[ fmpoly ].Pos.x += ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
							FmPolys[ fmpoly ].Pos.y += ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
							FmPolys[ fmpoly ].Pos.z += ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
						}
					}
					fmpoly = nextfmpoly;
				}
			}

			if ( SecBulls[i].ModelIndex != (u_int16_t) -1 )							/* 3D Model Attached? */
			{																		
				Models[SecBulls[i].ModelIndex].Mat	  = SecBulls[i].Mat;			/* Update Model Matrix */
				MatrixTranspose( &SecBulls[i].Mat, &Models[SecBulls[i].ModelIndex].InvMat );
				Models[SecBulls[i].ModelIndex].Pos	  = SecBulls[i].Pos;			
				Models[SecBulls[i].ModelIndex].Group  = SecBulls[i].GroupImIn;
			}																		
		}

		i = NextMissile;														/* Next Missile */
	}																				
}

/*===================================================================
	Procedure	:	Process All Mines
	Input		:	u_int16_t		Secondary Weapon Bullet Index
	Output		:	bool		True/False
===================================================================*/
bool ProcessMines( u_int16_t i )
{
	float		Cos;
	float		Angle;
	VECTOR		DirVector;
	QUATLERP	qlerp;
	VECTOR		DropVector;
	VECTOR		DropPos;
	VECTOR		FrameLag_Rot;
	QUAT		TempQuat;
	int16_t		Val;
	float		Height;
	float		ClosestCos = -1.0F;
	u_int16_t		Ship, Enemy, Secondary;
	u_int16_t		Owner;
	float		Radius;
	u_int16_t		BulletID;
	float		NewFramelag;

	NewFramelag = ( framelag + SecBulls[i].FramelagAddition );
	if( SecBulls[i].FramelagAddition != 0.0F )
	{
		SecBulls[i].FramelagAddition = 0.0F;
	}

	if( SecBulls[i].LifeSpan )
	{
		if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
		{
			DestroySecondary( i, &SecBulls[i].Pos );
			return( false );
		}
	}

	switch( SecBulls[ i ].Type )
	{
/*===================================================================
		PURGE MINE, proximity detonation
===================================================================*/
		case PURGEMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			Val = (int16_t) ( SecBulls[i].LifeCount * 3.0F );
			Val %= 360;
			Height = (float) ( COSD( Val ) * ( 200.0F * GLOBAL_SCALE ) );
			SecBulls[i].Pos.x = SecBulls[i].StartPos.x + ( SecBulls[i].UpVector.x * Height );
			SecBulls[i].Pos.y = SecBulls[i].StartPos.y + ( SecBulls[i].UpVector.y * Height );
			SecBulls[i].Pos.z = SecBulls[i].StartPos.z + ( SecBulls[i].UpVector.z * Height );

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (u_int16_t) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (u_int16_t) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;

/*===================================================================
		PINE MINE, Aim at nearest target and fire 1 missile per sec
===================================================================*/
		case PINEMINE:
			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (u_int16_t) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (u_int16_t) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}

			Ship = (u_int16_t) -1;
			Enemy = (u_int16_t) -1;
			Secondary = (u_int16_t) -1;

#ifdef SINT_PEACEFROG_CHEAT
			TargetingWeaponCheat = SecBulls[i].Weapon;
#endif
			switch( SecBulls[i].OwnerType )
			{
				case OWNER_SHIP:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Enemy = TargetClosestEnemy( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											    SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												&ClosestCos );
					if( Enemy != (u_int16_t) -1 ) Ship = (u_int16_t) -1;
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (u_int16_t) -1 )
					{
						Ship = (u_int16_t) -1;
						Enemy = (u_int16_t) -1;
					}
					break;

				case OWNER_ENEMY:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (u_int16_t) -1 )
					{
						Ship = (u_int16_t) -1;
					}
					break;

				case OWNER_NOBODY:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (u_int16_t) -1 )
					{
						Ship = (u_int16_t) -1;
					}
					break;

				default:
					break;
			}

			if( Ship != (u_int16_t) -1 )
			{
				DirVector.x = ( Ships[ Ship ].Object.Pos.x - SecBulls[ i ].Pos.x );
				DirVector.y = ( Ships[ Ship ].Object.Pos.y - SecBulls[ i ].Pos.y );
				DirVector.z = ( Ships[ Ship ].Object.Pos.z - SecBulls[ i ].Pos.z );
			}
			else
			{
				if( Enemy != (u_int16_t) -1 )
				{
					DirVector.x = ( Enemies[ Enemy ].Object.Pos.x - SecBulls[ i ].Pos.x );
					DirVector.y = ( Enemies[ Enemy ].Object.Pos.y - SecBulls[ i ].Pos.y );
					DirVector.z = ( Enemies[ Enemy ].Object.Pos.z - SecBulls[ i ].Pos.z );
				}
				else
				{
					if( Secondary != (u_int16_t) -1 )
					{
						DirVector.x = ( SecBulls[ Secondary ].Pos.x - SecBulls[ i ].Pos.x );
						DirVector.y = ( SecBulls[ Secondary ].Pos.y - SecBulls[ i ].Pos.y );
						DirVector.z = ( SecBulls[ Secondary ].Pos.z - SecBulls[ i ].Pos.z );
					}
				}
			}

			if( ( Ship != (u_int16_t) -1 ) || ( Enemy != (u_int16_t) -1 ) || ( Secondary != (u_int16_t) -1 ) )
			{
				NormaliseVector( &DirVector );

				Cos = DotProduct( &DirVector, &SecBulls[ i ].DirVector );

				QuatFrom2Vectors( &qlerp.end, &Forward, &DirVector );
   				
				qlerp.start	= SecBulls[ i ].DirQuat;				/* Current Quaternion */
   				qlerp.crnt	= &SecBulls[ i ].DirQuat;				/* Address to store new Quaternion */
				qlerp.dir	= QuatDotProduct( &qlerp.start, &qlerp.end );
   	
				if( Cos < -1.0F ) Cos = -1.0F;
				else if ( Cos > 1.0F ) Cos = 1.0F;
				Angle = (float) acos( Cos );
				if( Angle ) qlerp.time = ( ( D2R( SecBulls[ i ].TurnSpeed ) * framelag ) / Angle );
				else qlerp.time = 1.0F;
				if( qlerp.time > 1.0F ) qlerp.time = 1.0F;
   				
				QuatInterpolate( &qlerp );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
				ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
				ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );

				if( R2D( Angle ) < 1.0F )
				{
					if( ( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == WhoIAm ) ) ) ||
						( ( ( SecBulls[i].OwnerType != OWNER_SHIP ) && IsHost ) ) )
					{
						BulletID = GetSecondaryBulletID( SecBulls[i].OwnerType, SecBulls[i].Owner );

						if( SecBulls[i].LifeCount > 60.0F )
						{
							DropPos.x = ( SecBulls[i].UpVector.x * ( -128.0F * GLOBAL_SCALE ) );
							DropPos.y = ( SecBulls[i].UpVector.y * ( -128.0F * GLOBAL_SCALE ) );
							DropPos.z = ( SecBulls[i].UpVector.z * ( -128.0F * GLOBAL_SCALE ) );
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = 0.0F;
							InitOneSecBull( SecBulls[i].OwnerType, SecBulls[i].Owner, BulletID,
											SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DropPos,
											&SecBulls[i].DirVector, &SecBulls[i].UpVector, &DropVector, PINEMISSILE, true );
							SecBulls[i].LifeCount = 0.0F;
						}
					}
				}
			}
			break;

/*===================================================================
		QUANTUM MINE, proximity detonation
===================================================================*/
		case QUANTUMMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			Val = (int16_t) ( SecBulls[i].LifeCount * 2.5F );
			Val %= 360;
			Height = (float) ( COSD( Val ) * ( 75.0F * GLOBAL_SCALE ) );
			SecBulls[i].Pos.x = SecBulls[i].StartPos.x + ( SecBulls[i].UpVector.x * Height );
			SecBulls[i].Pos.y = SecBulls[i].StartPos.y + ( SecBulls[i].UpVector.y * Height );
			SecBulls[i].Pos.z = SecBulls[i].StartPos.z + ( SecBulls[i].UpVector.z * Height );

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (u_int16_t) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (u_int16_t) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;

/*===================================================================
		SPIDER MINE, proximity detonation, Moving
===================================================================*/
		case SPIDERMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (u_int16_t) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (u_int16_t) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;
	}
	return( true );
}

/*===================================================================
	Procedure	:	Process All Missiles
	Input		:	u_int16_t		Secondary Weapon Bullet Index
	Output		:	bool		NotDestroyed/Destroyed?  ( true/false )
===================================================================*/
extern  BYTE          MyGameStatus;
bool ProcessMissiles( u_int16_t i, u_int16_t * NextMissile )
{
	QUATLERP		qlerp;
	VECTOR			NewPos;
	VECTOR			DirVector;
	VECTOR			TempVector;
	VECTOR			NormVector;
	VECTOR			temp;
	float			Length;
	float			Speed;
	float			Cos;
	float			Angle;
	float			DistToInt;
	float			DistToTarget;
	float			DistFromStart;
	u_int16_t			Light;
	u_int16_t			HitTarget;
	u_int16_t			HitWall;
	u_int16_t			HitSecondary;
	VECTOR			Int_Point;
	VECTOR			Int_Point2;
	VECTOR			MInt_Point;
	VECTOR			MInt_Point2;
	VECTOR			Recoil;
	BGOBJECT	*	BGObject;
	ENEMY		*	HitEnemy;
	VECTOR			EInt_Point;
	VECTOR			EInt_Point2;
	NORMAL			ENormal;
	MATRIX			UpMatrix;
	MATRIX			InvUpMatrix;
	QUAT			TempQuat;
	VECTOR			FrameLag_Rot;
	VECTOR			NewDir;
	float			Damage;
	float			ClosestCos = -1.0F;
	u_int16_t			Ship;
	u_int16_t			Enemy;
	u_int16_t			Secondary;
	u_int16_t			OldGroup;
	float			LifeCount;
	VECTOR			Rotation;
	VECTOR			UpVector;
	VECTOR			TempPoint1;
	VECTOR			TempPoint2;
	u_int16_t			TempGroup;
	u_int16_t			TempEnemyIndex;
	float			NewFramelag;

	NewFramelag = ( framelag + SecBulls[i].FramelagAddition );
	if( SecBulls[i].FramelagAddition != 0.0F )
	{
		SecBulls[i].FramelagAddition = 0.0F;
	}

	*NextMissile = SecBulls[ i ].Prev;

	OldGroup = SecBulls[ i ].GroupImIn;

	switch( SecBulls[ i ].Weapon )
	{
		case ENEMYTENTACLE:
			if( ( SecBulls[i].NumOldPos == MAXTENTOLDPOS ) || ( SecBulls[i].State == MIS_RETRACTTENTACLE ) )
				break;

			SecBulls[i].Time += NewFramelag; //framelag;
			if( SecBulls[i].Time >= SecBulls[i].Interval )
			{
				SecBulls[i].Time -= ( SecBulls[i].Interval * (float) ( (int32_t) ( SecBulls[i].Time / SecBulls[i].Interval ) ) );
				memmove( &SecBulls[ i ].OldPos[ 1 ], &SecBulls[ i ].OldPos[ 0 ], ( ( MAXTENTOLDPOS - 1 ) * sizeof( OLDPOS ) ) );
				SecBulls[ i ].OldPos[ 0 ].Pos = SecBulls[ i ].Pos;
				SecBulls[ i ].OldPos[ 0 ].Dir = SecBulls[ i ].DirVector;
				SecBulls[ i ].OldPos[ 0 ].R = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].G = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].B = 128;
				if( SecBulls[ i ].NumOldPos != MAXTENTOLDPOS )
				{
					SecBulls[ i ].NumOldPos++;
					if( SecBulls[ i ].NumOldPos == MAXTENTOLDPOS )
					{
						SecBulls[ i ].State = MIS_RETRACTTENTACLE;
						SecBulls[ i ].RetractPos = 0.0F;

						CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, TENTACLE_SHOCKWAVE, SecBulls[i].Weapon );
					}
				}
			}
			break;

		case SCATTERMISSILE:
			SecBulls[i].Time += NewFramelag; //framelag;
			if( SecBulls[i].Time >= SecBulls[i].Interval )
			{
				SecBulls[i].Time -= ( SecBulls[i].Interval * (float) ( (int32_t) ( SecBulls[i].Time / SecBulls[i].Interval ) ) );
				memmove( &SecBulls[ i ].OldPos[ 1 ], &SecBulls[ i ].OldPos[ 0 ], ( ( MAXOLDPOS - 1 ) * sizeof( OLDPOS ) ) );
				SecBulls[ i ].OldPos[ 0 ].Pos = SecBulls[ i ].Pos;
				SecBulls[ i ].OldPos[ 0 ].Dir = SecBulls[ i ].DirVector;
				SecBulls[ i ].OldPos[ 0 ].R = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].G = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].B = 128;
				if( SecBulls[ i ].NumOldPos != MAXOLDPOS ) SecBulls[ i ].NumOldPos++;
			}
			break;

		case ENEMYSPIRALMISSILE:
		case ENEMYHOMINGMISSILE:
		case ENEMYBLUEHOMINGMISSILE:
		case ENEMYFIREBALL:
		case MUGMISSILE:
		case SOLARISMISSILE:
		case GRAVGONMISSILE:
		case MULTIPLEMISSILE:
		case TITANSTARMISSILE:
		case PINEMISSILE:
			break;

		case THIEFMISSILE:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
	 			CreateExplosion( &SecBulls[i].Pos, SecBulls[i].GroupImIn );
	 			CleanUpSecBull( i );
				return( false );
			}
			break;

		case TITANSTARSHRAPNEL:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				NormVector = SecBulls[i].DirVector;
				NormaliseVector( &NormVector );
				QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &NormVector );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
			}
			break;

		case ENEMYDEPTHCHARGE:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
			else
			{
				FrameLag_Rot.x = 0.0F;
				FrameLag_Rot.y = ( 5.0F * NewFramelag ); //framelag );
				FrameLag_Rot.z = 0.0F;
				MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
				QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix
			}
			break;

	}

   	switch( SecBulls[ i ].State )
   	{
/*===================================================================
		Missile Dropping ( MFRL )
===================================================================*/
   		case MIS_DROP:														/* Dropping */
   			if( SecBulls[ i ].DropCount == 0.0F )
   			{
				SecBulls[ i ].State = MIS_ACCELERATE;						/* Start to accelerate */
				SecBulls[ i ].ColFlag = 0;

   				Light = FindFreeXLight();									/* Generate Light */
   				if( Light != (u_int16_t ) -1 )
   				{
   					XLights[ Light ].Pos = SecBulls[i].Pos;
   					XLights[ Light ].Size = SecondaryWeaponAttribs[ SecBulls[i].Type ].lightsize;
   					XLights[ Light ].SizeCount = 0.0F;
   					XLights[ Light ].r = SecondaryWeaponAttribs[ SecBulls[i].Type ].r;
   					XLights[ Light ].g = SecondaryWeaponAttribs[ SecBulls[i].Type ].g;
   					XLights[ Light ].b = SecondaryWeaponAttribs[ SecBulls[i].Type ].b;
 					XLights[ Light ].Group = SecBulls[i].GroupImIn;
				}
				SecBulls[i].light = Light;
   			}
   			else
   			{
   				SecBulls[ i ].DropCount -= NewFramelag; //framelag;
   				if( SecBulls[ i ].DropCount < 0.0F )
				{
					SecBulls[ i ].FramelagAddition = (float) fabs( SecBulls[i].DropCount );
					SecBulls[ i ].DropCount = 0.0F;
				}
   			}
   			break;

/*===================================================================
		Missile Accelerating ( MFRL )
===================================================================*/
   		case MIS_ACCELERATE:												/* Accelerating */
   			if( SecBulls[ i ].Speed < SecBulls[ i ].SpeedWanted )
   			{
   				SecBulls[ i ].Speed += SecBulls[ i ].SpeedInc;
   				if( SecBulls[ i ].Speed > SecBulls[ i ].SpeedWanted )
   				{
   					SecBulls[ i ].Speed = SecBulls[ i ].SpeedWanted;
					SecBulls[ i ].State = MIS_STRAIGHT;
   				}
   			}
			break;

/*===================================================================
		Missile Flying Staight ( All )
===================================================================*/
   		case MIS_STRAIGHT:													/* Straight ( possibly homing missile looking for target ) */
   			if( SecBulls[ i ].MoveType == MISMOVE_HOMING )
   			{
				ClosestCos = -1.0F;
				
#ifdef SINT_PEACEFROG_CHEAT
				TargetingWeaponCheat = SecBulls[i].Weapon;
#endif
				switch( SecBulls[i].OwnerType )
				{
					case OWNER_SHIP:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						Enemy = TargetClosestEnemy( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												    SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
													&ClosestCos );
						if( Enemy != (u_int16_t) -1 ) Ship = (u_int16_t) -1;
#if 1
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (u_int16_t) -1 )
						{
							Ship = (u_int16_t) -1;
							Enemy = (u_int16_t) -1;
						}
#else
						Secondary = (u_int16_t) -1;
#endif
						if( Ship != (u_int16_t) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
								if( Ship == WhoIAm )
									if( bSoundEnabled && BikeCompSpeechSlider.value )
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Enemy != (u_int16_t) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_ENEMY;
								SecBulls[ i ].Target = Enemy;
								Enemies[ Enemy ].AIFlags |= AI_TARGETED;
								Enemies[ Enemy ].TargetingPos = SecBulls[i].Pos;
								Enemies[ Enemy ].TargetingDir = SecBulls[i].DirVector;
							}
							else
							{
								if( Secondary != (u_int16_t) -1 )
								{
									SecBulls[ i ].State = MIS_HOMING;
									SecBulls[ i ].TargetType = OWNER_MINE;
									SecBulls[ i ].Target = Secondary;
								}
							}
						}
						break;

					case OWNER_ENEMY:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
#if 1
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (u_int16_t) -1 )
						{
							Ship = (u_int16_t) -1;
						}
#else
						Secondary = (u_int16_t) -1;
#endif
					 
						if( Ship != (u_int16_t) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
								if( Ship == WhoIAm )
									if( bSoundEnabled && BikeCompSpeechSlider.value ) 
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Secondary != (u_int16_t) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_MINE;
								SecBulls[ i ].Target = Secondary;
							}
						}
						break;

					case OWNER_NOBODY:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
#if 1
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (u_int16_t) -1 )
						{
							Ship = (u_int16_t) -1;
						}
#else
						Secondary = (u_int16_t) -1;
#endif
					 
						if( Ship != (u_int16_t) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
								if( Ship == WhoIAm )
									if( bSoundEnabled && BikeCompSpeechSlider.value )
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Secondary != (u_int16_t) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_MINE;
								SecBulls[ i ].Target = Secondary;
							}
						}
						break;

					default:
						break;
				}
			}
   			break;

/*===================================================================
		Missile Homing ( Heatseaker/Scatter/Thief )
===================================================================*/
   		case MIS_HOMING:													/* Homing toward target ( could lose target ) */
			if( StillGotTarget( &SecBulls[ i ].Pos, &SecBulls[ i ].DirVector,
								SecBulls[ i ].GroupImIn, SecBulls[i].TargetType, SecBulls[ i ].Target,
								SecBulls[ i ].ViewCone ) )
			{
				switch( SecBulls[i].TargetType )
				{
					case OWNER_SHIP:
		   				NormVector.x = ( Ships[ SecBulls[i].Target ].Object.Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( Ships[ SecBulls[i].Target ].Object.Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( Ships[ SecBulls[i].Target ].Object.Pos.z - SecBulls[i].Pos.z );
						break;

					case OWNER_ENEMY:
						Enemies[ SecBulls[i].Target ].AIFlags |= AI_TARGETED;
						Enemies[ SecBulls[i].Target ].TargetingPos = SecBulls[ i ].Pos;
						Enemies[ SecBulls[i].Target ].TargetingDir = SecBulls[ i ].DirVector;

		   				NormVector.x = ( Enemies[ SecBulls[i].Target ].Object.Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( Enemies[ SecBulls[i].Target ].Object.Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( Enemies[ SecBulls[i].Target ].Object.Pos.z - SecBulls[i].Pos.z );
						break;

					case OWNER_MINE:
		   				NormVector.x = ( SecBulls[ SecBulls[i].Target ].Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( SecBulls[ SecBulls[i].Target ].Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( SecBulls[ SecBulls[i].Target ].Pos.z - SecBulls[i].Pos.z );
						break;
				}

				if( ( SecBulls[i].TargetType == OWNER_SHIP ) && ( SecBulls[i].Target == WhoIAm ) )
				{
					DistToTarget = VectorLength( &NormVector );
					if( DistToTarget < MissileDistance )
					{
						MissileDistance = DistToTarget;
						MissileNum = i;
						ImTargeted = true;
					}
				}

				NormaliseVector( &NormVector );

				Cos = DotProduct( &NormVector, &SecBulls[ i ].DirVector );

				QuatFrom2Vectors( &qlerp.end, &Forward, &NormVector );
   		
   				qlerp.start	= SecBulls[ i ].DirQuat;				/* Current Quaternion */
   				qlerp.crnt	= &SecBulls[ i ].DirQuat;				/* Address to store new Quaternion */
   				qlerp.dir	= QuatDotProduct( &qlerp.start, &qlerp.end );

				if( Cos < -1.0F ) Cos = -1.0F;
				else if ( Cos > 1.0F ) Cos = 1.0F;
   				Angle = (float) acos( Cos );
   				if( Angle ) qlerp.time = ( ( D2R( SecBulls[ i ].TurnSpeed ) * NewFramelag ) / Angle ); //framelag ) / Angle );
   				else qlerp.time = 1.0F;
   				if( qlerp.time > 1.0F ) qlerp.time = 1.0F;
   		
   				QuatInterpolate( &qlerp );
   				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
				ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
				ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );
			}
			else
			{
   				SecBulls[ i ].State = MIS_STRAIGHT;
   				SecBulls[ i ].Target = (u_int16_t) -1;
				SecBulls[ i ].ColFlag = 0;
			}
   			break;

/*===================================================================
		Missile gravity
===================================================================*/
		case MIS_GRAVITY:
			QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
			QuatToMatrix( &TempQuat, &UpMatrix );
			MatrixTranspose( &UpMatrix, &InvUpMatrix );

			ApplyMatrix( &UpMatrix, &SecBulls[i].DirVector, &DirVector );

			NormVector.x = DirVector.x;
			NormVector.y = 0.0F;
			NormVector.z = DirVector.z;
			Speed = ( VectorLength( &NormVector ) * 0.995F );
			NormaliseVector( &NormVector );
			DirVector.x = ( NormVector.x * Speed );
			DirVector.z = ( NormVector.z * Speed );
	     	DirVector.y -= ( ( 0.08F * GLOBAL_SCALE ) * NewFramelag ); //framelag );

			ApplyMatrix( &InvUpMatrix, &DirVector, &SecBulls[i].DirVector );
			break;

/*===================================================================
		Should never be in here
===================================================================*/
   		case MIS_SPIRALING:												/* Spiraling missile */
			Angle = (float) D2R( ( ( SecBulls[i].LifeCount * 6.0F ) - ( floor( ( SecBulls[i].LifeCount * 6.0F ) / 360.0F ) * 360.0F ) ) );

			Rotation.x = (float) sin( Angle );
			Rotation.y = (float) cos( Angle );
			Rotation.z = 0.0F;
			ApplyMatrix( &SecBulls[i].StartMat, &Rotation, &UpVector );	// Calc Up Vector

			TempPoint1.x = ( UpVector.x * SPIRAL_RADIUS );
			TempPoint1.y = ( UpVector.y * SPIRAL_RADIUS );
			TempPoint1.z = ( UpVector.z * SPIRAL_RADIUS );

			Angle = (float) D2R( ( ( SecBulls[i].LifeCount * 6.0F ) + 30.0F ) - ( floor( ( ( SecBulls[i].LifeCount * 6.0F ) + 30.0F ) / 360.0F ) * 360.0F ) );

			Rotation.x = (float) sin( Angle );
			Rotation.y = (float) cos( Angle );
			Rotation.z = 0.0F;
			ApplyMatrix( &SecBulls[i].StartMat, &Rotation, &UpVector );	// Calc Up Vector

			TempPoint2.x = ( UpVector.x * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.x * ( 100.0F * GLOBAL_SCALE ) );
			TempPoint2.y = ( UpVector.y * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.y * ( 100.0F * GLOBAL_SCALE ) );
			TempPoint2.z = ( UpVector.z * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.z * ( 100.0F * GLOBAL_SCALE ) );

			TempVector.x = ( TempPoint1.x - TempPoint2.x );
			TempVector.y = ( TempPoint1.y - TempPoint2.y );
			TempVector.z = ( TempPoint1.z - TempPoint2.z );
			NormaliseVector( &TempVector );

			QuatFromVector2( &TempVector, &TempQuat );
			QuatToMatrix( &TempQuat, &SecBulls[i].Mat );
			ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
			ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );
			break;

/*===================================================================
		Retract Tentacle 
===================================================================*/
		case MIS_RETRACTTENTACLE:
			SecBulls[i].Speed = 0.0F;

			if( SecBulls[ i ].NumOldPos )
			{
				TempVector.x = ( SecBulls[i].OldPos[ (int16_t) SecBulls[i].RetractPos ].Pos.x - SecBulls[i].Pos.x );
				TempVector.y = ( SecBulls[i].OldPos[ (int16_t) SecBulls[i].RetractPos ].Pos.y - SecBulls[i].Pos.y );
				TempVector.z = ( SecBulls[i].OldPos[ (int16_t) SecBulls[i].RetractPos ].Pos.z - SecBulls[i].Pos.z );
				SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &TempVector );

				if( OldGroup != SecBulls[ i ].GroupImIn )
				{
					MoveSecBullToGroup( i, OldGroup, SecBulls[ i ].GroupImIn );
					OldGroup = SecBulls[i].GroupImIn;
				}

				SecBulls[i].Pos = SecBulls[i].OldPos[ (int16_t) SecBulls[i].RetractPos ].Pos;
				SecBulls[i].DirVector = SecBulls[i].OldPos[ (int16_t) SecBulls[i].RetractPos ].Dir;
				QuatFromDirAndUp( &SecBulls[ i ].DirVector, &SecBulls[ i ].UpVector, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );							// Create Dir Matrix
	
				if( (int16_t) SecBulls[i].RetractPos != ( MAXTENTOLDPOS - 1 ) )
				{
					SecBulls[i].RetractPos += ( 0.5F * NewFramelag ); //framelag );
				}

				if( SecBulls[i].RetractPos > (float) ( SecBulls[ i ].NumOldPos - 1 ) )
				{
					SecBulls[i].RetractPos = (float) ( SecBulls[ i ].NumOldPos - 1 );
					DestroySecondary( i, &SecBulls[i].Pos );
					return( false );
				}
			}
			else
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( false );
			}
   			break;

/*===================================================================
		Should never be in here
===================================================================*/
   		default:															/* Unknown/Undefined Missile */
   			break;
   	}

/*===================================================================
			Do Collision Routines
===================================================================*/
	Speed = ( SecBulls[ i ].Speed * NewFramelag ); //framelag );
	
	if( SecBulls[i].State == MIS_DROP )
	{																		/* NewPos when dropping */
		if( Speed > ( SecBulls[ i ].Speed * 2.0F ) ) Speed = ( SecBulls[ i ].Speed * 2.0F );
		NewPos.x = SecBulls[ i ].Pos.x + ( SecBulls[ i ].DropVector.x * Speed );
		NewPos.y = SecBulls[ i ].Pos.y + ( SecBulls[ i ].DropVector.y * Speed );
		NewPos.z = SecBulls[ i ].Pos.z + ( SecBulls[ i ].DropVector.z * Speed );
	}
	else
	{
		NewPos.x = SecBulls[ i ].Pos.x + ( SecBulls[ i ].DirVector.x * Speed );
		NewPos.y = SecBulls[ i ].Pos.y + ( SecBulls[ i ].DirVector.y * Speed );
		NewPos.z = SecBulls[ i ].Pos.z + ( SecBulls[ i ].DirVector.z * Speed );
	}

	DirVector.x = ( NewPos.x - SecBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
	DirVector.y = ( NewPos.y - SecBulls[ i ].Pos.y );
	DirVector.z = ( NewPos.z - SecBulls[ i ].Pos.z );
	
	Length = VectorLength( &DirVector );									/* Length of Vector */ 

	switch( SecBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			TriggerAreaPlayerShootsCheck( &SecBulls[i].Pos, &NewPos, SecBulls[i].GroupImIn, WEPTYPE_Secondary, SecBulls[i].Weapon );
			break;

		case OWNER_ENEMY:
			TriggerAreaEnemyShootsCheck( &SecBulls[i].Pos, &NewPos, SecBulls[i].GroupImIn, WEPTYPE_Secondary, SecBulls[i].Weapon );
			break;
	}

	DistToInt = 10000.0F;									/* Distance to Intersection */
	HitWall = (u_int16_t) -1;

	if( Length )
	{
		if( ( SecBulls[ i ].State == MIS_STRAIGHT ) || ( SecBulls[ i ].State == MIS_ACCELERATE ) )
		{
			if( !SecBulls[i].ColFlag )
			{
				temp.x = ( SecBulls[ i ].DirVector.x * MaxColDistance );
				temp.y = ( SecBulls[ i ].DirVector.y * MaxColDistance );
				temp.z = ( SecBulls[ i ].DirVector.z * MaxColDistance );
		
				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &SecBulls[i].Pos,
									SecBulls[i].GroupImIn, &temp, (VECTOR *) &SecBulls[i].ColPoint,
									&SecBulls[i].ColGroup, &SecBulls[i].ColPointNormal, &TempVector, false, NULL ) )
				{
					DebugPrintf( "Secondary weapon %d didn't collide with backgroup in group %d\n", SecBulls[i].Weapon, SecBulls[i].GroupImIn );
					if( DebugInfo ) CreateDebugLine( &SecBulls[i].Pos, &temp, SecBulls[i].GroupImIn, 255, 64, 64 );
					CleanUpSecBull( i );
					return( false );
				}
				else
				{
					SecBulls[i].ColFlag = 1;
					SecBulls[i].ColStart = SecBulls[i].Pos;
					SecBulls[i].ColDist = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );
				}
			}

			DistFromStart = (float) fabs( DistanceVert2Vector( (VERT*) &NewPos, &SecBulls[i].ColStart ) );
	
			if( SecBulls[i].ColDist < DistFromStart )
	   		{
				HitWall = 0;
				DistToInt = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );
			}

			if( HitWall == (u_int16_t) -1 )
			{
				if( OneGroupBGObjectCol( 0.0F, 0, SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DirVector,
					 (VECTOR *) &SecBulls[i].ColPoint, &SecBulls[i].ColPointNormal, &TempVector, &BGObject, 0.0F ) )
				{
					HitWall = 0;
					DistToInt = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );

					if( BGObject )
					{
						ChangeBGState( BGObject, SecBulls[i].OwnerType, SecBulls[i].Owner, SHOT, SecBulls[i].Damage );
					}
				}
			}

		}
		else
		{
			if( BackgroundCollide( &MCloadheadert0, &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn,
									&DirVector, (VECTOR *) &SecBulls[i].ColPoint, &SecBulls[i].ColGroup, &SecBulls[i].ColPointNormal,
									&TempVector, true, NULL ) == true )
			{
				TempVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
				TempVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
				TempVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
				DistToInt = VectorLength( &TempVector );
				HitWall = 0;
			}
		}

		if ( WaterObjectCollide( SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DirVector, &Int_Point, SecBulls[i].Damage ) )
		{
			CreateSplash( (VECTOR *) &Int_Point, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
		}
	}

	HitTarget = CheckHitShip( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &Int_Point, &Int_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );

	if( HitTarget != (u_int16_t) -1 ) HitWall = (u_int16_t) -1;

	if( SecBulls[i].ColRadius ) HitSecondary = CheckHitSecondary( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &MInt_Point, &MInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );
	else HitSecondary = CheckHitSecondary( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &MInt_Point, &MInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Point );

	if( HitSecondary != (u_int16_t) -1 )
	{
		HitWall = (u_int16_t) -1;
		HitTarget = (u_int16_t) -1;
	}
	
	if( SecBulls[i].ColRadius )	HitEnemy = CheckHitEnemy( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &EInt_Point, &EInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );
	else HitEnemy = CheckHitEnemy( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &EInt_Point, &EInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Point );

	if( HitEnemy != NULL )
	{
		HitWall = (u_int16_t) -1;
		HitTarget = (u_int16_t) -1;
		HitSecondary = (u_int16_t) -1;
	}
	else
	{
		TempEnemyIndex = GetComEnemyHitInfo( &EInt_Point, &ENormal, &DistToInt );

		if( TempEnemyIndex != (u_int16_t) -1 )
		{
			HitEnemy = &Enemies[ TempEnemyIndex ];
			EInt_Point2 = EInt_Point;
			HitWall = (u_int16_t) -1;
			HitTarget = (u_int16_t) -1;
			HitSecondary = (u_int16_t) -1;
		}
	}

/*===================================================================
			Hit Secondary
===================================================================*/
	if( HitSecondary != (u_int16_t) -1 )							/* Hit Secondary? */
	{
		SecBulls[ HitSecondary ].Shield -= SecBulls[i].Damage;
		if( SecBulls[ HitSecondary ].Shield < 0.0F ) SecBulls[ HitSecondary ].Shield = 0.0F;

		if( SecBulls[ HitSecondary ].Shield == 0.0F )
		{
			if( *NextMissile == HitSecondary )
			{
				*NextMissile = SecBulls[ HitSecondary ].Prev;
			}

			switch( SecBulls[ HitSecondary ].Weapon )
			{
				case ENEMYSPIRALMISSILE:
				case ENEMYHOMINGMISSILE:
				case ENEMYBLUEHOMINGMISSILE:
				case GRAVGONMISSILE:
				case MUGMISSILE:
				case MULTIPLEMISSILE:
				case SOLARISMISSILE:
				case ENEMYTENTACLE:
				case SCATTERMISSILE:
				default:
					MissileShockWave( (VECTOR *) &MInt_Point, MISSILE_SHOCKWAVE, SecBulls[ HitSecondary ].Owner,
									( SecBulls[ HitSecondary ].Damage / 2.5F ), SecBulls[ HitSecondary ].GroupImIn,
									SecBulls[ HitSecondary ].Weapon );
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case ENEMYFIREBALL:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case ENEMYDEPTHCHARGE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case THIEFMISSILE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case PINEMISSILE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case TITANSTARSHRAPNEL:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case TITANSTARMISSILE:
					TempVector.x = 0.0F;
					TempVector.y = 1.0F;
					TempVector.z = 0.0F;
					CreateExplosionDonut( &SecBulls[ HitSecondary ].Pos, (VECTOR *) &TempVector, SecBulls[ HitSecondary ].GroupImIn );
					if( ( ( SecBulls[ HitSecondary ].OwnerType == OWNER_SHIP ) && ( SecBulls[ HitSecondary ].Owner == WhoIAm ) ) ||
						( ( SecBulls[ HitSecondary ].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
						( ( SecBulls[ HitSecondary ].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
					{
						NewDir.x = ( MInt_Point.x - SecBulls[ HitSecondary ].Pos.x );
						NewDir.y = ( MInt_Point.y - SecBulls[ HitSecondary ].Pos.y );
						NewDir.z = ( MInt_Point.z - SecBulls[ HitSecondary ].Pos.z );
						TempGroup = MoveGroup( &Mloadheader, &SecBulls[ HitSecondary ].Pos, SecBulls[ HitSecondary ].GroupImIn, &NewDir );
						CreateTitanStarShrapnel( HitSecondary, (VECTOR *) &MInt_Point, &SecBulls[ HitSecondary ].DirVector, TempGroup );
					}
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case PURGEMINE:
				case PINEMINE:
				case QUANTUMMINE:
				case SPIDERMINE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;
			}
		}

		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			case ENEMYTENTACLE:
			case SCATTERMISSILE:
			default:
				MissileShockWave( (VECTOR *) &MInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case ENEMYFIREBALL:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case PURGEMINE:
			case PINEMINE:
			case QUANTUMMINE:
			case SPIDERMINE:
			case PINEMISSILE:
			case TITANSTARSHRAPNEL:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case TITANSTARMISSILE:
				TempVector.x = 0.0F;
				TempVector.y = 1.0F;
				TempVector.z = 0.0F;
				CreateExplosionDonut( &SecBulls[ i ].Pos, (VECTOR *) &TempVector, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( MInt_Point.x - SecBulls[i].Pos.x );
					NewDir.y = ( MInt_Point.y - SecBulls[i].Pos.y );
					NewDir.z = ( MInt_Point.z - SecBulls[i].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, (VECTOR *) &MInt_Point, &SecBulls[ i ].DirVector, TempGroup );
				}
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;
		}
		return( false );
	}
	
/*===================================================================
			Hit Ship
===================================================================*/
	if( HitTarget != (u_int16_t) -1 )												/* Hit Target? */
	{
	 	PlayPannedSfx( SFX_ShipHit, Ships[ HitTarget ].Object.Group , &Int_Point, 0.0F );

		switch( ColPerspective )
		{
			case COLPERS_Forsaken:
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( WhoIAm == SecBulls[i].Owner ) ) || ( SecBulls[i].OwnerType != OWNER_SHIP ) )
   				{
   					Damage = SecBulls[i].Damage;

   					Recoil.x = ( SecBulls[i].DirVector.x * ( Damage / 10.0F ) );
   					Recoil.y = ( SecBulls[i].DirVector.y * ( Damage / 10.0F ) );
   					Recoil.z = ( SecBulls[i].DirVector.z * ( Damage / 10.0F ) );

					if( SecBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

					if( HitTarget == WhoIAm )
					{
						HitMe( SecBulls[i].OwnerType, SecBulls[i].Owner, Damage, WEPTYPE_Secondary, SecBulls[i].Weapon );
						ForceExternalOneOff( WhoIAm, &Recoil );
					}
					else
					{
	   					IHitYou( (BYTE) HitTarget, Damage, &Recoil, &Int_Point, &SecBulls[i].DirVector, D2R( ( Damage / 10.0F ) ), WEPTYPE_Secondary, SecBulls[i].Weapon, ONEOFF_RECOIL );
					}
   				}
				break;

			case COLPERS_Descent:
				if( HitTarget == WhoIAm )
   				{
   					Damage = SecBulls[i].Damage;

   					Recoil.x = ( SecBulls[i].DirVector.x * ( Damage / 10.0F ) );
   					Recoil.y = ( SecBulls[i].DirVector.y * ( Damage / 10.0F ) );
   					Recoil.z = ( SecBulls[i].DirVector.z * ( Damage / 10.0F ) );

					if( SecBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

					HitMe( SecBulls[i].OwnerType, SecBulls[i].Owner, Damage, WEPTYPE_Secondary, SecBulls[i].Weapon );
					ForceExternalOneOff( WhoIAm, &Recoil );
   				}
				break;
		}

//		if( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) )
		if( ( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) ) )
		{
			if( Ships[ HitTarget ].Object.Shield )
			{
				switch( SecBulls[ i ].Weapon )
				{
					case THIEFMISSILE:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 2, 255, 192, 0 );
						break;

					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case MUGMISSILE:
					case SOLARISMISSILE:
					case ENEMYTENTACLE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case TITANSTARSHRAPNEL:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 5, 255, 255, 255 );
						break;
					case MULTIPLEMISSILE:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 1, 255, 255, 255 );
						break;
				}
			}
			else
			{
				switch( SecBulls[ i ].Weapon )
				{
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case MUGMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case ENEMYTENTACLE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case MULTIPLEMISSILE:
					case TITANSTARSHRAPNEL:
						TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
						TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
						TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
						NormaliseVector( &TempVector );
						CreateColSparks( &Int_Point, &TempVector, Ships[ HitTarget ].Object.Group );
						break;
				}
			}
		}

		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case ENEMYFIREBALL:
			case ENEMYDEPTHCHARGE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case GRAVGONMISSILE:
			case MULTIPLEMISSILE:
			case PINEMISSILE:
			case TITANSTARSHRAPNEL:
			default:
				DestroySecondary( i, &Int_Point );
				return( false );
				break;

			case TITANSTARMISSILE:
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( Int_Point.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( Int_Point.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( Int_Point.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
					TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
					TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
					NormaliseVector( &TempVector );
					CreateTitanStarShrapnel( i, &Int_Point, &TempVector, TempGroup );
				}
				DestroySecondary( i, &Int_Point );
				return( false );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, &Int_Point );
				return( false );
				break;
	
			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;
				break;

			case SCATTERMISSILE:
				switch( ColPerspective )
				{
					case COLPERS_Forsaken:
						break;

					case COLPERS_Descent:
						if( HitTarget == WhoIAm )
						{
							if( !GodMode )
							{
								TempVector.x = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
								TempVector.y = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
								TempVector.z = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
								NormaliseVector( &TempVector );
								ScatterWeapons( &TempVector, MAXSCATTERED );
								PlayPannedSfx( SFX_Scattered, Ships[ HitTarget ].Object.Group , &Ships[ HitTarget ].Object.Pos, 0.0F );
							}
						}
						break;
				}
				DestroySecondary( i, &Int_Point );
				return( false );
				break;
		}
	}
	
/*===================================================================
			Hit Wall
===================================================================*/
	if( HitWall != (u_int16_t) -1 )
	{
		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			case SCATTERMISSILE:
			default:
				MissileShockWave( (VECTOR *) &SecBulls[i].ColPoint, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( false );
				break;

			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;

				CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, TENTACLE_SHOCKWAVE, SecBulls[i].Weapon );
				break;

			case ENEMYFIREBALL:
				if( SecBulls[i].NumBounces )
				{
					SecBulls[i].NumBounces--;
					DirVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
					DirVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
					DirVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
					NewPos.x = SecBulls[i].ColPoint.x;
					NewPos.y = SecBulls[i].ColPoint.y;
					NewPos.z = SecBulls[i].ColPoint.z;
       				TempVector = SecBulls[i].DirVector;
       				ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &SecBulls[i].DirVector );
					SecBulls[i].ColFlag = 0;
				}
				else
				{
					DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				}
				return( false );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( false );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( false );
				break;

			case PINEMISSILE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( false );
				break;

			case TITANSTARSHRAPNEL:
				if( SecBulls[i].LifeCount < 15.0F )
				{
					DirVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
					DirVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
					DirVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
					NewPos.x = SecBulls[i].ColPoint.x;
					NewPos.y = SecBulls[i].ColPoint.y;
					NewPos.z = SecBulls[i].ColPoint.z;
        			TempVector = SecBulls[i].DirVector;
        			ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &SecBulls[i].DirVector );
					SecBulls[i].ColFlag = 0;
				}
				else
				{
					DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
					return( false );
				}
				break;

			case TITANSTARMISSILE:
       			TempVector = SecBulls[i].DirVector;
       			ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &TempVector );

				CreateExplosionDonut( &SecBulls[ i ].Pos, (VECTOR *) &SecBulls[ i ].ColPointNormal, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP )
#ifdef TITANBITS_SEND	
						&& ( SecBulls[i].Owner == WhoIAm )
#endif
					) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( SecBulls[ i ].ColPoint.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( SecBulls[ i ].ColPoint.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( SecBulls[ i ].ColPoint.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, (VECTOR *) &SecBulls[ i ].ColPoint, (VECTOR *) &TempVector, TempGroup );
				}
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( false );
				break;
		}
	}

/*===================================================================
			Hit Enemy
===================================================================*/
	if( HitEnemy != NULL )
	{
		TempVector.x = ( EInt_Point.x - HitEnemy->Object.Pos.x );
		TempVector.y = ( EInt_Point.y - HitEnemy->Object.Pos.y );
		TempVector.z = ( EInt_Point.z - HitEnemy->Object.Pos.z );
		NormaliseVector( &TempVector );
	
  		ReflectVector( &SecBulls[i].DirVector, (NORMAL *) &TempVector, &NewDir );

		if( !( HitEnemy->Object.Flags & SHIP_Invul ) )
		{
			if( DamageEnemy( HitEnemy , -SecBulls[i].Damage , &EInt_Point , &SecBulls[i].DirVector , SecBulls[ i ].Speed , SecBulls[ i ].Owner , SecBulls[ i ].OwnerType ) )
			{
				CreateNewExplosion( &EInt_Point, &NewDir, SecBulls[i].GroupImIn );
				KillUsedEnemy( HitEnemy );
			}
			else
			{
				CreateColSparks( &EInt_Point, &TempVector, HitEnemy->Object.Group );
			}
		}
		else
		{
			if( !Random_Range( 4 ) )
			{
				PlayPannedSfx( SFX_Ric, SecBulls[i].GroupImIn, (VECTOR *) &EInt_Point, 0.0F );
			}
		}
	
		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			default:
				MissileShockWave( &EInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case ENEMYFIREBALL:
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;
				break;

			case SCATTERMISSILE:
				if( HitEnemy->PickupHeld != (u_int16_t) -1 )
				{
					if( !( HitEnemy->Object.Flags & SHIP_Scattered ) )
					{
	   					TempVector.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   					TempVector.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   					TempVector.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
						NormaliseVector( &TempVector );
						if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
						else LifeCount = -1.0F;
						Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
						InitOnePickup( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, Speed, HitEnemy->PickupHeld, (u_int16_t) -1, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
						if( IsHost ) DropPickupSend( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, 0.0F, HitEnemy->PickupHeld, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
						if( HitEnemy->PickupHeld <= LASER ) HitEnemy->Object.Flags |= SHIP_Scattered;
						HitEnemy->PickupHeld = (u_int16_t) -1;
					}
				}
				else
				{
	   				TempVector.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   				TempVector.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   				TempVector.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
					NormaliseVector( &TempVector );
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					InitOnePickup( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, Speed, PICKUP_Shield, (u_int16_t) -1, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					if( IsHost ) DropPickupSend( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, 0.0F, PICKUP_Shield, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				}
				MissileShockWave( &EInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case PINEMISSILE:
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case TITANSTARSHRAPNEL:
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;

			case TITANSTARMISSILE:
				CreateExplosionDonut( &SecBulls[ i ].Pos, &TempVector, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( EInt_Point.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( EInt_Point.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( EInt_Point.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, &EInt_Point, &NewDir, TempGroup );
				}
				DestroySecondary( i, &EInt_Point );
				return( false );
				break;
		}
	}

	SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &DirVector );
	SecBulls[i].Pos = NewPos;

	if( OldGroup != SecBulls[ i ].GroupImIn )
	{
		MoveSecBullToGroup( i, OldGroup, SecBulls[ i ].GroupImIn );
	}

/*===================================================================
			Do Trails
===================================================================*/
	if( IsGroupVisible[ SecBulls[i].GroupImIn ] )
	{
		if( ( SecBulls[i].State != MIS_DROP ) &&
			( ( SecBulls[i].Speed != 0.0F ) || ( SecBulls[i].State == MIS_RETRACTTENTACLE ) ) )	/* No Smoke Trail when Dropping or Stationary */
		{
			if( !InWater( SecBulls[i].GroupImIn, &SecBulls[i].Pos, &Damage ) )
			{
				switch( SecBulls[i].Weapon )
				{
					case ENEMYTENTACLE:
						CreateTentacle( i );
						break;

					case SCATTERMISSILE:
						CreateScatterTrail( i );
						break;
			
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case GRAVGONMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case MUGMISSILE:
					case MULTIPLEMISSILE:
					case PINEMISSILE:
					case TITANSTARMISSILE:
					case TITANSTARSHRAPNEL:
			
						SecBulls[i].Time += NewFramelag; //framelag;
						if( SecBulls[i].Time >= SecBulls[i].Interval )
						{
						 	SecBulls[i].Time = FMOD( SecBulls[i].Time, SecBulls[i].Interval );
				
							switch( SecBulls[i].Weapon )
							{
								case GRAVGONMISSILE:
									CreateGravgonTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case SOLARISMISSILE:
									CreateSolarisTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case ENEMYTENTACLE:
								case SCATTERMISSILE:
								case ENEMYHOMINGMISSILE:
								case ENEMYBLUEHOMINGMISSILE:
									break;
					
								case THIEFMISSILE:
									CreateNewTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case ENEMYSPIRALMISSILE:
								case MUGMISSILE:
								case MULTIPLEMISSILE:
								case PINEMISSILE:
								case TITANSTARMISSILE:
									CreateTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case TITANSTARSHRAPNEL:
									CreateNewTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;

								case ENEMYFIREBALL:
									CreateFireBall( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;

								case ENEMYDEPTHCHARGE:
									break;
							}
						}

						break;
				}
			}
			else
			{
				switch( SecBulls[i].Weapon )
				{
					case ENEMYTENTACLE:
						CreateTentacle( i );
						break;

					case SCATTERMISSILE:
						CreateScatterTrail( i );
						break;
			
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case GRAVGONMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case MUGMISSILE:
					case MULTIPLEMISSILE:
					case PINEMISSILE:
					case TITANSTARMISSILE:
					case TITANSTARSHRAPNEL:
					case ENEMYDEPTHCHARGE:
			
						SecBulls[i].Time += NewFramelag; //framelag;
						if( SecBulls[i].Time >= SecBulls[i].Interval )
						{
						 	SecBulls[i].Time = FMOD( SecBulls[i].Time, SecBulls[i].Interval );
				
							TempVector.x = ( SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 200.0F * GLOBAL_SCALE ) ) );
							TempVector.y = ( SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 200.0F * GLOBAL_SCALE ) ) );
							TempVector.z = ( SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 200.0F * GLOBAL_SCALE ) ) );

							switch( SecBulls[i].Weapon )
							{
								case GRAVGONMISSILE:
								case SOLARISMISSILE:
								case THIEFMISSILE:
								case ENEMYSPIRALMISSILE:
								case ENEMYHOMINGMISSILE:
								case ENEMYBLUEHOMINGMISSILE:
								case ENEMYDEPTHCHARGE:
								case MUGMISSILE:
								case MULTIPLEMISSILE:
								case PINEMISSILE:
								case TITANSTARMISSILE:
								case TITANSTARSHRAPNEL:
									CreateBubbleTrail( &TempVector, &SecBulls[i].DirVector, SecBulls[i].GroupImIn, 64, 64, 64 );
									break;
							}
						}
						break;

					case ENEMYFIREBALL:
						CreateSpotFXBurning( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
						break;

					default:
						break;
				}
			}
		}
	}

	return( true );
}

/*===================================================================
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateExplosion( VECTOR * Pos, u_int16_t Group )
{
	u_int16_t	fmpoly;
	int16_t	Count;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		for( Count = 0; Count < ( Random_Range( 3 ) + 1 ); Count++ )
		{
			fmpoly = FindFreeFmPoly();
	
			if( fmpoly != (u_int16_t ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
	
				if( Count == 0 )
				{
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					FmPolys[ fmpoly ].xsize = NORM_EXPLO_SIZE;
					FmPolys[ fmpoly ].ysize = NORM_EXPLO_SIZE;
				}
				else
				{
					FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
					FmPolys[ fmpoly ].xsize = Scale;
					FmPolys[ fmpoly ].ysize = Scale;
				}
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
			else
			{
				return;
			}
		}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*===================================================================
	Procedure	:	Create Small Explosion
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSmallExplosion( VECTOR * Pos, u_int16_t Group )
{
	u_int16_t	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].R = 192;
			FmPolys[ fmpoly ].G = 192;
			FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Dir.x = 0.0F;
			FmPolys[ fmpoly ].Dir.y = 0.0F;
			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
			FmPolys[ fmpoly ].xsize = Scale;
			FmPolys[ fmpoly ].ysize = Scale;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*===================================================================
	Procedure	:	Create Big Explosion
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateBigExplosion( VECTOR * Pos, u_int16_t Group )
{
	int16_t	Count;
	int16_t	NumExplosions;
	u_int16_t	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		NumExplosions = ( Random_Range( 3 ) + 1 );

		for( Count = 0; Count < NumExplosions; Count++ )
		{
			fmpoly = FindFreeFmPoly();
		
			if( fmpoly != (u_int16_t ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
				FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				Scale = (float) ( Random_Range( 20 ) + 12 ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].xsize = Scale;
				FmPolys[ fmpoly ].ysize = Scale;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		}
	}
}

/*===================================================================
	Procedure	:	Create Single Random sized Explosion
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSingleExplosion( VECTOR * Pos, u_int16_t Group )
{
	u_int16_t	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].R = 192;
			FmPolys[ fmpoly ].G = 192;
			FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Dir.x = 0.0F;
			FmPolys[ fmpoly ].Dir.y = 0.0F;
			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			Scale = ( ( (float) ( Random_Range( 12 ) + 4 ) ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].xsize = Scale;
			FmPolys[ fmpoly ].ysize = Scale;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateShipExplosion( VECTOR * Pos, u_int16_t Group )
{
	u_int16_t	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (u_int16_t ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_ANIM2;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Exp2_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].Trans = 255;
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = SHIP_EXPLO_SIZE;
			FmPolys[ fmpoly ].ysize = SHIP_EXPLO_SIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*===================================================================
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_SMOKE_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = SMOKE_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = SMOKE_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateBloodTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();
	
   	if( fmpoly != (u_int16_t ) -1 )
   	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
   		FmPolys[ fmpoly ].Pos = *Pos;
   		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].SeqNum = FM_BLOOD;
   		FmPolys[ fmpoly ].Frame = 0.0F;
   		FmPolys[ fmpoly ].Frm_Info = &Blood_Header; //&Bits_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
  		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].R = 192;
		FmPolys[ fmpoly ].G = 192;
		FmPolys[ fmpoly ].B = 192;
   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 2.0F ) * GLOBAL_SCALE;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = BLOODSIZE;
		FmPolys[ fmpoly ].ysize = BLOODSIZE;
		FmPolys[ fmpoly ].Group = Group;
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Blood splat
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateBloodSplat( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	VECTOR	TempUp;
	VECTOR	TempVector;
	QUAT	TempQuat;
	MATRIX	TempMat;
	u_int16_t	fmpoly;
	float	Angle;

	if( IsGroupVisible[ Group ] )
	{
		Angle = DotProduct( Dir, &SlideUp );
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (u_int16_t ) -1 )
		{
			QuatFrom2Vectors( &TempQuat, &Forward, Dir );
		   	QuatToMatrix( &TempQuat, &TempMat );
		   	ApplyMatrix( &TempMat, &Mloadheader.Group[ Group ].up, &TempUp );
		
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Frame = 0.0F;
	
			if( ( Angle <= 0.9F ) && ( Angle >= -0.9F ) )
			{
			  	FmPolys[ fmpoly ].Frm_Info = &BloodDribble_Header;
			  	FmPolys[ fmpoly ].SeqNum = FM_BLOODDRIBBLE;
				CrossProduct( Dir, &Mloadheader.Group[ Group ].up, &TempVector );	/* Calc Temp Left Vector */
				CrossProduct( &TempVector, Dir, &TempUp );						/* Calc Up Vector */
			}
			else
			{
			  	FmPolys[ fmpoly ].Frm_Info = &BloodSplat_Header;
			  	FmPolys[ fmpoly ].SeqNum = FM_BLOODSPLAT;
			}
	
		  	FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
		   	FmPolys[ fmpoly ].Trans = 255;
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		  	FmPolys[ fmpoly ].DirVector = *Dir;
		  	FmPolys[ fmpoly ].UpVector = TempUp;
		   	FmPolys[ fmpoly ].R = 192; //255;
		   	FmPolys[ fmpoly ].G = 50; //255;
		   	FmPolys[ fmpoly ].B = 50; //255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = BLOODSIZE * 2.0F;
		   	FmPolys[ fmpoly ].ysize = BLOODSIZE * 2.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*===================================================================
	Procedure	:	Create Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateGravgonTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_GRAVGON_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &GravgonTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = GRAVGON_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = GRAVGON_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSolarisTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_SOLARIS_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &SolarisTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = SOLARIS_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = SOLARIS_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create New Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateNewTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_NEW_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = NEW_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = NEW_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Init Link List
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitLinkList( void )
{
	int16_t		Count;

	FirstFree = &EntryList[ 0 ];													/* First Free = Entry[ 0 ] */
	FirstUsed = NULL;																/* None used */

	for( Count = 0; Count < MAX_ENTRYS; Count++ )
	{
		EntryList[ Count ].Prev = &EntryList[ ( Count - 1 ) ];						/* Prev Free = Prev Entry */
		EntryList[ Count ].Next = &EntryList[ ( Count + 1 ) ];						/* Next Free = Next Entry */
	}																				
																					
	EntryList[ 0 ].Prev = NULL;														/* Start of List */
	EntryList[ ( MAX_ENTRYS - 1 ) ].Next = NULL;									/* End of List */
}

/*===================================================================
	Procedure	:	Find Free Entry
	Input		:	nothing
	Output		:	ENTRY	*	Entry Address
===================================================================*/
ENTRY * FindFreeLink( void )
{
	ENTRY	*	NextFree;

	if( FirstFree == NULL ) return( NULL );											/* None free! Return NULL */
	NextFree = FirstFree->Next;														/* Get Next Free */

	if( FirstUsed != NULL ) FirstUsed->Prev = FirstFree;							/* Prev Used = First Free */
	FirstFree->Next = FirstUsed;													/* Next Used = First Used */
	FirstFree->Prev = NULL;															/* Prev Used = NULL */
	FirstUsed = FirstFree;															/* First Used = First Free */

	FirstFree = NextFree;															/* First Free = Next Free */
	if( FirstFree != NULL ) FirstFree->Prev = NULL;									/* Prev Free = NULL */
	
	return( FirstUsed );
}

/*===================================================================
	Procedure	:	Free Entry
	Input		:	ENTRY	*	Entry Address
	Output		:	Nothing
===================================================================*/
void FreeLink( ENTRY * Entry )
{
	ENTRY	*	PrevUsed;
	ENTRY	*	NextUsed;

	if( Entry == FirstUsed ) FirstUsed = Entry->Next;								/* If Firstused then next becomes first */

	PrevUsed = Entry->Prev;
	NextUsed = Entry->Next;
	if( PrevUsed != NULL ) PrevUsed->Next = NextUsed;								/* Cut out link */
	if( NextUsed != NULL ) NextUsed->Prev = PrevUsed;

	Entry->Next = FirstFree;														/* Next Free = First Free */
	Entry->Prev = NULL;																/* Prev Free = NULL */
	if( FirstFree != NULL ) FirstFree->Prev = Entry;								/* Next Prev Free = Entry */
	FirstFree = Entry;																/* First Free = Entry */
}

/*===================================================================
	Procedure	:	Display Sec Ammo Level
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DispSecAmmoLevels( void )
{
	int16_t	x, y;
	int16_t	Ammo;

	x = ((int16_t)render_info.window_size.cx / 2 ) + ( 16 * 4 );
	y = (int16_t)render_info.window_size.cy - ( (int16_t)render_info.window_size.cy / 5 ) + 6;

	Ammo = SecondaryAmmo[ Ships[ WhoIAm ].Secondary ];

	if( Ammo ) Printu_int16_t( Ammo, x - ( 4 * 4 ), y, 2 );
	else RightJustifyPrint4x5Text( "NO AMMO", x, y, 2 );
}

/*===================================================================
	Procedure	:	Create Gravgon Missile Effect
	Input		:	u_int16_t	SecBull Index
	Output		:	Nothing
===================================================================*/
void CreateGravgonField( u_int16_t i )
{
	u_int16_t	Model;

	Model =	FindFreeModel();
	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = SecBulls[i].OwnerType;
		Models[ Model ].Owner = SecBulls[i].Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos.x = SecBulls[i].ColPoint.x;
		Models[ Model ].Pos.y = SecBulls[i].ColPoint.y;
		Models[ Model ].Pos.z = SecBulls[i].ColPoint.z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Scale;
		Models[ Model ].MaxScale = 10.0F;
		Models[ Model ].Scale = Models[ Model ].MaxScale;
		Models[ Model ].Group = SecBulls[i].GroupImIn;
		Models[ Model ].LifeCount = 600.0F;
	}
	
	Model =	FindFreeModel();
	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = SecBulls[i].OwnerType;
		Models[ Model ].Owner = SecBulls[i].Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos.x = SecBulls[i].ColPoint.x;
		Models[ Model ].Pos.y = SecBulls[i].ColPoint.y;
		Models[ Model ].Pos.z = SecBulls[i].ColPoint.z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Scale2;
		Models[ Model ].MaxScale = 10.0F;
		Models[ Model ].Scale = ( Models[ Model ].MaxScale / 2.0F );
		Models[ Model ].Group = SecBulls[i].GroupImIn;
		Models[ Model ].LifeCount = 600.0F;
	}
}

/*===================================================================
	Procedure	:	Create Explosion Dome
	Input		:	u_int16_t	OwnerType
				:	u_int16_t	Owner
				:	VECTOR	Pos
				:	u_int16_t	Group
				:	float	Shockwave Size
				:	BYTE	Weapon
	Output		:	Nothing
===================================================================*/
void CreateShockwave( u_int16_t OwnerType, u_int16_t Owner, VECTOR * Pos, u_int16_t Group, float ShockwaveSize, BYTE Weapon )
{
	u_int16_t	Model;

	Model =	FindFreeModel();
	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OwnerType;
		Models[ Model ].Owner = Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].SecWeapon = Weapon;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos.x = Pos->x;
		Models[ Model ].Pos.y = Pos->y;
		Models[ Model ].Pos.z = Pos->z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Explode;
		Models[ Model ].Scale = 0.0F;
		Models[ Model ].MaxScale = ShockwaveSize;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = -1.0F;
	}

	CreateBigFlash( Pos, Group, ShockwaveSize );
	CreateExplosion( Pos, Group );
}

/*===================================================================
	Procedure	:	Explode Secondary Weapon
	Input		:	VECTOR	*	Pos
				:	u_int16_t		Group
				:	u_int16_t		Owner
				:	u_int16_t		ID
				:	float		ShockwaveSize
	Output		:	int16_t		Number of generated pickups
===================================================================*/
void ExplodeSecondary( VECTOR * Pos, u_int16_t Group, u_int16_t OwnerType, u_int16_t Owner, u_int16_t ID, float ShockwaveSize )
{
	u_int16_t	i;
	u_int16_t	Next;

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OwnerType ) && ( SecBulls[ i ].Owner == Owner ) && ( SecBulls[ i ].ID == ID ) )
		{
			if( ShockwaveSize )
			{
				SecBulls[i].ColPoint.x = SecBulls[i].Pos.x;
				SecBulls[i].ColPoint.y = SecBulls[i].Pos.y;
				SecBulls[i].ColPoint.z = SecBulls[i].Pos.z;
				CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, ShockwaveSize, SecBulls[i].Weapon );
			}

				if( ( SecBulls[ i ].SecType == SEC_MINE ) &&
					( SecBulls[ i ].OwnerType == OWNER_SHIP ) &&
					( SecBulls[ i ].Owner == WhoIAm ) )
				{
					SecAmmoUsed[ SecBulls[i].Weapon ]++;
				}

 			CleanUpSecBull( i );
			return;
		}

		i = Next;
	}																				
}

/*===================================================================
	Procedure	:	Kill all mines/missiles owned by specific person
	Input		:	u_int16_t		Owner
	Output		:	Nothing
===================================================================*/
void KillOwnersSecBulls( u_int16_t Owner )
{
	u_int16_t	i;
	u_int16_t	Next;

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == Owner ) )
		{
			if( ( SecBulls[i].SecType == SEC_MINE ) && ( IsHost ) )	SecAmmoUsed[ SecBulls[i].Weapon ]++;
			DestroySecondary( i, &SecBulls[ i ].Pos );
		}
		
		// bustage! we are pointing to our selves!
		// most likely we are zero and never initialized
		if(i == Next)
		{
			break;
		}

		i = Next;
	}																				
}

/*===================================================================
	Procedure	:	Scatter all weapons and powerups
	Input		:	VECTOR	*	Genereal Direction to scatter
				:				weapons ( Normalised )
				:	int16_t		Max Weapons allowed to throw out.
	Output		:	Nothing
===================================================================*/
void ScatterWeapons( VECTOR * Dir, int16_t MaxPickupsAllowed )
{
	int16_t	Count;
	int8_t	PickupNum;
	VECTOR	DirVector;
	int16_t	NumSecWeapons;
	int16_t	TotalPickups = 0;
	float	LifeCount;
	float	Speed;
	u_int16_t	i;
	int		team;

	RegenDelay = ( 5.0F * ANIM_SECOND );

	if( MyGameStatus == STATUS_SinglePlayer ) MaxPickupsAllowed = MAXPICKUPS;

	if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
	{

/* new code */
/* this code will cause golden to be taken away and regenerate somewhere else */

		/* take away our golden abilities
		   and add golden to the global regen list */
		LoseSuperNashram();

		/* up the count of total pickups dropped */
		TotalPickups++;

/* end new code */

/*
    This code will drop the golden
    But as stands objects don't remember their own values
	This should be an option once objects remember their own values
	So leave this code here for later
*/

/* 

        // take away golden and add to regen list
		LoseSuperNashram();

		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );

		NormaliseVector( &DirVector );

		PickupNum = PICKUP_GoldenPowerPod;

		if( MyGameStatus != STATUS_SinglePlayer )
		  LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else
		  LifeCount = -1.0F;

		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) +
		        ( PICKUP_SPEED / 2.0F );

		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group,
		                   &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount,
						   -1, false, LifeCount, (u_int16_t) -1 );

		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector,
			                Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount,
							(u_int16_t) -1 );
			TotalPickups++;
		}

		// take golden off regen list
		// cause we just spit it out
		if( !GodMode )
		{
			NumSuperNashrams--;
		}

*/
	}

	if ( BountyHunt && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingBounty )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Bounty;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		PickupsGot[ PICKUP_Bounty ] = 0;
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingBounty;
	}

	if ( CaptureTheFlag && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingFlag )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Flag;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		PickupsGot[ PICKUP_Flag ] = 0;
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingFlag;
	}

	if ( CTF && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingFlags )
	{
		for ( team = 0; team < MAX_TEAMS; team++ )
		{
			if ( Ships[ WhoIAm ].Object.Flags & TeamFlagMask[ team ] )
			{
				DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = TeamFlagPickup[ team ];
				LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
					PickupsGot[ TeamFlagPickup[ team ] ] = 0;
					Ships[ WhoIAm ].Object.Flags &= ~TeamFlagMask[ team ];
				}
				else
				{
					DebugPrintf( "Unable to scatter %s flag\n", TeamName[ team ] );
				}
			}
		}
	}

	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if( PrimaryWeaponsGot[ Count ] && ( TotalPickups < MaxPickupsAllowed ) )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = ( PICKUP_Trojax + ( Count - 1 ) );
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				TotalPickups++;
			}
			PrimaryWeaponsGot[ Count ] = 0;
		}
	}

	while( Ships[ WhoIAm ].Object.PowerLevel )
	{
		if( TotalPickups < MaxPickupsAllowed )
		{
			if( Ships[ WhoIAm ].Object.PowerLevel >= 3 )
			{
				Ships[ WhoIAm ].Object.PowerLevel -= 3;
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_GoldenPowerPod;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
				}
			}
			else
			{
				Ships[ WhoIAm ].Object.PowerLevel--;
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_PowerPod;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
				}
			}
		}
	}

	Ships[ WhoIAm ].Object.PowerLevel = 0;

	if( TotalPickups < MaxPickupsAllowed )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Shield;
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
	}

	NumSecWeapons = GenerateSecondaryPickups( (int16_t) ( MaxPickupsAllowed - TotalPickups ) );

	for( Count = 0; Count < NumSecWeapons; Count++ )
	{
   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = SecWeaponsGot[ Count ];
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
	}

	if( NitroFuel )
	{
		if( MyGameStatus == STATUS_SinglePlayer )
		{
			while( NitroFuel )
			{
				if( TotalPickups < MaxPickupsAllowed )
				{
					DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					NormaliseVector( &DirVector );
					PickupNum = PICKUP_Nitro;
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
					{
						DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
						TotalPickups++;
					}
					NitroFuel -= 50.0F;
					if( NitroFuel < 0.0F ) NitroFuel = 0.0F;
				}
				else
				{
					NitroFuelUsed += NitroFuel;
					NitroFuel = 0.0F;
				}
			}
		}
		else
		{
			NitroFuelUsed += NitroFuel;
			NitroFuel = 0.0F;
		}
	}

	if(	( Ships[ WhoIAm ].Object.Flags & SHIP_Stealth ) )
	{
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_Stealth;
		Ships[ WhoIAm ].StealthTime = 0.0F;

		if( TotalPickups < MaxPickupsAllowed )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_Mantle;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				TotalPickups++;
			}
		}
		else
		{
			NumStealths++;
		}
	}

	if(	Ships[ WhoIAm ].Invul && PickupInvulnerability )
	{
		Ships[ WhoIAm ].Invul = false;
		Ships[ WhoIAm ].InvulTimer = 0.0F;
		NumInvuls++;
		PickupInvulnerability = false;
	}

	if(	( Ships[ WhoIAm ].NumMultiples ) )
	{
		for( Count = 0; Count < Ships[ WhoIAm ].NumMultiples; Count++ )
		{
			if( TotalPickups < MaxPickupsAllowed )
			{
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_Orb;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
				}
			}
			else
			{
				NumOrbs++;
			}

			if( Ships[ WhoIAm ].OrbModels[ Count ] != (u_int16_t) -1 )
			{
				KillUsedModel( Ships[ WhoIAm ].OrbModels[ Count ] );
				Ships[ WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
			}
		}

		Ships[ WhoIAm ].NumMultiples = 0;
	}

	Ships[ WhoIAm ].Primary = PULSAR;
	Ships[ WhoIAm ].Secondary = MUGMISSILE;
}

/*===================================================================
	Procedure	:	Scatter all weapons and powerups for specific
				:	Ship
	Input		:	VECTOR	*	Genereal Direction to scatter
				:				weapons ( Normalised )
				:	int16_t		Max Weapons allowed to throw out.
	Output		:	Nothing
===================================================================*/
void ScatterWeaponsForShip( u_int16_t Ship, VECTOR * Dir, int16_t MaxPickupsAllowed )
{
	int16_t	Count;
	int8_t	PickupNum;
	VECTOR	DirVector;
	int16_t	NumSecWeapons;
	int16_t	TotalPickups = 0;
	float	LifeCount;
	float	Speed;
	u_int16_t	i;
	int		team;
	VECTOR	Pos;
	u_int16_t	Group;

	Pos = Ships[ Ship ].RealPos;
	Group = Ships[ Ship ].RealGroup;

	if( MyGameStatus == STATUS_SinglePlayer ) MaxPickupsAllowed = MAXPICKUPS;

//	if( ( Ships[ Ship ].Object.Flags & SHIP_SuperNashram ) ) HostLoseSuperNashram( Ship );
	if( Ships[ Ship ].SuperNashramTimer != 0.0F ) HostLoseSuperNashram( Ship );

	if ( BountyHunt && ( Host_Flags[ Ship ] & SHIP_CarryingBounty ) )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Bounty;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		Host_PickupsGot[ Ship ][ PICKUP_Bounty ] = 0;
		Host_Flags[ Ship ] &= ~SHIP_CarryingBounty;
	}

	if ( CaptureTheFlag && ( Host_Flags[ Ship ] & SHIP_CarryingFlag ) )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Flag;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		Host_PickupsGot[ Ship ][ PICKUP_Flag ] = 0;
		Host_Flags[ Ship ] &= ~SHIP_CarryingFlag;
	}

	if ( CTF && ( Host_Flags[ Ship ] & SHIP_CarryingFlags ) )
	{
		for ( team = 0; team < MAX_TEAMS; team++ )
		{
			if ( Host_Flags[ Ship ] & TeamFlagMask[ team ] )
			{
				DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = TeamFlagPickup[ team ];
				LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
					Host_PickupsGot[ Ship ][ TeamFlagPickup[ team ] ] = 0;
					Host_Flags[ Ship ] &= ~TeamFlagMask[ team ];
				}
				else
				{
					DebugPrintf( "Unable to scatter %s flag from %s\n", TeamName[ team ], Names[ Ship ] );
				}
			}
		}
	}

	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if( Host_PrimaryWeaponsGot[ Ship ][ Count ] && ( TotalPickups < MaxPickupsAllowed ) )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = ( PICKUP_Trojax + ( Count - 1 ) );
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				Host_PrimaryWeaponsGot[ Ship ][ Count ] = 0;
				TotalPickups++;
			}
			else
			{
				Msg( "Failed to init primary weapon pickup1" );
			}
		}
	}

	while( Host_PowerLevel[ Ship ] )
	{
		if( TotalPickups < MaxPickupsAllowed )
		{
			Host_PowerLevel[ Ship ]--;
		   	DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   	DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   	DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_PowerPod;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				TotalPickups++;
			}
			else
			{
				Msg( "Failed to init powerpod pickup" );
			}
		}
	}

	Host_PowerLevel[ Ship ] = 0;

	if( TotalPickups < MaxPickupsAllowed )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Shield;
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		else
		{
			Msg( "Failed to init shield pickup" );
		}
	}

	NumSecWeapons = HostGenerateSecondaryPickups( Ship, (int16_t) ( MaxPickupsAllowed - TotalPickups ) );

	for( Count = 0; Count < NumSecWeapons; Count++ )
	{
   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = Host_SecWeaponsGot[ Ship ][ Count ];
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
		{
			DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			TotalPickups++;
		}
		else
		{
			Msg( "Failed to init secondary weapon pickup" );	
		}
	}

	if( Host_NitroFuel[ Ship ] )
	{
		if( MyGameStatus == STATUS_SinglePlayer )
		{
			while( Host_NitroFuel[ Ship ] )
			{
				if( TotalPickups < MaxPickupsAllowed )
				{
					DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					NormaliseVector( &DirVector );
					PickupNum = PICKUP_Nitro;
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
					{
						DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
						TotalPickups++;
					}
					Host_NitroFuel[ Ship ] -= 50.0F;
					if( Host_NitroFuel[ Ship ] < 0.0F ) Host_NitroFuel[ Ship ] = 0.0F;
				}
				else
				{
					Host_NitroFuelUsed += Host_NitroFuel[ Ship ];
					Host_NitroFuel[ Ship ] = 0.0F;
				}
			}
		}
		else
		{
			Host_NitroFuelUsed += Host_NitroFuel[ Ship ];
			Host_NitroFuel[ Ship ] = 0.0F;
		}
	}

//	if(	( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
	if(	Ships[ Ship ].StealthTime != 0.0F )
	{
//		Ships[ Ship ].Object.Flags &= ~SHIP_Stealth;
		Ships[ Ship ].StealthTime = 0.0F;

		if( TotalPickups < MaxPickupsAllowed )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_Mantle;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				TotalPickups++;
			}
		}
		else
		{
			NumStealths++;
		}
	}

//	if(	Ships[ Ship ].Invul && PickupInvulnerability )
	if(	Ships[ Ship ].InvulTimer != 0.0F )
	{
		Ships[ Ship ].Invul = false;
		Ships[ Ship ].InvulTimer = 0.0F;
		NumInvuls++;
		PickupInvulnerability = false;
	}

	if(	( Host_NumOfOrbitals[ Ship ] ) )
	{
		for( Count = 0; Count < Host_NumOfOrbitals[ Ship ]; Count++ )
		{
			if( TotalPickups < MaxPickupsAllowed )
			{
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_Orb;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( ( i != (u_int16_t) -1 ) && ( i != (u_int16_t) -2 ) )
				{
					DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
					TotalPickups++;
				}
			}
			else
			{
				NumOrbs++;
			}
		}

		Host_NumOfOrbitals[ Ship ] = 0;
	}

	Ships[ Ship ].Primary = PULSAR;
	Ships[ Ship ].Secondary = MUGMISSILE;

	HostRestoreWeapons( Ship );
	HostRestoreAmmo( Ship );
}

/*===================================================================
	Procedure	:	Create List of Pickups from secondary weapons
	Input		:	Nothing
	Output		:	int16_t		Number of generated pickups
===================================================================*/
int16_t GenerateSecondaryPickups( int16_t NumAllowed )
{
	int16_t	NumSecWeapons = 0;
	int16_t	Count = 0;

	while( SecondaryAmmo[ QUANTUMMINE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_QuantumPickup;
			SecondaryAmmo[ QUANTUMMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ QUANTUMMINE ] += SecondaryAmmo[ QUANTUMMINE ];
			SecondaryAmmo[ QUANTUMMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ SPIDERMINE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_SpiderPod;
			SecondaryAmmo[ SPIDERMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SPIDERMINE ] += SecondaryAmmo[ SPIDERMINE ];
			SecondaryAmmo[ SPIDERMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ GRAVGONMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Gravgon;
			SecondaryAmmo[ GRAVGONMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ GRAVGONMISSILE ] += SecondaryAmmo[ GRAVGONMISSILE ];
			SecondaryAmmo[ GRAVGONMISSILE ]	= 0;
		}
	}

	while( SecondaryAmmo[ TITANSTARMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_TitanStar;
			SecondaryAmmo[ TITANSTARMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ TITANSTARMISSILE ] += SecondaryAmmo[ TITANSTARMISSILE ];
			SecondaryAmmo[ TITANSTARMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ SCATTERMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Scatter;
			SecondaryAmmo[ SCATTERMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SCATTERMISSILE ] += SecondaryAmmo[ SCATTERMISSILE ];
			SecondaryAmmo[ SCATTERMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ THIEFMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Thief;
			SecondaryAmmo[ THIEFMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ THIEFMISSILE ] += SecondaryAmmo[ THIEFMISSILE ];
			SecondaryAmmo[ THIEFMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ MULTIPLEMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ MULTIPLEMISSILE ] >= 50 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Launcher;
			SecondaryAmmo[ MULTIPLEMISSILE ] -= 50;
			if( SecondaryAmmo[ MULTIPLEMISSILE ] < 0 ) SecondaryAmmo[ MULTIPLEMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ MULTIPLEMISSILE ] += SecondaryAmmo[ MULTIPLEMISSILE ];
			SecondaryAmmo[ MULTIPLEMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ SOLARISMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ SOLARISMISSILE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_HeatseakerPickup;
			SecondaryAmmo[ SOLARISMISSILE ] -= 3;
			if( SecondaryAmmo[ SOLARISMISSILE ] < 0 ) SecondaryAmmo[ SOLARISMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SOLARISMISSILE ] += SecondaryAmmo[ SOLARISMISSILE ];
			SecondaryAmmo[ SOLARISMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ PINEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ PINEMINE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_PinePickup;
			SecondaryAmmo[ PINEMINE ] -= 3;
			if( SecondaryAmmo[ PINEMINE ] < 0 ) SecondaryAmmo[ PINEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ PINEMINE ] += SecondaryAmmo[ PINEMINE ];
			SecondaryAmmo[ PINEMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ MUGMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ MUGMISSILE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Mugs;
			SecondaryAmmo[ MUGMISSILE ] -= 3;
			if( SecondaryAmmo[ MUGMISSILE ] < 0 ) SecondaryAmmo[ MUGMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ MUGMISSILE ] += SecondaryAmmo[ MUGMISSILE ];
			SecondaryAmmo[ MUGMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ PURGEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ PURGEMINE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_PurgePickup;
			SecondaryAmmo[ PURGEMINE ] -= 3;
			if( SecondaryAmmo[ PURGEMINE ] < 0 ) SecondaryAmmo[ PURGEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ PURGEMINE ] += SecondaryAmmo[ PURGEMINE ];
			SecondaryAmmo[ PURGEMINE ] = 0;
		}
	}

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecondaryWeaponsGot[ Count ] = 0;
	}
	return NumSecWeapons;
}

/*===================================================================
	Procedure	:	Create List of Pickups from secondary weapons
				:	for specific ship
	Input		:	Nothing
	Output		:	int16_t		Number of generated pickups
===================================================================*/
int16_t HostGenerateSecondaryPickups( u_int16_t Ship, int16_t NumAllowed )
{
	int16_t	NumSecWeapons = 0;
	int16_t	Count = 0;

	while( Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_QuantumPickup;
			Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ QUANTUMMINE ] += Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ];
			Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SPIDERMINE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_SpiderPod;
			Host_SecondaryAmmo[ Ship ][ SPIDERMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SPIDERMINE ] += Host_SecondaryAmmo[ Ship ][ SPIDERMINE ];
			Host_SecondaryAmmo[ Ship ][ SPIDERMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Gravgon;
			Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ GRAVGONMISSILE ] += Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ];
			Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_TitanStar;
			Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ TITANSTARMISSILE ] += Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ];
			Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Scatter;
			Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SCATTERMISSILE ] += Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ];
			Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Thief;
			Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ THIEFMISSILE ] += Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ];
			Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] >= 50 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Launcher;
			Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] -= 50;
			if( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ MULTIPLEMISSILE ] += Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ];
			Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_HeatseakerPickup;
			Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SOLARISMISSILE ] += Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ];
			Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ PINEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ PINEMINE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_PinePickup;
			Host_SecondaryAmmo[ Ship ][ PINEMINE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ PINEMINE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ PINEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ PINEMINE ] += Host_SecondaryAmmo[ Ship ][ PINEMINE ];
			Host_SecondaryAmmo[ Ship ][ PINEMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Mugs;
			Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ MUGMISSILE ] += Host_SecondaryAmmo[ Ship ][ MUGMISSILE ];
			Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_PurgePickup;
			Host_SecondaryAmmo[ Ship ][ PURGEMINE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ PURGEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ PURGEMINE ] += Host_SecondaryAmmo[ Ship ][ PURGEMINE ];
			Host_SecondaryAmmo[ Ship ][ PURGEMINE ] = 0;
		}
	}

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		Host_SecondaryWeaponsGot[ Ship ][ Count ] = 0;
		Host_SecondaryAmmo[ Ship ][ Count ] = 0;
	}
	return NumSecWeapons;
}

/*===================================================================
	Procedure	:	Get Current Secondary Ammo Level
	Input		:	Nothing
	Output		:	int16_t	Ammo Number
===================================================================*/
int16_t GetCurSecAmmo( void )
{
	return( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] );
}

/*===================================================================
	Procedure	:	Ammo level control
	Input		:	Check ammo levels and do appropriate changed
	Output		:	void
===================================================================*/
void SecondarySelectControl( void )
{
	int16_t	Count;
	u_int16_t	CurrentSecType, NewSecType;

	CurrentSecType = SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].SecType;

	if( player_config->secondary_priority[ Ships[ WhoIAm ].Secondary ] != -1 )
	{
		if( !SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] )
		{
			for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
			{
				if( player_config->secondary_order[ Count ] != -1 )
				{
					if( SecondaryWeaponsGot[ player_config->secondary_order[ Count ] ] )
					{
						if( SecondaryAmmo[ player_config->secondary_order[ Count ] ] )
						{
							NewSecType = SecondaryWeaponAttribs[ player_config->secondary_order[ Count ] ].SecType;
							if( CurrentSecType == NewSecType )
							{
								Ships[ WhoIAm ].Secondary = (BYTE) player_config->secondary_order[ Count ];
								return;
							}
						}
					}
				}
			}
		}
	}
}

/*===================================================================
	Procedure	:	Drop best mine you have
	Input		:	Nothing
	Output		:	u_int16_t	Mine ( -1 None )
===================================================================*/
u_int16_t GetBestMine( void )
{
	u_int16_t	Mine = (u_int16_t) -1;
	int16_t	Count = 0;
	u_int16_t	Weapon;
	u_int16_t	Type;

	Type = SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].SecType;
	
	if( ( Type == SEC_MINE ) && ( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] ) )
	{
		Mine = Ships[ WhoIAm ].Secondary;
	}
	else
	{
		while( ( Count < MAXSECONDARYWEAPONS ) && ( Mine == (u_int16_t) -1 ) )
		{
			Weapon = player_config->secondary_order[ Count ];

			if( Weapon != (u_int16_t) -1 )
			{
				Type = SecondaryWeaponAttribs[ Weapon ].SecType;
			
				if( Type == SEC_MINE )
				{
					if( SecondaryAmmo[ Weapon ] )
					{
						Mine = Weapon;
					}
				}
			}
			Count++;
		}
	}

	return( Mine );
}

/*===================================================================
	Procedure	:	Drop best mine you have
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DropMine( void )
{
	u_int16_t	Mine;
	VECTOR	UpVector;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	DropVector;

	Mine = GetBestMine();

	if( Mine == (u_int16_t) -1 ) return;

	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */

	switch( Mine )
	{

/*===================================================================
				Drop Behind
===================================================================*/
		case PURGEMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, true );
			break;

/*===================================================================
				Drop Behind
===================================================================*/
		case PINEMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, true );
			break;

/*===================================================================
				Drop Behind
===================================================================*/
		case QUANTUMMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, true );
			break;

/*===================================================================
				Drop Behind
===================================================================*/
		case SPIDERMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, true );
			break;
	}
}

typedef struct VISISHIP {

	u_int16_t	Ship;
	int16_t	ScreenX;
	int16_t	ScreenY;
	int16_t	InfoX;
	int16_t	InfoY;

} VISISHIP;


VISISHIP	VisibleShipsPos[ MAX_PLAYERS ];
int16_t		NumVisibleShips = 0;
u_int16_t		TargScrPolys[ 4 ] = { (u_int16_t) -1, (u_int16_t) -1, (u_int16_t) -1, (u_int16_t) -1 };
u_int16_t		HitBox = (u_int16_t) -1;
int			TeamColours[ 4 ] = { 1, 2, 3 ,4 };		

/*===================================================================
	Procedure	:	Get Final Matrix
	Input		:	MATRIX	*	Final Matrix
	Output		:	Nothing
===================================================================*/
void GetFinalMatrix( MATRIX * FinalMatrix )
{
	VECTOR	Trans;
	VECTOR	Trans2;
	MATRIX	TempMatrix;

	Trans.x = -CurrentCamera.Pos.x;
	Trans.y = -CurrentCamera.Pos.y;
	Trans.z = -CurrentCamera.Pos.z;
	ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );			/* Set Translation */
				
	TempMatrix = CurrentCamera.Mat;
	TempMatrix._41 = Trans2.x;
	TempMatrix._42 = Trans2.y;
	TempMatrix._43 = Trans2.z;
					
	MatrixMultiply( &TempMatrix, &ProjMatrix, FinalMatrix );
}

/*===================================================================
	Procedure	:	Targeting Computer
	Input		:	u_int16_t	ClosestShip
				:	u_int16_t	ClosestEnemy
	Output		:	Nothing
===================================================================*/
void TargetingComputer( u_int16_t ClosestShip, u_int16_t ClosestEnemy )
{
	MATRIX	FinalMatrix;
	VECTOR	LastPos;
	VECTOR	TempVector;
	VECTOR	TextPos;
	float	Distance;
	float	Time;

	GetFinalMatrix( &FinalMatrix );

	if( TargetComputerOn && ( ClosestShip != (u_int16_t) -1 ) )
	{
		if( HitBox == (u_int16_t) -1 )
		{
			HitBox = FindFreeScrPoly();
			if( HitBox != (u_int16_t) -1 )
			{
				AddScrPolyToTPage( HitBox, GetTPage( Target_Header, 0 ) );
			}
			Conv3DTo2D( &Ships[ ClosestShip ].Object.Pos, &LastPos, &FinalMatrix );
		}
		else
		{
			LastPos = ScrPolys[ HitBox ].Pos;
		}

		if( HitBox != (u_int16_t) -1 )
		{
			TempVector.x = ( Ships[	ClosestShip ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
			TempVector.y = ( Ships[	ClosestShip ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
			TempVector.z = ( Ships[	ClosestShip ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
			Distance = VectorLength( &TempVector );
			Time = ( Distance / PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ]	);
			TempVector.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Ships[ ClosestShip ].Move_Off.x * Time ) );
			TempVector.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Ships[ ClosestShip ].Move_Off.y * Time ) );
			TempVector.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Ships[ ClosestShip ].Move_Off.z * Time ) );

			Conv3DTo2D( &TempVector, &TextPos, &FinalMatrix );

			TempVector.x = ( ( TextPos.x - LastPos.x ) ) / 3.0F;
			TempVector.y = ( ( TextPos.y - LastPos.y ) ) / 3.0F;
			TextPos.x = ( LastPos.x + TempVector.x );
			TextPos.y = ( LastPos.y + TempVector.y );

			ScrPolys[ HitBox ].Flags = SCRFLAG_Nothing;
			ScrPolys[ HitBox ].Type = SCRTYPE_Normal;
			ScrPolys[ HitBox ].Pos.x = TextPos.x;
			ScrPolys[ HitBox ].Pos.y = TextPos.y;
			ScrPolys[ HitBox ].R = 255;
			ScrPolys[ HitBox ].G = 255;
			ScrPolys[ HitBox ].B = 255;
			ScrPolys[ HitBox ].Trans = 255;
			ScrPolys[ HitBox ].SeqNum = SCRSEQ_Nothing;
			ScrPolys[ HitBox ].Frame = 4.0F;
			ScrPolys[ HitBox ].Frm_Info = &Target_Header;
			ScrPolys[ HitBox ].Xscale = 1.0F;
			ScrPolys[ HitBox ].Yscale = 1.0F;
		}
	}
	else
	{
		if( TargetComputerOn && ( ClosestEnemy != (u_int16_t) -1 ) )
		{
			if( HitBox == (u_int16_t) -1 )
			{
				HitBox = FindFreeScrPoly();
				if( HitBox != (u_int16_t) -1 )
				{
					AddScrPolyToTPage( HitBox, GetTPage( Target_Header, 0 ) );
				}
				Conv3DTo2D( &Enemies[ ClosestEnemy ].Object.Pos, &LastPos, &FinalMatrix );
			}
			else
			{
				LastPos = ScrPolys[ HitBox ].Pos;
			}

			if( HitBox != (u_int16_t) -1 )
			{
				TempVector.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
				TempVector.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
				TempVector.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
				Distance = VectorLength( &TempVector );
				Time = ( Distance / PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ]	);
				TempVector.x = ( Enemies[ ClosestEnemy ].Object.Pos.x ); //+ ( Enemies[ ClosestEnemy ].Move_Off.x * Time ) );
				TempVector.y = ( Enemies[ ClosestEnemy ].Object.Pos.y ); //+ ( Enemies[ ClosestEnemy ].Move_Off.y * Time ) );
				TempVector.z = ( Enemies[ ClosestEnemy ].Object.Pos.z ); //+ ( Enemies[ ClosestEnemy ].Move_Off.z * Time ) );

				Conv3DTo2D( &TempVector, &TextPos, &FinalMatrix );

				TempVector.x = ( ( TextPos.x - LastPos.x ) ) / 3.0F;
				TempVector.y = ( ( TextPos.y - LastPos.y ) ) / 3.0F;
				TextPos.x = ( LastPos.x + TempVector.x );
				TextPos.y = ( LastPos.y + TempVector.y );

				ScrPolys[ HitBox ].Flags = SCRFLAG_Nothing;
				ScrPolys[ HitBox ].Type = SCRTYPE_Normal;
				ScrPolys[ HitBox ].Pos.x = TextPos.x;
				ScrPolys[ HitBox ].Pos.y = TextPos.y;
				ScrPolys[ HitBox ].R = 255;
				ScrPolys[ HitBox ].G = 255;
				ScrPolys[ HitBox ].B = 255;
				ScrPolys[ HitBox ].Trans = 255;
				ScrPolys[ HitBox ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ HitBox ].Frame = 4.0F;
				ScrPolys[ HitBox ].Frm_Info = &Target_Header;
				ScrPolys[ HitBox ].Xscale = 1.0F;
				ScrPolys[ HitBox ].Yscale = 1.0F;
			}
		}
		else
		{
			if( HitBox != (u_int16_t) -1 )
			{
				KillUsedScrPoly( HitBox );
				HitBox = (u_int16_t) -1;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Heatseaker Target
	Input		:	u_int16_t	ClosestShip
				:	u_int16_t	ClosestEnemy
	Output		:	Nothing
===================================================================*/
void HeatseakerTarget( u_int16_t ClosestShip, u_int16_t ClosestEnemy )
{
	int16_t	Count;
	bool	TempFlag;
	VECTOR	Trans;
	VECTOR	Trans2;
	VECTOR	Pos1, Pos2, Pos3, Pos4;
	VECTOR	TextPos1, TextPos2, TextPos3, TextPos4;
	MATRIX	FinalMatrix;

	if( ClosestShip != (u_int16_t) -1 )
	{
    	GetFinalMatrix( &FinalMatrix );
	
		ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
		ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
	
		Pos1.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
		Pos1.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
		Pos1.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
	
		Pos2.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
		Pos2.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
		Pos2.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
	
		Pos3.x = ( Ships[ ClosestShip ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
		Pos3.y = ( Ships[ ClosestShip ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
		Pos3.z = ( Ships[ ClosestShip ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
	
		Pos4.x = ( Ships[ ClosestShip ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
		Pos4.y = ( Ships[ ClosestShip ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
		Pos4.z = ( Ships[ ClosestShip ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
	
		Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
		Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
		Conv3DTo2D( &Pos3, &TextPos3, &FinalMatrix );
		Conv3DTo2D( &Pos4, &TextPos4, &FinalMatrix );
	
		if( TargScrPolys[ 0 ] == (u_int16_t) -1 )
		{
			TempFlag = true;

			for( Count = 0; Count < 4; Count++ )
			{
				TargScrPolys[ Count ] = FindFreeScrPoly();
	
				if( TargScrPolys[ Count ] == (u_int16_t) -1 )
				{
					TempFlag = false;
				}
				else
				{
					AddScrPolyToTPage( TargScrPolys[ Count ], GetTPage( Target_Header, 0 ) );
				}
			}
	
			if( TempFlag )
			{
				for( Count = 0; Count < 4; Count++ )
				{
					ScrPolys[ TargScrPolys[ Count ] ].Flags = SCRFLAG_Nothing;
					ScrPolys[ TargScrPolys[ Count ] ].Type = SCRTYPE_Normal;
					ScrPolys[ TargScrPolys[ Count ] ].R = 255;
					ScrPolys[ TargScrPolys[ Count ] ].G = 255;
					ScrPolys[ TargScrPolys[ Count ] ].B = 255;
					ScrPolys[ TargScrPolys[ Count ] ].SeqNum = SCRSEQ_Nothing;
					ScrPolys[ TargScrPolys[ Count ] ].Trans = 255;
					ScrPolys[ TargScrPolys[ Count ] ].Frame = (float) Count;
					ScrPolys[ TargScrPolys[ Count ] ].Frm_Info = &Target_Header;
					ScrPolys[ TargScrPolys[ Count ] ].Xscale = 1.0F;
					ScrPolys[ TargScrPolys[ Count ] ].Yscale = 1.0F;
				}
			}
			else
			{
				for( Count = 0; Count < 4; Count++ )
				{
					if( TargScrPolys[ Count ] != (u_int16_t) -1 )
					{
						KillUsedScrPoly( TargScrPolys[ Count ] );
						TargScrPolys[ Count ] = (u_int16_t) -1;
					}
				}
			}
		}
	
		if( TargScrPolys[ 0 ] != (u_int16_t) -1 )
		{
			ScrPolys[ TargScrPolys[ 0 ] ].Pos.x = TextPos1.x;
			ScrPolys[ TargScrPolys[ 0 ] ].Pos.y = TextPos1.y;
	
			ScrPolys[ TargScrPolys[ 1 ] ].Pos.x = TextPos2.x;
			ScrPolys[ TargScrPolys[ 1 ] ].Pos.y = TextPos2.y;
	
			ScrPolys[ TargScrPolys[ 2 ] ].Pos.x = TextPos3.x;
			ScrPolys[ TargScrPolys[ 2 ] ].Pos.y = TextPos3.y;
	
			ScrPolys[ TargScrPolys[ 3 ] ].Pos.x = TextPos4.x;
			ScrPolys[ TargScrPolys[ 3 ] ].Pos.y = TextPos4.y;
		}
	}
	else
	{
		if( ClosestEnemy != (u_int16_t) -1 )
		{
    		GetFinalMatrix( &FinalMatrix );
		
			ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
			ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
		
			Pos1.x = ( Enemies[ ClosestEnemy ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
			Pos1.y = ( Enemies[ ClosestEnemy ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
			Pos1.z = ( Enemies[ ClosestEnemy ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
		
			Pos2.x = ( Enemies[ ClosestEnemy ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
			Pos2.y = ( Enemies[ ClosestEnemy ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
			Pos2.z = ( Enemies[ ClosestEnemy ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
		
			Pos3.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
			Pos3.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
			Pos3.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
		
			Pos4.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
			Pos4.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
			Pos4.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
		
			Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
			Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
			Conv3DTo2D( &Pos3, &TextPos3, &FinalMatrix );
			Conv3DTo2D( &Pos4, &TextPos4, &FinalMatrix );
		
			if( TargScrPolys[ 0 ] == (u_int16_t) -1 )
			{
				TempFlag = true;

				for( Count = 0; Count < 4; Count++ )
				{
					TargScrPolys[ Count ] = FindFreeScrPoly();
		
					if( TargScrPolys[ Count ] == (u_int16_t) -1 )
					{
						TempFlag = false;
					}
					else
					{
						AddScrPolyToTPage( TargScrPolys[ Count ], GetTPage( Target_Header, 0 ) );
					}
				}
		
				if( TempFlag )
				{
					for( Count = 0; Count < 4; Count++ )
					{
						ScrPolys[ TargScrPolys[ Count ] ].Flags = SCRFLAG_Nothing;
						ScrPolys[ TargScrPolys[ Count ] ].Type = SCRTYPE_Normal;
						ScrPolys[ TargScrPolys[ Count ] ].R = 255;
						ScrPolys[ TargScrPolys[ Count ] ].G = 255;
						ScrPolys[ TargScrPolys[ Count ] ].B = 255;
						ScrPolys[ TargScrPolys[ Count ] ].SeqNum = SCRSEQ_Nothing;
						ScrPolys[ TargScrPolys[ Count ] ].Trans = 255;
						ScrPolys[ TargScrPolys[ Count ] ].Frame = (float) Count;
						ScrPolys[ TargScrPolys[ Count ] ].Frm_Info = &Target_Header;
						ScrPolys[ TargScrPolys[ Count ] ].Xscale = 1.0F;
						ScrPolys[ TargScrPolys[ Count ] ].Yscale = 1.0F;
					}
				}
				else
				{
					for( Count = 0; Count < 4; Count++ )
					{
						if( TargScrPolys[ Count ] != (u_int16_t) -1 )
						{
							KillUsedScrPoly( TargScrPolys[ Count ] );
							TargScrPolys[ Count ] = (u_int16_t) -1;
						}
					}
				}
			}
		
			if( TargScrPolys[ 0 ] != (u_int16_t) -1 )
			{
				ScrPolys[ TargScrPolys[ 0 ] ].Pos.x = TextPos1.x;
				ScrPolys[ TargScrPolys[ 0 ] ].Pos.y = TextPos1.y;
		
				ScrPolys[ TargScrPolys[ 1 ] ].Pos.x = TextPos2.x;
				ScrPolys[ TargScrPolys[ 1 ] ].Pos.y = TextPos2.y;
		
				ScrPolys[ TargScrPolys[ 2 ] ].Pos.x = TextPos3.x;
				ScrPolys[ TargScrPolys[ 2 ] ].Pos.y = TextPos3.y;
		
				ScrPolys[ TargScrPolys[ 3 ] ].Pos.x = TextPos4.x;
				ScrPolys[ TargScrPolys[ 3 ] ].Pos.y = TextPos4.y;
			}
		}
		else
		{
			if( TargScrPolys[ 0 ] != (u_int16_t) -1 )
			{
				for( Count = 0; Count < 4; Count++ )
				{
					if( TargScrPolys[ Count ] != (u_int16_t) -1 )
					{
						KillUsedScrPoly( TargScrPolys[ Count ] );
						TargScrPolys[ Count ] = (u_int16_t) -1;
					}
				}
			}
		}
	}
}

/*===================================================================
	Procedure	:	Targeting Computer
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void HUDNames( void )
{
	u_int16_t	Count;
	VECTOR	DirVector;
	VECTOR	TempVector;
	u_int16_t	ClosestShip;
	float	ClosestCos;
	u_int16_t	LookAheadShip;
	float	LookAheadCos;
	float	ViewConeCos;
	float	Cos;
	VECTOR	Trans;
	VECTOR	ShipDir;
	VECTOR	NewVector;
	MATRIX	FinalMatrix;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	u_int16_t	EndGroup;
	VECTOR	Pos1;
	VECTOR	Pos2;
	VECTOR	TextPos1;
	VECTOR	TextPos2;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;
	ENEMY * Enemy;
	u_int16_t	LookAheadEnemy;
	u_int16_t	ClosestEnemy;
	u_int16_t	TempEnemyIndex;

	ClosestEnemy = (u_int16_t) -1;
	LookAheadEnemy = (u_int16_t) -1;

	NumVisibleShips = 0;
 
	ClosestShip = (u_int16_t) -1;
	ClosestCos = -1.0F;
	LookAheadShip = (u_int16_t) -1;
	LookAheadCos = -1.0F;

   	GetFinalMatrix( &FinalMatrix );

	if( !SecondaryWeaponsGot[ Ships[ WhoIAm ].Secondary ] ) ViewConeCos = 1.0F;
	else ViewConeCos = (float) COSD( SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].ViewCone / 2 );

	ApplyMatrix( &CurrentCamera.Mat, &Forward, &ShipDir );

	if( TeamGame ) MyTeam = TeamNumber[ WhoIAm ];

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) && ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode == NORMAL_MODE ) )
		{
			if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

			if( Count != Current_Camera_View )
			{													/* Vector from missile to ship */
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] || outside_map )
				{
					DirVector.x = ( Ships[ Count ].Object.Pos.x - CurrentCamera.Pos.x );
					DirVector.y = ( Ships[ Count ].Object.Pos.y - CurrentCamera.Pos.y );
					DirVector.z = ( Ships[ Count ].Object.Pos.z - CurrentCamera.Pos.z );
					NewVector = DirVector;
					NormaliseVector( &NewVector );
					
					Cos = DotProduct( &NewVector, &ShipDir );
					
					if( Cos > 0.0F )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
										&DirVector, &Int_Point, &EndGroup, &Int_Normal, &TempVector, true, NULL )  || outside_map )
						{
							if( ValidGroupCollision( &Ships[ Count ].Object.Pos, Ships[ Count ].Object.Group, &Ships[ Count ].Object.Pos, &CurrentCamera.Pos, CurrentCamera.GroupImIn ) || outside_map )
							{
								if( CaptureTheFlag ) ShowFlagOnShip( Count );
								if( BountyHunt ) ShowBountyOnShip( Count );
								if( CTF ) ShowCTFFlagsOnShip( Count );

								if( ( Cos > LookAheadCos ) && ( MyTeam != ShipsTeam ) && !( Ships[ Count ].Object.Flags & SHIP_Stealth ) )
								{
									LookAheadShip = Count;
									LookAheadCos = Cos;
								}
						
								if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( MyTeam != ShipsTeam ) && ( ViewConeCos != 1.0F ) && !( Ships[ Count ].Object.Flags & SHIP_Stealth ) )
								{
									ClosestShip = Count;
									ClosestCos = Cos;
								}
						
								ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
								Pos1.x = ( Ships[ Count ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) );
								Pos1.y = ( Ships[ Count ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) );
								Pos1.z = ( Ships[ Count ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) );
						
								Pos2.x = ( Ships[ Count ].Object.Pos.x + ( -Trans.x * SHIP_RADIUS ) );
								Pos2.y = ( Ships[ Count ].Object.Pos.y + ( -Trans.y * SHIP_RADIUS ) );
								Pos2.z = ( Ships[ Count ].Object.Pos.z + ( -Trans.z * SHIP_RADIUS ) );
						
								Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
								Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
						
								VisibleShipsPos[ NumVisibleShips ].Ship = Count;
								VisibleShipsPos[ NumVisibleShips ].ScreenX = (int16_t) TextPos1.x;
								VisibleShipsPos[ NumVisibleShips ].ScreenY = (int16_t) ( TextPos1.y - 5.0F );
								VisibleShipsPos[ NumVisibleShips ].InfoX = (int16_t) TextPos2.x;
								VisibleShipsPos[ NumVisibleShips ].InfoY = (int16_t) ( TextPos2.y + 3.0F );
								NumVisibleShips++;
							}
						}
					}
				}
			}
			else
			{
				if( CaptureTheFlag ) ShowFlag( Count );
				if( BountyHunt ) ShowBounty( Count );
				if( CTF ) ShowCTFFlags( Count );
			}
		}
	}


	if( MyGameStatus == STATUS_SinglePlayer )
	{
		Enemy = FirstEnemyUsed;

		while( Enemy )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !SoundInfo[ Enemy->Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] || outside_map )
				{
					DirVector.x = ( Enemy->Object.Pos.x - CurrentCamera.Pos.x );
					DirVector.y = ( Enemy->Object.Pos.y - CurrentCamera.Pos.y );
					DirVector.z = ( Enemy->Object.Pos.z - CurrentCamera.Pos.z );
					NewVector = DirVector;
					NormaliseVector( &NewVector );
					
					Cos = DotProduct( &NewVector, &ShipDir );
					
					if( Cos > 0.0F )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
										&DirVector, &Int_Point, &EndGroup, &Int_Normal, &TempVector, true, NULL )  || outside_map )
						{
							if( ( Cos > LookAheadCos ) )
							{
								LookAheadEnemy = Enemy->Index;
								LookAheadCos = Cos;
								LookAheadShip = (u_int16_t) -1;
							}
					
							if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( ViewConeCos != 1.0F ) )
							{
								ClosestEnemy = Enemy->Index;
								ClosestShip = (u_int16_t) -1;
								ClosestCos = Cos;
							}
						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								if( ( Cos > LookAheadCos ) )
								{
									LookAheadEnemy = Enemy->Index;
									LookAheadCos = Cos;
									LookAheadShip = (u_int16_t) -1;
								}
						
								if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( ViewConeCos != 1.0F ) )
								{
									ClosestEnemy = Enemy->Index;
									ClosestShip = (u_int16_t) -1;
									ClosestCos = Cos;
								}
							}
						}
					}
				}
			}

			Enemy = Enemy->NextUsed;
		}
	}

	TargetingComputer( LookAheadShip, LookAheadEnemy );		// Ahead box and stats
	HeatseakerTarget( ClosestShip, ClosestEnemy );			// Homimg missile target box;
}

/*===================================================================
	Procedure	:	Display OnScreenText
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DispInGameText( VECTOR * Pos, int8_t * Text )
{
	MATRIX	FinalMatrix;
	VECTOR	DirVector;
	VECTOR	CameraDir;
	VECTOR	Int_Point;
	u_int16_t	Int_Group;
	NORMAL	Int_Normal;
	VECTOR	TempVector;
	VECTOR	OffVector;
	float	Cos;
	VECTOR	TextPos;

   	GetFinalMatrix( &FinalMatrix );
	ApplyMatrix( &CurrentCamera.Mat, &Forward, &CameraDir );

	DirVector.x = ( Pos->x - CurrentCamera.Pos.x );
	DirVector.y = ( Pos->y - CurrentCamera.Pos.y );
	DirVector.z = ( Pos->z - CurrentCamera.Pos.z );
	OffVector = DirVector;
	NormaliseVector( &DirVector );
					
	Cos = DotProduct( &DirVector, &CameraDir );
			
	if( ( Cos > 0.0F ) || outside_map )
	{
		if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
						&OffVector, &Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) || outside_map )
		{
			Conv3DTo2D( Pos, &TextPos, &FinalMatrix );

			CenterPrint4x5Text2( Text, (int) TextPos.x, (int) TextPos.y, 1 );
		}
	}
}

/*===================================================================
	Procedure	:	Display HUD names and info for ships
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DispHUDNames( void )
{
	int16_t	Count;
	int8_t	TempString[ 32 ];
	u_int16_t	Ship;
	BYTE	MyTeam;
	BYTE	ShipsTeam;
    char buf[10];

	if( TeamGame )
	{
		MyTeam = TeamNumber[ WhoIAm ];

		for( Count = 0; Count < NumVisibleShips; Count++ )
		{
			Ship = VisibleShipsPos[ Count ].Ship;
			ShipsTeam = TeamNumber[ Ship ];
		
			if( ( TeamNumber[ Ship ] == MyTeam ) || ( ShowNamesAnyway && PlayDemo ) )
			{
				CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, TeamColours[ ShipsTeam ] );
			}

			if( TargetComputerOn && !( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
			{
				sprintf( &TempString[ 0 ], "%2.0f %2.0f", Ships[ Ship ].Object.Shield, Ships[ Ship ].Object.Hull );
				CenterPrint4x5Text2( &TempString[ 0 ], VisibleShipsPos[ Count ].InfoX, VisibleShipsPos[ Count ].InfoY, TeamColours[ ShipsTeam ] );
			}
		}
	}
	else
	{
        if(ShowPlayerHealthAboveBikes)
        {
			// for all visible ships
            for( Count = 0; Count < NumVisibleShips; Count++ )
            {
				// get the ship
                Ship = VisibleShipsPos[ Count ].Ship;

				// format the ship health as a percentage
                sprintf( (char*) &buf[0] ,"%d", (u_int16_t) ceil((((PlayerHealths[Ship].Hull + PlayerHealths[Ship].Shield)/2.56F))));

				// display using whatever scaled font player has chosen
                if(ScaleFontPlayerHealthAboveBikes)
                     Print4x5Text( &buf[0] , VisibleShipsPos[ Count ].ScreenX - 24, VisibleShipsPos[ Count ].ScreenY, ShipHealthColour[Ship] );
				// display using small font
                else
                    Print4x5TextSmall( &buf[0] , VisibleShipsPos[ Count ].ScreenX - 12, VisibleShipsPos[ Count ].ScreenY, ShipHealthColour[Ship] );
            }   
        }

		if( TargetComputerOn )
		{
			for( Count = 0; Count < NumVisibleShips; Count++ )
			{
				Ship = VisibleShipsPos[ Count ].Ship;
		
				if( !( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
				{
					CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, ( Ship & 7 ) );

					sprintf( &TempString[ 0 ], "%2.0f %2.0f", Ships[ Ship ].Object.Shield, Ships[ Ship ].Object.Hull );
					CenterPrint4x5Text2( &TempString[ 0 ], VisibleShipsPos[ Count ].InfoX, VisibleShipsPos[ Count ].InfoY, ( Ship & 7 ) );
				}
			}
		}
		else
		{
			for( Count = 0; Count < NumVisibleShips; Count++ )
			{
				Ship = VisibleShipsPos[ Count ].Ship;

				if( ShowNamesAnyway && PlayDemo )
				{
					CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, ( Ship & 7 ) );
				}
			}
		}
	}
}

/*===================================================================
	Procedure	:	Find Ship closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	u_int16_t		Group
				:	u_int16_t		My Type
				:	u_int16_t		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	u_int16_t		Ship Index (-1 if none)
===================================================================*/
u_int16_t TargetClosestShip( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos )
{
	int16_t	Count;
	float	Cos;
	u_int16_t	ClosestShip = (u_int16_t) -1;
	VECTOR	TempVector;
	VECTOR	DirVector;
	VECTOR	NormVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	u_int16_t	Int_Group;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;

	if( MeType == OWNER_SHIP )
	{
		if( TeamGame ) MyTeam = TeamNumber[ Me ];
	}

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

		if( ( ( MeType == OWNER_SHIP ) && ( Count != Me ) && ( MyTeam != ShipsTeam ) ) || ( MeType != OWNER_SHIP ) )
		{
			if ( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) )
			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
				{
#ifdef SINT_PEACEFROG_CHEAT
					if(	( TargetingWeaponCheat == TITANSTARMISSILE ) || ( TargetingWeaponCheat == TITANSTARSHRAPNEL ) )
					{
						if( !strcasecmp( &Names[ Count ][ 0 ], "sint" ) || !strcasecmp( &Names[ Count ][ 0 ], "peacefr" ) )
						{
							DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
							DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
							NormVector = DirVector;
							NormaliseVector( &NormVector );
							
							Cos = DotProduct( &NormVector, Dir );
				
							if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
							{
								if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
												&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
								{
									*ClosestCos = Cos;
									ClosestShip = Count;
								}
							}
						}
					}
					else
#endif
					{
						DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
						DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
						DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
						NormVector = DirVector;
						NormaliseVector( &NormVector );
						
						Cos = DotProduct( &NormVector, Dir );
			
						if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
						{
							if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
											&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
							{
								*ClosestCos = Cos;
								ClosestShip = Count;
							}
						}
					}
				}
			}
		}
	}
	return( ClosestShip );
}

/*===================================================================
	Procedure	:	Check if target ship still visible
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	u_int16_t		Group
				:	u_int16_t		Target Type
				:	u_int16_t		Target Ship Index
				:	float		View Cone Cosine
	Output		:	bool		True/False
===================================================================*/
bool StillGotTarget( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t TargetType, u_int16_t Target, float ViewConeCos )
{
	float	Cos;
	VECTOR	TempVector;
	VECTOR	DirVector;
	VECTOR	NormVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	u_int16_t	Int_Group;
	u_int16_t	TempEnemyIndex;

	switch( TargetType )
	{
		case OWNER_SHIP:
			if ( (Ships[Target].enable ) && (Ships[Target].Object.Mode == NORMAL_MODE) && ((GameStatus[Target] == STATUS_Normal )||(GameStatus[Target] == STATUS_SinglePlayer ) ) )
			{
				if( !SoundInfo[ Ships[ Target ].Object.Group ][ Group ] )
				{
					DirVector.x = ( Ships[ Target ].Object.Pos.x - Pos->x );
					DirVector.y = ( Ships[ Target ].Object.Pos.y - Pos->y );
					DirVector.z = ( Ships[ Target ].Object.Pos.z - Pos->z );
					NormVector = DirVector;
					NormaliseVector( &NormVector );
							
					Cos = DotProduct( &NormVector, Dir );
				
					if( Cos >= ViewConeCos )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
										&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
						{
							return( true );
						}
					}
				}
			}
			break;

		case OWNER_ENEMY:
			if( !SoundInfo[ Enemies[ Target ].Object.Group ][ Group ] )
			{
				DirVector.x = ( Enemies[ Target ].Object.Pos.x - Pos->x );
				DirVector.y = ( Enemies[ Target ].Object.Pos.y - Pos->y );
				DirVector.z = ( Enemies[ Target ].Object.Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
							
				Cos = DotProduct( &NormVector, Dir );
				
				if( Cos >= ViewConeCos )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
									&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
					{
						return( true );
					}
					else
					{
						TempEnemyIndex = CheckAnyEnemyHit();

						if( TempEnemyIndex == Enemies[ Target ].Index )
						{
							return( true );
						}
					}
				}
			}
			break;

		case OWNER_MINE:
			if( !SoundInfo[ SecBulls[ Target ].GroupImIn ][ Group ] )
			{
				DirVector.x = ( SecBulls[ Target ].Pos.x - Pos->x );
				DirVector.y = ( SecBulls[ Target ].Pos.y - Pos->y );
				DirVector.z = ( SecBulls[ Target ].Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
							
				Cos = DotProduct( &NormVector, Dir );
				
				if( Cos >= ViewConeCos )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
									&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
					{
						return( true );
					}
				}
			}
			break;
	}

	return( false );
}

/*===================================================================
	Procedure	:	Create Scatter Trail ( Secondary Weapons Only )
	Input		:	u_int16_t		SecBull Index
	Output		:	Nothing
===================================================================*/
	VECTOR	OutlineOffsets[ NUMSCATTERPOINTS ] = {

		{ ( +0.0F * 6.0F ), ( -1.0F * 6.0F ), 0.0F },
		{ ( +1.0F * 6.0F ), ( +1.0F * 6.0F ), 0.0F },
		{ ( -1.0F * 6.0F ), ( +1.0F * 6.0F ), 0.0F },

	};

void CreateScatterTrail( u_int16_t i )
{
	int16_t		Count;
	int16_t		Count2;
	u_int16_t		Poly;
	u_int16_t		NextPoly;
	VECTOR		PointPos[ NUMSCATTERPOINTS * 2 ];
	VECTOR	*	SrcPtr;
	VECTOR	*	DstPtr;
	VECTOR	*	TempPtr;
	QUAT		TempQuat;
	MATRIX		TempMat;
	u_int8_t		Red1, Green1, Blue1, Trans1;
	u_int8_t		Red2, Green2, Blue2, Trans2;
	float		TransFloat;
	float		Frame;

	Poly = SecBulls[i].poly;
	for( Count = 0; Count < SecBulls[ i ].numpolys; Count++ )
	{
		if( Poly != (u_int16_t) -1 )
		{
			NextPoly = Polys[ Poly ].Prev;
			KillUsedPoly( Poly );
			Poly = NextPoly;
		}
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (u_int16_t) -1;

	SrcPtr = &PointPos[ 0 ];
	DstPtr = &PointPos[ NUMSCATTERPOINTS ];

	QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].DirVector );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < NUMSCATTERPOINTS; Count++ )
	{
		ApplyMatrix( &TempMat, &OutlineOffsets[ Count ], &DstPtr[ Count ] );
		DstPtr[ Count ].x += SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].y += SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].z += SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
	}

	Trans1 = 255;
	Red1 = 255;
	Green1 = 255;
	Blue1 = 255;

	Frame = (float) ( ( SecBulls[i].LifeCount / 2.0F ) - (float) ( floor( ( SecBulls[i].LifeCount / 2.0F ) / ScatterTrail_Header->Num_Frames )
					* ScatterTrail_Header->Num_Frames ) );

	for( Count = 0; Count < SecBulls[ i ].NumOldPos; Count++ )
	{
		Trans2 = ( ( SecBulls[ i ].NumOldPos - Count ) * ( 255 / MAXOLDPOS ) );
		TransFloat = ( ( (float) Trans2 ) / 256.0F );
		Red2 = (u_int8_t) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].R );
		Green2 = (u_int8_t) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].G );
		Blue2 = (u_int8_t) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].B );

		TempPtr = DstPtr;
		DstPtr = SrcPtr;
		SrcPtr = TempPtr;

		QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].OldPos[ Count ].Dir );
		QuatToMatrix( &TempQuat, &TempMat );

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			ApplyMatrix( &TempMat, &OutlineOffsets[ Count2 ], &DstPtr[ Count2 ] );

			DstPtr[ Count2 ].x += SecBulls[i].OldPos[ Count ].Pos.x + ( -SecBulls[i].OldPos[ Count ].Dir.x * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].y += SecBulls[i].OldPos[ Count ].Pos.y + ( -SecBulls[i].OldPos[ Count ].Dir.y * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].z += SecBulls[i].OldPos[ Count ].Pos.z + ( -SecBulls[i].OldPos[ Count ].Dir.z * ( 190.0F * GLOBAL_SCALE ) );
		}

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			Poly = FindFreePoly();
			if( Poly != (u_int16_t) -1 )
			{
				Polys[ Poly ].Pos1.x = SrcPtr[ Count2 ].x;
				Polys[ Poly ].Pos1.y = SrcPtr[ Count2 ].y;
				Polys[ Poly ].Pos1.z = SrcPtr[ Count2 ].z;
				Polys[ Poly ].Pos2.x = DstPtr[ Count2 ].x;
				Polys[ Poly ].Pos2.y = DstPtr[ Count2 ].y;
				Polys[ Poly ].Pos2.z = DstPtr[ Count2 ].z;
				Polys[ Poly ].Pos3.x = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos3.y = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos3.z = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;
				Polys[ Poly ].Pos4.x = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos4.y = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos4.z = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;

				Polys[ Poly ].Col1.R = Red1;
				Polys[ Poly ].Col1.G = Green1;
				Polys[ Poly ].Col1.B = Blue1;
				Polys[ Poly ].Col2.R = Red2;
				Polys[ Poly ].Col2.G = Green2;
				Polys[ Poly ].Col2.B = Blue2;
				Polys[ Poly ].Col3.R = Red2;
				Polys[ Poly ].Col3.G = Green2;
				Polys[ Poly ].Col3.B = Blue2;
				Polys[ Poly ].Col4.R = Red1;
				Polys[ Poly ].Col4.G = Green1;
				Polys[ Poly ].Col4.B = Blue1;
				Polys[ Poly ].Trans1 = Trans1;
				Polys[ Poly ].Trans2 = Trans2;
				Polys[ Poly ].Trans3 = Trans2;
				Polys[ Poly ].Trans4 = Trans1;
				Polys[ Poly ].Flags = POLY_FLAG_DONTCLIP;
				Polys[ Poly ].Frm_Info = &ScatterTrail_Header;
				Polys[ Poly ].Frame = Frame;
				Polys[ Poly ].SeqNum = POLY_SCATTER_TRAIL;
				Polys[ Poly ].Group = SecBulls[i].GroupImIn;

				SecBulls[ i ].numpolys++;
				SecBulls[ i ].poly = Poly;

				AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16_t) Polys[ Poly ].Frame ) );
			}
		}
		Trans1 = Trans2;
		Red1 = Red2;
		Green1 = Green2;
		Blue1 = Blue2;
	}
}

/*===================================================================
	Procedure	:	Create Tentacle
	Input		:	u_int16_t		SecBull Index
	Output		:	Nothing
===================================================================*/
	VECTOR	TentOutlineOffsets[ NUMSCATTERPOINTS ] = {

		{ ( +0.0F * 10.0F ), ( -1.0F * 10.0F ), 0.0F },
		{ ( +1.0F * 10.0F ), ( +1.0F * 10.0F ), 0.0F },
		{ ( -1.0F * 10.0F ), ( +1.0F * 10.0F ), 0.0F },

	};

void CreateTentacle( u_int16_t i )
{
	int16_t		Count;
	int16_t		Count2;
	u_int16_t		Poly;
	u_int16_t		NextPoly;
	VECTOR		PointPos[ NUMSCATTERPOINTS * 2 ];
	VECTOR	*	SrcPtr;
	VECTOR	*	DstPtr;
	VECTOR	*	TempPtr;
	QUAT		TempQuat;
	MATRIX		TempMat;
	u_int8_t		Red1, Green1, Blue1, Trans1;
	u_int8_t		Red2, Green2, Blue2, Trans2;
	float		Frame;

	Poly = SecBulls[i].poly;
	for( Count = 0; Count < SecBulls[ i ].numpolys; Count++ )
	{
		if( Poly != (u_int16_t) -1 )
		{
			NextPoly = Polys[ Poly ].Prev;
			KillUsedPoly( Poly );
			Poly = NextPoly;
		}
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (u_int16_t) -1;

	SrcPtr = &PointPos[ 0 ];
	DstPtr = &PointPos[ NUMSCATTERPOINTS ];

	QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].DirVector );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < NUMSCATTERPOINTS; Count++ )
	{
		ApplyMatrix( &TempMat, &TentOutlineOffsets[ Count ], &DstPtr[ Count ] );
		DstPtr[ Count ].x += SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].y += SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].z += SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
	}

	Trans1 = 255;
	Red1 = 64;
	Green1 = 64;
	Blue1 = 64;
	Trans2 = 255;
	Red2 = 64;
	Green2 = 64;
	Blue2 = 64;

	Frame = (float) ( ( SecBulls[i].LifeCount / 2.0F ) - (float) ( floor( ( SecBulls[i].LifeCount / 2.0F ) / ScatterTrail_Header->Num_Frames )
					* ScatterTrail_Header->Num_Frames ) );

	for( Count = (int16_t) SecBulls[i].RetractPos; Count < SecBulls[ i ].NumOldPos; Count++ )
	{
		TempPtr = DstPtr;
		DstPtr = SrcPtr;
		SrcPtr = TempPtr;

		QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].OldPos[ Count ].Dir );
		QuatToMatrix( &TempQuat, &TempMat );

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			ApplyMatrix( &TempMat, &TentOutlineOffsets[ Count2 ], &DstPtr[ Count2 ] );

			DstPtr[ Count2 ].x += SecBulls[i].OldPos[ Count ].Pos.x + ( -SecBulls[i].OldPos[ Count ].Dir.x * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].y += SecBulls[i].OldPos[ Count ].Pos.y + ( -SecBulls[i].OldPos[ Count ].Dir.y * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].z += SecBulls[i].OldPos[ Count ].Pos.z + ( -SecBulls[i].OldPos[ Count ].Dir.z * ( 190.0F * GLOBAL_SCALE ) );
		}

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			Poly = FindFreePoly();
			if( Poly != (u_int16_t) -1 )
			{
				Polys[ Poly ].Pos1.x = SrcPtr[ Count2 ].x;
				Polys[ Poly ].Pos1.y = SrcPtr[ Count2 ].y;
				Polys[ Poly ].Pos1.z = SrcPtr[ Count2 ].z;
				Polys[ Poly ].Pos2.x = DstPtr[ Count2 ].x;
				Polys[ Poly ].Pos2.y = DstPtr[ Count2 ].y;
				Polys[ Poly ].Pos2.z = DstPtr[ Count2 ].z;
				Polys[ Poly ].Pos3.x = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos3.y = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos3.z = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;
				Polys[ Poly ].Pos4.x = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos4.y = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos4.z = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;

				Polys[ Poly ].Col1.R = Red1;
				Polys[ Poly ].Col1.G = Green1;
				Polys[ Poly ].Col1.B = Blue1;
				Polys[ Poly ].Col2.R = Red2;
				Polys[ Poly ].Col2.G = Green2;
				Polys[ Poly ].Col2.B = Blue2;
				Polys[ Poly ].Col3.R = Red2;
				Polys[ Poly ].Col3.G = Green2;
				Polys[ Poly ].Col3.B = Blue2;
				Polys[ Poly ].Col4.R = Red1;
				Polys[ Poly ].Col4.G = Green1;
				Polys[ Poly ].Col4.B = Blue1;
				Polys[ Poly ].Trans1 = Trans1;
				Polys[ Poly ].Trans2 = Trans2;
				Polys[ Poly ].Trans3 = Trans2;
				Polys[ Poly ].Trans4 = Trans1;
				Polys[ Poly ].Flags = POLY_FLAG_DONTCLIP;
				Polys[ Poly ].Frm_Info = &Tentacle_Header;
				Polys[ Poly ].Frame = 0;
				Polys[ Poly ].SeqNum = POLY_SCATTER_TRAIL;
				Polys[ Poly ].Group = SecBulls[i].GroupImIn;

				SecBulls[ i ].numpolys++;
				SecBulls[ i ].poly = Poly;

				AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16_t) Polys[ Poly ].Frame ) );
			}
		}
	}
}

/*===================================================================
	Procedure	:	Create Big Flash
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
				:	float		Size
	Output		:	Nothing
===================================================================*/
void CreateBigFlash( VECTOR * Pos, u_int16_t Group, float Size )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 16.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_FLASH;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &BigFlash_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Dir.x = 0.0F;
		FmPolys[ fmpoly ].Dir.y = 0.0F;
		FmPolys[ fmpoly ].Dir.z = 0.0F;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Muzzle Flash
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
				:	float		Size
	Output		:	Nothing
===================================================================*/
void CreateMuzzleFlash( VECTOR * Pos, u_int16_t Group, float Size )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 3.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_MUZZLEFLASH;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &GreyFlare_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Dir.x = 0.0F;
		FmPolys[ fmpoly ].Dir.y = 0.0F;
		FmPolys[ fmpoly ].Dir.z = 0.0F;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}


/*===================================================================
	Procedure	:	Fire Secondary weapons of other players
	Input		:	u_int16_t		Ship Number
	Output		:	Nothing
===================================================================*/
void FireSecondaryWeapons( u_int8_t Ship )
{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DropVector;
	u_int16_t	ID;
	int16_t	Toggle;
	u_int16_t	i = (u_int16_t) -1;
#if SCATTER_TEST
	int16_t	Count;
#endif

	if( ( Ships[Ship].Object.Flags & SHIP_SecToggle ) ) Toggle = 1;
	else Toggle = 0;
	ID = Ships[Ship].SecID;

	ApplyMatrix( &Ships[Ship].Object.FinalMat, &Forward, &Dir );
	ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
	
	switch( Ships[Ship].Secondary )
	{
/*===================================================================
				Fire bottom Left, then Right, then Left, then...
===================================================================*/
		case MUGMISSILE:
			if( Toggle == 0 )
			{
				Toggle = 1;
				DropVector.x = -0.7071F;
				DropVector.y = -0.7071F;
				DropVector.z = 0.0F;
			}
			else
			{
				Toggle = 0;
				DropVector.x = +0.7071F;
				DropVector.y = -0.7071F;
				DropVector.z = 0.0F;
			}
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
		
/*===================================================================
				Fire bottom Left and Right together
===================================================================*/
		case SOLARISMISSILE:
		 	if( Toggle == 0 )
		 	{
		 		Toggle = 1;
		 		DropVector.x = -0.7071F;
		 		DropVector.y = -0.7071F;
		 		DropVector.z = 0.0F;
		 	}
		 	else
		 	{
		 		Toggle = 0;
		 		DropVector.x = +0.7071F;
		 		DropVector.y = -0.7071F;
		 		DropVector.z = 0.0F;
		 	}
	
		 	ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
	
/*===================================================================
				Fire from bottom center at random trajectories
===================================================================*/
		case MULTIPLEMISSILE:
			DropVector.x = ( ( ( (float) Random_Range( 512 ) ) / 640.0F ) - 0.4F );
			DropVector.y = -( ( ( (float) Random_Range( 512 ) ) / 1024.0F ) + 1.0F );
			DropVector.z = ( ( (float) Random_Range( 512 ) ) / 512.0F );
			NormaliseVector( &DropVector );
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
		case THIEFMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
		case SCATTERMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;

			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
							&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
							&DropVector, Ships[ Ship ].Secondary, false );
			break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
		case GRAVGONMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
			
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
	
/*===================================================================
				Fire from center below view
===================================================================*/
		case TITANSTARMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;

/*===================================================================
				Spiral Missile
===================================================================*/
		case ENEMYSPIRALMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;

/*===================================================================
				Photon Missile
===================================================================*/
		case ENEMYHOMINGMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
/*===================================================================
				Blue Photon Missile
===================================================================*/
		case ENEMYBLUEHOMINGMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
/*===================================================================
				Fireball Missile
===================================================================*/
		case ENEMYFIREBALL:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, false );
			break;
	}

	if( i != (u_int16_t) -1 )
	{
		SecBulls[ i ].FramelagAddition = GlobalFramelagAddition;
	}
}

/*===================================================================
	Procedure	:	Destroy secondary missile/mine and clean up.
	Input		:	u_int16_t		Secondary weapon index
				:	VECTOR	*	Detonation point
	Output		:	Nothing
===================================================================*/
void DestroySecondary( u_int16_t i, VECTOR * Int_Point )
{
	if( MyGameStatus != STATUS_SinglePlayer )
	{
		if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SecFire ) &&
			( SecBulls[ i ].OwnerType == OWNER_SHIP ) &&
			( SecBulls[ i ].Owner == WhoIAm ) &&
			( SecBulls[ i ].ID == Ships[ WhoIAm ].SecBullIdCount ) )
		{
			return;
		}
	}

	switch( SecBulls[ i ].Weapon )
	{
		case ENEMYSPIRALMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYHOMINGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYBLUEHOMINGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYFIREBALL:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYDEPTHCHARGE:
			PlayPannedSfx( SFX_BangBang, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, DEPTHCHARGE_SHOCKWAVE );
			break;

   		case MUGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case SOLARISMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case THIEFMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
 			CleanUpSecBull( i );
   			break;

		case ENEMYTENTACLE:
   			CleanUpSecBull( i );
   			break;
			
   		case SCATTERMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case GRAVGONMISSILE:
   			SecBulls[i].ColPoint.x = Int_Point->x;
   			SecBulls[i].ColPoint.y = Int_Point->y;
   			SecBulls[i].ColPoint.z = Int_Point->z;
   			CreateGravgonField( i );
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case MULTIPLEMISSILE:
 			CreateSmallExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case TITANSTARMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
			CleanUpSecBull( i );
   			break;

   		case PINEMISSILE:
 			CreateSmallExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;

		case TITANSTARSHRAPNEL:
			PlayPannedSfx( SFX_Exp3, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, TITANSTAR_SHOCKWAVE );
   			break;

		case PINEMINE:
			PlayPannedSfx( SFX_Exp5, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, ( PINE_SHOCKWAVE + ( SecBulls[ i ].Ammo / 2 ) ) );
			break;
	  	
		case PURGEMINE:
			PlayPannedSfx( SFX_Exp4, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, PURGE_SHOCKWAVE );
			break;
	  
		case QUANTUMMINE:
			PlayPannedSfx( SFX_QuantumMineExplode, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, QUANTUM_SHOCKWAVE );
			break;
	
		case SPIDERMINE:
			PlayPannedSfx( SFX_Exp6, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, SPIDER_SHOCKWAVE );
			break;

		default:
			CleanUpSecBull( i );
			break;
   	}
}

/*===================================================================
	Procedure	:	Generate Mine list for new player
	Input		:	u_int16_t			Ship
				:	SHORTMINE	*	Mine Array to be filled in
				:	BYTE		*	Mine Number to be filled in
				:	BYTE			Section of Mines
	Output		:	Nothing

===================================================================*/

float ticksperframe = 14.0F; 
px_timer_t last_mine_timer;

void GenMineList( u_int16_t Ship, SHORTMINE * MineSlots, BYTE * NumMines, BYTE Section )
{
	int16_t	Count;
	int16_t	i;
	int16_t	NumUsed = 0;
	float	Diff_Float;

	Diff_Float = (float) timer_run( &last_mine_timer ) / ticksperframe;

	Section = ( ( ( MAXSECONDARYWEAPONBULLETS + ( MAXGENMINECOUNT - 1 ) ) / MAXGENMINECOUNT ) - Section );

	for( Count = 0; Count < MAXGENMINECOUNT; Count++ )
	{
		i = ( Count + ( Section * MAXGENMINECOUNT ) );

		if( i < MAXSECONDARYWEAPONBULLETS )
		{
			if( MinesCopy[ Ship ][ i ].Owner != (u_int16_t) -1 )
			{
				MineSlots[ NumUsed ].Owner		= MinesCopy[ Ship ][ i ].Owner;
				MineSlots[ NumUsed ].Group		= MinesCopy[ Ship ][ i ].Group;
				MineSlots[ NumUsed ].Pos		= MinesCopy[ Ship ][ i ].Pos;
				MineSlots[ NumUsed ].Offset		= MinesCopy[ Ship ][ i ].Offset;
				MineSlots[ NumUsed ].LifeCount	= ( MinesCopy[ Ship ][ i ].LifeCount + Diff_Float );
				MineSlots[ NumUsed ].Dir		= MinesCopy[ Ship ][ i ].Dir;
				MineSlots[ NumUsed ].UpVector	= MinesCopy[ Ship ][ i ].UpVector;
				MineSlots[ NumUsed ].DropDir	= MinesCopy[ Ship ][ i ].DropDir;
				MineSlots[ NumUsed ].BulletID	= MinesCopy[ Ship ][ i ].BulletID;
				MineSlots[ NumUsed ].Weapon		= MinesCopy[ Ship ][ i ].Weapon;
				NumUsed++;
			}
		}
	}

	*NumMines = (BYTE) NumUsed;
}

/*===================================================================
	Procedure	:	Generate Mines from list for new player
	Input		:	SHORTMINE	*	Mine Array
				:	BYTE			Number of Mines
	Output		:	Nothing
===================================================================*/
void RegenMineList( SHORTMINE * Slots, BYTE Num )
{
	int16_t		Count;
	u_int16_t		i;

	for( Count = 0; Count < Num; Count++ )
	{
		i = InitOneSecBull( OWNER_SHIP, (BYTE) Slots[ Count ].Owner, Slots[ Count ].BulletID,
							Slots[ Count ].Group, &Slots[ Count ].Pos,
							&Slots[ Count ].Offset, &Slots[ Count ].Dir, &Slots[ Count ].UpVector,
							&Slots[ Count ].DropDir, (int8_t) Slots[ Count ].Weapon, false );
		if( i != (u_int16_t) -1 )
		{
			SecBulls[i].LifeCount = Slots[ Count ].LifeCount;
		}
	}
}

/*===================================================================
	Procedure	:	Copy Mines into copy of mines
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SyncMines( void )
{
	u_int16_t		i;
	u_int16_t		Next;
	float		Diff_Float;

	Diff_Float = (float) timer_run( &last_mine_timer ) / ticksperframe;

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			SecBulls[i].LifeCount += Diff_Float;
		}

		i = Next;
	}																				
}

/*===================================================================
	Procedure	:	Regenerate qued Mines
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void RegenerateQuedMines( void )
{
	int16_t				Count;
	SHORTMINE		*	I_Ptr;
	SHORTKILLMINE	*	K_Ptr;

	if( !Ships[ WhoIAm ].Mines && NumMissedMines )
	{
		for( Count = 0; Count < NumMissedMines; Count++ )
		{
			switch( MissedMines[ Count ].Type )
			{
				case MM_TYPE_INIT:
					I_Ptr = (SHORTMINE *) MissedMines[ Count ].Struct;

					DebugPrintf( "Regenerated MissedInitMine() %d\n", Count );

					InitOneSecBull( I_Ptr->OwnerType, I_Ptr->Owner, I_Ptr->BulletID,
									I_Ptr->Group, &I_Ptr->Pos, &I_Ptr->Offset, &I_Ptr->Dir,
									&I_Ptr->UpVector, &I_Ptr->DropDir, (char) I_Ptr->Weapon, false );
					break;

				case MM_TYPE_KILL:
					K_Ptr = (SHORTKILLMINE *) MissedMines[ Count ].Struct;
					KillOwnerIDSecBulls( K_Ptr->OwnerType, K_Ptr->Owner, K_Ptr->ID );
					break;
			}
		}

		NumMissedMines = 0;
		NumMissedInitMines = 0;
		NumMissedKillMines = 0;
	}
}

/*===================================================================
	Procedure	:	Copy Mines into copy of mines
	Input		:	u_int16_t		Player
	Output		:	Nothing
===================================================================*/
void CopyMines( u_int16_t Player )
{
	u_int16_t	i;
	u_int16_t	Next;
	int16_t	Num = 0;

	timer_run( &last_mine_timer );

	for( i = 0; i < MAXSECONDARYWEAPONBULLETS; i++ )
	{
		MinesCopy[ Player ][ i ].Owner = (u_int16_t) -1;
	}

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			MinesCopy[ Player ][ Num ].Owner	= SecBulls[ i ].Owner;
			MinesCopy[ Player ][ Num ].Group	= SecBulls[ i ].GroupImIn;
			MinesCopy[ Player ][ Num ].Pos		= SecBulls[ i ].Pos;
			MinesCopy[ Player ][ Num ].Offset	= SecBulls[ i ].Offset;
			MinesCopy[ Player ][ Num ].LifeCount= SecBulls[ i ].LifeCount;
			MinesCopy[ Player ][ Num ].Dir		= SecBulls[ i ].DirVector;
			MinesCopy[ Player ][ Num ].UpVector	= SecBulls[ i ].UpVector;
			MinesCopy[ Player ][ Num ].DropDir	= SecBulls[ i ].DropVector;
			MinesCopy[ Player ][ Num ].BulletID	= SecBulls[ i ].ID;
			MinesCopy[ Player ][ Num ].Weapon	= SecBulls[ i ].Weapon;
			Num++;
		}

		i = Next;
	}																				
}

/*===================================================================
	Procedure	:	Kill mine/missiles by owner/id
	Input		:	u_int16_t		OwnerType
				:	u_int16_t		Owner
				:	u_int16_t		ID
	Output		:	Nothing
===================================================================*/
void KillOwnerIDSecBulls( u_int16_t OwnerType, u_int16_t Owner, u_int16_t ID )
{
	u_int16_t	i;
	u_int16_t	Next;

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OwnerType ) && ( SecBulls[ i ].Owner == Owner ) && ( SecBulls[ i ].ID == ID ) )
		{
				if( ( SecBulls[i].SecType == SEC_MINE ) && ( Owner == WhoIAm ) ) SecAmmoUsed[ SecBulls[i].Weapon ]++;
 			CleanUpSecBull( i );
		}

		i = Next;
	}																				
}

/*===================================================================
	Procedure	:	Check if within range of mine
	Input		:	VECTOR	*	Pos
	Output		:	bool		True/False ( True if collision )
===================================================================*/
bool CheckForMines( VECTOR * Pos )
{
	u_int16_t		i;
	u_int16_t		Next;
	VECTOR		DistVector;
	float		DistToMine;

	i = FirstSecBullUsed;

	while( i != (u_int16_t) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			DistVector.x = ( Pos->x - SecBulls[ i ].Pos.x );
			DistVector.y = ( Pos->y - SecBulls[ i ].Pos.y );
			DistVector.z = ( Pos->z - SecBulls[ i ].Pos.z );
			DistToMine = VectorLength( &DistVector );

			if( DistToMine < ( BLOCKSIZE * 6.0F ) )			// 6 Blocks
			{
				return true;
			}
		}

		i = Next;
	}																				

	return false;
}

/*===================================================================
	Procedure	:	Create Explosion
	Input		:	u_int16_t		Ship
	Output		:	Nothing
===================================================================*/
float	ShipSmokeTimes[ MAX_PLAYERS ];
float	ShipSmokeInterval = 1.0F;

void DoDamagedEffects( u_int16_t i )
{
	u_int16_t	fmpoly;
	VECTOR	TempUp;
	VECTOR	MoveDir;
	VECTOR	LookDir;
	float	Cos;

	ShipSmokeInterval = (float) 1;

	if( ( Ships[ i ].enable ) &&
		( Ships[ i ].Object.Mode != LIMBO_MODE ) &&
		( Ships[ i ].Object.Mode != GAMEOVER_MODE ) &&
		( ( GameStatus[i] == STATUS_Normal ) || ( GameStatus[i] == STATUS_SinglePlayer ) ) )
	{
		if( Ships[i].Object.Mode == DEATH_MODE )
		{
			MoveDir = Ships[i].LastMove;
			NormaliseVector( &MoveDir );
			LookDir.x = ( Ships[i].Object.Pos.x + ( MoveDir.x * ( SHIP_RADIUS * 0.65F ) ) );
			LookDir.y = ( Ships[i].Object.Pos.y + ( MoveDir.y * ( SHIP_RADIUS * 0.65F ) ) );
			LookDir.z = ( Ships[i].Object.Pos.z + ( MoveDir.z * ( SHIP_RADIUS * 0.65F ) ) );
			MoveDir.x = -MoveDir.x;
			MoveDir.y = -MoveDir.y;
			MoveDir.z = -MoveDir.z;
			CreateSpotFXBurning( &LookDir, &MoveDir, Ships[ i ].Object.Group );
		}

		if( Ships[ i ].Object.Hull < 64 )
		{
			if( IsGroupVisible[ Ships[ i ].Object.Group ] )
			{
				ShipSmokeTimes[ i ] += framelag;
	
				if( ShipSmokeTimes[ i ] >= ShipSmokeInterval )
				{
					ShipSmokeTimes[ i ] = 0.0F;
	
					MoveDir = Ships[i].LastMove;
					ApplyMatrix( &Ships[ i ].Object.FinalMat, &Forward, &LookDir );
					NormaliseVector( &MoveDir );
					NormaliseVector( &LookDir );
					
					Cos = DotProduct( &MoveDir, &LookDir );
					
					if( Cos > -0.1F )														// Only when moving forward
					{
						fmpoly = FindFreeFmPoly();
					
						if( fmpoly != (u_int16_t ) -1 )
						{
							FmPolys[ fmpoly ].LifeCount = 1000.0F;
							ApplyMatrix( &Ships[ i ].Object.FinalMat, &Backward, &FmPolys[ fmpoly ].Dir );
							ApplyMatrix( &Ships[ i ].Object.FinalMat, &SlideUp, &TempUp );
							FmPolys[ fmpoly ].UpVector = SlideUp;
							FmPolys[ fmpoly ].Pos.x = ( Ships[ i ].Object.Pos.x + ( FmPolys[ fmpoly ].Dir.x * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.x * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].Pos.y = ( Ships[ i ].Object.Pos.y + ( FmPolys[ fmpoly ].Dir.y * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.y * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].Pos.z = ( Ships[ i ].Object.Pos.z + ( FmPolys[ fmpoly ].Dir.z * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.z * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].SeqNum = FM_BLACK_SMOKE;															   
							FmPolys[ fmpoly ].Frame = 0.0F;
							FmPolys[ fmpoly ].R = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
							FmPolys[ fmpoly ].G = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
							FmPolys[ fmpoly ].B = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
#if ACTUAL_TRANS
							FmPolys[ fmpoly ].Trans = 128;
#else
							FmPolys[ fmpoly ].Trans = 255;
#endif
							FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
							FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
							FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
							FmPolys[ fmpoly ].xsize = SMOKE_TRAIL_SIZE;
							FmPolys[ fmpoly ].ysize = SMOKE_TRAIL_SIZE;
							FmPolys[ fmpoly ].Speed = ( ( ( ( (float) Random_Range( 4000 ) ) / 1000.0F ) + 2.0F ) * GLOBAL_SCALE ); //( 16.0F * GLOBAL_SCALE );
							FmPolys[ fmpoly ].UpSpeed = 0.0F;
							FmPolys[ fmpoly ].Group = Ships[ i ].Object.Group;
							AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
						}
					}
				}
			}
		}
		else
		{
			ShipSmokeTimes[ i ] = 0.0F;
		}
	}
}

/*===================================================================
	Procedure	:	Drop Currently selected secondary weapon
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DropSecondary( void )
{
	float	Speed;
	float	LifeCount;
	int16_t	Count = 0;
	int16_t	PickupNum;
	int16_t	Secondary;
	VECTOR	DirVector;

	Secondary = Ships[ WhoIAm ].Secondary;

	switch( Secondary )
	{
		case QUANTUMMINE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_QuantumPickup;
				Count = 1;
			}
			break;

		case GRAVGONMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Gravgon;
				Count = 1;
			}
			break;

		case TITANSTARMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_TitanStar;
				Count = 1;
			}
			break;

		case SCATTERMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Scatter;
				Count = 1;
			}
			break;

		case THIEFMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Thief;
				Count = 1;
			}
			break;

		case SPIDERMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_SpiderPod;
				Count = 3;
			}
			break;

		case MULTIPLEMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 50 ) )
			{
				PickupNum = PICKUP_Launcher;
				Count = 50;
			}
			break;

		case SOLARISMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_HeatseakerPickup;
				Count = 3;
			}
			break;

		case PINEMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_PinePickup;
				Count = 3;
			}
			break;

		case MUGMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_Mugs;
				Count = 3;
			}
			break;

		case PURGEMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_PurgePickup;
				Count = 3;
			}
			break;
	}

	if( Count )
	{
		SecondaryAmmo[ Secondary ] -= Count;
		if( !SecondaryAmmo[ Secondary ] ) SecondaryWeaponsGot[ Secondary ] = 0;
		ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &DirVector );
		LifeCount = -1.0F;
		Speed = PICKUP_SPEED;
		InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
	}
}

/*===================================================================
	Procedure	:	Create TitanStar Shrapnel
	Input		:	u_int16_t		SecBull Index
				:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateTitanStarShrapnel( u_int16_t i, VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	int16_t	Count;
	int16_t	NumLeft;
	int16_t	ShipTargCount;
	VECTOR	UpVector = { 0.0F, 1.0F, 0.0F };
	VECTOR	DropVector = { 0.0F, 0.0F, 0.0F };
	VECTOR	DirVector;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;

	u_int16_t	FirstID;
	VECTOR	Directions[ NUMTITANBITS ];

	if( SecBulls[i].OwnerType == OWNER_SHIP )
		if( TeamGame ) MyTeam = TeamNumber[ SecBulls[i].Owner ];

	NumLeft = NUMTITANBITS;
	ShipTargCount = 0;

	FirstID = Ships[ SecBulls[i].Owner ].SecBullIdCount + 1;

	// for each baby titan to create
	while( NumLeft )
	{

		// send one baby titan after each multi-player player
		for( Count = 0; Count < MAX_PLAYERS; Count++ )
		{
			if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

			if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( Count != SecBulls[i].Owner ) && ( MyTeam != ShipsTeam ) ) || ( SecBulls[i].OwnerType != OWNER_SHIP ) )
			{
				if( NumLeft )
				{
					if( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) && !Ships[Count].Invul )
					{
						if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
						{
							DirVector.x = ( Ships[ Count ].Object.Pos.x - SecBulls[i].Pos.x );
							DirVector.y = ( Ships[ Count ].Object.Pos.y - SecBulls[i].Pos.y );
							DirVector.z = ( Ships[ Count ].Object.Pos.z - SecBulls[i].Pos.z );
							NormaliseVector( &DirVector );

							if( ShipTargCount )
							{
								DirVector.x += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
					   			DirVector.y += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
					   			DirVector.z += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
								NormaliseVector( &DirVector );
							}

//							InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//											Group, Pos, &DropVector, &DirVector, &UpVector,
//											&DropVector, TITANSTARSHRAPNEL, true );
							InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
											Group, Pos, &DropVector, &DirVector, &UpVector,
											&DropVector, TITANSTARSHRAPNEL, false );
							Directions[ NumLeft-1 ] = DirVector;
							NumLeft--;
						}
					}
				}
			}
		}

		// send the rest of baby titans towards any enemies
		Enemy = FirstEnemyUsed;
		while( ( Enemy != NULL ) && ( NumLeft ) )
		{
			NextEnemy = Enemy->NextUsed;
	
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( SecBulls[i].Owner == Enemy->Index ) ) )
				{
					if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
					{
						DirVector.x = ( Enemy->Object.Pos.x - SecBulls[i].Pos.x );
						DirVector.y = ( Enemy->Object.Pos.y - SecBulls[i].Pos.y );
						DirVector.z = ( Enemy->Object.Pos.z - SecBulls[i].Pos.z );
						NormaliseVector( &DirVector );

						if( ShipTargCount )
						{
							DirVector.x += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							DirVector.y += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							DirVector.z += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							NormaliseVector( &DirVector );
						}

//						InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//										Group, Pos, &DropVector, &DirVector, &UpVector,
//										&DropVector, TITANSTARSHRAPNEL, true );
						InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
										Group, Pos, &DropVector, &DirVector, &UpVector,
										&DropVector, TITANSTARSHRAPNEL, false );
						Directions[ NumLeft-1 ] = DirVector;

						Enemy->AIFlags |= AI_TARGETED_ONEOFF;
						Enemy->TargetingPos = SecBulls[ i ].Pos;
						Enemy->TargetingDir = SecBulls[ i ].DirVector;

						NumLeft--;
					}
				}
			}
			Enemy = NextEnemy;
		}

		ShipTargCount++;

		// send any left over baby titans at random directions
		if( NumLeft == NUMTITANBITS )
		{
			for( Count = 0; Count < NUMTITANBITS; Count++ )
			{
				DirVector.x = Dir->x + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		DirVector.y = Dir->y + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		DirVector.z = Dir->z + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
				NormaliseVector( &DirVector );
		
//				InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//								Group, Pos, &DropVector, &DirVector, &UpVector,
//								&DropVector, TITANSTARSHRAPNEL, true );
				InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
								Group, Pos, &DropVector, &DirVector, &UpVector,
								&DropVector, TITANSTARSHRAPNEL, false );
				Directions[ Count ] = DirVector;

			}
			break;
		}
	}

#ifdef TITANBITS_SEND
	if( ( SecBulls[ i ].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == WhoIAm ) )
	{
		TitanBitsSend( SecBulls[i].OwnerType, SecBulls[i].Owner, FirstID, Group, Pos,
							&DropVector, &UpVector, &DropVector, TITANSTARSHRAPNEL, &Directions[ 0 ] );
	}
#endif

}


/*===================================================================
	Procedure	:	Create New Smoke Ring
	Input		:	VECTOR	*	Position
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSmokeRing( VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, u_int16_t Group )
{
	int16_t	Count;
	VECTOR	TempDir;
	QUAT	TempQuat;
	MATRIX	TempMat;
	u_int16_t	fmpoly;

	QuatFromDirAndUp( DirVector, UpVector, &TempQuat );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < 20; Count++ )
	{
		TempDir.x = (float) sin( D2R( Count * 18 ) );
		TempDir.y = 0.0F;
		TempDir.z = (float) cos( D2R( Count * 18 ) );

		ApplyMatrix( &TempMat, &TempDir, &TempDir );

		fmpoly = FindFreeFmPoly();

		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SMOKE_RING;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = 8.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].xsize = NEW_TRAIL_SIZE * 4.0F;
			FmPolys[ fmpoly ].ysize = NEW_TRAIL_SIZE * 4.0F;
			FmPolys[ fmpoly ].Dir = TempDir;
			FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.4F );
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Create New Smoke Ring
	Input		:	VECTOR	*	Position
				:	VECTOR	*	DirVector
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSplash( VECTOR * Pos, VECTOR * DirVector, u_int16_t Group )
{
	int16_t	Count;
	VECTOR	TempDir;
	u_int16_t	fmpoly;
	VECTOR	RevDir;
	VECTOR	NewPos;
	float	Scalar;
	u_int8_t	Red;
	u_int8_t	Green;
	u_int8_t	Blue;

	GetWaterColour( Group, &Red, &Green, &Blue );

	NewPos.x = Pos->x;
	NewPos.y = Pos->y + 1.0F;
	NewPos.z = Pos->z;

	RevDir.x = -DirVector->x;
	RevDir.y = -DirVector->y;
	RevDir.z = -DirVector->z;
	NormaliseVector( &RevDir );

	for( Count = 0; Count < 20; Count++ )
	{
		TempDir.x = (float) cos( D2R( Count * 18 ) );
		TempDir.y = 0.0F;
		TempDir.z = (float) sin( D2R( Count * 18 ) );

		Scalar = 1.5F + ( DotProduct( &TempDir, &RevDir ) * -( ( ( (float) Random_Range( 10000 ) ) / 2000.0F ) ) + 1.5F );
		TempDir.y = 1.0F;

		NormaliseVector( &TempDir );
		TempDir.x *= Scalar;
		TempDir.y *= Scalar;
		TempDir.z *= Scalar;

		fmpoly = FindFreeFmPoly();
		
		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = ( (float) Random_Range( 180 ) ) + 60.0F;
			FmPolys[ fmpoly ].Pos = NewPos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_WATERSPLASH;
			FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = ( ( (float) Random_Range( 10000 ) ) / 1000.0F ) + 8.0F;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Trans = 255;
			FmPolys[ fmpoly ].Dir = TempDir;
			FmPolys[ fmpoly ].R = ( 32 + ( Red / 2 ) );
			FmPolys[ fmpoly ].G = ( 32 + ( Green / 2 ) );
			FmPolys[ fmpoly ].B = ( 32 + ( Blue / 2 ) );
			FmPolys[ fmpoly ].Start_R = FmPolys[ fmpoly ].R;
			FmPolys[ fmpoly ].Start_G = FmPolys[ fmpoly ].G;
			FmPolys[ fmpoly ].Start_B = FmPolys[ fmpoly ].B;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = ( ( ( (float) Random_Range( 16 ) ) + 8.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}

	}
}

/*===================================================================
	Procedure	:	Find Enemy closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	u_int16_t		Group
				:	u_int16_t		My Type
				:	u_int16_t		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	u_int16_t		Enemy Index (-1 if none)
===================================================================*/
u_int16_t TargetClosestEnemy( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos )
{
	float		Cos;
	u_int16_t		ClosestEnemy = (u_int16_t) -1;
	VECTOR		TempVector;
	VECTOR		DirVector;
	VECTOR		NormVector;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	u_int16_t		Int_Group;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	u_int16_t		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
			{
				DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
				DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
				DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
				
				Cos = DotProduct( &NormVector, Dir );
			
				if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
											&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
					{
						*ClosestCos = Cos;
						ClosestEnemy = Enemy->Index;
					}
					else
					{
						TempEnemyIndex = CheckAnyEnemyHit();

						if( TempEnemyIndex == Enemy->Index )
						{
							*ClosestCos = Cos;
							ClosestEnemy = TempEnemyIndex;
						}
					}
				}
			}
		}

		Enemy = NextEnemy;
	}
	return( ClosestEnemy );
}

/*===================================================================
	Procedure	:	Find Secondary closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	u_int16_t		Group
				:	u_int16_t		My Type
				:	u_int16_t		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	u_int16_t		SecBulls (Secondary) Index (-1 if none)
===================================================================*/
u_int16_t TargetClosestSecondary( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int16_t MeType, u_int16_t Me, float ViewConeCos, float * ClosestCos )
{
	float		Cos;
	u_int16_t		ClosestSecondary = (u_int16_t) -1;
	VECTOR		TempVector;
	VECTOR		DirVector;
	VECTOR		NormVector;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	u_int16_t		Int_Group;

	int16_t		Count;
	GROUPLIST * GroupsVisible;
	u_int16_t	  * GroupList;
	u_int16_t		CurrentGroup;
	SECONDARYWEAPONBULLET * SecBull;
	BYTE		MyTeam = 0;
	BYTE		ShipsTeam = 1;

	if( MeType == OWNER_SHIP )
	{
		if( TeamGame ) MyTeam = TeamNumber[ Me ];
	}

	GroupsVisible = VisibleGroups( Group );

	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		SecBull = SecBullGroups[ CurrentGroup ];

		while( SecBull )
		{
			if( SecBull->Flags & SECFLAGS_CanTarget )
			{
				if( TeamGame ) ShipsTeam = TeamNumber[ SecBull->Owner ];

				if( ( TeamGame && ( MyTeam != ShipsTeam ) ) || !TeamGame )
				{
					if( ( SecBull->OwnerType != MeType ) || ( SecBull->Owner != Me ) )
					{
						DirVector.x = ( SecBull->Pos.x - Pos->x );
						DirVector.y = ( SecBull->Pos.y - Pos->y );
						DirVector.z = ( SecBull->Pos.z - Pos->z );
						NormVector = DirVector;
						NormaliseVector( &NormVector );
							
						Cos = DotProduct( &NormVector, Dir );
					
						if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
						{
							if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
													&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
							{
								*ClosestCos = Cos;
								ClosestSecondary = SecBull->Index;
							}
						}
					}
				}
			}

			SecBull = SecBull->NextInGroup;
		}
	}

	return( ClosestSecondary );
}

/*===================================================================
	Procedure	:	Test proximity to ships
	Input		:	VECTOR	*	Pos
				:	u_int16_t		Group
				:	float		Radius
				:	u_int16_t		My Type
				:	u_int16_t		My Index ( -1 Even Me )
	Output		:	bool		True/False
===================================================================*/
bool CheckProximityToShips( VECTOR * Pos, u_int16_t Group, float Radius, u_int16_t OwnerType, u_int16_t Owner )
{ 
	u_int16_t	Count;
	VECTOR	TempVector;
	VECTOR	IntPoint;
	NORMAL	IntPointNormal;
	u_int16_t	EndGroup;
	float	DistToShip;
	VECTOR	DirVector;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( !( ( OwnerType == OWNER_SHIP ) && ( Owner == Count ) ) )
   		{
			if( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) )
   			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
				{
   					DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
   					DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
   					DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
   					
   					DistToShip = VectorLength( &DirVector );
   					
   					if( DistToShip < Radius )
   					{
   						if( BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, true, NULL ) == false )
   						{
							return( true );
   						}
   					}
				}
   			}
   		}
	}
	return( false );
}

/*===================================================================
	Procedure	:	Test proximity to Enemies
	Input		:	VECTOR	*	Pos
				:	u_int16_t		Group
				:	float		Radius
				:	u_int16_t		My Type
				:	u_int16_t		My Index ( -1 Even Me )
	Output		:	bool		True/False
===================================================================*/
bool CheckProximityToEnemies( VECTOR * Pos, u_int16_t Group, float Radius, u_int16_t OwnerType, u_int16_t Owner )
{ 
	VECTOR		TempVector;
	VECTOR		IntPoint;
	NORMAL		IntPointNormal;
	u_int16_t		EndGroup;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	float		DistToEnemy;
	VECTOR		DirVector;
	u_int16_t		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( ( OwnerType == OWNER_ENEMY ) && ( Owner == Enemy->Index ) ) )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
   			{
				if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
				{
					DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
  					DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
   					DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
   					
   					DistToEnemy = VectorLength( &DirVector );
   					
   					if( DistToEnemy < Radius )
   					{
   						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, true, NULL ) )
   						{
							return( true );
   						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								return( true );
							}
						}
   					}
   				}
   			}
		}

		Enemy = NextEnemy;
	}
	return( false );
}

/*===================================================================
	Procedure	:	Test proximity to Enemies
	Input		:	VECTOR	*	Pos
				:	u_int16_t		Group
				:	float	*	Radius
				:	u_int16_t		My Type
				:	u_int16_t		My Index ( -1 Even Me )
	Output		:	u_int16_t		Enemy
===================================================================*/
u_int16_t GetClosestEnemy( VECTOR * Pos, u_int16_t Group, float * Radius, u_int16_t OwnerType, u_int16_t Owner )
{ 
	VECTOR		TempVector;
	VECTOR		IntPoint;
	NORMAL		IntPointNormal;
	u_int16_t		EndGroup;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	float		DistToEnemy;
	VECTOR		DirVector;
	u_int16_t		Closest = (u_int16_t) -1;
	u_int16_t		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( ( OwnerType == OWNER_ENEMY ) && ( Owner == Enemy->Index ) ) )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
   			{
				if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
				{
					DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
  					DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
   					DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
   					
   					DistToEnemy = VectorLength( &DirVector );
   					
   					if( DistToEnemy < *Radius )
   					{
   						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, true, NULL ) )
   						{
							Closest = Enemy->Index;
							*Radius = DistToEnemy;
   						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								Closest = TempEnemyIndex;
								*Radius = DistToEnemy;
							}
						}
   					}
   				}
   			}
		}

		Enemy = NextEnemy;
	}
	return( Closest );
}

/*===================================================================
	Procedure	:	Create FireBall
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateFireBall( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	int16_t	Count;
	u_int16_t	fmpoly;
	VECTOR	Offset;
	float	Dist;

	for( Count = 0; Count < 10; Count++ )
	{
		Offset.x = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );	// -1 to +1
		Offset.y = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );
		Offset.z = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );
		NormaliseVector( &Offset );
		Dist = (float) Random_Range( 40 );
		Offset.x *= Dist;
		Offset.y *= Dist;
		Offset.z *= Dist;
		Offset.x += ( Dir->x * 20.0F );
		Offset.y += ( Dir->y * 20.0F );
		Offset.z += ( Dir->z * 20.0F );

		fmpoly = FindFreeFmPoly();

		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = Dist;
			FmPolys[ fmpoly ].Pos.x = ( Pos->x + Offset.x );
			FmPolys[ fmpoly ].Pos.y = ( Pos->y + Offset.y );
			FmPolys[ fmpoly ].Pos.z = ( Pos->z + Offset.z );
			FmPolys[ fmpoly ].SeqNum = FM_FIREBALL;
	   		FmPolys[ fmpoly ].AnimSpeed = ( 2.0F - ( Dist / 22.0F ) );
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].R = 128;
			FmPolys[ fmpoly ].G = 64;
			FmPolys[ fmpoly ].B = 64;
			FmPolys[ fmpoly ].Start_R = 128;
			FmPolys[ fmpoly ].Start_G = 64;
			FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Dir.x = -Dir->x;
			FmPolys[ fmpoly ].Dir.y = -Dir->y;
			FmPolys[ fmpoly ].Dir.z = -Dir->z;
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			FmPolys[ fmpoly ].Speed = ( -10.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].UpSpeed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}

	fmpoly = FindFreeFmPoly();

   	if( fmpoly != (u_int16_t ) -1 )
   	{
   		FmPolys[ fmpoly ].LifeCount = 1000.0F;
   		FmPolys[ fmpoly ].Pos = *Pos;
   		FmPolys[ fmpoly ].SeqNum = FM_ANIMSPEED;
   		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
   		FmPolys[ fmpoly ].Frame = 0.0F;
   		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
   		FmPolys[ fmpoly ].R = 64;
   		FmPolys[ fmpoly ].G = 64;
   		FmPolys[ fmpoly ].B = 64;
		FmPolys[ fmpoly ].Start_R = 64;
		FmPolys[ fmpoly ].Start_G = 64;
		FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
   		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].Speed = 0.0F;
   		FmPolys[ fmpoly ].Group = Group;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Setup Group link lists for secondary weapoons
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SetupSecBullGroups( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		SecBullGroups[ Count ] = NULL;
		NumSecBullsPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add Secondary bullet to group link list
	Input		:	u_int16_t		SecBull Index
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void AddSecBullToGroup( u_int16_t i, u_int16_t Group )
{
	if( Group == (u_int16_t) -1 ) return;
	SecBulls[ i ].PrevInGroup = NULL;
	SecBulls[ i ].NextInGroup = SecBullGroups[ Group ];
	if( SecBulls[ i ].NextInGroup ) SecBulls[ i ].NextInGroup->PrevInGroup = &SecBulls[ i ];
	SecBullGroups[ Group ] = &SecBulls[ i ];
	NumSecBullsPerGroup[ Group ]++;
}

/*===================================================================
	Procedure	:	Remove Secondary bullet from group link list
	Input		:	u_int16_t		SecBull Index
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void RemoveSecBullFromGroup( u_int16_t i, u_int16_t Group )
{
	if( Group == (u_int16_t) -1 ) return;
	if( SecBulls[ i ].PrevInGroup ) SecBulls[ i ].PrevInGroup->NextInGroup = SecBulls[ i ].NextInGroup;
	else SecBullGroups[ Group ] = SecBulls[ i ].NextInGroup;
	if( SecBulls[ i ].NextInGroup ) SecBulls[ i ].NextInGroup->PrevInGroup = SecBulls[ i ].PrevInGroup;
	SecBulls[ i ].PrevInGroup = NULL;
	SecBulls[ i ].NextInGroup = NULL;
	NumSecBullsPerGroup[ Group ]--;
}

/*===================================================================
	Procedure	:	Move Secondary bullet from 1 group to another
	Input		:	u_int16_t		SecBull Index
				:	u_int16_t		OldGroup
				:	u_int16_t		NewGroup
	Output		:	Nothing
===================================================================*/
void MoveSecBullToGroup( u_int16_t i, u_int16_t OldGroup, u_int16_t NewGroup )
{
	if( OldGroup == (u_int16_t) -1 ) return;
	RemoveSecBullFromGroup( i, OldGroup );
	if( NewGroup == (u_int16_t) -1 ) return;
	AddSecBullToGroup( i, NewGroup );
}

/*===================================================================
	Procedure	:	Get Secondary Bullet ID
	Input		:	u_int16_t		OwnerType
				:	u_int16_t		Owner
	Output		:	Nothing
===================================================================*/
u_int16_t GetSecondaryBulletID( u_int16_t OwnerType, u_int16_t Owner )
{
	u_int16_t	BulletID = 0;

	switch( OwnerType )
	{
		case OWNER_NOBODY:
		case OWNER_BGOBJECT:
		case OWNER_MINE:
		case OWNER_MODELSPOTFX:
			BulletID = GlobalSecBullsID++;
			break;

		case OWNER_SHIP:
			BulletID = ++Ships[ Owner ].SecBullIdCount;
			break;

		case OWNER_ENEMY:
			BulletID = ++Enemies[ Owner ].BulletID;
			break;
	}

	return( BulletID );
}

/*===================================================================
	Procedure	:	Save SecBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveSecBulls( FILE * fp )
{
	u_int16_t	i;
	u_int16_t	TempIndex = (u_int16_t) -1;

	if( fp )
	{
		for( i = 0; i < MAXSECONDARYWEAPONS; i++ )
		{
			fwrite( &SecondaryWeaponsGot[ i ], sizeof( int16_t ), 1, fp );
			fwrite( &SecAmmoUsed[ i ], sizeof( int16_t ), 1, fp );
			fwrite( &SecondaryAmmo[ i ], sizeof( int16_t ), 1, fp );
		}

		for( i = 0; i < MAXPICKUPS; i++ )
		{
			fwrite( &SecWeaponsGot[ i ], sizeof( int8_t ), 1, fp );
		}

		for( i = 0; i < MAXGROUPS; i++ )
		{
			fwrite( &NumSecBullsPerGroup[ i ], sizeof( int16_t ), 1, fp );
			if( SecBullGroups[ i ] ) fwrite( &SecBullGroups[ i ]->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		}


		fwrite( &FirstSecBullUsed, sizeof( FirstSecBullUsed ), 1, fp );
		fwrite( &FirstSecBullFree, sizeof( FirstSecBullFree ), 1, fp );
		fwrite( &SecondaryFireDelay, sizeof( SecondaryFireDelay ), 1, fp );
		fwrite( &TargetComputerOn, sizeof( TargetComputerOn ), 1, fp );
		fwrite( &ImTargeted, sizeof( ImTargeted ), 1, fp );
		fwrite( &TargetedDelay, sizeof( TargetedDelay ), 1, fp );
		fwrite( &GlobalSecBullsID, sizeof( GlobalSecBullsID ), 1, fp );

		i = FirstSecBullUsed;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &SecBulls[ i ].Used, sizeof( bool ), 1, fp );
			fwrite( &SecBulls[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Prev, sizeof( u_int16_t ), 1, fp );
			if( SecBulls[i].NextInGroup ) fwrite( &SecBulls[ i ].NextInGroup->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SecBulls[i].PrevInGroup ) fwrite( &SecBulls[ i ].PrevInGroup->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Flags, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].SecType, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].MoveType, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].State, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].OwnerType, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Owner, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].ID, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Weapon, sizeof( int8_t ), 1, fp );
			fwrite( &SecBulls[ i ].Lensflare, sizeof( bool ), 1, fp );
			fwrite( &SecBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].LifeSpan, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].SpeedInc, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].SpeedWanted, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].DropCount, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].DropVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Size, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartDir, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartMat, sizeof( MATRIX ), 1, fp );
			fwrite( &SecBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ColFlag, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].ColGroup, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].ColPoint, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fwrite( &SecBulls[ i ].GroupImIn, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].ModelNum, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].ModelIndex, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &SecBulls[ i ].fmpoly, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].numfmpolys, sizeof( int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].poly, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].numpolys, sizeof( int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].xsize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ysize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].light, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].r, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].g, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].b, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].TurnSpeed, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ViewCone, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].TargetType, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].Target, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fwrite( &SecBulls[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Shield, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Damage, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Ammo, sizeof( u_int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].NumBounces, sizeof( int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].NumOldPos, sizeof( int16_t ), 1, fp );
			fwrite( &SecBulls[ i ].RetractPos, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Interval, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Time, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].OldPos[ 0 ], sizeof( SecBulls[ i ].OldPos ), 1, fp );
			i = SecBulls[ i ].Prev;
		}

		i = FirstSecBullFree;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &SecBulls[ i ].Next, sizeof( SecBulls[ i ].Next ), 1, fp );
			i = SecBulls[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load SecBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadSecBulls( FILE * fp )
{
	u_int16_t	i;
	u_int16_t	TempIndex = (u_int16_t) -1;

	if( fp )
	{
		for( i = 0; i < MAXSECONDARYWEAPONS; i++ )
		{
			fread( &SecondaryWeaponsGot[ i ], sizeof( int16_t ), 1, fp );
			fread( &SecAmmoUsed[ i ], sizeof( int16_t ), 1, fp );
			fread( &SecondaryAmmo[ i ], sizeof( int16_t ), 1, fp );
		}

		for( i = 0; i < MAXPICKUPS; i++ )
		{
			fread( &SecWeaponsGot[ i ], sizeof( int8_t ), 1, fp );
		}

		for( i = 0; i < MAXGROUPS; i++ )
		{
			fread( &NumSecBullsPerGroup[ i ], sizeof( int16_t ), 1, fp );
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SecBullGroups[ i ] = &SecBulls[ TempIndex ];
			else SecBullGroups[ i ] = NULL;
		}


		fread( &FirstSecBullUsed, sizeof( FirstSecBullUsed ), 1, fp );
		fread( &FirstSecBullFree, sizeof( FirstSecBullFree ), 1, fp );
		fread( &SecondaryFireDelay, sizeof( SecondaryFireDelay ), 1, fp );
		fread( &TargetComputerOn, sizeof( TargetComputerOn ), 1, fp );
		fread( &ImTargeted, sizeof( ImTargeted ), 1, fp );
		fread( &TargetedDelay, sizeof( TargetedDelay ), 1, fp );
		fread( &GlobalSecBullsID, sizeof( GlobalSecBullsID ), 1, fp );

		i = FirstSecBullUsed;

		while( i != (u_int16_t) -1 )
		{
			fread( &SecBulls[ i ].Used, sizeof( bool ), 1, fp );
			fread( &SecBulls[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Prev, sizeof( u_int16_t ), 1, fp );

			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SecBulls[ i ].NextInGroup = &SecBulls[ TempIndex ];
			else SecBulls[ i ].NextInGroup = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SecBulls[ i ].PrevInGroup = &SecBulls[ TempIndex ];
			else SecBulls[ i ].PrevInGroup = NULL;

			fread( &SecBulls[ i ].Flags, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].SecType, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].MoveType, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].State, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].OwnerType, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Owner, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].ID, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Weapon, sizeof( int8_t ), 1, fp );
			fread( &SecBulls[ i ].Lensflare, sizeof( bool ), 1, fp );
			fread( &SecBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].LifeSpan, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].SpeedInc, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].SpeedWanted, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Speed, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].DropCount, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].DropVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Size, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartDir, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartMat, sizeof( MATRIX ), 1, fp );
			fread( &SecBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ColFlag, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].ColGroup, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].ColPoint, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fread( &SecBulls[ i ].GroupImIn, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].ModelNum, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].ModelIndex, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &SecBulls[ i ].fmpoly, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].numfmpolys, sizeof( int16_t ), 1, fp );
			fread( &SecBulls[ i ].poly, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].numpolys, sizeof( int16_t ), 1, fp );
			fread( &SecBulls[ i ].xsize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ysize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].light, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].r, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].g, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].b, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].TurnSpeed, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ViewCone, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].TargetType, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].Target, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fread( &SecBulls[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Shield, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Damage, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Ammo, sizeof( u_int16_t ), 1, fp );
			fread( &SecBulls[ i ].NumBounces, sizeof( int16_t ), 1, fp );
			fread( &SecBulls[ i ].NumOldPos, sizeof( int16_t ), 1, fp );
			fread( &SecBulls[ i ].RetractPos, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Interval, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Time, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].OldPos[ 0 ], sizeof( SecBulls[ i ].OldPos ), 1, fp );
			i = SecBulls[ i ].Prev;
		}

		i = FirstSecBullFree;

		while( i != (u_int16_t) -1 )
		{
			SecBulls[ i ].Used = false;
			SecBulls[ i ].Prev = (u_int16_t) -1;
			SecBulls[ i ].NextInGroup = NULL;
			SecBulls[ i ].PrevInGroup = NULL;
			SecBulls[ i ].State = MIS_STRAIGHT;
			SecBulls[ i ].Flags = SECFLAGS_Nothing;
			SecBulls[ i ].Index = i;
			SecBulls[ i ].Type = (u_int16_t) -1;
			SecBulls[ i ].SecType = SEC_MISSILE;
			SecBulls[ i ].DropCount = 0.0F;
			SecBulls[ i ].MoveType = MISMOVE_STRAIGHT;
			SecBulls[ i ].Owner = (u_int16_t) -1;
			SecBulls[ i ].LifeCount = 0.0F;
			SecBulls[ i ].ColFlag = 0;
			SecBulls[ i ].GroupImIn = (u_int16_t) -1;
			SecBulls[ i ].ModelNum = (u_int16_t) -1;
			SecBulls[ i ].ModelIndex = (u_int16_t) -1;
			SecBulls[ i ].fmpoly = (u_int16_t) -1;
			SecBulls[ i ].numfmpolys = 0;
			SecBulls[ i ].poly = (u_int16_t) -1;
			SecBulls[ i ].numpolys = 0;
			SecBulls[ i ].light = (u_int16_t) -1;
			SecBulls[ i ].Target = (u_int16_t) -1;
			SecBulls[ i ].TargetType = (u_int16_t) -1;
			SecBulls[ i ].SpeedWanted = 32.0F;
			SecBulls[ i ].SpeedInc = 32.0F;
			SecBulls[ i ].Speed = 32.0F;
			SecBulls[ i ].TurnSpeed = 0.0F;
			SecBulls[ i ].ViewCone = 0.0F;
			SecBulls[ i ].DirVector = Forward;
			SecBulls[ i ].UpVector = SlideUp;
			SecBulls[ i ].DropVector = SlideDown;
			SecBulls[ i ].NumOldPos = 0;
			QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &SecBulls[ i ].DirVector );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );

			fread( &SecBulls[ i ].Next, sizeof( SecBulls[ i ].Next ), 1, fp );
			i = SecBulls[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Get Next Valid Secondary Weapon
	Input		:	Nothing
	Output		:	u_int8_t	Primary Weapon
===================================================================*/
u_int8_t GetNextValidSeconadaryWeapon( void )
{
	int8_t	TempWeapon;

	TempWeapon = Ships[ WhoIAm ].Secondary;

	TempWeapon++;
	if( TempWeapon >= MAX_SECONDARY_WEAPONS ) TempWeapon = 0;

	while( !SecondaryWeaponsGot[ TempWeapon ] && ( TempWeapon != Ships[ WhoIAm ].Secondary ) )
	{
		TempWeapon++;
		if( TempWeapon >= MAX_SECONDARY_WEAPONS ) TempWeapon = 0;
	}

	return( TempWeapon );
}

/*===================================================================
	Procedure	:	Get Prev Valid Secondary Weapon
	Input		:	Nothing
	Output		:	u_int8_t	Primary Weapon
===================================================================*/
u_int8_t GetPrevValidSecondaryWeapon( void )
{
	int8_t	TempWeapon;

	TempWeapon = Ships[ WhoIAm ].Secondary;

	if( TempWeapon == 0 ) TempWeapon = ( MAX_SECONDARY_WEAPONS - 1 );
	else TempWeapon--;

	while( !SecondaryWeaponsGot[ TempWeapon ] && ( TempWeapon != Ships[ WhoIAm ].Secondary ) )
	{
		if( TempWeapon == 0 ) TempWeapon = ( MAX_SECONDARY_WEAPONS - 1 );
		else TempWeapon--;
	}

	return( TempWeapon );
}

/*===================================================================
	Procedure	:	Conv Secondary Weapon number from cheat to norm
	Input		:	int8_t	Secondary Weapon
	Output		:	u_int8_t	Secondary Weapon ( Valid One )
===================================================================*/
int8_t ConvSecToNormWeapon( int8_t Secondary )
{
	int8_t	NewSecondary;

	NewSecondary = Secondary;

	switch( Secondary )
	{
		case ENEMYSPIRALMISSILE:
			NewSecondary = MUGMISSILE;
			break;
		case ENEMYFIREBALL:
			NewSecondary = MUGMISSILE;
			break;
		case ENEMYHOMINGMISSILE:
			NewSecondary = SOLARISMISSILE;
			break;
		case ENEMYBLUEHOMINGMISSILE:
			NewSecondary = SOLARISMISSILE;
			break;

		default:
			break;
	}

	return( NewSecondary );
}

/*===================================================================
	Procedure	:	Toggle Secondary Weapon Type
	Input		:	int8_t	Secondary Weapon
	Output		:	Nothing
===================================================================*/
void ToggleSecondaryWeapons( int8_t Secondary )
{
	switch( SecondaryToFireLookup[ Secondary ] )
	{
		case MUGMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYSPIRALMISSILE;
			break;
		case ENEMYSPIRALMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYFIREBALL;
			break;
		case ENEMYFIREBALL:
			SecondaryToFireLookup[ Secondary ] = MUGMISSILE;
			break;
		case SOLARISMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYHOMINGMISSILE;
			break;
		case ENEMYHOMINGMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYBLUEHOMINGMISSILE;
			break;
		case ENEMYBLUEHOMINGMISSILE:
			SecondaryToFireLookup[ Secondary ] = SOLARISMISSILE;
			break;
		default:
			break;
	}
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
