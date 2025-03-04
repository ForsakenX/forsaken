/*===================================================================
		Include File...	
===================================================================*/
#include <stdio.h>
#include "main.h"

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "config.h"
#include "controls.h"
#include "ships.h"
#include "triggers.h"
#include "pickups.h"
#include "primary.h"
#include "secondary.h"
#include "mxaload.h"
#include "lights.h"
#include "sphere.h"
#include "title.h"
#include "trigarea.h"
#include "node.h"
#include "models.h"
#include "enemies.h"
#include "extforce.h"
#include "teleport.h"
#include "water.h"
#include "spotfx.h"
#include "screenpolys.h"
#include "restart.h"
#include "goal.h"
#include "visi.h"
#include "text.h"
#include "local.h"
#include "util.h"
#include "timer.h"

//#undef MULTI_RAY_COLLISION
//#define MULTI_RAY_SLIDE
#undef MULTI_RAY_SLIDE

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif


#define	RGENINVULTIME 60.0F

#define FRAMES_PER_SEC	(60.0F)

#define BOB_XSIZE		(30.0F * GLOBAL_SCALE)
#define BOB_YSIZE		(10.0F * GLOBAL_SCALE)
#define BOB_XPERIOD		(2.0F * FRAMES_PER_SEC)
#define BOB_YPERIOD		(1.0F * FRAMES_PER_SEC)
#define BOB_XFREQ		(2.0F * PI / BOB_XPERIOD)
#define BOB_YFREQ		(2.0F * PI / BOB_YPERIOD)


bool ObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );
bool ObjectCollideOnly( OBJECT *Obj, VECTOR *Move_Off, float radius, VECTOR *Target_Off, BGOBJECT **BGObject );
bool Ship2ShipCollide( u_int16_t i , VECTOR * Move_Off );
bool CarryonDeathMove( GLOBALSHIP * ShipPnt,BYTE i);

bool	IsStartPosVacant( int16_t i , u_int16_t startpos );
void SpecialDestroyGame( void );
void MultiSfxHandle( void );

/*===================================================================
		Externals ...
===================================================================*/
extern SLIDER BikeCompSpeechSlider;
extern bool SeriousError;

extern float LevelTimeTaken;
extern	bool			CaptureTheFlag;
extern	bool			CTF;
extern	bool			BountyHunt;
extern bool	NeedFlagAtHome;
extern int8_t TeamFlagPickup[ MAX_TEAMS ];
extern u_int16_t	FlashScreenPoly;

bool TeamFlagAtHome[ MAX_TEAMS ];
u_int32_t TeamFlagMask[ MAX_TEAMS ] = {
	SHIP_CarryingFlag1,
	SHIP_CarryingFlag2,
	SHIP_CarryingFlag3,
	SHIP_CarryingFlag4,
};

extern int GoalScore;
extern int16_t PickupsGot[ MAXPICKUPTYPES ];
extern int FlagsToGenerate;
extern char *TeamName[ MAX_TEAMS ];
extern bool	bSoundEnabled;
extern	bool ShowNode;
extern	int16_t	SelectedBike;
extern u_int16_t	num_start_positions;
extern u_int16_t	last_start_position;
extern GAMESTARTPOS		StartPositions[MAXSTARTPOSITIONS];			// pos and group info...
extern	int16_t					Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16_t					Lives;
extern int  no_collision;
extern	XLIGHT	XLights[MAXXLIGHTS];
extern	bool	flush_input;
extern	MATRIX TempMatrix;
extern USERCONFIG *player_config;
extern char MyName[];
extern char NickName[];
extern	MLOADHEADER ModelHeaders[MAXMODELHEADERS];
extern	MXALOADHEADER MxaModelHeaders[MAXMXAMODELHEADERS];
extern	bool	DebugInfo;
extern	bool	GodMode;
extern MLOADHEADER Mloadheader;
extern MCLOADHEADER MCloadheader;
extern MCLOADHEADER MCloadheadert0;
extern	BYTE                    WhoIAm;
extern	RENDERMATRIX view;
extern	RENDERMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	VECTOR	SlideRight;
extern	VECTOR	SlideUp;
extern	VECTOR	Forward;
extern	float	real_framelag;
extern	float	framelag;
extern	float	Oldframelag;  
extern	float	Demoframelag;  
extern	SLIDER	DemoEyesSelect;

#ifdef DEMO_SUPPORT
extern	LONGLONG	GameElapsedTime;
#endif

extern	PICKUP	Pickups[ MAXPICKUPS ];
extern	u_int16_t	FirstPickupUsed;
extern	float	PickupMessageCount;
extern	int16_t	PickupMessageIndex;
extern	bool	RemoteCameraActive;
extern	u_int16_t	RandomStartPosModify;
extern	bool	DrawPanel;
extern	bool	OldDrawPanel;
extern	u_int16_t IsGroupVisible[MAXGROUPS];
extern	int16_t	NumStealths;
extern	int16_t	NumInvuls;
extern	float	GeneralAmmo;
extern	float	PowerLevel;

extern	BYTE	GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
											// this tells the drones what status the host thinks hes in..

extern	int		outside_group;
extern	bool	PickupInvulnerability;

extern	int16_t	BikeModels[ MAXBIKETYPES ];

extern	bool	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	int		CrystalsFound;
extern	int16_t	NumGoldBars;
extern	u_int16_t	FirstStartPositionInGroup[MAXGROUPS];
extern	bool		JustGenerated;
extern	bool		JustPickedUpShield;

// watch mode select player (Title.c)
extern SLIDER WatchPlayerSelect;

// statistics updates (stats.c)
extern int UpdateBonusStats(int Player, int Points);
extern int GetTotalKills(int Player);

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;
extern int PlayerMessageColour;

/*===================================================================
		Bike Modifiers.....
===================================================================*/
float	MaxMoveSpeed	=	MAXMOVESPEED;
float	MoveAccell		=	MOVEACCELL;		
float	MoveDecell		=	MOVEDECELL;		
float	MaxTurboSpeed   =	MAXTURBOSPEED;   
float	TurboAccell		=	TURBOACCELL;		
float	TurboDecell		=	TURBODECELL;		
float	MaxTurnSpeed	=	MAXTURNSPEED;	
float	TurnAccell		=	TURNACCELL;		
float	TurnDecell		=	TURNDECELL;		
float	MaxRollSpeed	=	MAXROLLSPEED;	
float	RollAccell		=	ROLLACCELL;		
float	RollDecell		=	ROLLDECELL;		
float	MaxBankAngle    =	MAXBANKANGLE;    
float	BankAccell		=	BANKACCELL;		
float	BankDecell		=	BANKDECELL;		
float	Start_Shield	=	START_SHIELD;
float	Start_Hull		=	START_HULL;
float	AutoLevel		=	1.0F;

#define	BikeMod  (2.0F)

