/*===================================================================
*	P i c k u p s . c
*	All routines to do with pickup objects.....
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"

#include "2dtextures.h"
#include "mload.h"

#include "primary.h"
#include "secondary.h"

#include "d3dmain.h"
#include <math.h>
#include <time.h>
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dpolys.h"
#include "quat.h"
#include "models.h"
#include "sphere.h"
#include "controls.h"
#include "visi.h"
#include "sfx.h"
#include <malloc.h>

#include "triggers.h"
#include "pickups.h"
#include "lines.h"
#include "text.h"
#include "screenpolys.h"
#include "magic.h"
#include "goal.h"
#include "local.h"
#include "XMem.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Defines
===================================================================*/

#define	PIC_VERSION_NUMBER	1
#define HOST_BOUNTY_TIME	( 10 * ANIM_SECOND )
#define HOST_FLAG_TIME		( 10 * ANIM_SECOND )


/*===================================================================
	Externs
===================================================================*/
extern SLIDER BikeCompSpeechSlider;
extern BOOL CTF;
extern BOOL CanCarryOwnFlag;
extern BOOL OwnFlagTeleportsHome;
extern uint32 TeamFlagMask[ MAX_TEAMS ];
extern BOOL TeamFlagAtHome[ MAX_TEAMS ];

extern float BountyTime;

extern BOOL BountyHunt;
extern BOOL CaptureTheFlag;
extern BOOL ShieldCritical;

extern	BYTE	TeamNumber[MAX_PLAYERS];
extern char *TeamName[ MAX_TEAMS ];

extern	VECTOR			Forward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	XLIGHT			XLights[ MAXXLIGHTS ];
extern	FMPOLY			FmPolys[ MAXNUMOF2DPOLYS ];
extern	MODEL			Models[ MAXNUMOFMODELS ];
extern	float			framelag;
extern	GLOBALSHIP		Ships[ MAX_PLAYERS ];
extern	BYTE			WhoIAm;
extern	LINE			Lines[ MAXLINES ];
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	FRAME_INFO	*	Flare_Header;
extern	FRAME_INFO	*	Circle_Header;
extern	FRAME_INFO	*	PickupRegen_Header;
extern	BYTE			Current_Camera_View;

extern	BOOL			JustPickedUpShield;

extern	int16			LevelNum;
extern	char			LevelNames[MAXLEVELS][128];

extern	int16			PrimaryWeaponsGot[ MAXPRIMARYWEAPONS ];
extern	int16			SecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
extern	float			GeneralAmmo;
extern	float			PyroliteAmmo;
extern	float			SussGunAmmo;
extern	float			GeneralAmmo;

extern	float			GeneralAmmoUsed;
extern	float			PyroliteAmmoUsed;
extern	float			SussGunAmmoUsed;
extern	int16			SecondaryAmmo[ MAXSECONDARYWEAPONS ];
extern	int16			SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	USERCONFIG *	player_config;
extern	PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern	SECONDARYWEAPONATTRIB SecondaryWeaponAttribs[ TOTALSECONDARYWEAPONS ];
extern	int				no_collision;		// disables player ship-to-background collisions
extern	uint16			IsGroupVisible[MAXGROUPS];
extern	BOOL			IsHost;

extern	MISSEDPICKUPMSG	MissedPickups[ MAXMISSEDPICKUPS ];
extern	SHORTPICKUP		MissedInitPickups[ MAXMISSEDPICKUPS ];
extern	SHORTKILLPICKUP	MissedKillPickups[ MAXMISSEDPICKUPS ];
extern	int16			NumMissedPickups;
extern	int16			NumMissedInitPickups;
extern	int16			NumMissedKillPickups;

extern	float			NitroFuel;
extern	float			NitroFuelUsed;

extern	SHORTNAMETYPE	Names;	// all the players short Names....
extern	BOOL            bSoundEnabled;

extern	int16			NumOfActivePlayers;

extern	BYTE			GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
extern	BOOL			DebugInfo;

extern	BOOL			PickupLightDetail;
extern	BOOL			PrimaryLightDetail;
extern	BOOL			SecondaryLightDetail;
extern	TRIGGERMOD	*	TrigMods;
extern	int				NumOfTrigMods;
extern	BOOL			TargetComputerOn;

extern	BYTE			CopyOfOrbitals;
extern	float			CopyOfOrbAmmo[ MAXMULTIPLES ];
extern	int16			CopyOfPowerLevel;
extern	float			CopyOfNitroFuel;
extern	float			CopyOfGeneralAmmo;
extern	float			CopyOfSussGunAmmo;
extern	float			CopyOfPyroliteAmmo;
extern	int				outside_group;
extern	BOOL			PlayDemo;
extern	int16			Lives;
extern	int				CrystalsFound;
extern	BYTE			ChangeLevel_MyGameStatus;
extern	float			LevelTimeTaken;

extern	uint16			Seed1;
extern	uint16			Seed2;
extern	uint16			FirstSecBullUsed;
extern	SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	MODELNAME		ModelNames[MAXMODELHEADERS];

extern BOOL	NeedFlagAtHome;
extern int GoalScore;

extern	char PrimaryNames[7][16];

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;
extern int PickupMessageColour;

/*===================================================================
	Globals
===================================================================*/
BOOL			RandomPickups;
BOOL			MyRandomPickups;
REGENPOINT	*	RegenPoints = NULL;
int16			NumRegenPoints = 0;
int16			NumPickupsPerGroup[ MAXGROUPS ];
int16			NumPickupType[ MAXPICKUPTYPES ];
int16			MaxPickupType[ MAXPICKUPTYPES ];
PICKUP	*		PickupGroups[ MAXGROUPS ];
PICKUP			Pickups[ MAXPICKUPS ];
PICKUP			PickupsCopy[ MAX_PLAYERS ][ MAXPICKUPS ];
REGENPOINT	*	RegenSlotsCopy[ MAX_PLAYERS ];
uint16			FirstPickupUsed;
uint16			FirstPickupFree;
char			UserMessage[ 256 ];
int16			NumStealths = 0;
int16			NumInvuls = 0;
int16			NumSuperNashrams = 0;
int16			NumOrbs = 0;
int16			NumPowerPods = 0;
int16			NumPrimWeapons[ MAXPRIMARYWEAPONS ] = { 0, 0, 0, 0, 0, 0 };
BOOL			PickupInvulnerability = FALSE;
int16			NumGoldBars = 0;
int16			PickupsGot[ MAXPICKUPTYPES ];
BOOL			MyPickupValid[ MAXPICKUPTYPES ];
BOOL			PickupValid[ MAXPICKUPTYPES ];
int				FlagsToGenerate = 0;
int				BountyToGenerate = 0;
int8			TeamFlagPickup[ MAX_TEAMS ] = {
	PICKUP_Flag1,
	PICKUP_Flag2,
	PICKUP_Flag3,
	PICKUP_Flag4,
};

