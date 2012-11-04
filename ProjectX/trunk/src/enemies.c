/*===================================================================
	Includes
===================================================================*/
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "node.h"
#include "networking.h"
#include "ships.h"
#include "2dpolys.h"
#include "lights.h"
#include "models.h"
#include "text.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "trigarea.h"
#include "sphere.h"
#include "pickups.h"
#include "enemies.h"
#include "controls.h"
#include "ai.h"

#include "extforce.h"
#include "teleport.h"
#include "spotfx.h"
#include "sfx.h"
#include "title.h"
#include "util.h"
#include "oct2.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	External Variables
===================================================================*/
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	XLIGHT			XLights[ MAXXLIGHTS ];
extern	FMPOLY			FmPolys[ MAXNUMOF2DPOLYS ];
extern	MODEL			Models[ MAXNUMOFMODELS ];
extern	float			framelag;
extern	MATRIX			MATRIX_Identity;
extern	int16_t			LevelNum;
extern	char			LevelNames[MAXLEVELS][128];
extern	BYTE			GameStatus[MAX_PLAYERS];
extern	int16_t			Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	TRIGGERMOD	*	TrigMods;
extern	u_int16_t	IsGroupVisible[MAXGROUPS];
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern USERCONFIG *player_config;
extern	SHIPCONTROL control;
extern	int16_t	NextNewModel;
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	bool			IsHost;
extern	MXALOADHEADER	MxaModelHeaders[ MAXMXAMODELHEADERS ];
extern	MXLOADHEADER	ModelHeaders[MAXMODELHEADERS];
extern	AIMDATA AimData;
extern	BYTE	ChangeLevel_MyGameStatus;
extern	FRAME_INFO	*	Exp_Header;
extern	NODENETWORKHEADER	NodeNetworkHeader;

extern	float	MaxMoveSpeed;
extern	float	MoveAccell;
extern	float	MoveDecell;
extern	float	MaxTurboSpeed;
extern	float	TurboAccell;
extern	float	TurboDecell;
extern	float	MaxTurnSpeed;
extern	float	TurnAccell;
extern	float	TurnDecell;
extern	float	MaxRollSpeed;
extern	float	RollAccell;
extern	float	RollDecell;
extern	float	MaxBankAngle;
extern	float	BankAccell;
extern	float	BankDecell;
extern	int		DifficultyLevel;
extern	int		outside_map;
extern	float	NmeDamageModifier;
extern	VECTOR	FleshmorphPos;
extern	MODELNAME	*	ModNames;

bool ObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
bool FindPointAboveGround( VECTOR * Pos , u_int16_t Group , VECTOR * NewPos , u_int16_t * NewGroup );
void AutoMovementExogenon( OBJECT * Object , ENEMY * Enemy );

/*===================================================================
	Defines
===================================================================*/
#define	ENEMY_SHIELD	128
#define	NME_VERSION_NUMBER	3
#define	SPECIALENEMYNODENUM	16384

/*===================================================================
	Global Variables
===================================================================*/
int		EnemiesActive = 0;
bool	ShowUntriggeredNMEs = false;
int		Exogenon_Num_StartPos = 0;
VECTOR	Exogenon_StartPos[6];

ENEMY * BigGeek = NULL;
float	FleshMorphTimer;
float	Difficulty = 1.0F;
//float	DifficlutyTab[4] = { 0.5F , 1.25F , 1.75F , 2.75F }; // easy medium hard deathwish....
//float	NmeDamageModifierTab[4] = { 0.35F, 0.65F , 0.95F , 1.9F }; // easy medium hard deathwish....
float	DifficlutyTab[4] = { 0.25F , 1.0F , 1.75F , 2.75F }; // easy medium hard deathwish....
float	NmeDamageModifierTab[4] = { 0.25F, 0.5F , 0.95F , 1.9F }; // easy medium hard deathwish....
char	EnemyNotFreedString[] = "Nowhere.c";
int8_t * EnemyNames[] =	{
 	"BeamTurret",			// 0
	"DuelTurret",			// 1
	"PulseTurret",			// 2
	"MissileTurret",		// 3
	"SnubTurret",			// 4
	"LazerBot",				// 5
	"SnubBot",				// 6
	"AirMoble",				// 7
	"AmmoDump",				// 8
	"LeviTank",				// 9
	"Max",					// 10
	"Mekton",				// 11
	"Boss_Nukecrane",		// 12
	"Supressor",			// 13
	"MineSweeper",			// 14
	"Swarm",				// 15
	"Shade",				// 16
	"MineLayer",			// 17
	"Hunter",				// 18
	"Legz",					// 19
	"GuardBot",				// 20
	"Boss_Metatank",		// 21
	"Boss_BigGeek",			// 22
	"Boss_Avatar",			// 23
	"Boss_FleshMorph",		// 24
	"Boss_Exogenon",		// 25
	"Bike_Lokasenna",  		// 26
	"Bike_Beard",       	// 27
	"Bike_LAJay",    		// 28
	"Bike_ExCop",     		// 29
	"Bike_RexHardy",  		// 30
	"Bike_Foetoid",    		// 31
	"Bike_NimSoo",    		// 32
	"Bike_Nutta",      		// 33
	"Bike_Sceptre",    		// 34
	"Bike_Jo",         		// 35
	"Bike_CuvelClark",		// 36
	"Bike_HK5",       		// 37
	"Bike_Nubia",      		// 38
	"Bike_Mofisto",    		// 39
	"Bike_Cerbero",    		// 40
	"Bike_Slick",      		// 41
	"LEADER_Swarm",			// 42
	"LEADER_Hunter",		// 43
	"LEADER_Mekton",		// 44
	"LEADER_SnubBot",		// 45
	"LEADER_Legz",			// 46
	"LEADER_Shade",			// 47
	"LEADER_Supressor",		// 48
	"LEADER_LeviTank",		// 49
	"LEADER_LazerBot",		// 50
	"LEADER_Max",			// 51
	"LEADER_AirMoble",		// 52
	"Fodder1"				// 53
	"Boss_LittleGeek",		// 54
	"Bike_FlyGirl",			// 55
};

GUNTYPE	GunTypes[] = {
	{//	GUN_Beam
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		60.0F,	//ReloadTime
		180.0F,	//BurstMasterTime
		0,	 	//BurstMasterCount
		10.0F,	//BurstAngle
		0.0F,	//XRotMin
		90.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		LASER,		//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Duel
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		0.1F,	//ReloadTime
		100.0F,	//BurstMasterTime
		2,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		-90.0F,	//XRotMin
		30.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		PULSAR,		//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Pulse
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		7.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		3,	 	//BurstMasterCount
		10.0F,	//BurstAngle
		-90.0F,	//XRotMin
		5.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		NME_BULLET1,	//WeaponType
		false,	//PreciseRotation
	},

	{//	GUN_Missile
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		60.0F,	//ReloadTime
		256.0F,	//BurstMasterTime
		3,	 	//BurstMasterCount
		10.0F,	//BurstAngle
		-75.0F,	//XRotMin
		30.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		MUGMISSILE,		//WeaponType
		false,	//PreciseRotation
	},

	{//	GUN_Snub
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		4.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		4,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		-45.0F,	//XRotMin
		45.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		MULTIPLEMISSILE,		//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Levi
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		0.1F,	//ReloadTime
		60.0F,	//BurstMasterTime
		2,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		-90.0F,	//XRotMin
		90.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		PULSAR,		//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Sec
		2.625F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.0611F,//TurnDecell
		420.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		0,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		-45.0F,	//XRotMin
		45.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		4.0F * 1024.0F * GLOBAL_SCALE,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		GRAVGONMISSILE,	//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Legz
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		300.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		0,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		0.0F,	//XRotMin
		0.0F,	//XRotMax
		-45.0F,	//YRotMin
		45.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		SOLARISMISSILE,	//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Pulse_Turret
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		7.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		3,	 	//BurstMasterCount
		10.0F,	//BurstAngle
		-90.0F,	//XRotMin
		0.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		ENEMYSPIRALMISSILE,	//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Mekton
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		7.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		3,	 	//BurstMasterCount
		10.0F,	//BurstAngle
		-90.0F,	//XRotMin
		0.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		false,	// false for Primary true for Secondary...
		NME_BULLET1,	//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_MetaTank
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		0.5F,	//ReloadTime
		100.0F,	//BurstMasterTime
		4,	 	//BurstMasterCount
		3.0F,	//BurstAngle
		0.0F,	//XRotMin
		0.0F,	//XRotMax
		-45.0F,	//YRotMin
		45.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		MULTIPLEMISSILE,	//WeaponType
		true,	//PreciseRotation
	},
	{//	GUN_MetaTankMain
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		200.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		0,	 	//BurstMasterCount
		9.0F,	//BurstAngle
		0.0F,	//XRotMin
		0.0F,	//XRotMax
		-90.0F,	//YRotMin
		90.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		ENEMYHOMINGMISSILE,	//WeaponType
		true,	//PreciseRotation
	},
	{//	GUN_Avatar
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		8.0F,	//ReloadTime
		180.0F,	//BurstMasterTime
		6,	 	//BurstMasterCount
		15.0F,	//BurstAngle
		0.0F,	//XRotMin
		0.0F,	//XRotMax
		-90.0F,	//YRotMin
		90.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		MULTIPLEMISSILE,	//WeaponType
		false,	//PreciseRotation
	},
	{//	GUN_Avatar2
		2.00F,	//MaxTurnSpeed
		0.065F,	//TurnAccell
		0.070F,//TurnDecell
		180.0F,	//ReloadTime
		100.0F,	//BurstMasterTime
		0,	 	//BurstMasterCount
		360.0F,	//BurstAngle
		0.0F,	//XRotMin
		0.0F,	//XRotMax
		0.0F,	//YRotMin
		0.0F,	//YRotMax
		0.0F,		// Range
		0,		//PowerLevel
		true,	// false for Primary true for Secondary...
		SOLARISMISSILE,	//WeaponType
		false,	//PreciseRotation
	},



};

//	bool	OnlyOnce = true;// Use this to display an enemy...

	int16_t		NumInitEnemies;
	int16_t		NumKilledEnemies = 0;
	ENEMY		Enemies[ MAXENEMIES ];
	ENEMY	*	EnemyGroups[ MAXGROUPS ];
	u_int16_t		NumEnemiesPerGroup[ MAXGROUPS ];
	ENEMY	*	FirstEnemyUsed = NULL;
	ENEMY	*	FirstEnemyFree = NULL;

ANIM_SEQ	PulseTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	SnubTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 1.5F * ANIM_SECOND },	// Opening
	{ 1.5F * ANIM_SECOND, 1.5F * ANIM_SECOND },	// Open
	{ 3.5F * ANIM_SECOND, 5.0F * ANIM_SECOND },	// Closing
	{ 1.5F * ANIM_SECOND, 1.5F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	MektonTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	SnubotTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	DuelTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	BeamTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	MislTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Stopped
	{ 0.0F * ANIM_SECOND, 2.0F * ANIM_SECOND },	// Starting
	{ 2.0F * ANIM_SECOND, 5.0F * ANIM_SECOND },	// Spinning
	{ 5.0F * ANIM_SECOND, 7.0F * ANIM_SECOND },	// Stopping
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	LevitankTurretSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closed
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Opening
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Open
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Closing
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Fire
};

ANIM_SEQ	LegzSeqs[] = {

	{ 0.0F * ANIM_SECOND, 2.0F * ANIM_SECOND },	// Walk

};

ANIM_SEQ	MetatankSeqs[] = {

	{ 4.0F * ANIM_SECOND, 6.0F * ANIM_SECOND },	// Main Gun Fire

};
ANIM_SEQ	ExogenonSeqs[] = {
	{ 10.5F * ANIM_SECOND, 12.0F * ANIM_SECOND },// Move Down
	{ 0.0F * ANIM_SECOND, 3.0F * ANIM_SECOND },	// Idle
	{ 3.0F * ANIM_SECOND, 9.0F * ANIM_SECOND },	// Fire
	{ 9.0F * ANIM_SECOND, 10.5F * ANIM_SECOND },// Move up
	{ 10.5F * ANIM_SECOND, 10.5F * ANIM_SECOND },// Stop Up..
};

ANIM_SEQ	LittleGeekSeqs[] = {

	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Stop
	{ 0.0F * ANIM_SECOND, 5.0F * ANIM_SECOND },	// Open

};

ANIM_SEQ	FleshmorphSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },// Stop
	{ 9.0F * ANIM_SECOND, 12.0F * ANIM_SECOND },// Pulsate
	{ 0.0F * ANIM_SECOND, 9.0F * ANIM_SECOND },	// Attack
};