BIKEMOD	BikeMods[MAXBIKETYPES+3] = {
	{	// Default
		MAXMOVESPEED, 
		MOVEACCELL,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED,
		TURNACCELL,	
		TURNDECELL,	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,
		START_SHIELD,
		START_HULL,
		1.0F
	},
	{	// In The Water...
		MAXMOVESPEED * 0.75F, 
		MOVEACCELL * 0.50F,	
		MOVEDECELL * 0.5F,	
		MAXTURBOSPEED * 0.75F,
		TURBOACCELL * 0.5F,
		TURBODECELL * 0.5F,
		MAXTURNSPEED * 0.5F,
		TURNACCELL * 0.5F,	
		TURNDECELL * 0.5F,	
		MAXROLLSPEED * 0.5F,
		ROLLACCELL * 0.5F,	
		ROLLDECELL * 0.5F,	
		MAXBANKANGLE, 
		BANKACCELL * 1.0F,	
		BANKDECELL * 1.0F,
		START_SHIELD,
		START_HULL,
		0.5F
	},
	{	// Lokasenna
		MAXMOVESPEED * (1.0F - ( 0.01F * BikeMod )), 
		MOVEACCELL *1.0F,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * 1.0F,
		START_HULL *1.0F,
		1.0F
	},
	{	// Beard
		MAXMOVESPEED * 1.0F, 
		MOVEACCELL * 1.0F,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * 1.0F,
		TURNACCELL * 1.0F,	
		TURNDECELL * 1.0F,	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * 1.0F,
		START_HULL * 1.0F,
		1.0F
	},
	{	// L.A. Jay
		MAXMOVESPEED * (1.0F + ( 0.03F * BikeMod )), 
		MOVEACCELL* (1.0F+ ( 0.03F * BikeMod )),	
		MOVEDECELL* (1.0F+ ( 0.03F * BikeMod )),	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED* (1.0F+ ( 0.03F * BikeMod )),
		TURNACCELL* (1.0F+ ( 0.03F * BikeMod )),	
		TURNDECELL* (1.0F+ ( 0.03F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F - ( 0.02F * BikeMod )),
		START_HULL * (1.0F - ( 0.03F * BikeMod )),
		1.0F
	},
	{	// Ex-Cop
		MAXMOVESPEED * (1.0F + ( 0.01F * BikeMod )), 
		MOVEACCELL * (1.0F - ( 0.01F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.01F * BikeMod )),
		START_HULL * (1.0F - ( 0.02F * BikeMod )),
		1.0F
	},
	{	// Rex Hardy
		MAXMOVESPEED * (1.0F - ( 0.03F * BikeMod )), 
		MOVEACCELL * (1.0F - ( 0.03F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F - ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F - ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F - ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.05F * BikeMod )),
		START_HULL * 1.0F,
		1.0F
	},
	{	// Foetoid
		MAXMOVESPEED, 
		MOVEACCELL * (1.0F - ( 0.01F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F - ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F - ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F - ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.02F * BikeMod )),
		START_HULL * 1.0F,
		1.0F
	},
	{	// Nim Soo
		MAXMOVESPEED * 1.0F, 
		MOVEACCELL * 1.0F,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F - ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F - ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F - ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F+ ( 0.01F * BikeMod )),
		START_HULL * 1.0F,
		1.0F
	},
	{	// Nutta
		MAXMOVESPEED * (1.0F + ( 0.05F * BikeMod )), 
		MOVEACCELL * (1.0F + ( 0.01F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F - ( 0.04F * BikeMod )),
		START_HULL * (1.0F - ( 0.03F * BikeMod )),
		1.0F
	},
	{	// Sceptre
		MAXMOVESPEED * (1.0F - ( 0.03F * BikeMod )), 
		MOVEACCELL * (1.0F - ( 0.03F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F - ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F - ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F - ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.04F * BikeMod )),
		START_HULL * 1.0F,
		1.0F
	},
	{	// Jo
		MAXMOVESPEED * 1.0F, 
		MOVEACCELL * (1.0F + ( 0.04F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F - ( 0.02F * BikeMod )),
		START_HULL * (1.0F - ( 0.02F * BikeMod )),
		1.0F
	},
	{	// Cuvel Clark
		MAXMOVESPEED * (1.0F + ( 0.05F * BikeMod )), 
		MOVEACCELL * (1.0F + ( 0.05F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.05F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.05F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.05F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F - ( 0.05F * BikeMod )),
		START_HULL * (1.0F - ( 0.05F * BikeMod )),
		1.0F
	},
	{	// HK-5
		MAXMOVESPEED * (1.0F - ( 0.02F * BikeMod )), 
		MOVEACCELL * (1.0F - ( 0.02F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F - ( 0.02F * BikeMod )),
		TURNACCELL * (1.0F - ( 0.02F * BikeMod )),	
		TURNDECELL * (1.0F - ( 0.02F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.03F * BikeMod )),
		START_HULL * (1.0F - ( 0.01F * BikeMod )),
		1.0F
	},
	{	// Nubia
		MAXMOVESPEED * (1.0F + ( 0.01F * BikeMod )), 
		MOVEACCELL * (1.0F + ( 0.03F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.01F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.01F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.01F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F - ( 0.02F * BikeMod )),
		START_HULL * (1.0F - ( 0.01F * BikeMod )),
		1.0F
	},
	{	// Mofisto
		MAXMOVESPEED * (1.0F - ( 0.02F * BikeMod )), 
		MOVEACCELL * (1.0F - ( 0.02F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * 1.0F,
		TURNACCELL * 1.0F,	
		TURNDECELL * 1.0F,	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * 1.0F,
		START_HULL * 1.0F,
		1.0F
	},
	{	// Cerbero
		MAXMOVESPEED, 
		MOVEACCELL,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED,
		TURNACCELL,	
		TURNDECELL,	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD,
		START_HULL,
		1.0F
	},
	{	// Slick
		MAXMOVESPEED, 
		MOVEACCELL * (1.0F + ( 0.01F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.03F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.03F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.03F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.01F * BikeMod )),
		START_HULL * (1.0F - ( 0.01F * BikeMod )),
		1.0F
	},
	{	// FlyGirl
		MAXMOVESPEED, 
		MOVEACCELL * (1.0F + ( 0.01F * BikeMod )),	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED * (1.0F + ( 0.03F * BikeMod )),
		TURNACCELL * (1.0F + ( 0.03F * BikeMod )),	
		TURNDECELL * (1.0F + ( 0.03F * BikeMod )),	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,	
		START_SHIELD * (1.0F + ( 0.01F * BikeMod )),
		START_HULL * (1.0F - ( 0.01F * BikeMod )),
		1.0F
	},
	{	// Slow Default
//
// Speed Cap ?
// Who ordered that ?
//		MAXMOVESPEED * 0.9F, 
//
		MAXMOVESPEED,
		MOVEACCELL,	
		MOVEDECELL,	
		MAXTURBOSPEED,
		TURBOACCELL,
		TURBODECELL,
		MAXTURNSPEED,
		TURNACCELL,	
		TURNDECELL,	
		MAXROLLSPEED,
		ROLLACCELL,	
		ROLLDECELL,	
		MAXBANKANGLE, 
		BANKACCELL,	
		BANKDECELL,
		START_SHIELD,
		START_HULL,
		1.0F
	},
};


/*===================================================================
		Detail Level Globals ...
===================================================================*/
#define	MAXBIKEDETAIL	4.0F
#define	MAXCOLLISIONS	3


int		outside_map = 0;
int		HullHit = 0;
int		ShieldHit = 0;
/*===================================================================
		Globals ...
===================================================================*/
u_int32_t UnderwaterSfxID;
float BountyTime = 0.0F;
extern int BountyBonusInterval;

bool ShieldCritical = false;
bool HullCritical = false;

RENDERMATRIX  TempWorld;	

float	WhiteOut = 0.0F;
float	RandomBubbleTimer = 0.0F;
DWORD max_time;
u_int16_t	NumCollides;

SHIPCONTROL control;
float	NitroFuel = 0.0F;
float	NitroFuelUsed = 0.0F;
extern	CAMERA			CurrentCamera;
extern	bool	PlayDemo;
extern	bool	PauseDemo;
float AutoLevelRot( u_int16_t ship, float autolevel_rate );
void BobShip( u_int16_t ship, VECTOR *bob );
void	UpdateStartPos( void );
void CreateSteam( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
float	SteamTime = 0.0F;
bool	IsStartPosVacantMutualyVisibleGroup( int16_t i , u_int16_t startpos );
bool	IsStartPosVacantVisibleGroup( int16_t i , u_int16_t startpos );
bool ObjectCollideNoBGObject( OBJECT *Obj, VECTOR *Move_Off, float radius );
bool SwitchedToWatchMode =false;

bool	RearCameraDisable = false;

/*===================================================================
	Function		:			Ship Control Mode Jump Table
===================================================================*/
void (* ModeControl[ ])( GLOBALSHIP * ShipPnt , BYTE i ) = {
		ShipMode0,
		ShipMode1,
		ShipMode2,
		RemoteCameraMode3,			// needed for demo playback!!!!!!!!!
		ShipMode4,
		WatchMode5,
};
/*===================================================================
	Function		:			RemoteCamera Control Mode Jump Table
===================================================================*/
void (* RemoteCameraModeControl[ ])( GLOBALSHIP * ShipPnt , BYTE i ) = {
		RemoteCameraMode0,
		RemoteCameraMode1,
		RemoteCameraMode1,		// there is no remote camera mode 2 so just use mode 1...
		RemoteCameraMode3
};

/*===================================================================
		Init All Ships ...
===================================================================*/

bool SetUpShips()
{
	sprintf( NickName, "%s", player_config->name );

	sprintf( MyName, "%s's game", player_config->name );

	return true;
}

#ifdef DEMO_SUPPORT
static	LONGLONG	TempTime;
static	LONGLONG	TempTime2;
#endif

static	float	Interp;	
extern  BYTE          MyGameStatus;

bool ProcessShips()
{
	BYTE	i;
#if 0
	float	Impact_Off_Length;
	VECTOR	Impact_Off;
	float	M;
#endif
	VECTOR	ImpactPoint;
	float	Speed;
	VECTOR	Move_Off;	
#ifndef MULTI_RAY_COLLISION
	VECTOR	Pos_New;	
	NORMAL	FaceNormal;
#endif
	VECTOR	Move_Dir;
	QUAT	StepQuat;	
	VECTOR	Bob;
	VECTOR	StartPos;
	BGOBJECT * BGObject;
	u_int16_t	NumToDo;
	u_int16_t	OldGroup;
	VECTOR	ExtForce;
	bool HasBeenExternal;
	float	ShieldModifier;
	u_int32_t	OldInWater;
	bool	Object2Object;
	NODE * NodePnt;
	OBJECT * ShipObjPnt;
	GLOBALSHIP * ShipPnt;
	int		goalcheck;
	bool OldWaterOneshot;

	UpdateStartPos();
	MultiSfxHandle();

	RearCameraDisable = false;

#if 0
	for( i = 0 ; i < 10 ; i ++ )
	{
		DebugPrintf( "pow( 0.9 ,framelag %f ) = %f \n", 1.0F - (i / 10.0F) , (float) pow( 0.9 , 1.0F - (i / 10.0F) ) );
	}
#endif

	if( PlayDemo )
		NumToDo = MAX_PLAYERS+1;
	else
		NumToDo = MAX_PLAYERS;

	// for every player in the game
	for( i=0;i<NumToDo;i++)
	{
		ShipObjPnt = &Ships[i].Object;
		ShipPnt = &Ships[i];
		Ships[i].Object.ID = ShipPnt;

		if( ShipPnt->enable )
		{
			if( ShipPnt->Invul )
			{
				if( !PlayDemo )
				{
					if( Current_Camera_View != i )
					{
						if( i != WhoIAm )
						{
							CreateInvEffect( i, 5, 255, 64, 64 );
						}
						else
						{
							if( Current_Camera_View != WhoIAm )
							{
								CreateInvEffect( i, 5, 255, 64, 64 );
							}
						}
					}
				}
				else
				{
					if( Current_Camera_View != i )
					{
						CreateInvEffect( i, 5, 255, 64, 64 );
					}
				}
			}

			DoDamagedEffects( i );							// Do Effects when damaged



			// Special Noise Distance Modifier...
			if( ShipObjPnt->Noise > 0.0F )
			{
				ShipObjPnt->Noise -= NOISEFALLOFFFACTOR * framelag;
			}else{
				ShipObjPnt->Noise = 0.0F;
			}


			StartPos = ShipObjPnt->Pos;
			OldGroup = ShipObjPnt->Group;

				if(
					ShipPnt->headlights &&                                    // has headlights on
					ShipObjPnt->Mode != LIMBO_MODE &&                         // not dead
					(ShipObjPnt->Mode != DEATH_MODE || Random_Range(2))       // blink 50% while dieing
				)
				{
					if( ShipObjPnt->light == (u_int16_t) -1 )
					{
						ShipObjPnt->light = FindFreeXLight();
					}
					if( ShipObjPnt->light != (u_int16_t) -1 )
					{
						ShipObjPnt->Flags |= SHIP_Light;

						XLights[ShipObjPnt->light].Size = (4096.0F+2048.0F)*GLOBAL_SCALE;
						XLights[ShipObjPnt->light].CosArc = (float)cos(D2R(35));
						
						ApplyMatrix( &ShipObjPnt->FinalMat, &Forward, &XLights[ShipObjPnt->light].Dir );			/* Calc Direction Vector */
						XLights[ShipObjPnt->light].r = 192.0F;
						XLights[ShipObjPnt->light].g = 192.0F;
						XLights[ShipObjPnt->light].b = 192.0F;
						XLights[ShipObjPnt->light].Type = SPOT_LIGHT;
						XLights[ShipObjPnt->light].Pos = ShipObjPnt->Pos;
						XLights[ShipObjPnt->light].Group = ShipObjPnt->Group;
					}
				}else{
					ShipObjPnt->Flags &= ~SHIP_Light;
					if( ShipObjPnt->light != (u_int16_t) -1 )
					{
						KillUsedXLight( ShipObjPnt->light );
						ShipObjPnt->light = (u_int16_t) -1;
					}
				}

			// processing you
			if( WhoIAm == i)
			{

				ShipObjPnt->NodeNetwork = 1;
				if( !ShipObjPnt->NearestNode )
				{
					FindNearestNode( &ShipPnt->Object );
				}else{
					UpdateNearestNodeSpecial( &ShipPnt->Object );
				}
				NodePnt = ShipObjPnt->NearestNode;
				if( NodePnt && ShowNode)
				{
					SteamTime -= framelag;
					if( SteamTime < 0.0F )
					{
						CreateSteam( &NodePnt->Pos, &SlideUp, NodePnt->Group );
						SteamTime = 10.0F;
					}
				}
				
				
				if( !GodMode )
				{
					if( ShipPnt->InvulTimer != 0.0F )
					{
						ShipPnt->InvulTimer -= framelag;
						if( ShipPnt->InvulTimer < 0.0F )
						{
							ShipPnt->InvulTimer = 0.0F;
						}
						else
						{
							ShipPnt->Invul = true;
						}
					}
					else
					{
						ShipPnt->Invul = false;
						if( PickupInvulnerability )
						{
							NumInvuls++;
							PickupInvulnerability = false;
						}
					}
					if ( BountyHunt && ShipObjPnt->Flags & SHIP_CarryingBounty )
					{
						BountyTime += framelag / 60.0F;
						if ( BountyBonusInterval > 0 && BountyTime >= BountyBonusInterval )
						{
							// update bonus 4 (stats.c) -- bounty hunt points
							UpdateBonusStats(i,(int16_t) floor( BountyTime / BountyBonusInterval ));
							BountyTime = FMOD( BountyTime, BountyBonusInterval );
						}
					}
				}

				if( ShipPnt->ShakeTimer )
				{
					ShipPnt->ShakeTimer -= framelag;
					if( ShipPnt->ShakeTimer > 0.0F )
					{
						ShipPnt->ShakeDirTimer -= framelag;
						if( ShipPnt->ShakeDirTimer <= 0.0F )
						{
							ShipPnt->ShakeDirTimer = 2.0F + ( Random_Range(32) * ( 1.0F / 32.0F ) );
					
							ExtForce.x = -(ShipPnt->ShakeForce * 0.5F ) + (Random_Range(32) * ( ShipPnt->ShakeForce / 32.0F));
							ExtForce.y = -(ShipPnt->ShakeForce * 0.5F ) + (Random_Range(32) * ( ShipPnt->ShakeForce / 32.0F));
							ExtForce.z = 0.0F;
							RotateInternal( i, ExtForce.x, ExtForce.y, ExtForce.z );
							ExtForce.x *= 0.5F;
							ExtForce.y *= 0.5F;
							ForceInternalOneOff( i, &ExtForce );
						}
						ShipPnt->ShakeForce *= 0.02F * framelag;
					}
					else
					{
						ShipPnt->ShakeTimer = 0.0F;
					}
				}

				// watch mode
				if(WatchPlayerSelect.value != WhoIAm && ShipObjPnt->Mode != DEMO_MODE	) 
					ShipObjPnt->Mode = WATCH_MODE; 
		
				( * ModeControl[ ShipObjPnt->Mode ] )( ShipPnt , i );		//go off and do his thing...

				if( (i==MAX_PLAYERS) || (ShipObjPnt->Mode != DEATH_MODE && ShipObjPnt->Mode != LIMBO_MODE && ShipObjPnt->Mode != GAMEOVER_MODE) )
				{

					if( ( ShipObjPnt->Flags & SHIP_Stealth ) )
					{
						ShipPnt->StealthTime -= framelag;
						if( ShipPnt->StealthTime <= 0.0F )
						{
							PlaySfx( SFX_DeCloaking, 1.0F );
							ShipPnt->StealthTime = 0.0F;
							ShipObjPnt->Flags &= ~SHIP_Stealth;
							NumStealths++;
						}
					}

					AccellDecell( &ShipObjPnt->Angle.y , TurnDecell );
			 		AccellDecell( &ShipObjPnt->Angle.x , TurnDecell );
			 		AccellDecell( &ShipObjPnt->Angle.z , RollDecell );
					ShipObjPnt->Autolevel = AutoLevelRot( i, AutoLevel * player_config->autolevel_rate * framelag );
			 		AccellDecell( &ShipObjPnt->Bank , BankDecell );
					AccellDecell( &ShipObjPnt->Speed.x , MoveDecell );
					AccellDecell( &ShipObjPnt->Speed.y , MoveDecell );

					if ( ( control.turbo || ( !control.forward && ShipObjPnt->CruiseControl == CRUISE_NITRO ) ) && NitroFuel > 0.0F )
					{
						if( !GodMode && !( ShipObjPnt->Flags & SHIP_SuperNashram ) )
						{
							if( NitroFuel <= ( NITROLOSS * framelag ) )
							{
								NitroFuelUsed += NitroFuel;
								NitroFuel = 0.0F;
							}
							else
							{
								NitroFuel -= ( NITROLOSS * framelag );
								NitroFuelUsed += ( NITROLOSS * framelag );
							}
						}
						ShipObjPnt->Flags |= SHIP_Turbo;

						AccellDecell( &ShipObjPnt->Speed.z , TurboDecell );
					}
					else
					{
						ShipObjPnt->Flags &= ~SHIP_Turbo;
						AccellDecell( &ShipObjPnt->Speed.z , MoveDecell );
					}
				
					MakeQuat( ShipObjPnt->Angle.x * framelag, ShipObjPnt->Angle.y * framelag, ShipObjPnt->Angle.z * framelag + ShipObjPnt->Autolevel, &StepQuat );
					QuatMultiply(  &ShipObjPnt->Quat , &StepQuat , &ShipObjPnt->Quat );
					QuatMultiply(  &ShipObjPnt->RotationForce , &ShipObjPnt->Quat , &ShipObjPnt->Quat );
					QuatToMatrix( &ShipObjPnt->Quat, &ShipObjPnt->Mat );
					MakeQuat( 0.0F, 0.0F, 0.0F, &ShipObjPnt->RotationForce );
				
					// carry out movements
					BobShip( i, &Bob );
					Move_Off.x = Bob.x - ShipObjPnt->LastBob.x;
					Move_Off.y = Bob.y - ShipObjPnt->LastBob.y;
					Move_Off.z = Bob.z - ShipObjPnt->LastBob.z;
					ShipObjPnt->LastBob = Bob;
					
					if ( ShipObjPnt->Speed.x )
					{
						Speed = ShipObjPnt->Speed.x * framelag;
						ApplyMatrix( &ShipObjPnt->Mat, &SlideRight, &Move_Dir );			/* Calc Direction Vector */
				   		Move_Off.x += ( Move_Dir.x * Speed );
				   		Move_Off.y += ( Move_Dir.y * Speed );
				   		Move_Off.z += ( Move_Dir.z * Speed );
					}
					if ( ShipObjPnt->Speed.y )
					{
						Speed = ShipObjPnt->Speed.y * framelag;
						ApplyMatrix( &ShipObjPnt->Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
				   		Move_Off.x += ( Move_Dir.x * Speed );
				   		Move_Off.y += ( Move_Dir.y * Speed );
				   		Move_Off.z += ( Move_Dir.z * Speed );
					}
					if ( ShipObjPnt->Speed.z )
					{
						Speed = ShipObjPnt->Speed.z * framelag;
						ApplyMatrix( &ShipObjPnt->Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
				   		Move_Off.x += ( Move_Dir.x * Speed );
				   		Move_Off.y += ( Move_Dir.y * Speed );
				   		Move_Off.z += ( Move_Dir.z * Speed );
					}

					// apply external and internal force vectors to ship movement
					AccellDecell( &ShipObjPnt->ExternalForce.x , MoveDecell );
					AccellDecell( &ShipObjPnt->ExternalForce.y , MoveDecell );
					AccellDecell( &ShipObjPnt->ExternalForce.z , MoveDecell );

					Move_Off.x += ShipObjPnt->ExternalForce.x * framelag;
					Move_Off.y += ShipObjPnt->ExternalForce.y * framelag;
					Move_Off.z += ShipObjPnt->ExternalForce.z * framelag;

					Object2Object = false;
					if( (i != MAX_PLAYERS) && (ShipObjPnt->Mode != LIMBO_MODE) )
					{
						Object2Object |= Ship2ShipCollide( i , &Move_Off );
					}
					
					// Collide with the enemies....
					Object2Object |= Ship2EnemyCollide( i , &Move_Off );
					
					// do collisions to background
					if( !no_collision )
					{
						NumCollides = 0;
						if ( ShipObjPnt->Group != (u_int16_t) -1 )
#ifdef MULTI_RAY_COLLISION
						{
#ifdef MULTI_RAY_SLIDE
							VECTOR Target_Off;

							BGObject = NULL;
							while ( ObjectCollideOnly( &ShipPnt->Object, &Move_Off, SHIP_RADIUS, &Target_Off, &BGObject ) )
							{
								if ( BGObject )
									ChangeBGState( BGObject, OWNER_SHIP, WhoIAm, BUMP, 0.0F );
								BGObject = NULL;
								Move_Off = Target_Off;
								if ( ++NumCollides > MAXCOLLISIONS )
									break;
							}
#else
							BGObject = NULL;

							if( !Object2Object )
							{
								ObjectCollide( &ShipPnt->Object, &Move_Off, SHIP_RADIUS, &BGObject );
								if( BGObject ) ChangeBGState( BGObject, OWNER_SHIP, WhoIAm, BUMP, 0.0F );
							}
							else
							{
								ObjectCollideNoBounce( &ShipPnt->Object, &Move_Off, SHIP_RADIUS, &BGObject );
								if( BGObject ) ChangeBGState( BGObject, OWNER_SHIP, WhoIAm, BUMP, 0.0F );
							}
#endif
							outside_map = !PointInsideSkin( &ShipObjPnt->Pos, ShipObjPnt->Group );
							if ( outside_map )
							{
								if ( !DebugInfo )
								{
									// player has somehow managed to get outside the map
									// so we "teleport" him back to his last start position
									// (BiaS strikes again...!)
									InitShipStartPos( i, 0 );
									PlaySfx( SFX_Teleport, 1.0F );
								}
								else
								{
									u_int16_t g;
									
									for ( g = 0; g < Mloadheader.num_groups; g++ )
									{
										if ( PointInsideSkin( &ShipObjPnt->Pos, g ) )
										{
											ShipObjPnt->Group = g;
											AddColourMessageToQue(SystemMessageColour, "You are now in %s", Mloadheader.Group[ g ].name );
											break;
										}
									}
								}
							}
						}
#else
						{
							while ( BackgroundCollide( &MCloadheader, &Mloadheader,
														&ShipObjPnt->Pos, ShipObjPnt->Group, &Move_Off,
														&ImpactPoint , &ImpactGroup, &FaceNormal, &Pos_New, true, &BGObject ) )
							{
								if( BGObject ) ChangeBGState( BGObject, OWNER_SHIP, WhoIAm, BUMP, 0.0F );
						
								ShipObjPnt->Pos = ImpactPoint;
								ShipObjPnt->Group = ImpactGroup;
								Move_Off.x = Pos_New.x - ShipObjPnt->Pos.x; 
								Move_Off.y = Pos_New.y - ShipObjPnt->Pos.y; 
								Move_Off.z = Pos_New.z - ShipObjPnt->Pos.z;
								
								if( VectorLength( &Move_Off ) <= MoveAccell )
								{
									Move_Off.x = 0.0F; 
									Move_Off.y = 0.0F; 
									Move_Off.z = 0.0F;
									break;								
								}
								
								NumCollides += 1;
								if( NumCollides	> MAXCOLLISIONS )
									break;
							}
							outside_map = outside_group;
						}
						
//						if( InBSPGroup( 0, &ImpactPoint )	)
						{
							// the end point is actually outside...
							ShipObjPnt->Pos = ImpactPoint;
							ShipObjPnt->Group = ImpactGroup;
						}
#endif
					}
					else
					{
						ShipObjPnt->Group = MoveGroup( &Mloadheader, &ShipObjPnt->Pos, ShipObjPnt->Group, &Move_Off );
						ShipObjPnt->Pos.x += Move_Off.x;
						ShipObjPnt->Pos.y += Move_Off.y;
						ShipObjPnt->Pos.z += Move_Off.z;
					}

					ExtForce.x = 0.0F;
					ExtForce.y = 0.0F;
					ExtForce.z = 0.0F;
					ShieldModifier = 0.0F;
					HasBeenExternal = ExternalForcesAreaCheck( &ShipObjPnt->Pos , &StartPos , ShipObjPnt->Group, &ExtForce , &ShieldModifier);
					TriggerAreaPlayerCheck( &StartPos , &ShipObjPnt->Pos , ShipObjPnt->Group);
					
					if( OldGroup != ShipObjPnt->Group )
					{
						TriggerAreaPlayerCheck( &StartPos , &ShipObjPnt->Pos , OldGroup);
					}

					if ( CaptureTheFlag && ShipObjPnt->Flags & SHIP_CarryingFlag )
					{
						goalcheck = GoalCheckTeam( &StartPos, &ShipObjPnt->Pos, ShipObjPnt->Group, TeamNumber[ i ] );
						if ( goalcheck == GOAL_SCORED )
						{
							PickupsGot[ PICKUP_Flag ] = 0;
							Ships[ i ].Object.Flags &= ~SHIP_CarryingFlag;
							FlagsToGenerate++;
							// update bonus 2 (stats.c) -- flag chase scored
							UpdateBonusStats(i,GoalScore);
							AddColourMessageToQue(FlagMessageColour, TEAM_SCORED,
								TeamName[ TeamNumber[ i ] ] );

							SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ScoredWithFlag, 0);
						}
						else if ( goalcheck == GOAL_WRONG )
						{
							AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
								TeamName[ TeamNumber[ i ] ] );
						}
					}

					if ( CTF && ShipObjPnt->Flags & SHIP_CarryingFlags )
					{
						static bool IKnowINeedFlag = false;

						goalcheck = GoalCheckTeam( &StartPos, &ShipObjPnt->Pos, ShipObjPnt->Group, TeamNumber[ i ] );
						if ( goalcheck == GOAL_SCORED )
						{
							if ( !NeedFlagAtHome
								|| TeamFlagAtHome[ TeamNumber[ i ] ]
								|| ShipObjPnt->Flags & TeamFlagMask[ TeamNumber[ i ] ] )
							{
									int team;
									int score;

									score = 0;
									for ( team = 0; team < MAX_TEAMS; team++ )
									{
										if ( ShipObjPnt->Flags & TeamFlagMask[ team ] )
										{
											PickupsGot[ TeamFlagPickup[ team ] ] = 0;
											Ships[ i ].Object.Flags &= ~TeamFlagMask[ team ];
											if ( team != TeamNumber[ i ] )
											{
												score += GoalScore;
											}
											GenerateFlagAtHome( team );
										}
									}
									if ( score )
									{
										// update bonus 3 (stats.c) -- CTF scored
										UpdateBonusStats(i,score);
										AddColourMessageToQue(FlagMessageColour, TEAM_SCORED,
											TeamName[ TeamNumber[ i ] ] );
										
										SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ScoredWithFlag, 0);
									}
									else
									{
										AddColourMessageToQue(FlagMessageColour, OTHER_TEAM_FLAG_RETURNED,
											TeamName[ TeamNumber[ i ] ] );
										SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ReturnedFlag, 0);
									}
								
								IKnowINeedFlag = false;
							}
							else
							{
								if ( !IKnowINeedFlag )
								{
									AddColourMessageToQue(FlagMessageColour, YOU_NEED_FLAG,
										TeamName[ TeamNumber[ i ] ] );
									IKnowINeedFlag = true;
								}
							}
						}
						else
						{
							if ( goalcheck == GOAL_WRONG )
							{
								AddColourMessageToQue(FlagMessageColour, TAKE_FLAG_TO_GOAL,
									TeamName[ TeamNumber[ i ] ] );
							}
							IKnowINeedFlag = false;
						}
					}

					// this is where we go into and out of water
					OldInWater = ShipObjPnt->Flags & SHIP_InWater;
					ShipObjPnt->Flags &= ~SHIP_InWater;
					if( InWater( ShipObjPnt->Group , &ShipObjPnt->Pos , &ShieldModifier) )
					{
						ShipObjPnt->Flags |= SHIP_InWater;
					}
					else
					{
						// not in water, therefore stop underwater looping sfx if currently playing
						if ( UnderwaterSfxID )
						{
							StopSfx( UnderwaterSfxID );
						}
					}

					if( (ShipObjPnt->Flags&SHIP_InWater) )
					{
						SetBikeMods( 1 );
					}
					if( !(ShipObjPnt->Flags&SHIP_InWater) )
					{
						if( MyGameStatus == STATUS_SinglePlayer )
						{
							SetBikeMods( (u_int16_t) (SelectedBike+2) );
						}
						else
						{
							SetBikeMods( 0 );
						}
					}

					// make a wake in any water....
					if( WaterObjectCollide( OldGroup , &StartPos, &Move_Off, &ImpactPoint , 4 ) )
					{
						// We hit the water....
						if( (ShipObjPnt->Flags&SHIP_InWater) )
						{
							// entered the water....
							PlaySfx( SFX_Submerge , 1.0F );
							// kill off any existing water sfx ( shouldn't be needed )
							if ( UnderwaterSfxID )
							{
								StopSfx( UnderwaterSfxID );
							}
							UnderwaterSfxID = PlaySfx( SFX_UnderWaterAmb , 1.0F );
						}
						else
						{
							// exited the water....
							PlaySfx( SFX_Surface , 1.0F );
						}
					}

					if( (ShipObjPnt->Flags&SHIP_InWater) )
					{
						// random Bubble noise.....
						OldWaterOneshot = RandomBubbleTimer >= (60.0F * 5.0F );
						RandomBubbleTimer -= framelag;

						if( OldWaterOneshot && RandomBubbleTimer < (60.0F * 5.0F ) )
						{
							PlaySfx( SFX_UnderwaterOneShot , 0.5F + Random_Range_Float( 0.5F ) );
						}
						if( RandomBubbleTimer < 0.0F )
						{
							RandomBubbleTimer = (60.0F * 5.0F ) + Random_Range_Float( 60.0F * 10.0F );
							PlaySfx( SFX_Bubbles , 0.5F + Random_Range_Float( 0.5F ) );
						}

					}
					
					if( HasBeenExternal )
					{
						ForceExternalOneOff( i, &ExtForce );
					}

					if( ShieldModifier )
					{
						ShipPnt->Damage = ShieldModifier;

						// this never gets called in multiplayer?
						if( DoDamage( DONT_OVERRIDE_INVUL ) )
						{
							ShipPnt->ShipThatLastKilledMe = i;
							ShipObjPnt->Mode = DEATH_MODE;
							ShipPnt->Timer = 0.0F;
							// print up I killed Myself
							AddColourMessageToQue(KillMessageColour, YOU_KILLED_YOURSELF );
							PlaySfx( SFX_BIKECOMP_DY, 1.0F );
						}
					}


					if( TeleportsAreaCheck( &ShipObjPnt->Pos , &StartPos , OldGroup, ShipObjPnt ) )
					{
					}

					if ( framelag )
					{
						ShipPnt->Move_Off.x = Move_Off.x / framelag;
						ShipPnt->Move_Off.y = Move_Off.y / framelag;
						ShipPnt->Move_Off.z = Move_Off.z / framelag;
					}

					CheckPickup();
				}

		
			} // end of processing 'you'

			// Start of Special Stuff for other players Ship Movement..Carries on even if no new packet arrives..
			else
			{
				if( !ShipPnt->JustRecievedPacket )
				{

					StartPos = ShipObjPnt->Pos;
					if( ShipObjPnt->Mode != LIMBO_MODE && ShipObjPnt->Mode != WATCH_MODE)
					{

#ifdef DEMO_SUPPORT

						// playing demo
						if( 
							(MyGameStatus == STATUS_PlayingDemo) &&
							ShipPnt->DemoInterpolate &&
							(ShipObjPnt->Mode != GAMEOVER_MODE) 
						)
						{
							TempTime = ShipPnt->NextTime - ShipPnt->OldTime;
							TempTime2 = GameElapsedTime - ShipPnt->OldTime;

							if( TempTime <= 5 )
							{
								Interp = 0.0F;
							}
							
							Interp = (float) ( ( 1.0F / (float ) TempTime ) * (float) TempTime2);
							if( (Interp >= 0.0F) && (Interp <= 1.001F) )
							{
								Interp = -Interp;
								ShipObjPnt->Pos.x = ShipPnt->OldPos.x + ( ( ShipPnt->OldPos.x - ShipPnt->NextPos.x ) * Interp);
								ShipObjPnt->Pos.y = ShipPnt->OldPos.y + ( ( ShipPnt->OldPos.y - ShipPnt->NextPos.y ) * Interp);
								ShipObjPnt->Pos.z = ShipPnt->OldPos.z + ( ( ShipPnt->OldPos.z - ShipPnt->NextPos.z ) * Interp);
								
								Quaternion_Slerp( -Interp, &ShipPnt->OldQuat, &ShipPnt->NextQuat, &ShipObjPnt->Quat, 0 );
								
								ShipObjPnt->Bank = ShipPnt->OldBank + ( ( ShipPnt->OldBank - ShipPnt->NextBank ) * Interp);
							}
							else
							{
								Interp = 0.0F;
							}

						}
						// not playing a demo
						else

#endif
						{
								// carry out movements
								Move_Off.x = ShipPnt->Move_Off.x * framelag;
								Move_Off.y = ShipPnt->Move_Off.y * framelag;
								Move_Off.z = ShipPnt->Move_Off.z * framelag;
								ShipObjPnt->Pos.x += Move_Off.x;		
								ShipObjPnt->Pos.y += Move_Off.y;		
								ShipObjPnt->Pos.z += Move_Off.z;
								MakeQuat( ShipPnt->LastAngle.x * framelag, ShipPnt->LastAngle.y * framelag, (ShipPnt->LastAngle.z * framelag) + ShipObjPnt->Autolevel, &StepQuat );
								QuatMultiply(  &ShipObjPnt->Quat , &StepQuat , &ShipObjPnt->Quat );

								ShipPnt->PacketDelay -= framelag;
								if( ShipPnt->PacketDelay <= 0.0F )
								{
									ShipPnt->PacketDelay = 0.0F;
									AccellDecell( &ShipPnt->Move_Off.x , MoveDecell );
									AccellDecell( &ShipPnt->Move_Off.y , MoveDecell );
									AccellDecell( &ShipPnt->Move_Off.z , MoveDecell );
									AccellDecell( &ShipPnt->LastAngle.y , TurnDecell );
				 					AccellDecell( &ShipPnt->LastAngle.x , TurnDecell );
				 					AccellDecell( &ShipPnt->LastAngle.z , RollDecell );
								}
						}

						// Still need to update the group....
						Move_Off.x = ShipObjPnt->Pos.x - StartPos.x;
						Move_Off.y = ShipObjPnt->Pos.y - StartPos.y;
						Move_Off.z = ShipObjPnt->Pos.z - StartPos.z;
						ShipObjPnt->Group = MoveGroup( &Mloadheader, &StartPos, ShipObjPnt->Group, &Move_Off );
					
						// update other player's bounty points
						if ( BountyHunt && ShipObjPnt->Flags & SHIP_CarryingBounty )
						{
							BountyTime += framelag / 60.0F;
							if ( BountyBonusInterval > 0 && BountyTime >= BountyBonusInterval )
							{
								// update bonus 7 (stats.c) -- bounty hunt points
								UpdateBonusStats(i,(int16_t) floor( BountyTime / BountyBonusInterval ));
								BountyTime = FMOD( BountyTime, BountyBonusInterval );
							}
						}					
					}
				}
				else
				{
					ShipPnt->JustRecievedPacket = false;
				}
			}
// End of Special Stuff for other players Ship Movement..Carries on even if no new packet arrives..
/*===================================================================ÄÄÄÄ
			Set the Banking Matrix
/*===================================================================ÄÄÄÄ*/
			MakeQuat( 0.0F , 0.0F , ShipObjPnt->Bank, &StepQuat );
			QuatMultiply(  &ShipObjPnt->Quat , &StepQuat , &ShipObjPnt->FinalQuat );
			QuatToMatrix( &ShipObjPnt->FinalQuat, &ShipObjPnt->FinalMat );
			MatrixTranspose( &ShipObjPnt->FinalMat, &ShipObjPnt->FinalInvMat );
#if 0
			if( ShipObjPnt->light == (u_int16_t) -1 )
			{
				ShipObjPnt->light = FindFreeXLight();
			}
			if( ShipObjPnt->light != (u_int16_t) -1 )
			{
				XLights[ShipObjPnt->light].Size = (4096.0F+2048.0F)*GLOBAL_SCALE;
				XLights[ShipObjPnt->light].CosArc = (float)cos(D2R(35));
				
				ApplyMatrix( &ShipObjPnt->FinalMat, &Forward, &XLights[ShipObjPnt->light].Dir );			/* Calc Direction Vector */
				XLights[ShipObjPnt->light].r = 255.0F;
				XLights[ShipObjPnt->light].g = 255.0F;
				XLights[ShipObjPnt->light].b = 255.0F;
				XLights[ShipObjPnt->light].Type = SPOT_LIGHT;
				XLights[ShipObjPnt->light].Pos = ShipObjPnt->Pos;
				XLights[ShipObjPnt->light].Group = ShipObjPnt->Group;
			}
#endif

#if 0
			if( ShipObjPnt->light == (u_int16_t) -1 )
			{
				ShipObjPnt->light = FindFreeXLight();
			}
			if( ShipObjPnt->light != (u_int16_t) -1 )
			{
				XLights[ShipObjPnt->light].Size = SHIP_RADIUS * 2;
				XLights[ShipObjPnt->light].r = 0.0F;
				XLights[ShipObjPnt->light].g = 0.0F;
				XLights[ShipObjPnt->light].b = 16.0F;
				XLights[ShipObjPnt->light].Pos = ShipObjPnt->Pos;
				XLights[ShipObjPnt->light].Group = ShipObjPnt->Group;
			}
#endif





			ShipPnt->LastMove.x = ShipObjPnt->Pos.x - StartPos.x;
			ShipPnt->LastMove.y = ShipObjPnt->Pos.y - StartPos.y;
			ShipPnt->LastMove.z = ShipObjPnt->Pos.z - StartPos.z;

		}

		if( PlayDemo && ( i == MAX_PLAYERS ) )
		{
			if( PauseDemo )
			{
				framelag = 0.0F;
			}else{
				framelag = Oldframelag * Demoframelag;
			}
		}

		UpdateShipModel( i );

	}

	if( !PlayDemo )
	{
		if( Ships[MAX_PLAYERS].enable != 0 )
		{
			// we have a remote Camera...
			Current_Camera_View = MAX_PLAYERS;		// which object is currently using the camera view....
			Process_Remote_Camera();
		}
		else
		{	// main camera view when alive in multiplayer
			Current_Camera_View = WatchPlayerSelect.value;//WhoIAm;			// which object is currently using the camera view....
		}
	}

	SetPosVelDir_Listner( &Ships[WhoIAm].Object.Pos , &Ships[WhoIAm].Move_Off , &Ships[WhoIAm].Object.Mat );
	
	return true;
}

void ForceExternal( u_int16_t ship, VECTOR *force )
{
	Ships[ ship ].Object.ExternalForce.x += ( force->x * framelag );
	Ships[ ship ].Object.ExternalForce.y += ( force->y * framelag );
	Ships[ ship ].Object.ExternalForce.z += ( force->z * framelag );
}


void ForceInternal( u_int16_t ship, VECTOR *force )
{
	VECTOR	NewForce;

	ApplyMatrix( &Ships[ ship ].Object.Mat, force, &NewForce );

	Ships[ ship ].Object.ExternalForce.x += ( NewForce.x * framelag );
	Ships[ ship ].Object.ExternalForce.y += ( NewForce.y * framelag );
	Ships[ ship ].Object.ExternalForce.z += ( NewForce.z * framelag );

}

void ForceExternalOneOff( u_int16_t ship, VECTOR *force )
{
	Ships[ ship ].Object.ExternalForce.x += force->x;
	Ships[ ship ].Object.ExternalForce.y += force->y;
	Ships[ ship ].Object.ExternalForce.z += force->z;
}


void ObjForceExternalOneOff( OBJECT *Obj, VECTOR *force )
{
	Obj->ExternalForce.x += force->x;
	Obj->ExternalForce.y += force->y;
	Obj->ExternalForce.z += force->z;
}



void ForceInternalOneOff( u_int16_t ship, VECTOR *force )
{
	VECTOR	NewForce;

	ApplyMatrix( &Ships[ ship ].Object.Mat, force, &NewForce );

	Ships[ ship ].Object.ExternalForce.x += NewForce.x;
	Ships[ ship ].Object.ExternalForce.y += NewForce.y;
	Ships[ ship ].Object.ExternalForce.z += NewForce.z;

}


void RotateExternal( u_int16_t ship, VECTOR *point, VECTOR *dir, float force )
{
	VECTOR to_ship;
	VECTOR axis;
	VECTOR near_ship;
	float t;
	float angle;
	QUAT rot;

	to_ship.x = Ships[ship].Object.Pos.x - point->x;
	to_ship.y = Ships[ship].Object.Pos.y - point->y;
	to_ship.z = Ships[ship].Object.Pos.z - point->z;
	CrossProduct( dir, &to_ship, &axis );
	NormaliseVector( &axis );
	t = DotProduct( dir, &to_ship ); // / DotProduct( dir, dir )  iff |dir| != 1
	near_ship.x = point->x + t * dir->x - Ships[ship].Object.Pos.x;
	near_ship.y = point->y + t * dir->y - Ships[ship].Object.Pos.y;
	near_ship.z = point->z + t * dir->z - Ships[ship].Object.Pos.z;
	angle = force * VectorLength( &near_ship ) / SHIP_RADIUS;
	QuatMake( &rot, axis.x, axis.y, axis.z, angle );
	QuatMultiply( &rot, &Ships[ship].Object.RotationForce, &Ships[ship].Object.RotationForce );
}


void RotateInternal( u_int16_t ship, float xrot, float yrot, float zrot )
{
	QUAT rot;

	MakeQuat( xrot, yrot, zrot, &rot );
	QuatMultiply( &Ships[ship].Object.RotationForce , &rot , &Ships[ship].Object.RotationForce );
}


// min/max autolevel pitch = +/-75 degrees from horizontal
#define MIN_AUTOLEVEL_PITCH	D2R(5.0F) 
#define MAX_AUTOLEVEL_PITCH	D2R(175.0F)

//#define MIN_AUTOLEVEL_ROLL_TOLERANCE	(0.01F)
#define MIN_AUTOLEVEL_ROLL_TOLERANCE	(0.2F)
#define AUTOLEVEL_RATE					(0.1F)


float AutoLevelRot( u_int16_t ship, float autolevel_rate )
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
	if ( fabs( Ships[ ship ].Object.Angle.z ) > MIN_AUTOLEVEL_ROLL_TOLERANCE )
	{
		return 0.0F; // ship still rolling under player control
	}
	up = Mloadheader.Group[ Ships[ ship ].Object.Group ].up;
	ApplyMatrix( &Ships[ ship ].Object.Mat, &Forward, &dir );
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
	ApplyMatrix( &Ships[ ship ].Object.Mat, &SlideRight, &side );
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


void BobShip( u_int16_t ship, VECTOR *bob )
{
	VECTOR move;
	float move_len;

	move = Ships[ ship ].Object.Speed;
	move_len = (float) sqrt( move.x * move.x + move.y * move.y + move.z * move.z );
	if ( move_len < MOVE_TOLERANCE && !DebugInfo )
	{
		move.x = BOB_XSIZE * (float) sin( Ships[ ship ].Object.BobCount * BOB_XFREQ );
		move.y = BOB_YSIZE * (float) sin( Ships[ ship ].Object.BobCount * BOB_YFREQ );
		move.z = 0.0F;
		ApplyMatrix( &Ships[ ship ].Object.Mat, &move, bob );
		Ships[ ship ].Object.BobCount += framelag;
	}
	else
	{
		bob->x = 0.0F;
		bob->y = 0.0F;
		bob->z = 0.0F;
		Ships[ ship ].Object.BobCount = 0.0F;
		Ships[ ship ].Object.LastBob = *bob;
	}
}

void InitShipsChangeLevel( MLOADHEADER * Mloadheader )
{
	u_int16_t	i;
	u_int16_t	spos;
	int16_t	Count;
	JustGenerated = true;

	for( i = 0 ; i < (MAX_PLAYERS+1) ; i++ )
	{
		if( i != WhoIAm )
		{
			Ships[i].enable = 0;
		}else{
			Ships[i].enable = true;
		}

		Ships[i].Object.NodeNetwork = 1;
		Ships[i].Object.NearestNode = NULL;
		Ships[i].Object.Type = OBJECT_TYPE_SHIP;

		Ships[i].Object.Flags = 0;		// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
		Ships[i].StealthTime = 0.0F; // Stealth Mode Time Limit
		Ships[i].InvulTimer = 0.0F;	// HowLong am I Invulnerable
		Ships[i].Invul = false;		// Am I Invulnerable...
		Ships[i].Object.Speed.x = 0.0F;
		Ships[i].Object.Speed.y = 0.0F;
		Ships[i].Object.Speed.z = 0.0F;
		Ships[i].Object.Angle.x = 0.0F;
		Ships[i].Object.Angle.y = 0.0F;
		Ships[i].Object.Angle.z = 0.0F;
		Ships[i].Damage = 0;		// how much Damege I Have Taken...
		Ships[i].PrimPowerLevel = 0.0F; // Trojax Powerlevel
		Ships[i].SuperNashramTimer = 0.0F;	// HowLong have i left with the super nashram?
		Ships[i].Object.PowerLevel = 0;
		Ships[i].Object.Mode = NORMAL_MODE;
		Ships[i].Object.Bank = 0.0F;
		Ships[i].PrimBullIdCount = 0;
		Ships[i].SecBullIdCount	= 0;
		Ships[i].Object.Shield	= Start_Shield;
		Ships[i].Object.Hull	= Start_Hull;
		Ships[i].Object.light = (u_int16_t) -1;
		Ships[i].headlights = false;

		for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;
		Ships[i].NumMultiples = 0;
		
		MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.Quat );
		
		if ( Mloadheader->state == true)
		{
			spos = (i+RandomStartPosModify) % num_start_positions;
			InitShipStartPos( i, spos );
		}else{
			InitShipStartPos( i, 0 );
		}
		Ships[i].Object.Mat = MATRIX_Identity;
		Ships[i].Object.FinalMat = MATRIX_Identity;
		Ships[i].Object.FinalInvMat = MATRIX_Identity;

		// reset external and internal force vectors to ship movement
		Ships[i].Object.ExternalForce.x = 0.0F;
		Ships[i].Object.ExternalForce.y = 0.0F;
		Ships[i].Object.ExternalForce.z = 0.0F;
		Ships[i].Object.InternalForce.x = 0.0F;
		Ships[i].Object.InternalForce.y = 0.0F;
		Ships[i].Object.InternalForce.z = 0.0F;

		Ships[i].Object.Autolevel = 0.0F;
		Ships[i].Object.BobCount = 0.0F;

		RestoreWeapons();
		RestoreAmmo();
	}
}



/*===================================================================
		True EnviroMent Mapping for an Mloadheader...
===================================================================*/
bool	ENV( MXLOADHEADER * Mxloadheader , MATRIX * Mat ,VECTOR * Pos)
{
	VECTOR Temp;
//	D3DEXECUTEBUFFERDESC	debDesc;
	u_int16_t group;
	u_int16_t vert;
	u_int16_t execbuf;
	LPLVERTEX	lpLVERTEX = NULL;
	float	u,v;

	for( group = 0 ; group < Mxloadheader->num_groups ; group ++ )
	{
		for( execbuf = 0 ; execbuf < Mxloadheader->Group[group].num_execbufs ; execbuf++ )
		{
			if( (Mxloadheader->Group[group].exec_type[execbuf]&HASENVIROMENTMAP) != 0 )
			{
				/*	lock the execute buffer	*/
//				memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
//				debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

//				if ( Mxloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( Mxloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
//					return false ; // bjd
//				if (FSLockExecuteBuffer(Mxloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
//					return false;
				if (!(FSLockVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf], &lpLVERTEX)))
				{
					return false;
				}

//				lpLVERTEX = ( LPLVERTEX ) debDesc.lpData;
				for( vert = 0 ; vert < 	Mxloadheader->Group[group].num_verts_per_execbuf[execbuf] ; vert ++ )
				{
					ApplyMatrix( Mat , (VECTOR *) lpLVERTEX,  &Temp);
					NormaliseVector( &Temp );

					u = (0.125f + (-0.125f * Temp.x) );
					v = (0.125f + (-0.125f * Temp.y) );

//					lpLVERTEX->color = RGBA_MAKE( (long) (u * 255), (long) (v * 255), 0, 255 );
					lpLVERTEX->tu = u;//+(Pos->x*0.001F)+(Pos->z*0.001F);
					lpLVERTEX->tv = v;//+(Pos->y*0.001F);
					lpLVERTEX++;
				}

				/*	unlock the execute buffer	*/
//				if ( Mxloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
//					return false ;
				if (!(FSUnlockVertexBuffer(&Mxloadheader->Group[group].renderObject[execbuf])))
				{
					return false;
				}
			}
		}
	}
	return true;
}



/*===================================================================ÄÄÄÄ
			Work Out How Much Damage Ive Taken
			Input : Nothing....Ships[WhoIAm].Damage must be set
			Output: 0 no kill...1 last hit killed me...
/*===================================================================ÄÄÄ*/
int16_t DoDamage( bool OverrideInvul )
{
	u_int8_t	Message[ 128 ];
	int16_t	MessageSFX = -1;
	int16_t	TriggerSFX = -1;

	Message[ 0 ] = 0;
			
	if( GodMode ) return 0;
	if( Ships[ WhoIAm ].Invul && !OverrideInvul && GameStatus[ WhoIAm ] == STATUS_SinglePlayer ) return 0;

	if( Ships[WhoIAm].Object.Mode == NORMAL_MODE )
	{
		if( Ships[WhoIAm].Damage )
		{
			if( Ships[ WhoIAm ].Invul) 
				Ships[WhoIAm].Damage *= -0.50F;
			else
				Ships[WhoIAm].Damage *= -1.0F;

			Ships[WhoIAm].Object.Shield += Ships[WhoIAm].Damage;

			if( Ships[WhoIAm].Damage < 0.0F )
			{
				if( Ships[WhoIAm].Object.Mode == NORMAL_MODE )
				{
					MakeScreenFlash( 208, 0, 0, 128, &FlashScreenPoly, SCRSEQ_Fade );

					// play pain cry once in a while ( will be overwritten if shield / hull critical )
					// no speech will be played on death
					if ( !Random_Range( 10 ) )
					{
						MessageSFX = SFX_BikerPain;
					}

					if ( ( Ships[ WhoIAm].Object.Shield < SHIELD_CRITICAL_LEVEL ) && !ShieldCritical )
					{
						MessageSFX = SFX_BIKECOMP_SC;
						TriggerSFX = SFX_BIKER_LE;
						sprintf( &Message[0], SHIELD_CRITICAL );
						ShieldCritical = true;
					}

					if ( ( Ships[ WhoIAm].Object.Hull < HULL_CRITICAL_LEVEL ) && !HullCritical )
					{
						MessageSFX = SFX_BIKECOMP_HC;
						sprintf( &Message[0], HULL_CRITICAL );
						HullCritical = true;
					}
				}
			}

			if( Ships[WhoIAm].Object.Shield  >= MAX_SHIELD )
			{
				Ships[WhoIAm].Object.Shield  = MAX_SHIELD;
			}else{
				ShieldHit = 6*4;
				if( Ships[WhoIAm].Object.Shield  < 0.0F )
				{
					Ships[WhoIAm].Object.Hull += Ships[WhoIAm].Object.Shield;
					Ships[WhoIAm].Object.Shield = 0.0F;
					HullHit = 6*4;

					if( Ships[WhoIAm].Object.Hull <= 0.0F )
					{
						// reset shield & hull critical flags...
						ShieldCritical = false;
						HullCritical = false;

						PlaySfx( SFX_Die, 1.0F );
						Ships[WhoIAm].Object.Hull = 0.0F;
						if( Ships[WhoIAm].ShipThatLastHitMe == WhoIAm )
						{
							// killed myself....Doh
							// stats.c (not sure when this is callled or what weapon caused it??)
							//UpdateKillStats(WhoIAm, WhoIAm, int WeaponType, int Weapon)
						}
						return 1;
					}
				}
			}
		}
	}

	if ( bSoundEnabled && BikeCompSpeechSlider.value && ( MessageSFX != -1 ) )
	{
		if ( ( TriggerSFX != -1 ) && !Random_Range( 4 ) )
			PlaySfxWithTrigger( MessageSFX, TriggerSFX );
		else
			PlaySfx( MessageSFX, 1.0F );
	}
	else
	{
		if ( Message[ 0 ] )
		{
			AddColourMessageToQue( SystemMessageColour, "%s", &Message[ 0 ] );
		}
	}
	
	return 0;
}

/*===================================================================ÄÄÄÄ
	Procedure	:	InitShipRandomStartPos
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void InitShipRandomPos( int16_t i )
{
	int16_t	e;
	int16_t	Count;
	int16_t	orgtry;
	int16_t	trys;
	u_int16_t	startpos;
	VECTOR	MineVec;

	Ships[i].Object.NodeNetwork = 1;
	Ships[i].Object.NearestNode = NULL;
	Ships[i].Object.Type = OBJECT_TYPE_SHIP;

	JustGenerated = true;
	Ships[i].Object.Shield = Start_Shield;
	Ships[i].Object.Hull	= Start_Hull;
	Ships[i].Object.Angle.x = 0.0F;
	Ships[i].Object.Angle.y = 0.0F;
	Ships[i].Object.Angle.z = 0.0F;
	Ships[i].Object.Speed.x = 0.0F;
	Ships[i].Object.Speed.y = 0.0F;
	Ships[i].Object.Speed.z = 0.0F;
	Ships[i].Object.CruiseControl = CRUISE_NORMAL;
	Ships[i].Object.PowerLevel = 0;
	Ships[i].Object.Flags = 0;
	Ships[i].StealthTime = 0.0F;
	Ships[i].NumMultiples = 0;
	Ships[i].Primary = PULSAR;
	Ships[i].Secondary = MUGMISSILE;
	for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;
	Ships[i].NumMultiples = 0;

	RestoreWeapons();
	RestoreAmmo();

	if( Mloadheader.state == true )
	{
		if( num_start_positions >= 2 )
		{
			orgtry = Random_Range( num_start_positions );
			// pick a random startpos to try first then try all others..

			
#if 0
			for( e = 0 ; e < num_start_positions ; e++ )
			{
				trys = ( orgtry + e ) % num_start_positions;
				
				MineVec.x = StartPositions[trys].Pos.x;
				MineVec.y = StartPositions[trys].Pos.y;
				MineVec.z = StartPositions[trys].Pos.z;
				
				if(	!IsStartPosVacantMutualyVisibleGroup( i , trys ) &&
					!CheckForMines( &MineVec) &&
					!CheckForGravgons( &MineVec ) )
				{
					// Found a vacant one
					InitShipStartPos( i, trys );
					return;
				}

			}
#endif
			for( e = 0 ; e < num_start_positions ; e++ )
			{
				trys = ( orgtry + e ) % num_start_positions;

				MineVec.x = StartPositions[trys].Pos.x;
				MineVec.y = StartPositions[trys].Pos.y;
				MineVec.z = StartPositions[trys].Pos.z;
				
				if(	!IsStartPosVacantVisibleGroup( i , trys ) &&
					!CheckForMines( &MineVec) &&
					!CheckForGravgons( &MineVec ) )
				{
					// Found a vacant one
					InitShipStartPos( i, trys );
					return;
				}
			}
			for( e = 0 ; e < num_start_positions ; e++ )
			{
				trys = ( orgtry + e ) % num_start_positions;

				MineVec.x = StartPositions[trys].Pos.x;
				MineVec.y = StartPositions[trys].Pos.y;
				MineVec.z = StartPositions[trys].Pos.z;
				
				if( !IsStartPosVacant( i , trys ) &&
					!CheckForMines( &MineVec) &&
					!CheckForGravgons( &MineVec ) )
				{
					// Found a vacant one
					InitShipStartPos( i, trys );
					return;
				}
			}
		}
		
		// 1 last try...
		startpos = Random_Range( num_start_positions );
		InitShipStartPos( i, startpos );
	}
}


/*===================================================================ÄÄÄÄ
	Procedure	:	InitShipStartPos
	Input		:	int16_t which Ship
					int16_t which pos
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void InitShipStartPos( int16_t i, int16_t pos )
{
	u_int16_t startpos;
	VECTOR	MineVec;
	VECTOR	Start_Dir;
	VECTOR	Start_Up;
	VECTOR	Start_Right;
	VECTOR	Ship_Up;
	QUAT	ZQuat;
#ifdef FUDGE_AUTOLEVEL
	QUAT	StepQuat;
	int		j;
	float	autolevel;
#endif
	JustGenerated = true;

	if( (MyGameStatus==STATUS_SinglePlayer) || (MyGameStatus==STATUS_StartingSinglePlayer) )
	{
		// Were in a Single Player Game so We start from the last start point we visited....
		pos = last_start_position;
	}


#if 0
	Ships[i].Shield = Start_Shield;
	Ships[i].Hull	= Start_Hull;
	Ships[i].Angle.x = 0.0F;
	Ships[i].Angle.y = 0.0F;
	Ships[i].Angle.z = 0.0F;
	Ships[i].PowerLevel = 0;
	Ships[i].Flags = 0;
	Ships[i].NumMultiples = 0;
	Ships[i].StealthTime = 0.0F;
	Ships[i].Primary = PULSAR;
	Ships[i].Secondary = MUGMISSILE;
	Ships[i].light = (u_int16_t) -1;
	RestoreWeapons();
	RestoreAmmo();
#endif
	MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.Quat );
	MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.RotationForce );
	Start_Dir.x = 0.0F;
	Start_Dir.y = 0.0F;
	Start_Dir.z = -1.0F;

	if( Mloadheader.state == true )
	{
		startpos = pos % num_start_positions;
		MineVec.x = StartPositions[startpos].Pos.x;
		MineVec.y = StartPositions[startpos].Pos.y;
		MineVec.z = StartPositions[startpos].Pos.z;
		
		Ships[i].Object.Pos.x = StartPositions[startpos].Pos.x;
		Ships[i].Object.Pos.y = StartPositions[startpos].Pos.y;			
		Ships[i].Object.Pos.z = StartPositions[startpos].Pos.z;
		Start_Dir = StartPositions[ startpos ].Dir;
#if 0
		Start_Dir.x = -Start_Dir.x;
		Start_Dir.y = -Start_Dir.y;
		Start_Dir.z = -Start_Dir.z;
		QuatFromVector2( &Start_Dir, &Ships[i].Quat );
#else
		QuatFrom2Vectors( &Ships[i].Object.Quat, &Forward, &Start_Dir );
#endif
		Ships[i].Object.Group = StartPositions[startpos].Group;
	}else{
		Ships[i].Object.Pos.x = -1.0F;
		Ships[i].Object.Pos.y = -1.0F;
		Ships[i].Object.Pos.z = -1.0F;
		Ships[i].Object.Group = 0;
	}
	QuatToMatrix( &Ships[i].Object.Quat, &Ships[i].Object.Mat );
	ApplyMatrix( &Ships[i].Object.Mat, &SlideUp, &Ship_Up );
#ifdef USE_ORIGINAL_STARTPOS_UP
	CrossProduct( &Start_Dir, &StartPositions[ startpos ].Up, &Start_Right );
#else
	CrossProduct( &Start_Dir, &Mloadheader.Group[ Ships[i].Object.Group ].up, &Start_Right );
#endif
	CrossProduct( &Start_Right, &Start_Dir, &Start_Up );
	QuatFrom2Vectors( &ZQuat, &Ship_Up, &Start_Up );
	QuatMultiply( &ZQuat, &Ships[i].Object.Quat, &Ships[i].Object.Quat );
#ifdef FUDGE_AUTOLEVEL
	QuatToMatrix( &Ships[i].Quat, &Ships[i].Mat ); // needed for AutoLevelRot() below
	
	// autolevel initial ship position
	for ( j = 0; j < 30; j++ )
	{
		autolevel = AutoLevelRot( i, 1.0F );
		if ( fabs( autolevel ) < 0.01 )
			break;
		MakeQuat( 0.0F, 0.0F, autolevel, &StepQuat );
		QuatMultiply(  &Ships[i].Quat , &StepQuat , &Ships[i].Quat );
		QuatToMatrix( &Ships[i].Quat, &Ships[i].Mat );
	}
	DebugPrintf( "autolevel = %f after %d iterations\n", autolevel, j );
#endif
	// reset external and internal force vectors to ship movement
	
	Ships[i].Object.ExternalForce.x = 0.0F;
	Ships[i].Object.ExternalForce.y = 0.0F;
	Ships[i].Object.ExternalForce.z = 0.0F;
	Ships[i].Object.InternalForce.x = 0.0F;
	Ships[i].Object.InternalForce.y = 0.0F;
	Ships[i].Object.InternalForce.z = 0.0F;

	Ships[i].Object.Autolevel = 0.0F;
	Ships[i].Object.BobCount = 0.0F;

	FindNearestNode( &Ships[i].Object );
}



/*===================================================================ÄÄÄÄ
	Procedure	:	GotoRoom
	Input		:	int16_t	which ship
					char *	roomname
	Output		:	true if room found, false otherwise
/*===================================================================ÄÄÄ*/
bool GotoRoom( int16_t i, char *roomname )
{
	u_int16_t startpos;
	VECTOR	MineVec;
	VECTOR	Start_Pos;
	VECTOR	Start_Dir;
	VECTOR	Start_Up;
	VECTOR	Start_Right;
	VECTOR	Ship_Up;
	QUAT	ZQuat;
	int16_t	group;

	if( Mloadheader.state == true )
	{
		for ( group = 0; group < Mloadheader.num_groups; group++ )
		{
			if ( !strcasecmp( roomname, Mloadheader.Group[ ( Ships[i].Object.Group + group ) % Mloadheader.num_groups ].name ) )
				break;
		}
		if ( group >= Mloadheader.num_groups )
			return false;
		group = ( Ships[i].Object.Group + group ) % Mloadheader.num_groups;
		for ( startpos = 0; startpos < num_start_positions; startpos++ )
		{
			if ( StartPositions[startpos].Group == group )
				break;
		}
		if ( startpos < num_start_positions )
		{
			Start_Pos = StartPositions[startpos].Pos;
			Start_Up = StartPositions[ startpos ].Up;
			Start_Dir = StartPositions[ startpos ].Dir;
		}
		else
		{
			Start_Pos.x = Mloadheader.Group[group].center.x;
			Start_Pos.y = Mloadheader.Group[group].center.y;
			Start_Pos.z = Mloadheader.Group[group].center.z;
			Start_Up.x = 0.0F;
			Start_Up.y = 1.0F;
			Start_Up.z = 0.0F;
			Start_Dir.x = 0.0F;
			Start_Dir.y = 0.0F;
			Start_Dir.z = -1.0F;
		}
		MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.Quat );
		MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.RotationForce );
#if 0
		Start_Dir.x = -Start_Dir.x;
		Start_Dir.y = -Start_Dir.y;
		Start_Dir.z = -Start_Dir.z;
		QuatFromVector2( &Start_Dir, &Ships[i].Quat );
#else
		QuatFrom2Vectors( &Ships[i].Object.Quat, &Forward, &Start_Dir );
#endif
		Ships[i].Object.Group = group;
	}else{
		return false;
	}
	MineVec.x = Start_Pos.x;
	MineVec.y = Start_Pos.y;
	MineVec.z = Start_Pos.z;
	Ships[i].Object.Pos.x = Start_Pos.x;
	Ships[i].Object.Pos.y = Start_Pos.y;			
	Ships[i].Object.Pos.z = Start_Pos.z;
	QuatToMatrix( &Ships[i].Object.Quat, &Ships[i].Object.Mat );
	ApplyMatrix( &Ships[i].Object.Mat, &SlideUp, &Ship_Up );
	CrossProduct( &Start_Dir, &Start_Up, &Start_Right );
	CrossProduct( &Start_Right, &Start_Dir, &Start_Up );
	QuatFrom2Vectors( &ZQuat, &Ship_Up, &Start_Up );
	QuatMultiply( &ZQuat, &Ships[i].Object.Quat, &Ships[i].Object.Quat );
	
	// reset external and internal force vectors to ship movement
	
	Ships[i].Object.ExternalForce.x = 0.0F;
	Ships[i].Object.ExternalForce.y = 0.0F;
	Ships[i].Object.ExternalForce.z = 0.0F;
	Ships[i].Object.InternalForce.x = 0.0F;
	Ships[i].Object.InternalForce.y = 0.0F;
	Ships[i].Object.InternalForce.z = 0.0F;

	Ships[i].Object.Autolevel = 0.0F;
	Ships[i].Object.BobCount = 0.0F;

	return true;
}



/*===================================================================ÄÄÄÄ
	Procedure	:	Ship Mode 0	Normal Ship...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void ShipMode0( GLOBALSHIP * ShipPnt , BYTE i )
{

	// make sure the white out stuff is reset..
	WhiteOut = 0.0F;
	
	//			Handle all the Input Stuff
	control_ship( player_config, &control );
	
	ShipPnt->Object.Angle.x += control.pitch;
	ShipPnt->Object.Angle.y += control.yaw;
	ShipPnt->Object.Angle.z += control.roll;
	
	ShipPnt->Object.Bank	 += control.bank;

	ShipPnt->Object.CruiseControl += control.cruise_control;
	if ( ShipPnt->Object.CruiseControl > CRUISE_NITRO )
		ShipPnt->Object.CruiseControl = CRUISE_NITRO;
	else if ( ShipPnt->Object.CruiseControl < CRUISE_REVERSE )
		ShipPnt->Object.CruiseControl = CRUISE_REVERSE;

	if ( control.forward )
	{
		ShipPnt->Object.Speed.z += control.forward;
	}
	else
	{
		switch( ShipPnt->Object.CruiseControl )
		{
		case CRUISE_NORMAL: // normal control
			ShipPnt->Object.Speed.z += control.forward;
			break;
		case CRUISE_FORWARD: // full speed ahead
			ShipPnt->Object.Speed.z += MoveAccell * MaxMoveSpeed * framelag;
			break;
		case CRUISE_NITRO: // nitro ahead
			if ( NitroFuel )
			{
				if ( control.cruise_control > 0 )
					PlaySfx( SFX_NitroStart, 0.66F );
				ShipPnt->Object.Speed.z += TurboAccell * MaxTurboSpeed * framelag;
			}
			else
			{
				ShipPnt->Object.Speed.z += MoveAccell * MaxMoveSpeed * framelag;
				ShipPnt->Object.CruiseControl = CRUISE_FORWARD;
			}
			break;
		case CRUISE_REVERSE: // full speed reverse
			ShipPnt->Object.Speed.z -= MoveAccell * MaxMoveSpeed * framelag;
			break;
		}
	}
	
	ShipPnt->Object.Speed.x += control.right;
	ShipPnt->Object.Speed.y += control.up;
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Ship Mode 1	Death Mode...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
#include "input.h"
void ShipMode1( GLOBALSHIP * ShipPnt , BYTE i )
{
	VECTOR	Move_Dir;
	VECTOR	Move_Off;
	VECTOR	ScatterDir;
	bool	Impact = false;

	input_grab(false);

	control_ship( NULL, &control );
	// fudge to stop you turboing while your dieing...
	control.turbo = 0;
	ShipPnt->Object.CruiseControl = CRUISE_NORMAL;

	// if timer is 0 then set up remote Camera....	
	if( ShipPnt->Timer == 0 )
	{
		Ships[MAX_PLAYERS].enable = 1;
		Ships[MAX_PLAYERS].Object.Angle.x = 0.0F;
		Ships[MAX_PLAYERS].Object.Angle.y = 0.0F;
		Ships[MAX_PLAYERS].Object.Angle.z = 0.0F;

		Move_Off = ShipPnt->Move_Off;

		if( Move_Off.x < 1.0F && Move_Off.x > -1.0F &&
			Move_Off.y < 1.0F && Move_Off.y > -1.0F &&
			Move_Off.z < 1.0F && Move_Off.z > -1.0F )
		{
			// The Camera cant end up in exactly the same position as the ship....
			Move_Off.z = 1.0F;
		}
		NormaliseVector(&Move_Off);

		Move_Off.x = Move_Off.x * -(512.0F*GLOBAL_SCALE);
		Move_Off.y = Move_Off.y * -(512.0F*GLOBAL_SCALE);
		Move_Off.z = Move_Off.z * -(512.0F*GLOBAL_SCALE);
		Ships[MAX_PLAYERS].Object.Pos = ShipPnt->Object.Pos;
		Ships[MAX_PLAYERS].Object.Group = ShipPnt->Object.Group;
		
		ObjectCollide( &Ships[ MAX_PLAYERS ].Object, &Move_Off, SHIP_RADIUS, NULL );
		Ships[MAX_PLAYERS].Object.Mode = DEATH_MODE;
		if( Ships[MAX_PLAYERS].Object.Pos.x == Ships[WhoIAm].Object.Pos.x &&
			Ships[MAX_PLAYERS].Object.Pos.y == Ships[WhoIAm].Object.Pos.y &&
			Ships[MAX_PLAYERS].Object.Pos.z == Ships[WhoIAm].Object.Pos.z )
		{
			// The Camera cant end up in exactly the same position as the ship....
			Ships[MAX_PLAYERS].Object.Pos.x += 1.0F;
			Ships[MAX_PLAYERS].Object.Pos.y += 1.0F;
			Ships[MAX_PLAYERS].Object.Pos.z += 1.0F;
		}


		ApplyMatrix( &Ships[WhoIAm].Object.Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
		MakeViewMatrix( &Ships[MAX_PLAYERS].Object.Pos, &Ships[WhoIAm].Object.Pos, &Move_Dir, &Ships[MAX_PLAYERS].Object.FinalMat);
		MatrixTranspose( &Ships[MAX_PLAYERS].Object.FinalMat, &Ships[MAX_PLAYERS].Object.FinalInvMat );


		ShipPnt->Object.ExternalForce.x = ShipPnt->Move_Off.x;
		ShipPnt->Object.ExternalForce.y = ShipPnt->Move_Off.y;
		ShipPnt->Object.ExternalForce.z = ShipPnt->Move_Off.z;

		Current_Camera_View = MAX_PLAYERS;
		OldDrawPanel = DrawPanel;
		DrawPanel = false;
		RearCameraDisable = true;
	}
	
	ShipPnt->Timer -= real_framelag;

	ShipPnt->Object.Angle.y += ( MaxTurnSpeed * 0.04F ) * framelag;
	ShipPnt->Object.Angle.x += ( MaxTurnSpeed * 0.03F ) * framelag;
	ShipPnt->Object.Angle.z += ( MaxTurnSpeed * 0.02F ) * framelag;
	Impact = CarryonDeathMove( ShipPnt , i );

	if( (ShipPnt->Timer < (RESPAWN_TIMER*0.5f)) || Impact )
	{
		ScatterDir = ShipPnt->LastMove;
		NormaliseVector( &ScatterDir );
		ScatterWeapons( &ScatterDir, MAXSCATTERED );		// Scatter weapons in all directions.

		CreateShockwave( OWNER_SHIP, WhoIAm, &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, 6.0F, (BYTE) -1 );
		CreateShockwaveSend( OWNER_SHIP, WhoIAm, &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, 6.0F, (BYTE) -1 );

		ShipPnt->Object.Mode = LIMBO_MODE;

		ThrowOutRider( i );
		PlayPannedSfx( SFX_BikeExplode, Ships[ WhoIAm ].Object.Group , &Ships[WhoIAm].Object.Pos, 0.0F );
		StopTaunt();
	}
	else
	{
		if( !Random_Range( 8 ) )
		{
			CreateSingleExplosion( &ShipPnt->Object.Pos, ShipPnt->Object.Group );
		}
	}
}
/*===================================================================ÄÄÄÄ
	Procedure	:	Ship Mode 2	Limbo Mode...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
extern void clear_last_mouse_state(void);
void ShipMode2( GLOBALSHIP * ShipPnt , BYTE i )
{
	input_grab(false);
	if( GodMode )
	{
		Current_Camera_View = WhoIAm;		// set it back to me...
		Ships[MAX_PLAYERS].enable = 0;		// Turn Off the remote camera...
		ShipMode0( ShipPnt , i );
	}
	else
	{
		// commented out because it was causing conflicts with watch mode
	//	Current_Camera_View = MAX_PLAYERS;	// set it back to Remote Camera..
	//	Ships[MAX_PLAYERS].enable = 1;		// Turn Off the remote camera...
		ShipPnt->Timer -= real_framelag;

		WhiteOut += framelag;
		if( WhiteOut >= 512.0F )
			WhiteOut = 512.0F;
		if( ShipPnt->Timer < RESPAWN_TIMER )
			ShipPnt->Timer = RESPAWN_TIMER - 1;

#ifdef LUA_BOT
		// bot automatically respawns
		if( ShipPnt->Timer < RESPAWN_TIMER )
#else
		// hit respawn key
		if( ( ShipPnt->Timer < RESPAWN_TIMER ) && ( AnyKeyReleased() != 0 ) )
#endif
		{
			input_grab(true);

			// single player
			if( MyGameStatus == STATUS_SinglePlayer )
			{
				if( Lives == 0 )
				{
					AddColourMessageToQue(SystemMessageColour, NO_LIVES_LEFT );
					ShipPnt->Timer = 0.0F;
					ShipPnt->Object.Mode = GAMEOVER_MODE;
					PlaySfx( SFX_BIKER_LP, 1.0F );
					return;
				}
				Lives--;
				if( Lives == 0 )
					AddColourMessageToQue(SystemMessageColour, LAST_LIFE );
				else
				{
					if( Lives == 1 )
						AddColourMessageToQue(SystemMessageColour, ONE_LIFE_LEFT , Lives );
					else
						AddColourMessageToQue(SystemMessageColour, LIVES_LEFT, Lives );
				}
			}

			// reset watch mode
			if(SwitchedToWatchMode)	
			{
				SwitchedToWatchMode = false;
				// send a message to everybody saying im in watch mode
				SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_ExitedWatchMode, 0);
			}

			WhiteOut = 0.0F;
			InitShipRandomPos( i );
			JustGenerated = true;
			ShipPnt->Object.Mode = NORMAL_MODE;
			ShipPnt->InvulTimer = RGENINVULTIME;		// approx 1 second of invulnerability...			
			ShipPnt->Invul = true;
			PlaySfx( SFX_PlayerGenerate, 1.0F );
			
			// play a general biker phrase on respawning, but only in single player
			if( MyGameStatus == STATUS_SinglePlayer )
			{
				PlaySfx( SFX_BIKER_GP, 1.0F );
			}

			Ships[MAX_PLAYERS].enable = 0;		// Turn Off the remote camera...
			Current_Camera_View = WhoIAm;		// set it back to me...
			flush_input = true;
			control_ship( player_config, &control );

			// so we don't fire from birth
			clear_last_mouse_state();
			control_ship( NULL, &control );
		}
		// respawn key not hit
		else
		{
			control_ship( NULL, &control ); // clear ship controls
		}
	}
}
/*===================================================================ÄÄÄÄ
	Procedure	:	Ship Mode 4	Game over mode...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void ShipMode4( GLOBALSHIP * ShipPnt , BYTE i )
{
	input_grab(false);
	Current_Camera_View = MAX_PLAYERS;	// set it back to Remote Camera..
	Ships[MAX_PLAYERS].enable = 1;		// Turn On the remote camera...
	ShipPnt->Timer -= real_framelag;
	WhiteOut += framelag;
	if( WhiteOut >= 512.0F )
		WhiteOut = 512.0F;
	if( ( ShipPnt->Timer < (RESPAWN_TIMER*0.5f) ) && ( AnyKeyReleased() != 0 ) )
	{
		ShipPnt->Timer = (RESPAWN_TIMER*0.5f) - 1;
		// All Lives have been lost.......
		SpecialDestroyGame();

		NumGoldBars = 0;
		CrystalsFound = 0;
	}
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Watch Mode 5	enables watching other players...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void WatchMode5( GLOBALSHIP * ShipPnt , BYTE i )
{
	VECTOR	ScatterDir;

	input_grab(false);

	// clear any white out
	WhiteOut = 0.0F;

	// only scatter weapons once
	if(!SwitchedToWatchMode)
	{
		// make sure weapons are scattered
		ScatterDir = ShipPnt->LastMove;
		NormaliseVector( &ScatterDir );
		ScatterWeapons( &ScatterDir, 15 );

		// send a message to everybody saying im in watch mode
		SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_EnteredWatchMode, 0);

		// set flag so we don't redo this
		SwitchedToWatchMode = true;
	}

	//	Handle all the Input Stuff
	control_ship( player_config, &control );
	// i dont have a bike in the game
	ShipPnt->Object.Mode = LIMBO_MODE;

}

/*===================================================================ÄÄÄÄ
	Procedure	:	What the Remote Camera Does...
	Input		:	Nothing
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void Process_Remote_Camera( void )
{
 	( * RemoteCameraModeControl[ Ships[MAX_PLAYERS].Object.Mode ] )( &Ships[MAX_PLAYERS] , MAX_PLAYERS );		//go off and do his thing...
}

/*===================================================================ÄÄÄÄ
	Procedure	:	Remote Camera Mode 0	Normal Ship...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void RemoteCameraMode0( GLOBALSHIP * ShipPnt , BYTE i )
{
	VECTOR	Move_Dir;
	VECTOR	Move_Dir2;
	VECTOR	temp;
	float	Speed;

	VECTOR	ImpactPoint;
	VECTOR	Move_Off;	
	VECTOR	Pos_New;	
	u_int16_t	ImpactGroup;
	NORMAL	FaceNormal;

	if( !DebugInfo )
	{
		temp.x = ShipPnt->Object.Pos.x - Ships[WhoIAm].Object.Pos.x; 
		temp.y = ShipPnt->Object.Pos.y - Ships[WhoIAm].Object.Pos.y; 
		temp.z = ShipPnt->Object.Pos.z - Ships[WhoIAm].Object.Pos.z;
		if( (float) sqrt( (temp.x*temp.x) + (temp.y*temp.y) + (temp.z*temp.z) ) <= (SHIP_RADIUS+(SHIP_RADIUS*0.5F) ) )
		{
			Current_Camera_View = WhoIAm;
		}else{
			Current_Camera_View = i;
		}
		
		Move_Dir.x = ShipPnt->Object.Pos.x - Ships[WhoIAm].Object.Pos.x ;
		Move_Dir.y = ShipPnt->Object.Pos.y - Ships[WhoIAm].Object.Pos.y ;
		Move_Dir.z = ShipPnt->Object.Pos.z - Ships[WhoIAm].Object.Pos.z ;
		
		if ( Ships[WhoIAm].Object.Group != (u_int16_t) -1 )
		{
			if( BackgroundCollide( &MCloadheadert0, &Mloadheader,
   										&Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Move_Dir,
										&ImpactPoint , &ImpactGroup, &FaceNormal, &Pos_New, true, NULL ) == true )
			{
				ShipPnt->Object.Pos.x = ImpactPoint.x;
				ShipPnt->Object.Pos.y = ImpactPoint.y;
				ShipPnt->Object.Pos.z = ImpactPoint.z;
				ShipPnt->Object.Group = ImpactGroup;
			}
		}
		
		ApplyMatrix( &Ships[WhoIAm].Object.Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
		ApplyMatrix( &Ships[WhoIAm].Object.Mat, &SlideUp, &Move_Dir2 );			/* Calc Direction Vector */
		
		Speed = 800.0F*GLOBAL_SCALE;
		Move_Dir.x = -( Move_Dir.x * Speed ) + ( Move_Dir2.x * 32.0F ) ;
		Move_Dir.y = -( Move_Dir.y * Speed ) + ( Move_Dir2.y * 32.0F );
		Move_Dir.z = -( Move_Dir.z * Speed ) + ( Move_Dir2.z * 32.0F );
		
		temp = Ships[WhoIAm].Object.Pos;
		NumCollides = 0;
		ImpactGroup = Ships[WhoIAm].Object.Group;
		if ( Ships[WhoIAm].Object.Group != (u_int16_t) -1 )
		{
			while ( BackgroundCollide( &MCloadheader, &Mloadheader,
   										&temp, ImpactGroup, &Move_Dir,
   										&ImpactPoint , &ImpactGroup, &FaceNormal, &Pos_New, true, NULL ) )
			{
				temp = ImpactPoint;
				Move_Dir.x = Pos_New.x - temp.x; 
				Move_Dir.y = Pos_New.y - temp.y; 
				Move_Dir.z = Pos_New.z - temp.z;
				NumCollides += 1;
				if( NumCollides	> MAXCOLLISIONS )
					break;
			}
		}
		temp = ImpactPoint;
		
		Move_Off.x = temp.x - ShipPnt->Object.Pos.x ;
		Move_Off.y = temp.y - ShipPnt->Object.Pos.y ;
		Move_Off.z = temp.z - ShipPnt->Object.Pos.z ;
		
		
		Speed = 0.15F;
		
		Move_Off.x = ( Move_Off.x * Speed )*framelag;
		Move_Off.y = ( Move_Off.y * Speed )*framelag;
		Move_Off.z = ( Move_Off.z * Speed )*framelag;
		// do collisions to background
		ObjectCollide( &ShipPnt->Object, &Move_Off, SHIP_RADIUS, NULL );
	}else{

		ApplyMatrix( &Ships[WhoIAm].Object.Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
		Speed = 800.0F*GLOBAL_SCALE;
		
		ShipPnt->Object.Group = Ships[WhoIAm].Object.Group;
		ShipPnt->Object.Pos.x = Ships[WhoIAm].Object.Pos.x - ( Move_Dir.x * Speed );
		ShipPnt->Object.Pos.y = Ships[WhoIAm].Object.Pos.y - ( Move_Dir.y * Speed );
		ShipPnt->Object.Pos.z = Ships[WhoIAm].Object.Pos.z - ( Move_Dir.z * Speed );

	}

	ApplyMatrix( &Ships[WhoIAm].Object.Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
	MakeViewMatrix( &ShipPnt->Object.Pos, &Ships[WhoIAm].Object.Pos, &Move_Dir, &ShipPnt->Object.FinalMat);
	MatrixTranspose( &ShipPnt->Object.FinalMat, &ShipPnt->Object.FinalInvMat );
}
/*===================================================================ÄÄÄÄ
	Procedure	:	Remote Camera Mode 1	Death Mode...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void RemoteCameraMode1( GLOBALSHIP * ShipPnt , BYTE i )
{
	VECTOR	Bob;
	VECTOR	Move_Off;
	VECTOR	NewPos;
	VECTOR	NewBob;
	VECTOR	Move_Dir;
	float tmp;

	if( Ships[WhoIAm].Object.Mode != LIMBO_MODE )
	{
		ApplyMatrix( &ShipPnt->Object.FinalMat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
		MakeViewMatrix( &ShipPnt->Object.Pos, &Ships[WhoIAm].Object.Pos, &Move_Dir, &ShipPnt->Object.FinalMat);
		MatrixTranspose( &ShipPnt->Object.FinalMat, &ShipPnt->Object.FinalInvMat );
		return;
	}
	
	BobShip( i, &Bob );
	Move_Off.x = Bob.x - ShipPnt->Object.LastBob.x;
	Move_Off.y = Bob.y - ShipPnt->Object.LastBob.y;
	Move_Off.z = Bob.z - ShipPnt->Object.LastBob.z;
	ShipPnt->Object.LastBob = Bob;
	NewBob = Move_Off;

	// protect against division by 0
	if(framelag) tmp = 50.0F / framelag;
	else         tmp = 0.0F;

	NewPos.x = Ships[WhoIAm].Object.Pos.x + ( NewBob.x * tmp );
	NewPos.y = Ships[WhoIAm].Object.Pos.y + ( NewBob.y * tmp );
	NewPos.z = Ships[WhoIAm].Object.Pos.z + ( NewBob.z * tmp );

	ApplyMatrix( &ShipPnt->Object.FinalMat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
	MakeViewMatrix( &ShipPnt->Object.Pos, &NewPos, &Move_Dir, &ShipPnt->Object.FinalMat);
	MatrixTranspose( &ShipPnt->Object.FinalMat, &ShipPnt->Object.FinalInvMat );

}

/*===================================================================
	Procedure	:	Is there a Bike near this restart point...
	Input		:	nothing
	Output		:	false/true
===================================================================*/
#ifdef USEINLINE
#ifdef FINAL_RELEASE
__inline
#endif
#endif
bool	IsStartPosVacant( int16_t i , u_int16_t startpos )
{
	int	e;
	for( e = 0 ; e < MAX_PLAYERS ; e++ )
	{
		if( i != e )
		{
			if( GameStatus[e] == MyGameStatus )
			{
				if( DistanceVector2Vector( &StartPositions[startpos].Pos , &Ships[e].Object.Pos ) < ( BLOCKSIZE * 6.0F ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*===================================================================
	Procedure	:	Is there a Bike in a Mutually visible Group from this restart point...
	Input		:	nothing
	Output		:	false/true
===================================================================*/
#ifdef USEINLINE
#ifdef FINAL_RELEASE
__inline
#endif
#endif
bool	IsStartPosVacantMutualyVisibleGroup( int16_t i , u_int16_t startpos )
{
	int	e;
	for( e = 0 ; e < MAX_PLAYERS ; e++ )
	{
		if( i != e )
		{
			if( GameStatus[e] == MyGameStatus )
			{
				if( VisibleOverlap( Ships[ e ].Object.Group, StartPositions[startpos].Group , NULL ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*===================================================================
	Procedure	:	Is there a Bike in a visible group from this restart point...
	Input		:	nothing
	Output		:	false/true
===================================================================*/
#ifdef USEINLINE
#ifdef FINAL_RELEASE
__inline
#endif
#endif
bool	IsStartPosVacantVisibleGroup( int16_t i , u_int16_t startpos )
{
	int	e;
	for( e = 0 ; e < MAX_PLAYERS ; e++ )
	{
		if( i != e )
		{
			if( GameStatus[e] == MyGameStatus )
			{
				if( GroupsAreVisible( Ships[ e ].Object.Group, StartPositions[startpos].Group ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}


/*===================================================================ÄÄÄÄ
	Procedure	:	Remote Camera Mode 3	Demo Playback...
	Input		:	int16_t which Ship
	Output		:	Nothing
/*===================================================================ÄÄÄ*/
void RemoteCameraMode3( GLOBALSHIP * ShipPnt , BYTE i )
{
	framelag = Oldframelag;
	if ( DemoEyesSelect.value != MAX_PLAYERS )
	{
		int j, k;
		bool ok;

		// make sure we are looking through the eyes of a valid player in demo playback
		ok = false;
		for ( j = 0; j < MAX_PLAYERS; j++ )
		{
			k = ( DemoEyesSelect.value + j ) % MAX_PLAYERS;
			if ( k != MAX_PLAYERS )
			{
				switch ( GameStatus[ k ] )
				{
				case STATUS_Joining:
				case STATUS_Left:
				case STATUS_LeftCrashed:
				case STATUS_Null:
					// skip players in invalid modes
					break;
				default:
					// any other mode is OK to use for demo playback
					DemoEyesSelect.value = k;
					ok = true;
				}
				if ( ok )
					break;
			}
		}
	}
	Current_Camera_View = DemoEyesSelect.value;
	TeamNumber[ i ] = TeamNumber[ Current_Camera_View ];
	ShipMode0( ShipPnt , i );
}

/*===================================================================
	Procedure	:	Update Nearest StartPos visited...
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void	UpdateStartPos( void )
{
	VECTOR	Temp;
	u_int16_t	startpos;
	
	if( (MyGameStatus==STATUS_SinglePlayer) || (MyGameStatus==STATUS_StartingSinglePlayer) )
	{
		
		if( last_start_position >= num_start_positions )
		{
			last_start_position = 0;
		}

		startpos = FirstStartPositionInGroup[Ships[WhoIAm].Object.Group];
		
		while( startpos != (u_int16_t) -1 )
		{
			if( ( startpos != last_start_position ) &&
				( StartPositions[startpos].Group != StartPositions[last_start_position].Group ) )
			{
				// There is a start position in the current group....Did I get close enough ??
				Temp.x = Ships[WhoIAm].Object.Pos.x - StartPositions[startpos].Pos.x;
				if( Temp.x < 0.0F )	Temp.x *= -1.0F;
				Temp.y =  Ships[WhoIAm].Object.Pos.y - StartPositions[startpos].Pos.y;
				if( Temp.y < 0.0F )	Temp.y *= -1.0F;
				Temp.z =  Ships[WhoIAm].Object.Pos.z - StartPositions[startpos].Pos.z;
				if( Temp.z < 0.0F )	Temp.z *= -1.0F;
				if ( (Temp.x <= SHIP_RADIUS * 4.0F ) &&
					(Temp.y <= SHIP_RADIUS * 4.0F ) &&
					(Temp.z <= SHIP_RADIUS * 4.0F ) )
				{
					ActivateRestartPoint( startpos, last_start_position );

					PlaySfx( SFX_RestartPointReached, 1.0F );
					AddColourMessageToQue(SystemMessageColour, RESTART_ACTIVATED );
					last_start_position = startpos;
					return;
				}
			}
			startpos = StartPositions[startpos].NextInGroup;
		}
	}else{
		last_start_position = 0;
	}
}
/*===================================================================
	Procedure	:	Start Screen Shake...
	Input		:	float Time;
	Output		:	nothing
===================================================================*/
void StartShipScreenShake( float Force )
{
	Ships[WhoIAm].ShakeTimer = 2.0F * 60.0F;
	Ships[WhoIAm].ShakeForce = Force;
}
/*===================================================================
	Procedure	:	Stop Screen Shake...
	Input		:	float Time;
	Output		:	nothing
===================================================================*/
void StopShipScreenShake( void )
{
	Ships[WhoIAm].ShakeTimer = 0.0F;
}


/*===================================================================
	Procedure	:	Set Bike Mods...
	Input		:	BikeNumber..
	Output		:	nothing
===================================================================*/
void SetBikeMods( u_int16_t Bike )
{
	if( Bike >= MAXBIKETYPES+3 )
		return;
	MaxMoveSpeed	=	BikeMods[Bike].MaxMoveSpeed;
	MoveAccell		=	BikeMods[Bike].MoveAccell;	  
	MoveDecell		=	BikeMods[Bike].MoveDecell;	  
	MaxTurboSpeed   =	BikeMods[Bike].MaxTurboSpeed;   
	TurboAccell		=	BikeMods[Bike].TurboAccell;	  	
	TurboDecell		=	BikeMods[Bike].TurboDecell;	  	
	MaxTurnSpeed	=	BikeMods[Bike].MaxTurnSpeed; 
	TurnAccell		=	BikeMods[Bike].TurnAccell;	  
	TurnDecell		=	BikeMods[Bike].TurnDecell;	  
	MaxRollSpeed	=	BikeMods[Bike].MaxRollSpeed;  
	RollAccell		=	BikeMods[Bike].RollAccell;	  
	RollDecell		=	BikeMods[Bike].RollDecell;	  
	MaxBankAngle    =	BikeMods[Bike].MaxBankAngle;    
	BankAccell		=	BikeMods[Bike].BankAccell;	  
	BankDecell		=	BikeMods[Bike].BankDecell;	  
	Start_Shield	=	BikeMods[Bike].Start_Shield;
	Start_Hull		=	BikeMods[Bike].Start_Hull;	
	AutoLevel		=	BikeMods[Bike].AutoLevel;	
}

/*===================================================================
	Procedure	:	Ship 2 Ship collide...
	Input		:	BikeNumber..
	Output		:	Move_Off filled in...
===================================================================*/
bool Ship2ShipCollide( u_int16_t i , VECTOR * Move_Off )
{
	u_int16_t Count;
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
	bool	ok;
	u_int16_t	NewGroup;
	
	Norm_Move_Off = *Move_Off;
	NormaliseVector( &Norm_Move_Off );
	OldMove = VectorLength( &Ships[i].Move_Off);
	
	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		
		if ( (Ships[Count].enable != 0) && (Ships[Count].Object.Mode != LIMBO_MODE) && (Ships[Count].Object.Mode != GAMEOVER_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) &&
			 ( Count != i ) )//&& ( VectorLength( &Ships[Count].Move_Off) <= OldMove) && !( ( OldMove == 0.0F ) && (VectorLength( &Ships[Count].Move_Off) == 0.0F ) ) )
		{
			ok = false;
			if( Ships[Count].Object.Group != Ships[i].Object.Group )
			{
				// if ship are in different groups then we must do a move group to see if they end up in the same group...
				Move_Dir.x = Ships[Count].Object.Pos.x - Ships[i].Object.Pos.x;
				Move_Dir.y = Ships[Count].Object.Pos.y - Ships[i].Object.Pos.y;
				Move_Dir.z = Ships[Count].Object.Pos.z - Ships[i].Object.Pos.z;
				NewGroup = MoveGroup( &Mloadheader, &Ships[i].Object.Pos, Ships[i].Object.Group, &Move_Dir );
				if( NewGroup == Ships[Count].Object.Group )
					ok = true;
			}else{
				ok = true;
			}

			if( ok )
			{
				Move_Length = DistanceVector2Vector( &Ships[i].Object.Pos , &Ships[Count].Object.Pos );

				NewPos.x = Ships[i].Object.Pos.x + Move_Off->x;
				NewPos.y = Ships[i].Object.Pos.y + Move_Off->y;
				NewPos.z = Ships[i].Object.Pos.z + Move_Off->z;

				Next_Move_Length = DistanceVector2Vector( &NewPos , &Ships[Count].Object.Pos );
				
				if( (Move_Length < ( SHIP_RADIUS*2.0F ) ) && ( Next_Move_Length < Move_Length ) )
				{
					if( Move_Length < 1.0F )
						Move_Length = 1.0F;
		
					// the ships are to close....Move them apart...Guarenteed..
					Move_Dir.x = Ships[i].Object.Pos.x - Ships[Count].Object.Pos.x;
					Move_Dir.y = Ships[i].Object.Pos.y - Ships[Count].Object.Pos.y;
					Move_Dir.z = Ships[i].Object.Pos.z - Ships[Count].Object.Pos.z;
					NormaliseVector( &Move_Dir );

					Move_Length = ( (SHIP_RADIUS*2.0F)+1.0F ) - Move_Length;

	//				if( Move_Length >= (SHIP_RADIUS * 0.1F) )
	//					Move_Length = (SHIP_RADIUS * 0.1F);
					if( Move_Dir.x == 0.0F && Move_Dir.y == 0.0F && Move_Dir.z == 0.0F )
						return false;
					Move_Off->x = Move_Dir.x * Move_Length;
					Move_Off->y = Move_Dir.y * Move_Length;
					Move_Off->z = Move_Dir.z * Move_Length;
	//				Ships[i].Object.Speed.x = 0.0F;
	//				Ships[i].Object.Speed.y = 0.0F;
	//				Ships[i].Object.Speed.z = 0.0F;


					return true;
				}else{
	#if 1
					if( RaytoSphere2( &Ships[Count].Object.Pos, (SHIP_RADIUS*2.0F)-2.0F, &Ships[i].Object.Pos, &Norm_Move_Off, &inter, &inter2 ) )
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
	#endif
				}
			}
		}
	}
	return HasBeen;
}
#if 1
bool CarryonDeathMove( GLOBALSHIP * ShipPnt,BYTE i)
{
	VECTOR	Move_Off;	
	QUAT	StepQuat;	
	BGOBJECT * BGObject;
	float	ShieldModifier;
	u_int32_t	OldInWater;
	bool Impact = false;

	AccellDecell( &ShipPnt->Object.Angle.y , TurnDecell );
	AccellDecell( &ShipPnt->Object.Angle.x , TurnDecell );
	AccellDecell( &ShipPnt->Object.Angle.z , RollDecell );

	MakeQuat( ShipPnt->Object.Angle.x * framelag, ShipPnt->Object.Angle.y * framelag, ShipPnt->Object.Angle.z * framelag + ShipPnt->Object.Autolevel, &StepQuat );
	QuatMultiply(  &ShipPnt->Object.Quat , &StepQuat , &ShipPnt->Object.Quat );
	QuatMultiply(  &ShipPnt->Object.RotationForce , &ShipPnt->Object.Quat , &ShipPnt->Object.Quat );
	QuatToMatrix( &ShipPnt->Object.Quat, &ShipPnt->Object.Mat );
	MakeQuat( 0.0F, 0.0F, 0.0F, &ShipPnt->Object.RotationForce );

	// apply external and internal force vectors to ship movement
	Move_Off.x = ShipPnt->Object.ExternalForce.x * framelag;
	Move_Off.y = ShipPnt->Object.ExternalForce.y * framelag;
	Move_Off.z = ShipPnt->Object.ExternalForce.z * framelag;

	ShipPnt->Object.ExternalForce.x -= Mloadheader.Group[ ShipPnt->Object.Group ].up.x * 0.01F * framelag;
	ShipPnt->Object.ExternalForce.y -= Mloadheader.Group[ ShipPnt->Object.Group ].up.y * 0.01F * framelag;
	ShipPnt->Object.ExternalForce.z -= Mloadheader.Group[ ShipPnt->Object.Group ].up.z * 0.01F * framelag;

	if( (i != MAX_PLAYERS) && (ShipPnt->Object.Mode != LIMBO_MODE) )
	{
		if( Ship2ShipCollide(  i , &Move_Off ) )
			Impact = true;
	}
	// Collide with the enemies....
	if( Ship2EnemyCollide( i , &Move_Off ) )
	{
		Impact = true;
	}
		
	BGObject = NULL;
	if( ObjectCollide( &ShipPnt->Object, &Move_Off, SHIP_RADIUS, &BGObject ) )
		Impact = true;
	if( BGObject ) ChangeBGState( BGObject, OWNER_SHIP, WhoIAm, BUMP, 0.0F );

	OldInWater = ShipPnt->Object.Flags & SHIP_InWater;
	ShipPnt->Object.Flags &= ~SHIP_InWater;
	if( InWater( ShipPnt->Object.Group , &ShipPnt->Object.Pos , &ShieldModifier) )
	{
		ShipPnt->Object.Flags |= SHIP_InWater;
	}else
	{
		// not in water, therefore stop underwater looping sfx if currently playing
		if ( UnderwaterSfxID )
		{
			StopSfx( UnderwaterSfxID );
		}
	}

	if ( framelag )
	{
		ShipPnt->Move_Off.x = Move_Off.x / framelag;
		ShipPnt->Move_Off.y = Move_Off.y / framelag;
		ShipPnt->Move_Off.z = Move_Off.z / framelag;
	}
	else
	{
		ShipPnt->Move_Off.x = 0.0F;
		ShipPnt->Move_Off.y = 0.0F;
		ShipPnt->Move_Off.z = 0.0F;
	}

	return Impact;
}
#endif




/*===================================================================
	Procedure	:	Multiplayer Sfx handler...
	Input		:	Nothing..
	Output		:	Nothing..
===================================================================*/
#define MAXKILLMEMORY 8
#define KILLSOVERTIME 5			// Number of kills to get....
#define TIMEKILLSOVER 60		// Length of time on seconds over which the kills have to have been made...
#define BADTIMEKILLSOVER 50		// if you go without a kill for this amoutn of time you get a taunt for your trouble...
#define	CAMPINGTIME 45			// how long you have to stay in 1 approx position...
#define	CAMPINGRANGE ( 1024.0F * GLOBAL_SCALE * 4.0F )	// Range for camping

bool MultiSfxTaunt1Off = false;

u_int16_t CurrentKillPos = 0;
px_timer_t	KillMemoryTime[MAXKILLMEMORY];
int16_t	KillMemory[MAXKILLMEMORY];

px_timer_t	camping_time;
VECTOR		CampingPos;
bool	CampingPos1Off = false;


int16_t OldKills = 0;

void MultiSfxHandle( void )
{
	int16_t	KillIndex;
	bool	NewKill;

	if( MyGameStatus != STATUS_Normal || WhoIAm >= MAX_PLAYERS )
		return;

	NewKill = false;

	if( GetTotalKills(WhoIAm) > OldKills )
	{
		NewKill = true;
		OldKills = GetTotalKills(WhoIAm);
	}

	KillIndex = (CurrentKillPos - KILLSOVERTIME) & ( MAXKILLMEMORY -1 );
	if( NewKill )
	{
		if( timer_peek( &KillMemoryTime[KillIndex] ) > TIMEKILLSOVER )
		{
			// play congrat gloat thing...
			PlaySfx( SFX_BIKECOMP_MK, 1.0F );

			// Clear stuff out so it doesnt get played again..
			while( KillIndex != CurrentKillPos )
			{
				KillMemory[KillIndex] = -1;
				KillIndex += 1;
				KillIndex &= ( MAXKILLMEMORY -1 );
			}
			timer_run( &KillMemoryTime[KillIndex] );
		}
	}

	KillIndex = (CurrentKillPos - 1) & ( MAXKILLMEMORY -1 );
	if( timer_peek( &KillMemoryTime[KillIndex] ) > BADTIMEKILLSOVER )
	{
		if( MultiSfxTaunt1Off )
		{
			DebugPrintf("Playing taunt for not getting a kill in a long time...\n");
			PlaySfx( SFX_BIKECOMP_NK, 1.0F );
			timer_run( &camping_time );
		}
		else
		{
			MultiSfxTaunt1Off = true;
		}
		timer_run( &KillMemoryTime[KillIndex] );
	}

	if( !CampingPos1Off )
	{
		timer_run( &camping_time );
		CampingPos1Off = true;
		CampingPos = Ships[WhoIAm].Object.Pos;
	}
	else
	{
		if( DistanceVector2Vector( &CampingPos , &Ships[WhoIAm].Object.Pos ) < CAMPINGRANGE )
		{
			if( timer_peek( &camping_time ) > CAMPINGTIME )
			{
				PlaySfx( SFX_BIKECOMP_CA, 1.0F );
				timer_run( &camping_time );
			}
		}
		else
		{
			timer_run( &camping_time );
			CampingPos = Ships[WhoIAm].Object.Pos;
		}
	}

}

/*===================================================================
	Procedure	:	Makes a note of when the last kill was made..
	Input		:	Nothing..
	Output		:	Nothing..
===================================================================*/
void AddKill( void )
{
	KillMemory[CurrentKillPos] = GetTotalKills(WhoIAm);
	timer_run( &KillMemoryTime[CurrentKillPos] );
	CurrentKillPos++;
	if( CurrentKillPos >= MAXKILLMEMORY )
		CurrentKillPos = 0;
}


void InitMultiSfxHandle( void )
{
	int i;
	for( i = 0 ; i < MAXKILLMEMORY ; i ++ )
	{
		KillMemory[i] = -1;
		timer_run( &KillMemoryTime[i] );
	}
	CurrentKillPos = 0;
	MultiSfxTaunt1Off = false;

	CampingPos1Off = false;
}

/*===================================================================
	Procedure	:	Release All Componeted Ships
	Input		:	Nothing..
	Output		:	Nothing..
===================================================================*/
void ReleaseComponentedShips( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( Ships[ Count ].Object.Components )
		{
			FreeCompObjChildren( Ships[ Count ].Object.Components, 1 );
			free( Ships[ Count ].Object.Components );
			Ships[ Count ].Object.Components = NULL;
			Ships[ Count ].ModelNum = (u_int16_t) -1;
		}
		else
		{
			Ships[ Count ].ModelNum = (u_int16_t) -1;
		}
	}
}

/*===================================================================
	Procedure	:	Save Ship Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveShips( FILE * fp )
{
	u_int16_t			i;
	COMP_OBJ	*	TempPtr = NULL;

	if( fp )
	{
		i = WhoIAm;

		Ships[ i ].Object.ID = &Ships[ i ];
		Ships[ i ].Object.Type = OBJECT_TYPE_SHIP;

		if( Ships[ i ].Object.Components )
		{
			TempPtr = Ships[ i ].Object.Components;
			Ships[ i ].Object.Components = NULL;
		}

		if( !Object_Save( fp , &Ships[ i ].Object ) )
		{
			if( TempPtr )
			{
				Ships[ i ].Object.Components = TempPtr;
				TempPtr = NULL;
			}

			fclose( fp );
			return( NULL );
		}

		if( TempPtr )
		{
			Ships[ i ].Object.Components = TempPtr;
			TempPtr = NULL;
		}
			
		fwrite( &Ships[ i ].enable, sizeof( Ships[ i ].enable ), 1, fp );
		fwrite( &Ships[ i ].ShipThatLastKilledMe, sizeof( Ships[ i ].ShipThatLastKilledMe ), 1, fp );
		fwrite( &Ships[ i ].ShipThatLastHitMe, sizeof( Ships[ i ].ShipThatLastHitMe ), 1, fp );
		fwrite( &Ships[ i ].NumMultiples, sizeof( Ships[ i ].NumMultiples ), 1, fp );
		fwrite( &Ships[ i ].StealthTime, sizeof( Ships[ i ].StealthTime ), 1, fp );
		fwrite( &Ships[ i ].Timer, sizeof( Ships[ i ].Timer ), 1, fp );
		fwrite( &Ships[ i ].InvulTimer, sizeof( Ships[ i ].InvulTimer ), 1, fp );
		fwrite( &Ships[ i ].Invul, sizeof( Ships[ i ].Invul ), 1, fp );
		fwrite( &Ships[ i ].LastAngle, sizeof( Ships[ i ].LastAngle ), 1, fp );
		fwrite( &Ships[ i ].PrimBullIdCount, sizeof( Ships[ i ].PrimBullIdCount ), 1, fp );
		fwrite( &Ships[ i ].SecBullIdCount, sizeof( Ships[ i ].SecBullIdCount ), 1, fp );
		fwrite( &Ships[ i ].PickupIdCount, sizeof( Ships[ i ].PickupIdCount ), 1, fp );
		fwrite( &Ships[ i ].Damage, sizeof( Ships[ i ].Damage ), 1, fp );
		fwrite( &Ships[ i ].Primary, sizeof( Ships[ i ].Primary ), 1, fp );
		fwrite( &Ships[ i ].Secondary, sizeof( Ships[ i ].Secondary ), 1, fp );
		fwrite( &Ships[ i ].ModelNum, sizeof( Ships[ i ].ModelNum ), 1, fp );
		fwrite( &Ships[ i ].BikeNum, sizeof( Ships[ i ].BikeNum ), 1, fp );
		fwrite( &Ships[ i ].Pickups, sizeof( Ships[ i ].Pickups ), 1, fp );
		fwrite( &Ships[ i ].RegenSlots, sizeof( Ships[ i ].RegenSlots ), 1, fp );
		fwrite( &Ships[ i ].LastMove, sizeof( Ships[ i ].LastMove ), 1, fp );
		fwrite( &Ships[ i ].Move_Off, sizeof( Ships[ i ].Move_Off ), 1, fp );
		fwrite( &Ships[ i ].OrbModels, sizeof( Ships[ i ].OrbModels ), 1, fp );
		fwrite( &Ships[ i ].OrbAmmo, sizeof( Ships[ i ].OrbAmmo ), 1, fp );
		fwrite( &Ships[ i ].PrimPowerLevel, sizeof( Ships[ i ].PrimPowerLevel ), 1, fp );
		fwrite( &Ships[ i ].PrimID, sizeof( Ships[ i ].PrimID ), 1, fp );
		fwrite( &Ships[ i ].SecID, sizeof( Ships[ i ].SecID ), 1, fp );
		fwrite( &Ships[ i ].SecWeapon, sizeof( Ships[ i ].SecWeapon ), 1, fp );
		fwrite( &Ships[ i ].SuperNashramTimer, sizeof( Ships[ i ].SuperNashramTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeTimer, sizeof( Ships[ i ].ShakeTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeDirTimer, sizeof( Ships[ i ].ShakeDirTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeForce, sizeof( Ships[ i ].ShakeForce ), 1, fp );

		i = MAX_PLAYERS;

		Ships[ i ].Object.ID = &Ships[ i ];
		Ships[ i ].Object.Type = OBJECT_TYPE_SHIP;
		if( !Object_Save( fp , &Ships[ i ].Object ) )
		{
			fclose( fp );
			return( NULL );
		}
			
		fwrite( &Ships[ i ].enable, sizeof( Ships[ i ].enable ), 1, fp );
		fwrite( &Ships[ i ].ShipThatLastKilledMe, sizeof( Ships[ i ].ShipThatLastKilledMe ), 1, fp );
		fwrite( &Ships[ i ].ShipThatLastHitMe, sizeof( Ships[ i ].ShipThatLastHitMe ), 1, fp );
		fwrite( &Ships[ i ].NumMultiples, sizeof( Ships[ i ].NumMultiples ), 1, fp );
		fwrite( &Ships[ i ].StealthTime, sizeof( Ships[ i ].StealthTime ), 1, fp );
		fwrite( &Ships[ i ].Timer, sizeof( Ships[ i ].Timer ), 1, fp );
		fwrite( &Ships[ i ].InvulTimer, sizeof( Ships[ i ].InvulTimer ), 1, fp );
		fwrite( &Ships[ i ].Invul, sizeof( Ships[ i ].Invul ), 1, fp );
		fwrite( &Ships[ i ].LastAngle, sizeof( Ships[ i ].LastAngle ), 1, fp );
		fwrite( &Ships[ i ].PrimBullIdCount, sizeof( Ships[ i ].PrimBullIdCount ), 1, fp );
		fwrite( &Ships[ i ].SecBullIdCount, sizeof( Ships[ i ].SecBullIdCount ), 1, fp );
		fwrite( &Ships[ i ].PickupIdCount, sizeof( Ships[ i ].PickupIdCount ), 1, fp );
		fwrite( &Ships[ i ].Damage, sizeof( Ships[ i ].Damage ), 1, fp );
		fwrite( &Ships[ i ].Primary, sizeof( Ships[ i ].Primary ), 1, fp );
		fwrite( &Ships[ i ].Secondary, sizeof( Ships[ i ].Secondary ), 1, fp );
		fwrite( &Ships[ i ].ModelNum, sizeof( Ships[ i ].ModelNum ), 1, fp );
		fwrite( &Ships[ i ].BikeNum, sizeof( Ships[ i ].BikeNum ), 1, fp );
		fwrite( &Ships[ i ].Pickups, sizeof( Ships[ i ].Pickups ), 1, fp );
		fwrite( &Ships[ i ].RegenSlots, sizeof( Ships[ i ].RegenSlots ), 1, fp );
		fwrite( &Ships[ i ].LastMove, sizeof( Ships[ i ].LastMove ), 1, fp );
		fwrite( &Ships[ i ].Move_Off, sizeof( Ships[ i ].Move_Off ), 1, fp );
		fwrite( &Ships[ i ].OrbModels, sizeof( Ships[ i ].OrbModels ), 1, fp );
		fwrite( &Ships[ i ].OrbAmmo, sizeof( Ships[ i ].OrbAmmo ), 1, fp );
		fwrite( &Ships[ i ].PrimPowerLevel, sizeof( Ships[ i ].PrimPowerLevel ), 1, fp );
		fwrite( &Ships[ i ].PrimID, sizeof( Ships[ i ].PrimID ), 1, fp );
		fwrite( &Ships[ i ].SecID, sizeof( Ships[ i ].SecID ), 1, fp );
		fwrite( &Ships[ i ].SecWeapon, sizeof( Ships[ i ].SecWeapon ), 1, fp );
		fwrite( &Ships[ i ].SuperNashramTimer, sizeof( Ships[ i ].SuperNashramTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeTimer, sizeof( Ships[ i ].ShakeTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeDirTimer, sizeof( Ships[ i ].ShakeDirTimer ), 1, fp );
		fwrite( &Ships[ i ].ShakeForce, sizeof( Ships[ i ].ShakeForce ), 1, fp );

		fwrite( &LevelTimeTaken, sizeof( float ), 1, fp );
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load Ship Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadShips( FILE * fp )
{
	u_int16_t	i;

	if( fp )
	{
		i = WhoIAm;

		if( !Object_Load( fp , &Ships[ i ].Object ) )
		{
			fclose( fp );
			return( NULL );
		}
			
		fread( &Ships[ i ].enable, sizeof( Ships[ i ].enable ), 1, fp );
		fread( &Ships[ i ].ShipThatLastKilledMe, sizeof( Ships[ i ].ShipThatLastKilledMe ), 1, fp );
		fread( &Ships[ i ].ShipThatLastHitMe, sizeof( Ships[ i ].ShipThatLastHitMe ), 1, fp );
		fread( &Ships[ i ].NumMultiples, sizeof( Ships[ i ].NumMultiples ), 1, fp );
		fread( &Ships[ i ].StealthTime, sizeof( Ships[ i ].StealthTime ), 1, fp );
		fread( &Ships[ i ].Timer, sizeof( Ships[ i ].Timer ), 1, fp );
		fread( &Ships[ i ].InvulTimer, sizeof( Ships[ i ].InvulTimer ), 1, fp );
		fread( &Ships[ i ].Invul, sizeof( Ships[ i ].Invul ), 1, fp );
		fread( &Ships[ i ].LastAngle, sizeof( Ships[ i ].LastAngle ), 1, fp );
		fread( &Ships[ i ].PrimBullIdCount, sizeof( Ships[ i ].PrimBullIdCount ), 1, fp );
		fread( &Ships[ i ].SecBullIdCount, sizeof( Ships[ i ].SecBullIdCount ), 1, fp );
		fread( &Ships[ i ].PickupIdCount, sizeof( Ships[ i ].PickupIdCount ), 1, fp );
		fread( &Ships[ i ].Damage, sizeof( Ships[ i ].Damage ), 1, fp );
		fread( &Ships[ i ].Primary, sizeof( Ships[ i ].Primary ), 1, fp );
		fread( &Ships[ i ].Secondary, sizeof( Ships[ i ].Secondary ), 1, fp );
		fread( &Ships[ i ].ModelNum, sizeof( Ships[ i ].ModelNum ), 1, fp );
		fread( &Ships[ i ].BikeNum, sizeof( Ships[ i ].BikeNum ), 1, fp );
		fread( &Ships[ i ].Pickups, sizeof( Ships[ i ].Pickups ), 1, fp );
		fread( &Ships[ i ].RegenSlots, sizeof( Ships[ i ].RegenSlots ), 1, fp );
		fread( &Ships[ i ].LastMove, sizeof( Ships[ i ].LastMove ), 1, fp );
		fread( &Ships[ i ].Move_Off, sizeof( Ships[ i ].Move_Off ), 1, fp );
		fread( &Ships[ i ].OrbModels, sizeof( Ships[ i ].OrbModels ), 1, fp );
		fread( &Ships[ i ].OrbAmmo, sizeof( Ships[ i ].OrbAmmo ), 1, fp );
		fread( &Ships[ i ].PrimPowerLevel, sizeof( Ships[ i ].PrimPowerLevel ), 1, fp );
		fread( &Ships[ i ].PrimID, sizeof( Ships[ i ].PrimID ), 1, fp );
		fread( &Ships[ i ].SecID, sizeof( Ships[ i ].SecID ), 1, fp );
		fread( &Ships[ i ].SecWeapon, sizeof( Ships[ i ].SecWeapon ), 1, fp );
		fread( &Ships[ i ].SuperNashramTimer, sizeof( Ships[ i ].SuperNashramTimer ), 1, fp );
		fread( &Ships[ i ].ShakeTimer, sizeof( Ships[ i ].ShakeTimer ), 1, fp );
		fread( &Ships[ i ].ShakeDirTimer, sizeof( Ships[ i ].ShakeDirTimer ), 1, fp );
		fread( &Ships[ i ].ShakeForce, sizeof( Ships[ i ].ShakeForce ), 1, fp );

		i = MAX_PLAYERS;

		if( !Object_Load( fp , &Ships[ i ].Object ) )
		{
			fclose( fp );
			return( NULL );
		}
			
		fread( &Ships[ i ].enable, sizeof( Ships[ i ].enable ), 1, fp );
		fread( &Ships[ i ].ShipThatLastKilledMe, sizeof( Ships[ i ].ShipThatLastKilledMe ), 1, fp );
		fread( &Ships[ i ].ShipThatLastHitMe, sizeof( Ships[ i ].ShipThatLastHitMe ), 1, fp );
		fread( &Ships[ i ].NumMultiples, sizeof( Ships[ i ].NumMultiples ), 1, fp );
		fread( &Ships[ i ].StealthTime, sizeof( Ships[ i ].StealthTime ), 1, fp );
		fread( &Ships[ i ].Timer, sizeof( Ships[ i ].Timer ), 1, fp );
		fread( &Ships[ i ].InvulTimer, sizeof( Ships[ i ].InvulTimer ), 1, fp );
		fread( &Ships[ i ].Invul, sizeof( Ships[ i ].Invul ), 1, fp );
		fread( &Ships[ i ].LastAngle, sizeof( Ships[ i ].LastAngle ), 1, fp );
		fread( &Ships[ i ].PrimBullIdCount, sizeof( Ships[ i ].PrimBullIdCount ), 1, fp );
		fread( &Ships[ i ].SecBullIdCount, sizeof( Ships[ i ].SecBullIdCount ), 1, fp );
		fread( &Ships[ i ].PickupIdCount, sizeof( Ships[ i ].PickupIdCount ), 1, fp );
		fread( &Ships[ i ].Damage, sizeof( Ships[ i ].Damage ), 1, fp );
		fread( &Ships[ i ].Primary, sizeof( Ships[ i ].Primary ), 1, fp );
		fread( &Ships[ i ].Secondary, sizeof( Ships[ i ].Secondary ), 1, fp );
		fread( &Ships[ i ].ModelNum, sizeof( Ships[ i ].ModelNum ), 1, fp );
		fread( &Ships[ i ].BikeNum, sizeof( Ships[ i ].BikeNum ), 1, fp );
		fread( &Ships[ i ].Pickups, sizeof( Ships[ i ].Pickups ), 1, fp );
		fread( &Ships[ i ].RegenSlots, sizeof( Ships[ i ].RegenSlots ), 1, fp );
		fread( &Ships[ i ].LastMove, sizeof( Ships[ i ].LastMove ), 1, fp );
		fread( &Ships[ i ].Move_Off, sizeof( Ships[ i ].Move_Off ), 1, fp );
		fread( &Ships[ i ].OrbModels, sizeof( Ships[ i ].OrbModels ), 1, fp );
		fread( &Ships[ i ].OrbAmmo, sizeof( Ships[ i ].OrbAmmo ), 1, fp );
		fread( &Ships[ i ].PrimPowerLevel, sizeof( Ships[ i ].PrimPowerLevel ), 1, fp );
		fread( &Ships[ i ].PrimID, sizeof( Ships[ i ].PrimID ), 1, fp );
		fread( &Ships[ i ].SecID, sizeof( Ships[ i ].SecID ), 1, fp );
		fread( &Ships[ i ].SecWeapon, sizeof( Ships[ i ].SecWeapon ), 1, fp );
		fread( &Ships[ i ].SuperNashramTimer, sizeof( Ships[ i ].SuperNashramTimer ), 1, fp );
		fread( &Ships[ i ].ShakeTimer, sizeof( Ships[ i ].ShakeTimer ), 1, fp );
		fread( &Ships[ i ].ShakeDirTimer, sizeof( Ships[ i ].ShakeDirTimer ), 1, fp );
		fread( &Ships[ i ].ShakeForce, sizeof( Ships[ i ].ShakeForce ), 1, fp );

		fread( &LevelTimeTaken, sizeof( float ), 1, fp );
	}

	return( fp );
}

float	MyShield;
float	MyHull;

void StoreMyShieldHull( void )
{
	MyShield = Ships[WhoIAm].Object.Shield * 1234.0F;
	MyHull = Ships[WhoIAm].Object.Hull * 4321.0F;
}
void ReStoreMyShieldHull( void )
{
	Ships[WhoIAm].Object.Shield = MyShield / 1234.0F;
	Ships[WhoIAm].Object.Hull = MyHull / 4321.0F;
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