int16	Host_PrimaryWeaponsGot[ MAX_PLAYERS ][ MAXPRIMARYWEAPONS ];
int16	Host_SecondaryWeaponsGot[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
float	Host_GeneralAmmo[ MAX_PLAYERS ];
float	Host_PyroliteAmmo[ MAX_PLAYERS ];
float	Host_SussGunAmmo[ MAX_PLAYERS ];
float	Host_GeneralAmmoUsed;
float	Host_PyroliteAmmoUsed;
float	Host_SussGunAmmoUsed;
int16	Host_SecondaryAmmo[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
int16	Host_SecAmmoUsed[ MAXSECONDARYWEAPONS ];
int8	Host_SecWeaponsGot[ MAX_PLAYERS ][ MAXPICKUPS ];
float	Host_NitroFuel[ MAX_PLAYERS ];
float	Host_NitroFuelUsed;
BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
float	Host_OrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
int16	Host_PowerLevel[ MAX_PLAYERS ];
BYTE	Host_CopyOfNumOfOrbitals[ MAX_PLAYERS ];
float	Host_CopyOfOrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
int16	Host_CopyOfPowerLevel[ MAX_PLAYERS ];
float	Host_CopyOfNitroFuel[ MAX_PLAYERS ];
float	Host_CopyOfGeneralAmmo[ MAX_PLAYERS ];
float	Host_CopyOfSussGunAmmo[ MAX_PLAYERS ];
float	Host_CopyOfPyroliteAmmo[ MAX_PLAYERS ];
int16	Host_PickupsGot[ MAX_PLAYERS ][ MAXPICKUPTYPES ];
uint32  Host_Flags[ MAX_PLAYERS ];
float   Host_BountyTimer;
float   Host_FlagTimer;
float   Host_TeamFlagTimer[ MAX_TEAMS ];
extern int FlagsToGenerate;
extern int GoalTeamMembers[ MAX_TEAMS ];

int16	NumPrimaryPickups = 1;

char			CTFMessage[ MAXTEXTMSG ];

char	*	Messages[] = {

			LT_PICKUP_TROJAX				,//"Trojax",						// Primary Weapons
			LT_PICKUP_PYROLITE				,//"Pyrolite Rifle",
			LT_PICKUP_TRANSPULSE			,//"Transpulse Cannon",
			LT_PICKUP_SUSSGUN				,//"Suss-Gun",
			LT_PICKUP_LASER					,//"Beam Laser",
			LT_PICKUP_MUG					,//"Mug Missile",					// Secondary Weapons
			LT_PICKUP_MUG_PACK				,//"Mug Missiles",
			LT_PICKUP_SOLARIS				,//"Heatseeker Missile",
			LT_PICKUP_SOLARIS_PACK			,//"Heatseeker Missile Pack",
			LT_PICKUP_THIEF					,//"Thief Missile",
			LT_PICKUP_SCATTER				,//"Scatter Missile",
			LT_PICKUP_GRAVGON				,//"Gravgon Missile",
			LT_PICKUP_MFRL					,//"Rocket Launcher",
			LT_PICKUP_TITAN					,//"TitanStar Missile",
			LT_PICKUP_PURGE					,//"Purge Mine Pack",
			LT_PICKUP_PINE					,//"Pine Mine Pack",
			LT_PICKUP_QUANTUM				,//"Quantum Mine Pack",
			LT_PICKUP_SPIDER				,//"Spider Mine Pack",
			LT_PICKUP_PARASITE				,//"Parasite Mine",
			LT_PICKUP_FLARE					,//"Flare",
			LT_PICKUP_GENAMMO				,//"General Ammo",					// Ammo
			LT_PICKUP_PYROFUEL				,//"Pyrolite Fuel",
			LT_PICKUP_SUSSAMMO				,//"Suss-Gun Ammo",
			LT_PICKUP_POWER_POD				,//"Power Pod",					// Extras
			LT_PICKUP_SHIELD				,//"Shield",
			LT_PICKUP_INVULBERABILITY		,//"Invulnerability",
			LT_PICKUP_EXTRA_LIFE			,//"Extra Life",
			LT_PICKUP_TARGETING_COMPUTER	,//"Targeting Computer",
			LT_PICKUP_SMOKE					,//"Smoke Streamer",
			LT_PICKUP_NITRO					,//"Nitro",
			LT_PICKUP_GOGGLES				,//"Goggles",
			LT_PICKUP_GOLD_BARS				,//"Gold Bars",
			LT_PICKUP_STEALTH				,//"Stealth Mantle",
			LT_PICKUP_CRYSTAL				,//"Crystal",
			LT_PICKUP_ORBITAL				,//"Orbit Pulsar",
			LT_PICKUP_GOLDEN_POWER_POD		,//"Golden Power Pod",
			LT_PICKUP_DNA					,//"DNA",
			"",														// skeleton key
			LT_PICKUP_BOMB					,//"Bomb",
			LT_PICKUP_GOLDEN_IDOL			,//"Golden Idol",
			"",								// flag
			"",
			"",								// flag 1
			"",								// flag 2
			"",								// flag 3
			"",								// flag 4
};

QUEDPICKUP		QuedPickups[ MAXPICKUPS ];
QUEDPICKUP	*	FirstQuedPickupUsed = NULL;
QUEDPICKUP	*	FirstQuedPickupFree = NULL;
QUEDPICKUP	*	FirstQuedPickupToProcess = NULL;

PICKUPATTRIB PickupAttribs[ MAXPICKUPTYPES ] = {

/*===================================================================
	0 Trojax
===================================================================*/
	{
		MODEL_Trojax,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;	   	// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	1 Pyrolite
===================================================================*/
	{
		MODEL_Pyrolite,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	2 Transpulse
===================================================================*/
	{
		MODEL_Transpulse,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	3 Sussgun
===================================================================*/
	{
		MODEL_SussGun,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	4 Beam Laser
===================================================================*/
	{
		MODEL_Laser,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},

/*===================================================================
	5 Mug Missile
===================================================================*/
	{
		MODEL_Mug,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 128.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	6 Mug Missiles
===================================================================*/
	{
		MODEL_Mugs,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	7 Solaris Heatseaker
===================================================================*/
	{
		MODEL_Heatseaker,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	8 Heatseaker Pickup
===================================================================*/
	{
		MODEL_HeatseakerPickup,		// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	9 Thief Missile
===================================================================*/
	{
//		MODEL_Thief,				// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	10 Scatter Missile
===================================================================*/
	{
		MODEL_Scatter,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	11 Gravgon Missile
===================================================================*/
	{
		MODEL_Gravgon,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	12 Rocket Launcher
===================================================================*/
	{
		MODEL_Launcher,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	13 TitanStar Missile
===================================================================*/
	{
		MODEL_TitanStar,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	14 Purge Mine Pickup
===================================================================*/
	{
		MODEL_PurgePickup,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	15 Pine Mine Pickup
===================================================================*/
	{
		MODEL_PinePickup,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	16 Quantum Mine Pickup
===================================================================*/
	{
		MODEL_QuantumPickup,		// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	17 Spider Pod New
===================================================================*/
	{
//		MODEL_SpiderPod,			// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	18 Parasite Mine
===================================================================*/
	{
//		MODEL_Parasite,				// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 128.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	19 Flare
===================================================================*/
	{
//		MODEL_Flare,				// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},

/*===================================================================
	20 General Ammo
===================================================================*/
	{
		MODEL_GeneralAmmo,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	21 Pyrolite Fuel
===================================================================*/
	{
		MODEL_PyroliteAmmo,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	22 SussGun Ammo
===================================================================*/
	{
		MODEL_SussGunAmmo,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},


/*===================================================================
	23 Power Pod
===================================================================*/
	{
		MODEL_PowerPod,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
	    COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	24 Shield
===================================================================*/
	{
		MODEL_Shield,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	25 Invulnerability
===================================================================*/
	{
		MODEL_Inv,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	26 Extra Life
===================================================================*/
	{
		MODEL_ExtraLife,			// uint16	ModelType;	// if 3D model which one....
		0, //1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	27 Targeting Computer
===================================================================*/
	{
		MODEL_Computer,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	28 Smoke Streamer
===================================================================*/
	{
//		MODEL_Smoke,				// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	29 Nitro
===================================================================*/
	{
		MODEL_Nitro,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	30 Goggles
===================================================================*/
	{
//		MODEL_Goggles,				// uint16	ModelType;	// if 3D model which one....
		MODEL_Eyeball,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	31 Gold Bars
===================================================================*/
	{
		MODEL_Gold,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	32 Mantle ( Cloaking Device )
===================================================================*/
	{
		MODEL_Mantle,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	33 Crystal
===================================================================*/
	{
		MODEL_Crystal,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	34 Orb
===================================================================*/
	{
		MODEL_Orb,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},

/*===================================================================
	35 Golden Power Pod
===================================================================*/
	{
		MODEL_GoldenPowerPod,		// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 0.0F			// float	b;			// what color does it emit....
	},
/*===================================================================
	36 DNA Pickup
===================================================================*/
	{
		MODEL_DNA,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 255.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	37 Skeleton Key Pickup
===================================================================*/
	{
		MODEL_SkeletonKey,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 0.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 0.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 128.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	38 Bomb Pickup
===================================================================*/
	{
		MODEL_Bomb,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 64.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 64.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	39 Gold Figure
===================================================================*/
	{
		MODEL_GoldFigure,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 192.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	40 Flag
===================================================================*/
	{
		MODEL_Flag,					// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 192.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 128.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	41 Bounty
===================================================================*/
	{
		MODEL_Gold	,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 192.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 192.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	42 Flag1 (red)
===================================================================*/
	{
		MODEL_RedFlag,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 192.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 64.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	43 Flag2 (green)
===================================================================*/
	{
		MODEL_GreenFlag,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 64.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 192.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	44 Flag3 (blue)
===================================================================*/
	{
		MODEL_BlueFlag,				// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 64.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 64.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 192.0F		// float	b;			// what color does it emit....
	},
/*===================================================================
	45 Flag4 (yellow)
===================================================================*/
	{
		MODEL_YellowFlag,			// uint16	ModelType;	// if 3D model which one....
		1,							// uint16	light;		// do I produce Light
		PICKUPLIGHT_RADIUS,			// float	lightsize;	// how big is the light
		COLOUR_SCALE * 192.0F,		// float	r;			// what color does it emit....
		COLOUR_SCALE * 192.0F,		// float	g;			// what color does it emit....
		COLOUR_SCALE * 64.0F		// float	b;			// what color does it emit....
	},
};

/*===================================================================
	Procedure	:	Set up And Init all Pickups
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void InitPickups( void )
{
	uint16		i;

	InitFailedKillSlots();

	FirstPickupUsed = (uint16) -1;
	FirstPickupFree = 0;

	SetupPickupGroups();
	ClearPickupsGot();

	for( i = 0; i < MAXPICKUPS; i++ )
	{
		memset( &Pickups[ i ], 0, sizeof( PICKUP ) );
		Pickups[ i ].Next = i + 1;
		Pickups[ i ].Prev = (uint16) -1;

		Pickups[ i ].NextInGroup = NULL;
		Pickups[ i ].PrevInGroup = NULL;

		Pickups[ i ].Index = i;
		Pickups[ i ].LifeCount = -1.0F;
		Pickups[ i ].Mode = PICKUPMODE_Normal;
		Pickups[ i ].Type = (uint16) -1;
		Pickups[ i ].Rot.x = -1.2F;
		Pickups[ i ].Rot.y = 2.0F;
		Pickups[ i ].Rot.z = 1.0F;
		Pickups[ i ].Group = 0;
		Pickups[ i ].DirVector = Forward;
		Pickups[ i ].UpVector = SlideUp;
		Pickups[ i ].RegenSlot = -1;
		Pickups[ i ].TriggerModPtr = NULL;
		Pickups[ i ].TriggerMod = (uint16) -1;
		QuatFrom2Vectors( &Pickups[ i ].DirQuat, &Forward, &Pickups[ i ].DirVector );
		QuatToMatrix( &Pickups[ i ].DirQuat, &Pickups[ i ].Mat );
	}
	Pickups[ MAXPICKUPS-1 ].Next = (uint16) -1;
}

#if 0
/*===================================================================
	Table info for new pickup code
===================================================================*/
REGENPICKUPINFO	RegenPickupInfo[ MAXPICKUPTYPES ] = {

	{ 1,	0 },				// 0  PICKUP_Trojax
	{ 1,	0 },				// 1  PICKUP_Pyrolite
	{ 1,	0 },				// 2  PICKUP_Transpulse
	{ 1,	0 },				// 3  PICKUP_SussGun
	{ 1,	0 },				// 4  PICKUP_Laser
	{ 1,	0 },				// 5  PICKUP_Mug
	{ 3,	0 },				// 6  PICKUP_Mugs
	{ 1,	0 },				// 7  PICKUP_Heatseaker
	{ 3,	0 },				// 8  PICKUP_HeatseakerPickup
	{ 1,	0 },				// 9  PICKUP_Thief
	{ 1,	0 },				// 10 PICKUP_Scatter
	{ 1,	0 },				// 11 PICKUP_Gravgon
	{ 50,	0 },				// 12 PICKUP_Launcher
	{ 1,	0 },				// 13 PICKUP_TitanStar
	{ 3,	0 },				// 14 PICKUP_PurgePickup
	{ 3,	0 },				// 15 PICKUP_PinePickup
	{ 1,	0 },				// 16 PICKUP_QuantumPickup
	{ 1,	0 },				// 17 PICKUP_SpiderPod
	{ 1,	0 },				// 18 PICKUP_Parasite
	{ 1,	0 },				// 19 PICKUP_Flare
	{ 1,	0 },				// 20 PICKUP_GeneralAmmo
	{ 1,	0 },				// 21 PICKUP_PyroliteAmmo
	{ 1,	0 },				// 22 PICKUP_SussGunAmmo
	{ 1,	0 },				// 23 PICKUP_PowerPod
	{ 1,	0 },				// 24 PICKUP_Shield
	{ 1,	0 },				// 25 PICKUP_Inv
	{ 1,	0 },				// 26 PICKUP_ExtraLife
	{ 1,	0 },				// 27 PICKUP_Computer
	{ 1,	0 },				// 28 PICKUP_Smoke
	{ 1,	0 },				// 29 PICKUP_Nitro
	{ 1,	0 },				// 30 PICKUP_Goggles
	{ 1,	0 },				// 31 PICKUP_Gold
	{ 1,	0 },				// 32 PICKUP_Mantle
	{ 1,	0 },				// 33 PICKUP_Crystal
	{ 1,	0 },				// 34 PICKUP_Orb
	{ 1,	0 },				// 35 PICKUP_GoldenPowerPod
	{ 1,	0 },				// 36 PICKUP_DNA
	{ 1,	0 },				// 37 PICKUP_SkeletonKey
	{ 1,	0 },				// 38 PICKUP_Bomb
	{ 1,	0 },				// 39 PICKUP_GoldFigure
	{ 1,	0 },				// 40 PICKUP_Flag
	{ 1,	0 },				// 41 PICKUP_Bounty
	{ 1,	0 },				// 42 PICKUP_Flag1
	{ 1,	0 },				// 43 PICKUP_Flag2
	{ 1,	0 },				// 44 PICKUP_Flag3
	{ 1,	0 },				// 45 PICKUP_Flag4
};

int16 PrimWeaponPickups[ MAXPRIMARYWEAPONS ] = {

	-1,							// Pulsar
	PICKUP_Trojax,				// Trojax
	PICKUP_Pyrolite,			// Pyrolite
	PICKUP_Transpulse,			// Transpulse
	PICKUP_SussGun,				// SussGun
	PICKUP_Laser,				// Laser
};

int16 SecWeaponPickups[ MAXSECONDARYWEAPONS ] = {

	PICKUP_Mugs,				// Mug
	PICKUP_HeatseakerPickup,	// Solaris
	PICKUP_Thief,				// Thief
	PICKUP_Scatter,				// Scatter
	PICKUP_Gravgon,				// Gravgon
	PICKUP_Launcher,			// MFRL
	PICKUP_TitanStar,			// Titan
	PICKUP_PurgePickup,			// Purge Mine
	PICKUP_PinePickup,			// Pine Mine
	PICKUP_QuantumPickup,		// Quantum Mine
	PICKUP_SpiderPod,			// Spider Mine
};

/*===================================================================
	Procedure	:	Initialise Pickup Regeneration Que
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void InitQuedPickups( void )
{
	uint16	i;

	FirstQuedPickupUsed = NULL;
	FirstQuedPickupFree = &QuedPickups[ 0 ];

	for( i = 0; i < MAXPICKUPS; i++ )
	{
		memset( &QuedPickups[ i ], 0, sizeof( QUEDPICKUP ) );
		QuedPickups[ i ].Type -1;
		QuedPickups[ i ].NextUsed = NULL;
		QuedPickups[ i ].PrevUsed = NULL;
		QuedPickups[ i ].NextFree = &QuedPickups[ i + 1 ];
		QuedPickups[ i ].PrevFree = &QuedPickups[ i - 1 ];
	}

	QuedPickups[ 0 ].PrevFree = NULL;
	QuedPickups[ MAXPICKUPS - 1 ].NextFree = NULL;

	for( i = 0; i < MAXPICKUPTYPES; i++ )
	{
		RegenPickupInfo[ i ].Num = 0;
	}
}

/*===================================================================
	Procedure	:	Get free Slots
	Input		:	int16			Pickup Type
				:	int16			*	Num Random Slots ( TBFI )
				:	FREESLOTINFO	*	Random Slots Ptr ( TBFI )
				:	int16			*	Num Constant Slots ( TBFI )
				:	FREESLOTINFO	*	Constant Slots Ptr ( TBFI )
	Output		:	Nothing
===================================================================*/
void GetFreeSlots( int16 Type, int16 * NumRandomSlots, FREESLOTINFO * RandomSlots,
				   int16 * NumConstantSlots, FREESLOTINFO * ConstantSlots )
{
	int16	Slot;
	float	Distance;
	VECTOR	DistVector;

	*NumRandomSlots = 0;
	*NumConstantSlots = 0;

	for( Slot = 0; Slot < NumRegenPoints; Slot++ )
	{
		if( ( RegenPoints[ Slot ].Status == PU_REGENSTAT_Free ) &&
			( RegenPoints[ Slot ].Wait == 0.0F ) )
		{
			DistVector.x = ( RegenPoints[ Slot ].Pos.x - Ships[ WhoIAm ].Object.Pos.x );
			DistVector.y = ( RegenPoints[ Slot ].Pos.y - Ships[ WhoIAm ].Object.Pos.y );
			DistVector.z = ( RegenPoints[ Slot ].Pos.z - Ships[ WhoIAm ].Object.Pos.z );
			Distance = VectorLength( &DistVector );

			if( RegenPoints[ Slot ].RegenType == PU_REGENTYPE_Const )
			{
				if( RegenPoints[ Slot ].Type == Type )
				{
					ConstantSlots[ *NumConstantSlots ].Slot = Slot;
					ConstantSlots[ *NumConstantSlots ].Distance = Distance;
					*NumConstantSlots++;
				}
			}
			else
			{
				RandomSlots[ *NumRandomSlots ].Slot = Slot;
				RandomSlots[ *NumRandomSlots ].Distance = Distance;
				*NumRandomSlots++;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Add Primary Weapon To Regeneration Que
	Input		:	int16	Primary Weapon
	Output		:	BOOL	True/False
===================================================================*/
BOOL AddPrimaryToRegenQue( int8 Weapon )
{
	return( AddPickupToRegenQue( PrimWeaponPickups[ Weapon ] ) );
}

/*===================================================================
	Procedure	:	Add Secondary Weapon To Regeneration Que
	Input		:	int16	Primary Weapon
	Output		:	BOOL	True/False
===================================================================*/
BOOL AddSecondaryToRegenQue( int8 Weapon )
{
	return( AddPickupToRegenQue( SecWeaponPickups[ Weapon ] ) );
}

/*===================================================================
	Procedure	:	Add Pickup to end of regeneration que
	Input		:	int16	Type
	Output		:	BOOL	True/False
===================================================================*/
BOOL AddPickupToRegenQue( int16 Type )
{
	QUEDPICKUP	*	QuedPickup;

	if( Type == -1 ) return ( FALSE );

	RegenPickupInfo[ Type ].Num++;

	if( RegenPickupInfo[ Type ].Num < RegenPickupInfo[ Type ].Required )
	{
		return( TRUE );
	}

	QuedPickup = FirstQuedPickupFree;

	if( QuedPickup != NULL )
	{
		RegenPickupInfo[ Type ].Num -= RegenPickupInfo[ Type ].Required;

		FirstQuedPickupFree = QuedPickup->NextFree;
		if( FirstQuedPickupFree != NULL )
		{
			FirstQuedPickupFree->PrevFree = NULL;
		}

		if( FirstQuedPickupUsed != NULL )
		{
			FirstQuedPickupUsed->PrevUsed = QuedPickup;
			QuedPickup->NextUsed = FirstQuedPickupUsed;
		}
		else
		{
			FirstQuedPickupToProcess = QuedPickup;
			QuedPickup->NextUsed = NULL;
		}

		QuedPickup->PrevUsed = NULL;
		FirstQuedPickupUsed = QuedPickup;
	}
	else
	{
		return( FALSE );
	}

	QuedPickup->Type = Type;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Remove Pickup from Regeneration Que
	Input		:	QUEDPICKUP	*	QuedPickup Ptr
	Output		:	Nothing
===================================================================*/
void FreeQuedPickup( QUEDPICKUP * QuedPickup )
{
	QUEDPICKUP	*	PrevQuedPickup;
	QUEDPICKUP	*	NextQuedPickup;

	if( QuedPickup != NULL )
	{
		if( FirstQuedPickupToProcess == QuedPickup )
		{
			FirstQuedPickupToProcess = FirstQuedPickupToProcess->PrevUsed;
		}

		if( QuedPickup == FirstQuedPickupUsed )
		{
			NextQuedPickup = FirstQuedPickupUsed->NextUsed;

			FirstQuedPickupUsed = NextQuedPickup;

			if( FirstQuedPickupUsed != NULL )
			{
				FirstQuedPickupUsed->PrevUsed = NULL;
			}
		}
		else
		{
			PrevQuedPickup = QuedPickup->PrevUsed;
			NextQuedPickup = QuedPickup->NextUsed;

			if( PrevQuedPickup != NULL )
			{
				PrevQuedPickup->NextUsed = NextQuedPickup;
			}

			if( NextQuedPickup != NULL )
			{
				NextQuedPickup->PrevUsed = PrevQuedPickup;
			}
		}

		if( FirstQuedPickupFree != NULL )
		{
			FirstQuedPickupFree->PrevFree = QuedPickup;
		}

		QuedPickup->NextFree = FirstQuedPickupFree;
		QuedPickup->PrevFree = NULL;
		FirstQuedPickupFree = QuedPickup;
	}
}

/*===================================================================
	Procedure	:	Find free Background Object
	Input		:	nothing
	Output		:	Nothing
===================================================================*/
void ProcessQuedPickups( void )
{
	int16			Slot;
	int16			Type;
	uint16			i;
	int16			NumRandomSlots;
	FREESLOTINFO	RandomSlots[ MAXPICKUPS ];
	int16			NumConstantSlots;
	FREESLOTINFO	ConstantSlots[ MAXPICKUPS ];

	if( FirstQuedPickupToProcess )
	{
		Type = FirstQuedPickupToProcess->Type;

		GetFreeSlots( Type, &NumRandomSlots, &RandomSlots[ 0 ], &NumConstantSlots, &ConstantSlots[ 0 ] );

		if( NumConstantSlots )
		{
			Slot = ConstantSlots[ Random_Range( NumConstantSlots ) ].Slot;

			i = InitOnePickup( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Forward, 0.0F, Type, WhoIAm, ++Ships[WhoIAm].PickupIdCount, Slot, TRUE, -1.0F, (uint16) -1 );

			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				DropPickupSend( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Forward, 0.0F, Type, Ships[WhoIAm].PickupIdCount, Slot, TRUE, -1.0F, (uint16) -1 );
				FreeQuedPickup( FirstQuedPickupToProcess );
			}
		}
		else
		{
			if( NumRandomSlots >= 5 )
			{
				Slot = RandomSlots[ Random_Range( NumRandomSlots ) ].Slot;
		
				i = InitOnePickup( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Forward, 0.0F, Type, WhoIAm, ++Ships[WhoIAm].PickupIdCount, Slot, TRUE, -1.0F, (uint16) -1 );

				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					DropPickupSend( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Forward, 0.0F, Type, Ships[WhoIAm].PickupIdCount, Slot, TRUE, -1.0F, (uint16) -1 );
					FreeQuedPickup( FirstQuedPickupToProcess );
				}
			}
		}
	}
}

#endif

/*===================================================================
	Procedure	:	Clear Pickups Got
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ClearPickupsGot( void )
{
	int16	Count;

	/* clear all pickups held */
	for( Count = 0; Count < MAXPICKUPTYPES; Count++ )
		PickupsGot[ Count ] = 0;

	/* clear all primaries */
	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
		NumPrimWeapons[ Count ] = 0;

	/* clear special pickups */
	NumStealths			= 0;
	NumInvuls			= 0;
	NumSuperNashrams	= 0;
	NumOrbs				= 0;
	NumPowerPods		= 0;

}

/*===================================================================
	Procedure	:	Collect Pickup from scene
	Input		:	uint16	Pickup to get
	Output		:	BOOL	True/False
===================================================================*/
BOOL CollectPickup( uint16 i )
{
	int16	PickupEnable = TRUE;
	float	Sfx_Volume = 1.0F;
	int16	Temp;
	float	TempFloat;

	uint8	Message[ 128 ];
	int16	MessageSFX = -1;
	int16	TriggeredSFX = -1;
	BOOL	ShowTextAnyway = FALSE;
	BOOL	Speech = TRUE;
	
	sprintf( &Message[0], "%s", Messages[ Pickups[i].Type ] );

	if( i != (uint16) -1 )
	{
		switch( Pickups[i].Type )
		{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Trojax: 
				if( !PrimaryWeaponsGot[ TROJAX ] )
				{
					PrimaryWeaponsGot[ TROJAX ] = 1;
					MessageSFX = SFX_Select_Trojax;
					TriggeredSFX = SFX_BIKER_BW;
					NewPrimaryWeapon( TROJAX );
				}
				else
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Pyrolite:

				if( !PrimaryWeaponsGot[ PYROLITE_RIFLE ] )
				{
					PrimaryWeaponsGot[ PYROLITE_RIFLE ] = 1;

					if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
					{
						CopyOfPyroliteAmmo = MAXPYROLITEAMMO;
					}
					else
					{
						PyroliteAmmo = MAXPYROLITEAMMO;
					}
					NewPrimaryWeapon( PYROLITE_RIFLE );
					MessageSFX = SFX_Select_Pyrolite;
				}
				else
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Transpulse:

				if( !PrimaryWeaponsGot[ TRANSPULSE_CANNON ] )
				{
					PrimaryWeaponsGot[ TRANSPULSE_CANNON ] = 1;
					NewPrimaryWeapon( TRANSPULSE_CANNON );
					MessageSFX = SFX_Select_Transpulse;
					TriggeredSFX = SFX_BIKER_BW;
				}
				else
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGun:
				if( !PrimaryWeaponsGot[ SUSS_GUN ] )
				{
					PrimaryWeaponsGot[ SUSS_GUN ] = 1;

					if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
					{
						CopyOfSussGunAmmo = MAXSUSSGUNAMMO;
					}
					else
					{
						SussGunAmmo = MAXSUSSGUNAMMO;
					}
					NewPrimaryWeapon( SUSS_GUN );
					MessageSFX = SFX_Select_SussGun;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Laser:

				if( !PrimaryWeaponsGot[ LASER ] )
				{
					PrimaryWeaponsGot[ LASER ] = 1;
					NewPrimaryWeapon( LASER );
					MessageSFX = SFX_Select_BeamLaser;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mug:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mugs:
				if( SecondaryAmmo[ MUGMISSILE ] < 10 )
				{
					SecondaryAmmo[ MUGMISSILE ] += 3;
					if( SecondaryAmmo[ MUGMISSILE ] > 10 )
					{
						Temp = ( SecondaryAmmo[ MUGMISSILE ] - 10 );
						SecAmmoUsed[ MUGMISSILE ] = Temp;
						SecondaryAmmo[ MUGMISSILE ] = 10;
					}
					NewSecondaryWeapon( MUGMISSILE );
					MessageSFX = SFX_Select_MugMissile;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Heatseaker:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_HeatseakerPickup:
				if( SecondaryAmmo[ SOLARISMISSILE ] < 10 )
				{
					SecondaryAmmo[ SOLARISMISSILE ] += 3;
					if( SecondaryAmmo[ SOLARISMISSILE ] > 10 )
					{
						Temp = ( SecondaryAmmo[ SOLARISMISSILE ] - 10 );
						SecAmmoUsed[ SOLARISMISSILE ] = Temp;
						SecondaryAmmo[ SOLARISMISSILE ] = 10;
					}
					NewSecondaryWeapon( SOLARISMISSILE );
					MessageSFX = SFX_Select_SolarisMissile;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Thief:

				if( !SecondaryAmmo[ THIEFMISSILE ] )
				{
					SecondaryAmmo[ THIEFMISSILE ] += 1;
					NewSecondaryWeapon( THIEFMISSILE );
//					MessageSFX = SFX_Select_ThiefMissile;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Scatter:

				if( SecondaryAmmo[ SCATTERMISSILE ] < 3 )
				{
					SecondaryAmmo[ SCATTERMISSILE ] += 1;
					NewSecondaryWeapon( SCATTERMISSILE );
					MessageSFX = SFX_Select_ScatterMissile;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gravgon:
				if( !SecondaryAmmo[ GRAVGONMISSILE ] )
				{
					SecondaryAmmo[ GRAVGONMISSILE ] += 1;
					NewSecondaryWeapon( GRAVGONMISSILE );
					MessageSFX = SFX_Select_GravgonMissile;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Launcher:
				if( SecondaryAmmo[ MULTIPLEMISSILE ] < 100 )
				{
					SecondaryAmmo[ MULTIPLEMISSILE ] += 50;
					if( SecondaryAmmo[ MULTIPLEMISSILE ] > 100 )
					{
						Temp = ( SecondaryAmmo[ MULTIPLEMISSILE ] - 100 );
						SecAmmoUsed[ MULTIPLEMISSILE ] = Temp;
						SecondaryAmmo[ MULTIPLEMISSILE ] = 100;
					}
					NewSecondaryWeapon( MULTIPLEMISSILE );
					MessageSFX = SFX_Select_MFRL;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_TitanStar:
				if( SecondaryAmmo[ TITANSTARMISSILE ] < 3 )
				{
					SecondaryAmmo[ TITANSTARMISSILE ] += 1;
					NewSecondaryWeapon( TITANSTARMISSILE );
					MessageSFX = SFX_Select_TitanStarMissile;
					TriggeredSFX = SFX_BIKER_BW;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PurgePickup:
				if( SecondaryAmmo[ PURGEMINE ] < 10 )
				{
					SecondaryAmmo[ PURGEMINE ] += 3;
					if( SecondaryAmmo[ PURGEMINE ] > 10 )
					{
						Temp = ( SecondaryAmmo[ PURGEMINE ] - 10 );
						SecAmmoUsed[ PURGEMINE ] = Temp;
						SecondaryAmmo[ PURGEMINE ] = 10;
					}
					NewSecondaryWeapon( PURGEMINE );
					MessageSFX = SFX_Select_PurgeMine;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PinePickup:

				if( SecondaryAmmo[ PINEMINE ] < 6 )
				{
					SecondaryAmmo[ PINEMINE ] += 3;
					if( SecondaryAmmo[ PINEMINE ] > 6 )
					{
						Temp = ( SecondaryAmmo[ PINEMINE ] - 6 );
						SecAmmoUsed[ PINEMINE ] = Temp;
						SecondaryAmmo[ PINEMINE ] = 6;
					}
					NewSecondaryWeapon( PINEMINE );
					MessageSFX = SFX_Select_PineMine;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_QuantumPickup:

				if( !SecondaryAmmo[ QUANTUMMINE ] )
				{
					SecondaryAmmo[ QUANTUMMINE ] += 1;
					NewSecondaryWeapon( QUANTUMMINE );
					MessageSFX = SFX_Select_QuantumMine;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SpiderPod:

				if( SecondaryAmmo[ SPIDERMINE ] < 6 )
				{
					SecondaryAmmo[ SPIDERMINE ] += 3;
					if( SecondaryAmmo[ SPIDERMINE ] > 6 )
					{
						Temp = ( SecondaryAmmo[ SPIDERMINE ] - 6 );
						SecAmmoUsed[ SPIDERMINE ] = Temp;
						SecondaryAmmo[ SPIDERMINE ] = 6;
					}
					NewSecondaryWeapon( SPIDERMINE );
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Parasite:

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flare:

				MessageSFX = SFX_BIKECOMP_FL;

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GeneralAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfGeneralAmmo < MAXGENERALAMMO )
					{
						CopyOfGeneralAmmo += GENERALAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( GeneralAmmo < MAXGENERALAMMO )
					{
						GeneralAmmo += GENERALAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PyroliteAmmo:

				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfPyroliteAmmo < MAXPYROLITEAMMO )
					{
						CopyOfPyroliteAmmo += PYROLITEAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( PyroliteAmmo < MAXPYROLITEAMMO )
					{
						PyroliteAmmo += PYROLITEAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGunAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfSussGunAmmo < MAXSUSSGUNAMMO )
					{
						CopyOfSussGunAmmo += SUSSGUNAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( SussGunAmmo < MAXSUSSGUNAMMO )
					{
						SussGunAmmo += SUSSGUNAMMOPERPOD;
						MessageSFX = SFX_Select_Ammo;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PowerPod:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfPowerLevel != ( MAXPOWERLEVELS - 1 ) )
					{
						CopyOfPowerLevel++;
						if( CopyOfPowerLevel == ( MAXPOWERLEVELS - 1 ) )
						{
							sprintf( &Message[ 0 ], FULL_POWER_LEVEL );
						}
						else
						{
							sprintf( &Message[ 0 ], POWER_POD_LEVEL, Ships[ WhoIAm ].Object.PowerLevel+1 );
						}
						ShowTextAnyway = TRUE;
						MessageSFX = SFX_Select_PowerPod;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX_POWER );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( Ships[ WhoIAm ].Object.PowerLevel != ( MAXPOWERLEVELS - 1 ) )
					{
						Ships[ WhoIAm ].Object.PowerLevel++;
						if( Ships[ WhoIAm ].Object.PowerLevel == ( MAXPOWERLEVELS - 1 ) )
						{
							sprintf( &Message[ 0 ], FULL_POWER_LEVEL );
						}
						else
						{
							sprintf( &Message[ 0 ], POWER_POD_LEVEL, Ships[ WhoIAm ].Object.PowerLevel+1 );
						}
						ShowTextAnyway = TRUE;
						MessageSFX = SFX_Select_PowerPod;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX_POWER );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldenPowerPod:

				if( !( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					GivemeSuperNashram();
					MessageSFX = SFX_Select_GoldenPowerPod;
				}
				else
				{
					sprintf( &Message[0], YOU_CANT_HANDLE_ANY_MORE );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Shield:
				if( Ships[ WhoIAm ].Object.Shield != MAX_SHIELD )
				{
					Ships[ WhoIAm ].Object.Shield += 32.0F;
					if ( Ships[ WhoIAm ].Object.Shield > SHIELD_CRITICAL_LEVEL )
						ShieldCritical = FALSE;
					if( Ships[ WhoIAm ].Object.Shield > MAX_SHIELD ) Ships[ WhoIAm ].Object.Shield = MAX_SHIELD;
					MessageSFX = SFX_Select_Shield;
					JustPickedUpShield = TRUE;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Inv:

				if( !Ships[ WhoIAm ].Invul )
				{
					Ships[ WhoIAm ].InvulTimer = INVULNERABILITY_TIME;
					Ships[ WhoIAm ].Invul = TRUE;			
					PickupInvulnerability = TRUE;
					MessageSFX = SFX_Select_Invul;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_ExtraLife:
				Lives += 1;
				MessageSFX = SFX_ResnicReanimator;
				PlaySfx( SFX_ExtraLife, 1.0F );
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Computer:

				TargetComputerOn = TRUE;

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Smoke:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Nitro:

				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfNitroFuel < MAX_NITRO_FUEL )
					{
						CopyOfNitroFuel += 50.0F;
						if( CopyOfNitroFuel > MAX_NITRO_FUEL )
						{
							TempFloat = ( CopyOfNitroFuel - MAX_NITRO_FUEL );
							NitroFuelUsed += TempFloat;
							CopyOfNitroFuel = MAX_NITRO_FUEL;
						}
						MessageSFX = SFX_Select_Nitro;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( NitroFuel < MAX_NITRO_FUEL )
					{
						NitroFuel += 50.0F;
						if( NitroFuel > MAX_NITRO_FUEL )
						{
							TempFloat = ( NitroFuel - MAX_NITRO_FUEL );
							NitroFuelUsed += TempFloat;
							NitroFuel = MAX_NITRO_FUEL;
						}
						MessageSFX = SFX_Select_Nitro;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Goggles:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gold:
				NumGoldBars ++;
				if( !( NumGoldBars % 10 ) )
				{
//					NumGoldBars = 0;
					Lives += 1;
					sprintf( &Message[0], YOUVE_EARNED_EXTRA_LIFE );
					PlaySfx( SFX_ExtraLife, 1.0F );
				}
				else
				{
					if( NumGoldBars == 1 ) sprintf( &Message[0], YOU_HAVE_ONE_GOLD_BAR, NumGoldBars );
					else sprintf( &Message[0], YOU_HAVE_GOLD_BARS, NumGoldBars );
				}
				MessageSFX = SFX_GoldBarPickup;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mantle:

				if( !( Ships[ WhoIAm ].Object.Flags & SHIP_Stealth ) )
				{
					Ships[ WhoIAm ].Object.Flags |= SHIP_Stealth;
					Ships[ WhoIAm ].StealthTime = ( 60.0F * ANIM_SECOND );
					MessageSFX = SFX_Cloaking;
					Speech = FALSE;
				}
				else
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}

				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Crystal:
				CrystalsFound += 1;
				if( CrystalsFound == 1 )
				{
					sprintf( &Message[0], YOU_HAVE_ONE_CRYSTAL, CrystalsFound );
				}
				else
				{
					sprintf( &Message[0], YOU_HAVE_CRYSTALS, CrystalsFound );
				}
				MessageSFX = SFX_PickupCrystal;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_DNA:

				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SkeletonKey:

				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bomb:

				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldFigure:

				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag:
				if ( CaptureTheFlag )
				{
					// now I have the flag....?
					Ships[ WhoIAm ].Object.Flags |= SHIP_CarryingFlag;
					AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
						TeamName[ TeamNumber[ WhoIAm ] ] );

					sprintf( CTFMessage, SOMEONE_HAS_GOT_THE_FLAG, Names[ WhoIAm ], TeamName[ TeamNumber[ WhoIAm ] ] );
					SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_CaptureFlagMessage, 0);
					
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bounty:
				if ( BountyHunt )
				{
					// now I have the bounty....?
					BountyTime = 0.0F;
					Ships[ WhoIAm ].Object.Flags |= SHIP_CarryingBounty;
					AddColourMessageToQue(FlagMessageColour, YOU_HAVE_GOT_THE_BOUNTY );
					
					SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_BountyMessage, 0);
					
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag1:
			case PICKUP_Flag2:
			case PICKUP_Flag3:
			case PICKUP_Flag4:
				if ( CTF )
				{
					int team;

					for ( team = 0; team < MAX_TEAMS; team++ )
					{
						if ( Pickups[i].Type == TeamFlagPickup[ team ] )
						{
							if ( team == TeamNumber[ WhoIAm ] )
							{
								if ( TeamFlagAtHome[ team ] )
								{
									PickupEnable = FALSE;
								}
								else if ( OwnFlagTeleportsHome )
								{
									GOAL *goal;

									// teleport flag back to own goal
									PickupEnable = FALSE;
									goal = TeamGoal( team );
									if ( goal )
									{
										VECTOR Dir = { 0.0F, 0.0F, 0.0F };

										KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
										KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
										GenerateFlagAtHome( team );
										AddColourMessageToQue(FlagMessageColour, TEAM_FLAG_RETURNED,
											TeamName[ TeamNumber[ WhoIAm ] ] );
										SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturnedFlag, 0);
									}
								}
								else if ( CanCarryOwnFlag )
								{
									Ships[ WhoIAm ].Object.Flags |= TeamFlagMask[ team ];
									AddColourMessageToQue(FlagMessageColour, RETURN_TEAM_FLAG,
										TeamName[ TeamNumber[ WhoIAm ] ] );
									SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturningFlag, 0);
								}
								else
								{
									AddColourMessageToQue(FlagMessageColour, CANNOT_PICKUP_OWN_FLAG );
									PickupEnable = FALSE;
								}
							}
							else
							{
								// now I have the flag....?
								Ships[ WhoIAm ].Object.Flags |= TeamFlagMask[ team ];
								AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
									TeamName[ TeamNumber[ WhoIAm ] ] );
								
								sprintf( CTFMessage, SOMEONE_HAS_OTHER_TEAM_FLAG,
									Names[ WhoIAm ],
									TeamName[ TeamNumber[ WhoIAm ] ],
									TeamName[ team ] );
								SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_CaptureFlagMessage, 0);
							}
							break;
						}
					}
					
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Orb:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfOrbitals < MAXMULTIPLES )
					{
						CopyOfOrbAmmo[ CopyOfOrbitals ] = MAXMULTIPLEAMMO;
						CopyOfOrbitals++;
						MessageSFX = SFX_Orbital;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( Ships[ WhoIAm ].NumMultiples < MAXMULTIPLES )
					{
//						Ships[ WhoIAm ].Object.Flags |= SHIP_Multiple;
						Ships[ WhoIAm ].OrbAmmo[ Ships[ WhoIAm ].NumMultiples ] = MAXMULTIPLEAMMO;
						Ships[ WhoIAm ].NumMultiples++;
						CreateOrbitPulsar( WhoIAm );
						MessageSFX = SFX_Orbital;
					}
					else
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				break;		 
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		}

		if( PickupEnable || no_collision )
		{

			if( ( MessageSFX != -1 ) && ( bSoundEnabled ) && ( BikeCompSpeechSlider.value || !Speech ) )
			{
				if ( ( TriggeredSFX != -1 ) && !Random_Range( 4 ) )
					PlaySfxWithTrigger( MessageSFX, TriggeredSFX );
				else
					PlaySfx( MessageSFX, Sfx_Volume );
				if( ShowTextAnyway )
					AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
			}
			else
				if( Speech )
					PlaySfx( SFX_PickupGeneral, Sfx_Volume );

			PickupsGot[ Pickups[ i ].Type ]++;

			KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
			KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
			return TRUE;
		}
		else
		{
			if( !Pickups[ i ].CouldNotPickup )
			{
				if( ( MessageSFX != -1 ) && ( bSoundEnabled ) && ( BikeCompSpeechSlider.value || !Speech ) )
				{
					if ( ( TriggeredSFX != -1 ) && !Random_Range( 4 ) )
						PlaySfxWithTrigger( MessageSFX, TriggeredSFX );
					else
						PlaySfx( MessageSFX, Sfx_Volume );
	 
					if( ShowTextAnyway )
						AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
				}
				Pickups[ i ].CouldNotPickup = TRUE;
			}
			return FALSE;
		}
	}
	else
		return FALSE;
}

/*===================================================================
	Procedure	:	New Secondary Weapon ( AutoSelect )
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void NewSecondaryWeapon( int16 Weapon )
{
	uint16	Current_Type;

	SecondaryWeaponsGot[ Weapon ] = 1;

	Current_Type = SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].SecType;
	if( SecondaryWeaponAttribs[ Weapon ].SecType != Current_Type ) return;

	if( player_config->secondary_priority[ Weapon ] != -1 )
	{
		if( player_config->secondary_priority[ Weapon ] < player_config->secondary_priority[ Ships[ WhoIAm ].Secondary ] )
		{
			Ships[ WhoIAm ].Secondary = (BYTE) Weapon;
		}
	}
}

/*===================================================================
	Procedure	:	New Primary Weapon ( AutoSelect )
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void NewPrimaryWeapon( int16 Weapon )
{
	if( player_config->primary_priority[ Weapon ] < player_config->primary_priority[ Ships[ WhoIAm ].Primary ] )
	{
		PlaySfx( SFX_LoadWeapon, 1.0F );
		Ships[ WhoIAm ].Primary = (BYTE) Weapon;
	}
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Ship to Pickups collision
	Input		:	Nothing
	Output		:	All registers reserved
/*===================================================================ÄÄÄ*/
void CheckPickup( void )
{
	float		Length;
	VECTOR		Pos;
	VECTOR		Dir;
	int16		Count;
	GROUPLIST *	GroupsVisible;
	uint16	  *	GroupList;
	uint16		CurrentGroup;
	PICKUP	*	Pickup;
	PICKUP	*	NextPickup;

	if( Ships[ WhoIAm ].Object.Mode != NORMAL_MODE ) return;
	if( DebugInfo ) return;

	Dir.x = ( Ships[ WhoIAm ].Move_Off.x * framelag );
	Dir.y = ( Ships[ WhoIAm ].Move_Off.y * framelag );
	Dir.z = ( Ships[ WhoIAm ].Move_Off.z * framelag );

	Pos.x = ( Ships[ WhoIAm ].Object.Pos.x - Dir.x );
	Pos.y = ( Ships[ WhoIAm ].Object.Pos.y - Dir.y );
	Pos.z = ( Ships[ WhoIAm ].Object.Pos.z - Dir.z );

	Length = VectorLength( &Dir );
	NormaliseVector( &Dir );

	GroupsVisible = VisibleGroups( Ships[ WhoIAm ].Object.Group );

	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		Pickup = PickupGroups[ CurrentGroup ];

		while( Pickup )
		{
			NextPickup = Pickup->NextInGroup;

   			if( ( ( Pickup->Owner == WhoIAm ) && ( Pickup->Speed == 0.0F ) ) || ( Pickup->Owner != WhoIAm ) )
   			{
				if( RaytoSphereShort( (VECTOR *) &Pickup->Pos, PICKUP_RADIUS, &Pos, &Dir, Length ) )
						CollectPickup( Pickup->Index );
				else
					Pickup->CouldNotPickup = FALSE;
			}

			Pickup = NextPickup;
		}
	}																				
}

/*===================================================================
	Procedure	:	Kill a specific Pickup
	Input		:	uint16	Owner
				:	uint16	ID
				:	int16	Style to kill
	Output		:	nothing
===================================================================*/
void KillPickup( uint16 Owner, uint16 ID, int16 Style )
{
	uint16	i;
	uint16	NextPickup;
	uint16	fmpoly;
	int16	Count = 0;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		if( ( Pickups[ i ].Owner == Owner ) && ( Pickups[ i ].ID == ID ) )
		{
#if DEBUG_PICKUPS
			DebugPrintf( "Killed '%s' Owner '%s', ID %d\n", Messages[ Pickups[i].Type ], &Names[ Pickups[i].Owner ][ 0 ], Pickups[i].ID );
#endif

			switch( Style )
			{
				case PICKUPKILL_Immediate:
					if ( CTF )
					{
						int team;

						team = -1;
						switch ( Pickups[ i ].Type )
						{
						case PICKUP_Flag1:
							team = 0;
							break;
						case PICKUP_Flag2:
							team = 1;
							break;
						case PICKUP_Flag3:
							team = 2;
							break;
						case PICKUP_Flag4:
							team = 3;
							break;
						}
						if ( team >= 0 )
						{
							TeamFlagAtHome[ team ] = FALSE;
						}
					}
					CleanUpPickup( i );
					Count++;
					if( Count > 1 )
					{
						Msg( "Killed more than 1 pickup with same type\n" );
					}
					break;

				case PICKUPKILL_Disappear:
					if( Pickups[ i ].Mode == PICKUPMODE_Disappear ) break;

					Pickups[ i ].LifeCount = 0.0F;
					Pickups[ i ].Mode = PICKUPMODE_Disappear;
					Pickups[ i ].PickupCount = 32.0F;
			
					fmpoly = FindFreeFmPoly();
					if( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 1000.0F;
						FmPolys[ fmpoly ].Pos = Pickups[ i ].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_PICKUP_REGEN;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].Frm_Info = &PickupRegen_Header;
						FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
						FmPolys[ fmpoly ].Trans = 192;
						FmPolys[ fmpoly ].Dir.x = 0.0F;
						FmPolys[ fmpoly ].Dir.y = 0.0F;
						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].R = 192;
						FmPolys[ fmpoly ].G = 192;
						FmPolys[ fmpoly ].B = 192;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].Rot = 0.0F;
						FmPolys[ fmpoly ].Group = Pickups[ i ].Group;
						FmPolys[ fmpoly ].RotSpeed = 0.0F;
						FmPolys[ fmpoly ].xsize = PICKUP_REGEN_SIZE;
						FmPolys[ fmpoly ].ysize = PICKUP_REGEN_SIZE;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
					break;
			}
			return;
		}

		i = NextPickup;											/* Next Pickup */
	}																				
	

	AddFailedKillToQue( Owner, ID, Style, (uint16) -1 );

#if DEBUG_PICKUPS
	if( Owner != (uint16) -1 ) DebugPrintf( "Couldnt't kill pickup of Owner '%s', ID %d\n", &Names[ Owner ][ 0 ], ID );
	else DebugPrintf( "Couldnt't kill pickup of No Owner, ID %d\n", ID );
#endif
}


/*===================================================================
	Procedure	:	Find a free Pickup and move it from the free
				:	list to	the used list
	Input		:	nothing
	Output		:	uint16 number of Pickup free....
===================================================================*/
uint16 FindFreePickup( void )
{
	uint16 i;

	i = FirstPickupFree;
	
	if ( i == (uint16) -1 ) return i;
 
	Pickups[i].Prev = FirstPickupUsed;
	if( FirstPickupUsed != (uint16) -1)
	{
		Pickups[ FirstPickupUsed ].Next = i;
	}

	Pickups[i].CouldNotPickup = FALSE;

	FirstPickupUsed = i;
	FirstPickupFree = Pickups[ i ].Next;
	return i ;
}

/*===================================================================
	Procedure	:	Kill a used Pickup and move it from the used
				:	list to the free list
	Input		:	uint16 number of Pickup to kill.... 
	Output		:	nothing
===================================================================*/
void KillUsedPickup( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;

	if( Pickups[ i ].TriggerModPtr != NULL )
	{
		ApplyTriggerMod( Pickups[i].TriggerModPtr );
	}

	its_prev = Pickups[ i ].Prev;
	its_next = Pickups[ i ].Next;

	if ( i == FirstPickupUsed ) FirstPickupUsed = Pickups[ i ].Prev;
	if( its_prev != (uint16) -1 ) Pickups[ its_prev ].Next = its_next;
	if( its_next != (uint16) -1 ) Pickups[ its_next ].Prev = its_prev;

	if( Pickups[ i ].RegenSlot != -1 )
	{
		if( Pickups[ i ].RegenSlot < NumRegenPoints )
		{
			switch( RegenPoints[ Pickups[ i ].RegenSlot ].RegenType )
			{
				case PU_REGENTYPE_Random:		// Random each time
					RegenPoints[ Pickups[ i ].RegenSlot ].GenType = PU_GENTYPE_Initialised,		// Initialised immediatly
					RegenPoints[ Pickups[ i ].RegenSlot ].PickupIndex = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].TriggerMod = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].Status = PU_REGENSTAT_Free;
					RegenPoints[ Pickups[ i ].RegenSlot ].Wait = ( 5.0F * ANIM_SECOND );
					break;

				case PU_REGENTYPE_Const:		// Same each time
					RegenPoints[ Pickups[ i ].RegenSlot ].GenType = PU_GENTYPE_Initialised,		// Initialised immediatly
					RegenPoints[ Pickups[ i ].RegenSlot ].PickupIndex = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].TriggerMod = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].Status = PU_REGENSTAT_Free;
					RegenPoints[ Pickups[ i ].RegenSlot ].Wait = ( 5.0F * ANIM_SECOND );
					break;

				case PU_REGENTYPE_Onceonly:		// Once only ( Never used again )
					if( MaxPickupType[ Pickups[ i ].Type ] ) MaxPickupType[ Pickups[ i ].Type ]--;
					RegenPoints[ Pickups[ i ].RegenSlot ].GenType = PU_GENTYPE_Initialised,		// Initialised immediatly
					RegenPoints[ Pickups[ i ].RegenSlot ].PickupIndex = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].TriggerMod = (uint16) -1;
					RegenPoints[ Pickups[ i ].RegenSlot ].Status = PU_REGENSTAT_Used;
					RegenPoints[ Pickups[ i ].RegenSlot ].Wait = 0.0F;
					break;
			}

#if DEBUG_PICKUPS
			if( Pickups[i].Owner != (uint16) -1 ) DebugPrintf( "Killed '%s' from slot %d, Owner '%s', ID %d\n", Messages[ Pickups[i].Type ], Pickups[i].RegenSlot, &Names[ Pickups[i].Owner ][ 0 ], Pickups[i].ID );
			else DebugPrintf( "Killed '%s' from slot %d, No Owner, ID %d\n", Messages[ Pickups[i].Type ], Pickups[i].RegenSlot, Pickups[i].ID );
#endif
		}
		else
		{
			Pickups[ i ].RegenSlot = -1;
#if DEBUG_PICKUPS
			if( Pickups[i].Owner != (uint16) -1 ) DebugPrintf( "Killed '%s'. Owner '%s', ID %d with slot grater than max slots\n", Messages[ Pickups[i].Type ], &Names[ Pickups[i].Owner ][ 0 ], Pickups[i].ID );
			else DebugPrintf( "Killed '%s'. No Owner, ID %d with slot grater than max slots\n", Messages[ Pickups[i].Type ], Pickups[i].ID );
#endif
		}
	}
	else
	{
#if DEBUG_PICKUPS
		if( Pickups[i].Owner != (uint16) -1 ) DebugPrintf( "Killed '%s' from slot %d, Owner '%s', ID %d\n", Messages[ Pickups[i].Type ], Pickups[i].RegenSlot, &Names[ Pickups[i].Owner ][ 0 ], Pickups[i].ID  );
		else DebugPrintf( "Killed '%s' from slot %d, No Owner, ID %d\n", Messages[ Pickups[i].Type ], Pickups[i].RegenSlot, Pickups[i].ID  );
#endif
	}

	Pickups[ i ].Type = (uint16) -1;
	Pickups[ i ].Prev = (uint16) -1;
	Pickups[ i ].Next = FirstPickupFree;
	FirstPickupFree	= i;

	RemovePickupFromGroup( i, Pickups[ i ].Group );
}

/*===================================================================
	Procedure	:	Clean up and kill a Pickup
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void CleanUpPickup( uint16 i )
{

    /* remove pickup from global counter */
	NumPickupType[ Pickups[ i ].Type ]--;

#if DEBUG_PICKUPS
	if( NumPickupType[ Pickups[ i ].Type ] < 0 )
	{
		DebugPrintf( "Killed more %s's than allowed\n", Messages[ Pickups[ i ].Type ] );
	}
#endif

	/* kill the pickups light */
	if( Pickups[ i ].Light != (uint16) -1 )
	{
		/* cause a light to go red and get smaller then die */
		SetLightDie( Pickups[ i ].Light );
		Pickups[ i ].Light = (uint16) -1;
	}

	/* kill the pickups faceme poly */
	if ( Pickups[ i ].Fmpoly != (uint16) -1 )
	{
		/* Kill a used FmPoly and move it from the used list to the free list */
		KillUsedFmPoly( Pickups[ i ].Fmpoly );
		Pickups[ i ].Fmpoly = (uint16) -1;
	}

	/* kill the pickups model */
	if ( Pickups[ i ].ModelNum != (uint16) -1 )
	{
		/* Kill a used Model and move it from the used list to the free list */
		KillUsedModel( Pickups[ i ].ModelNum );
		Pickups[ i ].ModelNum = (uint16) -1;
	}

	/* kill and remove pickup from used list to free list */
	KillUsedPickup( i );

}

/*===================================================================
	Procedure	:	Init Pickup
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	VECTOR	*	Dir
				:	float		Speed
				:	int16		Type
				:	uint16		Owner
				:	uint16		ID
				:	int16		Regen Slot
				:	BOOL		Twinkle Into life?
				:	float		LifeCount
				:	uint16		TriggerMod Index
	Output		:	uint16		-1 if none free
				:				-2 if too many of same type
===================================================================*/
uint16 InitOnePickup( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type, uint16 Owner, uint16 ID, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod )
{
	uint16	i;
	uint16	light;
	uint16	fmpoly;
	uint16	model;

	if( !DebugInfo && ( MyGameStatus != STATUS_SinglePlayer ) )
	{
	   	if( NumPickupType[ Type ] >= MaxPickupType[ Type ] )
	   	{
	   		switch( Type )
	   		{
	   			case PICKUP_Mugs:
	   			case PICKUP_Shield:
//	   			case PICKUP_PowerPod:
	   			case PICKUP_GeneralAmmo:
	   			case PICKUP_SussGunAmmo:
	   			case PICKUP_PyroliteAmmo:
					break;

				case PICKUP_Bounty:
				case PICKUP_Flag:
				case PICKUP_Flag1:
				case PICKUP_Flag2:
				case PICKUP_Flag3:
				case PICKUP_Flag4:
					KillAllPickupsOfType( Type, PICKUPKILL_Immediate );
	   				break;
	
	   			default:
#if DEBUG_PICKUPS
	   				DebugPrintf( "Tried to generated more %s's than allowed\n", Messages[ Type ] );
#endif
					return( (uint16) -2 );
	   				break;
			}
	   	}
	}

	i = FindFreePickup();

	if( i != (uint16) -1 )
	{
		if( RegenSlot != -1 )
		{
			if( RegenSlot < 0 || RegenSlot >= NumRegenPoints )
			{
				// EvaluateMessage() MSG_VERYSHORTDROPPICKUP sends 255
				RegenSlot = -1;	// if trying to use bad slot then use no slot.
			}
			else if ( RegenPoints[ RegenSlot ].Status == PU_REGENSTAT_Used )
			{
				RegenSlot = -1;	// if trying to use used slot then use no slot.
			}
		}

		AddPickupToGroup( i, Group );

		if( Sparkle )
		{
			PlayPannedSfx( SFX_Respawn, Group , Pos, 0.0F );
			Pickups[ i ].Mode = PICKUPMODE_Appear;
		}
		else Pickups[ i ].Mode = PICKUPMODE_Normal;
		Pickups[ i ].Speed = Speed;					//PICKUP_SPEED;
		Pickups[ i ].ExternalSpeed = 0.0F;
		Pickups[ i ].LifeCount = LifeCount;
		Pickups[ i ].PickupTime = 0.0F;
		Pickups[ i ].PickupCount = 0.0F;			//16.0F;
		Pickups[ i ].Type = (int8) Type;
		Pickups[ i ].Owner = Owner;
		Pickups[ i ].Dir = *Dir;
		Pickups[ i ].Rot.x = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].Rot.y = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].Rot.z = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].ID = ID;
		Pickups[ i ].Pos = *Pos;
		Pickups[ i ].DirVector.x = 0.0F;
		Pickups[ i ].DirVector.y = 0.0F;
		Pickups[ i ].DirVector.z = 1.0F;
		Pickups[ i ].ColFlag = 0;
		Pickups[ i ].Group = Group;
		Pickups[ i ].RegenSlot = RegenSlot;
		Pickups[ i ].ModelType = PickupAttribs[ Type ].ModelType;
		Pickups[ i ].TriggerMod = TriggerMod;
		if( TriggerMod == (uint16) -1 ) Pickups[ i ].TriggerModPtr = NULL;
		else Pickups[ i ].TriggerModPtr = &TrigMods[ TriggerMod ];
		QuatFrom2Vectors( &Pickups[ i ].DirQuat, &Forward, &Pickups[ i ].DirVector );
		QuatToMatrix( &Pickups[ i ].DirQuat, &Pickups[ i ].Mat );			// Create Dir Matrix

		NumPickupType[ Pickups[ i ].Type ]++;

		if( RegenSlot != -1 )
		{
			if( RegenSlot < NumRegenPoints )
			{
				RegenPoints[ RegenSlot ].Status = PU_REGENSTAT_Used;
				RegenPoints[ RegenSlot ].Wait = 0.0F;
				RegenPoints[ RegenSlot ].PickupIndex = i;
				RegenPoints[ RegenSlot ].PickupID = ID;
#if DEBUG_PICKUPS
				if( Owner != (uint16) -1 ) DebugPrintf( "Initialised '%s' in slot %d. Owner '%s', ID %d\n", Messages[ Type ], RegenSlot, &Names[ Owner ][ 0 ], ID );
				else DebugPrintf( "Initialised '%s' in slot %d. No Owner, ID %d\n", Messages[ Type ], RegenSlot, ID );
#endif
			}
			else
			{
#if DEBUG_PICKUPS
				if( Owner != (uint16) -1 ) DebugPrintf( "Tried to init '%s', Owner '%s', ID %d, in regen slot grater than max slots\n", Messages[ Type ], &Names[ Owner ][ 0 ], ID );
				else DebugPrintf( "Tried to init '%s', No Owner, ID %d, in regen slot grater than max slots\n", Messages[ Type ], ID );
#endif
			}
		}
		else
		{
#if DEBUG_PICKUPS
			if( Owner != (uint16) -1 ) DebugPrintf( "Initialised '%s' in slot %d, Owner '%s', ID %d\n", Messages[ Type ], RegenSlot, &Names[ Owner ][ 0 ], ID );
			else DebugPrintf( "Initialised '%s' in slot %d, No Owner, ID %d\n", Messages[ Type ], RegenSlot, ID );
#endif
		}

		light = (uint16) -1;
		model = (uint16) -1;

		if( PickupAttribs[ Type ].Light != 0 )
		{
			light = FindFreeXLight();
			if( light != (uint16 ) -1 )
			{
				if( !PickupLightDetail ) XLights[ light ].Visible = FALSE;
				XLights[ light ].Pos = *Pos;
				XLights[ light ].Size = PickupAttribs[ Type ].Lightsize;
				XLights[ light ].SizeCount = 0.0F;
				XLights[ light ].r = PickupAttribs[ Type ].R;
				XLights[ light ].g = PickupAttribs[ Type ].G;
				XLights[ light ].b = PickupAttribs[ Type ].B;
				XLights[ light ].Group = Pickups[ i ].Group;
			}
		}
		Pickups[ i ].Light = light;

		model =	FindFreeModel();
		if( model != (uint16 ) -1 )
   		{
   			Models[ model ].OwnerType = OWNER_SHIP;
   			Models[ model ].Owner = Pickups[ i ].Owner;
   			Models[ model ].ModelNum = Pickups[ i ].ModelType;
   			Models[ model ].Type = MODTYPE_Pickup;
			Models[ model ].Flags = MODFLAG_Clip;
			if( Sparkle ) Models[ model ].Visible = FALSE;
			else Models[ model ].Visible = TRUE;
   			Models[ model ].Pos = *Pos;
   			Models[ model ].Mat = Pickups[ i ].Mat;
   			Models[ model ].Scale = 1.0F;
   			Models[ model ].Func = MODFUNC_Nothing;
   			Models[ model ].Group = Pickups[ i ].Group;

   			MatrixTranspose( &Models[ model ].Mat, &Models[ model ].InvMat );
   		}

		if( Sparkle )
		{
			Pickups[ i ].PickupCount = 32.0F;

			fmpoly = FindFreeFmPoly();
			if( fmpoly != (uint16) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].SeqNum = FM_PICKUP_REGEN;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &PickupRegen_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].Trans = 192;
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].Group = Pickups[ i ].Group;
				FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = PICKUP_REGEN_SIZE;
				FmPolys[ fmpoly ].ysize = PICKUP_REGEN_SIZE;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		}

		Pickups[ i ].ModelNum = model;
		Pickups[ i ].Fmpoly = (uint16) -1;

		// check flag position
		if ( CTF )
		{
			int team;
			
			team = -1;
			switch ( Pickups[ i ].Type )
			{
			case PICKUP_Flag1:
				team = 0;
				break;
			case PICKUP_Flag2:
				team = 1;
				break;
			case PICKUP_Flag3:
				team = 2;
				break;
			case PICKUP_Flag4:
				team = 3;
				break;
			}
			if ( team >= 0 )
			{
				Ships[ WhoIAm ].Object.Flags &= ~TeamFlagMask[ team ];
				switch ( GoalCheckTeam( &Pickups[ i ].Pos, &Pickups[ i ].Pos, Pickups[ i ].Group, team ) )
				{
				case GOAL_SCORED:
					TeamFlagAtHome[ team ] = TRUE;
					break;
				default:
					TeamFlagAtHome[ team ] = FALSE;
				}
			}
		}

		if ( BountyHunt && Pickups[ i ].Type == PICKUP_Bounty )
				Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingBounty;

		if ( CaptureTheFlag && Pickups[ i ].Type == PICKUP_Flag )
				Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingFlag;
	}
	else
	{
#if DEBUG_PICKUPS
		if( Owner != (uint16) -1 ) DebugPrintf( "Couldn't Initialise '%s' in slot %d. Owner '%s', ID %d\n", Messages[ Type ], RegenSlot, &Names[ Owner ][ 0 ], ID );
		else DebugPrintf( "Couldn't Initialise '%s' in slot %d. No Owner, ID %d\n", Messages[ Type ], RegenSlot, ID );
#endif
	}

	return i;
}

/*===================================================================
	Procedure	:	Init Joining Pickup
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	VECTOR	*	Dir
				:	float		Speed
				:	int16		Type
				:	uint16		Owner
				:	uint16		ID
				:	int16		Regen Slot
				:	BOOL		Twinkle Into life?
				:	float		LifeCount
				:	uint16		TriggerMod Index
	Output		:	uint16		-1 if none free
				:				-2 if too many of same type
===================================================================*/
uint16 InitJoinPickup( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type, uint16 Owner, uint16 ID, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod )
{
	uint16	i;
	uint16	light;
	uint16	fmpoly;
	uint16	model;

	i = FindFreePickup();

	if( i != (uint16) -1 )
	{
#if 0 //-----------------------------------------------------------*/
		if( RegenSlot != -1 )
				if( RegenPoints[ RegenSlot ].Status == PU_REGENSTAT_Used ) RegenSlot = -1;	// if trying to use used slot then use no slot.
#endif //----------------------------------------------------------*/

		AddPickupToGroup( i, Group );

		if( Sparkle ) Pickups[ i ].Mode = PICKUPMODE_Appear;
		else Pickups[ i ].Mode = PICKUPMODE_Normal;
		Pickups[ i ].Speed = Speed;					//PICKUP_SPEED;
		Pickups[ i ].ExternalSpeed = 0.0F;
		Pickups[ i ].LifeCount = LifeCount;
		Pickups[ i ].PickupCount = 0.0F;			//16.0F;
		Pickups[ i ].Type = (int8) Type;
		Pickups[ i ].Owner = Owner;
		Pickups[ i ].Dir = *Dir;
		Pickups[ i ].Rot.x = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].Rot.y = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].Rot.z = ( ( (float) Random_Range( 256 ) ) / 64.0F ) - 2.0F;
		Pickups[ i ].ID = ID;
		Pickups[ i ].Pos = *Pos;
		Pickups[ i ].DirVector.x = 0.0F;
		Pickups[ i ].DirVector.y = 0.0F;
		Pickups[ i ].DirVector.z = 1.0F;
		Pickups[ i ].ColFlag = 0;
		Pickups[ i ].Group = Group;
		Pickups[ i ].RegenSlot = RegenSlot;
		Pickups[ i ].ModelType = PickupAttribs[ Type ].ModelType;
		Pickups[ i ].TriggerMod = TriggerMod;
		if( TriggerMod == (uint16) -1 ) Pickups[ i ].TriggerModPtr = NULL;
		else Pickups[ i ].TriggerModPtr = &TrigMods[ TriggerMod ];
		QuatFrom2Vectors( &Pickups[ i ].DirQuat, &Forward, &Pickups[ i ].DirVector );
		QuatToMatrix( &Pickups[ i ].DirQuat, &Pickups[ i ].Mat );			// Create Dir Matrix

		NumPickupType[ Pickups[ i ].Type ]++;
#if 1 //-----------------------------------------------------------*/
		if( RegenSlot != -1 )
		{
			RegenPoints[ RegenSlot ].Status = PU_REGENSTAT_Used;
			RegenPoints[ RegenSlot ].Wait = 0.0F;
			RegenPoints[ RegenSlot ].PickupIndex = i;
			RegenPoints[ RegenSlot ].PickupID = ID;
		}
#endif //----------------------------------------------------------*/

		light = (uint16) -1;
		model = (uint16) -1;

		if( PickupAttribs[ Type ].Light != 0 )
		{
			light = FindFreeXLight();
			if( light != (uint16 ) -1 )
			{
				if( !PickupLightDetail ) XLights[ light ].Visible = FALSE;
				XLights[ light ].Pos = *Pos;
				XLights[ light ].Size = PickupAttribs[ Type ].Lightsize;
				XLights[ light ].SizeCount = 0.0F;
				XLights[ light ].r = PickupAttribs[ Type ].R;
				XLights[ light ].g = PickupAttribs[ Type ].G;
				XLights[ light ].b = PickupAttribs[ Type ].B;
				XLights[ light ].Group = Pickups[ i ].Group;
			}
		}
		Pickups[ i ].Light = light;

		model =	FindFreeModel();
		if( model != (uint16 ) -1 )
   		{
   			Models[ model ].OwnerType = OWNER_SHIP;
   			Models[ model ].Owner = Pickups[ i ].Owner;
   			Models[ model ].ModelNum = Pickups[ i ].ModelType;
   			Models[ model ].Type = MODTYPE_Pickup;
			Models[ model ].Flags = MODFLAG_Clip;
			if( Sparkle ) Models[ model ].Visible = FALSE;
			else Models[ model ].Visible = TRUE;
   			Models[ model ].Pos = *Pos;
   			Models[ model ].Mat = Pickups[ i ].Mat;
   			Models[ model ].Scale = 1.0F;
   			Models[ model ].Func = MODFUNC_Nothing;
   			Models[ model ].Group = Pickups[ i ].Group;

   			MatrixTranspose( &Models[ model ].Mat, &Models[ model ].InvMat );
   		}

		if( Sparkle )
		{
			Pickups[ i ].PickupCount = 32.0F;

			fmpoly = FindFreeFmPoly();
			if( fmpoly != (uint16) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].SeqNum = FM_PICKUP_REGEN;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &PickupRegen_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].Trans = 192;
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].Group = Pickups[ i ].Group;
				FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = PICKUP_REGEN_SIZE;
				FmPolys[ fmpoly ].ysize = PICKUP_REGEN_SIZE;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		}

		Pickups[ i ].ModelNum = model;
		Pickups[ i ].Fmpoly = (uint16) -1;
	}
	else
	{
#if DEBUG_PICKUPS
		if( Owner != (uint16) -1 ) DebugPrintf( "Couldn't Initialise '%s' in slot %d. Owner '%s', ID %d\n", Messages[ Type ], RegenSlot, &Names[ Owner ][ 0 ], ID );
		else DebugPrintf( "Couldn't Initialise '%s' in slot %d. No Owner, ID %d\n", Messages[ Type ], RegenSlot, ID );
#endif
	}
	return i;
}

/*===================================================================
	Procedure	:	Process Secondary Bullets
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessPickups( void )
{
	uint16		i;
	uint16		NextPickup;
	uint16		OldGroup;
	float		Speed;
	QUAT		TempQuat;
	VECTOR		TempVector;
	VECTOR		NewDir;
	VECTOR		FrameLag_Rot;
	int16		WantedPowerPods;
	int16		EnabledPlayers = 0;
	int16		NumPowerPodsGot = 0;
	VECTOR		NewPos;
	VECTOR		OldPos;

	ProcessFailedKills();

	for( i = 0; i < MAX_PLAYERS; i++ )
	{
		if( ( Ships[ i ].enable ) && ( ( GameStatus[ i ] == STATUS_Normal ) || ( GameStatus[ i ] == STATUS_SinglePlayer) ) )
		{
			EnabledPlayers++;
			NumPowerPodsGot += ( Ships[ i ].Object.PowerLevel ); 
		}
	}

	WantedPowerPods = ( 2 + EnabledPlayers );
	if( IsHost ) NumPowerPods = ( WantedPowerPods - ( NumPickupType[ PICKUP_PowerPod ] + NumPowerPodsGot + NumPowerPods ) );
	MaxPickupType[ PICKUP_PowerPod ] = WantedPowerPods;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		Pickups[ i ].PickupTime -= framelag;
		if( Pickups[ i ].PickupTime < 0.0F )
		{
			Pickups[ i ].PickupTime = 0.0F;
		}

		switch( Pickups[ i ].Mode )
		{
			case PICKUPMODE_Appear:
				Pickups[ i ].PickupCount -= framelag;
				if( Pickups[ i ].PickupCount <= 0.0F )
				{
					if( Pickups[ i ].ModelNum != (uint16) -1 )
					{
						Models[ Pickups[ i ].ModelNum ].Visible = TRUE;
					}
					Pickups[ i ].PickupCount = 0.0F;
					Pickups[ i ].Mode = PICKUPMODE_Normal;
				}
				break;

			case PICKUPMODE_Normal:
				if( ( ( Pickups[ i ].Owner == WhoIAm ) ) ||
					( ( ( Pickups[ i ].Owner == (uint16) -1 ) && ( IsHost ) ) ) ||
					( ( ( Pickups[ i ].Owner != (uint16) -1 ) && !Ships[ Pickups[ i ].Owner ].enable && ( IsHost ) ) ) ||
					( PlayDemo ) )
				{
					if( Pickups[ i ].LifeCount != -1.0F )
					{
						Pickups[ i ].LifeCount -= framelag;
						if( Pickups[ i ].LifeCount <= 0.0F )
						{
//							if( ( Pickups[ i ].Type >= PICKUP_Trojax ) && ( Pickups[ i ].Type <= PICKUP_Laser ) )
							{
#if DEBUG_PICKUPS
								DebugPrintf( "Tell %s to dissapear\n", Messages[ Pickups[ i ].Type ] );
#endif
							}

							KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Disappear );
							KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Disappear );

							i = NextPickup;
						}
					}
				}
				break;

			case PICKUPMODE_Disappear:
				Pickups[ i ].PickupCount -= framelag;
				if( Pickups[ i ].PickupCount <= 0.0F )
				{
					if( ( ( Pickups[ i ].Owner == WhoIAm ) ) ||
						( ( ( Pickups[ i ].Owner == (uint16) -1 ) && ( IsHost ) ) ) ||
						( ( ( Pickups[ i ].Owner != (uint16) -1 ) && !Ships[ Pickups[ i ].Owner ].enable && ( IsHost ) ) ) ||
						( PlayDemo ) )
					{
//						if( ( Pickups[ i ].Type >= PICKUP_Trojax ) && ( Pickups[ i ].Type <= PICKUP_Laser ) )
						{
#if DEBUG_PICKUPS
							DebugPrintf( "Kill %s immediatly\n", Messages[ Pickups[ i ].Type ] );
#endif
						}

						AddPickupToRegen( Pickups[ i ].Type );
						KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
						KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
						i = NextPickup;
						break;
					}
					else
					{
						if( Pickups[ i ].PickupCount < -( ANIM_SECOND * 5.0F ) )
						{
							KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
							i = NextPickup;
						}
					}
				}
				break;

			default:
				break;
		}

		if( i != NextPickup )
		{
			OldPos = Pickups[ i ].Pos;

	   		if( ( Pickups[ i ].Speed != 0.0F ) || ( Pickups[ i ].ExternalSpeed != 0.0F ) )
	   		{
	   			Speed = ( Pickups[ i ].Speed * framelag );
				Speed += ( Pickups[ i ].ExternalSpeed * framelag );

	   			NewDir.x = ( Pickups[ i ].Dir.x * Speed );
	   			NewDir.y = ( Pickups[ i ].Dir.y * Speed );
	   			NewDir.z = ( Pickups[ i ].Dir.z * Speed );
	
				OldGroup = Pickups[ i ].Group;

/*===================================================================
	Do first collision, when initialised or changed direction
===================================================================*/
				NewPos.x = ( Pickups[ i ].Pos.x + NewDir.x );
				NewPos.y = ( Pickups[ i ].Pos.y + NewDir.y );
				NewPos.z = ( Pickups[ i ].Pos.z + NewDir.z );
				
				if( QCollide( &Pickups[ i ].Pos, Pickups[ i ].Group, &NewDir, PICKUP_MODEL_RADIUS,
							  (VECTOR *) &Pickups[ i ].ColPoint, &Pickups[ i ].Group, &Pickups[ i ].ColPointNormal ) )
				{
	   				TempVector = Pickups[i].Dir;
	   				NormaliseVector( &TempVector );
	   				ReflectVector( &TempVector, &Pickups[i].ColPointNormal, &Pickups[i].Dir );

					Pickups[ i ].Speed *= 0.75F;
					Pickups[ i ].ExternalSpeed *= 0.75F;

	   				NewPos.x = Pickups[i].ColPoint.x;
	   				NewPos.y = Pickups[i].ColPoint.y;
	   				NewPos.z = Pickups[i].ColPoint.z;
				}

				if( Pickups[ i ].Speed > PICKUP_SPEED ) Pickups[ i ].Speed = PICKUP_SPEED;
				if( Pickups[ i ].ExternalSpeed > PICKUP_SPEED ) Pickups[ i ].ExternalSpeed = PICKUP_SPEED;

   				Pickups[ i ].Pos = NewPos;
   				if( Pickups[ i ].Speed > 0.0F ) Pickups[ i ].Speed -= ( PICKUP_DEC * framelag );
				if( Pickups[ i ].Speed < 0.0F ) Pickups[ i ].Speed = 0.0F;
   				if( Pickups[ i ].ExternalSpeed > 0.0F ) Pickups[ i ].ExternalSpeed -= ( PICKUP_DEC * framelag );
				if( Pickups[ i ].ExternalSpeed < 0.0F ) Pickups[ i ].ExternalSpeed = 0.0F;

				if( OldGroup != Pickups[ i ].Group )
				{
					MovePickupToGroup( i, OldGroup, Pickups[ i ].Group );
				}
	
		   		if( Pickups[ i ].Light != (uint16) -1 )									/* Light Attached? */
		   		{																		
		   			XLights[ Pickups[ i ].Light ].Pos = Pickups[ i ].Pos;				/* Update Light Position */
					XLights[ Pickups[ i ].Light ].Group = Pickups[ i ].Group;
					if( XLights[ Pickups[ i ].Light ].Type == SPOT_LIGHT )
					{
						ApplyMatrix( &Pickups[ i ].Mat, &Forward, &XLights[ Pickups[ i ].Light ].Dir );
						NormaliseVector( &XLights[ Pickups[ i ].Light ].Dir );
					}
		   		}																		
		   																				
		   		if( Pickups[ i ].Fmpoly != (uint16) -1 )								/* Faceme Poly Attached? */
		   		{																		
		   			FmPolys[ Pickups[ i ].Fmpoly ].Pos = Pickups[ i ].Pos;				/* Update FmPoly Position */
		   			FmPolys[ Pickups[ i ].Fmpoly ].Group = Pickups[ i ].Group;
		   		}																		
		   																				
				if( Pickups[ i ].ModelNum != (uint16) -1 )								/* 3D Model Attached? */
		   		{																		
		   			Models[ Pickups[ i ].ModelNum ].Pos = Pickups[ i ].Pos;
		   			Models[ Pickups[ i ].ModelNum ].Group = Pickups[ i ].Group;
		   		}																		
	   		}
	
			switch( Pickups[ i ].Type )
			{
				case PICKUP_Flag:
				case PICKUP_Flag1:
				case PICKUP_Flag2:
				case PICKUP_Flag3:
				case PICKUP_Flag4:
					if( Pickups[ i ].ModelNum != (uint16) -1 )								/* 3D Model Attached? */
			   		{																		
						Speed = ( LevelTimeTaken * 4.0F );
						Models[ Pickups[ i ].ModelNum ].Frame = (int16) ( FMOD( Speed, ( 2.5F * ANIM_SECOND ) ) / ( 0.25F * ANIM_SECOND ) );
						Models[ Pickups[ i ].ModelNum ].InterpFrame = ( Models[ Pickups[ i ].ModelNum ].Frame + 1 );
						if( Models[ Pickups[ i ].ModelNum ].InterpFrame == 10 ) Models[ Pickups[ i ].ModelNum ].InterpFrame = 0;
						Models[ Pickups[ i ].ModelNum ].InterpTime = FMOD( ( FMOD( Speed, ( 2.5F * ANIM_SECOND ) ) / ( 0.25F * ANIM_SECOND ) ), 1.0F );
					}
					if ( CTF )
					{
						int team;
						
						team = -1;
						switch ( Pickups[ i ].Type )
						{
						case PICKUP_Flag1:
							team = 0;
							break;
						case PICKUP_Flag2:
							team = 1;
							break;
						case PICKUP_Flag3:
							team = 2;
							break;
						case PICKUP_Flag4:
							team = 3;
							break;
						}
						if ( team >= 0 )
						{
							VECTOR Move;
							float MoveDist;

							switch ( GoalCheckTeam( &OldPos, &Pickups[ i ].Pos, Pickups[ i ].Group, team ) )
							{
							case GOAL_SCORED:
								if ( !TeamFlagAtHome[ team ] )
								{
									Move.x = Pickups[ i ].Pos.x - OldPos.x;
									Move.y = Pickups[ i ].Pos.y - OldPos.y;
									Move.z = Pickups[ i ].Pos.z - OldPos.z;
									MoveDist = VectorLength( &Move );
									if ( MoveDist > 0.0F )
										AddColourMessageToQue(FlagMessageColour, FLAG_DRIFTED_BACK, TeamName[ team ] );
									TeamFlagAtHome[ team ] = TRUE;
									//								SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_FlagDriftedIn, 0);
								}
								break;
							default:
								if ( TeamFlagAtHome[ team ] )
								{
									Move.x = Pickups[ i ].Pos.x - OldPos.x;
									Move.y = Pickups[ i ].Pos.y - OldPos.y;
									Move.z = Pickups[ i ].Pos.z - OldPos.z;
									MoveDist = VectorLength( &Move );
									if ( MoveDist > 0.0F )
										AddColourMessageToQue(FlagMessageColour, FLAG_ESCAPED, TeamName[ team ] );
									TeamFlagAtHome[ team ] = FALSE;
									//								SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_FlagEscaped, 0);
								}
							}
						}
					}
					break;
				default:
					break;
			}

	   		if( IsGroupVisible[ Pickups[i].Group ] )								// Don't Rotate if not visible
	   		{
	   			FrameLag_Rot.x = ( Pickups[i].Rot.x * framelag );
	   			FrameLag_Rot.y = ( Pickups[i].Rot.y * framelag );
	   			FrameLag_Rot.z = ( Pickups[i].Rot.z * framelag );
	   			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
	   			QuatMultiply( &TempQuat, &Pickups[ i ].DirQuat, &Pickups[ i ].DirQuat );
	   			QuatToMatrix( &Pickups[ i ].DirQuat, &Pickups[ i ].Mat );				// Create Dir Matrix
	
				if( Pickups[ i ].ModelNum != (uint16) -1 )								/* 3D Model Attached? */
		   		{																		
					Models[ Pickups[ i ].ModelNum ].Mat = Pickups[ i ].Mat;				/* Update Model Matrix */
		   			MatrixTranspose( &Pickups[ i ].Mat, &Models[ Pickups[ i ].ModelNum ].InvMat );
		   			Models[ Pickups[ i ].ModelNum ].Pos = Pickups[ i ].Pos;
		   			Models[ Pickups[ i ].ModelNum ].Group = Pickups[ i ].Group;
				}
	   		}

			i = NextPickup;											/* Next Pickup */
		}
	}																				
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

/*===================================================================
	Procedure	:	Save Pickup Positions
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
	char		NewFilename[ 128 ];

void SavePickupsPositions( void )
{
	char	*	NewExt = ".PIC";
	FILE	*	fp;
	uint16		i;
	int16		Num_Pickups;
	uint16		GenType = PU_GENTYPE_Initialised;
	uint16		ReGenType = PU_REGENTYPE_Random;
	float		GenDelay = -1.0F;
	float		LifeSpan = -1.0F;
	uint16		TriggerMod = (uint16) -1;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );

	fp = fopen( &NewFilename[ 0 ], "wb" );

	if( fp != NULL )
	{
		i = FirstPickupUsed;
		Num_Pickups = 0;

		while( i != (uint16) -1 )
		{
			Num_Pickups++;
			i = Pickups[ i ].Prev;					/* Next Pickup */
		}

		fwrite( &Num_Pickups, sizeof( int16 ), 1, fp );

		i = FirstPickupUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &GenType, sizeof( uint16 ), 1, fp );
			fwrite( &ReGenType, sizeof( uint16 ), 1, fp );
			fwrite( &GenDelay, sizeof( float ), 1, fp );
			fwrite( &LifeSpan, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Pos.x, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Pos.y, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Pos.z, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Group, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Type, sizeof( uint16 ), 1, fp );
			fwrite( &TriggerMod, sizeof( uint16 ), 1, fp );

			i = Pickups[ i ].Prev;					/* Next Pickup */
		}

		fclose( fp );

		AddColourMessageToQue(SystemMessageColour, "Saved Pickup Positions" );
		AddColourMessageToQue(SystemMessageColour, &NewFilename[ 0 ] );
	}
	else
	{
		AddColourMessageToQue(SystemMessageColour, "Error Saving Pickup Positions" );
		AddColourMessageToQue(SystemMessageColour, &NewFilename[ 0 ] );
	}
}

/*===================================================================
	Procedure	:		Randomize Pickup Regen Slots
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
uint16	CopyOfSeed1;
uint16	CopyOfSeed2;

void RandomizePickups( void )
{
	int16	Count;
	int16	Slot1, Slot2;
	VECTOR	TempPos;
	uint16	TempGroup;

	/* Shuffle the regen points around x number of times */
	for( Count = 0; Count < 1000; Count++ )
	{
		/* pick 2 random regen points */
		Slot1 = Random_Range( NumRegenPoints );
		Slot2 = Random_Range( NumRegenPoints );

		/* make sure they are differen't points */
		if( Slot1 == Slot2 )
			continue;

		/* copy location of slot2 into a temporary holder */
		TempGroup = RegenPoints[ Slot2 ].Group;
		TempPos   = RegenPoints[ Slot2 ].Pos;

		/* assign the position of slot 1 to slot 2 */
		RegenPoints[ Slot2 ].Group = RegenPoints[ Slot1 ].Group;
		RegenPoints[ Slot2 ].Pos   = RegenPoints[ Slot1 ].Pos;

		/* assign the temporary position to slot 1 */
		RegenPoints[ Slot1 ].Group = TempGroup;
		RegenPoints[ Slot1 ].Pos   = TempPos;

	}
}


/*===================================================================
	Procedure	:		Check and correct valid regen slots
	Input		:		int16	Slot
	Output		:		BOOL	True/False
===================================================================*/
BOOL CheckValidRegenSlot( int16 Slot )
{
	/* if the position is inside the group */
	if( PointInsideSkin( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group ) )
		return( TRUE );

	/* set it to be used even though its really not
	   so we don't assign a pickup to it again */

	RegenPoints[ Slot ].Status = PU_REGENSTAT_Used;
	RegenPoints[ Slot ].Wait = 0.0F;
	RegenPoints[ Slot ].Type = (uint16) -1;

	/* if you are host add pickup thats in the slot to regen list */
	if( ( IsHost ) )
		AddPickupToRegen( RegenPoints[ Slot ].Type );

	/* let people know the pic file is messed up! */
    DebugPrintf("Bad regen point detected! The PIC file needs to be fixed!\n");

	/* failed */
	return( FALSE );

}


/*===================================================================
	Procedure	:	Save Pickup Positions
	Input		:	Nothing
	Output		:	BOOL	True/False
===================================================================*/
BOOL LoadPickupsPositions( void )
{

  /* the extension of the file with pickup data */
  char  * NewExt = ".PIC";

  FILE    * fp;
  int16   Count;
  VECTOR  Dir = { 0.0F, 0.0F, 0.0F };
  uint16  i;
  uint16  TriggerMod = (uint16) -1;
  int32   StartingState = 1;
  int32   FileSize;
  uint32  MagicNumber;
  uint32  VersionNumber;

  /* wipes out all pickups, so we can load new ones */
  KillAllPickups();

#if 0
  DebugPrintf( "RandomSeeds %d, %d : NumPrimaryPickups %d\n", CopyOfSeed1, CopyOfSeed2, NumPrimaryPickups );
#endif

  /* convert the level_name into level_name.pic */
  Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );

  /* reset number of pickups in groups to 0 */
  for( Count = 0; Count < MAXGROUPS; Count++ )
    NumPickupsPerGroup[ Count ] = 0;

  /* reset the number of each pickup to 0
     and the max number of each to 1000 */
  for( Count = 0; Count < MAXPICKUPTYPES; Count++ )
  {
    NumPickupType[ Count ] = 0;
    MaxPickupType[ Count ] = 1000;
  }

  /* counter */
  NumRegenPoints = 0;

  /* reset number of pickups in current biker */
  Ships[WhoIAm].PickupIdCount = (uint16) 0;

  /* get the size of the file */
  FileSize = Get_File_Size( &NewFilename[ 0 ] );

  /* open the file for reading */
  fp = fopen( &NewFilename[ 0 ], "rb" );

  /* if the file handle is valid */
  if( fp != NULL )
  {

    /* read in the magic number */
    fread( &MagicNumber, sizeof( uint32 ), 1, fp );

    /* read in the pic version number */
    fread( &VersionNumber, sizeof( uint32 ), 1, fp );

    /* fail if the magic/version numbers are not proper */
    if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != PIC_VERSION_NUMBER  ) )
    {
      fclose( fp );
      Msg( "LoadPickupsPositions() Incompatible pickup (.PIC) file %s", &NewFilename[ 0 ] );
      return( FALSE );
    }

    /* read in the number of regen points contained in the file */
    fread( &NumRegenPoints, sizeof( int16 ), 1, fp );

    /* if global RegenPoints list of regen points IS set */
    if( RegenPoints != NULL )
    {
      /* free the memory */
      free( RegenPoints );

      /* for each possible player */
      for( Count = 0; Count < MAX_PLAYERS; Count++ )

        /* if regen slot copy IS set */
        if( RegenSlotsCopy[ Count ] != NULL )

          /* free the memory */
          free( RegenSlotsCopy[ Count ] );

    }

    /* allocate memory for the global which holds the regen points */
    RegenPoints = malloc( sizeof( REGENPOINT ) * NumRegenPoints );

    /* allocate memory for each regen slot copy */
    for( Count = 0; Count < MAX_PLAYERS; Count++ )
      RegenSlotsCopy[ Count ] = malloc( sizeof( REGENPOINT ) * NumRegenPoints );

    /* if the global is not NULL */
    if( RegenPoints != NULL )
    {
      /* loop each regen point in the file */
      for( Count = 0; Count < NumRegenPoints; Count++ )
      {
        /* reset the regen point */
        RegenPoints[ Count ].Status = PU_REGENSTAT_Free;
        RegenPoints[ Count ].Wait = 0.0F;
    
        /* extract each field for the regen point */
        fread( &RegenPoints[ Count ].GenType,  sizeof( uint16 ), 1, fp );
        fread( &RegenPoints[ Count ].RegenType,  sizeof( uint16 ), 1, fp );
        fread( &RegenPoints[ Count ].GenDelay,   sizeof( float ),  1, fp );
        fread( &RegenPoints[ Count ].Lifespan,   sizeof( float ),  1, fp );
        fread( &RegenPoints[ Count ].Pos.x,    sizeof( float ),  1, fp );
        fread( &RegenPoints[ Count ].Pos.y,    sizeof( float ),  1, fp );
        fread( &RegenPoints[ Count ].Pos.z,    sizeof( float ),  1, fp );
        fread( &RegenPoints[ Count ].Group,    sizeof( uint16 ), 1, fp );
        fread( &RegenPoints[ Count ].Type,     sizeof( uint16 ), 1, fp );
        fread( &RegenPoints[ Count ].TriggerMod, sizeof( uint16 ), 1, fp );

        /* if pickup is not valid */
        if(
          /* if pickup is NOT a valid type */
          ( RegenPoints[ Count ].Type >= MAXPICKUPTYPES ) ||
          /* or */
          (
             /* triggerMod is set */
             /* and is NOT valid */
            ( RegenPoints[ Count ].TriggerMod != (uint16) -1 ) &&
            ( RegenPoints[ Count ].TriggerMod >= NumOfTrigMods ) 
          )
        )
        {
          /* quit */
          Msg( "Error loading pickups\nIllegal pickup or trigger\n" );
          fclose( fp );
          return( FALSE );
        }
      }
    }

    /* done reading file */
    fclose( fp );

    /* if not single player */
    if( !( ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
         ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
         ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) ) )
      /* if randomize pickups is set and we have more than one regen point */
      if( RandomPickups && ( NumRegenPoints > 1 ) )
        /* shuffle around regen points */
        RandomizePickups();

    /* for each regeneration point */
    for( Count = 0; Count < NumRegenPoints; Count++ )
    {

      /* if the regen point is outside of its group
         it will be decalred as used and forgetten about
         the pickup it holds will be added to the regenerate list */

      if( ! CheckValidRegenSlot( Count ) )
        continue;

      /* if pickup is NOT valid */
      if( !(

         /* this pickup is on the global valid list */
         FilterPickup( RegenPoints[ Count ].Type ) &&

         /* this pickup if it is flag only good in ctf mode */
         ( (RegenPoints[ Count ].Type != PICKUP_Flag)   || CaptureTheFlag ) && 

         /* this pickup if it is bounty only good in bounty mode) */
         ( (RegenPoints[ Count ].Type != PICKUP_Bounty) || BountyHunt )

         ))
        /* not valid pickup */
      continue;

      /* setup regen point
         based on its GenType */

      switch( RegenPoints[ Count ].GenType )
      {

        /* Initialised Regen Point */
        /* AKA: Regen Points that have their pickups loaded at start */
        case PU_GENTYPE_Initialised:

          /* add one to global count for this type of pickup */
          MaxPickupType[ RegenPoints[ Count ].Type ]++;

          /* try to create the pickup */
          i = InitOnePickup(
                &RegenPoints[ Count ].Pos,
                RegenPoints[ Count ].Group,
                &Dir,
                0.0F,
                RegenPoints[ Count ].Type,
                (uint16) -1,
                ++Ships[WhoIAm].PickupIdCount,
                Count,
                FALSE,
                RegenPoints[ Count ].Lifespan,
                RegenPoints[ Count ].TriggerMod
              );

          break;

        /* timed regen point */
        /* AKA: The Regen point will continously generate the pickup ever N seconds */
        case PU_GENTYPE_Time:

          /* apply the generation delay */
          RegenPoints[ Count ].GenDelay = ( RegenPoints[ Count ].GenDelay * ANIM_SECOND );

          /* add one to global count for this type of pickup */
          MaxPickupType[ RegenPoints[ Count ].Type ]++;

          /* ? */
          RegenPoints[ Count ].PickupID = ++Ships[WhoIAm].PickupIdCount;

          /* set the status of the regen point */
          RegenPoints[ Count ].Status = PU_REGENSTAT_TimeGen;

          /* set timer to regen event */
          RegenPoints[ Count ].Wait = RegenPoints[ Count ].GenDelay;

          break;

        case PU_GENTYPE_Trigger:

          /* add one to global count for this type of pickup */
          MaxPickupType[ RegenPoints[ Count ].Type ]++;

          /* ? */
          RegenPoints[ Count ].PickupID = ++Ships[WhoIAm].PickupIdCount;

          /* set the status of the regen point */
          RegenPoints[ Count ].Status = PU_REGENSTAT_TrigGen;

          break;
      }
    }
  }

  /* minus 1000 from the count of pickups
     why did we even add 1000 further up ?? */

  for( Count = 0; Count < MAXPICKUPTYPES; Count++ )
    MaxPickupType[ Count ] -= 1000;

  /* if were not in the blow modes */
  if(
    !(
	  ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer )			  ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer)
     )
  )
  {
    /* for each of the primary weapons */
    for( Count = 0; Count < MAXPRIMARYWEAPONS; Count++ )
    {
      /* if NOT an enabled weapon */
      if( ! FilterPickup( i ) )
        /* then bail out */
        continue;

      /* this probably garentees that we have at least a 1 to 1 mapping
         for primary ammo and number of primary guns in the level */

      /* if this pickup type */
      if(
        /* has a minimum number of pickups that must always exist*/
        /* this is usually ammo */
        ( MaxPickupType[ Count ] != 0 )  &&
        /* and the minimum is less than the number of current primary weapons */
        ( MaxPickupType[ Count ] < NumPrimaryPickups )
      )
      {
        /* set the minimum number of this type
           to the number of primary pickups */

        MaxPickupType[ Count ] = NumPrimaryPickups;

        /* if we are the game manager */
		if( IsHost )
		{
          /* add N number of these pickups to the current regen list */
          /* N = (minimum that must exist) - (currently existing) */
          NumPrimWeapons[ Count ] += ( MaxPickupType[ Count ] - NumPickupType[ Count ] );
		}
      }
    }
  }

  /* if the minimum ammount of general ammo to allways exist is lower then 3 */
  if( MaxPickupType[ PICKUP_GeneralAmmo ] < 3 )
    /* set the minimum of general ammo to to 3 */
    MaxPickupType[ PICKUP_GeneralAmmo ] = 3;

  /* if the minimum ammount of pryolite ammo to allways exist is 0 */
  /* and we currently have a pyro on the level */
  if( ( MaxPickupType[ PICKUP_PyroliteAmmo ] == 0 ) &&
    ( MaxPickupType[ PICKUP_Pyrolite ] != 0 ) )
    /* set the minimum of pyro ammo that always exists to 2 */
    MaxPickupType[ PICKUP_PyroliteAmmo ] = 2;

  /* if the minimum ammount of suss gun ammo to allways exist is 0 */
  /* and we currently have a suss gun on the level */
  if( ( MaxPickupType[ PICKUP_SussGunAmmo ] == 0 ) &&
    ( MaxPickupType[ PICKUP_SussGun ] != 0 ) )
    /* set the minimum of suss gun ammot that allways exists to 2 */
    MaxPickupType[ PICKUP_SussGunAmmo ] = 2;

  /* if we are in ctf mode */
  if ( CTF )
    /* for each team */
    for ( Count = 0; Count < MAX_TEAMS; Count++ )
      /* set the minimum ammount of flags for this team that allways has to exist to 1 */
      MaxPickupType[ TeamFlagPickup[ Count ] ] = 1;

  /* finally done */
  return( TRUE );

}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:		Initialise a pickup given only regen slot
	Input		:		uint16		Slot Number
	Output		:		uint16		Pickup Index (-1 or -2 error)
===================================================================*/
uint16 InitSlotPickup( uint16 Slot )
{
	uint16		i = (uint16) -1;
	VECTOR		Dir = { 0.0F, 0.0F, 0.0F };

	if( RegenPoints[ Slot ].Status != PU_REGENSTAT_TrigGen )
		return( (uint16) -1 );

	if( RegenPoints[ Slot ].GenDelay )
	{
		RegenPoints[ Slot ].GenDelay = ( RegenPoints[ Slot ].GenDelay * ANIM_SECOND );
		RegenPoints[ Slot ].Status = PU_REGENSTAT_TimeGen;
		RegenPoints[ Slot ].Wait = RegenPoints[ Slot ].GenDelay;
	}
	else
	{
		i =	InitOnePickup( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Dir, 0.0F, RegenPoints[ Slot ].Type,
							(uint16) -1, RegenPoints[ Slot ].PickupID, Slot, TRUE, -1.0F, RegenPoints[ Slot ].TriggerMod );
#if DEBUG_PICKUPS
		if( ( i == (uint16) -1 ) || ( i == (uint16) -2 ) )
		{
			DebugPrintf( "Unable to initialise pickup in slot %d\n", Slot );
		}
#endif
   	}
	return( i );
}

/*===================================================================
	Procedure	:	Regenerate required pickups
	Input		:	nothing
	Output		:	nothing
===================================================================*/
float	RegenDelay = 0.0F;

void RegeneratePickups( void )
{
	int16				Count;
	VECTOR				Dir = { 0.0F, 0.0F, 1.0F };
	uint16				i;
 
	if( RegenDelay != 0.0F )
	{
		RegenDelay -= framelag;
		if( RegenDelay < 0.0F ) RegenDelay = 0.0F;
	}

	RegenerateQuedPickups();

	/* for all regen locations
	   if location is a time based regen
	   check the time and regenerate it */

	for( Count = 0; Count < NumRegenPoints; Count++ )
	{
		if( ( RegenPoints[ Count ].Status == PU_REGENSTAT_Free ) &&
			( RegenPoints[ Count ].Wait > 0.0F ) )
		{
			RegenPoints[ Count ].Wait -= framelag;
			if( RegenPoints[ Count ].Wait < 0.0F ) RegenPoints[ Count ].Wait = 0.0F;
		}

		if( RegenPoints[ Count ].Status == PU_REGENSTAT_TimeGen )
		{
			RegenPoints[ Count ].Wait -= framelag;
			if( RegenPoints[ Count ].Wait < 0.0F )
			{
				if( ( IsHost ) )
				{
					i = InitOnePickup( &RegenPoints[ Count ].Pos, RegenPoints[ Count ].Group,
									   &Dir, 0.0F, RegenPoints[ Count ].Type, (uint16) -1,
									   ++Ships[WhoIAm].PickupIdCount, Count, TRUE,
									   RegenPoints[ Count ].Lifespan, RegenPoints[ Count ].TriggerMod );
		
					if( ( i != (uint16) -1 )  && ( i != (uint16) -2 ) )
					{
						DropPickupSend( &RegenPoints[ Count ].Pos, RegenPoints[ Count ].Group,
										&Dir, 0.0F, RegenPoints[ Count ].Type, Ships[WhoIAm].PickupIdCount,
										Count, FALSE, RegenPoints[ Count ].Lifespan, RegenPoints[ Count ].TriggerMod );
					}
				}
	
				RegenPoints[ Count ].Wait = 0.0F;
			}
		}
	}


	// if single player mode
	// or regen delay is at 0
	if(
		(MyGameStatus == STATUS_SinglePlayer) ||
		(RegenDelay != 0.0F)
	)
	{
		// quit
		return;
	}

	// regenerate ctf flags
	if ( CaptureTheFlag )
	{
		while (
			(FlagsToGenerate > 0) &&
		    RegeneratePickup( PICKUP_Flag )
		)
			FlagsToGenerate--;
	}

	/* regenerate bounty */
	if ( BountyHunt )
	{
		while ( BountyToGenerate > 0 && RegeneratePickup( PICKUP_Bounty ) )
			BountyToGenerate--;
	}

	/* regenerate nitro */
	if( NitroFuelUsed >= ( 50.0F - 0.1F ) )	// Floating point inacuaracy !!!!!
	{
		if( RegeneratePickup( PICKUP_Nitro ) )
		{
			if( NitroFuelUsed <= 50.0F ) NitroFuelUsed = 0.0F;
			else NitroFuelUsed -= 50.0F;
		}
	}

	/* regen primaries */
	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if(
			NumPrimWeapons[ Count ] &&
			RegeneratePickup( (uint16) ( PICKUP_Trojax + ( Count - 1 ) ) )
		)
			NumPrimWeapons[ Count ]--;
	}

	/* regen power pods */
	if(
		NumPowerPods > 0 &&
		RegeneratePickup( PICKUP_PowerPod )
	)
	{
		NumPowerPods--;
	}

	/* host controls regenerating ammo
	   if overall ammo is lower than the level minumum */

	if( IsHost )
	{
		if( NumPickupType[ PICKUP_GeneralAmmo ] < MaxPickupType[ PICKUP_GeneralAmmo ] )
		{
			RegeneratePickup( PICKUP_GeneralAmmo );
		}

		if( NumPickupType[ PICKUP_SussGunAmmo ] < MaxPickupType[ PICKUP_SussGunAmmo ] )
		{
			RegeneratePickup( PICKUP_SussGunAmmo );
		}

		if( NumPickupType[ PICKUP_PyroliteAmmo ] < MaxPickupType[ PICKUP_PyroliteAmmo ] )
		{
			RegeneratePickup( PICKUP_PyroliteAmmo );
		}
	}

	/* regenerate golden */
	if( NumSuperNashrams )
	{
		if( RegeneratePickup( PICKUP_GoldenPowerPod ) ) NumSuperNashrams--;
	}

	if( NumStealths )
	{
		if( RegeneratePickup( PICKUP_Mantle ) ) NumStealths--;
	}

	if( NumInvuls )
	{
		if( RegeneratePickup( PICKUP_Inv ) ) NumInvuls--;
	}

	if( NumOrbs )
	{
		if( RegeneratePickup( PICKUP_Orb ) ) NumOrbs--;
	}

	if( SecAmmoUsed[ SOLARISMISSILE ] >= 3 )
	{
		if( RegeneratePickup( PICKUP_HeatseakerPickup ) )
			SecAmmoUsed[ SOLARISMISSILE ] -= 3;
	}

	if( SecAmmoUsed[ THIEFMISSILE ] )
	{
		if( RegeneratePickup( PICKUP_Thief ) ) SecAmmoUsed[ THIEFMISSILE ] --;
	}

	if( SecAmmoUsed[ SCATTERMISSILE ] )
	{
		if( RegeneratePickup( PICKUP_Scatter ) ) SecAmmoUsed[ SCATTERMISSILE ]--;
	}

	if( SecAmmoUsed[ GRAVGONMISSILE ] )
	{
		if( RegeneratePickup( PICKUP_Gravgon ) ) SecAmmoUsed[ GRAVGONMISSILE ]--;
	}

	if( SecAmmoUsed[ MULTIPLEMISSILE ] >= 50  )
	{
		if( RegeneratePickup( PICKUP_Launcher ) ) SecAmmoUsed[ MULTIPLEMISSILE ] -= 50;
	}

	if( SecAmmoUsed[ TITANSTARMISSILE ] )
	{
		if( RegeneratePickup( PICKUP_TitanStar ) ) SecAmmoUsed[ TITANSTARMISSILE ]--;
	}

	if( SecAmmoUsed[ PURGEMINE ] >= 3 )
	{
		if( RegeneratePickup( PICKUP_PurgePickup ) ) SecAmmoUsed[ PURGEMINE ] -= 3;
	}

	if( SecAmmoUsed[ PINEMINE ] >= 3 )
	{
		if( RegeneratePickup( PICKUP_PinePickup ) ) SecAmmoUsed[ PINEMINE ] -= 3;
	}

	if( SecAmmoUsed[ QUANTUMMINE ] )
	{
		if( RegeneratePickup( PICKUP_QuantumPickup ) ) SecAmmoUsed[ QUANTUMMINE ]--;
	}

	if( SecAmmoUsed[ SPIDERMINE ] >= 3 )
	{
		if( RegeneratePickup( PICKUP_SpiderPod ) ) SecAmmoUsed[ SPIDERMINE ]--;
	}

}

/*===================================================================
	Procedure	:	Process Regen Slots
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessRegenSlots( void )
{
	int16	Count;
	uint16	i;
	VECTOR	Dir = { 0.0F, 0.0F, 0.0F };
 
	for( Count = 0; Count < NumRegenPoints; Count++ )
	{
		switch( RegenPoints[ Count ].Status )
		{
			case PU_REGENSTAT_Free:
				if( RegenPoints[ Count ].Wait > 0.0F )
				{
					RegenPoints[ Count ].Wait -= framelag;
					if( RegenPoints[ Count ].Wait < 0.0F ) RegenPoints[ Count ].Wait = 0.0F;
				}
				break;

			case PU_REGENSTAT_TimeGen:
				if( RegenPoints[ Count ].Wait > 0.0F )
				{
					RegenPoints[ Count ].Wait -= framelag;
					if( RegenPoints[ Count ].Wait < 0.0F )
					{
						RegenPoints[ Count ].Wait = 0.0F;

						i = InitOnePickup( &RegenPoints[ Count ].Pos, RegenPoints[ Count ].Group,
										   &Dir, 0.0F, RegenPoints[ Count ].Type, (uint16) -1,
										   ++Ships[WhoIAm].PickupIdCount, Count, FALSE,
										   RegenPoints[ Count ].Lifespan, RegenPoints[ Count ].TriggerMod );
	
#if DEBUG_PICKUPS
						if( ( i == (uint16) -1 ) || ( i == (uint16) -2 ) )
						{
							DebugPrintf( "Unable to initialise pickup in slot %d\n", Count );
						}
#endif
					}
				}
				break;

			case PU_REGENSTAT_Used:
			case PU_REGENSTAT_TrigGen:
			default:
				break;
		}
	}
}

/*===================================================================
	Procedure	:	Regenerate qued pickups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void RegenerateQuedPickups( void )
{
	int16						Count;
	SHORTPICKUP				*	IP_Ptr;
	SHORTKILLPICKUP			*	KP_Ptr;

	if(
		(!Ships[ WhoIAm ].Pickups)		&&
		(!Ships[ WhoIAm ].RegenSlots)	&&
		(NumMissedPickups)
	)
	{
		for( Count = 0; Count < NumMissedPickups; Count++ )
		{
			switch( MissedPickups[ Count ].Type )
			{
				case MPU_TYPE_INIT:
					IP_Ptr = (SHORTPICKUP *) MissedPickups[ Count ].Struct;
#if DEBUG_PICKUPS
					DebugPrintf( "QUED INIT : " );
#endif
					InitOnePickup( &IP_Ptr->Pos, IP_Ptr->Group,
								   &IP_Ptr->Dir, IP_Ptr->Speed,
								   IP_Ptr->Type, IP_Ptr->Owner,
								   IP_Ptr->ID, IP_Ptr->RegenSlot,
								   IP_Ptr->Sparkle, IP_Ptr->LifeCount,
								   IP_Ptr->TriggerMod );
					break;

				case MPU_TYPE_KILL:
					KP_Ptr = (SHORTKILLPICKUP *) MissedPickups[ Count ].Struct;
#if DEBUG_PICKUPS
					DebugPrintf( "QUED KILL :" );
#endif
					KillPickup( KP_Ptr->Owner, KP_Ptr->ID, KP_Ptr->Style );
					break;
			}
		}

		NumMissedPickups = 0;
		NumMissedInitPickups = 0;
		NumMissedKillPickups = 0;
	}
}

/*===================================================================
	Procedure	:	Regenerate required pickups
	Input		:	uint16		Pickup Type
	Output		:	BOOL		True/False
===================================================================*/

BOOL RegeneratePickup( uint16 Type )
{
	int16				Slot;
	uint16				i;
	VECTOR				Dir = { 0.0F, 0.0F, 0.0F };
	int16				FreeSlots[ MAXPICKUPS ];
	int16				NumFreeSlots;
	int16				FreeTypeSlots[ MAXPICKUPS ];
	int16				NumFreeTypeSlots;

	NumFreeSlots = 0;
	NumFreeTypeSlots = 0;

	/* locate all free regen points
	   if any regen points cator specifically to the given pickup type 
	   then exclusively use them instead of random points */

	/* for each regen point */
	for( Slot = 0; Slot < NumRegenPoints; Slot++ )
	{

		/* if the regen point is NOT free continue */

		if( !( ( RegenPoints[ Slot ].Status == PU_REGENSTAT_Free ) &&
			   ( RegenPoints[ Slot ].Wait == 0.0F ) ))
          continue;

		/* if regen point can only accept one type of pickup */

		if( RegenPoints[ Slot ].RegenType == PU_REGENTYPE_Const )
		{

			/* if given pickup type is alloud here */
			if( RegenPoints[ Slot ].Type == Type )
			{
				/* then add this regen point to the list of free slots */
				FreeTypeSlots[ NumFreeTypeSlots ] = Slot;
				NumFreeTypeSlots++;
			}
		}

		/* if regen location can accept any type of pickup */
		/* skip this if type specific regen locations found for this type */

		else if ( ! NumFreeTypeSlots )
		{

			/* add the location to the free slot list */
			FreeSlots[ NumFreeSlots ] = Slot;
			NumFreeSlots++;

		}

	}

	/* if there is regen points which only hold our specific type of pickup
	   then use them exclusively for this current pickup */

	if( NumFreeTypeSlots )
	{
		
		/* pick a random slot */
		Slot = FreeTypeSlots[ Random_Range( NumFreeTypeSlots ) ];

	}

	
	/* There are no regen points which only cator the current pickup
	   So use any pickup location that doesn't have a defined type */

	else
	{
		
		/* pick a random location */
		Slot = FreeSlots[ Random_Range( NumFreeSlots ) ];
		
     	/* if slots less than 5 ditch out */
		if( NumFreeSlots < 5 )
			return FALSE;

	}

	/* Try to init a pickup */
	i = InitOnePickup( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Dir,
		               0.0F, Type, WhoIAm, ++Ships[WhoIAm].PickupIdCount, Slot,
					   TRUE, -1.0F, (uint16) -1 );

	/* the location given is occupied */
	if ( i == (uint16) -1 )
	{
		DebugPrintf("RegeneratePickup (FAILED) location given is occupied.\n");
		return FALSE;
	}
	
	/* too many of same type */
	if( i == (uint16) -2 ){
		DebugPrintf("RegeneratePickup (FAILED) too many of same type.\n");
		return TRUE; /* dont complain */
	}

	/* tell others in networked game */
	RegenDelay = ( ANIM_SECOND * 0.5F );
	DropPickupSend( &RegenPoints[ Slot ].Pos, RegenPoints[ Slot ].Group, &Dir,
		            0.0F, Type, Ships[WhoIAm].PickupIdCount, Slot, TRUE, -1.0F,
					(uint16) -1 );

	/* Success ! */
	return TRUE;

}

/*===================================================================
	Procedure	:	Generate pickup list for new player
	Input		:	uint16			Ship
				:	SHORTPICKUP	*	Pickup Array to be filled in
				:	BYTE		*	Pickup Number to be filled in
				:	BYTE			Section of pickups
	Output		:	Nothing
===================================================================*/
void GenPickupList( uint16 Ship, SHORTPICKUP * PickupSlots, BYTE * NumPickups, BYTE Section )
{
	int16	Count;
	int16	i;
	int16	NumUsed = 0;

	Section = ( ( ( MAXPICKUPS + ( MAXGENPICKUPCOUNT - 1 ) ) / MAXGENPICKUPCOUNT ) - Section );

	for( Count = 0; Count < MAXGENPICKUPCOUNT; Count++ )
	{
		i = ( Count + ( Section * MAXGENPICKUPCOUNT ) );

		if( i < MAXPICKUPS )
		{
			if( PickupsCopy[ Ship ][ i ].Type != (uint16) -1 )
			{
				PickupSlots[ NumUsed ].Type = PickupsCopy[ Ship ][ i ].Type;
				PickupSlots[ NumUsed ].Group = PickupsCopy[ Ship ][ i ].Group;
				PickupSlots[ NumUsed ].Pos = PickupsCopy[ Ship ][ i ].Pos;
				PickupSlots[ NumUsed ].Owner = PickupsCopy[ Ship ][ i ].Owner;
				PickupSlots[ NumUsed ].ID = PickupsCopy[ Ship ][ i ].ID;
				PickupSlots[ NumUsed ].Dir = PickupsCopy[ Ship ][ i ].Dir;
				PickupSlots[ NumUsed ].Speed = PickupsCopy[ Ship ][ i ].Speed;
				PickupSlots[ NumUsed ].RegenSlot = PickupsCopy[ Ship ][ i ].RegenSlot;
				PickupSlots[ NumUsed ].Sparkle = FALSE;
				PickupSlots[ NumUsed ].LifeCount = PickupsCopy[ Ship ][ i ].LifeCount;
				PickupSlots[ NumUsed ].TriggerMod = PickupsCopy[ Ship ][ i ].TriggerMod;
				NumUsed++;
			}
		}
	}

	*NumPickups = (BYTE) NumUsed;
}

/*===================================================================
	Procedure	:	Generate pickups from list for new player
	Input		:	SHORTPICKUP	*	Pickup Array
				:	BYTE			Number of Pickups
	Output		:	Nothing
===================================================================*/
void RegenPickupList( SHORTPICKUP * Slots, BYTE Num )
{
	int16	Count;

	for( Count = 0; Count < Num; Count++ )
	{
		InitJoinPickup( &Slots[ Count ].Pos, Slots[ Count ].Group, &Slots[ Count ].Dir, Slots[ Count ].Speed,
						Slots[ Count ].Type, Slots[ Count ].Owner, Slots[ Count ].ID, Slots[ Count ].RegenSlot,
						Slots[ Count ].Sparkle, Slots[ Count ].LifeCount, Slots[ Count ].TriggerMod );
	}
}

/*===================================================================
	Procedure	:	Generate RegenSlot list for new player
	Input		:	uint16			Ship
				:	SHORTREGENSLOT*	RegenSlot Array to be filled in
				:	BYTE		*	RegenSlot Number to be filled in
				:	BYTE			Section of RegenSlot
	Output		:	Nothing
===================================================================*/
void GenRegenSlotList( uint16 Ship, SHORTREGENSLOT * RegenSlots, BYTE * NumRegenSlots, BYTE Section )
{
	int16	Count;
	int16	i;
	int16	NumUsed = 0;

#if DEBUG_PICKUPS
	DebugPrintf( "Generate RegenSlot List : %d Regen Slots\n", NumRegenPoints );
#endif

	for( Count = 0; Count < MAXGENREGENSLOTCOUNT; Count++ )
	{
		i = ( Count + ( ( Section - 1 ) * MAXGENREGENSLOTCOUNT ) );

		if( i < NumRegenPoints )
		{
			memcpy( RegenSlots, ( RegenSlotsCopy[ Ship ] ) + i, sizeof( REGENPOINT ) );
			RegenSlots++;
			NumUsed++;

#if DEBUG_PICKUPS
			DebugPrintf( "Section %d, Index %d\n", ( Section - 0 ), i );
#endif
		}
	}

	*NumRegenSlots = (BYTE) NumUsed;
}

/*===================================================================
	Procedure	:	Regeneralte RegenSlots from list for new player
	Input		:	SHORTREGENSLOT*	RegenSlot Array
				:	BYTE			Number of RegenSlots
	Output		:	Nothing
===================================================================*/
void RegenRegenSlotList( SHORTREGENSLOT * Slots, BYTE Num )
{
	REGENPOINT	*	Ptr;

#if DEBUG_PICKUPS
	DebugPrintf( "Regenerate RegenSlot List : %d Regen Slots\n", NumRegenPoints );
#endif

	if( RegenPoints )
	{
		Ptr = RegenPoints + ( ( Ships[ WhoIAm ].RegenSlots + 0 ) * MAXGENREGENSLOTCOUNT );

#if DEBUG_PICKUPS
		DebugPrintf( "Section %d, Number %d\n", ( Ships[ WhoIAm ].RegenSlots + 0 ), Num );
#endif
		memcpy( Ptr, Slots, ( sizeof( REGENPOINT ) * Num ) );
	}
}

/*===================================================================
	Procedure	:	Kill All Pickups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/

void KillAllPickups( void )
{
	uint16	i;
	uint16	NextPickup;

	/* get the first pickup */
	i = FirstPickupUsed;

	/* if pointer points to a valid pickup id */
	while( i != (uint16) -1 )
	{

		/* get the next pickup */
		NextPickup = Pickups[ i ].Prev;
 
		/* Reset the pickups triggerMod */
		Pickups[ i ].TriggerModPtr = NULL;
		Pickups[ i ].TriggerMod    = (uint16) -1;

		/* cleanup the pickup
		   remove any lighting or other affect the pickup does */

		CleanUpPickup( i );
 		
		/* go to the next pickup */
		i = NextPickup;

	}																				
}

/*===================================================================
	Procedure	:	Copy Pickups into copy of pickups
	Input		:	uint16		Player
	Output		:	Nothing
===================================================================*/
void CopyPickups( uint16 Player )
{
	uint16	i;
	int16	Num = 0;

	for( i = 0; i < MAXPICKUPS; i++ )
	{
		PickupsCopy[ Player ][ i ].Type = (uint16) -1;
	}

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		memcpy( &PickupsCopy[ Player ][ Num ], &Pickups[i], sizeof( PICKUP ) );
		Num++;

		i = Pickups[ i ].Prev;							/* Next Secondary Bullet */
	}																				
}

/*===================================================================
	Procedure	:	Copy RegenSlots into copy of regenslots
	Input		:	uint16		Player
	Output		:	Nothing
===================================================================*/
void CopyRegenSlots( uint16 Player )
{
	REGENPOINT	*	Ptr;

	Ptr = RegenSlotsCopy[ Player ];

	if( Ptr != NULL )
	{
		memcpy( Ptr, RegenPoints, ( NumRegenPoints * sizeof( REGENPOINT ) ) );
	}
	else
	{
#if DEBUG_PICKUPS
		DebugPrintf( "No regen point copy slot!\n" );
#endif
	}
}

/*===================================================================
	Procedure	:	Enable All Pickup Lights
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void EnablePickupLights( void )
{
	uint16	i;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		if( Pickups[ i ].Light != (uint16) -1 )
		{
			XLights[ Pickups[ i ].Light ].Visible = TRUE;
		}
 		
		i = Pickups[ i ].Prev;							/* Next Pickup */
	}																				
}

/*===================================================================
	Procedure	:	Enable All Pickup Lights
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DisablePickupLights( void )
{
	uint16	i;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		if( Pickups[ i ].Light != (uint16) -1 )
		{
			XLights[ Pickups[ i ].Light ].Visible = FALSE;
		}

		i = Pickups[ i ].Prev;							/* Next Pickup */
	}																				
}

/*===================================================================
	Procedure	:	Add Pickup to regeneration que
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void AddPickupToRegen( uint16 Type )
{
	switch( Type )
	{
		case PICKUP_Trojax:
			NumPrimWeapons[ TROJAX ]++;
			break;

		case PICKUP_Pyrolite:
			NumPrimWeapons[ PYROLITE_RIFLE ]++;
			break;

		case PICKUP_Transpulse:
			NumPrimWeapons[ TRANSPULSE_CANNON ]++;
			break;

		case PICKUP_SussGun:
			NumPrimWeapons[ SUSS_GUN ]++;
			break;

		case PICKUP_Laser:
			NumPrimWeapons[ LASER ]++;
			break;

		case PICKUP_PowerPod:
			NumPowerPods++;
			break;

		case PICKUP_GoldenPowerPod:
			NumSuperNashrams++;
			break;

		case PICKUP_PurgePickup:
			SecAmmoUsed[ PURGEMINE ] += 3;
			break;

		case PICKUP_PinePickup:
			SecAmmoUsed[ PINEMINE ] += 3;
			break;

		case PICKUP_QuantumPickup:
			SecAmmoUsed[ QUANTUMMINE ] += 1;
			break;

		case PICKUP_SpiderPod:
			SecAmmoUsed[ SPIDERMINE ] += 3;
			break;

		case PICKUP_Gravgon:
			SecAmmoUsed[ GRAVGONMISSILE ] += 1;
			break;

		case PICKUP_TitanStar:
			SecAmmoUsed[ TITANSTARMISSILE ] += 1;
			break;

		case PICKUP_Scatter:
			SecAmmoUsed[ SCATTERMISSILE ] += 1;
			break;

		case PICKUP_Thief:
			SecAmmoUsed[ THIEFMISSILE ] += 1;
			break;

		case PICKUP_Launcher:
			SecAmmoUsed[ MULTIPLEMISSILE ] += 50;
			break;

		case PICKUP_HeatseakerPickup:
			SecAmmoUsed[ SOLARISMISSILE ] += 3;
			break;

		case PICKUP_Mugs:
			SecAmmoUsed[ MUGMISSILE ] += 3;
			break;

		case PICKUP_Mantle:
			NumStealths++;
			break;

		case PICKUP_Inv:
			NumInvuls++;
			break;

		case PICKUP_Orb:
			NumOrbs++;
			break;
	}
}

/*===================================================================
	Procedure	:	Enable All Pickup Lights
	Input		:	Nothing
	Output		:	uint16	Pickup Index ( -1 = None )
===================================================================*/
uint16 FindClosestPickup( void )
{
	uint16	i;
	VECTOR	DistVector;
	uint16	ClosestPickup = (uint16) -1;
	float	ClosestDist;
	float	Dist;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		DistVector.x = ( Pickups[ i ].Pos.x - Ships[ WhoIAm ].Object.Pos.x );
		DistVector.y = ( Pickups[ i ].Pos.y - Ships[ WhoIAm ].Object.Pos.y );
		DistVector.z = ( Pickups[ i ].Pos.z - Ships[ WhoIAm ].Object.Pos.z );
		Dist = VectorLength( &DistVector );

		if( ClosestPickup != (uint16) -1 )
		{
			if( Dist < ClosestDist )
			{
				ClosestDist = Dist;
				ClosestPickup = i;
			}
		}
		else
		{
			ClosestDist = Dist;
			ClosestPickup = i;
		}

		i = Pickups[ i ].Prev;							/* Next Pickup */
	}																				

	return( ClosestPickup );
}

/*===================================================================
	Procedure	:	Find Closest Ship
	Input		:	Nothing
	Output		:	uint16		Ship Index ( -1 If none hit )
===================================================================*/
uint16 FindClosestShip( void )
{
	uint16		Count;
	VECTOR		TempVector;
	float		DistToSphere;
	uint16		ClosestShip = (uint16) -1;
	float		ClosestLength;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if ( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) )&& ( Count != WhoIAm ) )
		{
			if( Ships[ Count ].Object.Mode == NORMAL_MODE )
			{
				TempVector.x = ( Ships[ Count ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
				TempVector.y = ( Ships[ Count ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
				TempVector.z = ( Ships[ Count ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
			
				DistToSphere = VectorLength( &TempVector );
			
				if( ClosestShip != (uint16) -1 )
				{
					if( DistToSphere < ClosestLength )
					{
						ClosestLength = DistToSphere;
						ClosestShip = Count;
					}
				}
				else
				{
					ClosestLength = DistToSphere;
					ClosestShip = Count;
				}
			}
		}
	}

	return( ClosestShip );
}


/*===================================================================
	Procedure	:	Play Sfx for Collect Pickup from scene
	Input		:	uint16	pickup to get
	Output		:	BOOL	True/False
===================================================================*/
void SfxForCollectPickup( uint16 Owner, uint16 ID )
{
	uint16	i;
	uint16	NextPickup;

	float	Sfx_Volume = 1.0F;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		if( ( Pickups[i].Owner == Owner ) && ( Pickups[i].ID == ID ) )
		{
			switch( Pickups[i].Type )
			{
			case PICKUP_Trojax: 
				PlaySfx( SFX_Select_Trojax, Sfx_Volume );
				return;
			case PICKUP_Pyrolite:
				PlaySfx( SFX_Select_Pyrolite, Sfx_Volume );
				return;
			case PICKUP_Transpulse:
				PlaySfx( SFX_Select_Transpulse, Sfx_Volume );
				return;
			case PICKUP_SussGun:
				PlaySfx( SFX_Select_SussGun, Sfx_Volume );
				return;
			case PICKUP_Laser:
				PlaySfx( SFX_Select_BeamLaser, Sfx_Volume );
				return;
			case PICKUP_Mug:
				return;
			case PICKUP_Mugs:
				PlaySfx( SFX_Select_MugMissile, Sfx_Volume );
				return;
			case PICKUP_Heatseaker:
				return;
			case PICKUP_HeatseakerPickup:
				PlaySfx( SFX_Select_SolarisMissile, Sfx_Volume );
				return;
			case PICKUP_Thief:
//				PlaySfx( SFX_Select_ThiefMissile, Sfx_Volume );
				return;
			case PICKUP_Scatter:
				PlaySfx( SFX_Select_ScatterMissile, Sfx_Volume );
				return;
			case PICKUP_Gravgon:
				PlaySfx( SFX_Select_GravgonMissile, Sfx_Volume );
				return;
			case PICKUP_Launcher:
				PlaySfx( SFX_Select_MFRL, Sfx_Volume );
				return;
			case PICKUP_TitanStar:
				PlaySfx( SFX_Select_TitanStarMissile, Sfx_Volume );
				return;
			case PICKUP_PurgePickup:
				PlaySfx( SFX_Select_PurgeMine, Sfx_Volume );
				return;
			case PICKUP_PinePickup:
				PlaySfx( SFX_Select_PineMine, Sfx_Volume );
				return;
			case PICKUP_QuantumPickup:
				PlaySfx( SFX_Select_QuantumMine, Sfx_Volume );
				return;
			case PICKUP_SpiderPod:
				return;
			case PICKUP_Parasite:
				return;
			case PICKUP_Flare:
				return;
			case PICKUP_GeneralAmmo:
				PlaySfx( SFX_Select_Ammo, Sfx_Volume );
				return;
			case PICKUP_PyroliteAmmo:
				PlaySfx( SFX_Select_Ammo, Sfx_Volume );
				return;
			case PICKUP_SussGunAmmo:
				PlaySfx( SFX_Select_Ammo, Sfx_Volume );
				return;
			case PICKUP_PowerPod:
				PlaySfx( SFX_Select_PowerPod, Sfx_Volume );
				return;
			case PICKUP_GoldenPowerPod:
				PlaySfx( SFX_Select_GoldenPowerPod, Sfx_Volume );
				return;
			case PICKUP_Shield:
				PlaySfx( SFX_Select_Shield, Sfx_Volume );
				return;
			case PICKUP_Inv:
				PlaySfx( SFX_Select_Invul, Sfx_Volume );
				return;
			case PICKUP_ExtraLife:
				return;
			case PICKUP_Computer:
				return;
			case PICKUP_Smoke:
				return;
			case PICKUP_Nitro:
				PlaySfx( SFX_Select_Nitro, Sfx_Volume );
				return;
			case PICKUP_Goggles:
				return;
			case PICKUP_Gold:
				return;
			case PICKUP_Mantle:
				PlaySfx( SFX_Cloaking, Sfx_Volume );
				return;
			case PICKUP_Crystal:
				return;
			case PICKUP_Orb:
				PlaySfx( SFX_Orbital, Sfx_Volume );
				return;
			case PICKUP_DNA:
				return;
			case PICKUP_SkeletonKey:
				return;
			case PICKUP_Bomb:
				return;
			case PICKUP_GoldFigure:
				return;
			}
		}

		i = NextPickup;											/* Next Pickup */
	}
}

/*===================================================================
	Procedure	:	Setup Pickup group link list
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SetupPickupGroups( void )
{
	int16	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		PickupGroups[ Count ] = NULL;
		NumPickupsPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add Pickup to group link list
	Input		:	uint16		Pickup Index
				:	uint16		Group
	Output		:	Nothing
===================================================================*/
void AddPickupToGroup( uint16 i, uint16 Group )
{
	Pickups[ i ].PrevInGroup = NULL;
	Pickups[ i ].NextInGroup = PickupGroups[ Group ];
	if( Pickups[ i ].NextInGroup ) Pickups[ i ].NextInGroup->PrevInGroup = &Pickups[ i ];
	PickupGroups[ Group ] = &Pickups[ i ];
	NumPickupsPerGroup[ Group ]++;
}

/*===================================================================
	Procedure	:	Remove Pickup from group link list
	Input		:	uint16		Pickup Index
				:	uint16		Group
	Output		:	Nothing
===================================================================*/
void RemovePickupFromGroup( uint16 i, uint16 Group )
{
	if( Pickups[ i ].PrevInGroup ) Pickups[ i ].PrevInGroup->NextInGroup = Pickups[ i ].NextInGroup;
	else PickupGroups[ Group ] = Pickups[ i ].NextInGroup;
	if( Pickups[ i ].NextInGroup ) Pickups[ i ].NextInGroup->PrevInGroup = Pickups[ i ].PrevInGroup;
	Pickups[ i ].PrevInGroup = NULL;
	Pickups[ i ].NextInGroup = NULL;
	NumPickupsPerGroup[ Group ]--;
}

/*===================================================================
	Procedure	:	Move Pickup from 1 group to another
	Input		:	uint16		Pickup Index
				:	uint16		OldGroup
				:	uint16		NewGroup
	Output		:	Nothing
===================================================================*/
void MovePickupToGroup( uint16 i, uint16 OldGroup, uint16 NewGroup )
{
	RemovePickupFromGroup( i, OldGroup );
	AddPickupToGroup( i, NewGroup );
}

void InitValidPickups()
{
	int i;
	
	for ( i = 0; i < MAXPICKUPTYPES; i++ )
	{
		PickupValid[ i ] = TRUE;
	}
}

/*===================================================================
	Procedure	:	Filter Pickup
	Input		:	uint16		Pickup Type
	Output		:	BOOL		( True/False ( Allowed/Not )
===================================================================*/
BOOL FilterPickup( uint16 PickupType )
{
	// if in these modes
	if( ( ChangeLevel_MyGameStatus == STATUS_StartingMultiplayer ) ||
		( ChangeLevel_MyGameStatus == STATUS_WaitingAfterScore ) ||
		( ChangeLevel_MyGameStatus == STATUS_PreStartingMultiplayerSynch ) ||
		( ChangeLevel_MyGameStatus == STATUS_StartingMultiplayerSynch ) ||
		( ChangeLevel_MyGameStatus == STATUS_ChangeLevelPostInitView ) ||
		( ChangeLevel_MyGameStatus == STATUS_ChangeLevelPostPlayingDemo )
		)
	{
		// valid pickup type
		if( PickupValid[ PickupType ] )
		{
			// allow
			return( TRUE );
		}
		else
		{
			// deny
			return( FALSE );
		}
	}

	// allow
	return( TRUE );
}

void PackPickupInfo( uint32 *packed )
{
	uint8 i;
	uint32 temp;
	int psize = sizeof( packed[ 0 ] ) * 8;

	for ( i = 0; i < MAX_PICKUPFLAGS; i++ )
	{
		packed[ i ] = 0;
	}
	for ( i = 0; i < MAXPICKUPTYPES; i++ )
	{
		if ( PickupValid[ i ] )
		{
			temp = ( 1 << ( i % psize ) );
			packed[ i / psize ] |= temp;
		}
	}
}

void UnpackPickupInfo( uint32 *packed )
{
	uint8 i;
	int psize = sizeof( packed[ 0 ] ) * 8;
	
	for ( i = 0; i < MAXPICKUPTYPES; i++ )
	{
		PickupValid[ i ] = ( packed[ i / psize ] & ( 1 << ( i % psize ) ) ) ? TRUE : FALSE;
	}
}

/*===================================================================
	Procedure	:	Save Pickups Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveAllPickups( FILE * fp )
{
	uint16	i;
	uint16	TempIndex = (uint16) -1; 

	if( fp )
	{
		for( i = 0; i < MAXGROUPS; i++ )
		{
			fwrite( &NumPickupsPerGroup[ i ], sizeof( int16 ), 1, fp );
			if( PickupGroups[ i ] != NULL ) fwrite( &PickupGroups[ i ]->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
		}

		for( i = 0; i < MAXPICKUPTYPES; i++ )
		{
			fwrite( &NumPickupType[ i ], sizeof( int16 ), 1, fp );
			fwrite( &MaxPickupType[ i ], sizeof( int16 ), 1, fp );
			fwrite( &PickupsGot[ i ], sizeof( int16 ), 1, fp );
			fwrite( &PickupValid[ i ], sizeof( BOOL ), 1, fp );
		}

		for( i = 0; i < MAXPRIMARYWEAPONS; i++ )
		{
			fwrite( &NumPrimWeapons[ i ], sizeof( int16 ), 1, fp );
		}

		fwrite( &NitroFuel, sizeof( NitroFuel ), 1, fp );
		fwrite( &NitroFuelUsed, sizeof( NitroFuelUsed ), 1, fp );
		fwrite( &NumStealths, sizeof( int16 ), 1, fp );
		fwrite( &NumInvuls, sizeof( int16 ), 1, fp );
		fwrite( &NumSuperNashrams, sizeof( int16 ), 1, fp );
		fwrite( &NumOrbs, sizeof( int16 ), 1, fp );
		fwrite( &NumPowerPods, sizeof( int16 ), 1, fp );
		fwrite( &PickupInvulnerability, sizeof( BOOL ), 1, fp );
		fwrite( &NumGoldBars, sizeof( int16 ), 1, fp );
		fwrite( &FirstPickupUsed, sizeof( uint16 ), 1, fp );
		fwrite( &FirstPickupFree, sizeof( uint16 ), 1, fp );
		fwrite( &NumRegenPoints, sizeof( int16 ), 1, fp );
		fwrite( &CrystalsFound, sizeof( CrystalsFound ), 1, fp );

		for( i = 0; i < NumRegenPoints; i++ )
		{
			fwrite( &RegenPoints[ i ].GenType, sizeof( uint16 ), 1, fp );
			fwrite( &RegenPoints[ i ].RegenType, sizeof( uint16 ), 1, fp );
			fwrite( &RegenPoints[ i ].GenDelay, sizeof( float ), 1, fp );
			fwrite( &RegenPoints[ i ].Lifespan, sizeof( float ), 1, fp );
			fwrite( &RegenPoints[ i ].Wait, sizeof( float ), 1, fp );
			fwrite( &RegenPoints[ i ].Status, sizeof( int16 ), 1, fp );
			fwrite( &RegenPoints[ i ].Group, sizeof( int16 ), 1, fp );
			fwrite( &RegenPoints[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &RegenPoints[ i ].Type, sizeof( uint16 ), 1, fp );
			fwrite( &RegenPoints[ i ].TriggerMod, sizeof( uint16 ), 1, fp );
			fwrite( &RegenPoints[ i ].PickupIndex, sizeof( uint16 ), 1, fp );
			fwrite( &RegenPoints[ i ].PickupID, sizeof( uint16 ), 1, fp );
		}

		i = FirstPickupUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &Pickups[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Prev, sizeof( uint16 ), 1, fp );
			if( Pickups[i].NextInGroup ) fwrite( &Pickups[ i ].NextInGroup->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
			if( Pickups[i].PrevInGroup ) fwrite( &Pickups[ i ].PrevInGroup->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Index, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Type, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Owner, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ID, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Mode, sizeof( int16 ), 1, fp );
			fwrite( &Pickups[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].PickupCount, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Rot, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].ExternalSpeed, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Group, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ModelType, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &Pickups[ i ].Fmpoly, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Xsize, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Ysize, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].Light, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].Lightsize, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].R, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].G, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].B, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fwrite( &Pickups[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].RegenSlot, sizeof( int16 ), 1, fp );
			fwrite( &Pickups[ i ].TriggerMod, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fwrite( &Pickups[ i ].ColDist, sizeof( float ), 1, fp );
			fwrite( &Pickups[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fwrite( &Pickups[ i ].ColPoint, sizeof( VERT ), 1, fp );
			fwrite( &Pickups[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fwrite( &Pickups[ i ].CouldNotPickup, sizeof( BOOL ), 1, fp );
			i = Pickups[ i ].Prev;
		}

		i = FirstPickupFree;

		while( i != (uint16) -1 )
		{
			fwrite( &Pickups[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Pickups[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load Pickups Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadAllPickups( FILE * fp )
{
	uint16	i;
	uint16	TempIndex;

	if( fp )
	{
		for( i = 0; i < MAXGROUPS; i++ )
		{
			fread( &NumPickupsPerGroup[ i ], sizeof( int16 ), 1, fp );
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) PickupGroups[ i ] = &Pickups[ TempIndex ];
			else PickupGroups[ i ] = NULL;
		}

		for( i = 0; i < MAXPICKUPTYPES; i++ )
		{
			fread( &NumPickupType[ i ], sizeof( int16 ), 1, fp );
			fread( &MaxPickupType[ i ], sizeof( int16 ), 1, fp );
			fread( &PickupsGot[ i ], sizeof( int16 ), 1, fp );
			fread( &PickupValid[ i ], sizeof( BOOL ), 1, fp );
		}

		for( i = 0; i < MAXPRIMARYWEAPONS; i++ )
		{
			fread( &NumPrimWeapons[ i ], sizeof( int16 ), 1, fp );
		}

		fread( &NitroFuel, sizeof( NitroFuel ), 1, fp );
		fread( &NitroFuelUsed, sizeof( NitroFuelUsed ), 1, fp );
		fread( &NumStealths, sizeof( int16 ), 1, fp );
		fread( &NumInvuls, sizeof( int16 ), 1, fp );
		fread( &NumSuperNashrams, sizeof( int16 ), 1, fp );
		fread( &NumOrbs, sizeof( int16 ), 1, fp );
		fread( &NumPowerPods, sizeof( int16 ), 1, fp );
		fread( &PickupInvulnerability, sizeof( BOOL ), 1, fp );
		fread( &NumGoldBars, sizeof( int16 ), 1, fp );
		fread( &FirstPickupUsed, sizeof( uint16 ), 1, fp );
		fread( &FirstPickupFree, sizeof( uint16 ), 1, fp );
		fread( &NumRegenPoints, sizeof( int16 ), 1, fp );
		fread( &CrystalsFound, sizeof( CrystalsFound ), 1, fp );

		for( i = 0; i < NumRegenPoints; i++ )
		{
			fread( &RegenPoints[ i ].GenType, sizeof( uint16 ), 1, fp );
			fread( &RegenPoints[ i ].RegenType, sizeof( uint16 ), 1, fp );
			fread( &RegenPoints[ i ].GenDelay, sizeof( float ), 1, fp );
			fread( &RegenPoints[ i ].Lifespan, sizeof( float ), 1, fp );
			fread( &RegenPoints[ i ].Wait, sizeof( float ), 1, fp );
			fread( &RegenPoints[ i ].Status, sizeof( int16 ), 1, fp );
			fread( &RegenPoints[ i ].Group, sizeof( int16 ), 1, fp );
			fread( &RegenPoints[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &RegenPoints[ i ].Type, sizeof( uint16 ), 1, fp );
			fread( &RegenPoints[ i ].TriggerMod, sizeof( uint16 ), 1, fp );
			fread( &RegenPoints[ i ].PickupIndex, sizeof( uint16 ), 1, fp );
			fread( &RegenPoints[ i ].PickupID, sizeof( uint16 ), 1, fp );
		}

		i = FirstPickupUsed;

		while( i != (uint16) -1 )
		{
			fread( &Pickups[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) Pickups[ i ].NextInGroup = &Pickups[ TempIndex ];
			else Pickups[ i ].NextInGroup = NULL;
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) Pickups[ i ].PrevInGroup = &Pickups[ TempIndex ];
			else Pickups[ i ].PrevInGroup = NULL;
			fread( &Pickups[ i ].Index, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Type, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Owner, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].ID, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Mode, sizeof( int16 ), 1, fp );
			fread( &Pickups[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].PickupCount, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].Rot, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].Speed, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].ExternalSpeed, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].Group, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].ModelType, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &Pickups[ i ].Fmpoly, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Xsize, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].Ysize, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].Light, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].Lightsize, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].R, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].G, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].B, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fread( &Pickups[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].RegenSlot, sizeof( int16 ), 1, fp );
			fread( &Pickups[ i ].TriggerMod, sizeof( uint16 ), 1, fp );
			if( Pickups[ i ].TriggerMod == (uint16) -1 ) Pickups[ i ].TriggerModPtr = NULL;
			else Pickups[ i ].TriggerModPtr = &TrigMods[ Pickups[ i ].TriggerMod ];
			fread( &Pickups[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fread( &Pickups[ i ].ColDist, sizeof( float ), 1, fp );
			fread( &Pickups[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fread( &Pickups[ i ].ColPoint, sizeof( VERT ), 1, fp );
			fread( &Pickups[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fread( &Pickups[ i ].CouldNotPickup, sizeof( BOOL ), 1, fp );
			i = Pickups[ i ].Prev;
		}

		i = FirstPickupFree;

		while( i != (uint16) -1 )
		{
			memset( &Pickups[ i ], 0, sizeof( PICKUP ) );
			Pickups[ i ].Prev = (uint16) -1;
			Pickups[ i ].NextInGroup = NULL;
			Pickups[ i ].PrevInGroup = NULL;
			Pickups[ i ].Index = i;
			Pickups[ i ].LifeCount = -1.0F;
			Pickups[ i ].Mode = PICKUPMODE_Normal;
			Pickups[ i ].Type = (uint16) -1;
			Pickups[ i ].Rot.x = -1.2F;
			Pickups[ i ].Rot.y = 2.0F;
			Pickups[ i ].Rot.z = 1.0F;
			Pickups[ i ].Group = 0;
			Pickups[ i ].DirVector = Forward;
			Pickups[ i ].UpVector = SlideUp;
			Pickups[ i ].RegenSlot = -1;
			Pickups[ i ].TriggerModPtr = NULL;
			Pickups[ i ].TriggerMod = (uint16) -1;
			QuatFrom2Vectors( &Pickups[ i ].DirQuat, &Forward, &Pickups[ i ].DirVector );
			QuatToMatrix( &Pickups[ i ].DirQuat, &Pickups[ i ].Mat );

			fread( &Pickups[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Pickups[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Kill all pickups of a specific type
	Input		:	uint16	Type
				:	int16	Style to kill
	Output		:	nothing
===================================================================*/
void KillAllPickupsOfType( uint16 Type, int16 Style )
{
	uint16	i;
	uint16	NextPickup;
	uint16	fmpoly;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		if( Pickups[ i ].Type == Type )
		{
			switch( Style )
			{
				case PICKUPKILL_Immediate:
					if ( CTF )
					{
						int team;

						team = -1;
						switch ( Pickups[ i ].Type )
						{
						case PICKUP_Flag1:
							team = 0;
							break;
						case PICKUP_Flag2:
							team = 1;
							break;
						case PICKUP_Flag3:
							team = 2;
							break;
						case PICKUP_Flag4:
							team = 3;
							break;
						}
						if ( team >= 0 )
						{
							TeamFlagAtHome[ team ] = FALSE;
						}
					}
					CleanUpPickup( i );
					break;

				case PICKUPKILL_Disappear:
					if( Pickups[ i ].Mode == PICKUPMODE_Disappear ) break;

					Pickups[ i ].LifeCount = 0.0F;
					Pickups[ i ].Mode = PICKUPMODE_Disappear;
					Pickups[ i ].PickupCount = 32.0F;
			
					fmpoly = FindFreeFmPoly();
					if( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 1000.0F;
						FmPolys[ fmpoly ].Pos = Pickups[ i ].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_PICKUP_REGEN;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].Frm_Info = &PickupRegen_Header;
						FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
						FmPolys[ fmpoly ].Trans = 192;
						FmPolys[ fmpoly ].Dir.x = 0.0F;
						FmPolys[ fmpoly ].Dir.y = 0.0F;
						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].R = 192;
						FmPolys[ fmpoly ].G = 192;
						FmPolys[ fmpoly ].B = 192;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].Rot = 0.0F;
						FmPolys[ fmpoly ].Group = Pickups[ i ].Group;
						FmPolys[ fmpoly ].RotSpeed = 0.0F;
						FmPolys[ fmpoly ].xsize = PICKUP_REGEN_SIZE;
						FmPolys[ fmpoly ].ysize = PICKUP_REGEN_SIZE;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
					break;
			}
		}

		i = NextPickup;											/* Next Pickup */
	}																				
}

/*===================================================================
	Procedure	:	Kill all pickups of a specific type and send to
				:	all players
	Input		:	uint16	Type
				:	int16	Style to kill
	Output		:	nothing
===================================================================*/
void KillAllPickupsOfTypeAndSend( uint16 Type, int16 Style )
{
	uint16	i;
	uint16	NextPickup;
	uint16	fmpoly;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		if( Pickups[ i ].Type == Type )
		{
			switch( Style )
			{
				case PICKUPKILL_Immediate:
					if ( CTF )
					{
						int team;

						team = -1;
						switch ( Pickups[ i ].Type )
						{
						case PICKUP_Flag1:
							team = 0;
							break;
						case PICKUP_Flag2:
							team = 1;
							break;
						case PICKUP_Flag3:
							team = 2;
							break;
						case PICKUP_Flag4:
							team = 3;
							break;
						}
						if ( team >= 0 )
						{
							TeamFlagAtHome[ team ] = FALSE;
						}
					}

					KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
					CleanUpPickup( i );
					break;

				case PICKUPKILL_Disappear:
					if( Pickups[ i ].Mode == PICKUPMODE_Disappear ) break;

					KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Disappear );

					Pickups[ i ].LifeCount = 0.0F;
					Pickups[ i ].Mode = PICKUPMODE_Disappear;
					Pickups[ i ].PickupCount = 32.0F;
			
					fmpoly = FindFreeFmPoly();
					if( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 1000.0F;
						FmPolys[ fmpoly ].Pos = Pickups[ i ].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_PICKUP_REGEN;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].Frm_Info = &PickupRegen_Header;
						FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
						FmPolys[ fmpoly ].Trans = 192;
						FmPolys[ fmpoly ].Dir.x = 0.0F;
						FmPolys[ fmpoly ].Dir.y = 0.0F;
						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].R = 192;
						FmPolys[ fmpoly ].G = 192;
						FmPolys[ fmpoly ].B = 192;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].Rot = 0.0F;
						FmPolys[ fmpoly ].Group = Pickups[ i ].Group;
						FmPolys[ fmpoly ].RotSpeed = 0.0F;
						FmPolys[ fmpoly ].xsize = PICKUP_REGEN_SIZE;
						FmPolys[ fmpoly ].ysize = PICKUP_REGEN_SIZE;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
					break;
			}
		}

		i = NextPickup;											/* Next Pickup */
	}																				
}

/*===================================================================
	Procedure	:	Check if model for pickup is valid
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void PickupModelValid( void )
{
	uint16	i;
	uint16	NextPickup;
	uint16	Model;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		NextPickup = Pickups[ i ].Prev;							/* Next Pickup */

		Model = Pickups[ i ].ModelNum;

		if( Model != (uint16) -1 )
		{
			if( Models[ Model ].ModelNum != PickupAttribs[ Pickups[ i ].Type ].ModelType )
			{
				Msg( "Model for Pickup %s invalid ( %s )\n", Messages[ Pickups[ i ].Type ], &ModelNames[ Models[ Model ].ModelNum ].Name[ 0 ] );
			}
		}

		i = NextPickup;											/* Next Pickup */
	}																				
}

#if 1

/*===================================================================
	Procedure	:	Collect Pickup from scene
	Input		:	uint16	Pickup to get
	Output		:	BOOL	True/False
===================================================================*/
BOOL CanPlayerCollectPickup( uint16 i, uint16 Player )
{
	int16	PickupEnable = FALSE;
	int16	Temp;
	float	TempFloat;

	if( i != (uint16) -1 )
	{
		switch( Pickups[i].Type )
		{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Trojax: 
				if( !Host_PrimaryWeaponsGot[ Player ][ TROJAX ] )
				{
					Host_PrimaryWeaponsGot[ Player ][ TROJAX ] = 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Pyrolite:
				if( !Host_PrimaryWeaponsGot[ Player ][ PYROLITE_RIFLE ] )
				{
					Host_PrimaryWeaponsGot[ Player ][ PYROLITE_RIFLE ] = 1;

//					if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
					if( Ships[ Player ].SuperNashramTimer != 0.0F )
					{
						Host_CopyOfPyroliteAmmo[ Player ] = MAXPYROLITEAMMO;
					}
					else
					{
						Host_PyroliteAmmo[ Player ] = MAXPYROLITEAMMO;
					}
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Transpulse:
				if( !Host_PrimaryWeaponsGot[ Player ][ TRANSPULSE_CANNON ] )
				{
					Host_PrimaryWeaponsGot[ Player ][ TRANSPULSE_CANNON ] = 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGun:
				if( !Host_PrimaryWeaponsGot[ Player ][ SUSS_GUN ] )
				{
					Host_PrimaryWeaponsGot[ Player ][ SUSS_GUN ] = 1;

//					if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
					if( Ships[ Player ].SuperNashramTimer != 0.0F )
					{
						Host_CopyOfSussGunAmmo[ Player ] = MAXSUSSGUNAMMO;
					}
					else
					{
						Host_SussGunAmmo[ Player ] = MAXSUSSGUNAMMO;
					}

					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Laser:
				if( !Host_PrimaryWeaponsGot[ Player ][ LASER ] )
				{
					Host_PrimaryWeaponsGot[ Player ][ LASER ] = 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mug:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mugs:
				if( Host_SecondaryAmmo[ Player ][ MUGMISSILE ] < 10 )
				{
					Host_SecondaryWeaponsGot[ Player ][ MUGMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ MUGMISSILE ] += 3;

					if( Host_SecondaryAmmo[ Player ][ MUGMISSILE ] > 10 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ MUGMISSILE ] - 10 );
						Host_SecAmmoUsed[ MUGMISSILE ] = Temp;
						Host_SecondaryAmmo[ Player ][ MUGMISSILE ] = 10;
					}

					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Heatseaker:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_HeatseakerPickup:
				if( Host_SecondaryAmmo[ Player ][ SOLARISMISSILE ] < 10 )
				{
					Host_SecondaryWeaponsGot[ Player ][ SOLARISMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ SOLARISMISSILE ] += 3;

					if( Host_SecondaryAmmo[ Player ][ SOLARISMISSILE ] > 10 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ SOLARISMISSILE ] - 10 );
						Host_SecAmmoUsed[ SOLARISMISSILE ] = Temp;
						Host_SecondaryAmmo[ Player ][ SOLARISMISSILE ] = 10;
					}

					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Thief:
				if( !Host_SecondaryAmmo[ Player ][ THIEFMISSILE ] )
				{
					Host_SecondaryWeaponsGot[ Player ][ THIEFMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ THIEFMISSILE ] += 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Scatter:
				if( Host_SecondaryAmmo[ Player ][ SCATTERMISSILE ] < 3 )
				{
					Host_SecondaryWeaponsGot[ Player ][ SCATTERMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ SCATTERMISSILE ] += 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gravgon:
				if( !Host_SecondaryAmmo[ Player ][ GRAVGONMISSILE ] )
				{
					Host_SecondaryWeaponsGot[ Player ][ GRAVGONMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ GRAVGONMISSILE ] += 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Launcher:
				if( Host_SecondaryAmmo[ Player ][ MULTIPLEMISSILE ] < 100 )
				{
					Host_SecondaryWeaponsGot[ Player ][ MULTIPLEMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ MULTIPLEMISSILE ] += 50;
					if( Host_SecondaryAmmo[ Player ][ MULTIPLEMISSILE ] > 100 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ MULTIPLEMISSILE ] - 100 );
						Host_SecAmmoUsed[ MULTIPLEMISSILE ] = Temp;
						Host_SecondaryAmmo[ Player ][ MULTIPLEMISSILE ] = 100;
					}
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_TitanStar:
				if( Host_SecondaryAmmo[ Player ][ TITANSTARMISSILE ] < 3 )
				{
					Host_SecondaryWeaponsGot[ Player ][ TITANSTARMISSILE ] = 1;
					Host_SecondaryAmmo[ Player ][ TITANSTARMISSILE ] += 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PurgePickup:
				if( Host_SecondaryAmmo[ Player ][ PURGEMINE ] < 10 )
				{
					Host_SecondaryWeaponsGot[ Player ][ PURGEMINE ] = 1;
					Host_SecondaryAmmo[ Player ][ PURGEMINE ] += 3;
					if( Host_SecondaryAmmo[ Player ][ PURGEMINE ] > 10 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ PURGEMINE ] - 10 );
						Host_SecAmmoUsed[ PURGEMINE ] = Temp;
						Host_SecondaryAmmo[ Player ][ PURGEMINE ] = 10;
					}
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PinePickup:
				if( Host_SecondaryAmmo[ Player ][ PINEMINE ] < 6 )
				{
					Host_SecondaryWeaponsGot[ Player ][ PINEMINE ] = 1;
					Host_SecondaryAmmo[ Player ][ PINEMINE ] += 3;
					if( Host_SecondaryAmmo[ Player ][ PINEMINE ] > 6 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ PINEMINE ] - 6 );
						Host_SecAmmoUsed[ PINEMINE ] = Temp;
						Host_SecondaryAmmo[ Player ][ PINEMINE ] = 6;
					}
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_QuantumPickup:
				if( !Host_SecondaryAmmo[ Player ][ QUANTUMMINE ] )
				{
					Host_SecondaryWeaponsGot[ Player ][ QUANTUMMINE ] = 1;
					Host_SecondaryAmmo[ Player ][ QUANTUMMINE ] += 1;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SpiderPod:
				if( Host_SecondaryAmmo[ Player ][ SPIDERMINE ] < 6 )
				{
					Host_SecondaryWeaponsGot[ Player ][ SPIDERMINE ] = 1;
					Host_SecondaryAmmo[ Player ][ SPIDERMINE ] += 3;

					if( Host_SecondaryAmmo[ Player ][ SPIDERMINE ] > 6 )
					{
						Temp = ( Host_SecondaryAmmo[ Player ][ SPIDERMINE ] - 6 );
						Host_SecAmmoUsed[ SPIDERMINE ] = Temp;
						Host_SecondaryAmmo[ Player ][ SPIDERMINE ] = 6;
					}
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Parasite:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flare:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GeneralAmmo:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfGeneralAmmo[ Player ] < MAXGENERALAMMO )
					{
						Host_CopyOfGeneralAmmo[ Player ] += GENERALAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_GeneralAmmo[ Player ] < MAXGENERALAMMO )
					{
						Host_GeneralAmmo[ Player ] += GENERALAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PyroliteAmmo:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfPyroliteAmmo[ Player ] < MAXPYROLITEAMMO )
					{
						Host_CopyOfPyroliteAmmo[ Player ] += PYROLITEAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_PyroliteAmmo[ Player ] < MAXPYROLITEAMMO )
					{
						Host_PyroliteAmmo[ Player ] += PYROLITEAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGunAmmo:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfSussGunAmmo[ Player ] < MAXSUSSGUNAMMO )
					{
						Host_CopyOfSussGunAmmo[ Player ] += SUSSGUNAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_SussGunAmmo[ Player ] < MAXSUSSGUNAMMO )
					{
						Host_SussGunAmmo[ Player ] += SUSSGUNAMMOPERPOD;
						PickupEnable = TRUE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PowerPod:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfPowerLevel[ Player ] != ( MAXPOWERLEVELS - 1 ) )
					{
						Host_CopyOfPowerLevel[ Player ]++;
						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_PowerLevel[ Player ] != ( MAXPOWERLEVELS - 1 ) )
					{
						Host_PowerLevel[ Player ]++;
						PickupEnable = TRUE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldenPowerPod:
//				if( !( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer == 0.0F )
				{
//					GivemeSuperNashram();
					HostGivemeSuperNashram( Player );
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Shield:
#if 0
				if( Ships[ Player ].Object.Shield != MAX_SHIELD )
				{
					Ships[ Player ].Object.Shield += 32.0F;
					if( Ships[ Player ].Object.Shield > MAX_SHIELD ) Ships[ Player ].Object.Shield = MAX_SHIELD;
					PickupEnable = TRUE;
				}
#endif
				PickupEnable = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Inv:
//				if( !Ships[ Player ].Invul )
				if( Ships[ Player ].InvulTimer == 0.0F )
				{
					Ships[ Player ].InvulTimer = INVULNERABILITY_TIME;
					Ships[ Player ].Invul = TRUE;			
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_ExtraLife:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Computer:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Smoke:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Nitro:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfNitroFuel[ Player ] < MAX_NITRO_FUEL )
					{
						Host_CopyOfNitroFuel[ Player ] += 50.0F;

						if( Host_CopyOfNitroFuel[ Player ] > MAX_NITRO_FUEL )
						{
							TempFloat = ( Host_CopyOfNitroFuel[ Player ] - MAX_NITRO_FUEL );
							Host_NitroFuelUsed += TempFloat;
							Host_CopyOfNitroFuel[ Player ] = MAX_NITRO_FUEL;
						}

						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_NitroFuel[ Player ] < MAX_NITRO_FUEL )
					{
						Host_NitroFuel[ Player ] += 50.0F;

						if( Host_NitroFuel[ Player ] > MAX_NITRO_FUEL )
						{
							TempFloat = ( Host_NitroFuel[ Player ] - MAX_NITRO_FUEL );
							Host_NitroFuelUsed += TempFloat;
							Host_NitroFuel[ Player ] = MAX_NITRO_FUEL;
						}

						PickupEnable = TRUE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Goggles:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gold:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mantle:
//				Ships[ Player ].Object.Flags |= SHIP_Stealth;
				if( Ships[ Player ].StealthTime == 0.0F )
				{
					Ships[ Player ].StealthTime = ( 60.0F * ANIM_SECOND );
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Crystal:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_DNA:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SkeletonKey:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bomb:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldFigure:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag:
				if( CaptureTheFlag )
				{
					Host_Flags[ Player ] |= SHIP_CarryingFlag;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bounty:
				if ( BountyHunt )
				{
					Host_Flags[ Player ] |= SHIP_CarryingBounty;
					PickupEnable = TRUE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag1:
			case PICKUP_Flag2:
			case PICKUP_Flag3:
			case PICKUP_Flag4:
				if ( CTF )
				{
					int team;

					for ( team = 0; team < MAX_TEAMS; team++ )
					{
						if ( Pickups[i].Type == TeamFlagPickup[ team ] )
						{
							if ( team == TeamNumber[ Player ] )
							{
								if ( TeamFlagAtHome[ team ] )
								{
									PickupEnable = FALSE;
								}
								else if ( OwnFlagTeleportsHome )
								{
									GOAL *goal;

									// teleport flag back to own goal
									PickupEnable = FALSE;
									goal = TeamGoal( team );
									if ( goal )
									{
										KillPickupSend( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
										KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
										GenerateFlagAtHome( team );
										sprintf( CTFMessage, THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED, TeamName[ TeamNumber[ Player ] ] );
										SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturnedFlag, 0);
									}
								}
								else if ( CanCarryOwnFlag )
								{
									PickupEnable = TRUE;
									Host_Flags[ Player ] |= TeamFlagMask[ team ];
									TeamFlagAtHome[ team ] = FALSE;
									WhoIAm = (uint8) Player;
									sprintf( CTFMessage, THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG, TeamName[ TeamNumber[ Player ] ]);
									SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturningFlag, 0);
								}
								else
								{
									PickupEnable = FALSE;
								}
							}
							else
							{
								// now Player have the flag....?
								PickupEnable = TRUE;
								Host_Flags[ Player ] |= TeamFlagMask[ team ];
								TeamFlagAtHome[ team ] = FALSE;
								
								sprintf( CTFMessage, SOMEONE_HAS_OTHER_TEAM_FLAG,
									Names[ Player ],
									TeamName[ TeamNumber[ Player ] ],
									TeamName[ team ] );
								WhoIAm = (uint8) Player;
								SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_CaptureFlagMessage, 0);
							}
							break;
						}
					}
					WhoIAm = 0;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Orb:
//				if( ( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
				if( Ships[ Player ].SuperNashramTimer != 0.0F )
				{
					if( Host_CopyOfNumOfOrbitals[ Player ] < MAXMULTIPLES )
					{
						Host_CopyOfOrbAmmo[ Player ][ Host_CopyOfNumOfOrbitals[ Player ] ] = MAXMULTIPLEAMMO;
						Host_CopyOfNumOfOrbitals[ Player ]++;
						PickupEnable = TRUE;
					}
				}
				else
				{
					if( Host_NumOfOrbitals[ Player ] < MAXMULTIPLES )
					{
						Host_OrbAmmo[ Player ][ Host_NumOfOrbitals[ Player ] ] = MAXMULTIPLEAMMO;
						Host_NumOfOrbitals[ Player ]++;
						PickupEnable = TRUE;
					}
				}
				break;		 

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		}

		if( PickupEnable )
		{
//			if( ( Pickups[ i ].Type >= PICKUP_Trojax ) && ( Pickups[ i ].Type <= PICKUP_Laser ) )
			{
#if DEBUG_PICKUPS
				DebugPrintf( "Tell %s to collect %s\n", &Names[ Player ][ 0 ], Messages[ Pickups[ i ].Type ] );
#endif
			}

			Host_PickupsGot[ Player ][ Pickups[ i ].Type ]++;
			//			KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
			CleanUpPickup( i );
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*===================================================================
	Procedure	:	Collect Pickup from scene
	Input		:	uint16	Pickup to get
	Output		:	BOOL	True/False
===================================================================*/
BOOL ActuallyCollectPickup( uint16 i )
{
	int16	PickupEnable = TRUE;
	float	Sfx_Volume = 1.0F;
	int16	Temp;
	float	TempFloat;
	uint8	Message[ 128 ];
	int16	MessageSFX = -1;
	int16	TriggeredSFX = -1;
	BOOL	ShowTextAnyway = FALSE;
	BOOL	Speech = TRUE;
	
	sprintf( &Message[0], "%s", Messages[ Pickups[i].Type ] );

	if( i != (uint16) -1 )
	{
		switch( Pickups[i].Type )
		{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Trojax: 
				PrimaryWeaponsGot[ TROJAX ] = 1;
				MessageSFX = SFX_Select_Trojax;
				TriggeredSFX = SFX_BIKER_BW;
				NewPrimaryWeapon( TROJAX );
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Pyrolite:
				PrimaryWeaponsGot[ PYROLITE_RIFLE ] = 1;

				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfPyroliteAmmo = MAXPYROLITEAMMO;
				}
				else
				{
					PyroliteAmmo = MAXPYROLITEAMMO;
				}

				NewPrimaryWeapon( PYROLITE_RIFLE );
				MessageSFX = SFX_Select_Pyrolite;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Transpulse:
				PrimaryWeaponsGot[ TRANSPULSE_CANNON ] = 1;
				NewPrimaryWeapon( TRANSPULSE_CANNON );
				MessageSFX = SFX_Select_Transpulse;
				TriggeredSFX = SFX_BIKER_BW;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGun:
				PrimaryWeaponsGot[ SUSS_GUN ] = 1;

				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfSussGunAmmo = MAXSUSSGUNAMMO;
				}
				else
				{
					SussGunAmmo = MAXSUSSGUNAMMO;
				}

				NewPrimaryWeapon( SUSS_GUN );
				MessageSFX = SFX_Select_SussGun;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Laser:
				PrimaryWeaponsGot[ LASER ] = 1;
				NewPrimaryWeapon( LASER );
				MessageSFX = SFX_Select_BeamLaser;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mug:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mugs:
				SecondaryAmmo[ MUGMISSILE ] += 3;

				if( SecondaryAmmo[ MUGMISSILE ] > 10 )
				{
					Temp = ( SecondaryAmmo[ MUGMISSILE ] - 10 );
					SecAmmoUsed[ MUGMISSILE ] = Temp;
					SecondaryAmmo[ MUGMISSILE ] = 10;
				}

				NewSecondaryWeapon( MUGMISSILE );
				MessageSFX = SFX_Select_MugMissile;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Heatseaker:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_HeatseakerPickup:
				SecondaryAmmo[ SOLARISMISSILE ] += 3;

				if( SecondaryAmmo[ SOLARISMISSILE ] > 10 )
				{
					Temp = ( SecondaryAmmo[ SOLARISMISSILE ] - 10 );
					SecAmmoUsed[ SOLARISMISSILE ] = Temp;
					SecondaryAmmo[ SOLARISMISSILE ] = 10;
				}

				NewSecondaryWeapon( SOLARISMISSILE );
				MessageSFX = SFX_Select_SolarisMissile;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Thief:
				SecondaryAmmo[ THIEFMISSILE ] += 1;
				NewSecondaryWeapon( THIEFMISSILE );
//				MessageSFX = SFX_Select_ThiefMissile;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Scatter:
				SecondaryAmmo[ SCATTERMISSILE ] += 1;
				NewSecondaryWeapon( SCATTERMISSILE );
				MessageSFX = SFX_Select_ScatterMissile;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gravgon:
				SecondaryAmmo[ GRAVGONMISSILE ] += 1;
				NewSecondaryWeapon( GRAVGONMISSILE );
				MessageSFX = SFX_Select_GravgonMissile;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Launcher:
				SecondaryAmmo[ MULTIPLEMISSILE ] += 50;

				if( SecondaryAmmo[ MULTIPLEMISSILE ] > 100 )
				{
					Temp = ( SecondaryAmmo[ MULTIPLEMISSILE ] - 100 );
					SecAmmoUsed[ MULTIPLEMISSILE ] = Temp;
					SecondaryAmmo[ MULTIPLEMISSILE ] = 100;
				}

				NewSecondaryWeapon( MULTIPLEMISSILE );
				MessageSFX = SFX_Select_MFRL;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_TitanStar:
				SecondaryAmmo[ TITANSTARMISSILE ] += 1;
				NewSecondaryWeapon( TITANSTARMISSILE );
				MessageSFX = SFX_Select_TitanStarMissile;
				TriggeredSFX = SFX_BIKER_BW;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PurgePickup:
				SecondaryAmmo[ PURGEMINE ] += 3;

				if( SecondaryAmmo[ PURGEMINE ] > 10 )
				{
					Temp = ( SecondaryAmmo[ PURGEMINE ] - 10 );
					SecAmmoUsed[ PURGEMINE ] = Temp;
					SecondaryAmmo[ PURGEMINE ] = 10;
				}
				
				NewSecondaryWeapon( PURGEMINE );
				MessageSFX = SFX_Select_PurgeMine;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PinePickup:
				SecondaryAmmo[ PINEMINE ] += 3;

				if( SecondaryAmmo[ PINEMINE ] > 6 )
				{
					Temp = ( SecondaryAmmo[ PINEMINE ] - 6 );
					SecAmmoUsed[ PINEMINE ] = Temp;
					SecondaryAmmo[ PINEMINE ] = 6;
				}

				NewSecondaryWeapon( PINEMINE );
				MessageSFX = SFX_Select_PineMine;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_QuantumPickup:
				SecondaryAmmo[ QUANTUMMINE ] += 1;
				NewSecondaryWeapon( QUANTUMMINE );
				MessageSFX = SFX_Select_QuantumMine;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SpiderPod:
				SecondaryAmmo[ SPIDERMINE ] += 3;

				if( SecondaryAmmo[ SPIDERMINE ] > 6 )
				{
					Temp = ( SecondaryAmmo[ SPIDERMINE ] - 6 );
					SecAmmoUsed[ SPIDERMINE ] = Temp;
					SecondaryAmmo[ SPIDERMINE ] = 6;
				}

				NewSecondaryWeapon( SPIDERMINE );
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Parasite:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flare:
				MessageSFX = SFX_BIKECOMP_FL;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GeneralAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfGeneralAmmo += GENERALAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				else
				{
					GeneralAmmo += GENERALAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PyroliteAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfPyroliteAmmo += PYROLITEAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				else
				{
					PyroliteAmmo += PYROLITEAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGunAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfSussGunAmmo += SUSSGUNAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				else
				{
					SussGunAmmo += SUSSGUNAMMOPERPOD;
					MessageSFX = SFX_Select_Ammo;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PowerPod:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfPowerLevel++;
					if( CopyOfPowerLevel >= ( MAXPOWERLEVELS - 1 ) )
					{
						sprintf( &Message[ 0 ], FULL_POWER_LEVEL );
						CopyOfPowerLevel = ( MAXPOWERLEVELS - 1 );
					}
					else
					{
						sprintf( &Message[ 0 ], POWER_POD_LEVEL, Ships[ WhoIAm ].Object.PowerLevel+1 );
					}
					ShowTextAnyway = TRUE;
					MessageSFX = SFX_Select_PowerPod;
				}
				else
				{
					Ships[ WhoIAm ].Object.PowerLevel++;
					if( Ships[ WhoIAm ].Object.PowerLevel >= ( MAXPOWERLEVELS - 1 ) )
					{
						sprintf( &Message[ 0 ], FULL_POWER_LEVEL );
						Ships[ WhoIAm ].Object.PowerLevel = ( MAXPOWERLEVELS - 1 );
					}
					else
					{
						sprintf( &Message[ 0 ], POWER_POD_LEVEL, Ships[ WhoIAm ].Object.PowerLevel+1 );
					}
					ShowTextAnyway = TRUE;
					MessageSFX = SFX_Select_PowerPod;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldenPowerPod:
				GivemeSuperNashram();
				MessageSFX = SFX_Select_GoldenPowerPod;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Shield:
				Ships[ WhoIAm ].Object.Shield += 32.0F;

				JustPickedUpShield = TRUE;

				if ( Ships[ WhoIAm ].Object.Shield > SHIELD_CRITICAL_LEVEL )
				{
					ShieldCritical = FALSE;
				}

				if( Ships[ WhoIAm ].Object.Shield > MAX_SHIELD )
				{
					Ships[ WhoIAm ].Object.Shield = MAX_SHIELD;
				}

				MessageSFX = SFX_Select_Shield;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Inv:
				Ships[ WhoIAm ].InvulTimer = INVULNERABILITY_TIME;
				Ships[ WhoIAm ].Invul = TRUE;			
				PickupInvulnerability = TRUE;
				MessageSFX = SFX_Select_Invul;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_ExtraLife:
				Lives += 1;
				MessageSFX = SFX_ResnicReanimator;
				PlaySfx( SFX_ExtraLife, 1.0F );
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Computer:
				TargetComputerOn = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Smoke:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Nitro:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfNitroFuel += 50.0F;
					if( CopyOfNitroFuel > MAX_NITRO_FUEL )
					{
						TempFloat = ( CopyOfNitroFuel - MAX_NITRO_FUEL );
						NitroFuelUsed += TempFloat;
						CopyOfNitroFuel = MAX_NITRO_FUEL;
					}
					MessageSFX = SFX_Select_Nitro;
				}
				else
				{
					NitroFuel += 50.0F;
					if( NitroFuel > MAX_NITRO_FUEL )
					{
						TempFloat = ( NitroFuel - MAX_NITRO_FUEL );
						NitroFuelUsed += TempFloat;
						NitroFuel = MAX_NITRO_FUEL;
					}
					MessageSFX = SFX_Select_Nitro;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Goggles:
				PickupEnable = FALSE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gold:
				NumGoldBars ++;
				if( !( NumGoldBars % 10 ) )
				{
//					NumGoldBars = 0;
					Lives += 1;
					sprintf( &Message[0], YOUVE_EARNED_EXTRA_LIFE );
					PlaySfx( SFX_ExtraLife, 1.0F );
				}
				else
				{
					if( NumGoldBars == 1 ) sprintf( &Message[0], YOU_HAVE_ONE_GOLD_BAR, NumGoldBars );
					else sprintf( &Message[0], YOU_HAVE_GOLD_BARS, NumGoldBars );
				}
				MessageSFX = SFX_GoldBarPickup;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mantle:
				Ships[ WhoIAm ].Object.Flags |= SHIP_Stealth;
				Ships[ WhoIAm ].StealthTime = ( 60.0F * ANIM_SECOND );
				MessageSFX = SFX_Cloaking;
				Speech = FALSE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Crystal:
				CrystalsFound += 1;
				if( CrystalsFound == 1 )
				{
					sprintf( &Message[0], YOU_HAVE_ONE_CRYSTAL, CrystalsFound );
				}
				else
				{
					sprintf( &Message[0], YOU_HAVE_CRYSTALS, CrystalsFound );
				}
				MessageSFX = SFX_PickupCrystal;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_DNA:
				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SkeletonKey:
				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bomb:
				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldFigure:
				MessageSFX = SFX_PickupGeneral;
				Speech = FALSE;
				ShowTextAnyway = TRUE;
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag:
				if ( CaptureTheFlag )
				{
					// now I have the flag....?
					Ships[ WhoIAm ].Object.Flags |= SHIP_CarryingFlag;
					AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
						TeamName[ TeamNumber[ WhoIAm ] ] );

					sprintf( CTFMessage, SOMEONE_HAS_GOT_THE_FLAG, Names[ WhoIAm ], TeamName[ TeamNumber[ WhoIAm ] ] );
					SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_CaptureFlagMessage, 0);
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bounty:
				if ( BountyHunt )
				{
					// now I have the bounty....?
					BountyTime = 0.0F;
					Ships[ WhoIAm ].Object.Flags |= SHIP_CarryingBounty;
					AddColourMessageToQue(FlagMessageColour, YOU_HAVE_GOT_THE_BOUNTY );
					
					SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_BountyMessage, 0);
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag1:
			case PICKUP_Flag2:
			case PICKUP_Flag3:
			case PICKUP_Flag4:
				if ( CTF )
				{
					int team;

					for ( team = 0; team < MAX_TEAMS; team++ )
					{
						if ( Pickups[i].Type == TeamFlagPickup[ team ] )
						{
							if ( team == TeamNumber[ WhoIAm ] )
							{
								if ( CanCarryOwnFlag )
								{
									Ships[ WhoIAm ].Object.Flags |= TeamFlagMask[ team ];
									TeamFlagAtHome[ team ] = FALSE;
									AddColourMessageToQue(FlagMessageColour, RETURN_TEAM_FLAG,
										TeamName[ TeamNumber[ WhoIAm ] ] );
								}
							}
							else
							{
								// now I have the flag....?
								Ships[ WhoIAm ].Object.Flags |= TeamFlagMask[ team ];
								TeamFlagAtHome[ team ] = FALSE;
								AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
									TeamName[ TeamNumber[ WhoIAm ] ] );
							}
							break;
						}
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Orb:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					CopyOfOrbAmmo[ CopyOfOrbitals ] = MAXMULTIPLEAMMO;
					CopyOfOrbitals++;
					if( CopyOfOrbitals > 8 ) CopyOfOrbitals = 8;
					MessageSFX = SFX_Orbital;
				}
				else
				{
					if( Ships[ WhoIAm ].NumMultiples < MAXMULTIPLES )
					{
//						Ships[ WhoIAm ].Object.Flags |= SHIP_Multiple;
						Ships[ WhoIAm ].OrbAmmo[ Ships[ WhoIAm ].NumMultiples ] = MAXMULTIPLEAMMO;
						Ships[ WhoIAm ].NumMultiples++;
						CreateOrbitPulsar( WhoIAm );
					}
					else
					{
						Ships[ WhoIAm ].NumMultiples = MAXMULTIPLES;
					}
					MessageSFX = SFX_Orbital;
				}
				break;		 
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		}

		if( PickupEnable || no_collision )
		{
			if( ( MessageSFX != -1 ) && ( bSoundEnabled ) && ( BikeCompSpeechSlider.value || !Speech ) )
			{
				if ( ( TriggeredSFX != -1 ) && !Random_Range( 4 ) )
					PlaySfxWithTrigger( MessageSFX, TriggeredSFX );
				else
					PlaySfx( MessageSFX, Sfx_Volume );
				if( ShowTextAnyway )
				{
					AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
				}
			}
			else
			{
				if( Speech )
				{
					PlaySfx( SFX_PickupGeneral, Sfx_Volume );
				}

				AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
			}

			PickupsGot[ Pickups[ i ].Type ]++;
			CleanUpPickup( i );
//			KillPickup( Pickups[ i ].Owner, Pickups[ i ].ID, PICKUPKILL_Immediate );
			return TRUE;
		}
		else
		{
			if( !Pickups[ i ].CouldNotPickup )
			{
				if( ( MessageSFX != -1 ) && ( bSoundEnabled ) && ( BikeCompSpeechSlider.value || !Speech ) )
				{
					if ( ( TriggeredSFX != -1 ) && !Random_Range( 4 ) )
						PlaySfxWithTrigger( MessageSFX, TriggeredSFX );
					else
						PlaySfx( MessageSFX, Sfx_Volume );
	 
					if( ShowTextAnyway )
					{
						AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
					}
				}
				else
				{
					AddColourMessageToQue( SystemMessageColour, &Message[ 0 ] );
				}
				Pickups[ i ].CouldNotPickup = TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*===================================================================
	Procedure	:	Collect Pickup from scene
	Input		:	uint16	Pickup to get
	Output		:	BOOL	True/False
===================================================================*/
BOOL PretendCollectPickup( uint16 i )
{
	int16	PickupEnable = TRUE;
	float	Sfx_Volume = 1.0F;
	uint8	Message[ 128 ];
	int16	MessageSFX = -1;
	int16	TriggeredSFX = -1;
	BOOL	ShowTextAnyway = FALSE;
	BOOL	Speech = TRUE;
	
	sprintf( &Message[0], "%s", Messages[ Pickups[i].Type ] );

	if( i != (uint16) -1 )
	{
		switch( Pickups[i].Type )
		{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Trojax: 
				if( PrimaryWeaponsGot[ TROJAX ] )
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Pyrolite:
				if( PrimaryWeaponsGot[ PYROLITE_RIFLE ] )
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Transpulse:
				if( PrimaryWeaponsGot[ TRANSPULSE_CANNON ] )
				{
					PickupEnable = FALSE;
					MessageSFX = SFX_BIKECOMP_AP;
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGun:
				if( PrimaryWeaponsGot[ SUSS_GUN ] )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Laser:
				if( PrimaryWeaponsGot[ LASER ] )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_A, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mug:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mugs:
				if( SecondaryAmmo[ MUGMISSILE ] >= 10 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Heatseaker:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_HeatseakerPickup:
				if( SecondaryAmmo[ SOLARISMISSILE ] >= 10 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Thief:
				if( SecondaryAmmo[ THIEFMISSILE ] )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Scatter:
				if( SecondaryAmmo[ SCATTERMISSILE ] >= 3 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gravgon:
				if( SecondaryAmmo[ GRAVGONMISSILE ] )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Launcher:
				if( SecondaryAmmo[ MULTIPLEMISSILE ] >= 100 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_TitanStar:
				if( SecondaryAmmo[ TITANSTARMISSILE ] >= 3 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PurgePickup:
				if( SecondaryAmmo[ PURGEMINE ] >= 10 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PinePickup:
				if( SecondaryAmmo[ PINEMINE ] >= 6 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_QuantumPickup:
				if( SecondaryAmmo[ QUANTUMMINE ] )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SpiderPod:
				if( SecondaryAmmo[ SPIDERMINE ] >= 6 )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Parasite:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flare:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GeneralAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfGeneralAmmo >= MAXGENERALAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( GeneralAmmo >= MAXGENERALAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PyroliteAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfPyroliteAmmo >= MAXPYROLITEAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( PyroliteAmmo >= MAXPYROLITEAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SussGunAmmo:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfSussGunAmmo >= MAXSUSSGUNAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				else
				{
					if( SussGunAmmo >= MAXSUSSGUNAMMO )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						PickupEnable = FALSE;
						MessageSFX = SFX_BIKECOMP_MA;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_PowerPod:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfPowerLevel == ( MAXPOWERLEVELS - 1 ) )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX_POWER );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( Ships[ WhoIAm ].Object.PowerLevel == ( MAXPOWERLEVELS - 1 ) )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX_POWER );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldenPowerPod:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					sprintf( &Message[0], YOU_CANT_HANDLE_ANY_MORE );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Shield:
				if( Ships[ WhoIAm ].Object.Shield == MAX_SHIELD )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Inv:
				if( Ships[ WhoIAm ].Invul )
				{
					sprintf( &Message[0], YOU_ALREADY_HAVE, Messages[ Pickups[i].Type ] );
					MessageSFX = SFX_BIKECOMP_AP;
					PickupEnable = FALSE;
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_ExtraLife:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Computer:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Smoke:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Nitro:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfNitroFuel >= MAX_NITRO_FUEL )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( NitroFuel >= MAX_NITRO_FUEL )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Goggles:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Gold:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Mantle:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Crystal:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_DNA:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_SkeletonKey:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bomb:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_GoldFigure:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Bounty:
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Flag1:
			case PICKUP_Flag2:
			case PICKUP_Flag3:
			case PICKUP_Flag4:
				if ( CTF )
				{
					int team;

					for ( team = 0; team < MAX_TEAMS; team++ )
					{
						if ( Pickups[i].Type == TeamFlagPickup[ team ] )
						{
							if ( team == TeamNumber[ WhoIAm ] )
							{
								if ( !TeamFlagAtHome[ team ] &&	!OwnFlagTeleportsHome && !CanCarryOwnFlag )
								{
									AddColourMessageToQue(FlagMessageColour, CANNOT_PICKUP_OWN_FLAG );
									PickupEnable = FALSE;
								}
							}
							break;
						}
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PICKUP_Orb:
				if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
				{
					if( CopyOfOrbitals >= MAXMULTIPLES )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				else
				{
					if( Ships[ WhoIAm ].NumMultiples >= MAXMULTIPLES )
					{
						sprintf( &Message[0], YOU_ALREADY_HAVE_MAX, Messages[ Pickups[i].Type ] );
						MessageSFX = SFX_BIKECOMP_AP;
						PickupEnable = FALSE;
					}
				}
				break;		 
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		}

		if( PickupEnable || no_collision )
		{
			return TRUE;
		}
		else
		{
			if( !Pickups[ i ].CouldNotPickup )
			{
				if( ( MessageSFX != -1 ) && ( bSoundEnabled ) && ( BikeCompSpeechSlider.value || !Speech ) )
				{
					if ( ( TriggeredSFX != -1 ) && !Random_Range( 4 ) )
						PlaySfxWithTrigger( MessageSFX, TriggeredSFX );
					else
						PlaySfx( MessageSFX, Sfx_Volume );
	 
					if( ShowTextAnyway )
					{
						AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
					}
				}
				else
				{
					AddColourMessageToQue( PickupMessageColour, &Message[ 0 ] );
				}
				Pickups[ i ].CouldNotPickup = TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Ships to Pickups collision
	Input		:	Nothing
	Output		:	All registers reserved
/*===================================================================ÄÄÄ*/
void CheckPickupAllPlayers( void )
{
	float		Length;
	VECTOR		Pos;
	VECTOR		Dir;
	int16		Count;
	GROUPLIST *	GroupsVisible;
	uint16	  *	GroupList;
	uint16		CurrentGroup;
	PICKUP	*	Pickup;
	PICKUP	*	NextPickup;
	uint16		Ship;
	float		Ammo;

	for( Ship = 1; Ship < MAX_PLAYERS; Ship++ )
	{
		if( ( GameStatus[ Ship ] == STATUS_Normal ) && ( Ships[ Ship ].Object.Mode == NORMAL_MODE ) )
		{
			if( !( Ships[ Ship ].Object.Flags & SHIP_SuperNashram ) && ( Ships[ Ship ].Object.Flags & SHIP_Turbo ) )
			{
				if( Host_NitroFuel[ Ship ] <= ( NITROLOSS * framelag ) )
				{
					Host_NitroFuelUsed += Host_NitroFuel[ Ship ];
					Host_NitroFuel[ Ship ] = 0.0F;
				}
				else
				{
					Host_NitroFuelUsed += ( NITROLOSS * framelag );
					Host_NitroFuel[ Ship ] -= ( NITROLOSS * framelag );
				}
			}

			if( ( Ships[ Ship ].Object.Flags & SHIP_Charging ) )
			{
				if( !( Ships[ Ship ].Object.Flags & SHIP_SuperNashram ) )
				{
					Ammo = PrimaryWeaponAttribs[ Ships[ Ship ].Primary ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ] * framelag;
					Host_GeneralAmmoUsed += Ammo;
					Host_GeneralAmmo[ Ship ] -= Ammo;
				}
			}

			if( Ships[ Ship ].SuperNashramTimer != 0.0F )
			{
				Ships[ Ship ].SuperNashramTimer -= framelag;

				if( Ships[ Ship ].SuperNashramTimer <= 0.0F )
				{
					HostLoseSuperNashram( Ship );
				}
			}

			if( Ships[ Ship ].InvulTimer != 0.0F )
			{
				Ships[ Ship ].InvulTimer -= framelag;

				if( Ships[ Ship ].InvulTimer <= 0.0F )
				{
					Ships[ Ship ].InvulTimer = 0.0F;
					if( MaxPickupType[ PICKUP_Inv ] )
							NumInvuls++;
				}
			}

			if( Ships[ Ship ].StealthTime != 0.0F )
			{
				Ships[ Ship ].StealthTime -= framelag;

				if( Ships[ Ship ].StealthTime <= 0.0F )
				{
					Ships[ Ship ].StealthTime = 0.0F;
					NumStealths++;
				}
			}

			Dir.x = ( Ships[ Ship ].Move_Off.x * framelag );
			Dir.y = ( Ships[ Ship ].Move_Off.y * framelag );
			Dir.z = ( Ships[ Ship ].Move_Off.z * framelag );

			Pos.x = ( Ships[ Ship ].Object.Pos.x - Dir.x );
			Pos.y = ( Ships[ Ship ].Object.Pos.y - Dir.y );
			Pos.z = ( Ships[ Ship ].Object.Pos.z - Dir.z );

			Length = VectorLength( &Dir );
			NormaliseVector( &Dir );

			GroupsVisible = VisibleGroups( Ships[ Ship ].Object.Group );

			GroupList = GroupsVisible->group;

			for( Count = 0; Count < GroupsVisible->groups; Count++ )
			{
				CurrentGroup = GroupList[ Count ];

				Pickup = PickupGroups[ CurrentGroup ];

				while( Pickup )
				{
					NextPickup = Pickup->NextInGroup;

//					if( ( Pickup->Speed == 0.0F ) && ( Pickup->Mode == PICKUPMODE_Normal ) && ( Pickup->PickupTime > 0.0F ) )
					if( Pickup->Mode == PICKUPMODE_Normal )
					{
						if( Pickup->Speed == 0.0F )
						{
							if( RaytoSphereShort( (VECTOR *) &Pickup->Pos, PICKUP_RADIUS, &Pos, &Dir, Length ) )
								CanPlayerCollectPickup( Pickup->Index, Ship );
							else
								Pickup->CouldNotPickup = FALSE;
						}
					}

					Pickup = NextPickup;
				}
			}

			if ( CaptureTheFlag && ( Host_Flags[ Ship ] & SHIP_CarryingFlag ) )
			{
				int		goalcheck;

				goalcheck = GoalCheckTeam( &Pos, &Ships[ Ship ].Object.Pos, Ships[ Ship ].Object.Group, TeamNumber[ Ship ] );
				if ( goalcheck == GOAL_SCORED )
				{
					Host_PickupsGot[ Ship ][ PICKUP_Flag ] = 0;
					Host_Flags[ Ship ] &= ~SHIP_CarryingFlag;
					FlagsToGenerate++;
				}
			}

			if ( CTF && ( Host_Flags[ Ship ] & SHIP_CarryingFlags ) )
			{
				int		goalcheck;
				
				goalcheck = GoalCheckTeam( &Pos, &Ships[ Ship ].Object.Pos, Ships[ Ship ].Object.Group, TeamNumber[ Ship ] );
				if ( goalcheck == GOAL_SCORED )
				{
					if ( !NeedFlagAtHome
						|| TeamFlagAtHome[ TeamNumber[ Ship ] ]
						|| ( Host_Flags[ Ship ] & TeamFlagMask[ TeamNumber[ Ship ] ] ) )
					{
						int team;
						int score;
						
						score = 0;
						for ( team = 0; team < MAX_TEAMS; team++ )
						{
							if ( Host_Flags[ Ship ] & TeamFlagMask[ team ] )
							{
								Host_PickupsGot[ Ship ][ TeamFlagPickup[ team ] ] = 0;
								Host_Flags[ Ship ] &= ~TeamFlagMask[ team ];
								if ( team != TeamNumber[ Ship ] )
								{
									score += GoalScore;
								}
								GenerateFlagAtHome( team );
							}
						}
						if ( !score )
						{
							sprintf( CTFMessage, THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED, TeamName[ TeamNumber[ Ship ] ] );
							SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturnedFlag, 0);
						}
					}
				}
			}
		}
	}
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Correct for Extra or Missing Pickups
	Input		:	Nothing
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
int16 SecondaryFromPickupTab[ MAXSECONDARYWEAPONS * 2 ] = {

	PICKUP_Mugs,				3,
	PICKUP_HeatseakerPickup,	3,
	PICKUP_Thief,				1,
	PICKUP_Scatter,				1,
	PICKUP_Gravgon,				1,
	PICKUP_Launcher,			50,
	PICKUP_TitanStar,			1,
	PICKUP_PurgePickup,			3,
	PICKUP_PinePickup,			3,
	PICKUP_QuantumPickup,		1,
	PICKUP_SpiderPod,			3,
};

int16	PrimaryInLevel[ MAXPRIMARYWEAPONS ];
int16	SecondaryInLevel[ MAXSECONDARYWEAPONS ];
int16	PrimaryInPlayers[ MAXPRIMARYWEAPONS ];
int16	SecondaryInPlayers[ MAXSECONDARYWEAPONS ];
int16	PrimaryToGenerate[ MAXPRIMARYWEAPONS ];
int16	SecondaryToGenerate[ MAXSECONDARYWEAPONS ];
int16	MinesInLevel[ MAXSECONDARYWEAPONS ];
int16	OrbsInLevel;
int16	OrbsToGenerate;
int16	OrbsInPlayers;
int16	StealthsInLevel;
int16	StealthsToGenerate;
int16	StealthsInPlayers;
int16	InvulsInLevel;
int16	InvulsToGenerate;
int16	InvulsInPlayers;
int16	SuperNashramsInLevel;
int16	SuperNashramsToGenerate;
int16	SuperNashramsInPlayers;

int16	CopyPrimaryInLevel[ MAXPRIMARYWEAPONS ];
int16	CopyPrimaryInPlayers[ MAXPRIMARYWEAPONS ];
int16	CopyPrimaryToGenerate[ MAXPRIMARYWEAPONS ];

int		BountyInLevel;
int		BountyInShips;
int		FlagsInLevel;
int		FlagsInShips;
int		TeamFlagsInLevel[ MAX_TEAMS ];
int		TeamFlagsInShips[ MAX_TEAMS ];

void CorrectForExtraOrMissingPickups( void )
{
	int16	Count;
	int16	Count2;
	int16	Player;
	int16	Weapon;
	int16	NumWeapons;
	int16	Diff;
	int16	Pickup;
#if 0
	int16	OldNumWeapons;
#endif

	CountMinesInLevel();

	BountyInShips = 0;
	BountyInLevel = NumPickupType[ PICKUP_Bounty ];
	FlagsInShips = 0;
	FlagsInLevel = NumPickupType[ PICKUP_Flag ];
	for ( Count = 0; Count < MAX_TEAMS; Count++ )
	{
		TeamFlagsInLevel[ Count ] = NumPickupType[ TeamFlagPickup[ Count ] ];
		TeamFlagsInShips[ Count ] = 0;
	}

	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		CopyPrimaryInLevel[ Count ] = PrimaryInLevel[ Count ];
		CopyPrimaryInPlayers[ Count ] = PrimaryInPlayers[ Count ];
		CopyPrimaryToGenerate[ Count ] = PrimaryToGenerate[ Count ];
	}

	for( Count = 0; Count < MAXPICKUPTYPES; Count++ )
	{
		switch( Count )
		{
			case PICKUP_Trojax:
			case PICKUP_Pyrolite:
			case PICKUP_Transpulse:
			case PICKUP_SussGun:
			case PICKUP_Laser:
				Weapon = ( Count - PICKUP_Trojax ) + 1;

				PrimaryInLevel[ Weapon ] = NumPickupType[ Count ];
				PrimaryToGenerate[ Weapon ] = NumPrimWeapons[ Weapon ];
				PrimaryInPlayers[ Weapon ] = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
						if( Host_PrimaryWeaponsGot[ Player ][ Weapon ] )
						{
							PrimaryInPlayers[ Weapon ]++;
						}
					}
				}
				break;

			case PICKUP_Mug:
			case PICKUP_Heatseaker:
			case PICKUP_Parasite:
				break;

			case PICKUP_Mugs:
			case PICKUP_HeatseakerPickup:
			case PICKUP_Thief:
			case PICKUP_Scatter:
			case PICKUP_Gravgon:
			case PICKUP_Launcher:
			case PICKUP_TitanStar:
			case PICKUP_PurgePickup:
			case PICKUP_PinePickup:
			case PICKUP_QuantumPickup:
			case PICKUP_SpiderPod:
				for( Count2 = 0; Count2 < MAXSECONDARYWEAPONS; Count2++ )
				{
					if( SecondaryFromPickupTab[ ( Count2 * 2 ) ] == Count )
					{
						Weapon = Count2;
						break;
					}
				}

				SecondaryInLevel[ Weapon ] = NumPickupType[ Count ];
//				SecondaryToGenerate[ Weapon ] = ( ( Host_SecAmmoUsed[ Weapon ] + SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] - 1 ) / SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] );
				SecondaryToGenerate[ Weapon ] = Host_SecAmmoUsed[ Weapon ];
				SecondaryInPlayers[ Weapon ] = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
//						SecondaryInPlayers[ Weapon ] += ( ( Host_SecondaryAmmo[ Player ][ Weapon ] + SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] - 1 ) / SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] );
						SecondaryInPlayers[ Weapon ] += Host_SecondaryAmmo[ Player ][ Weapon ];
					}
				}
				break;

			case PICKUP_Orb:
				OrbsInLevel = NumPickupType[ Count ];
				OrbsToGenerate = NumOrbs;
				OrbsInPlayers = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
						if( Ships[ Player ].SuperNashramTimer != 0.0F )
						{
							OrbsInPlayers += Host_CopyOfNumOfOrbitals[ Player ];
						}
						else
						{
							OrbsInPlayers += Host_NumOfOrbitals[ Player ];
						}
					}
				}
				break;

			case PICKUP_Mantle:
				StealthsInLevel = NumPickupType[ Count ];
				StealthsToGenerate = NumStealths;
				StealthsInPlayers = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
						if( Ships[ Player ].StealthTime != 0.0F )
						{
							StealthsInPlayers++;
						}
					}
				}
				break;


			case PICKUP_Inv:
				InvulsInLevel = NumPickupType[ Count ];
				InvulsToGenerate = NumInvuls;
				InvulsInPlayers = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
						if( Ships[ Player ].InvulTimer != 0.0F )
						{
							InvulsInPlayers++;
						}
					}
				}
				break;

			case PICKUP_GoldenPowerPod:
				SuperNashramsInLevel = NumPickupType[ Count ];
				SuperNashramsToGenerate = NumSuperNashrams;
				SuperNashramsInPlayers = 0;

				for( Player = 1; Player < MAX_PLAYERS; Player++ )
				{
					if( GameStatus[ Player ] == STATUS_Normal )
					{
						if( Ships[ Player ].SuperNashramTimer != 0.0F )
						{
							SuperNashramsInPlayers++;
						}
					}
				}
				break;

			case PICKUP_Nitro:
				break;

			case PICKUP_Flare:
			case PICKUP_GeneralAmmo:
			case PICKUP_PyroliteAmmo:
			case PICKUP_SussGunAmmo:
			case PICKUP_PowerPod:
			case PICKUP_Shield:
			case PICKUP_ExtraLife:
			case PICKUP_Computer:
			case PICKUP_Smoke:
			case PICKUP_Goggles:
			case PICKUP_Gold:
			case PICKUP_Crystal:
			case PICKUP_DNA:
			case PICKUP_SkeletonKey:
			case PICKUP_Bomb:
			case PICKUP_GoldFigure:
				break;

			case PICKUP_Bounty:
				break;

			case PICKUP_Flag:
				break;

			case PICKUP_Flag1:
			case PICKUP_Flag2:
			case PICKUP_Flag3:
			case PICKUP_Flag4:
				break;
		}
	}


/*===================================================================ÄÄÄÄ
	Correct for fucked up secondary weapons
/*===================================================================ÄÄÄ*/
	for( Weapon = 1; Weapon < MAXSECONDARYWEAPONS; Weapon++ )
	{
		Pickup = SecondaryFromPickupTab[ ( Weapon * 2 ) ];

		NumWeapons = SecondaryInLevel[ Weapon ] + 
					( ( SecondaryToGenerate[ Weapon ] + SecondaryInPlayers[ Weapon ] + MinesInLevel[ Weapon ] ) /
					SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] );

		if( NumWeapons > MaxPickupType[ Pickup ] )
		{
			Diff = ( NumWeapons - MaxPickupType[ Pickup ] );

			if( Host_SecAmmoUsed[ Weapon ] >= ( Diff * SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] ) )
			{
				Host_SecAmmoUsed[ Weapon ] -= ( Diff * SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] );
			}
			else
			{
				Host_SecAmmoUsed[ Weapon ] = 0;
			}
		}
		else
		{
			if( NumWeapons < MaxPickupType[ Pickup ] )
			{
				Diff = ( MaxPickupType[ Pickup ] - NumWeapons );
				Host_SecAmmoUsed[ Weapon ] += ( Diff * SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] );
			}
		}
	}

/*===================================================================ÄÄÄÄ
	Correct for fucked up orbitals
/*===================================================================ÄÄÄ*/
	NumWeapons = ( OrbsInLevel + OrbsInPlayers + OrbsToGenerate );

	if( NumWeapons > MaxPickupType[ PICKUP_Orb ] )
	{
		Diff = ( NumWeapons - MaxPickupType[ PICKUP_Orb ] );

		if( NumOrbs >= Diff )
		{
			NumOrbs -= Diff;
		}
		else
		{
			NumOrbs = 0;
		}
	}
	else
	{
		if( NumWeapons < MaxPickupType[ PICKUP_Orb ] )
		{
			Diff = ( MaxPickupType[ PICKUP_Orb ] - NumWeapons );
			NumOrbs += Diff;
		}
	}


/*===================================================================ÄÄÄÄ
	Correct for fucked up stealths
/*===================================================================ÄÄÄ*/
	NumWeapons = ( StealthsInLevel + StealthsInPlayers + StealthsToGenerate );

	if( NumWeapons > MaxPickupType[ PICKUP_Mantle] )
	{
		Diff = ( NumWeapons - MaxPickupType[ PICKUP_Mantle ] );

		if( NumStealths >= Diff )
		{
			NumStealths -= Diff;
		}
		else
		{
			NumStealths = 0;
		}
	}
	else
	{
		if( NumWeapons < MaxPickupType[ PICKUP_Mantle ] )
		{
			Diff = ( MaxPickupType[ PICKUP_Mantle ] - NumWeapons );
			NumStealths += Diff;
		}
	}

/*===================================================================ÄÄÄÄ
	Correct for fucked up Invulnerabilities
/*===================================================================ÄÄÄ*/
	NumWeapons = ( InvulsInLevel + InvulsInPlayers + InvulsToGenerate );

	if( NumWeapons > MaxPickupType[ PICKUP_Inv ] )
	{
		Diff = ( NumWeapons - MaxPickupType[ PICKUP_Inv ] );

		if( NumInvuls >= Diff )
		{
			NumInvuls -= Diff;
		}
		else
		{
			NumInvuls = 0;
		}
	}
	else
	{
		if( NumWeapons < MaxPickupType[ PICKUP_Inv ] )
		{
			Diff = ( MaxPickupType[ PICKUP_Inv ] - NumWeapons );
			NumInvuls += Diff;
		}
	}

/*===================================================================ÄÄÄÄ
	Correct for fucked up supernashrams
/*===================================================================ÄÄÄ*/
	NumWeapons = ( SuperNashramsInLevel + SuperNashramsInPlayers + SuperNashramsToGenerate );

	if( NumWeapons > MaxPickupType[ PICKUP_GoldenPowerPod ] )
	{
		Diff = ( NumWeapons - MaxPickupType[ PICKUP_GoldenPowerPod ] );

		if( NumSuperNashrams >= Diff )
		{
			NumSuperNashrams -= Diff;
		}
		else
		{
			NumSuperNashrams = 0;
		}
	}
	else
	{
		if( NumWeapons < MaxPickupType[ PICKUP_GoldenPowerPod ] )
		{
			Diff = ( MaxPickupType[ PICKUP_GoldenPowerPod ] - NumWeapons );
			NumSuperNashrams += Diff;
		}
	}

	// check bounty still OK
	if ( BountyHunt )
	{
		for ( Count = 1; Count < MAX_PLAYERS; Count++ )
		{
			if ( ( Host_Flags[ Count ] & Ships[ Count ].Object.Flags & SHIP_CarryingBounty ) )
				BountyInShips++;
		}
		if ( ( BountyInLevel + BountyInShips ) == 1 )
		{
			Host_BountyTimer = 0; // correct number of bounty -> cancel any existing timer
		}
		else if ( Host_BountyTimer > 0 )
		{
			Host_BountyTimer -= framelag;
			if ( Host_BountyTimer <= 0 )
			{
				if ( !BountyInLevel && !BountyInShips )
				{
					BountyToGenerate++; // generate bounty
				}
				else
				{
					KillAllPickupsOfTypeAndSend( PICKUP_Bounty, PICKUPKILL_Immediate );
				}
				Host_BountyTimer = 0; // cancel timer
			}
		}
		else
		{
			Host_BountyTimer = HOST_BOUNTY_TIME; // something's wrong with number of bounty -> start timer
		}
	}

	// check flag chase still OK
	if ( CaptureTheFlag )
	{
		for ( Count = 1; Count < MAX_PLAYERS; Count++ )
		{
			if ( ( Host_Flags[ Count ] & Ships[ Count ].Object.Flags & SHIP_CarryingFlag ) )
				FlagsInShips++;
		}
		if ( ( FlagsInLevel + FlagsInShips ) == 1 )
		{
			Host_FlagTimer = 0; // correct number of flags -> cancel any existing timer
		}
		else if ( Host_FlagTimer > 0 )
		{
			Host_FlagTimer -= framelag;
			if ( Host_FlagTimer <= 0 )
			{
				if ( !FlagsInLevel && !FlagsInShips )
				{
					FlagsToGenerate++; // generate flag
				}
				else
				{
					KillAllPickupsOfTypeAndSend( PICKUP_Flag, PICKUPKILL_Immediate );
				}
				Host_FlagTimer = 0; // cancel timer
			}
		}
		else
		{
			Host_FlagTimer = HOST_FLAG_TIME; // something's wrong with number of flags -> start timer
		}
	}

	// check CTF flags OK
	if ( CTF )
	{
		int team;

		for ( Count = 1; Count < MAX_PLAYERS; Count++ )
		{
			if ( Host_Flags[ Count ] & SHIP_CarryingFlags ) 
			{
				for ( team = 0; team < MAX_TEAMS; team++ )
				{
					if ( Host_Flags[ Count ] & Ships[ Count ].Object.Flags & TeamFlagMask[ team ] )
						TeamFlagsInShips[ team ]++;
				}
			}
		}

		for ( team = 0; team < MAX_TEAMS; team++ )
		{
			if ( GoalTeamMembers[ team ] )
			{
				if ( ( TeamFlagsInLevel[ team ] + TeamFlagsInShips[ team ] ) == 1 )
				{
					if ( Host_TeamFlagTimer[ team ] > 0 )
						DebugPrintf( "The %s flag has sorted itself out\n", TeamName[ team ] );
					Host_TeamFlagTimer[ team ] = 0; // correct number of flags -> cancel any existing timer
				}
				else if ( Host_TeamFlagTimer[ team ] > 0 )
				{
					Host_TeamFlagTimer[ team ] -= framelag;
					if ( Host_TeamFlagTimer[ team ] <= 0 )
					{
						if ( !TeamFlagsInLevel[ team ] && !TeamFlagsInShips[ team ] )
						{
							DebugPrintf( "%s flag regenerated back in goal\n", TeamName[ team ] );
							GenerateFlagAtHome( team ); // generate missing flag back in its goal
						}
						else
						{
							KillAllPickupsOfTypeAndSend( TeamFlagPickup[ team ], PICKUPKILL_Immediate );
							DebugPrintf( "%d %s flags in level killed\n", TeamFlagsInShips[ team ], TeamName[ team ] );
						}
						Host_TeamFlagTimer[ team ] = 0; // cancel timer
					}
				}
				else
				{
					DebugPrintf( "The %s flag has messed up (%d player, %d level)\n",
								TeamName[ team ], TeamFlagsInShips[ team ], TeamFlagsInLevel[ team ] );
					Host_TeamFlagTimer[ team ] = HOST_FLAG_TIME; // something's wrong with number of flags -> start timer
				}
			}
			else
			{
				if ( TeamFlagsInLevel[ team ] )
					KillAllPickupsOfTypeAndSend( TeamFlagPickup[ team ], PICKUPKILL_Immediate );
				if ( TeamFlagsInShips[ team ] )
				{
					for ( Count = 0; Count < MAX_PLAYERS; Count++ )
					{
						Host_Flags[ Count ] &= ~TeamFlagMask[ team ];
					}
				}
			}
		}
	}

#if 0
	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		OldNumWeapons = ( CopyPrimaryInLevel[ Count ] + CopyPrimaryInPlayers[ Count ] + CopyPrimaryToGenerate[ Count ] );
		NumWeapons = ( PrimaryInLevel[ Count ] + PrimaryInPlayers[ Count ] + PrimaryToGenerate[ Count ] );

		if( NumWeapons < MaxPickupType[ PICKUP_Trojax + ( Count - 1 ) ] )
		{
			DebugPrintf( "OLD L %hd - P %hd - G %hd %s\n", CopyPrimaryInLevel[ Count ], CopyPrimaryInPlayers[ Count ], CopyPrimaryToGenerate[ Count ], PrimaryNames[ Count ] );
			DebugPrintf( "NEW L %hd - P %hd - G %hd %s\n", PrimaryInLevel[ Count ], PrimaryInPlayers[ Count ], PrimaryToGenerate[ Count ], PrimaryNames[ Count ] );
			Msg( "A %s has disappeared from the level", PrimaryNames[ Count ] );
		}
		else
		{
			if( NumWeapons > MaxPickupType[ PICKUP_Trojax + ( Count - 1 ) ] )
			{
				DebugPrintf( "OLD L %hd - P %hd - G %hd %s\n", CopyPrimaryInLevel[ Count ], CopyPrimaryInPlayers[ Count ], CopyPrimaryToGenerate[ Count ], PrimaryNames[ Count ] );
				DebugPrintf( "NEW L %hd - P %hd - G %hd %s\n", PrimaryInLevel[ Count ], PrimaryInPlayers[ Count ], PrimaryToGenerate[ Count ], PrimaryNames[ Count ] );
				Msg( "An extra %s has appeared in the level", PrimaryNames[ Weapon ] );
			}
		}
	}
#endif
}

/*===================================================================
	Procedure	:	Count Mines currently active in level
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void CountMinesInLevel( void )
{
	uint16	i;
	uint16	Next;

	for( i = 0; i < MAXSECONDARYWEAPONS; i++ ) MinesInLevel[ i ] = 0;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[i].SecType == SEC_MINE )
		{
			MinesInLevel[ SecBulls[i].Weapon ]++;
		}

		i = Next;
	}																				
}

#endif

int16		FirstFailedKillUsed = -1;
int16		FirstFailedKillFree = -1;
FAILEDKILL	FailedKill[ MAXFAILEDKILLS ];

/*===================================================================
	Procedure	:	Init FailedKill Slots
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitFailedKillSlots( void )
{
	int16	i;

	FirstFailedKillUsed = -1;
	FirstFailedKillFree = 0;

	for( i = 0; i < MAXFAILEDKILLS; i++ )
	{
		memset( &FailedKill[ i ], 0, sizeof( FAILEDKILL ) );
		FailedKill[ i ].Next = i + 1;
		FailedKill[ i ].Prev = -1;
	}
	FailedKill[ MAXFAILEDKILLS-1 ].Next = -1;
}

/*===================================================================
	Procedure	:	Add FailedKill to que
	Input		:	uint16	Owner
				:	uint16	ID
				:	int16	Style
				:	uint16	NewOwner
	Output		:	BOOL	TRUE/FALSE
===================================================================*/
BOOL AddFailedKillToQue( uint16 Owner, uint16 ID, int16 Style, uint16 NewOwner )
{
	int16	i;

	i = FindFreeFailedKillSlot();

	if( i != -1 )
	{
		DebugPrintf( "Added Failed KillPickup() Owner %d, ID %d to que\n", Owner, ID );
		FailedKill[ i ].ID = ID;
		FailedKill[ i ].Owner = Owner;
		FailedKill[ i ].NewOwner = NewOwner;
		FailedKill[ i ].Style = Style;
		FailedKill[ i ].Life = FAILEDKILLTIMEOUT;
		return( TRUE );
	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Find Free failed kill slot
	Input		:	Nothing
	Output		:	int16	Failed pickup index
===================================================================*/
int16 FindFreeFailedKillSlot( void )
{
	int16 i;

	i = FirstFailedKillFree;
	
	if ( i == -1 )
	{
		Msg( "Failed Kill Pickup Que Overflowed!!!\n" );
		return( i );
	}
 
	FailedKill[ i ].Prev = FirstFailedKillUsed;

	if( FirstFailedKillUsed != -1 )
	{
		FailedKill[ FirstFailedKillUsed ].Next = i;
	}

	FirstFailedKillUsed = i;
	FirstFailedKillFree = Pickups[ i ].Next;
	return( i );
}

/*===================================================================
	Procedure	:	release a failed kill slot
	Input		:	int16	Slot number
	Output		:	Nothing
===================================================================*/
void ReleaseFailedKillSlot( int16 i )
{
	int16	its_prev, its_next;

	its_prev = FailedKill[ i ].Prev;
	its_next = FailedKill[ i ].Next;

	if ( i == FirstFailedKillUsed ) FirstFailedKillUsed = FailedKill[ i ].Prev;
	if( its_prev != -1 ) FailedKill[ its_prev ].Next = its_next;
	if( its_next != -1 ) FailedKill[ its_next ].Prev = its_prev;

	FailedKill[ i ].Prev = -1;
	FailedKill[ i ].Next = FirstFailedKillFree;
	FirstFailedKillFree	= i;
}

/*===================================================================
	Procedure	:	Process Failed Kill Pickups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ProcessFailedKills( void )
{
	uint16		Pickup;
	int16		i, NextFailedKill;

	i = FirstFailedKillUsed;

	while( i != -1 )
	{
		NextFailedKill = FailedKill[ i ].Prev;

		FailedKill[ i ].Life -= framelag;
		
		if( FailedKill[ i ].Life < 0.0F )
		{
			DebugPrintf( "************** Dropped Failed KillPickup() Owner %d, ID %d from que\n", FailedKill[ i ].Owner, FailedKill[ i ].ID );
			ReleaseFailedKillSlot( i );
		}
		else
		{
			Pickup = FindPickup( FailedKill[ i ].Owner, FailedKill[ i ].ID );

			if( Pickup != (uint16) -1 )
			{
				KillPickup( FailedKill[ i ].Owner, FailedKill[ i ].ID, FailedKill[ i ].Style );
				DebugPrintf( "Removed Failed KillPickup() Owner %d, ID %d from que\n", FailedKill[ i ].Owner, FailedKill[ i ].ID );
				ReleaseFailedKillSlot( i );
			}
		}

		i = NextFailedKill;
	}
}

/*===================================================================
	Procedure	:	Find Pickup from owner and id
	Input		:	uint16		Owner
				:	uint16		ID
	Output		:	uint16		Index
===================================================================*/
uint16 FindPickup( uint16 Owner, uint16 ID )
{
	uint16	i;

	i = FirstPickupUsed;

	while( i != (uint16) -1 )
	{
		if( ( Pickups[ i ].Owner == Owner ) && ( Pickups[ i ].ID == ID ) )
		{
			return( i );
		}

		i = Pickups[ i ].Prev;
	}																				

	return( (uint16) -1 );
}	
	
#ifdef OPT_ON
#pragma optimize( "", off )
#endif