ANIM_SEQ	MaxSeqs[] = {
	{ 0.0F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Stop
};


FIREPOS	PulseTurretFirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( 70.0F * GLOBAL_SCALE ), ( 240.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	BeamTurretFirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( -80.0F * GLOBAL_SCALE ), ( 520.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	SnubTurretFirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( 50.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	SnubotFirePos = {
		2,
		{
			{ ( -280.0F * GLOBAL_SCALE ), ( 320.0F * GLOBAL_SCALE ), ( 20.0F * GLOBAL_SCALE ) },
			{ ( +280.0F * GLOBAL_SCALE ), ( 320.0F * GLOBAL_SCALE ), ( 20.0F * GLOBAL_SCALE ) } 
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	MislTurretFirePos = {
		6,
		{
			{ ( 120.0F * GLOBAL_SCALE ), ( 460.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
			{ ( -120.0F * GLOBAL_SCALE ), ( 460.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
			{ ( 240.0F * GLOBAL_SCALE ), ( 280.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
			{ ( -240.0F * GLOBAL_SCALE ), ( 280.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
			{ ( 120.0F * GLOBAL_SCALE ), ( 80.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
			{ ( -120.0F * GLOBAL_SCALE ), ( 80.0F * GLOBAL_SCALE ), ( 440.0F * GLOBAL_SCALE ) },
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	DuelTurretFirePos = {
		2,
		{
			 { ( -160.0F * GLOBAL_SCALE ), ( 540.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) },
			 { ( +160.0F * GLOBAL_SCALE ), ( 540.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F }
		}


};

FIREPOS	MektonTurretFirePos = {
		1,
		{
			{ ( -60.0F * GLOBAL_SCALE ), ( 180.0F * GLOBAL_SCALE ), ( 380.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}


};

FIREPOS	SupresFirePos = {
		2,
		{
			{ ( -300.0F * GLOBAL_SCALE ), ( -160.0F * GLOBAL_SCALE ), ( 580.0F * GLOBAL_SCALE ) },
			{ ( +300.0F * GLOBAL_SCALE ), ( -160.0F * GLOBAL_SCALE ), ( 580.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	LevitankFirePos = {
		2,
		{
			{ ( 280.0F * GLOBAL_SCALE ), ( 150.0F * GLOBAL_SCALE ), ( -80.0F * GLOBAL_SCALE ) },
			{ ( -60.0F * GLOBAL_SCALE ), ( 150.0F * GLOBAL_SCALE ), ( -80.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F },
			{ 0.0F , 0.0F, 1.0F }
		}


};

FIREPOS	GuardBotFirePos = {
		1,
		{
			{ ( 80.0F * GLOBAL_SCALE ), ( 212.0F * GLOBAL_SCALE ), ( 468.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}


};

FIREPOS	LegzFirePos = {
		1,
		{
			{ ( 120.0F * GLOBAL_SCALE ), ( 120.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	LazbotFirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( 330.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};


FIREPOS	MetaTank1FirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 1760.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	MetaTank2FirePos = {
		1,
		{
			{ ( 940.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -380.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	MetaTank3FirePos = {
		1,
		{
			{ ( -940.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -380.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	MetaTank4FirePos = {
		1,
		{
			{ ( 0.0F * GLOBAL_SCALE ), ( 360.0F * GLOBAL_SCALE ), ( 1280.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	Avatar1FirePos = { 
		1,
		{
			{ ( 520.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -960.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , -0.5F, 5.0F }
		}
};
FIREPOS	Avatar2FirePos = { 
		1,
		{
			{ ( -320.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -920.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , -0.5F, 0.5F }
		}
};
FIREPOS	Avatar3FirePos = { 
		1,
		{
			{ ( -200.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	Avatar4FirePos = { 
		1,
		{
			{ ( 360.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 880.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};
FIREPOS	Avatar5FirePos = { 
		1,
		{
			{ ( -360.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 1120.0F * GLOBAL_SCALE ) }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};

FIREPOS	LittleGeekFirePos = {
		6,
		{
			{ -260.0F * GLOBAL_SCALE, -200.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
			{ -220.0F * GLOBAL_SCALE, -260.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, -260.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
			{ 260.0F * GLOBAL_SCALE, -200.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
			{ 220.0F * GLOBAL_SCALE, -260.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, -260.0F * GLOBAL_SCALE, 1620.0F * GLOBAL_SCALE },
		},
		//Fire Dir
		{
			{ 0.0F , -0.05F, 0.95F },
			{ 0.05F , 0.0F, 0.95F },
			{ -0.05F , 0.0F, 0.95F },
			{ 0.0F , -0.05F, 0.95F },
			{ 0.05F , 0.0F, 0.95F },
			{ -0.05F , 0.0F, 0.95F }
		}

};

FIREPOS	BigGeekFirePos = {
		4,
		{
			{ -260.0F * GLOBAL_SCALE, -350.0F * GLOBAL_SCALE, 220.0F * GLOBAL_SCALE },
			{ 260.0F * GLOBAL_SCALE, -350.0F * GLOBAL_SCALE, 220.0F * GLOBAL_SCALE },
			{ -460.0F * GLOBAL_SCALE, 350.0F * GLOBAL_SCALE, 220.0F * GLOBAL_SCALE },
			{ 460.0F * GLOBAL_SCALE, 350.0F * GLOBAL_SCALE, 220.0F * GLOBAL_SCALE },
		},
		//Fire Dir
		{
			{ -0.3F , -0.1F, 0.6F },
			{ 0.3F , -0.1F, 0.6F },
			{ -0.3F , 0.1F, 0.6F },
			{ 0.3F , 0.1F, 0.6F }
		}

};
FIREPOS	BigGeekFirePos2 = {
		6,
		{
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, 400.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -200.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -400.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -500.0F * GLOBAL_SCALE },
			{ -300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -600.0F * GLOBAL_SCALE },
		},
		//Fire Dir
		{
			{ -0.4F , 0.4F, 0.2F },
			{ -0.4F , 0.4F, -0.2F },
			{ -0.2F , 0.7F, 0.1F },
			{ -0.2F , 0.7F, -0.1F },
			{ -0.1F , 0.4F, -0.5F },
			{ -0.1F , 0.2F, -0.7F },
		}

};
FIREPOS	BigGeekFirePos3 = {
		6,
		{
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, 400.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -200.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -400.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -500.0F * GLOBAL_SCALE },
			{ 300.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, -600.0F * GLOBAL_SCALE },
		},
		//Fire Dir
		{
			{ 0.4F , 0.4F, 0.2F },
			{ 0.4F , 0.4F, -0.2F },
			{ 0.2F , 0.7F, 0.1F },
			{ 0.2F , 0.7F, -0.1F },
			{ 0.1F , 0.4F, -0.5F },
			{ 0.1F , 0.2F, -0.7F },
		}

};



FIREPOS	FleshMorphFirePos = {
		4,
		{
			// back left
			{ -70.0F * GLOBAL_SCALE, 400.0F * GLOBAL_SCALE, 300.0F * GLOBAL_SCALE },
			// back right
			{ 700.0F * GLOBAL_SCALE, 400.0F * GLOBAL_SCALE, -100.0F * GLOBAL_SCALE },
			// front left
			{ -70.0F * GLOBAL_SCALE, 200.0F * GLOBAL_SCALE, 800.0F * GLOBAL_SCALE },
			// front right
			{ 750.0F * GLOBAL_SCALE, 100.0F * GLOBAL_SCALE, 800.0F * GLOBAL_SCALE },
		},
		//Fire Dir
		{
			{ -0.35F , 0.5F, 0.15F },
			{ 0.35F , 0.5F, 0.15F },
			{ -0.5F , 0.0F, 0.5F },
			{ 0.5F , 0.0F, 0.5F },
		}

};


FIREPOS	DummyFirePos = {
		1,
		{
			{ 0.0F, 0.0F, 0.0F }
		},
		//Fire Dir
		{
			{ 0.0F , 0.0F, 1.0F }
		}
};


VECTOR	PulseTurretAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 40.0F * GLOBAL_SCALE ), ( -60.0F * GLOBAL_SCALE ) };
VECTOR	BeamTurretAimPos = { ( 0.0F * GLOBAL_SCALE ), ( -80.0F * GLOBAL_SCALE ), ( 0.0F * GLOBAL_SCALE ) };
VECTOR	SnubTurretAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 50.0F * GLOBAL_SCALE ), ( 0.0F * GLOBAL_SCALE ) };
VECTOR	SnubotAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 320.0F * GLOBAL_SCALE ), ( -280.0F * GLOBAL_SCALE ) };
VECTOR	MislTurretAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 480.0F * GLOBAL_SCALE ), ( 0.0F * GLOBAL_SCALE ) };
VECTOR	DuelTurretAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 540.0F * GLOBAL_SCALE ), ( -100.0F * GLOBAL_SCALE ) };
VECTOR	MektonTurretAimPos = { ( -60.0F * GLOBAL_SCALE ), ( 180.0F * GLOBAL_SCALE ), ( -60.0F * GLOBAL_SCALE ) };
VECTOR	SupresAimPos = { ( 0.0F * GLOBAL_SCALE ), ( -160.0F * GLOBAL_SCALE ), ( 560.0F * GLOBAL_SCALE ) };
VECTOR	LevitankAimPos = { ( -110.0F * GLOBAL_SCALE ), ( 150.0F * GLOBAL_SCALE ), ( -80.0F * GLOBAL_SCALE ) };
VECTOR	GuardBotAimPos = { ( -100.0F * GLOBAL_SCALE ), ( 480.0F * GLOBAL_SCALE ), ( 240.0F * GLOBAL_SCALE ) };
VECTOR	LegzAimPos = { ( 120.0F * GLOBAL_SCALE ), ( 120.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) };
VECTOR	LazbotAimPos = { ( 0.0F * GLOBAL_SCALE ), ( 330.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) };

VECTOR	MetaTank1AimPos = { ( 0.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 1760.0F * GLOBAL_SCALE ) };
VECTOR	MetaTank2AimPos = { ( 940.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -380.0F * GLOBAL_SCALE ) };
VECTOR	MetaTank3AimPos = { ( -940.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -380.0F * GLOBAL_SCALE ) };
VECTOR	MetaTank4AimPos = { ( 0.0F * GLOBAL_SCALE ), ( 360.0F * GLOBAL_SCALE ), ( 800.0F * GLOBAL_SCALE ) };

VECTOR	Avatar1AimPos = { ( 520.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -960.0F * GLOBAL_SCALE ) };
VECTOR	Avatar2AimPos = { ( -320.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( -920.0F * GLOBAL_SCALE ) };
VECTOR	Avatar3AimPos = { ( -200.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 400.0F * GLOBAL_SCALE ) };
VECTOR	Avatar4AimPos = { ( 360.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 880.0F * GLOBAL_SCALE ) };
VECTOR	Avatar5AimPos = { ( -360.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ), ( 1120.0F * GLOBAL_SCALE ) };

VECTOR	DummyAimPos = { 0.0F, 0.0F, 0.0F };

ENEMY_TYPES	EnemyTypes[ MAX_ENEMY_TYPES ]
= {
	{	"Beamtrt.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&BeamTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_TURRET_AI,
		AIMODE_IDLE,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Beam},	// u_int16_t	GunType;			// What type of gun???
		{&BeamTurretFirePos},
		{&BeamTurretAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		512,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}
	},				// 0 Beam Turret
	{	"Duel.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&DuelTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_TURRET_AI,
		AIMODE_IDLE,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Duel},	// u_int16_t	GunType;			// What type of gun???
		{&DuelTurretFirePos},
		{&DuelTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		512+256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.9F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 1 Duel Turret
	{	"pulse.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&PulseTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_TURRET_AI,
		AIMODE_IDLE,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse_Turret},	// u_int16_t	GunType;			// What type of gun???
		{&PulseTurretFirePos},
		{&PulseTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}
	},				// 2 Pulse Turret
	{	"Misltrt.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&MislTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_TURRET_AI,
		AIMODE_IDLE,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Missile},	// u_int16_t	GunType;			// What type of gun???
		{&MislTurretFirePos},
		{&MislTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		512,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.9F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 3 Missile Turret
	{	"gun.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&SnubTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_TURRET_AI,
		AIMODE_IDLE,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&SnubTurretFirePos},
		{&SnubTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		512,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		600.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.9F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 4 Pulse Turret
	{	"Lazbot.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&LazbotFirePos},
		{&LazbotAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 5 Lazer Bot
	{	"Snubot.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&SnubotTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH, //FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Snub},	// u_int16_t	GunType;			// What type of gun???
		{&SnubotFirePos},
		{&SnubotAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			AI_BEHAVIOUR_ATTACK_ONSITE,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 20.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 10.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 6 Snub Bot
	{	"Airmoble.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
//		ENEMY_CONTROLTYPE_SPLINE,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_DONTSTOPANDTURN | AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 7 AirMoble
	{	"Dump.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_NONE,
		AIMODE_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 8 Ammo Dump
	{	"Levitank.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&LevitankTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Levi},	// u_int16_t	GunType;			// What type of gun???
		{&LevitankFirePos},
		{&LevitankAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		5.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		1024,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ICANTPITCH | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.2F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			2.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			2.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 9 Levi Tank
	{	"Max.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&MaxSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			AI_BEHAVIOUR_PRECISECONTROL,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 10 Max Bot
	{	"Mekton.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&MektonTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH, //FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Mekton},	// u_int16_t	GunType;			// What type of gun???
		{&MektonTurretFirePos},
		{&MektonTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		3.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		500.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 20.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 10.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 11 Mekton
	{	"titan.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_NONE,
		AIMODE_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 12 NukeCrans ( Boss )
	{	"Supres.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&SupresFirePos},
		{&SupresAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		128,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


    	{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			5.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			5.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 13 Supressor
	{	"Sweep.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		5.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		512,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NME_LIGHTNING,				//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			1.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			1.0F,				// u_int16_t	Avoid_Secondary;				//
			100.0F,			// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F *  8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 14 Mine Sweeper
//		{	"fod1.cob",
//		{	"Metatank.cob",
		{	"Swarm.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
//		ENEMY_CONTROLTYPE_SPLINE,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		90.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_LEAVENETWORK | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 15 Swarm
	{	"Shade.mx",
		YES_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
//		ENEMY_CONTROLTYPE_SPLINE,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},


		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			8.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			5.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 16 Shade
	{	"Layer.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,	
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},


		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		5.0F,		// MaxMoveRate
		90.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		1024,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		120.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		PURGEMINE,					//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_DROPMINES | AI_BEHAVIOUR_AVOIDMINES,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 17 Mine Layer
	{	"Hunter.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
//		ENEMY_CONTROLTYPE_SPLINE,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			8.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			8.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 18 Hunter
	{	"legz.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&LegzSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Legz},	// u_int16_t	GunType;			// What type of gun???
		{&LegzFirePos},
		{&LegzAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ATTACK_ONSITE,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 19 Legz
	{	"guardbot.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Sec},	// u_int16_t	GunType;			// What type of gun???
		{&GuardBotFirePos},
		{&GuardBotAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 20 GuardBot
	{	"Metatank.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		COMPONENTCOLLISION,
		-1,
		&MetatankSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_SPLINE,
		AIMODE_FOLLOWPATH,
		4,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_MetaTank,GUN_MetaTank,GUN_MetaTank,GUN_MetaTankMain},	// u_int16_t	GunType;			// What type of gun???
		{&MetaTank1FirePos,&MetaTank2FirePos,&MetaTank3FirePos,&MetaTank4FirePos},
		{&MetaTank1AimPos,&MetaTank2AimPos,&MetaTank3AimPos,&MetaTank4AimPos},

		0.065F,		// TurnRateAccell
		0.01F,	// TurnRateDecell
		0.5F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		2048,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		600.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 64.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 21 Boss Metatank
	{	"sbfront.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		COMPONENTCOLLISION,
		-1,
		NULL,
		&BigGeekFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.025F,		// TurnRateAccell
		0.0211F,	// TurnRateDecell
		0.50F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		1.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		2048,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		900.0F * GLOBAL_SCALE,		//	float	Radius;
		120.0F,						//	float	PrimaryFireRate;
		60.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,					//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		ENEMYBLUEHOMINGMISSILE,		//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_RANDSECONDARYFIREPOS | AI_BEHAVIOUR_NOBOB | AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_AVOIDMINES | AI_BEHAVIOUR_PRECISECONTROL,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			(ONE_SECOND * 1.0F),			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 1.0F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 22 Boss BigGeek
	{	"avatar.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		5,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Avatar2,GUN_Avatar2,GUN_Avatar,GUN_Avatar,GUN_Avatar},	// u_int16_t	GunType;			// What type of gun???
		{&Avatar1FirePos,&Avatar2FirePos,&Avatar3FirePos,&Avatar4FirePos,&Avatar5FirePos},
		{&Avatar1AimPos,&Avatar2AimPos,&Avatar3AimPos,&Avatar4AimPos,&Avatar5AimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.0F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		4096,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		900.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			AI_BEHAVIOUR_ICANTPITCH | AI_BEHAVIOUR_NOBOB | AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			5.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			3.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			(ONE_SECOND * 1.0F),			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.1F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 23 Avatar
	{	"flesh.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		COMPONENTCOLLISION,
		-1,
		&FleshmorphSeqs[0],
		&FleshMorphFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLESHMORPH,
		AIMODE_FLESHMORPH_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.035F,		// TurnRateAccell
		0.0311F,	// TurnRateDecell
		1.0F,		// MaxTurnRate
		0.015F,		// MoveRateAccell
		0.0111F,	// MoveRateDecell
		2.25F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		1536,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
//		600.0F * GLOBAL_SCALE,		//	float	Radius;
		0.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		60.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		ENEMYTENTACLE,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
//			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_DONTSTOPANDTURN | AI_BEHAVIOUR_NOTURN ,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_DONTSTOPANDTURN | AI_BEHAVIOUR_NOTURN | AI_BEHAVIOUR_RANDSECONDARYFIREPOS,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			(ONE_SECOND * 1.0F),			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 1.0F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 24 Boss FleshMorph
	{	"exogenon.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		COMPONENTCOLLISION,
		-1,
		&ExogenonSeqs[0],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_EXOGENON,
		AIMODE_EXOGENON_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		2048,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		2,							//	int16_t	PowerLevel;
		NME_POWERLASER,				//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...


		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 21 Boss Exogenon
	{	"Borg400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 26 Lokasenna
		{	"Brd400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 27 Beard
		{	"Laj400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 28 LaJay
		{	"xcop400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 29 Excop
		{	"truk400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 30 Rex Hardy
		{	"foet400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 31 Foetoid
		{	"jap400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 32 NimSoo
		{	"nut400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 33 Nutta
		{	"rhes400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 33 Sceptre
		{	"jo400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 35 Jo
		{	"shrk400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 36 Cuvel Clark
		{	"hk400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 37 hk5
		{	"nbia400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 38 Nubia
		{	"mofi400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 39 Mofisto
		{	"cerb400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}


	},				// 40 Cerbero
		{	"slick400.mxa",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.075F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.075F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		8.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		256.0F,		// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		60.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		1,							//	int16_t	PowerLevel;
		TROJAX,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
			0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 41 Slick
	{	"LSwarm.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		90.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_LEAVENETWORK | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 42 Leader Swarm
	{	"LHunter.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			8.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			8.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 43 Leader Hunter
	{	"LMekton.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&MektonTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH, //FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Mekton},	// u_int16_t	GunType;			// What type of gun???
		{&MektonTurretFirePos},
		{&MektonTurretAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		3.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		500.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 20.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 10.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}
	},				// 44 Leader Mekton
	{	"LSnubot.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&SnubotTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH, //FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Snub},	// u_int16_t	GunType;			// What type of gun???
		{&SnubotFirePos},
		{&SnubotAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.5F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 20.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 10.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 45 Leader Snub Bot
	{	"LLegz.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&LegzSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_CRAWL_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Legz},	// u_int16_t	GunType;			// What type of gun???
		{&LegzFirePos},
		{&LegzAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		2.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 46 Leader Legz
	{	"LShade.mx",
		YES_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			8.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			5.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},		// 47 Leader Shade
	{	"LSupres.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&SupresFirePos},
		{&SupresAimPos},
		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		128,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
    	{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			5.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			5.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 48 Leader Supressor
	{	"LLevitank.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&LevitankTurretSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		1,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Levi},	// u_int16_t	GunType;			// What type of gun???
		{&LevitankFirePos},
		{&LevitankAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		5.0F,		// MaxMoveRate
		180.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		1024,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ICANTPITCH | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.2F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			2.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			2.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 49 Leader Levi Tank
	{	"LLazbot.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_NONE,
		AIMODE_IDLE,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&LazbotFirePos},
		{&LazbotAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			0,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 50 Leader Lazer Bot
	{	"LMax.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		&MaxSeqs[ 0 ],
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_PRECISECONTROL,				// u_int32_t	Flags;							// flags to show what stuff I can Do..
			0.0F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			0.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			0.0F,			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 51 Leader Max Bot
	{	"LAirmoble.mx",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_FLY_AI,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		PULSAR,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_DONTSTOPANDTURN | AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			0.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 52 Leader AirMoble
		{	"fod1.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		SPHERECOLLISION,
		-1,
		NULL,
		&DummyFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_SPLINE,
		AIMODE_FOLLOWPATH,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.065F,		// TurnRateAccell
		0.0611F,	// TurnRateDecell
		2.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		10.0F,		// MaxMoveRate
		90.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		256,		// Shield
		(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
		300.0F * GLOBAL_SCALE,		//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		30.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		NO_SECONDARY,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_LEAVENETWORK | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_ATTACKMINES | AI_BEHAVIOUR_AVOIDMINES,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			0.0F,			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 53 Fodder 1
	{	"sbback.cob",
		NO_STEALTH_MODE,
		NOT_LEVEL_SPECIFIC,
		COMPONENTCOLLISION,
		-1,
		&LittleGeekSeqs[0],
		&LittleGeekFirePos,
		&DummyAimPos,
		ENEMY_CONTROLTYPE_LITTLEGEEK,
		AIMODE_FORMATION,
		0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
		{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
		{&DummyFirePos},
		{&DummyAimPos},

		0.035F,		// TurnRateAccell
		0.0311F,	// TurnRateDecell
		1.625F,		// MaxTurnRate
		0.065F,		// MoveRateAccell
		0.0611F,	// MoveRateDecell
		5.0F,		// MaxMoveRate
		360.0F,		// Viewcone
		( 1024.0F * 2.0F ) * 1.5F * GLOBAL_SCALE,	// float	MinRange;			// minimum range I can dogfight at...
		( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
		1024,						// Shield
		(ONE_SECOND * 0.5F),		//	float	MoveRestrictClearTime;
		0.0F,						//	float	Radius;
		13.0F,						//	float	PrimaryFireRate;
		45.0F,						//	float	SecondaryFireRate;
		0,							//	int16_t	PowerLevel;
		NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
		MULTIPLEMISSILE,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
		{
			AI_BEHAVIOUR_NOBOB | AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_AVOIDMINES | AI_BEHAVIOUR_PRECISECONTROL,		// u_int32_t	Flags;	// flags to show what stuff I can Do..
			0.5F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
			0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
			10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
			20.0F,				// u_int16_t	Avoid_Secondary;				//
			30.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
			(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
			(ONE_SECOND * 1.0F),			// float	ScanTime;						// How long do I Scan For...
			(ONE_SECOND * 1.0F),			// float	ThinkTime;						// How often do I Update my Think Status...
			(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
			(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
			(10.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
			(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
		}

	},				// 54 Boss LittleGeek
	{	"FlyGirl.cob",
	NO_STEALTH_MODE,
	NOT_LEVEL_SPECIFIC,
	SPHERECOLLISION,
	-1,
	NULL,
	&DummyFirePos,
	&DummyAimPos,
	ENEMY_CONTROLTYPE_FLY_AI,
	AIMODE_FOLLOWPATH,
	0,			// u_int16_t	NumOfGuns;			// How many Guns do I Have???
	{GUN_Pulse},	// u_int16_t	GunType;			// What type of gun???
	{&DummyFirePos},
	{&DummyAimPos},

	0.075F,		// TurnRateAccell
	0.0611F,	// TurnRateDecell
	2.625F,		// MaxTurnRate
	0.075F,		// MoveRateAccell
	0.0611F,	// MoveRateDecell
	8.0F,		// MaxMoveRate
	180.0F,		// Viewcone
	256.0F,		// float	MinRange;			// minimum range I can dogfight at...
	( 1024.0F * 2.0F ) * 2.25F * GLOBAL_SCALE,	// float	MaxRange;			// maximum range I can dogfight at...
	256,		// Shield
	(ONE_SECOND * 0.75F),		//	float	MoveRestrictClearTime;
	300.0F * GLOBAL_SCALE,		//	float	Radius;
	60.0F,						//	float	PrimaryFireRate;
	30.0F,						//	float	SecondaryFireRate;
	2,							//	int16_t	PowerLevel;
	NO_PRIMARY,						//	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
	ENEMYFIREBALL,				//	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...
	{
		AI_BEHAVIOUR_ATTACK_ONSITE | AI_BEHAVIOUR_ATTACK_PROVOKE | AI_BEHAVIOUR_AVOID_PRIMARY | AI_BEHAVIOUR_AVOID_SECONDARY | AI_BEHAVIOUR_ANTICIPATE_MOVE | AI_BEHAVIOUR_NOFRIENDLYFIRE | AI_BEHAVIOUR_ATTACK_FIND | AI_BEHAVIOUR_WEAPONKNOWLEDGE | AI_BEHAVIOUR_PRECISECONTROL | AI_BEHAVIOUR_LEAVENETWORK,
		0.7F,			// float	Anticipate_Move;				// when targeting the player how far ahead can I predict where he will be....
		0.0F,			// float	NetworkRange;					// how far from my net work do I Wander...
		10.0F,				// u_int16_t	Avoid_Primary;					// if player fires at me and I can see it check to see if I should avoid...
		20.0F,				// u_int16_t	Avoid_Secondary;				//
		0.0F,				// u_int16_t	Avoid_Mines;					// If I move towards a mine should I try to avoid it...
		(ONE_SECOND * 1.0F),			// float	IdleTime;						// How long do I Stay Idle...
		0.0F,			// float	ScanTime;						// How long do I Scan For...
		(ONE_SECOND * 0.25F),			// float	ThinkTime;						// How often do I Update my Think Status...
		(1024.0F * 16.0F * GLOBAL_SCALE),			// float	ThinkRange;						// How far away does a valid player target have to be before I go To Sleep...
		(1024.0F * 8.0F * GLOBAL_SCALE),			// float	HearRange;						// How far away does a valid player target have to be before I can hear him...
		(20.0F * 60.0F),			// float	BlindSearchTime;						// If I lose my target how long will I Persue him...
		(20.0F * 60.0F),			// float	RetreatTime;					//  how long will I Run away..
	}
},				// 55 FlyGirl

};

ENEMY * TestEnemy = NULL;
	
/*===================================================================
	Enemy Control Routines...
===================================================================*/
ENEMY * PutEnemiesAtNodes(void);
void AutoMovement( OBJECT * Object , ENEMY * Enemy , bool AngleDecellBefore );
void AutoMovementCrawl( OBJECT * Object , ENEMY * Enemy );
void AutoDisplay( OBJECT * Object );
void CarryOutAIMovementCommands( ENEMY * Enemy );
void CarryOutPreciseAIMovementCommands( ENEMY * Enemy );
void CarryOutGUN_AIMovementCommands( GUNOBJECT * Object );
void CarryOutGUN_PreciseAIMovementCommands( GUNOBJECT * GObject );
void GunRotation( GUNOBJECT * Object, bool AngleDecellBefore  );
void RestrictMovement( ENEMY * Enemy , VECTOR * Move );
bool SplineAutoMovement( OBJECT * Object , ENEMY * Enemy );
void AutoMovementFleshmorph( OBJECT * Object , ENEMY * Enemy );

void EnemyFlyUnderPlayerControl( ENEMY * Enemy );
void EnemyFlyUnderAiControl( ENEMY * Enemy );
void EnemyCrawlUnderPlayerControl( ENEMY * Enemy );
void EnemyCrawlUnderAiControl( ENEMY * Enemy );
void EnemyTurretUnderPlayerControl( ENEMY * Enemy );
void EnemyTurretUnderAiControl( ENEMY * Enemy );
void EnemyUnderSplineControl( ENEMY * Enemy );
void EnemyUnderExogenonControl( ENEMY * Enemy );
void EnemyUnderFleshMorphControl( ENEMY * Enemy );
void EnemyUnderLittleGeekControl( ENEMY * Enemy );
void InitGuns( BYTE how_many_guns , u_int16_t * GunType , OBJECT * Object );
void SetWheelPos( VECTOR * DestPos , VECTOR * SourcePos , float xoff , float zoff , VECTOR * Right, VECTOR * Forward, u_int16_t Group , u_int16_t * DestGroup );
bool Enemy2EnemyCollide( ENEMY * SEnemy , VECTOR * Move );
bool Enemy2EnemyCollideSpecial( ENEMY * SEnemy , VECTOR * StartPos);
void AutoDisplayMatrix( OBJECT * Object );

	
void (* EnemyControlType[ ])( ENEMY * Enemy ) = {
	NULL,
	EnemyFlyUnderPlayerControl,
	EnemyFlyUnderAiControl,
	EnemyCrawlUnderPlayerControl,
	EnemyCrawlUnderAiControl,
	EnemyTurretUnderPlayerControl,
	EnemyTurretUnderAiControl,
	EnemyUnderSplineControl,
	EnemyUnderExogenonControl,
	EnemyUnderLittleGeekControl,
	EnemyUnderFleshMorphControl,
};

void AccellDecell(  float *  value ,  float  Decell ) 
{
	*value *= (float) (pow( 1.0 - Decell, framelag) );
}
#define MIN_AUTOLEVEL_PITCH	D2R(5.0F) 
#define MAX_AUTOLEVEL_PITCH	D2R(175.0F)

#define MIN_AUTOLEVEL_ROLL_TOLERANCE	(0.01F)
#define AUTOLEVEL_RATE					(0.1F)

#define FRAMES_PER_SEC	(60.0F)

#define BOB_XSIZE		(30.0F * GLOBAL_SCALE)
#define BOB_YSIZE		(10.0F * GLOBAL_SCALE)
#define BOB_XPERIOD		(2.0F * FRAMES_PER_SEC)
#define BOB_YPERIOD		(1.0F * FRAMES_PER_SEC)
#define BOB_XFREQ		(2.0F * PI / BOB_XPERIOD)
#define BOB_YFREQ		(2.0F * PI / BOB_YPERIOD)

void ObjectBob( OBJECT * Object, VECTOR *bob )
{
	VECTOR move;
	float move_len;

	move = Object->Speed;
	move_len = (float) sqrt( move.x * move.x + move.y * move.y + move.z * move.z );
	if ( move_len < MOVE_TOLERANCE )
	{
		move.x = BOB_XSIZE * (float) sin( Object->BobCount * BOB_XFREQ );
		move.y = BOB_YSIZE * (float) sin( Object->BobCount * BOB_YFREQ );
		move.z = 0.0F;
		ApplyMatrix( &Object->Mat, &move, bob );
		Object->BobCount += framelag;
	}
	else
	{
		bob->x = 0.0F;
		bob->y = 0.0F;
		bob->z = 0.0F;
		Object->BobCount = 0.0F;
		Object->LastBob = *bob;
	}
}


float ObjectAutoLevelRot( OBJECT * Object, float autolevel_rate )
{
	VECTOR up;
	VECTOR dir;
	VECTOR level;
	VECTOR ship_up;
	VECTOR side;
	float cos_pitch;
	float pitch;
	float cos_roll;
	float roll;
	float autolevel;
	float rot;

	if ( !autolevel_rate )
	{
		return 0.0F; // autolevelling disabled
	}
	if ( fabs( Object->Angle.z ) > MIN_AUTOLEVEL_ROLL_TOLERANCE )
	{
		return 0.0F; // ship still rolling under player control
	}
	up = Mloadheader.Group[ Object->Group ].up;
	ApplyMatrix( &Object->Mat, &Forward, &dir );
	cos_pitch = up.x * dir.x + up.y * dir.y + up.z * dir.z;
	if ( cos_pitch < -1.0F )
		cos_pitch = -1.0F;
	else if ( cos_pitch > 1.0F )
		cos_pitch = 1.0F;
	pitch = (float) acos( cos_pitch );
	if ( pitch < MIN_AUTOLEVEL_PITCH || pitch > MAX_AUTOLEVEL_PITCH )
	{
		return 0.0F;
	}
	CrossProduct( &up, &dir, &level );
	CrossProduct( &dir, &level, &ship_up );
	ApplyMatrix( &Object->Mat, &SlideRight, &side );
	cos_roll = ship_up.x * side.x + ship_up.y * side.y + ship_up.z * side.z;
	if ( cos_roll < -1.0F )
		cos_roll = -1.0F;
	else if ( cos_roll > 1.0F )
		cos_roll = 1.0F;
	roll = (float) acos( cos_roll );
	autolevel = ( R2D( roll ) - 90.0F );
	rot = autolevel_rate * autolevel;
	if ( fabs( rot ) > fabs( autolevel ) )
		rot = autolevel;
	return rot;
}

	
/*===================================================================
	Procedure	:	Initialise all Enemies
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void InitEnemies( void )
{
	u_int16_t	i;

	SetupEnemyGroups();

	FirstEnemyUsed = NULL;
	FirstEnemyFree = &Enemies[ 0 ];

	for( i = 0; i < MAXENEMIES; i++ )
	{
		memset( &Enemies[ i ], 0, sizeof( ENEMY ) );

		Enemies[ i ].Used = false;
		Enemies[ i ].NextUsed = NULL;
		Enemies[ i ].PrevUsed = NULL;
		Enemies[ i ].NextFree = &Enemies[ i + 1 ];
		Enemies[ i ].PrevFree = &Enemies[ i - 1 ];

		Enemies[ i ].NextInGroup = NULL;
		Enemies[ i ].PrevInGroup = NULL;
		Enemies[ i ].Index = i;
		Enemies[ i ].Object.Type = OBJECT_TYPE_ENEMY;
	}

	Enemies[ 0 ].PrevFree = NULL;
	Enemies[ MAXENEMIES - 1 ].NextFree = NULL;
}

/*===================================================================
	Procedure	:	Find free Enemy
	Input		:	nothing
	Output		:	ENEMY	*	Object ( NULL if not available )
===================================================================*/
ENEMY * FindFreeEnemy( void )
{
	ENEMY * Object;

	Object = FirstEnemyFree;

	if( Object )
	{
		if( Object->Used )
		{
			// This enemy has been used before....
			Msg( "An enemy has been allocated more than once\n" );
			return NULL;
		}

		FirstEnemyFree = Object->NextFree;
		if( FirstEnemyFree != NULL )
		{
			FirstEnemyFree->PrevFree = NULL;
		}

		if( FirstEnemyUsed != NULL )
		{
			FirstEnemyUsed->PrevUsed = Object;
			Object->NextUsed = FirstEnemyUsed;
		}
		else
		{
			Object->NextUsed = NULL;
		}

		Object->PrevUsed = NULL;
		FirstEnemyUsed = Object;
		Object->Used = true;

	}

	return( Object );
}

/*===================================================================
	Procedure	:	Kill an Enemy
	Input		:	ENEMY	*	Object
	Output		:	nothing
===================================================================*/
extern  BYTE          MyGameStatus;
#ifdef DEBUG_ON
void KillUsedEnemyDebugOn( ENEMY * Object, char *in_file, int in_line )
#else
void KillUsedEnemy( ENEMY * Object )
#endif
{
	ENEMY	*	PrevObject;
	ENEMY	*	NextObject;
	VECTOR		Dir = { 0.0F, 0.0F, 0.0F };
	float		Speed;
	float		LifeCount;
	int16_t		Pickup;

	if( Object )
	{

		if( !Object->Used )
		{

#ifdef DEBUG_ON
			DebugPrintf(
				"An %s enemy Freed Previously in %s line %d\n has been freed again in %s line %d\n" ,
				EnemyTypes[Object->Type].ModelFilename,
				Object->EnemyInFile,
				Object->EnemyInLine,
				in_file,
				in_line
				);
#endif

			Msg( "An %s enemy has been Freed more than once\n" , EnemyTypes[Object->Type].ModelFilename );

		}

#ifdef DEBUG_ON
		Object->EnemyInFile = in_file;
		Object->EnemyInLine = in_line;
#endif
		

		NumKilledEnemies++;
		StopEnemyBikerTaunt( Object );
		Object->Alive = false;
		RemoveEnemyFromGroup( Object, Object->Object.Group );

		if( Object->Object.FirstGun )
		{
			free( Object->Object.FirstGun);
			Object->Object.FirstGun = NULL;
			Object->Object.HowManyGuns = 0;
		}

		if( ( Object->PickupHeld != PICKUP_GeneralAmmo ) &&
			( Object->PickupHeld != PICKUP_PyroliteAmmo ) &&
			( Object->PickupHeld != PICKUP_SussGunAmmo ) )
		{
			Pickup = GetAmmoPickupToGen();

			if( Pickup != -1 )
			{
	   			Dir.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   			Dir.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   			Dir.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				NormaliseVector( &Dir );
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				InitOnePickup( &Object->Object.Pos, Object->Object.Group, &Dir, Speed, Pickup, (u_int16_t) -1, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
				if( IsHost ) DropPickupSend( &Object->Object.Pos, Object->Object.Group, &Dir, 0.0F, Object->PickupHeld, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			}
		}

		if( ( Object->PickupHeld != (u_int16_t) -1 ) && !( Object->Object.Flags & SHIP_Scattered ) )
		{
	   		Dir.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   		Dir.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   		Dir.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			NormaliseVector( &Dir );
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (u_int16_t) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			InitOnePickup( &Object->Object.Pos, Object->Object.Group, &Dir, Speed, Object->PickupHeld, (u_int16_t) -1, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
			if( IsHost ) DropPickupSend( &Object->Object.Pos, Object->Object.Group, &Dir, 0.0F, Object->PickupHeld, ++Ships[WhoIAm].PickupIdCount, -1, false, LifeCount, (u_int16_t) -1 );
		}

		if( Object->TriggerModPtr != NULL )
		{
			ApplyTriggerMod( Object->TriggerModPtr );
		}

		if( Object == FirstEnemyUsed )
		{
			NextObject = FirstEnemyUsed->NextUsed;

			FirstEnemyUsed = NextObject;

			if( FirstEnemyUsed != NULL )
			{
				FirstEnemyUsed->PrevUsed = NULL;
			}
		}
		else
		{
			PrevObject = Object->PrevUsed;
			NextObject = Object->NextUsed;

			if( PrevObject != NULL )
			{
				PrevObject->NextUsed = NextObject;
			}

			if( NextObject != NULL )
			{
				NextObject->PrevUsed = PrevObject;
			}
		}

		if( FirstEnemyFree != NULL )
		{
			FirstEnemyFree->PrevFree = Object;
		}

		if( Object->ModelIndex != (u_int16_t) -1 )
		{
			KillUsedModel( Object->ModelIndex );
			Object->ModelIndex = (u_int16_t) -1;
		}

		if( Object->Object.Components )
		{
			FreeCompObjChildren( Object->Object.Components, 1 );
			if( Object->Object.Components )
			{
				free( Object->Object.Components );
				Object->Object.Components = NULL;
			}
		}

		Object->NextFree = FirstEnemyFree;
		Object->PrevFree = NULL;
		FirstEnemyFree = Object;
		Object->Used = false;

	}
}

/*===================================================================
	Procedure	:	Release all enemies
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ReleaseAllEnemies( void )
{
	ENEMY	*	Object;
	ENEMY	*	NextObject;
	ENEMY	*	PrevObject;
	ENEMY	*	NextUsedObject;

	Object = FirstEnemyUsed;

	while( Object != NULL )
	{
		NextUsedObject = Object->NextUsed;

		RemoveEnemyFromGroup( Object, Object->Object.Group );

		if( Object->Object.FirstGun )
		{
			free( Object->Object.FirstGun);
			Object->Object.FirstGun = NULL;
		}

		if( Object == FirstEnemyUsed )
		{
			NextObject = FirstEnemyUsed->NextUsed;

			FirstEnemyUsed = NextObject;

			if( FirstEnemyUsed != NULL )
			{
				FirstEnemyUsed->PrevUsed = NULL;
			}
		}
		else
		{
			PrevObject = Object->PrevUsed;
			NextObject = Object->NextUsed;

			if( PrevObject != NULL )
			{
				PrevObject->NextUsed = NextObject;
			}

			if( NextObject != NULL )
			{
				NextObject->PrevUsed = PrevObject;
			}
		}

		if( FirstEnemyFree != NULL )
		{
			FirstEnemyFree->PrevFree = Object;
		}

		if( Object->ModelIndex != (u_int16_t) -1 )
		{
			KillUsedModel( Object->ModelIndex );
			Object->ModelIndex = (u_int16_t) -1;
		}

		if( Object->Object.Components )
		{
			FreeCompObjChildren( Object->Object.Components, 1 );
			if( Object->Object.Components )
			{
				free( Object->Object.Components );
				Object->Object.Components = NULL;
			}
		}

		Object->NextFree = FirstEnemyFree;
		Object->PrevFree = NULL;
		FirstEnemyFree = Object;

		Object = NextUsedObject;
	}
}

/*===================================================================
	Procedure	:	PreLoad Enemies
	Input		:	Nothing
	Output		:	bool	True/False
===================================================================*/
extern	int16_t		LevelNum;
bool PreLoadEnemies( void )
{
	FILE	*	fp;
	int16_t		Count;
	char	*	NewExt = ".NME";
	char		NewFilename[ 128 ];
	char		TempFilename[ 256 ];
	u_int16_t		Group;
	u_int16_t		ModelNum;
	u_int16_t		TriggerMod;
	u_int16_t		GenType;
	u_int32_t		Network;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	char		Ext[ 16 ];
	int16_t		NextEnemyModel;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	int16_t		Pickup;
	int16_t		Formationlink;
	u_int16_t		DupModel;
	float		GenerationDelay;

	ModNames = &ModelNames[ 0 ];

	Difficulty = DifficlutyTab[DifficultyLevel];
	NmeDamageModifier = NmeDamageModifierTab[DifficultyLevel];

	if( ( ChangeLevel_MyGameStatus != STATUS_PostStartingSinglePlayer ) &&
		( ChangeLevel_MyGameStatus != STATUS_SinglePlayer ) &&
		( ChangeLevel_MyGameStatus != STATUS_TitleLoadGamePostStartingSinglePlayer ) )
		return true;

	ReadEnemyTxtFile( "data\\txt\\Enemies.txt" );

	NextEnemyModel = NextNewModel;

	for( Count = 0; Count < MAX_ENEMY_TYPES; Count++ ) EnemyTypes[ Count ].ModelNumber = -1;
	
	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );

	fp = file_open( &NewFilename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != NME_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "PreLoadEnemies() Incompatible enemy file %s", &NewFilename[ 0 ] );
			return( false );
		}

		fread( &NumInitEnemies, sizeof( int16_t ), 1, fp );
		NumKilledEnemies = 0;

		for( Count = 0; Count < NumInitEnemies; Count++ )
		{
			fread( &Group, sizeof( u_int16_t ), 1, fp );
			fread( &ModelNum, sizeof( u_int16_t ), 1, fp );
			fread( &GenType, sizeof( u_int16_t ), 1, fp );
			fread( &Pos, sizeof( VECTOR ), 1, fp );
			fread( &DirVector, sizeof( VECTOR ), 1, fp );
			fread( &UpVector, sizeof( VECTOR ), 1, fp );
			fread( &TriggerMod, sizeof( u_int16_t ), 1, fp );
			fread( &Network, sizeof( u_int32_t ), 1, fp );
			fread( &Pickup, sizeof( int16_t ), 1, fp );
			fread( &Formationlink, sizeof( int16_t ), 1, fp );
			fread( &GenerationDelay, sizeof( float ), 1, fp );

			if( EnemyTypes[ ModelNum ].ModelFilename[ 0 ] )
			{

				Get_Ext( &EnemyTypes[ ModelNum ].ModelFilename[ 0 ], &Ext[ 0 ] );

				if( !strcasecmp( &Ext[ 0 ], "MXA" ) )
				{
					if( EnemyTypes[ ModelNum ].ModelNumber == -1 )
					{
						DupModel = FindDuplicateModel( &EnemyTypes[ ModelNum ].ModelFilename[ 0 ], NextEnemyModel );

						if( DupModel != (u_int16_t) -1 )
						{
							EnemyTypes[ ModelNum ].ModelNumber = DupModel;
						}
						else
						{
							strcpy( &ModelNames[ NextEnemyModel ].Name[ 0 ], EnemyTypes[ ModelNum ].ModelFilename );

							ModelNames[ NextEnemyModel ].LOD = 0;
							ModelNames[ NextEnemyModel ].Panel = false;
							ModelNames[ NextEnemyModel ].DoIMorph = true;
							ModelNames[ NextEnemyModel ].ModelIndex = NextEnemyModel;
							ModelNames[ NextEnemyModel ].StoreTriangles = false;
							ModelNames[ NextEnemyModel ].AllocateTpage = LOAD_TPAGES;
							ModelNames[ NextEnemyModel ].LevelSpecific = EnemyTypes[ ModelNum ].LevelSpecific;
							ModelNames[ NextEnemyModel ].LoadEnable = true;
							EnemyTypes[ ModelNum ].ModelNumber = NextEnemyModel;
							NextEnemyModel++;

							if( EnemyTypes[ ModelNum ].StealthMode )
							{
								ModelNames[ NextEnemyModel ].Name[ 0 ] = 'S';
								strcpy( &ModelNames[ NextEnemyModel ].Name[ 1 ], EnemyTypes[ ModelNum ].ModelFilename );
								ModelNames[ NextEnemyModel ].LOD = 0;
								ModelNames[ NextEnemyModel ].Panel = false;
								ModelNames[ NextEnemyModel ].DoIMorph = true;
								ModelNames[ NextEnemyModel ].ModelIndex = NextEnemyModel;
								ModelNames[ NextEnemyModel ].StoreTriangles = false;
								ModelNames[ NextEnemyModel ].AllocateTpage = LOAD_TPAGES;
								ModelNames[ NextEnemyModel ].LevelSpecific = EnemyTypes[ ModelNum ].LevelSpecific;
								ModelNames[ NextEnemyModel ].LoadEnable = true;
								NextEnemyModel++;
							}
						}
					}
				}
				else
				{
					if( !strcasecmp( &Ext[ 0 ], "MX" ) )
					{
						if( EnemyTypes[ ModelNum ].ModelNumber == -1 )
						{
							DupModel = FindDuplicateModel( &EnemyTypes[ ModelNum ].ModelFilename[ 0 ], NextEnemyModel );

							if( DupModel != (u_int16_t) -1 )
							{
								EnemyTypes[ ModelNum ].ModelNumber = DupModel;
							}
							else
							{
								strcpy( &ModelNames[ NextEnemyModel ].Name[ 0 ], EnemyTypes[ ModelNum ].ModelFilename );

								ModelNames[ NextEnemyModel ].LOD = 0;
								ModelNames[ NextEnemyModel ].Panel = false;
								ModelNames[ NextEnemyModel ].DoIMorph = false;
								ModelNames[ NextEnemyModel ].ModelIndex = NextEnemyModel;
								ModelNames[ NextEnemyModel ].StoreTriangles = false;
								ModelNames[ NextEnemyModel ].AllocateTpage = LOAD_TPAGES;
								ModelNames[ NextEnemyModel ].LevelSpecific = EnemyTypes[ ModelNum ].LevelSpecific;
								ModelNames[ NextEnemyModel ].LoadEnable = true;
								EnemyTypes[ ModelNum ].ModelNumber = NextEnemyModel;
								NextEnemyModel++;

								if( EnemyTypes[ ModelNum ].StealthMode )
								{
									ModelNames[ NextEnemyModel ].Name[ 0 ] = 'S';
									strcpy( &ModelNames[ NextEnemyModel ].Name[ 1 ], EnemyTypes[ ModelNum ].ModelFilename );
									ModelNames[ NextEnemyModel ].LOD = 0;
									ModelNames[ NextEnemyModel ].Panel = false;
									ModelNames[ NextEnemyModel ].DoIMorph = false;
									ModelNames[ NextEnemyModel ].ModelIndex = NextEnemyModel;
									ModelNames[ NextEnemyModel ].StoreTriangles = false;
									ModelNames[ NextEnemyModel ].AllocateTpage = LOAD_TPAGES;
									ModelNames[ NextEnemyModel ].LevelSpecific = EnemyTypes[ ModelNum ].LevelSpecific;
									ModelNames[ NextEnemyModel ].LoadEnable = true;
									NextEnemyModel++;
								}
							}
						}
					}
					else
					{
						if( !strcasecmp( &Ext[ 0 ], "COB" ) )
						{
							if( EnemyTypes[ ModelNum ].ModelNumber == -1 )
							{
								EnemyTypes[ ModelNum ].ModelNumber = NextEnemyModel;

								if( EnemyTypes[ ModelNum ].LevelSpecific )
								{
									sprintf( &TempFilename[ 0 ], "data\\levels\\%s\\bgobjects\\%s", &ShortLevelNames[ LevelNum ][ 0 ], EnemyTypes[ ModelNum ].ModelFilename );
								}
								else
								{
									sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", EnemyTypes[ ModelNum ].ModelFilename );
								}

								if( !PreLoadCompObj( &TempFilename[ 0 ], &NextEnemyModel, EnemyTypes[ ModelNum ].LevelSpecific ) )
								{
									return( false );
								}
							}
						}
					}
				}
			}
			else
			{
				DebugPrintf( "Enemy %d has no model\n", ModelNum );
			}

		}

		NextNewModel = NextEnemyModel;

		fclose( fp );
	}
	return( true );
}

/*===================================================================
	Procedure	:	Load Enemies
	Input		:	Nothing
	Output		:	bool	True/False
===================================================================*/
bool LoadEnemies( void )
{
	FILE	*	fp;
	int16_t		Count;
	int16_t		CompCount;
	char	*	NewExt = ".NME";
	char		NewFilename[ 128 ];
	char		TempFilename[ 256 ];
	u_int16_t		Group;
	u_int16_t		ModelNum;
	u_int16_t		TriggerMod;
	u_int16_t		GenType;
	u_int32_t		Network;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	ENEMY	*	Enemy;
	int16_t		TempNum;
	char		Ext[ 16 ];
	COMP_OBJ *	Comp = NULL;
	COMP_OBJ *	TempComp = NULL;
	float		OverallTime, MidTime;
	u_int16_t		EnemyType;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	ENEMY * LinkEnemy;
	VECTOR	Offset;
	int16_t		Pickup;
	int16_t		Formationlink;
	VECTOR		TempVector = { 0.0F, 0.0F, 0.0F };
	float		GenerationDelay;

	Exogenon_Num_StartPos = 0;
	BigGeek = NULL;
	EnemyTypes[ENEMY_Boss_LittleGeek].Radius = 0.0F;
	EnemyTypes[ENEMY_Boss_BigGeek].Radius = 1200.0F * GLOBAL_SCALE;

	if( ChangeLevel_MyGameStatus != STATUS_PostStartingSinglePlayer && ChangeLevel_MyGameStatus != STATUS_SinglePlayer && ChangeLevel_MyGameStatus != STATUS_TitleLoadGamePostStartingSinglePlayer )
	{
		NumInitEnemies = 0;
		return true;
	}

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );

	fp = file_open( &NewFilename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != NME_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadEnemies() Incompatible enemy (.NME) file %s", &NewFilename[ 0 ] );
			return( false );
		}

		fread( &NumInitEnemies, sizeof( int16_t ), 1, fp );

		for( Count = 0; Count < NumInitEnemies; Count++ )
		{
			fread( &Group, sizeof( u_int16_t ), 1, fp );
			fread( &EnemyType, sizeof( u_int16_t ), 1, fp );
			fread( &GenType, sizeof( u_int16_t ), 1, fp );
			fread( &Pos, sizeof( VECTOR ), 1, fp );
			fread( &DirVector, sizeof( VECTOR ), 1, fp );
			fread( &UpVector, sizeof( VECTOR ), 1, fp );
			fread( &TriggerMod, sizeof( u_int16_t ), 1, fp );
			fread( &Network, sizeof( u_int32_t ), 1, fp );
			fread( &Pickup, sizeof( int16_t ), 1, fp );
			fread( &Formationlink, sizeof( int16_t ), 1, fp );
			fread( &GenerationDelay, sizeof( float ), 1, fp );

			ModelNum = EnemyType;

			if( (EnemyType != ENEMY_Boss_Exogenon) || (Exogenon_Num_StartPos == 0) )
			{
				if( EnemyTypes[ ModelNum ].ModelFilename[ 0 ] )
				{
					Get_Ext( EnemyTypes[ ModelNum ].ModelFilename, &Ext[ 0 ] );

					if( !strcasecmp( &Ext[ 0 ], "MXA" ) )
					{
						if( EnemyTypes[ ModelNum ].ModelNumber != -1 )
						{
							ModelNum = EnemyTypes[ ModelNum ].ModelNumber;
							Comp = NULL;
						}
						else
						{
							Msg( "Error loading enemy %s\n", &NewFilename[ 0 ] );
							return( false );
						}
					}
					else
					{
						if( !strcasecmp( &Ext[ 0 ], "MX" ) )
						{
							if( EnemyTypes[ ModelNum ].ModelNumber != -1 )
							{
								ModelNum = EnemyTypes[ ModelNum ].ModelNumber;
								Comp = NULL;
							}
							else
							{
								Msg( "Error loading enemy %s\n", &NewFilename[ 0 ] );
								return( false );
							}
						}
						else
						{
							if( !strcasecmp( &Ext[ 0 ], "COB" ) )
							{
								if( EnemyTypes[ ModelNum ].ModelNumber != -1 )
								{
									TempNum = EnemyTypes[ ModelNum ].ModelNumber;

									if( EnemyTypes[ ModelNum ].LevelSpecific )
									{
										sprintf( &TempFilename[ 0 ], "data\\levels\\%s\\bgobjects\\%s", &ShortLevelNames[ LevelNum ][ 0 ], EnemyTypes[ ModelNum ].ModelFilename );
									}
									else
									{
										sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", EnemyTypes[ ModelNum ].ModelFilename );
									}

									Comp = LoadCompObj( &TempFilename[ 0 ], &Pos, &DirVector, Group,
														&OverallTime, &MidTime, &TempNum, OWNER_ENEMY, Count );
									if( Comp )
									{
										ModelNum = (u_int16_t) -1;
									}
									else
									{
										Msg( "Error loading enemy %s\n", &NewFilename[ 0 ] );
										return( false );
									}
								}
							}
							else
							{
								Msg( "Error loading enemy %s\n", &NewFilename[ 0 ] );
								return( false );
							}
						}
					}
				}
				else
				{
					DebugPrintf( "Enemy %d has no model\n", ModelNum );
				}

				Enemy = InitOneEnemy( GenType, &Pos, &DirVector, &UpVector, Group, ModelNum, TriggerMod ,EnemyType , Network , Pickup , (u_int16_t) Formationlink , GenerationDelay ); //-1);

				if( Enemy != NULL )
				{
					Enemy->Object.AnimSpeed = 1.0F;
					Enemy->Object.AnimSeqs = EnemyTypes[EnemyType].AnimSeqs;
					if( Enemy->Object.AnimSeqs ) Enemy->Object.CurAnimSeq = 0;
					else Enemy->Object.CurAnimSeq = -1;
					Enemy->Object.Components = Comp;

					if( Comp )
					{
						Enemy->CompCollision = EnemyTypes[ EnemyType ].CompCollision;

						GetCompObjBoundingBox( &MATRIX_Identity, &TempVector, Enemy->Object.Components, 1,
											   Enemy->Object.OverallTime, &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );

						MaximizeBoundingBox( &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );

						if( !( Enemy->Status & ENEMY_STATUS_Enable ) )
						{
							SetCompObjModelsState( Comp, 1, false );
						}
					}
					else
					{
						if( ModelNames[ ModelNum ].DoIMorph )
						{
							GetMXABoundingBox( &MxaModelHeaders[ ModelNum ], &MATRIX_Identity, &TempVector, &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );
							MaximizeBoundingBox( &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );
						}
						else
						{
							GetMXBoundingBox( &ModelHeaders[ ModelNum ], &MATRIX_Identity, &TempVector, &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );
							MaximizeBoundingBox( &Enemy->Object.TopLeft, &Enemy->Object.BottomRight );
						}
					}

					Enemy->Object.Time = 0.0F;
					Enemy->Object.OverallTime = OverallTime;

					switch( EnemyType )
					{
						case ENEMY_SnubTurret:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 2, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 3, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							break;

						case ENEMY_BeamTurret:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 0, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 1, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}
							}
							break;

						case ENEMY_DuelTurret:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 2, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}
							}
							break;

						case ENEMY_PulseTurret:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 2, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}
							}
							break;

						case ENEMY_MissileTurret:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 2, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}
							}
							break;

						case ENEMY_LEADER_SnubBot:
						case ENEMY_SnubBot:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 2, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}

								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}
							}
							break;

						case ENEMY_AirMoble:
						case ENEMY_AmmoDump:
						case ENEMY_Max:
							break;

						case ENEMY_LEADER_LeviTank:
						case ENEMY_LeviTank:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 1, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							break;

						case ENEMY_LEADER_Mekton:
						case ENEMY_Mekton:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 2, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}

								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}

							}
							break;

						case ENEMY_GuardBot:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 3, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = GetCompObjAddress( 4, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							if( Enemy->Object.UserContComps[ TURRETCOMP_Gun ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Gun ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 1.0F;
									TempComp->UserAxis.y = 0.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}

								if( TempComp->UserAxis.x > 0.0F )			// Temp Fix;
								{
									TempComp->UserAxis.x = -TempComp->UserAxis.x;
									TempComp->UserAxis.y = -TempComp->UserAxis.y;
									TempComp->UserAxis.z = -TempComp->UserAxis.z;
								}

							}
							break;

						case ENEMY_LEADER_Legz:
						case ENEMY_Legz:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							break;

						case ENEMY_LazerBot:
						case ENEMY_LEADER_LazerBot:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							if( Enemy->Object.UserContComps[ TURRETCOMP_Base ] )
							{
								TempComp = Enemy->Object.UserContComps[ TURRETCOMP_Base ];
								TempComp->UserControl = true;
								if( !GetCompObjAxis( TempComp ) )
								{
									TempComp->UserAxis.x = 0.0F;
									TempComp->UserAxis.y = 1.0F;
									TempComp->UserAxis.z = 0.0F;
									TempComp->UserAxisPoint.x = 0.0F;
									TempComp->UserAxisPoint.y = 0.0F;
									TempComp->UserAxisPoint.z = 0.0F;
								}
							}
							break;

						case ENEMY_Boss_Metatank:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 2 ] = GetCompObjAddress( 2, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun + 2 ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 4 ] = GetCompObjAddress( 3, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun+ 4 ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 6 ] = GetCompObjAddress( 5, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun + 6 ] = NULL;

							for( CompCount = 0; CompCount < 4; CompCount++ )
							{
								if( Enemy->Object.UserContComps[ CompCount * 2 ] )
								{
									TempComp = Enemy->Object.UserContComps[ CompCount * 2 ];
									TempComp->UserControl = true;
									if( !GetCompObjAxis( TempComp ) )
									{
										TempComp->UserAxis.x = 0.0F;
										TempComp->UserAxis.y = 1.0F;
										TempComp->UserAxis.z = 0.0F;
										TempComp->UserAxisPoint.x = 0.0F;
										TempComp->UserAxisPoint.y = 0.0F;
										TempComp->UserAxisPoint.z = 0.0F;
									}
								}
							}
							break;

						case ENEMY_Boss_Avatar:
							Enemy->Object.UserContComps[ TURRETCOMP_Base ] = GetCompObjAddress( 4, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 2 ] = GetCompObjAddress( 5, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun + 2 ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 4 ] = GetCompObjAddress( 3, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun+ 4 ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 6 ] = GetCompObjAddress( 2, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun + 6 ] = NULL;
							Enemy->Object.UserContComps[ TURRETCOMP_Base + 8 ] = GetCompObjAddress( 1, 1, Comp );
							Enemy->Object.UserContComps[ TURRETCOMP_Gun + 8 ] = NULL;

							for( CompCount = 0; CompCount < 5; CompCount++ )
							{
								if( Enemy->Object.UserContComps[ CompCount * 2 ] )
								{
									TempComp = Enemy->Object.UserContComps[ CompCount * 2 ];
									TempComp->UserControl = true;
									if( !GetCompObjAxis( TempComp ) )
									{
										TempComp->UserAxis.x = 0.0F;
										TempComp->UserAxis.y = 1.0F;
										TempComp->UserAxis.z = 0.0F;
										TempComp->UserAxisPoint.x = 0.0F;
										TempComp->UserAxisPoint.y = 0.0F;
										TempComp->UserAxisPoint.z = 0.0F;
									}
								}
							}
							break;

						case ENEMY_Boss_Nukecrane:
						case ENEMY_Supressor:
						case ENEMY_MineSweeper:
						case ENEMY_Swarm:
						case ENEMY_Shade:
						case ENEMY_MineLayer:
						case ENEMY_Hunter:
							break;
					}
				}
				else
				{
					fclose( fp );
					Msg( "Error loading Enemies\n %s\n Unable to init enemy\n", &NewFilename[ 0 ] );
					return( false );
				}
			}else{
				// we must fill in an enemy for the exogenon placeholder....
				Enemy = InitOneEnemy( ENEMY_STATUS_Nothing, &Pos, &DirVector, &UpVector, Group, (u_int16_t) -1, (u_int16_t) -1 ,EnemyType , 0 , -1 , (u_int16_t) -1 , 0.0F );
				Enemy->Status &= ~ENEMY_STATUS_Enable;
				Enemy->Alive = false;
			}
			if( EnemyType == ENEMY_Boss_Exogenon )
			{
				Exogenon_StartPos[Exogenon_Num_StartPos] = Pos;
				Exogenon_Num_StartPos++;
			}
			if( EnemyType == ENEMY_Boss_BigGeek )
			{
				BigGeek = Enemy;
			}
			if( EnemyType == ENEMY_Boss_LittleGeek )
			{
				EnemyTypes[EnemyType].ControlType = ENEMY_CONTROLTYPE_LITTLEGEEK;
				EnemyTypes[EnemyType].Radius = 0.0F;
			}
			if( EnemyType == ENEMY_Boss_FleshMorph )
			{
				FleshMorphTimer = 60.0F * 4.0F;
				FleshmorphPos = Enemy->Object.Pos;
			}
			if( EnemyType == ENEMY_Max || EnemyType == ENEMY_LEADER_Max )
			{
				SetCurAnimSeq( 0, &Enemy->Object );
			}
		}

		fclose( fp );
	}

	// work out formation offsets....	
	Enemy = FirstEnemyUsed;
	while( Enemy )
	{
		if( Enemy->Type == ENEMY_Boss_LittleGeek )
		{
			Enemy->FormationLink = BigGeek;
		}
		if( Enemy->FormationLink )
		{
			LinkEnemy = Enemy->FormationLink;
			Offset.x = Enemy->Object.Pos.x - LinkEnemy->Object.Pos.x;
			Offset.y = Enemy->Object.Pos.y - LinkEnemy->Object.Pos.y;
			Offset.z = Enemy->Object.Pos.z - LinkEnemy->Object.Pos.z;
			ApplyMatrix( &LinkEnemy->Object.InvMat, &Offset, &Enemy->FormationOffset );
			Enemy->Object.AI_Mode = AIMODE_FORMATION;
		}
		Enemy = Enemy->NextUsed;
	}
	return( true );
}

/*===================================================================
	Procedure	:	Init Enemy
	Input		:	u_int16_t		GenType
				:	VECTOR	*	Pos
				:	VECTOR	*	Dir Vector
				:	VECTOR	*	Up Vector
				:	u_int16_t		Group
				:	u_int16_t		ModelNum
				:	u_int16_t		TriggerMod
				:	u_int16_t		EnemyType
				:	u_int32_t		Network
				:	int32_t		PickupHeld
				:	u_int16_t		FormationLink
	Output		:	ENEMY	*	Enemy
===================================================================*/
ENEMY * InitOneEnemy( u_int16_t GenType, VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group, u_int16_t ModelNum, u_int16_t TriggerMod , u_int16_t EnemyType , u_int32_t Network , int32_t PickupHeld , u_int16_t FormationLink , float GenerationDelay)
{
	int16_t		Count;
	ENEMY	*	Enemy;
	u_int16_t		ModelIndex;
	VECTOR		TempPos;
	u_int16_t		TempGroup;

	Enemy = FindFreeEnemy();

	if( Enemy != NULL )
	{
		
#ifdef DEBUG_ON
		Enemy->EnemyInFile = &EnemyNotFreedString[0];
		Enemy->EnemyInLine = 0;
#endif
		AddEnemyToGroup( Enemy, Group );


		Enemy->TempSplineNode.NodeNum = SPECIALENEMYNODENUM;

		Enemy->GenType = GenType;

		if( GenType == ENEMY_GENTYPE_Time )
		{
			Enemy->Status = ENEMY_STATUS_Nothing;
			Enemy->GenerationDelay = GenerationDelay * 60.0F;
			Enemy->MasterGenerationDelay = 0.0F;
		}else if( GenType == ENEMY_GENTYPE_Initialised )
		{
			Enemy->Status = ENEMY_STATUS_Enable;
			Enemy->GenerationDelay = 0.0F;
			Enemy->MasterGenerationDelay = 0.0F;
		}else{ 
			Enemy->Status = ENEMY_STATUS_Nothing;
			Enemy->MasterGenerationDelay = GenerationDelay * 60.0F;
			Enemy->GenerationDelay = 0.0F;
		}
		
		Enemy->Alive = true;
		Enemy->CompCollision = false;

		Enemy->Type	= EnemyType;
		Enemy->BulletID = 0;
		Enemy->Object.FinalMat = MATRIX_Identity;
		MatrixTranspose( &Enemy->Object.FinalMat, &Enemy->Object.FinalInvMat );

		Enemy->Object.ControlType =	EnemyTypes[EnemyType].ControlType;

		Enemy->Object.Type = OBJECT_TYPE_ENEMY;
		Enemy->Object.ID = (void*) Enemy;
		Enemy->Object.Components = NULL;
		Enemy->Object.Flags = 0;

		for( Count = 0; Count < MAX_USER_CONT_COMPS; Count++ )
		{
			Enemy->Object.UserContComps[ Count ] = NULL;
		}
		Enemy->Object.Pos = *Pos;
		Enemy->Object.Group = Group;
		Enemy->ModelNum = ModelNum;
		Enemy->Object.DirVector = *Dir;
//		Enemy->Object.UpVector = *Up;
		Enemy->TriggerMod = TriggerMod;
		Enemy->Object.Shield = Difficulty * EnemyTypes[EnemyType].Shield;
		Enemy->Object.NodeNetwork = Network;
		Enemy->Object.NearestNode = NULL;
		Enemy->PickupHeld = (u_int16_t) PickupHeld;

		Enemy->FirePosCount = 0;

		Enemy->NextTNode = NULL;
		Enemy->LastTNode = NULL;
		Enemy->ImInNodeTransition = false;
		Enemy->PickNewNodeNow = false;
		Enemy->SmokeTime = 0.0F;
		
		Enemy->SplineNode1 = NULL;
		Enemy->SplineNode2 = NULL;
		Enemy->SplineNode3 = NULL;
		Enemy->SplineNode4 = NULL;
		
		Enemy->Object.AI_Mode = EnemyTypes[EnemyType].AiMode;

		if( (FormationLink != (u_int16_t) -1) && (EnemyTypes[EnemyType].ControlType == ENEMY_CONTROLTYPE_FLY_AI) )
		{
			Enemy->FormationLink = &Enemies[FormationLink];
		}else{
			Enemy->FormationLink = NULL;
		}
		
		
		Enemy->Timer = 1.0F;
		
		Enemy->Viewcone = (float) COSD( EnemyTypes[EnemyType].Viewcone / 2 );
		
		Enemy->Object.Angle.x = 0.0F;
		Enemy->Object.Angle.y = 0.0F;
		Enemy->Object.Angle.z = 0.0F;
		Enemy->Object.Bank	 = 0.0F;
		Enemy->Object.Speed.x = 0.0F;
		Enemy->Object.Speed.y = 0.0F;
		Enemy->Object.Speed.z = 0.0F;
		Enemy->Object.Autolevel = 0.0F;

		Enemy->Object.FirstGun = NULL;
		Enemy->Object.HowManyGuns = 0;
		if( EnemyTypes[EnemyType].NumOfGuns )
		{
			InitGuns( (BYTE) EnemyTypes[EnemyType].NumOfGuns , &EnemyTypes[EnemyType].GunType[0] , &Enemy->Object );
		}

		// this sets a ground based enemy exactly 75.0F above the ground....
		if( Enemy->Object.ControlType == ENEMY_CONTROLTYPE_CRAWL_AI )
		{
			if( FindPointAboveGround( &Enemy->Object.Pos , Enemy->Object.Group , &TempPos , &TempGroup ) )
			{
				Enemy->Object.Pos = TempPos;
				Enemy->Object.Group = TempGroup;
			}
		}
	
		if( Enemy->Object.ControlType != ENEMY_CONTROLTYPE_TURRET_PLAYER &&
			Enemy->Object.ControlType != ENEMY_CONTROLTYPE_TURRET_AI )
			FindNearestNode( &Enemy->Object );
		
		QuatFromDirAndUp( Dir, Up, &Enemy->Object.Quat );

		MakeQuat( 0.0F, 0.0F, 0.0F, &Enemy->Object.RotationForce );
		
		QuatToMatrix( &Enemy->Object.Quat, &Enemy->Object.Mat );
		MatrixTranspose( &Enemy->Object.Mat, &Enemy->Object.InvMat );
		

		Enemy->TriggerMod = TriggerMod;
		if( TriggerMod == (u_int16_t) -1 ) Enemy->TriggerModPtr = NULL;
		else Enemy->TriggerModPtr = &TrigMods[ TriggerMod ];

		if( ModelNum != (u_int16_t) -1 )
		{
			ModelIndex = FindFreeModel();
	
			if( ModelIndex != (u_int16_t) -1 )
			{
				Models[ ModelIndex ].ModelNum = ( ModelNum );
				Models[ ModelIndex ].Type = MODTYPE_Field;
				Models[ ModelIndex ].Flags = MODFLAG_Nothing;

				if( ( Enemy->Status & ENEMY_STATUS_Enable ) ) Models[ ModelIndex ].Visible = true;
				else Models[ ModelIndex ].Visible = false;

				Models[ ModelIndex ].OwnerType = OWNER_ENEMY;
				Models[ ModelIndex ].Owner = Enemy->Index;
				Models[ ModelIndex ].Pos = *Pos;
				Models[ ModelIndex ].Dir = *Dir;
				Models[ ModelIndex ].Rot.x = 0.0F;
				Models[ ModelIndex ].Rot.y = 0.0F;
				Models[ ModelIndex ].Rot.z = 0.0F;
				Models[ ModelIndex ].Quat = Enemy->Object.Quat;
			   	Models[ ModelIndex ].Mat = Enemy->Object.Mat;
				Models[ ModelIndex ].InvMat = Enemy->Object.InvMat;

				Models[ ModelIndex ].Func = MODFUNC_Nothing;

				if( ModelNames[ Models[ ModelIndex ].ModelNum ].DoIMorph )
				{
					if( MxaModelHeaders[ Models[ ModelIndex ].ModelNum ].NumSpotFX &&
						MxaModelHeaders[ Models[ ModelIndex ].ModelNum ].SpotFX )
					{
						Models[ ModelIndex ].Func = MODFUNC_ProcessSpotFX;
						SetupModelSpotFX( ModelIndex );
					}
				}
				else
				{
					if( ModelHeaders[ Models[ ModelIndex ].ModelNum ].NumSpotFX &&
						ModelHeaders[ Models[ ModelIndex ].ModelNum ].SpotFX )
					{
						Models[ ModelIndex ].Func = MODFUNC_ProcessSpotFX;
						SetupModelSpotFX( ModelIndex );
					}
				}

				Models[ ModelIndex ].AxisRot = 0.0F;
				Models[ ModelIndex ].Scale = 1.0F;
				Models[ ModelIndex ].MaxScale = 1.0F;
				Models[ ModelIndex ].Group = Group;
				Models[ ModelIndex ].LifeCount = -1.0F;
			}
		}
		else
		{
			ModelIndex = (u_int16_t) -1;
		}

		Enemy->ModelIndex = ModelIndex;

		return( Enemy );
	
	}
	return( NULL );
}

/*===================================================================
	Procedure	:	Process all Enemies
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessEnemies( void )
{
	GUNOBJECT	*	GunObject;
	ENEMY		*	Enemy;
	ENEMY		*	NextEnemy;
	u_int16_t			Model;
	u_int16_t			OldGroup;
	VECTOR			OldPos;
	bool			OldComponentCollide;

#if ENABLEENEMYCOLLISIONS
	VECTOR			PushVector;
	float			Damage;
#endif
	EnemiesActive = 0;
	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			Model = Enemy->ModelIndex;
			OldGroup = Enemy->Object.Group;
			OldPos = Enemy->Object.Pos;

			Enemy->TauntPause -= framelag;
			if ( Enemy->TauntPause < 0.0F )
			{
				Enemy->TauntPause = ENEMY_TAUNT_PAUSE;
				PlayEnemyBikerTaunt( Enemy );
			}
			
			if( (Enemy->Object.ControlType != ENEMY_CONTROLTYPE_NONE) && 
				( (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_FLESHMORPH ) || (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_EXOGENON ) || (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_TURRET_AI ) || Enemy->Object.NearestNode ) )
			{
				GunObject = Enemy->Object.FirstGun;
				while( GunObject )
				{
					GunObject->AIMoveFlags = 0;
					GunObject->ReloadTime -= framelag;
					if( GunObject->ReloadTime < 0.0F )
						GunObject->ReloadTime = 0.0F;
					GunObject->BurstTime -= framelag;
					if( GunObject->BurstTime < 0.0F )
						GunObject->BurstTime = 0.0F;
					
					GetLastCompDispMatrix( &Enemy->Object, &GunObject->Mat, &GunObject->InvMat, &GunObject->FirePos , GunObject->GunNum);
					GunObject = GunObject->Next;
				}

				if( Enemy->IveBeenHitTimer )
				{
					Enemy->IveBeenHitTimer -= framelag;
					if( Enemy->IveBeenHitTimer < 0.0F )
						Enemy->IveBeenHitTimer = 0.0F;
				}

				Enemy->AI_Angle.x = 0.0F;
				Enemy->AI_Angle.y = 0.0F;
				Enemy->AI_Angle.z = 0.0F;
				OldComponentCollide = Enemy->CompCollision;
				Enemy->CompCollision = false;
				( * EnemyControlType[ Enemy->Object.ControlType ] )(Enemy);		//go off and do his thing...
				Enemy->CompCollision = OldComponentCollide;

				if( !(Enemy->Status & ENEMY_STATUS_Enable) )
					goto KilledInAI;
 		

				if( ( (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_FLY_AI) ||
					  (Enemy->Object.ControlType == ENEMY_CONTROLTYPE_SPLINE) ) &&
					  ( ( OldPos.x != Enemy->Object.Pos.x) || ( OldPos.y != Enemy->Object.Pos.y) || ( OldPos.z != Enemy->Object.Pos.z) ) )
					UpdateNearestNode( &Enemy->Object );
				
				if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
				{
					EnemiesActive++;
				
					GunObject = Enemy->Object.FirstGun;
					while( GunObject )
					{
						if( GunTypes[GunObject->Type].PreciseRotation )
						{
							CarryOutGUN_PreciseAIMovementCommands( GunObject );
							GunRotation( GunObject , false );
						}else{
							CarryOutGUN_AIMovementCommands( GunObject );
							GunRotation( GunObject , true );
						}
						SetTurretVector( &Enemy->Object, &GunObject->Dir , GunObject->GunNum );
						
						GunObject = GunObject->Next;
					}
				}
				
			}else{
				AutoDisplay( &Enemy->Object );
			}
			
			if( OldGroup != Enemy->Object.Group )
			{
				MoveEnemyToGroup( Enemy, OldGroup, Enemy->Object.Group );
			}


			
//			if( ( Ships[WhoIAm].Object.Group == (u_int16_t) -1 ) || VisibleOverlap( Ships[WhoIAm].Object.Group, Enemy->Object.Group , NULL ) )
			{
				UpdateObjectAmbientColour( &Enemy->Object );
  
				if( Model != (u_int16_t) -1 )
				{
					Models[ Model ].Pos = Enemy->Object.Pos;
					Models[ Model ].Mat = Enemy->Object.FinalMat;
					Models[ Model ].InvMat = Enemy->Object.FinalInvMat;
					Models[ Model ].Quat = Enemy->Object.FinalQuat;
					Models[ Model ].Group = Enemy->Object.Group;
					Models[ Model ].Red = (int)Enemy->Object.Red;
					Models[ Model ].Green = (int)Enemy->Object.Green;
					Models[ Model ].Blue = (int)Enemy->Object.Blue;

					Models[ Model ].Flags |= MODFLAG_AmbientLight;

					if( Enemy->Object.Flags & SHIP_Stealth ) Models[ Model ].Flags |= MODFLAG_Stealth;
					else Models[ Model ].Flags &= ~MODFLAG_Stealth;
				}
				
				if( Enemy->Object.Components )
				{
					if( Enemy->Object.CurAnimSeq != -1 && Enemy->Object.AnimSeqs )
					{
						if( Enemy->Object.AnimSeqs[ Enemy->Object.CurAnimSeq ].StartTime ==
							Enemy->Object.AnimSeqs[ Enemy->Object.CurAnimSeq ].EndTime )
						{
							Enemy->Object.Time = Enemy->Object.AnimSeqs[ Enemy->Object.CurAnimSeq ].StartTime;
							Enemy->Object.Animating = false;
						}
						else
						{
							Enemy->Object.Time += ( framelag * Enemy->Object.AnimSpeed );
							if( Enemy->Object.Time > Enemy->Object.AnimSeqs[ Enemy->Object.CurAnimSeq ].EndTime )
							{
								Enemy->Object.Time = Enemy->Object.AnimSeqs[ Enemy->Object.CurAnimSeq ].EndTime;
								Enemy->Object.Animating = false;
							}
						}
					}
					else
					{
						Enemy->Object.Animating = true;
						Enemy->Object.Time += ( framelag * Enemy->Object.AnimSpeed );
						if( Enemy->Object.Time > Enemy->Object.OverallTime )
						{
							Enemy->Object.Time = 0.0F;
						}
					}
					
					UpdateCompObjChildren( Enemy->Object.Components, 1, &Enemy->Object.FinalMat, &Enemy->Object.Pos, Enemy->Object.Time, Enemy->Object.Group, &Enemy->Object.Pos );
					UpdateCompObjColours( Enemy->Object.Components, 1, (int)Enemy->Object.Red, (int)Enemy->Object.Green, (int)Enemy->Object.Blue );

#if ENABLEENEMYCOLLISIONS
					if( CheckCompObjectCollision( &Ships[ WhoIAm ].Object.Pos, Enemy->Object.Components, &PushVector, &Damage, SHIP_RADIUS ) )
					{
						ForceExternalOneOff( WhoIAm, &PushVector );
					}
#endif
					if( ( Enemy->Type == ENEMY_Boss_FleshMorph ) || ( Enemy->Type == ENEMY_Boss_BigGeek ) || ( Enemy->Type == ENEMY_Boss_LittleGeek ) || ( Enemy->Type == ENEMY_Boss_Exogenon ) || ( Enemy->Type == ENEMY_Boss_Metatank ) )
						UpdateCompObjFlags( Enemy->Object.Components, 1, 0 );
					
					if( AmIInvulnerable( Enemy->Object.Components, 1, Enemy->Object.Time ) )
					{
						Enemy->Object.Flags |= SHIP_Invul;
					}
					else
					{
						Enemy->Object.Flags &= ~SHIP_Invul;
					}
				}
			}

//			DoNmeDamagedEffects( Enemy );
		}
		else
		{
			if( ( Enemy->Status & ENEMY_STATUS_Generating ) )
			{
				Enemy->Delay -= framelag;

				if( Enemy->Delay <= 0.0F )
				{
					if( Enemy->Object.Components )
					{
						SetCompObjModelsState( Enemy->Object.Components, 1, true );
					}

					if( Enemy->ModelIndex != (u_int16_t) -1 )
					{
						Models[ Enemy->ModelIndex ].Visible = true;
					}

					Enemy->Status &= ~ENEMY_STATUS_Generating;
					Enemy->Status |= ENEMY_STATUS_Enable;
				}
			}else{
				if( Enemy->GenerationDelay > 0.0F )
				{
					Enemy->GenerationDelay -= framelag;

					if( Enemy->GenerationDelay <= 0.0F )
					{
						Enemy->GenerationDelay = 0.0F;
						EnableEnemy(Enemy->Index);
					}
				}
			}
		}
KilledInAI:
		Enemy = NextEnemy;
	}
}

/*===================================================================
	Procedure	:	Display Untriggered NME's
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DispUntriggeredNMEs( void )
{
	ENEMY		*	Enemy;
	ENEMY		*	NextEnemy;

	if( !ShowUntriggeredNMEs ) return;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[ Enemy->Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] || outside_map )
			{
				DispInGameText( &Enemy->Object.Pos, EnemyNames[ Enemy->Type ] );

				if( framelag )
				{
					CreateSpotFXSteam( &Enemy->Object.Pos, &SlideUp, Enemy->Object.Group, 255, 255, 255 );
				}
			}
		}

		Enemy = NextEnemy;
	}
}

/*===================================================================
	Procedure	:	Set Current Anim Seq
	Input		:	int16_t	Seq Number
				:	OBJECT * Object
	Output		:	Nothing
===================================================================*/
void SetCurAnimSeq( int16_t Seq, OBJECT * Object )
{
	if( Seq != -1 && Object->AnimSeqs )
	{
		Object->Time = Object->AnimSeqs[ Seq ].StartTime;
		Object->CurAnimSeq = Seq;
		Object->Animating = true;
	}

}

/*===================================================================
	Procedure	:	Enable Enemy
	Input		:	u_int16_t	EnemyIndex
	Output		:	Nothing
===================================================================*/
void EnableEnemy( u_int16_t EnemyIndex )
{
	ENEMY	*	Enemy;
	VECTOR		TempVector;
	VECTOR		DirVector;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	u_int16_t		Int_Group;

	if( EnemyIndex < NumInitEnemies )
	{
		Enemy = &Enemies[ EnemyIndex];

		if( Enemy != NULL )
		{
			if( !Enemy->Alive || (Enemy->Status & ENEMY_STATUS_Enable) )
			{
				return;
			}
			if( Enemy->GenerationDelay )
				return;
			if( Enemy->MasterGenerationDelay )
			{
				Enemy->GenerationDelay = Enemy->MasterGenerationDelay;
				Enemy->MasterGenerationDelay = 0.0F;
				return;
			}
			DirVector.x = ( Enemy->Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
			DirVector.y = ( Enemy->Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
			DirVector.z = ( Enemy->Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );

			if( BackgroundCollide( &MCloadheadert0, &Mloadheader, &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group,
							   &DirVector, &Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) && !outside_map )
			{
				if( Enemy->Object.Components )
				{
					SetCompObjModelsState( Enemy->Object.Components, 1, true );
				}

				if( Enemy->ModelIndex != (u_int16_t) -1 )
				{
					Models[ Enemy->ModelIndex ].Visible = true;
				}

				Enemy->Status &= ~ENEMY_STATUS_Generating;
				Enemy->Status |= ENEMY_STATUS_Enable;
			}
			else
			{
				CreateNmeGenEffect( &Enemy->Object.Pos, Enemy->Object.Group );
				Enemy->Delay = ( 1.0F * ANIM_SECOND );
				Enemy->Status |= ENEMY_STATUS_Generating;
				Enemy->Object.Shield = Difficulty * EnemyTypes[Enemy->Type].Shield;
			}
			PlayPannedSfx( SFX_EnemySpawn, Enemy->Object.Group , &Enemy->Object.Pos, 0.0F );
			Enemy->TauntPause = ENEMY_TAUNT_PAUSE;
			PlayEnemyBikerTaunt( Enemy );
		}
	}
}

/*===================================================================
	Procedure	:	Disable Enemy
	Input		:	ENEMY	*	Enemy
	Output		:	Nothing
===================================================================*/
void DisableEnemy( ENEMY * Enemy )
{
	Enemy->Status &= ~ENEMY_STATUS_Enable;

	if( Enemy->TriggerModPtr != NULL )
	{
		ApplyTriggerMod( Enemy->TriggerModPtr );
	}
}

/*===================================================================
	Procedure	:	Check if hit Enemy
	Input		:	u_int16_t		OwnerType
				:	u_int16_t		Owner of weapon
				:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	VECTOR	*	UpDir
				:	float		Length of Ray
				:	VECTOR	*	Int_Point;
				:	VECTOR	*	Int_Point2;
				:	float	*	Dist to Int_Point
				:	float		Weapon Radius;
				:	u_int16_t		Colision Type
	Output		:	ENEMY	*	Enemy Address ( NULL If none hit )
===================================================================*/
ENEMY * CheckHitEnemy( u_int16_t OwnerType, u_int16_t Owner, VECTOR * Pos, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point,
						VECTOR * Int_Point2, float * Dist, float WeaponRadius, u_int16_t ColType )
{
	VECTOR		TempVector;
	VECTOR		Int_Temp;
	VECTOR		Int_Temp2;
	float		DistToSphere;
	ENEMY	*	ClosestEnemy;
	float		ClosestLength;
	float		Cos;
	float		ColRadius = 0.0f;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;

//	return( NULL );

	ClosestEnemy = NULL;
	ClosestLength = *Dist;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( ( OwnerType == OWNER_ENEMY ) && ( Owner == Enemy->Index ) ) )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) && ( Enemy->CompCollision == SPHERECOLLISION ) )
			{
				if( Enemy->Type == ENEMY_Boss_Avatar )
				{
					if( !IsAvatarActive() )
						goto SkipIt;
				}
			
				switch( ColType )
				{
					case COLTYPE_Trojax:
						TempVector.x = ( Enemy->Object.Pos.x - Pos->x );			// Bul - Enemy
						TempVector.y = ( Enemy->Object.Pos.y - Pos->y );
						TempVector.z = ( Enemy->Object.Pos.z - Pos->z );
						NormaliseVector( &TempVector );
						Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
						ColRadius = ( EnemyTypes[Enemy->Type].Radius + ( WeaponRadius * Cos ) );
						break;

					case COLTYPE_Transpulse:
						TempVector.x = ( Enemy->Object.Pos.x - Pos->x );			// Bul - Enemy
						TempVector.y = ( Enemy->Object.Pos.y - Pos->y );
						TempVector.z = ( Enemy->Object.Pos.z - Pos->z );
						NormaliseVector( &TempVector );
						Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
						Cos = (float) ( Cos * ( 1.0F - fabs( DotProduct( &TempVector, UpDir ) ) ) );
						ColRadius = ( EnemyTypes[Enemy->Type].Radius + ( WeaponRadius * Cos ) );
						break;

					case COLTYPE_Sphere:
						ColRadius = ( EnemyTypes[Enemy->Type].Radius + WeaponRadius );
						break;

					case COLTYPE_Point:
						ColRadius = EnemyTypes[Enemy->Type].Radius;
						break;
				}

/*===================================================================
	Does Ray hit target directly?
===================================================================*/
				if( RaytoSphere2( (VECTOR *) &Enemy->Object.Pos, EnemyTypes[Enemy->Type].Radius, Pos, Dir,
									&Int_Temp, &Int_Temp2 ) )
				{
					TempVector.x = ( Int_Temp.x - Pos->x );
					TempVector.y = ( Int_Temp.y - Pos->y );
					TempVector.z = ( Int_Temp.z - Pos->z );
					
					DistToSphere = VectorLength( &TempVector );
					
					if( ( DistToSphere <= DirLength ) && ( DistToSphere < ClosestLength ) )
					{
						ClosestLength = DistToSphere;
						ClosestEnemy = Enemy;
						*Int_Point = Int_Temp;
						*Int_Point2 = Int_Temp2;
					}
				}
				else
				{
/*===================================================================
	Is Starting pos inside sphere already?
===================================================================*/
					if( PointToSphere( (VERT *) &Enemy->Object.Pos, EnemyTypes[Enemy->Type].Radius, (VERT *) Pos ) )
					{
						ClosestLength = 0.0F;
						ClosestEnemy = Enemy;
						*Int_Point = *Pos;
						*Int_Point2 = *Pos;
					}
   					else
   					{
   						if( ColRadius > EnemyTypes[Enemy->Type].Radius )
   						{
/*===================================================================
	Do i graze sphere?
===================================================================*/
   							if( RaytoSphere2( (VECTOR *) &Enemy->Object.Pos, ColRadius, Pos, Dir,
   											  &Int_Temp, &Int_Temp2 ) )
   							{
   								TempVector.x = ( Int_Temp.x - Pos->x );
   								TempVector.y = ( Int_Temp.y - Pos->y );
   								TempVector.z = ( Int_Temp.z - Pos->z );
   						
   								DistToSphere = VectorLength( &TempVector );
   						
   								if( ( DistToSphere <= DirLength ) && ( DistToSphere < ClosestLength ) )
   								{
   									ClosestLength = DistToSphere;
   									ClosestEnemy = Enemy;
   									*Int_Point = Int_Temp;
   									*Int_Point2 = Int_Temp2;
   								}
   							}
   							else
   							{
   								if( PointToSphere( (VERT *) &Enemy->Object.Pos, ColRadius, (VERT *) Pos ) )
   								{
   									ClosestLength = 0.0F;
   									ClosestEnemy = Enemy;
   									*Int_Point = *Pos;
   									*Int_Point2 = *Pos;
   								}
   							}
   						}
   					}
				}
			}
		}
SkipIt:
		Enemy = NextEnemy;
	}

	if( ClosestEnemy != NULL ) *Dist = ClosestLength;

	return( ClosestEnemy );
}



/*===================================================================
	Procedure	:	Flying Enemy Under Player Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyFlyUnderPlayerControl( ENEMY * Enemy )
{
	Enemy->Object.Angle.x += control.pitch;
	Enemy->Object.Angle.y += control.yaw;
	Enemy->Object.Angle.z += control.roll;
	
	Enemy->Object.Bank	 += control.bank;

	Enemy->Object.Speed.x += control.right;
	Enemy->Object.Speed.y += control.up;
	Enemy->Object.Speed.z += control.forward;
	AutoMovement( &Enemy->Object , Enemy , true);
}
/*===================================================================
	Procedure	:	Flying Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyFlyUnderAiControl( ENEMY * Enemy )
{
	Enemy->AIMoveFlags &= (AI_CONTROL_NO_FORWARD +AI_CONTROL_NO_BACK +AI_CONTROL_NO_LEFT +AI_CONTROL_NO_RIGHT +AI_CONTROL_NO_UP +AI_CONTROL_NO_DOWN+AI_CONTROL_COLLISION );
	
	
	// wooooh shit .......
	if( Enemy->Type == ENEMY_Boss_Avatar )
	{
		if( IsAvatarActive() )
			AI_AIR( Enemy );
	}else{
		AI_AIR( Enemy );
	}

	if( ( Enemy->AIFlags & AI_ANYPLAYERINRANGE ) || (Enemy->Type == ENEMY_Boss_Avatar) )
	{

		if( Enemy->AIFlags & AI_PLAYERISTARGETINGME )
			TestEnemy = Enemy;
		if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_PRECISECONTROL )
		{
			CarryOutPreciseAIMovementCommands( Enemy );
			AutoMovement( &Enemy->Object , Enemy ,false );
		}else{
			CarryOutAIMovementCommands( Enemy );
			AutoMovement( &Enemy->Object , Enemy ,true);
		}
	}
				
}
/*===================================================================
	Procedure	:	Crawling Enemy Under Player Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyCrawlUnderPlayerControl( ENEMY * Enemy )
{
	SHIPCONTROL ctrl;


	ctrl.yaw		= 0.0F;
	ctrl.forward	= 0.0F;

	Enemy->Object.Angle.y += ctrl.yaw;
//	Enemy->Object.Speed.z += ctrl.forward * Enemy->Object.WheelsOnGround;

//	if( Enemy->Object.WheelsOnGround > 0.25F )
//	{
//		Enemy->Object.ExternalForce.y = -1.0F;
//	}
	Enemy->Object.ExternalForce.y -= 0.30F * framelag;

	AutoMovementCrawl( &Enemy->Object , Enemy );
}
/*===================================================================
	Procedure	:	Crawling Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyCrawlUnderAiControl( ENEMY * Enemy )
{
	float Turn;
	float Move = 0.0f;
	Enemy->AIMoveFlags = 0;

	// wooooh shit .......
	AI_CRAWL( Enemy );

	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		
		if( Enemy->AIMoveFlags )
		{
			Turn = EnemyTypes[Enemy->Type].TurnRateAccell * EnemyTypes[Enemy->Type].MaxTurnRate * framelag;
			Move = EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate * framelag;
		}
		
		if ( Enemy->AIMoveFlags&AI_CONTROL_TURNLEFT)
		{
			if( Enemy->AI_Angle.y < (Enemy->Object.Angle.y*framelag) )
				Enemy->Object.Angle.y -= Turn;
		}
		if ( Enemy->AIMoveFlags&AI_CONTROL_TURNRIGHT )
		{
			if( Enemy->AI_Angle.y > (Enemy->Object.Angle.y*framelag) )
				Enemy->Object.Angle.y += Turn;
		}
		if ( Enemy->AIMoveFlags&AI_CONTROL_FORWARD )
			Enemy->Object.Speed.z += Move;
		if ( Enemy->AIMoveFlags&AI_CONTROL_BACK )
			Enemy->Object.Speed.z -= Move;
		
		AutoMovementCrawl( &Enemy->Object , Enemy );
	}
}

/*===================================================================
	Procedure	:	Turret Enemy Under Player Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyTurretUnderPlayerControl( ENEMY * Enemy )
{
}
/*===================================================================
	Procedure	:	Turret Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyTurretUnderAiControl( ENEMY * Enemy )
{
	Enemy->AIMoveFlags = 0;
	// wooooh shit .......
	AI_TURRET( Enemy );
	AutoDisplay( &Enemy->Object );
}

/*===================================================================
	Procedure	:	Enemy Under Spline Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyUnderSplineControl( ENEMY * Enemy )
{
	OBJECT * Object;
	VECTOR	StartPos;
	u_int16_t	OldGroup;
	Object = &Enemy->Object;

	StartPos = Object->Pos;
	OldGroup = Object->Group;

	if( SplineAutoMovement( Object , Enemy ) )
	{
		Enemy->Timer = 0.0F;
		Enemy->TempSplineNode.Pos = Enemy->Object.Pos;
		Enemy->SplineNode1 = (void*) &Enemy->TempSplineNode;
		Enemy->SplineNode2 = (void*) &Enemy->TempSplineNode;
	}else{
		AI_SPLINE( Enemy );
	}


	TriggerAreaEnemyCheck( &StartPos , &Object->Pos , Object->Group);
	if( OldGroup != Object->Group )
	{
		TriggerAreaEnemyCheck( &StartPos , &Object->Pos , OldGroup);
	}

}


/*===================================================================
	Procedure	:	Exogenon Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyUnderExogenonControl( ENEMY * Enemy )
{
	float Turn;
	Enemy->AIMoveFlags = 0;
	// wooooh shit .......
	AI_EXOGENON( Enemy );
	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		
		if( Enemy->AIMoveFlags )
		{
			Turn = EnemyTypes[Enemy->Type].TurnRateAccell * EnemyTypes[Enemy->Type].MaxTurnRate * framelag;
			if( Enemy->Object.AI_Mode == AIMODE_EXOGENON_SWEEP )
				Turn *= 0.5F;

		}
		
		if ( Enemy->AIMoveFlags&AI_CONTROL_TURNLEFT)
		{
			if( Enemy->AI_Angle.y < (Enemy->Object.Angle.y*framelag) )
				Enemy->Object.Angle.y -= Turn;
		}
		if ( Enemy->AIMoveFlags&AI_CONTROL_TURNRIGHT )
		{
			if( Enemy->AI_Angle.y > (Enemy->Object.Angle.y*framelag) )
				Enemy->Object.Angle.y += Turn;
		}
		AutoMovementExogenon( &Enemy->Object , Enemy );
	}

}
/*===================================================================
	Procedure	:	FleshMorph Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyUnderFleshMorphControl( ENEMY * Enemy )
{
	float Move;
	Enemy->AIMoveFlags = 0;

	// wooooh shit .......
	AI_FLESHMORPH( Enemy );

	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		Move = EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate * framelag;
		if ( Enemy->AIMoveFlags&AI_CONTROL_FORWARD )
			Enemy->Object.Speed.z += Move;
		if ( Enemy->AIMoveFlags&AI_CONTROL_BACK )
			Enemy->Object.Speed.z -= Move;
		AutoMovementFleshmorph( &Enemy->Object , Enemy );
	}
}
/*===================================================================
	Procedure	:	LittleGeek Enemy Under Ai Control...
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void EnemyUnderLittleGeekControl( ENEMY * Enemy )
{
	Enemy->AIMoveFlags &= (AI_CONTROL_NO_FORWARD +AI_CONTROL_NO_BACK +AI_CONTROL_NO_LEFT +AI_CONTROL_NO_RIGHT +AI_CONTROL_NO_UP +AI_CONTROL_NO_DOWN+AI_CONTROL_COLLISION );
	
	// wooooh shit .......
	AI_LITTLEGEEK( Enemy );

	if( Enemy->AIFlags & AI_ANYPLAYERINRANGE )
	{
		if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_PRECISECONTROL )
		{
			CarryOutPreciseAIMovementCommands( Enemy );
			AutoMovement( &Enemy->Object , Enemy ,false );
		}else{
			CarryOutAIMovementCommands( Enemy );
			AutoMovement( &Enemy->Object , Enemy ,true);
		}
	}
}


/*===================================================================
	Procedure	:	Carry out all movement / collision/ turning...
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
void AutoMovement( OBJECT * Object , ENEMY * Enemy , bool AngleDecellBefore )
{
	VECTOR	ImpactPoint;
	float	Speed;
	VECTOR	Move_Off;	
	VECTOR	Move_Dir;
	QUAT	StepQuat;	
	u_int16_t	ImpactGroup;
	VECTOR	Bob;
	BGOBJECT * BGObject;
	VECTOR	ExtForce;
	bool	HasBeenExternal;
	VECTOR	StartPos;
	u_int16_t	OldGroup;
	float	ShieldModifier;


	StartPos = Object->Pos;
	OldGroup = Object->Group;

	if( AngleDecellBefore )
	{
		AccellDecell( &Object->Angle.x , EnemyTypes[Enemy->Type].TurnRateDecell );
		AccellDecell( &Object->Angle.y , EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F);
	}
	AccellDecell( &Object->Angle.z , RollDecell );
	Object->Autolevel = ObjectAutoLevelRot( Object , 0.05F * framelag );
	
	AccellDecell( &Object->Bank , BankDecell );

	AccellDecell( &Object->Speed.x , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->Speed.y , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->Speed.z , EnemyTypes[Enemy->Type].MoveRateDecell );

	if( AngleDecellBefore )
	{
		MakeQuat( Object->Angle.x * framelag, Object->Angle.y * framelag, Object->Angle.z * framelag + Object->Autolevel, &StepQuat );
	}else{
		MakeQuat( Object->Angle.x , Object->Angle.y , Object->Angle.z * framelag + Object->Autolevel, &StepQuat );
	}
	QuatMultiply(  &Object->Quat , &StepQuat , &Object->Quat );

#if 1
	QuatMultiply(  &Object->RotationForce , &Object->Quat , &Object->Quat );
	MakeQuat( 0.0F, 0.0F, 0.0F, &Enemy->Object.RotationForce );
#else
	if( Object->RotationForce.w >= 1.0F )
		Object->RotationForce.w = 1.0F;
	else{
		Object->RotationForce.w *= 1.0F + ( EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F * framelag );
		if( Object->RotationForce.w > 1.0F )
			Object->RotationForce.w = 1.0F;
	}
	AccellDecell( &Object->RotationForce.x , EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F);
	AccellDecell( &Object->RotationForce.y , EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F);
	AccellDecell( &Object->RotationForce.z , EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F);
	QuatMultiply(  &Object->RotationForce , &Object->Quat , &Object->Quat );
#endif

	QuatToMatrix( &Object->Quat, &Object->Mat );

	MatrixTranspose( &Object->Mat, &Object->InvMat );
	
	// carry out movements
	Move_Off.x = 0.0F;
	Move_Off.y = 0.0F;
	Move_Off.z = 0.0F;

	if( !EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_NOBOB )
	{
		ObjectBob( Object , &Bob );
		Move_Off.x += Bob.x - Object->LastBob.x;
		Move_Off.y += Bob.y - Object->LastBob.y;
		Move_Off.z += Bob.z - Object->LastBob.z;
		Object->LastBob = Bob;
	}
	
	if ( Object->Speed.x )
	{
		Speed = Object->Speed.x * framelag;
		ApplyMatrix( &Object->Mat, &SlideRight, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}
	if ( Object->Speed.y )
	{
		Speed = Object->Speed.y * framelag;
		ApplyMatrix( &Object->Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}
	if ( Object->Speed.z )
	{
		Speed = Object->Speed.z * framelag;
		ApplyMatrix( &Object->Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}
	
	// apply external and internal force vectors to ship movement
	AccellDecell( &Object->ExternalForce.x , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->ExternalForce.y , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->ExternalForce.z , EnemyTypes[Enemy->Type].MoveRateDecell );

	Move_Off.x += Object->ExternalForce.x * framelag;
	Move_Off.y += Object->ExternalForce.y * framelag;
	Move_Off.z += Object->ExternalForce.z * framelag;
	
	// Collide with other enemies....
	Enemy2EnemyCollide( Enemy , &Move_Off );
	
	// do collisions to background
	
	if( VectorLength( &Move_Off ) <= EnemyTypes[Enemy->Type].MoveRateAccell )
	{
		Move_Off.x = 0.0F; 
		Move_Off.y = 0.0F; 
		Move_Off.z = 0.0F;
		ImpactPoint = Object->Pos;
		ImpactGroup = Object->Group;
		
	}else{
	
		// Clear out the restric movement flags....
		Enemy->AIMoveRestrictClearTime -= framelag;
		if( Enemy->AIMoveRestrictClearTime < 0 )
		{
			Enemy->AIMoveFlags &= ~(AI_CONTROL_NO_FORWARD +AI_CONTROL_NO_BACK +AI_CONTROL_NO_LEFT +AI_CONTROL_NO_RIGHT +AI_CONTROL_NO_UP +AI_CONTROL_NO_DOWN );
			Enemy->AIMoveRestrictClearTime = EnemyTypes[Enemy->Type].MoveRestrictClearTime;
		}

		
		if ( Object->Group != (u_int16_t) -1 )
		{
			BGObject = NULL;
			if( ObjectCollide( Object, &Move_Off, EnemyTypes[Enemy->Type].Radius, &BGObject ) )
			{
				Enemy->AIMoveFlags |= AI_CONTROL_COLLISION;
				RestrictMovement( Enemy , &Move_Off );
			}
			if( BGObject ) ChangeBGState( BGObject, OWNER_ENEMY, Enemy->Index, BUMP, 0.0F );
		}else{
				Object->Pos.x += Move_Off.x;
				Object->Pos.y += Move_Off.y;
				Object->Pos.z += Move_Off.z;
		}

		TriggerAreaEnemyCheck( &StartPos , &Object->Pos , Object->Group);
		if( OldGroup != Object->Group )
		{
			TriggerAreaEnemyCheck( &StartPos , &Object->Pos , OldGroup);
		}
		
		ExtForce.x = 0.0F;
		ExtForce.y = 0.0F;
		ExtForce.z = 0.0F;
		HasBeenExternal = ExternalForcesAreaCheck( &Object->Pos , &StartPos , Object->Group, &ExtForce , &ShieldModifier);
		if( HasBeenExternal )
		{
			ObjectForceExternalOneOff( Object, &ExtForce );
		}

		if( TeleportsAreaCheck( &Object->Pos , &StartPos , OldGroup, Object ) )
		{
			FindNearestNode( Object );
			Enemy->TNode = NULL;
			Enemy->TShip = NULL;
		}
	}
/*===================================================================컴컴
			Set the Banking Matrix
/*===================================================================컴컴*/
	MakeQuat( 0.0F , 0.0F , Object->Bank, &StepQuat );
	QuatMultiply(  &Object->Quat , &StepQuat , &Object->FinalQuat );
	QuatToMatrix( &Object->FinalQuat, &Object->FinalMat );
	MatrixTranspose( &Object->FinalMat, &Object->FinalInvMat );

	if( !AngleDecellBefore )
	{
		Object->Angle.x = 0.0F;
		Object->Angle.y = 0.0F;
	}
}
/*===================================================================
	Procedure	:	Carry out all movement / collision/ turning for a Crawling Enemy...
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
void AutoMovementCrawl( OBJECT * Object , ENEMY * Enemy )
{
	VECTOR	Move_Off = { 0.0F , 0.0F , 0.0F };	
	MATRIX	StepMat;	
	VECTOR	StartPos;
	u_int16_t	OldGroup;
	NODE	* TNode;
	NODE	* NextTNode;
	NODE	* LastTNode;
	float	length = 75.0F;
	VECTOR	AimPos;
	VECTOR	TempPos;
	VECTOR	AnglePos;
	float	Distance;
	VECTOR	Up;
	float	XRot = 0.0F;

	StartPos = Object->Pos;
	OldGroup = Object->Group;

	AccellDecell( &Object->Speed.z , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->Angle.y , EnemyTypes[Enemy->Type].TurnRateDecell );

	if( Enemy->TNode )
	{
		TNode = (NODE*) Enemy->TNode;
		LastTNode = (NODE*) Enemy->LastTNode;
		NextTNode = (NODE*) Enemy->NextTNode;

		Move_Off.x = TNode->SolidPos.x - Object->Pos.x;
		Move_Off.y = TNode->SolidPos.y - Object->Pos.y;
		Move_Off.z = TNode->SolidPos.z - Object->Pos.z;
		NormaliseVector( &Move_Off );


		if( DistanceVector2Vector( &Object->Pos , &TNode->SolidPos ) < length )
		{
			
			// We are in transition...
			if( NextTNode && ( NextTNode != Enemy->TNode ) )
			{
				if( NextTNode->SolidPos.y > TNode->SolidPos.y && ( (NextTNode->SolidPos.y - TNode->SolidPos.y) > 32.0F ) )
				{
					// we will be going up a slope....
					Move_Off.x = NextTNode->SolidPos.x - TNode->SolidPos.x;
					Move_Off.y = NextTNode->SolidPos.y - TNode->SolidPos.y;
					Move_Off.z = NextTNode->SolidPos.z - TNode->SolidPos.z;
					NormaliseVector( &Move_Off );

					AimPos.x = TNode->SolidPos.x + ( Move_Off.x * length );
					AimPos.y = TNode->SolidPos.y + ( Move_Off.y * length );
					AimPos.z = TNode->SolidPos.z + ( Move_Off.z * length );

					Distance = DistanceVector2Vector( &Object->Pos , &AimPos );


					if(	!Enemy->ImInNodeTransition )
					{
						Enemy->DistanceToPointOnSlope = Distance;
					}
					
					Distance = 1.0F - (Distance / Enemy->DistanceToPointOnSlope);

					if( Distance > 0.0F && Distance < 1.0F )
					{
						AnglePos.x = TNode->SolidPos.x + ( ( (length*16.0F) * Distance) * Move_Off.x );
						AnglePos.y = TNode->SolidPos.y + ( ( (length*16.0F) * Distance) * Move_Off.y );
						AnglePos.z = TNode->SolidPos.z + ( ( (length*16.0F) * Distance) * Move_Off.z );
						ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &Up );
						MakeViewMatrix(&Enemy->Object.Pos, &AnglePos, &Up, &Enemy->Object.Mat);
					}
					
					Move_Off.x = AimPos.x - Object->Pos.x;
					Move_Off.y = AimPos.y - Object->Pos.y;
					Move_Off.z = AimPos.z - Object->Pos.z;
					NormaliseVector( &Move_Off );

					AimPos.x = ( TNode->SolidPos.x - Object->Pos.x );
					AimPos.y = ( TNode->SolidPos.y - Object->Pos.y );
					AimPos.z = ( TNode->SolidPos.z - Object->Pos.z );

				}else if( LastTNode &&  (LastTNode->SolidPos.y > TNode->SolidPos.y && ( (LastTNode->SolidPos.y - TNode->SolidPos.y) > 32.0F ) ))
				{
					// we will be down a slope....
					Move_Off.x = NextTNode->SolidPos.x - TNode->SolidPos.x;
					Move_Off.y = NextTNode->SolidPos.y - TNode->SolidPos.y;
					Move_Off.z = NextTNode->SolidPos.z - TNode->SolidPos.z;
					NormaliseVector( &Move_Off );

					AimPos.x = TNode->SolidPos.x + ( Move_Off.x * length );
					AimPos.y = TNode->SolidPos.y + ( Move_Off.y * length );
					AimPos.z = TNode->SolidPos.z + ( Move_Off.z * length );

					Distance = DistanceVector2Vector( &Object->Pos , &AimPos );

					if(	!Enemy->ImInNodeTransition )
					{
						Enemy->DistanceToPointOnSlope = Distance;
					}
					
					Distance = 1.0F - (Distance / Enemy->DistanceToPointOnSlope);

					if( Distance > 0.0F && Distance < 1.0F )
					{
						AnglePos.x = TNode->SolidPos.x + ( ( (length*16.0F) * Distance) * Move_Off.x );
						AnglePos.y = TNode->SolidPos.y + ( ( (length*16.0F) * Distance) * Move_Off.y );
						AnglePos.z = TNode->SolidPos.z + ( ( (length*16.0F) * Distance) * Move_Off.z );
						ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &Up );
						MakeViewMatrix(&Enemy->Object.Pos, &AnglePos, &Up, &Enemy->Object.Mat);
					}
					
					Move_Off.x = AimPos.x - Object->Pos.x;
					Move_Off.y = AimPos.y - Object->Pos.y;
					Move_Off.z = AimPos.z - Object->Pos.z;
					NormaliseVector( &Move_Off );

					AimPos.x = ( TNode->SolidPos.x - Object->Pos.x );
					AimPos.y = ( TNode->SolidPos.y - Object->Pos.y );
					AimPos.z = ( TNode->SolidPos.z - Object->Pos.z );


				}else{
					// its Level...
					AimPos.x = ( TNode->SolidPos.x - Object->Pos.x );
					AimPos.y = ( TNode->SolidPos.y - Object->Pos.y );
					AimPos.z = ( TNode->SolidPos.z - Object->Pos.z );

					ApplyMatrix( &Object->InvMat, &AimPos, &TempPos );
					if( TempPos.z >= 0.0F )
					{
						if( TNode && LastTNode )
						{
							AI_AimAtTarget( &Enemy->Object.InvMat , &LastTNode->SolidPos, &TNode->SolidPos);
							if( AimData.Angle.y > -3.0F && AimData.Angle.y < 3.0F )
							{
								XRot = (AimData.Angle.x * (0.1F * framelag));
							}
						}
					}
					Object->Angle.z = 0.0F;
					Object->Autolevel = ObjectAutoLevelRot( Object , 0.05F * framelag );
					
					BuildRotMatrix( 0.0F, 0.0F, Object->Autolevel, &StepMat );
					MatrixMultiply( &Object->Mat , &StepMat , &Object->Mat );
				}
			 	Enemy->ImInNodeTransition = true;
			}else{
				Enemy->PickNewNodeNow = true;
			}


		}else{
			if( TNode && LastTNode )
			{
				AI_AimAtTarget( &Enemy->Object.InvMat , &LastTNode->SolidPos, &TNode->SolidPos);
				if( AimData.Angle.y > -3.0F && AimData.Angle.y < 3.0F )
				{
					XRot = (AimData.Angle.x * (0.1F * framelag));
				}
			}
			Enemy->ImInNodeTransition = false;
		}

		Move_Off.x = Move_Off.x * Object->Speed.z * framelag;
		Move_Off.y = Move_Off.y * Object->Speed.z * framelag;
		Move_Off.z = Move_Off.z * Object->Speed.z * framelag;

		Object->Pos.x += Move_Off.x;
		Object->Pos.y += Move_Off.y;
		Object->Pos.z += Move_Off.z;
		
		BuildRotMatrix( XRot , -(Object->Angle.y * framelag), 0.0F, &StepMat );
		MatrixMultiply( &Object->Mat , &StepMat , &Object->Mat );
		Object->Group = MoveGroup( &Mloadheader, &StartPos, OldGroup, &Move_Off );

		
		if(Enemy->ImInNodeTransition && (TNode = (NODE*) Enemy->TNode) )
		{
			ApplyMatrix( &Object->InvMat, &AimPos, &TempPos );
			if( TempPos.z <= 0.0F )
			{
				Enemy->PickNewNodeNow = true;
			}
		}

	}
	
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;

	TriggerAreaEnemyCheck( &StartPos , &Object->Pos , Object->Group);
	if( OldGroup != Object->Group )
	{
		TriggerAreaEnemyCheck( &StartPos , &Object->Pos , OldGroup);
	}


	if( Enemy->AIMoveRestrictClearTime )
	{
		Enemy->AIMoveRestrictClearTime -= framelag;
		if( Enemy->AIMoveRestrictClearTime < 0.0F )
			Enemy->AIMoveRestrictClearTime = 0.0F;
	}else{

		if( Enemy2EnemyCollideSpecial( Enemy , &StartPos) )
		{

			TNode = Enemy->TNode;
			LastTNode = Enemy->LastTNode;
			NextTNode = Enemy->NextTNode;
			Enemy->TNode = LastTNode;
			Enemy->LastTNode = TNode;
			Enemy->NextTNode = NULL;
			Enemy->AIMoveRestrictClearTime = 30.0F + Random_Range_Float (30.0F) ;
		}
	}
}



/*===================================================================
	Procedure	:	Carry out all movement / collision/ turning for a Exogenon Enemy...
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
void AutoMovementExogenon( OBJECT * Object , ENEMY * Enemy )
{
	MATRIX	StepMat;	

	AccellDecell( &Object->Angle.y , EnemyTypes[Enemy->Type].TurnRateDecell );
	BuildRotMatrix( 0.0F , -(Object->Angle.y * framelag), 0.0F, &StepMat );
	MatrixMultiply( &Object->Mat , &StepMat , &Object->Mat );
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;
}




/*===================================================================
	Procedure	:	Set a wheel position..checks if group changes...
	Output		:	VECTOR * DestPos
				:	VECTOR * SourcePos
				:	float xoff...width*0.5
				:	float yoff...length*0.5
				:	VECTOR * Right
				:	VECTOR * Forward
				:	u_int16_t	startgroup
				:	u_int16_t * DestGroup
	Output		:	Nothing
===================================================================*/
__inline
void SetWheelPos( VECTOR * DestPos , VECTOR * SourcePos , float xoff , float zoff , VECTOR * Right, VECTOR * Forward, u_int16_t Group , u_int16_t * DestGroup )
{
	VECTOR	MoveOffset;
	MoveOffset.x = ( xoff*Right->x ) + ( zoff*Forward->x );
	MoveOffset.y = ( xoff*Right->y ) + ( zoff*Forward->y );
	MoveOffset.z = ( xoff*Right->z ) + ( zoff*Forward->z );

	DestPos->x = SourcePos->x + MoveOffset.x;
	DestPos->y = SourcePos->y + MoveOffset.y;
	DestPos->z = SourcePos->z + MoveOffset.z;
	*DestGroup = MoveGroup( &Mloadheader, SourcePos, Group, &MoveOffset );
}

/*===================================================================
	Procedure	:	Carry out all updating of quats/mats..
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
__inline void AutoDisplay( OBJECT * Object )
{
	QuatToMatrix( &Object->Quat, &Object->Mat );
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalQuat = Object->Quat;
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;
}
/*===================================================================
	Procedure	:	Carry out all updating of quats/mats..
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
__inline
void AutoDisplayMatrix( OBJECT * Object )
{
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalQuat = Object->Quat;
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;
}




/*===================================================================
	Procedure	:	Carry out the movement required by AI....
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void CarryOutAIMovementCommands( ENEMY * Enemy )
{
	float Turn = 0.0f;
	float Move = 0.0f;

	if( Enemy->AIMoveFlags )
	{
		Turn = EnemyTypes[Enemy->Type].TurnRateAccell * EnemyTypes[Enemy->Type].MaxTurnRate * framelag;
		Move = EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate * framelag;
	}


	if ( Enemy->AIMoveFlags&AI_CONTROL_TURNLEFT)
	{
		if( Enemy->AI_Angle.y < (Enemy->Object.Angle.y*framelag) )
		{
			Enemy->Object.Angle.y -= Turn;
			Enemy->Object.Bank += BankAccell * MaxBankAngle * framelag;
		}
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_TURNRIGHT )
	{
		if( Enemy->AI_Angle.y > (Enemy->Object.Angle.y*framelag) )
		{
			Enemy->Object.Angle.y += Turn;
			Enemy->Object.Bank -= BankAccell * MaxBankAngle * framelag;
		}
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_TURNDOWN )
	{
		if( Enemy->AI_Angle.x > (Enemy->Object.Angle.x*framelag) )
		{
			Enemy->Object.Angle.x -= Turn * -1.0F;
		}
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_TURNUP )
	{
		if( Enemy->AI_Angle.x < (Enemy->Object.Angle.x*framelag) )
		{
			Enemy->Object.Angle.x += Turn * -1.0F;
		}
	}

	if ( Enemy->AIMoveFlags&AI_CONTROL_LEFT )
	{
		Enemy->Object.Speed.x -= Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_RIGHT )
	{
		Enemy->Object.Speed.x += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_DOWN )
	{
		Enemy->Object.Speed.y -= Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_UP )
	{
		Enemy->Object.Speed.y += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_FORWARD )
	{
		Enemy->Object.Speed.z += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_BACK )
	{
		Enemy->Object.Speed.z -= Move;
	}
}
/*===================================================================
	Procedure	:	Carry out the movement required by AI....
	Output		:	ENEMY	*	Enemy 
	Output		:	Nothing
===================================================================*/
void CarryOutPreciseAIMovementCommands( ENEMY * Enemy )
{
	float Move = 0.0f;

	if( Enemy->AIMoveFlags )
	{
		Move = EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate * framelag;
	}


	if( Enemy->AI_Angle.x < 0.0F )
	{
		if( Enemy->AI_Angle.x < -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
		{
			Enemy->AI_Angle.x = -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F);
		}
	}else{
		if( Enemy->AI_Angle.x > 0.0F )
		{
			
			if( Enemy->AI_Angle.x > (EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
			{
				Enemy->AI_Angle.x = EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F;
			}
		}
	}
	Enemy->Object.Angle.x = Enemy->AI_Angle.x;
	if( Enemy->AI_Angle.y < 0.0F )
	{
		if( Enemy->AI_Angle.y < -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
		{
			Enemy->AI_Angle.y = -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F);
		}
	}else{
		if( Enemy->AI_Angle.y > 0.0F )
		{
			
			if( Enemy->AI_Angle.y > (EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
			{
				Enemy->AI_Angle.y = EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F;
			}
		}
	}
	Enemy->Object.Angle.y = Enemy->AI_Angle.y;


	if ( Enemy->AIMoveFlags&AI_CONTROL_LEFT )
	{
		Enemy->Object.Speed.x -= Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_RIGHT )
	{
		Enemy->Object.Speed.x += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_DOWN )
	{
		Enemy->Object.Speed.y -= Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_UP )
	{
		Enemy->Object.Speed.y += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_FORWARD )
	{
		Enemy->Object.Speed.z += Move;
	}
	if ( Enemy->AIMoveFlags&AI_CONTROL_BACK )
	{
		Enemy->Object.Speed.z -= Move;
	}
}


/*===================================================================
	Procedure	:	Carry out the movement required by AI....For A GUN
	Output		:	GUNOBJECT * GObject..
	Output		:	Nothing
===================================================================*/
void CarryOutGUN_AIMovementCommands( GUNOBJECT * GObject )
{
	float Turn = 0.0f;

	if( GObject->AIMoveFlags )
	{
		Turn = GunTypes[GObject->Type].gtTurnAccell * GunTypes[GObject->Type].gtMaxTurnSpeed * framelag;
	}
	if ( GObject->AIMoveFlags&AI_CONTROL_TURNLEFT)
	{
		if( GObject->AI_Angle.y < (GObject->Angle.y*framelag) )
		{
			GObject->Angle.y -= Turn;
		}
	}
	if ( GObject->AIMoveFlags&AI_CONTROL_TURNRIGHT )
	{
		if( GObject->AI_Angle.y > (GObject->Angle.y*framelag) )
		{
			GObject->Angle.y += Turn;
		}
	}
	if ( GObject->AIMoveFlags&AI_CONTROL_TURNDOWN )
	{
		if( GObject->AI_Angle.x > (GObject->Angle.x*framelag) )
		{
			GObject->Angle.x -= Turn * -1.0F;
		}
	}
	if ( GObject->AIMoveFlags&AI_CONTROL_TURNUP )
	{
		if( GObject->AI_Angle.x < (GObject->Angle.x*framelag) )
		{
			GObject->Angle.x += Turn * -1.0F;
		}
	}

}
/*===================================================================
	Procedure	:	Carry out the movement required by AI....For A GUN
	Output		:	GUNOBJECT * GObject..
	Output		:	Nothing
===================================================================*/
void CarryOutGUN_PreciseAIMovementCommands( GUNOBJECT * GObject )
{
	float Turn;

	Turn = GunTypes[GObject->Type].gtMaxTurnSpeed * framelag * 0.5F;
	
	if( GObject->AI_Angle.x < 0.0F )
	{
		if( GObject->AI_Angle.x < -Turn )
		{
			GObject->AI_Angle.x = -Turn;
		}
	}else{
		if( GObject->AI_Angle.x > 0.0F )
		{
			
			if( GObject->AI_Angle.x > Turn )
			{
				GObject->AI_Angle.x = Turn;
			}
		}
	}
	if( GObject->AI_Angle.y < 0.0F )
	{
		if( GObject->AI_Angle.y < -Turn )
		{
			GObject->AI_Angle.y = -Turn;
		}
	}else{
		if( GObject->AI_Angle.y > 0.0F )
		{
			
			if( GObject->AI_Angle.y > Turn )
			{
				GObject->AI_Angle.y = Turn;
			}
		}
	}
	GObject->Angle.x = GObject->AI_Angle.x;
	GObject->Angle.y = GObject->AI_Angle.y;

}


/*===================================================================
	Procedure	:	Carry out all turning for component gunturret...
				:	Uses Mats instead of Quats....
	Output		:	GUNOBJECT * Object
	Output		:	Nothing
===================================================================*/
void GunRotation( GUNOBJECT * Object, bool AngleDecellBefore  )
{
	GUNTYPE	* GunType;
	GunType = &GunTypes[Object->Type];

	if(AngleDecellBefore )
	{
		Object->Dir.x += Object->Angle.x * framelag;
		Object->Dir.y += Object->Angle.y * framelag;
	}else{
		Object->Dir.x += Object->Angle.x;
		Object->Dir.y += Object->Angle.y;
	}

	
	if( !( GunType->XRotMin == 0.0F && GunType->XRotMax == 0.0F ) )
	{
		if( Object->Dir.x < GunType->XRotMin )
		{
			Object->Dir.x = GunType->XRotMin;
			Object->Angle.x = 0.0F;
		}
		if( Object->Dir.x > GunType->XRotMax )
		{
			Object->Dir.x = GunType->XRotMax;
			Object->Angle.x = 0.0F;
		}
	}else{
		while( Object->Dir.x < -180.0F )
		{
			Object->Dir.x +=360.0F;
		}
		while( Object->Dir.x > 180.0F )
		{
			Object->Dir.x -=360.0F;
		}
	}

	if( !( GunType->YRotMin == 0.0F && GunType->YRotMax == 0.0F ) )
	{
		if( Object->Dir.y < GunType->YRotMin )
		{
			Object->Dir.y = GunType->YRotMin;
			Object->Angle.y = 0.0F;
		}
		if( Object->Dir.y > GunType->YRotMax )
		{
			Object->Dir.y = GunType->YRotMax;
			Object->Angle.y = 0.0F;
		}
	}else{
		while( Object->Dir.y < -180.0F )
		{
			Object->Dir.y +=360.0F;
		}
		while( Object->Dir.y > 180.0F )
		{
			Object->Dir.y -=360.0F;
		}
	}
	if(AngleDecellBefore )
	{
		AccellDecell( &Object->Angle.y , GunTypes[Object->Type].gtTurnDecell );
		AccellDecell( &Object->Angle.x , GunTypes[Object->Type].gtTurnDecell );
	}else{
		Object->Angle.y = 0.0F;
		Object->Angle.x	= 0.0F;
	}
}
/*===================================================================
	Procedure	:	Get Display Matrix of last
				:	user controlled componented object.
	Input		:	OBJECT	*	Object;
				:	MATRIX	*	Matrix ( TBFI )
				:	MATRIX	*	Transposed Matrix ( TBFI )
				:	VECTOR	*	FirePos
				:	int16_t		BaseIndex
	Output		:	bool		true/false
===================================================================*/
bool GetLastCompDispMatrix( OBJECT * Object, MATRIX * Matrix, MATRIX * TMatrix, VECTOR * FirePos, int16_t BaseIndex )
{
	int16_t	Last = -1;

	if( Object->UserContComps[ BaseIndex + 1 ] )
	{
		Last = ( BaseIndex + 1 );
	}
	else
	{
		if( Object->UserContComps[ BaseIndex ] )
		{
			Last = BaseIndex;
		}
	}

	if( Last == -1 )
	{
		return( false );
	}

	*Matrix = Object->UserContComps[ Last ]->DisplayMatrix;
	*FirePos = Object->UserContComps[ Last ]->DisplayPos;
	MatrixMultiply( Matrix, &Object->UserContComps[ Last ]->InitMatrix, Matrix );
	MatrixTranspose( Matrix, TMatrix );

	return( true );
}

/*===================================================================
	Procedure	:	Set turret Vector
	Input		:	OBJECT	*	Object;
				:	VECTOR	*	Vector ( Local )
				:	int16_t		BaseIndex
	Output		:	Nothing
===================================================================*/
void SetTurretVector( OBJECT * Object, VECTOR * Vector, int16_t BaseIndex )
{
	if( Object->UserContComps[ BaseIndex ] )
	{
		Object->UserContComps[ BaseIndex ]->UserAngle = D2R( -Vector->y );
	}

	if( Object->UserContComps[ BaseIndex + 1 ] )
	{
		Object->UserContComps[ BaseIndex + 1 ]->UserAngle = D2R( Vector->x );
	}
}



/*===================================================================
	Procedure	:	Init a gun..
	Input		:	u_int16_t GunType
				:	OBJECT	*	Object;
	Output		:	Nothing
===================================================================*/
void InitGuns( BYTE how_many_guns , u_int16_t * GunType , OBJECT * Object )
{
	GUNOBJECT * GunObject;
	int i;
	
	if( Object->FirstGun )
		free( Object->FirstGun );
	
	Object->FirstGun = (GUNOBJECT*) malloc( how_many_guns * sizeof(GUNOBJECT) );
	
	GunObject = Object->FirstGun;
	
	if( !GunObject )
		return;
	
	Object->HowManyGuns = how_many_guns;
	
	for( i = 0 ; i < how_many_guns ; i++ )
	{
		GunObject->GunNum = i*2;
		GunObject->Type = GunType[i];
		GunObject->ParentObject = Object;
		GunObject->Angle.x = 0.0F;
		GunObject->Angle.y = 0.0F;
		GunObject->Angle.z = 0.0F;
		GunObject->Dir.x = 0.0F;
		GunObject->Dir.y = 0.0F;
		GunObject->Dir.z = 0.0F;
		if( i < how_many_guns-1 )
		{
			GunObject->Next = GunObject+1;
		}else{
			GunObject->Next = NULL;
		}
		GunObject->BurstTime = 0.0F;
		GunObject->ReloadTime = 0.0F;
		GunObject->BurstCount = 0;
		GunObject->FirePosCount = 0;
		GunObject++;
	}
}

/*===================================================================
	Procedure	:	Restrict Movement....
	Input		:	ENEMY * Enemy
				:	VECTOR * Move offset....
	Output		:	Nothing
===================================================================*/
void RestrictMovement( ENEMY * Enemy , VECTOR * Move )
{
	VECTOR v;
	ApplyMatrix( &Enemy->Object.InvMat , Move , &v);


	if( v.x < -( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Left Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_LEFT;
	}
	if( v.x > ( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Right Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_RIGHT;
	}
	if( v.y < -( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Left Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_DOWN;
	}
	if( v.y > ( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Up Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_UP;
	}
	if( v.z < -( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Back Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_BACK;
	}
	if( v.z > ( EnemyTypes[Enemy->Type].MoveRateAccell * EnemyTypes[Enemy->Type].MaxMoveRate ) )
	{
		// Probably no Forward Movement...
		Enemy->AIMoveFlags |= AI_CONTROL_NO_FORWARD;
	}
}

/*===================================================================
	Procedure	:	Enemy to Enemy Collide...
	Input		:	ENEMY * Enemy
				:	VECTOR * Move offset....
	Output		:	Nothing
===================================================================*/
bool Enemy2EnemyCollide( ENEMY * SEnemy , VECTOR * Move )
{
	ENEMY * TEnemy;
	float Move_Length;
	VECTOR	Move_Dir;

	TEnemy = FirstEnemyUsed;

	if( !EnemyTypes[SEnemy->Type].Radius )
		return false;
	while( TEnemy != NULL )
	{
		if( TEnemy != SEnemy )
		{
			if( ( TEnemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !SoundInfo[SEnemy->Object.Group][TEnemy->Object.Group] && EnemyTypes[TEnemy->Type].Radius )
				{
					// Two Enemies are within visible params....
					Move_Length = DistanceVector2Vector( &SEnemy->Object.Pos , &TEnemy->Object.Pos );
					if( Move_Length < ( EnemyTypes[SEnemy->Type].Radius + EnemyTypes[TEnemy->Type].Radius ) )
					{
						// the ships are to close....Move them apart...Guarenteed..
						Move_Dir.x = SEnemy->Object.Pos.x - TEnemy->Object.Pos.x;
						Move_Dir.y = SEnemy->Object.Pos.y - TEnemy->Object.Pos.y;
						Move_Dir.z = SEnemy->Object.Pos.z - TEnemy->Object.Pos.z;
						NormaliseVector( &Move_Dir );

						if( SEnemy->Index < TEnemy->Index )
						{
							SEnemy->Object.ExternalForce.x += Move_Dir.x * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 1.9F) * framelag );
							SEnemy->Object.ExternalForce.y += Move_Dir.y * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 1.9F) * framelag );
							SEnemy->Object.ExternalForce.z += Move_Dir.z * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 1.9F) * framelag );
						}else{
							SEnemy->Object.ExternalForce.x += Move_Dir.x * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 2.1F) * framelag );
							SEnemy->Object.ExternalForce.y += Move_Dir.y * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 2.1F) * framelag );
							SEnemy->Object.ExternalForce.z += Move_Dir.z * ( EnemyTypes[SEnemy->Type].MaxMoveRate * (EnemyTypes[SEnemy->Type].MoveRateAccell * 2.1F) * framelag );
						}
						SEnemy->AIMoveFlags |= AI_CONTROL_COLLISION;
						return true;
					}
				}
			}
		}
		TEnemy = TEnemy->NextUsed;
	}
	return false;

}
/*===================================================================
	Procedure	:	Enemy to Enemy Collide Special for crawling enemies only..
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
bool Enemy2EnemyCollideSpecial( ENEMY * SEnemy , VECTOR * StartPos)
{
	ENEMY * TEnemy;
	float Move_Length;
 	float Move_Length2;

	TEnemy = FirstEnemyUsed;

	while( TEnemy != NULL )
	{
		if( TEnemy != SEnemy && (TEnemy->Status & ENEMY_STATUS_Enable) && (TEnemy->Object.ControlType == ENEMY_CONTROLTYPE_CRAWL_AI) )
		{
			if( ( TEnemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !SoundInfo[SEnemy->Object.Group][TEnemy->Object.Group] )
				{
					// Two Enemies are within visible params....
					Move_Length = DistanceVector2Vector( &SEnemy->Object.Pos , &TEnemy->Object.Pos );
					if( Move_Length < ( ( EnemyTypes[SEnemy->Type].Radius + EnemyTypes[TEnemy->Type].Radius ) * 1.5F ) )
					{
						Move_Length2 = DistanceVector2Vector( StartPos , &TEnemy->Object.Pos );
						if( Move_Length2 > Move_Length )
						{
							// If it moved closer with its last move then bounce it otherwise let it go...
							return true;
						}
					}
				}
			}
		}
		TEnemy = TEnemy->NextUsed;
	}
	return false;

}

/*===================================================================
	Procedure	:	Ship 2 Ship collide...
	Input		:	BikeNumber..
	Output		:	Move_Off filled in...
===================================================================*/
bool Ship2EnemyCollide( u_int16_t i , VECTOR * Move_Off )
{
	float Move_Length;
	float Next_Move_Length;
	VECTOR Move_Dir;
	float OldMove;
	VECTOR	inter;
	VECTOR	inter2;
	VECTOR	Norm_Move_Off;
	VECTOR	Temp_Move_Off;
	VECTOR	NewPos;
	bool	HasBeen = false;
	ENEMY * TEnemy;

	Norm_Move_Off = *Move_Off;
	NormaliseVector( &Norm_Move_Off );
	OldMove = VectorLength( &Ships[i].Move_Off);

	TEnemy = FirstEnemyUsed;
	
	while( TEnemy )
	{
		if( ( TEnemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[Ships[i].Object.Group][TEnemy->Object.Group] )
			{
				Move_Length = DistanceVector2Vector( &Ships[i].Object.Pos , &TEnemy->Object.Pos );
				NewPos.x = Ships[i].Object.Pos.x + Move_Off->x;
				NewPos.y = Ships[i].Object.Pos.y + Move_Off->y;
				NewPos.z = Ships[i].Object.Pos.z + Move_Off->z;

				Next_Move_Length = DistanceVector2Vector( &NewPos , &TEnemy->Object.Pos );
				
				if( (Move_Length < ( SHIP_RADIUS+EnemyTypes[TEnemy->Type].Radius ) ) && ( Next_Move_Length < Move_Length ) )
				{
					if( Move_Length < 1.0F )
						Move_Length = 1.0F;

					// the ships are to close....Move them apart...Guarenteed..
					Move_Dir.x = Ships[i].Object.Pos.x - TEnemy->Object.Pos.x;
					Move_Dir.y = Ships[i].Object.Pos.y - TEnemy->Object.Pos.y;
					Move_Dir.z = Ships[i].Object.Pos.z - TEnemy->Object.Pos.z;
					NormaliseVector( &Move_Dir );

					Move_Length = ( (SHIP_RADIUS+EnemyTypes[TEnemy->Type].Radius)+1.0F ) - Move_Length;

					if( Move_Dir.x == 0.0F && Move_Dir.y == 0.0F && Move_Dir.z == 0.0F )
						return false;
					Move_Off->x = Move_Dir.x * Move_Length;
					Move_Off->y = Move_Dir.y * Move_Length;
					Move_Off->z = Move_Dir.z * Move_Length;
					return true;
				}else{
					if( RaytoSphere2( &TEnemy->Object.Pos, (SHIP_RADIUS+EnemyTypes[TEnemy->Type].Radius)-2.0F, &Ships[i].Object.Pos, &Norm_Move_Off, &inter, &inter2 ) )
					{
						Temp_Move_Off.x = inter.x - Ships[i].Object.Pos.x;
						Temp_Move_Off.y = inter.y - Ships[i].Object.Pos.y;
						Temp_Move_Off.z = inter.z - Ships[i].Object.Pos.z;
						if( VectorLength( &Temp_Move_Off) < VectorLength( Move_Off) )
						{
							*Move_Off = Temp_Move_Off;
							Norm_Move_Off = *Move_Off;
							NormaliseVector( &Norm_Move_Off );
							HasBeen = true;
						}
					}
				}

			}
		}
		TEnemy = TEnemy->NextUsed;
	}
	return HasBeen;
}

/*===================================================================
	Procedure	:	Setup Enemy group link list
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SetupEnemyGroups( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		EnemyGroups[ Count ] = NULL;
		NumEnemiesPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add Enemy to group link list
	Input		:	ENEMY	*	Enemy
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void AddEnemyToGroup( ENEMY * Enemy, u_int16_t Group )
{
	Enemy->PrevInGroup = NULL;
	Enemy->NextInGroup = EnemyGroups[ Group ];
	if( Enemy->NextInGroup ) Enemy->NextInGroup->PrevInGroup = Enemy;
	EnemyGroups[ Group ] = Enemy;
	NumEnemiesPerGroup[ Group ]++;
}

/*===================================================================
	Procedure	:	Remove Enemy from group link list
	Input		:	u_int16_t		Enemy Index
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void RemoveEnemyFromGroup( ENEMY * Enemy, u_int16_t Group )
{
	if( Enemy->PrevInGroup ) Enemy->PrevInGroup->NextInGroup = Enemy->NextInGroup;
	else EnemyGroups[ Group ] = Enemy->NextInGroup;
	if( Enemy->NextInGroup ) Enemy->NextInGroup->PrevInGroup = Enemy->PrevInGroup;
	Enemy->PrevInGroup = NULL;
	Enemy->NextInGroup = NULL;
	NumEnemiesPerGroup[ Group ]--;
}

/*===================================================================
	Procedure	:	Move Enemy from 1 group to another
	Input		:	ENEMY	*	Enemy
				:	u_int16_t		OldGroup
				:	u_int16_t		NewGroup
	Output		:	Nothing
===================================================================*/
void MoveEnemyToGroup( ENEMY * Enemy, u_int16_t OldGroup, u_int16_t NewGroup )
{
	RemoveEnemyFromGroup( Enemy, OldGroup );
	AddEnemyToGroup( Enemy, NewGroup );
}

/*===================================================================
	Procedure	:	Update Enemies ClipGroup
	Input		:	CAMERA	*	Camera
	Output		:	nothing
===================================================================*/
void UpdateEnemiesClipGroup( CAMERA * Camera  )
{
	ENEMY		*	Enemy;
	ENEMY		*	NextEnemy;
	u_int16_t			Model;
	u_int16_t			ClipGroup;
	VECTOR			TopLeft;
	VECTOR			BottomRight;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( VisibleOverlap( Camera->GroupImIn, Enemy->Object.Group, NULL ) )
			{
				TopLeft.x = ( Enemy->Object.TopLeft.x + Enemy->Object.Pos.x );
				TopLeft.y = ( Enemy->Object.TopLeft.y + Enemy->Object.Pos.y );
				TopLeft.z = ( Enemy->Object.TopLeft.z + Enemy->Object.Pos.z );
				BottomRight.x = ( Enemy->Object.BottomRight.x + Enemy->Object.Pos.x );
				BottomRight.y = ( Enemy->Object.BottomRight.y + Enemy->Object.Pos.y );
				BottomRight.z = ( Enemy->Object.BottomRight.z + Enemy->Object.Pos.z );

				ClipGroup = FindClipGroup( Camera, &Mloadheader, &TopLeft, &BottomRight );

				if( Enemy->Object.Components )
				{
					UpdateCompObjClipGroup( Enemy->Object.Components, 1, ClipGroup );
				}
				else
				{
					Model = Enemy->ModelIndex;

					if( Model != (u_int16_t) -1 )
					{
						if( ClipGroup != (u_int16_t) -1 )
						{
							Models[ Model ].Flags |= MODFLAG_UseClipGroup;
							Models[ Model ].ClipGroup = ClipGroup;
						}
						else
						{
							Models[ Model ].Flags &= ~MODFLAG_UseClipGroup;
							Models[ Model ].ClipGroup = Models[ Model ].Group;
						}
					}
				}
			}
		}
		Enemy = NextEnemy;
	}
}



#if 1
/*===================================================================
	Stuff to do with reading in the Enemies.txt!!!!
===================================================================*/

u_int16_t	CurrentEnemy = 0;
u_int16_t	CurrentGun = 0;
// Read the number of Guns
static int read_NumOfGuns( FILE *f, char *last_token )
{
	if ( fscanf( f, " %hu", &EnemyTypes[CurrentEnemy].NumOfGuns ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Gun Type
static int read_GunType( FILE *f, char *last_token )
{
	// NOTE - GunType[CurrentGun] use to be GunType but that's a compiler error in unix somehow it gets away on msvc
	// none the less this function appears to never be used during reading of the enemies.txt
	if ( fscanf( f, " %hu", &EnemyTypes[CurrentEnemy].GunType[CurrentGun] ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read the TurnRateAccell
static int
read_TurnRateAccell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].TurnRateAccell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the TurnRateDecell
static int
read_TurnRateDecell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].TurnRateDecell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MaxTRurnRate
static int
read_MaxTurnRate( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MaxTurnRate ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MoveRateAccell
static int
read_MoveRateAccell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MoveRateAccell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MoveRateDecell
static int
read_MoveRateDecell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MoveRateDecell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MaxTRurnRate
static int
read_MaxMoveRate( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MaxMoveRate ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Viewcone
static int
read_Viewcone( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Viewcone ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MinRange
static int
read_MinRange( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MinRange ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MaxRange
static int
read_MaxRange( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MaxRange ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Shield
static int read_Shield( FILE *f, char *last_token )
{
	if ( fscanf( f, " %hd", &EnemyTypes[CurrentEnemy].Shield ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the PowerLevel
static int read_PowerLevel( FILE *f, char *last_token )
{
	if ( fscanf( f, " %hd", &EnemyTypes[CurrentEnemy].PowerLevel ) == 1 )
	{
		EnemyTypes[CurrentEnemy].PowerLevel--;
		if( EnemyTypes[CurrentEnemy].PowerLevel < 0 )
			EnemyTypes[CurrentEnemy].PowerLevel = 0;
		if( EnemyTypes[CurrentEnemy].PowerLevel >= 3 )
			EnemyTypes[CurrentEnemy].PowerLevel = 2;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the MoveRestrictClearTime
static int
read_MoveRestrictClearTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].MoveRestrictClearTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Radius
static int
read_Radius( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Radius ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the PrimaryFireRate
static int
read_PrimaryFireRate( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].PrimaryFireRate ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the SecondaryFireRate
static int
read_SecondaryFireRate( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].SecondaryFireRate ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Anticipate_Move
static int
read_Anticipate_Move( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.Anticipate_Move ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the NetworkRange
static int
read_NetworkRange( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.NetworkRange ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}


// Read the Avoid_Primary
static int read_Avoid_Primary( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.Avoid_Primary ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Avoid_Secondary
static int read_Avoid_Secondary( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.Avoid_Secondary ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the AvoidMines
static int read_AvoidMines( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.Avoid_Mines ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read the IdleTime
static int
read_IdleTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.IdleTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the ScanTime
static int
read_ScanTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.ScanTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the ThinkTime
static int
read_ThinkTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.ThinkTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the ThinkRange
static int
read_ThinkRange( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.ThinkRange ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the HearRange
static int
read_HearRange( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.HearRange ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the BlindSearchTime
static int
read_BlindSearchTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &EnemyTypes[CurrentEnemy].Behave.BlindSearchTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read the ControlType
static int
read_ControlType( FILE *f, char *last_token )
{
		return 0;
}
// Read the AiMode
static int
read_AiMode( FILE *f, char *last_token )
{
		return 0;
}
// Read the PrimaryWeaponType
static int
read_PrimaryWeaponType( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		EnemyTypes[CurrentEnemy].PrimaryWeaponType = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the SecondaryWeaponType
static int
read_SecondaryWeaponType( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		EnemyTypes[CurrentEnemy].SecondaryWeaponType = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Flags
typedef struct {
	char *keyword;
	u_int32_t Flags;
} READENEMYFLAGS;

static int
read_Flags( FILE *f, char *last_token )
{
	int i;
	READENEMYFLAGS	tab[] = {
		{"AI_BEHAVIOUR_LEAVENETWORK"		 ,AI_BEHAVIOUR_LEAVENETWORK		},		
		{"AI_BEHAVIOUR_NOFRIENDLYFIRE"		 ,AI_BEHAVIOUR_NOFRIENDLYFIRE	},	
		{"AI_BEHAVIOUR_AVOID_PRIMARY"		 ,AI_BEHAVIOUR_AVOID_PRIMARY	},	
		{"AI_BEHAVIOUR_AVOID_SECONDARY"		 ,AI_BEHAVIOUR_AVOID_SECONDARY	},
		{"AI_BEHAVIOUR_AVOID_MINES"			 ,AI_BEHAVIOUR_AVOID_MINES		},
		{"AI_BEHAVIOUR_ANTICIPATE_MOVE"		 ,AI_BEHAVIOUR_ANTICIPATE_MOVE	},
		{"AI_BEHAVIOUR_ATTACK_ONSITE"		 ,AI_BEHAVIOUR_ATTACK_ONSITE	},	
		{"AI_BEHAVIOUR_ATTACK_PROVOKE"		 ,AI_BEHAVIOUR_ATTACK_PROVOKE	},	
		{"AI_BEHAVIOUR_ATTACK_FIND"			 ,AI_BEHAVIOUR_ATTACK_FIND		},
		{"AI_BEHAVIOUR_NETWORKRANGE"		 ,AI_BEHAVIOUR_NETWORKRANGE		},
		{"AI_BEHAVIOUR_WEAPONKNOWLEDGE"		 ,AI_BEHAVIOUR_WEAPONKNOWLEDGE	},
		{"AI_BEHAVIOUR_ICANTPITCH"			 ,AI_BEHAVIOUR_ICANTPITCH		},	
		{"AI_BEHAVIOUR_ICANTYAW"			 ,AI_BEHAVIOUR_ICANTYAW			},
		{"AI_BEHAVIOUR_ATTACKMINES"			 ,AI_BEHAVIOUR_ATTACKMINES		},
		{"AI_BEHAVIOUR_AVOIDMINES"			 ,AI_BEHAVIOUR_AVOIDMINES		},	
		{"AI_BEHAVIOUR_DROPMINES"			 ,AI_BEHAVIOUR_DROPMINES		},	
		{"AI_BEHAVIOUR_RAMMINES"			 ,AI_BEHAVIOUR_RAMMINES			},
		{"AI_BEHAVIOUR_RETREAT"				 ,AI_BEHAVIOUR_RETREAT			},
		{"AI_BEHAVIOUR_DONTSTOPANDTURN"		 ,AI_BEHAVIOUR_DONTSTOPANDTURN	},
		{"AI_BEHAVIOUR_PRECISECONTROL"		 ,AI_BEHAVIOUR_PRECISECONTROL	},
		{"AI_BEHAVIOUR_CLOAK"				 ,AI_BEHAVIOUR_CLOAK			},
		{ NULL , 0 } };
	
	EnemyTypes[CurrentEnemy].Behave.Flags = 0;
	while ( fscanf( f, " %80s", last_token ) == 1 )
	{
		i = 0;
		while( tab[i].keyword )
		{
			if ( !strcasecmp( last_token, tab[i].keyword ) )
			{
				EnemyTypes[CurrentEnemy].Behave.Flags |= tab[i].Flags;
				break;
			}
			i++;
		}
		if( !tab[i].keyword )
			return 0;
	}
	
	fscanf( f, " %80s", last_token );
	return 1;
}


// read the enemy type.....
typedef struct {
	char *keyword;
	u_int16_t EnemyType;
} READENEMYTYPE;

static int read_EnemyType( FILE *f, char *last_token )
{
	int i;
	READENEMYTYPE	tab[] = {
		{"ENEMY_BeamTurret",		ENEMY_BeamTurret		 },
		{"ENEMY_DuelTurret",		ENEMY_DuelTurret		 },
		{"ENEMY_PulseTurret",		ENEMY_PulseTurret		 },
		{"ENEMY_MissileTurret",	ENEMY_MissileTurret		 },
		{"ENEMY_SnubTurret",		ENEMY_SnubTurret		 },
		{"ENEMY_LazerBot",		ENEMY_LazerBot			 },
		{"ENEMY_SnubBot",			ENEMY_SnubBot			 },
		{"ENEMY_AirMoble",		ENEMY_AirMoble			 },
		{"ENEMY_AmmoDump",		ENEMY_AmmoDump			 },
		{"ENEMY_LeviTank",		ENEMY_LeviTank			 },
		{"ENEMY_Max",				ENEMY_Max				 },
		{"ENEMY_Mekton",			ENEMY_Mekton			 },
		{"ENEMY_Boss_Nukecrane",	ENEMY_Boss_Nukecrane	 },
		{"ENEMY_Supressor",		ENEMY_Supressor			 },
		{"ENEMY_MineSweeper",		ENEMY_MineSweeper		 },
		{"ENEMY_Swarm",			ENEMY_Swarm				 },
		{"ENEMY_Shade",			ENEMY_Shade				 },
		{"ENEMY_MineLayer",		ENEMY_MineLayer			 },
		{"ENEMY_Hunter",			ENEMY_Hunter			 },
		{"ENEMY_Legz",			ENEMY_Legz				 },
		{"ENEMY_GuardBot",		ENEMY_GuardBot			 },
		{"ENEMY_Boss_Metatank",		 ENEMY_Boss_Metatank },     
		{"ENEMY_Boss_BigGeek",		 ENEMY_Boss_BigGeek  },     
		{"ENEMY_Boss_Avatar",		 ENEMY_Boss_Avatar   },     
		{"ENEMY_Boss_FleshMorph",	 ENEMY_Boss_FleshMorph },   
		{"ENEMY_Boss_Exogenon",		 ENEMY_Boss_Exogenon } ,
		{"ENEMY_Bike_Lokasena",      ENEMY_Bike_Lokasenna } ,  		 	
		{"ENEMY_Bike_Beard",       	 ENEMY_Bike_Beard } ,       	
		{"ENEMY_Bike_LAJay",    	 ENEMY_Bike_LAJay } ,    		
		{"ENEMY_Bike_ExCop",     	 ENEMY_Bike_ExCop } ,     		
		{"ENEMY_Bike_RexHardy",  	 ENEMY_Bike_RexHardy } ,  		
		{"ENEMY_Bike_Foetoid",    	 ENEMY_Bike_Foetoid } ,    		
		{"ENEMY_Bike_NimSoo",    	 ENEMY_Bike_NimSoo } ,    		
		{"ENEMY_Bike_Nutta",      	 ENEMY_Bike_Nutta } ,      		
		{"ENEMY_Bike_Sceptre",    	 ENEMY_Bike_Sceptre } ,    		
		{"ENEMY_Bike_Jo",         	 ENEMY_Bike_Jo } ,         		
		{"ENEMY_Bike_CuvelClark",	 ENEMY_Bike_CuvelClark } ,		
		{"ENEMY_Bike_HK5",       	 ENEMY_Bike_HK5 } ,       		
		{"ENEMY_Bike_Nubia",      	 ENEMY_Bike_Nubia } ,      		
		{"ENEMY_Bike_Mofisto",    	 ENEMY_Bike_Mofisto } ,    		
		{"ENEMY_Bike_Cerbero",    	 ENEMY_Bike_Cerbero } ,    		
		{"ENEMY_Bike_Slick",      	 ENEMY_Bike_Slick } ,      		
		{"ENEMY_LEADER_Swarm",	  	 ENEMY_LEADER_Swarm } ,	
		{"ENEMY_LEADER_Hunter",	  	 ENEMY_LEADER_Hunter } ,
		{"ENEMY_LEADER_Mekton",	  	 ENEMY_LEADER_Mekton } ,
		{"ENEMY_LEADER_SnubBot",  	 ENEMY_LEADER_SnubBot } ,
		{"ENEMY_LEADER_Legz",	  	 ENEMY_LEADER_Legz } ,	
		{"ENEMY_LEADER_Shade",	  	 ENEMY_LEADER_Shade } ,	
		{"ENEMY_LEADER_Supressor",	 ENEMY_LEADER_Supressor } ,
		{"ENEMY_LEADER_LeviTank", 	 ENEMY_LEADER_LeviTank } ,
		{"ENEMY_LEADER_LazerBot", 	 ENEMY_LEADER_LazerBot } ,
		{"ENEMY_LEADER_Max",	  	 ENEMY_LEADER_Max } ,	
		{"ENEMY_LEADER_AirMoble", 	 ENEMY_LEADER_AirMoble } ,
		{"ENEMY_Fodder1", 			 ENEMY_Fodder1 } ,
		{"ENEMY_Boss_LittleGeek",	ENEMY_Boss_LittleGeek },
		{"ENEMY_Bike_FlyGirl",      ENEMY_Bike_FlyGirl } ,  		 	
	};

	if ( fscanf( f, " %80s", last_token ) == 1 )
	{
		for( i = 0 ; i < ENEMY_LAST ; i++ )
		{
			if (
				(tab[i].keyword != NULL) &&
				(!strcasecmp( last_token, tab[i].keyword ))
			)
				break;
		}
		if( i < ENEMY_LAST )
		{
			CurrentEnemy = tab[i].EnemyType;
		}else{
			// Illegal Enemy Type
		}
	}
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read Gun Type
static int read_NewGunType( FILE *f, char *last_token )
{
	if ( fscanf( f, " %hu", &CurrentGun ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Gun Max Turn Speed
static int read_Gun_MaxTurnSpeed( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].gtMaxTurnSpeed ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Gun Turn Accell..
static int read_Gun_TurnAccell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].gtTurnAccell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Gun Turn Decell..
static int read_Gun_TurnDecell( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].gtTurnDecell ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Gun Reload Time...
static int read_Gun_ReloadTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].ReloadTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Burst Master Time...
static int read_Gun_BurstMasterTime( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].BurstMasterTime ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Burst Master Count...
static int read_Gun_BurstMasterCount( FILE *f, char *last_token )
{
	if ( fscanf( f, " %d", &GunTypes[CurrentGun].BurstMasterCount ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the Burst Angle...
static int read_Gun_BurstAngle( FILE *f, char *last_token )
{
	if ( fscanf( f, " %f", &GunTypes[CurrentGun].BurstAngle ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the PowerLevel...
static int read_Gun_PowerLevel( FILE *f, char *last_token )
{
	if ( fscanf( f, " %hd", &GunTypes[CurrentGun].PowerLevel ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the PrimarySecondary...
static int read_Gun_PrimarySecondary( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		if( temp ) GunTypes[CurrentGun].PrimarySecondary = true;
		else GunTypes[CurrentGun].PrimarySecondary = false;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read the PreciseRotation...
static int read_Gun_PreciseRotation( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		if( temp ) GunTypes[CurrentGun].PreciseRotation = true;
		else GunTypes[CurrentGun].PreciseRotation = false;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read the WeaponType...
static int read_Gun_WeaponType( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		GunTypes[CurrentGun].WeaponType = (BYTE) temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

/*===================================================================
	Procedure	:	Read in the enemy txt file..
	Input		:	char * Filename
	Output		:	bool true/false
===================================================================*/
typedef int (*ReadEnemy)( FILE *, char * );
typedef struct {
	char *keyword;
	ReadEnemy handle;
} READENEMYTXTFILE;

bool ReadEnemyTxtFile( char *Filename )
{
	static READENEMYTXTFILE jumptab[] = {
		{ "EnemyType",				read_EnemyType				},
		{ "ControlType",			read_ControlType			},
		{ "AiMode",					read_AiMode					},
		{ "NumOfGuns",				read_NumOfGuns				},
		{ "GunType",				read_GunType				},
		{ "TurnRateAccell",			read_TurnRateAccell			},
		{ "TurnRateDecell",			read_TurnRateDecell			},
		{ "MaxTurnRate",			read_MaxTurnRate			},
		{ "MoveRateAccell",			read_MoveRateAccell			},
		{ "MoveRateDecell",			read_MoveRateDecell			},
		{ "MaxMoveRate",			read_MaxMoveRate			},
		{ "Viewcone",				read_Viewcone				},
		{ "MinRange",				read_MinRange				},
		{ "MaxRange",				read_MaxRange				},
		{ "Shield",					read_Shield					},
		{ "MoveRestrictClearTime",	read_MoveRestrictClearTime	},
		{ "Radius",					read_Radius					},
		{ "PrimaryFireRate",		read_PrimaryFireRate		},
		{ "SecondaryFireRate",		read_SecondaryFireRate		},
		{ "PowerLevel",				read_PowerLevel				},
		{ "PrimaryWeaponType",		read_PrimaryWeaponType		},
		{ "SecondaryWeaponType",	read_SecondaryWeaponType	},
		{ "Flags",					read_Flags					},
		{ "Anticipate_Move",		read_Anticipate_Move		},
		{ "NetworkRange",			read_NetworkRange			},
		{ "Avoid_Primary",			read_Avoid_Primary			},
		{ "Avoid_Secondary",		read_Avoid_Secondary		},
		{ "Avoid_Mines",			read_AvoidMines				},
		{ "IdleTime",				read_IdleTime				},
		{ "ScanTime",				read_ScanTime				},
		{ "ThinkTime",				read_ThinkTime				},
		{ "ThinkRange",				read_ThinkRange				},
		{ "HearRange",				read_HearRange				},
		{ "BlindSearchTime",		read_BlindSearchTime		},
		{ "Gun_NewGunType",			read_NewGunType		     	},
		{ "Gun_MaxTurnSpeed",		read_Gun_MaxTurnSpeed     	},
		{ "Gun_TurnAccell",			read_Gun_TurnAccell       	},
		{ "Gun_TurnDecell",			read_Gun_TurnDecell       	},
		{ "Gun_ReloadTime",			read_Gun_ReloadTime       	},
		{ "Gun_BurstMasterTime",	read_Gun_BurstMasterTime  	},
		{ "Gun_BurstMasterCount",	read_Gun_BurstMasterCount 	},
		{ "Gun_BurstAngle",			read_Gun_BurstAngle       	},
		{ "Gun_PowerLevel",			read_Gun_PowerLevel       	},
		{ "Gun_PrimarySecondary",	read_Gun_PrimarySecondary 	},
		{ "Gun_WeaponType",			read_Gun_WeaponType       	},
		{ "Gun_PreciseRotation",	read_Gun_PreciseRotation 	},
		{ NULL,						NULL						}
	};
	FILE *f;
	char token[256];
	int j;

	CurrentEnemy = 0;
	CurrentGun = 0;

	f = file_open( Filename, "r" );
	if ( !f )
	{
		DebugPrintf("Could not read enemy text file: %s\n", Filename);
		Msg("Could not read enemy text file: %s\n",Filename);
		return false;
	}

	if ( fscanf( f, " %80s", token ) == 1 )
	{
		do
		{
			for ( j = 0; jumptab[ j ].keyword; j++ )
			{
				if ( !strcasecmp( token, jumptab[ j ].keyword ) )
					break;
			}
			if ( jumptab[ j ].handle )
			{
				(jumptab[ j ].handle)( f, token );
			}
			else
				fscanf( f, " %80s", token );
		} while ( !feof( f ) );
	}
	fclose( f );

	return true;
}

void ObjectForceExternalOneOff( OBJECT * Object, VECTOR *force )
{
	Object->ExternalForce.x += force->x;
	Object->ExternalForce.y += force->y;
	Object->ExternalForce.z += force->z;
}

/*===================================================================
	Procedure	:	Carry out all movement / collision/ turning...
	Output		:	OBJECt * Object
	Output		:	Nothing
===================================================================*/
void AutoMovementFleshmorph( OBJECT * Object , ENEMY * Enemy )
{
	float	Speed;
	VECTOR	Move_Off;	
	VECTOR	Move_Dir;
//	QUAT	StepQuat;	
	VECTOR	StartPos;
	u_int16_t	OldGroup;


	StartPos = Object->Pos;
	OldGroup = Object->Group;
	AccellDecell( &Object->Speed.x , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->Speed.y , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->Speed.z , EnemyTypes[Enemy->Type].MoveRateDecell );

#if 0
	AccellDecell( &Object->Angle.x , EnemyTypes[Enemy->Type].TurnRateDecell );
	AccellDecell( &Object->Angle.y , EnemyTypes[Enemy->Type].TurnRateDecell * 2.0F);
	AccellDecell( &Object->Angle.z , RollDecell );
	MakeQuat( Object->Angle.x * framelag, Object->Angle.y * framelag, Object->Angle.z * framelag, &StepQuat );
	QuatMultiply(  &Object->Quat , &StepQuat , &Object->Quat );
#endif

	QuatToMatrix( &Object->Quat, &Object->Mat );
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;

	// carry out movements
	Move_Off.x = 0.0F;
	Move_Off.y = 0.0F;
	Move_Off.z = 0.0F;
	if ( Object->Speed.x )
	{
		Speed = Object->Speed.x * framelag;
		ApplyMatrix( &Object->Mat, &SlideRight, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}
	if ( Object->Speed.y )
	{
		Speed = Object->Speed.y * framelag;
		ApplyMatrix( &Object->Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}
	if ( Object->Speed.z )
	{
		Speed = Object->Speed.z * framelag;
		ApplyMatrix( &Object->Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
	   	Move_Off.x += ( Move_Dir.x * Speed );
	   	Move_Off.y += ( Move_Dir.y * Speed );
	   	Move_Off.z += ( Move_Dir.z * Speed );
	}

	Object->Pos.x += Move_Off.x;
	Object->Pos.y += Move_Off.y;
	Object->Pos.z += Move_Off.z;

	Object->Group = MoveGroup( &Mloadheader, &StartPos, Object->Group, &Move_Off );
}

/*===================================================================
	Procedure	:	Carry out all External force modifiers for Spline Following enemies..
	Output		:	OBJECt * Object
	Output		:	bool TURE/false....Wether there was any 
===================================================================*/
bool SplineAutoMovement( OBJECT * Object , ENEMY * Enemy )
{
	VECTOR	ImpactPoint;
	VECTOR	Move_Off;	
	u_int16_t	ImpactGroup;
	BGOBJECT * BGObject;
	VECTOR	ExtForce;
	bool	HasBeenExternal;
	VECTOR	StartPos;
	u_int16_t	OldGroup;
	float	ShieldModifier;
	QUAT	StepQuat;
	NODE * Node3;


	Node3 = (NODE*)Enemy->SplineNode3;
	if( Node3 )
	{
		AI_AimAtTarget( &Enemy->Object.InvMat , &Enemy->Object.Pos, &Node3->Pos );
		Enemy->AI_Angle = AimData.Angle;
	}else{
		Enemy->AI_Angle.x = 0.0F;
		Enemy->AI_Angle.y = 0.0F;
		Enemy->AI_Angle.z = 0.0F;
	}

	Object->Autolevel = ObjectAutoLevelRot( Object , 0.05F * framelag );

#if 1
	Enemy->AI_Angle.x *= EnemyTypes[Enemy->Type].TurnRateDecell * framelag;
	Enemy->AI_Angle.y *= EnemyTypes[Enemy->Type].TurnRateDecell * framelag;
#else
	if( Enemy->AI_Angle.x < 0.0F )
	{
		if( Enemy->AI_Angle.x < -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
		{
			Enemy->AI_Angle.x = -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F);
		}
	}else{
		if( Enemy->AI_Angle.x > 0.0F )
		{

			if( Enemy->AI_Angle.x > (EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
			{
				Enemy->AI_Angle.x = EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F;
			}
		}
	}
	if( Enemy->AI_Angle.y < 0.0F )
	{
		if( Enemy->AI_Angle.y < -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
		{
			Enemy->AI_Angle.y = -(EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F);
		}
	}else{
		if( Enemy->AI_Angle.y > 0.0F )
		{

			if( Enemy->AI_Angle.y > (EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F) )
			{
				Enemy->AI_Angle.y = EnemyTypes[Enemy->Type].MaxTurnRate*framelag*0.5F;
			}
		}
	}
#endif
	

	MakeQuat( Enemy->AI_Angle.x , Enemy->AI_Angle.y , Object->Autolevel, &StepQuat );
	QuatMultiply(  &Object->Quat , &StepQuat , &Object->Quat );
	QuatMultiply(  &Object->RotationForce , &Object->Quat , &Object->Quat );
	QuatToMatrix( &Object->Quat, &Object->Mat );
	MatrixTranspose( &Object->Mat, &Object->InvMat );
	Object->FinalQuat = Object->Quat;
	Object->FinalMat = Object->Mat;
	Object->FinalInvMat = Object->InvMat;
	
	MakeQuat( 0.0F, 0.0F, 0.0F, &Enemy->Object.RotationForce );
	
	StartPos = Object->Pos;
	OldGroup = Object->Group;
	
	// carry out movements
	Move_Off.x = 0.0F;
	Move_Off.y = 0.0F;
	Move_Off.z = 0.0F;


	// apply external and internal force vectors to ship movement
	AccellDecell( &Object->ExternalForce.x , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->ExternalForce.y , EnemyTypes[Enemy->Type].MoveRateDecell );
	AccellDecell( &Object->ExternalForce.z , EnemyTypes[Enemy->Type].MoveRateDecell );
	
	// Collide with other enemies....
	Enemy2EnemyCollide( Enemy , &Move_Off );

	Move_Off.x += Object->ExternalForce.x * framelag;
	Move_Off.y += Object->ExternalForce.y * framelag;
	Move_Off.z += Object->ExternalForce.z * framelag;
	
	
	if( VectorLength( &Move_Off ) <= EnemyTypes[Enemy->Type].MoveRateAccell )
	{
		Move_Off.x = 0.0F; 
		Move_Off.y = 0.0F; 
		Move_Off.z = 0.0F;
		ImpactPoint = Object->Pos;
		ImpactGroup = Object->Group;
	}else{
		if ( Object->Group != (u_int16_t) -1 )
		{
			BGObject = NULL;
			ObjectCollide( Object, &Move_Off, EnemyTypes[Enemy->Type].Radius, &BGObject );
			if( BGObject ) ChangeBGState( BGObject, OWNER_ENEMY, Enemy->Index, BUMP, 0.0F );
		}else{
				Object->Pos.x += Move_Off.x;
				Object->Pos.y += Move_Off.y;
				Object->Pos.z += Move_Off.z;
		}
		ExtForce.x = 0.0F;
		ExtForce.y = 0.0F;
		ExtForce.z = 0.0F;
		HasBeenExternal = ExternalForcesAreaCheck( &Object->Pos , &StartPos , Object->Group, &ExtForce , &ShieldModifier);
		if( HasBeenExternal )
		{
			ObjectForceExternalOneOff( Object, &ExtForce );
		}
	
	}
	
	if( Move_Off.x + Move_Off.y + Move_Off.z )
		return true;
	else
		return false;
}



#endif

/*===================================================================
	Procedure	:	Crappy Bodge to fix mappers fuckup
	Input		:	BYTE	Primary Weapon
				:	u_int16_t	PickupHeld
	Output		:	BYTE	Primary Weapon to Fire
===================================================================*/
BYTE BodgePrimaryWeapon( BYTE Weapon, u_int16_t Pickup )
{
	if( Pickup <= PICKUP_Laser )
	{
		Weapon = ( Pickup + TROJAX );
	}
	return( Weapon );
}

/*===================================================================
	Procedure	:	Find Duplicate Model ( If exists )
	Input		:	int8_t	*	Filename of Model
				:	int16_t		NumModels so far
	Output		:	u_int16_t		Model Number ( -1 if not found )
===================================================================*/
u_int16_t FindDuplicateModel( int8_t * Filename, int16_t NumModels )
{
	int16_t	Count;

	for( Count = 0; Count < NumModels; Count++ )
	{
		if( !strcasecmp( &ModelNames[ Count ].Name[ 0 ], Filename ) )
		{
			return( Count );
		}
	}
	return( (u_int16_t) -1 );
}

/*===================================================================
	Procedure	:	Perform Damage on an Enemy....If its not in a death
				:	MODE then put it in one or Destroy it....
	Input		:	ENEMY * Enemy
	Output		:	bool Destroyed 
===================================================================*/
bool DamageEnemy( ENEMY * Enemy , float Damage , VECTOR * ImpactPoint , VECTOR * Dir, float Speed , u_int16_t Owner , u_int16_t OwnerType )
{
	u_int16_t Random;
	VECTOR	TempVector;
	VECTOR	Pos;
	Enemy->Object.Shield += Damage;

	if( OwnerType == OWNER_SHIP )
	{
		if( EnemyTypes[Enemy->Type].Behave.Flags & AI_BEHAVIOUR_ATTACK_PROVOKE )
		{
			Enemy->TShip = &Ships[Owner].Object;
			Enemy->IveBeenHitTimer = 10.0F * 60.0F;
		}
		Enemy->AIFlags |= AI_IVEBEENHITBYPLAYER;
	}

	if( Enemy->Type == 	ENEMY_Boss_FleshMorph )
	{
		if( Random_Range( 20 ) > 15 )
		{
			PlayPannedSfx( SFX_FleshmorphPainCry, Enemy->Object.Group , &Enemy->Object.Pos, 0.0F );
		}
	}

//	Enemy->Object.Red = 0.0F;
//	Enemy->Object.Green = 255.0F;
//	Enemy->Object.Blue = 255.0F;

	if( Enemy->Object.Shield < 0.0F )
	{
		if(	Enemy->Type == 	ENEMY_Boss_Metatank ||
			Enemy->Type == 	ENEMY_Boss_BigGeek	||
			Enemy->Type == 	ENEMY_Boss_Avatar	||
			Enemy->Type == 	ENEMY_Boss_FleshMorph ||
			Enemy->Type == 	ENEMY_Boss_Exogenon	 ||
			Enemy->Type == 	ENEMY_Boss_LittleGeek )
		{
			PlaySfx( SFX_BIKER_VP, 0.0F );

			ApplyMatrix( &Enemy->Object.Mat, &SlideUp, &TempVector );
			CreateExplosionDonut( &Enemy->Object.Pos, &TempVector, Enemy->Object.Group );

			if( Enemy->Type == ENEMY_Boss_FleshMorph )
			{
				PlayPannedSfx( SFX_FleshmorphDeathCry, Enemy->Object.Group , &Enemy->Object.Pos, 0.0F );
			}

			
			Pos = Enemy->Object.Pos;

			Pos.x += 512.0F;
			ForcePlayPannedSfx( SFX_Exp2, Enemy->Object.Group , &Pos, 0.0F );
			Pos.x -= 1024.0F;
			ForcePlayPannedSfx( SFX_BangBang, Enemy->Object.Group , &Pos, 0.0F );
			Pos.x += 512.0F;
			Pos.z += 512.0F;
			ForcePlayPannedSfx( SFX_Exp3, Enemy->Object.Group , &Pos, 0.0F );
			Pos.z -= 1024.0F;
			ForcePlayPannedSfx( SFX_Exp4, Enemy->Object.Group , &Pos, 0.0F );

			Ships[WhoIAm].ShakeTimer = 3.0F * 60.0F;
			Ships[WhoIAm].ShakeForce = 3.0F;
		}else
		{
			// Play victory phrase once in a while...
			if ( !Random_Range( 6 ) )
				PlaySfx( SFX_BIKER_VP, 0.0F );
		}
	
		if( !(Enemy->Object.AI_Mode >= AIMODE_DEATH_CRASHNBURN ) &&
			 (Enemy->Object.ControlType != ENEMY_CONTROLTYPE_FLY_AI) )
		{
			// Die Now....
			return true;
		}

		if( Enemy->Object.AI_Mode < AIMODE_DEATH_CRASHNBURN )
		{

			if( !Random_Range(30) )
				return true;
			Random = 0;
			if( EnemyTypes[Enemy->Type].Radius <= 100.0F )
			{
				Random = Random_Range(7);
			}
			switch( Random )
			{
			case 0:
			case 1:
				Enemy->Object.AI_Mode = AIMODE_DEATH_CRASHNBURN;
				Enemy->Timer = 3.0F * 60.0F;
				Enemy->AIMoveFlags &= ~AI_CONTROL_COLLISION;
				Enemy->AIMoveRestrictClearTime = EnemyTypes[Enemy->Type].MoveRestrictClearTime;
				break;
			case 2:
			case 3:
 			case 4:
			default:
				Enemy->Object.AI_Mode = AIMODE_DEATH_PINGOFF;
				Enemy->Timer = PINGOFFTIME;
				Enemy->AIMoveFlags &= ~AI_CONTROL_COLLISION;
				Enemy->AIMoveRestrictClearTime = EnemyTypes[Enemy->Type].MoveRestrictClearTime;
				Enemy->LastDamagePos = Enemy->Object.Pos;
				Enemy->LastDamageImpactPos = *ImpactPoint;
				Enemy->LastDamageDir = *Dir;
				Damage *= 0.1F;
				if( Damage > 16.0F )
					Damage = 16.0F;
				if( Damage < 8.0F )
					Damage = 8.0F;
				Enemy->LastDamage = Damage;
				Enemy->LastDamageSpeed = Speed;
				break;
			case 5:
			case 6:
				Enemy->Object.AI_Mode = AIMODE_DEATH_PSYCHO;
				Enemy->Timer = 1.5F * 60.0F;
				Enemy->AIMoveFlags &= ~AI_CONTROL_COLLISION;
				Enemy->AIMoveRestrictClearTime = EnemyTypes[Enemy->Type].MoveRestrictClearTime;
				PlayPannedSfx( SFX_EnemySpin, Enemy->Object.Group , &Enemy->Object.Pos, 0.0F );
				break;
			}
		}
	}
	return false;
}


void ObjectRotateExternal( OBJECT * Object, VECTOR * Pos , VECTOR *point, VECTOR *dir, float force , float Radius )
{
	VECTOR to_ship;
	VECTOR axis;
	VECTOR near_ship;
	float t;
	float angle;
	QUAT rot;

	to_ship.x = Pos->x - point->x;
	to_ship.y = Pos->y - point->y;
	to_ship.z = Pos->z - point->z;
	CrossProduct( dir, &to_ship, &axis );
	NormaliseVector( &axis );
	t = DotProduct( dir, &to_ship ); // / DotProduct( dir, dir )  iff |dir| != 1
	near_ship.x = point->x + t * dir->x - Pos->x;
	near_ship.y = point->y + t * dir->y - Pos->y;
	near_ship.z = point->z + t * dir->z - Pos->z;
	angle = force * VectorLength( &near_ship ) / Radius;
	QuatMake( &rot, axis.x, axis.y, axis.z, angle );
	QuatMultiply( &rot, &Object->RotationForce, &Object->RotationForce );
}


/*===================================================================
	Procedure	:		Find Point 75.0F above the ground...
	Input		:		VECTOR * Pos , u_int16_t Group , VECTOR * NewPos , u_int16_t * NewGroup
	Output		:		bool false/true
===================================================================*/
bool FindPointAboveGround( VECTOR * Pos , u_int16_t Group , VECTOR * NewPos , u_int16_t * NewGroup )
{
	int			i;
	NORMAL		TempNormal;
	VECTOR		TempPos_New;
	VECTOR		Move_Off = { 0.0F , -4000.0F , 0.0F };
	bool		LegalGroup;

	*NewGroup = Group;
	*NewPos = *Pos;

	LegalGroup = false;
		
	if( !PointInsideSkin( Pos, Group ) )
	{
		for( i = 0 ; i < Mloadheader.num_groups ; i++ )
		{
			if( PointInsideSkin( Pos, (u_int16_t) i ) )
			{
				Group = i;
				LegalGroup = true;
				break;
			}
		}
	}else{
		LegalGroup = true;
	}

	if( LegalGroup )
	{
		if(	BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &Move_Off,
			NewPos , NewGroup, &TempNormal, &TempPos_New, false, NULL ) )
		{
			VECTOR move;
			float move_len;

			move.x = NewPos->x - Pos->x;
			move.y = NewPos->y - Pos->y;
			move.z = NewPos->z - Pos->z;
			move_len = VectorLength( &move );
			if ( move_len > 4000.0F )
				return false;
			NewPos->y += 75.0F;
		}else{
			return false;
		}

	}
	
	return true;
}

/*===================================================================
	Procedure	:		Do Enemy Damage Effects
	Input		:		ENEMY	*	Enemy
	Output		:		Nothing
===================================================================*/
void DoNmeDamagedEffects( ENEMY * Enemy )
{
	u_int16_t	fmpoly;
	VECTOR	TempUp;
	float	EnemySmokeInterval;
	u_int8_t	Colour;

	EnemySmokeInterval = (float) 1;

	if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
	{
		if( Enemy->Object.Shield <= (EnemyTypes[Enemy->Type].Shield * 0.25F ) )
		{
			if( IsGroupVisible[ Enemy->Object.Group ] )
			{
				Enemy->SmokeTime += framelag;
	
				if( Enemy->SmokeTime >= EnemySmokeInterval )
				{
					Enemy->SmokeTime = FMOD( Enemy->SmokeTime, EnemySmokeInterval );
	
					fmpoly = FindFreeFmPoly();
					
					Colour = (u_int8_t) 128.0F; //( ( ( EnemyTypes[Enemy->Type].Shield * 0.25F ) / 192.0F ) * Enemy->Object.Shield );

					if( fmpoly != (u_int16_t ) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 1000.0F;
						ApplyMatrix( &Enemy->Object.FinalMat, &Backward, &FmPolys[ fmpoly ].Dir );
						ApplyMatrix( &Enemy->Object.FinalMat, &SlideUp, &TempUp );
						FmPolys[ fmpoly ].UpVector = SlideUp;
						FmPolys[ fmpoly ].Pos = Enemy->Object.Pos;
						FmPolys[ fmpoly ].SeqNum = FM_BLACK_SMOKE;															   
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].R = Colour;
						FmPolys[ fmpoly ].G = Colour;
						FmPolys[ fmpoly ].B = Colour;
#if ACTUAL_TRANS
						FmPolys[ fmpoly ].Trans = 128;
#else
						FmPolys[ fmpoly ].Trans = 255;
#endif
						FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
						FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
						FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
						FmPolys[ fmpoly ].xsize = SMOKE_TRAIL_SIZE * 2.0F;
						FmPolys[ fmpoly ].ysize = SMOKE_TRAIL_SIZE * 2.0F;
						FmPolys[ fmpoly ].Speed = ( ( ( ( (float) Random_Range( 4000 ) ) / 1000.0F ) + 2.0F ) * GLOBAL_SCALE ); //( 16.0F * GLOBAL_SCALE );
						FmPolys[ fmpoly ].UpSpeed = 0.0F;
						FmPolys[ fmpoly ].Group = Enemy->Object.Group;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
				}
			}
			else
			{
				Enemy->SmokeTime = 0.0F;
			}
		}
		else
		{
			Enemy->SmokeTime = 0.0F;
		}
	}
	else
	{
		Enemy->SmokeTime = 0.0F;
	}
}



#if 0
typedef long fix;				//16 bits int, 16 bits frac

typedef struct vms_vector {
	fix x,y,z;
} vms_vector;

//A 3x3 rotation matrix.  Sorry about the numbering starting with one.
//Ordering is across then down, so <m1,m2,m3> is the first row
typedef struct vms_matrix {
	vms_vector rvec,uvec,fvec;
} vms_matrix;


typedef struct frame_info {
1	ubyte				type;						// What type of packet
3	ubyte				pad[3];					// Pad out length of frame_info packet
4	int				numpackets;			
12	vms_vector		obj_pos;
36	vms_matrix		obj_orient;
12	vms_vector		phys_velocity;
12	vms_vector		phys_rotvel;
2	short				obj_segnum;
2	ushort			data_size;		// Size of data appended to the net packet
1	ubyte				playernum;
1	ubyte				obj_render_type;
1	ubyte				level_num;
1	ubyte				data[NET_XDATA_SIZE];		// extra data to be tacked on the end
} frame_info;
// Descent Packet size = 88


typedef struct _VERYSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16_t				Bank;		// How much Am I banked....
	u_int16_t				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
	// 36 bytes norm 58
} VERYSHORTGLOBALSHIP, *LPVERYSHORTGLOBALSHIP;

typedef struct _FSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16_t				Bank;		// How much Am I banked....
	u_int16_t				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
} FSHORTGLOBALSHIP, *LPFSHORTGLOBALSHIP;
//38 bytes norm 65
#endif



/*===================================================================
	Procedure	:		Enemy Save...
	Input		:		FILE * fp
	Output		:		bool true/false
===================================================================*/
bool Enemy_Save( FILE * fp )
{
	int i;
	int e;
	NODE * Node;
	ENEMY * Enemy;


	fwrite( &NumInitEnemies, sizeof( NumInitEnemies ), 1, fp );
	fwrite( &NumKilledEnemies, sizeof( NumKilledEnemies ), 1, fp );
	fwrite( &DifficultyLevel, sizeof( DifficultyLevel ), 1, fp );
	

	e = -1;
	if( Enemy = FirstEnemyUsed )
	{
		e = Enemy->Index;
	}
	fwrite( &e, sizeof( e ), 1, fp );

	e = -1;
	if( Enemy = FirstEnemyFree )
	{
		e = Enemy->Index;
	}
	fwrite( &e, sizeof( e ), 1, fp );




	fwrite( &FleshMorphTimer, sizeof( FleshMorphTimer ), 1, fp );
	
	for( i = 0 ; i < NumInitEnemies ; i++ )
	{
		fwrite( &Enemies[i].Used, sizeof( Enemies[i].Used ), 1, fp );
		fwrite( &Enemies[i].Alive, sizeof( Enemies[i].Alive ), 1, fp );
		fwrite( &Enemies[i].CompCollision, sizeof( Enemies[i].CompCollision ), 1, fp );

		if( !Object_Save( fp , &Enemies[i].Object ) )
			return false;

		fwrite( &Enemies[i].Type, sizeof( Enemies[i].Type ), 1, fp );
		fwrite( &Enemies[i].AIMoveFlags, sizeof( Enemies[i].AIMoveFlags ), 1, fp );
		fwrite( &Enemies[i].AIFlags, sizeof( Enemies[i].AIFlags ), 1, fp );
		fwrite( &Enemies[i].AI_Angle, sizeof( Enemies[i].AI_Angle ), 1, fp );
		fwrite( &Enemies[i].Timer, sizeof( Enemies[i].Timer ), 1, fp );
		fwrite( &Enemies[i].ThinkTimer, sizeof( Enemies[i].ThinkTimer ), 1, fp );
		fwrite( &Enemies[i].AIMoveRestrictClearTime, sizeof( Enemies[i].AIMoveRestrictClearTime ), 1, fp );
		fwrite( &Enemies[i].IveBeenHitTimer, sizeof( Enemies[i].IveBeenHitTimer ), 1, fp );
		e = -1;
		if( Enemies[i].TShip )
		{
			for( e = 0 ; e < MAX_PLAYERS ; e++ )
			{
				if( Enemies[i].TShip == &Ships[e] )
				{
					break;
				}
			}
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Node = Enemies[i].TNode )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Node = Enemies[i].NextTNode )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Node = Enemies[i].LastTNode )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		fwrite( &Enemies[i].LastDistance, sizeof( Enemies[i].LastDistance ), 1, fp );
		fwrite( &Enemies[i].LastPrimaryFiredAtMe, sizeof( Enemies[i].LastPrimaryFiredAtMe ), 1, fp );
		fwrite( &Enemies[i].LastSecondaryFiredAtMe, sizeof( Enemies[i].LastSecondaryFiredAtMe ), 1, fp );
		fwrite( &Enemies[i].Viewcone, sizeof( Enemies[i].Viewcone ), 1, fp );
		fwrite( &Enemies[i].PickupHeld, sizeof( Enemies[i].PickupHeld ), 1, fp );
		fwrite( &Enemies[i].TargetingPos, sizeof( Enemies[i].TargetingPos ), 1, fp );
		fwrite( &Enemies[i].TargetingDir, sizeof( Enemies[i].TargetingDir ), 1, fp );

		fwrite( &Enemies[i].MinePos, sizeof( Enemies[i].MinePos ), 1, fp );
		fwrite( &Enemies[i].MineDistance, sizeof( Enemies[i].MineDistance ), 1, fp );
		fwrite( &Enemies[i].MineNum, sizeof( Enemies[i].MineNum ), 1, fp );

		fwrite( &Enemies[i].AvoidTimer, sizeof( Enemies[i].AvoidTimer), 1, fp );
		fwrite( &Enemies[i].AvoidType, sizeof( Enemies[i].AvoidType ), 1, fp );

		fwrite( &Enemies[i].Status, sizeof( Enemies[i].Status ), 1, fp );
		fwrite( &Enemies[i].GenType, sizeof( Enemies[i].GenType ), 1, fp );
		fwrite( &Enemies[i].BulletID, sizeof( Enemies[i].BulletID ), 1, fp );
		fwrite( &Enemies[i].ModelNum, sizeof( Enemies[i].ModelNum ), 1, fp );
		fwrite( &Enemies[i].ModelIndex, sizeof( Enemies[i].ModelIndex ), 1, fp );
		fwrite( &Enemies[i].TriggerMod, sizeof( Enemies[i].TriggerMod ), 1, fp );
		fwrite( &Enemies[i].Delay, sizeof( Enemies[i].Delay ), 1, fp );

		e = -1;
		if( Enemy = Enemies[i].PrevUsed )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Enemy = Enemies[i].NextUsed )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Enemy = Enemies[i].PrevFree )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Enemy = Enemies[i].NextFree )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Enemy = Enemies[i].NextInGroup )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Enemy = Enemies[i].PrevInGroup )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		fwrite( &Enemies[i].PrimaryFireTimer, sizeof( Enemies[i].PrimaryFireTimer ), 1, fp );
		fwrite( &Enemies[i].SecondaryFireTimer, sizeof( Enemies[i].SecondaryFireTimer ), 1, fp );
		fwrite( &Enemies[i].ImInNodeTransition, sizeof( Enemies[i].ImInNodeTransition ), 1, fp );
		fwrite( &Enemies[i].PickNewNodeNow, sizeof( Enemies[i].PickNewNodeNow ), 1, fp );
		fwrite( &Enemies[i].DistanceToPointOnSlope, sizeof( Enemies[i].DistanceToPointOnSlope ), 1, fp );

		e = -1;
		if( Enemy = Enemies[i].FormationLink )
		{
			e = Enemy->Index;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		fwrite( &Enemies[i].FormationOffset, sizeof( Enemies[i].FormationOffset ), 1, fp );


		e = -1;
		if( Node = Enemies[i].SplineNode1 )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );
		e = -1;
		if( Node = Enemies[i].SplineNode2 )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		e = -1;
		if( Node = Enemies[i].SplineNode3 )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		e = -1;
		if( Node = Enemies[i].SplineNode4 )
		{
			e = Node->NodeNum;
		}
		fwrite( &e, sizeof( e ), 1, fp );

		fwrite( &Enemies[i].TempSplineNode, sizeof( Enemies[i].TempSplineNode ), 1, fp );

		fwrite( &Enemies[i].LastDamagePos, sizeof( Enemies[i].LastDamagePos ), 1, fp );
		fwrite( &Enemies[i].LastDamageImpactPos, sizeof( Enemies[i].LastDamageImpactPos ), 1, fp );
		fwrite( &Enemies[i].LastDamageDir, sizeof( Enemies[i].LastDamageDir ), 1, fp );
		fwrite( &Enemies[i].LastDamage, sizeof( Enemies[i].LastDamage ), 1, fp );
		fwrite( &Enemies[i].LastDamageSpeed, sizeof( Enemies[i].LastDamageSpeed ), 1, fp );
		fwrite( &Enemies[i].CloakTimer, sizeof( Enemies[i].CloakTimer ), 1, fp );
		fwrite( &Enemies[i].GenerationDelay, sizeof( Enemies[i].GenerationDelay ), 1, fp );
		fwrite( &Enemies[i].MasterGenerationDelay, sizeof( Enemies[i].MasterGenerationDelay ), 1, fp );
		fwrite( &Enemies[i].SmokeTime, sizeof( Enemies[i].SmokeTime ), 1, fp );
		fwrite( &Enemies[i].FirePosCount, sizeof( Enemies[i].FirePosCount ), 1, fp );
	}


	return true;
}
/*===================================================================
	Procedure	:		Enemy Load...
	Input		:		FILE * fp
	Output		:		bool true/false
===================================================================*/
bool Enemy_Load( FILE * fp )
{
	int i;
	int e;
	int16_t		TempNumInitEnemies;


	fread( &TempNumInitEnemies, sizeof( TempNumInitEnemies ), 1, fp );

	if( NumInitEnemies != TempNumInitEnemies )
	{
		fclose(fp);
		return false;
	}


	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		EnemyGroups[i] = NULL;
		NumEnemiesPerGroup[i] = 0;
	}

	
	fread( &NumKilledEnemies, sizeof( NumKilledEnemies ), 1, fp );
	fread( &DifficultyLevel, sizeof( DifficultyLevel ), 1, fp );

	fread( &e, sizeof( e ), 1, fp );
	if( e == -1 )
	{
		FirstEnemyUsed = NULL;
	}else{
		FirstEnemyUsed = &Enemies[e];
	}
	fread( &e, sizeof( e ), 1, fp );
	if( e == -1 )
	{
		FirstEnemyFree = NULL;
	}else{
		FirstEnemyFree = &Enemies[e];
	}

	fread( &FleshMorphTimer, sizeof( FleshMorphTimer ), 1, fp );
	
	for( i = 0 ; i < NumInitEnemies ; i++ )
	{
		fread( &Enemies[i].Used, sizeof( Enemies[i].Used ), 1, fp );
		fread( &Enemies[i].Alive, sizeof( Enemies[i].Alive ), 1, fp );
		fread( &Enemies[i].CompCollision, sizeof( Enemies[i].CompCollision ), 1, fp );

		if( !Object_Load( fp, &Enemies[i].Object ) )
			return false;

		fread( &Enemies[i].Type, sizeof( Enemies[i].Type ), 1, fp );
		fread( &Enemies[i].AIMoveFlags, sizeof( Enemies[i].AIMoveFlags ), 1, fp );
		fread( &Enemies[i].AIFlags, sizeof( Enemies[i].AIFlags ), 1, fp );
		fread( &Enemies[i].AI_Angle, sizeof( Enemies[i].AI_Angle ), 1, fp );
		fread( &Enemies[i].Timer, sizeof( Enemies[i].Timer ), 1, fp );
		fread( &Enemies[i].ThinkTimer, sizeof( Enemies[i].ThinkTimer ), 1, fp );
		fread( &Enemies[i].AIMoveRestrictClearTime, sizeof( Enemies[i].AIMoveRestrictClearTime ), 1, fp );
		fread( &Enemies[i].IveBeenHitTimer, sizeof( Enemies[i].IveBeenHitTimer ), 1, fp );

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].TShip = NULL;
		}else{
			Enemies[i].TShip = &Ships[e];
		}

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].TNode = NULL;
		}else{
			Enemies[i].TNode = NodeNetworkHeader.FirstNode + e;
		}

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].NextTNode = NULL;
		}else{
			Enemies[i].NextTNode = NodeNetworkHeader.FirstNode + e;
		}

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].LastTNode = NULL;
		}else{
			Enemies[i].LastTNode = NodeNetworkHeader.FirstNode + e;
		}

		fread( &Enemies[i].LastDistance, sizeof( Enemies[i].LastDistance ), 1, fp );
		fread( &Enemies[i].LastPrimaryFiredAtMe, sizeof( Enemies[i].LastPrimaryFiredAtMe ), 1, fp );
		fread( &Enemies[i].LastSecondaryFiredAtMe, sizeof( Enemies[i].LastSecondaryFiredAtMe ), 1, fp );
		fread( &Enemies[i].Viewcone, sizeof( Enemies[i].Viewcone ), 1, fp );
		fread( &Enemies[i].PickupHeld, sizeof( Enemies[i].PickupHeld ), 1, fp );
		fread( &Enemies[i].TargetingPos, sizeof( Enemies[i].TargetingPos ), 1, fp );
		fread( &Enemies[i].TargetingDir, sizeof( Enemies[i].TargetingDir ), 1, fp );

		fread( &Enemies[i].MinePos, sizeof( Enemies[i].MinePos ), 1, fp );
		fread( &Enemies[i].MineDistance, sizeof( Enemies[i].MineDistance ), 1, fp );
		fread( &Enemies[i].MineNum, sizeof( Enemies[i].MineNum ), 1, fp );

		fread( &Enemies[i].AvoidTimer, sizeof( Enemies[i].AvoidTimer), 1, fp );
		fread( &Enemies[i].AvoidType, sizeof( Enemies[i].AvoidType ), 1, fp );

		fread( &Enemies[i].Status, sizeof( Enemies[i].Status ), 1, fp );
		fread( &Enemies[i].GenType, sizeof( Enemies[i].GenType ), 1, fp );
		fread( &Enemies[i].BulletID, sizeof( Enemies[i].BulletID ), 1, fp );
		fread( &Enemies[i].ModelNum, sizeof( Enemies[i].ModelNum ), 1, fp );
		fread( &Enemies[i].ModelIndex, sizeof( Enemies[i].ModelIndex ), 1, fp );
		fread( &Enemies[i].TriggerMod, sizeof( Enemies[i].TriggerMod ), 1, fp );
		fread( &Enemies[i].Delay, sizeof( Enemies[i].Delay ), 1, fp );


		
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].PrevUsed = NULL;
		}else{
			Enemies[i].PrevUsed = &Enemies[e];
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].NextUsed = NULL;
		}else{
			Enemies[i].NextUsed = &Enemies[e];
		}



		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].PrevFree = NULL;
		}else{
			Enemies[i].PrevFree = &Enemies[e];
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].NextFree = NULL;
		}else{
			Enemies[i].NextFree = &Enemies[e];
		}

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].NextInGroup = NULL;
		}else{
			Enemies[i].NextInGroup = &Enemies[e];
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].PrevInGroup = NULL;
		}else{
			Enemies[i].PrevInGroup = &Enemies[e];
		}

		fread( &Enemies[i].PrimaryFireTimer, sizeof( Enemies[i].PrimaryFireTimer ), 1, fp );
		fread( &Enemies[i].SecondaryFireTimer, sizeof( Enemies[i].SecondaryFireTimer ), 1, fp );
		fread( &Enemies[i].ImInNodeTransition, sizeof( Enemies[i].ImInNodeTransition ), 1, fp );
		fread( &Enemies[i].PickNewNodeNow, sizeof( Enemies[i].PickNewNodeNow ), 1, fp );
		fread( &Enemies[i].DistanceToPointOnSlope, sizeof( Enemies[i].DistanceToPointOnSlope ), 1, fp );

		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].FormationLink = NULL;
		}else{
			Enemies[i].FormationLink = &Enemies[e];
		}


		fread( &Enemies[i].FormationOffset, sizeof( Enemies[i].FormationOffset ), 1, fp );


		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].SplineNode1 = NULL;
		}else if( e == SPECIALENEMYNODENUM ){
			Enemies[i].SplineNode1 = &Enemies[i].TempSplineNode;
		}else{
			Enemies[i].SplineNode1 = NodeNetworkHeader.FirstNode + e;
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].SplineNode2 = NULL;
		}else if( e == SPECIALENEMYNODENUM ){
			Enemies[i].SplineNode2 = &Enemies[i].TempSplineNode;
		}else{
			Enemies[i].SplineNode2 = NodeNetworkHeader.FirstNode + e;
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].SplineNode3 = NULL;
		}else if( e == SPECIALENEMYNODENUM ){
			Enemies[i].SplineNode3 = &Enemies[i].TempSplineNode;
		}else{
			Enemies[i].SplineNode3 = NodeNetworkHeader.FirstNode + e;
		}
		fread( &e, sizeof( e ), 1, fp );
		if( e == -1 )
		{
			Enemies[i].SplineNode4 = NULL;
		}else if( e == SPECIALENEMYNODENUM ){
			Enemies[i].SplineNode4 = &Enemies[i].TempSplineNode;
		}else{
			Enemies[i].SplineNode4 = NodeNetworkHeader.FirstNode + e;
		}

		fread( &Enemies[i].TempSplineNode, sizeof( Enemies[i].TempSplineNode ), 1, fp );

		fread( &Enemies[i].LastDamagePos, sizeof( Enemies[i].LastDamagePos ), 1, fp );
		fread( &Enemies[i].LastDamageImpactPos, sizeof( Enemies[i].LastDamageImpactPos ), 1, fp );
		fread( &Enemies[i].LastDamageDir, sizeof( Enemies[i].LastDamageDir ), 1, fp );
		fread( &Enemies[i].LastDamage, sizeof( Enemies[i].LastDamage ), 1, fp );
		fread( &Enemies[i].LastDamageSpeed, sizeof( Enemies[i].LastDamageSpeed ), 1, fp );
		fread( &Enemies[i].CloakTimer, sizeof( Enemies[i].CloakTimer ), 1, fp );
		fread( &Enemies[i].GenerationDelay, sizeof( Enemies[i].GenerationDelay ), 1, fp );
		fread( &Enemies[i].MasterGenerationDelay, sizeof( Enemies[i].MasterGenerationDelay ), 1, fp );
		fread( &Enemies[i].SmokeTime, sizeof( Enemies[i].SmokeTime ), 1, fp );
		fread( &Enemies[i].FirePosCount, sizeof( Enemies[i].FirePosCount ), 1, fp );

		if( Enemies[i].Alive || (Enemies[i].Status & ENEMY_STATUS_Enable) )
		{
			AddEnemyToGroup( &Enemies[i] , Enemies[i].Object.Group );
		}
	}
	return true;
}



/*===================================================================
	Procedure	:		Object Save...
	Input		:		FILE * fp , OBJECT * Obj
	Output		:		bool true/false
===================================================================*/
bool Object_Save( FILE * fp , OBJECT * Obj )
{
	int i;
	int e;
	NODE * Node;
	int16_t	NumChildren;

	fwrite( &Obj->Flags, sizeof( Obj->Flags ), 1, fp );
	fwrite( &Obj->Type, sizeof( Obj->Type ), 1, fp );
	e = -1;
	if( Obj->Type == OBJECT_TYPE_ENEMY )
	{
		for( i = 0 ; i < NumInitEnemies ; i++ )
		{
			if( &Enemies[i] == Obj->ID )
			{
				e = i;
				break;
			}
		}
	}else if( Obj->Type == OBJECT_TYPE_SHIP ){
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( &Ships[i] == Obj->ID )
			{
				e = i;
				break;
			}
		}
	}
	else
	{
		DebugPrintf( "Object_Save() Unknown ID\n" );
	}
	fwrite( &e, sizeof( e ), 1, fp );
	fwrite( &Obj->AI_Mode, sizeof( Obj->AI_Mode ), 1, fp );
	fwrite( &Obj->Mode, sizeof( Obj->Mode ), 1, fp );
	fwrite( &Obj->ControlType, sizeof( Obj->ControlType ), 1, fp );

	fwrite( &Obj->Pos, sizeof( Obj->Pos ), 1, fp );
	fwrite( &Obj->Quat, sizeof( Obj->Quat ), 1, fp );
	fwrite( &Obj->Bank, sizeof( Obj->Bank ), 1, fp );
	fwrite( &Obj->Mat, sizeof( Obj->Mat ), 1, fp );
	fwrite( &Obj->FinalMat, sizeof( Obj->FinalMat ), 1, fp );
	fwrite( &Obj->FinalQuat, sizeof( Obj->FinalQuat ), 1, fp );

//	fwrite( &Obj->DirVector, sizeof( Obj->DirVector ), 1, fp );
//	fwrite( &Obj->UpVector, sizeof( Obj->UpVector ), 1, fp );
//	fwrite( &Obj->LeftVector, sizeof( Obj->LeftVector ), 1, fp );
	fwrite( &Obj->Group, sizeof( Obj->Group ), 1, fp );
	fwrite( &Obj->Speed, sizeof( Obj->Speed ), 1, fp );
	fwrite( &Obj->Angle, sizeof( Obj->Angle ), 1, fp );
	fwrite( &Obj->CruiseControl, sizeof( Obj->CruiseControl ), 1, fp );
	fwrite( &Obj->Shield, sizeof( Obj->Shield ), 1, fp );
	fwrite( &Obj->Hull, sizeof( Obj->Hull ), 1, fp );
	fwrite( &Obj->Energy, sizeof( Obj->Energy ), 1, fp );
	fwrite( &Obj->PowerLevel, sizeof( Obj->PowerLevel ), 1, fp );
	fwrite( &Obj->ExternalForce, sizeof( Obj->ExternalForce ), 1, fp );
	fwrite( &Obj->InternalForce, sizeof( Obj->InternalForce ), 1, fp );
	fwrite( &Obj->RotationForce, sizeof( Obj->RotationForce ), 1, fp );
	fwrite( &Obj->Autolevel, sizeof( Obj->Autolevel ), 1, fp );
	fwrite( &Obj->BobCount, sizeof( Obj->BobCount ), 1, fp );
	fwrite( &Obj->LastBob, sizeof( Obj->LastBob ), 1, fp );
	fwrite( &Obj->light, sizeof( Obj->light ), 1, fp );
	fwrite( &Obj->Noise, sizeof( Obj->Noise ), 1, fp );
	fwrite( &Obj->NodeNetwork, sizeof( Obj->NodeNetwork ), 1, fp );


	e = -1;
	if( Node = Obj->NearestNode )
	{
		e = Node->NodeNum;
	}
	fwrite( &e, sizeof( e ), 1, fp );
	e = -1;
	if( Node = Obj->LastNearestNode )
	{
		e = Node->NodeNum;
	}
	fwrite( &e, sizeof( e ), 1, fp );
	
	fwrite( &Obj->HowManyGuns, sizeof( Obj->HowManyGuns ), 1, fp );

	if( Obj->HowManyGuns )
	{
		if( !Gun_Save( Obj->HowManyGuns , Obj->FirstGun , fp ) )
			return false;
	}

	fwrite( &Obj->Time, sizeof( Obj->Time ), 1, fp );
	fwrite( &Obj->OverallTime, sizeof( Obj->OverallTime ), 1, fp );
	fwrite( &Obj->AnimSpeed, sizeof( Obj->AnimSpeed ), 1, fp );

	fwrite( &Obj->TopLeft, sizeof( Obj->TopLeft ), 1, fp );
	fwrite( &Obj->BottomRight, sizeof( Obj->BottomRight ), 1, fp );

	fwrite( &Obj->CurAnimSeq, sizeof( Obj->CurAnimSeq ), 1, fp );
	fwrite( &Obj->Animating, sizeof( Obj->Animating ), 1, fp );

	if( Obj->Components )
	{
		NumChildren = 1;
		fwrite( &NumChildren, sizeof( NumChildren ), 1, fp );
		fp = SaveAllCompObj( fp, Obj->Components, 1 );
	}
	else
	{
		NumChildren = 0;
		fwrite( &NumChildren, sizeof( NumChildren ), 1, fp );
	}

	fwrite( &Obj->Red, sizeof( Obj->Red ), 1, fp );
	fwrite( &Obj->Green, sizeof( Obj->Green ), 1, fp );
	fwrite( &Obj->Blue, sizeof( Obj->Blue ), 1, fp );
	return true;
}

/*===================================================================
	Procedure	:		Gun Save...
	Input		:		BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp
	Output		:		bool true/false
===================================================================*/
bool Gun_Save( BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp )
{
	int i;

	for( i = 0 ; i < HowManyGuns ; i++ )
	{
		fwrite( &GO->Type, sizeof( GO->Type ), 1, fp );
		fwrite( &GO->AIMoveFlags, sizeof( GO->AIMoveFlags ), 1, fp );
		fwrite( &GO->AI_Angle, sizeof( GO->AI_Angle ), 1, fp );
		fwrite( &GO->Mat, sizeof( GO->Mat ), 1, fp );
		fwrite( &GO->Dir, sizeof( GO->Dir ), 1, fp );
		fwrite( &GO->FirePos, sizeof( GO->FirePos ), 1, fp );
		fwrite( &GO->FirePosCount, sizeof( GO->FirePosCount ), 1, fp );
		fwrite( &GO->Angle, sizeof( GO->Angle ), 1, fp );
		fwrite( &GO->BurstTime, sizeof( GO->BurstTime ), 1, fp );
		fwrite( &GO->BurstCount, sizeof( GO->BurstCount ), 1, fp );
		fwrite( &GO->ReloadTime, sizeof( GO->ReloadTime ), 1, fp );
		fwrite( &GO->BurstStartSign, sizeof( GO->BurstStartSign ), 1, fp );
		fwrite( &GO->GunNum, sizeof( GO->GunNum ), 1, fp );
		GO++;
	}
	return true;
}
/*===================================================================
	Procedure	:		Object Load...
	Input		:		FILE * fp , OBJECT * Obj
	Output		:		bool true/false
===================================================================*/
bool Object_Load( FILE * fp , OBJECT * Obj )
{
	int e;
	int16_t	NumChildren;

	fread( &Obj->Flags, sizeof( Obj->Flags ), 1, fp );
	fread( &Obj->Type, sizeof( Obj->Type ), 1, fp );

	fread( &e, sizeof( e ), 1, fp );
	if( Obj->Type == OBJECT_TYPE_ENEMY )
	{
		Obj->ID = &Enemies[e];
	}else if( Obj->Type == OBJECT_TYPE_SHIP ){

		Obj->ID = &Ships[e];
	}else
	{
		DebugPrintf( "LoadShips() Unknown Type %d, ID %d\n", Obj->Type, e );
		return false;
	}
	fread( &Obj->AI_Mode, sizeof( Obj->AI_Mode ), 1, fp );
	fread( &Obj->Mode, sizeof( Obj->Mode ), 1, fp );
	fread( &Obj->ControlType, sizeof( Obj->ControlType ), 1, fp );

	fread( &Obj->Pos, sizeof( Obj->Pos ), 1, fp );
	fread( &Obj->Quat, sizeof( Obj->Quat ), 1, fp );
	fread( &Obj->Bank, sizeof( Obj->Bank ), 1, fp );
	fread( &Obj->Mat, sizeof( Obj->Mat ), 1, fp );
	fread( &Obj->FinalMat, sizeof( Obj->FinalMat ), 1, fp );
	fread( &Obj->FinalQuat, sizeof( Obj->FinalQuat ), 1, fp );

	MatrixTranspose( &Obj->Mat, &Obj->InvMat );
	MatrixTranspose( &Obj->FinalMat, &Obj->FinalInvMat );

//	fread( &Obj->DirVector, sizeof( Obj->DirVector ), 1, fp );
//	fread( &Obj->UpVector, sizeof( Obj->UpVector ), 1, fp );
//	fread( &Obj->LeftVector, sizeof( Obj->LeftVector ), 1, fp );
	fread( &Obj->Group, sizeof( Obj->Group ), 1, fp );
	fread( &Obj->Speed, sizeof( Obj->Speed ), 1, fp );
	fread( &Obj->Angle, sizeof( Obj->Angle ), 1, fp );
	fread( &Obj->CruiseControl, sizeof( Obj->CruiseControl ), 1, fp );
	fread( &Obj->Shield, sizeof( Obj->Shield ), 1, fp );
	fread( &Obj->Hull, sizeof( Obj->Hull ), 1, fp );
	fread( &Obj->Energy, sizeof( Obj->Energy ), 1, fp );
	fread( &Obj->PowerLevel, sizeof( Obj->PowerLevel ), 1, fp );
	fread( &Obj->ExternalForce, sizeof( Obj->ExternalForce ), 1, fp );
	fread( &Obj->InternalForce, sizeof( Obj->InternalForce ), 1, fp );
	fread( &Obj->RotationForce, sizeof( Obj->RotationForce ), 1, fp );
	fread( &Obj->Autolevel, sizeof( Obj->Autolevel ), 1, fp );
	fread( &Obj->BobCount, sizeof( Obj->BobCount ), 1, fp );
	fread( &Obj->LastBob, sizeof( Obj->LastBob ), 1, fp );
	fread( &Obj->light, sizeof( Obj->light ), 1, fp );
	fread( &Obj->Noise, sizeof( Obj->Noise ), 1, fp );
	fread( &Obj->NodeNetwork, sizeof( Obj->NodeNetwork ), 1, fp );


	fread( &e, sizeof( e ), 1, fp );
	if( e == -1 )
	{
		Obj->NearestNode = NULL;
	}else{
		Obj->NearestNode = NodeNetworkHeader.FirstNode + e;
	}
	fread( &e, sizeof( e ), 1, fp );
	if( e == -1 )
	{
		Obj->LastNearestNode = NULL;
	}else{
		Obj->LastNearestNode = NodeNetworkHeader.FirstNode + e;
	}

	fread( &Obj->HowManyGuns, sizeof( Obj->HowManyGuns ), 1, fp );

	if( Obj->HowManyGuns )
	{
		if( !Gun_Load( Obj->HowManyGuns , Obj->FirstGun , fp ) )
		{
			DebugPrintf( "LoadShips() GunLoad Failed\n" );
			return false;
		}
	}

	fread( &Obj->Time, sizeof( Obj->Time ), 1, fp );
	fread( &Obj->OverallTime, sizeof( Obj->OverallTime ), 1, fp );
	fread( &Obj->AnimSpeed, sizeof( Obj->AnimSpeed ), 1, fp );

	fread( &Obj->TopLeft, sizeof( Obj->TopLeft ), 1, fp );
	fread( &Obj->BottomRight, sizeof( Obj->BottomRight ), 1, fp );

	fread( &Obj->CurAnimSeq, sizeof( Obj->CurAnimSeq ), 1, fp );
	fread( &Obj->Animating, sizeof( Obj->Animating ), 1, fp );

	fread( &NumChildren, sizeof( NumChildren ), 1, fp );

	if( NumChildren )
	{
		fp = LoadAllCompObj( fp, Obj->Components, 1 );
	}
	else
	{
		if( Obj->Components && NumChildren )
		{
			FreeCompObjChildren( Obj->Components, 1 );
			free( Obj->Components );
			Obj->Components = NULL;
		}
	}

	fread( &Obj->Red, sizeof( Obj->Red ), 1, fp );
	fread( &Obj->Green, sizeof( Obj->Green ), 1, fp );
	fread( &Obj->Blue, sizeof( Obj->Blue ), 1, fp );
	return true;
}

/*===================================================================
	Procedure	:		Gun Load...
	Input		:		BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp
	Output		:		bool true/false
===================================================================*/
bool Gun_Load( BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp )
{
	int i;

	for( i = 0 ; i < HowManyGuns ; i++ )
	{
		fread( &GO->Type, sizeof( GO->Type ), 1, fp );
		fread( &GO->AIMoveFlags, sizeof( GO->AIMoveFlags ), 1, fp );
		fread( &GO->AI_Angle, sizeof( GO->AI_Angle ), 1, fp );
		fread( &GO->Mat, sizeof( GO->Mat ), 1, fp );
		MatrixTranspose( &GO->Mat, &GO->InvMat );
		fread( &GO->Dir, sizeof( GO->Dir ), 1, fp );
		fread( &GO->FirePos, sizeof( GO->FirePos ), 1, fp );
		fread( &GO->FirePosCount, sizeof( GO->FirePosCount ), 1, fp );
		fread( &GO->Angle, sizeof( GO->Angle ), 1, fp );
		fread( &GO->BurstTime, sizeof( GO->BurstTime ), 1, fp );
		fread( &GO->BurstCount, sizeof( GO->BurstCount ), 1, fp );
		fread( &GO->ReloadTime, sizeof( GO->ReloadTime ), 1, fp );
		fread( &GO->BurstStartSign, sizeof( GO->BurstStartSign ), 1, fp );
		fread( &GO->GunNum, sizeof( GO->GunNum ), 1, fp );
		GO++;
	}
	return true;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif













