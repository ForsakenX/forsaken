
//
// General Networking
//

#include "main.h"
#include <stdio.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "2dtextures.h"
#include "mload.h"
#include "triggers.h"
#include "pickups.h"
#include "primary.h"
#include "secondary.h"
#include "mload.h"
#include "text.h"
#include "models.h"
#include "render.h"
#include "net.h"
#include "lights.h"
#include "title.h"
#include "multiplayer.h"
#include "controls.h"
#include "screenpolys.h"
#include "goal.h"
#include <stdio.h>
#include "local.h"
#include "stats.h"
#include "version.h"
#include "net_tracker.h"
#include "timer.h"
#include "oct2.h"


BYTE WhoIAm = UNASSIGNED_SHIP;

extern bool Debug;

int	PacketGot[256];
int	PacketSize[256];

u_int32_t RecPacketSize				= 0;
u_int32_t MaxRecPacketSize				= 0;
u_int32_t BytesPerSecRec				= 0;
u_int32_t BytesPerSecSent				= 0;
u_int32_t CurrentBytesPerSecRec		= 0;
u_int32_t CurrentBytesPerSecSent		= 0;
u_int32_t MaxCurrentBytesPerSecRec		= 0;
u_int32_t MaxCurrentBytesPerSecSent	= 0;
float BytesPerSecTimer				= 0.0F;

extern MENU  * GetPlayerNumMenu;

extern int FontHeight;

extern	int16_t	PickupsGot[ MAXPICKUPTYPES ];
extern	u_int32_t	TeamFlagMask[ MAX_TEAMS ];
extern	int8_t	TeamFlagPickup[ MAX_TEAMS ];

extern int16_t	NumPrimaryPickups;

extern	bool	CountDownOn;
extern char LevelNames[MAXLEVELS][128];

extern	SLIDER	MaxPlayersSlider;

extern SLIDER	GoalScoreSlider;

extern LIST	LevelList;
extern	SLIDER	TimeLimit;
extern	int		GameType;
extern	bool	MyBrightShips;
extern bool BountyBonus;

extern float GetPlayerNumCount1;
extern float GetPlayerNumCount2;
extern int   GetPlayerNumCount;

extern bool	PickupValid[ MAXPICKUPTYPES ];

#define YourVersion "YOUR VERSION: " PXVersion

#define	ONEOVER32767 (1.0F / 32767.0F)
#define	ONEOVER256 (1.0F / 256.0F)

#define	SHORTBANKMODIFIER (32767.0F / MAXBANKANGLE )
#define	SHORTANGLEMODIFIERPACK (32767.0F / 45.0F)
#define	SHORTANGLEMODIFIERUNPACK (45.0F / 32767.0F)

extern char CTFMessage[];
extern	bool IMustQuit;

extern bool NeedFlagAtHome;
extern bool OwnFlagTeleportsHome;
extern bool CanCarryOwnFlag;
extern SLIDER CTFSlider;

void SetTeamGoals( u_int16_t *TeamGoals );

u_int32_t BuildShipFlags( BYTE Player );
void UnPackShipFlags( BYTE Player , u_int32_t Flags );

extern int FlagsToGenerate;
extern  u_int32_t  Host_Flags[ MAX_PLAYERS ];

extern bool BountyHunt;
extern int BountyBonusInterval;

extern bool	CTF;
extern bool CaptureTheFlag;
extern int GoalScore;
extern int GameCompleted;
extern USERCONFIG	*player_config;

extern char *TeamName[ MAX_TEAMS ];

extern int	AllowedBike[ ALLOWED_BIKETYPES ];
extern int32_t ColPerspective;
extern bool Panel;

extern bool	BrightShips;
extern bool	BikeExhausts;
extern	bool IllegalTime;
extern	float	Countdown_Float;
extern	SLIDER  PacketsSlider;

extern char CurrentTauntVariant;

float NetUpdateInterval = 4.0F;
float HealthUpdateInterval = 4.0F;
int OldPPSValue;
int OldHealthPPSValue;
int OldUseShortPackets;
int OldColPerspective;
float PacketDelay = 4.0F;					// How long before I start to Declerate him.....
float HostDutyTimer = 0.0F;
float NetUpdateIntervalHostDuties = 30.0F;
void SetShipBankAndMat( OBJECT * ShipObjPnt );

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif


extern	SLIDER	MaxKillsSlider;
int16_t	MaxKills = 0;

#ifdef DEMO_SUPPORT
extern	LONGLONG	DemoStartedTime;		// when the game started
extern	LONGLONG	DemoEndedTime;		// when the game started
extern	int32_t		DemoGameLoops;
extern	LONGLONG	TimeDiff;
extern	LONGLONG	Freq;
#endif

extern	float	DemoAvgFps;
extern	MENUITEM	NewJoinItem;
extern	bool	Buffer1InUse;
extern	int		Buffer1Count;
extern	DWORD	Buffer1Offset;
extern	BYTE *	Buffer1Pnt;

extern	bool	Buffer2InUse;
extern	int		Buffer2Count;
extern	DWORD	Buffer2Offset;
extern	BYTE *	Buffer2Pnt;

extern	bool	RecordDemo;
extern	bool	PlayDemo;
extern	FILE	*	DemoFp;
extern	FILE	*	DemoFpClean;
extern	LIST	DemoList;

#ifdef DEMO_SUPPORT
extern	LONGLONG	GameCurrentTime;		// How long the game has been going...
#endif

extern bool	bSoundEnabled;

extern float framelag;

extern	float	Start_Shield;
extern	float	Start_Hull;

void SpecialDestroyGame( void );
void GetLevelName( char *buf, int bufsize, int level );

bool	CanDoDamage[MAX_PLAYERS+1];

extern	MATRIX			MATRIX_Identity;

extern	TEXT	MacroText1;
extern	TEXT	MacroText2;
extern	TEXT	MacroText3;
extern	TEXT	MacroText4;
extern  TEXT	QuickText;
extern  TEXT	QuickTextWhisper;
extern	int8_t	PrimaryToFireLookup[ MAXPRIMARYWEAPONS ];
extern	int8_t	SecondaryToFireLookup[ MAXSECONDARYWEAPONS ];
extern	float	GlobalFramelagAddition;
extern	bool ResetKillsPerLevel;

bool		JustGenerated = false;
bool		JustPickedUpShield = false;

int16_t	NextworkOldBikeNum = -1;

float PrevHull;
float PrevShield;
float		Interval = 0.0F;
float HealthInterval = 0.0F;
SHIPHEALTHMSG PlayerHealths[ MAX_PLAYERS+1 ];
u_int8_t ShipHealthColour[ MAX_PLAYERS+1 ];
float ShipHealthColourInterval[ MAX_PLAYERS+1 ];

#ifdef DEMO_SUPPORT
extern	LONGLONG	GameStartedTime;
extern	LONGLONG	GameElapsedTime;
LONGLONG	TempTime;
LONGLONG	DemoTimeSoFar = 0;
#endif

extern	u_int16_t		Seed1;
extern	u_int16_t		Seed2;
extern	u_int16_t		CopyOfSeed1;
extern	u_int16_t		CopyOfSeed2;
extern	bool		RandomPickups;


int16_t	BikeModels[ MAXBIKETYPES ] = {

	MODEL_Borg,
	MODEL_Beard,
	MODEL_LaJay,
	MODEL_Excop,
	MODEL_Trucker,
	MODEL_Foetoid,
	MODEL_JapBird,
	MODEL_Nutter,
	MODEL_Rhesus,
	MODEL_Joe,
	MODEL_Shark,
	MODEL_HK5,
	MODEL_Nubia,
	MODEL_Mofisto,
	MODEL_Cerbero,
	MODEL_Slick,
	MODEL_Borg,

};

char					MyName[ 32 ] = "Default game";
char					NickName[ 32 ] = "Default game";
char					tempstr[MAXTEXTMSG];
int16_t					NamesAreLegal = 1;
SHORTNAMETYPE			Names;	// all the players short Names....
BYTE					MyGameStatus = STATUS_Normal;//
BYTE					PreDemoEndMyGameStatus = 0;
BYTE					OverallGameStatus = STATUS_Null;
BYTE					GameStatus[MAX_PLAYERS + 1];	// Game Status for every Ship...
														// this tells the drones what status the host thinks hes in..
BYTE					OldGameStatus[MAX_PLAYERS + 1];	// Game Status for every Ship...
int16_t					Lives = 3;
int16_t					StatsCount = -1;

network_player_t * host_network_player = NULL;

void SfxForCollectPickup( u_int16_t Owner, u_int16_t ID );

void CreateReGen( u_int16_t ship );
bool InitLevels( char *levels_list );
extern	MODEL	Models[MAXNUMOFMODELS];

bool	HostDuties = false;
bool					IsHost = true;

BYTE					Current_Camera_View = 0;		// which object is currently using the camera view....
bool					RemoteCameraActive = false;

PRIMBULLPOSDIR				TempPrimBullPosDir;
SECBULLPOSDIR				TempSecBullPosDir;
SHORTGLOBALSHIP			ShortGlobalShip;
VERYSHORTGLOBALSHIP	VeryShortGlobalShip;
SHIPDIEDINFO					TempDied;
SETTIME							TempTimeSet;
TITANBITS						TempTitanBits;

FSHORTGLOBALSHIP			FShortGlobalShip;
FVERYSHORTGLOBALSHIP	FVeryShortGlobalShip;
GROUPONLY_FVERYSHORTGLOBALSHIP	GroupOnly_FVeryShortGlobalShip;

PICKUPINFO						TempPickup;
VERYSHORTPICKUPINFO		VeryShortTempPickup;
KILLPICKUPINFO				TempKillPickup;
EXPSECONDARYINFO			TempExplodeSecondary;
TEAMGOALSINFO				TempTeamGoals;
SHOCKWAVEINFO				TempShockwave;
BGOUPDATEINFO				TempBGOUpdate;
SHIPHIT							TempShipHit;
SHORTSHIPHIT					ShortTempShipHit;

MISSEDPICKUPMSG			MissedPickups[ MAXMISSEDPICKUPS ];
SHORTPICKUP					MissedInitPickups[ MAXMISSEDPICKUPS ];
SHORTKILLPICKUP				MissedKillPickups[ MAXMISSEDPICKUPS ];
int16_t								NumMissedPickups = 0;
int16_t								NumMissedInitPickups = 0;
int16_t								NumMissedKillPickups = 0;

MISSEDMINEMSG				MissedMines[ MAXMISSEDMINES ];
SHORTMINE						MissedInitMines[ MAXMISSEDMINES ];
SHORTKILLMINE				MissedKillMines[ MAXMISSEDMINES ];
int16_t								NumMissedMines = 0;
int16_t								NumMissedInitMines = 0;
int16_t								NumMissedKillMines = 0;

extern u_int16_t	num_start_positions;

GLOBALSHIP              Ships[MAX_PLAYERS+1];
bool	DemoShipInit[MAX_PLAYERS+1];

px_timer_t	LastPacketTime[MAX_PLAYERS+1];
BYTE	CommBuff[MAX_BUFFER_SIZE];

int		RealPacketSize[256];

extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	VECTOR			NullVector;
extern	MLOADHEADER Mloadheader;
extern	int16_t LevelNum;
extern	float PowerLevel;
extern	SECONDARYWEAPONATTRIB SecondaryWeaponAttribs[ TOTALSECONDARYWEAPONS ];
extern	SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	SHIPCONTROL control;
extern	char * Messages[];
extern	int16_t	SelectedBike;
extern	bool	GodMode;

extern	bool	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	u_int16_t	RandomStartPosModify;

bool	ChAngleevel( void );
void	DebugPrintf( const char * format, ... );
void AddTitleMessage(LPTEXTMSG LpTextMsg);

MATRIX	TempMatrix = {
				1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 1.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F };

extern	char biker_name[256];

extern	int16_t			NumRegenPoints;
extern	int				NumOfTrigVars;
extern	int				NumOfTriggers;

bool	UseShortPackets;
bool	MyUseShortPackets;

extern	int16_t	NumOrbs;
extern	PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern	char *SecondaryDescription[MAXSECONDARYWEAPONS];
extern	char *PrimaryDescription[];
extern	int16_t	Host_PrimaryWeaponsGot[ MAX_PLAYERS ][ MAXPRIMARYWEAPONS ];
extern	int16_t	Host_SecondaryWeaponsGot[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	float	Host_GeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmo[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmo[ MAX_PLAYERS ];
//extern	float	Host_GeneralAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_GeneralAmmoUsed; //[ MAX_PLAYERS ];
//extern	float	Host_PyroliteAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmoUsed;
//extern	float	Host_SussGunAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmoUsed;
extern	int16_t	Host_SecondaryAmmo[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
//extern	int16_t	Host_SecAmmoUsed[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	int16_t	Host_SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	int8_t	Host_SecWeaponsGot[ MAX_PLAYERS ][ MAXPICKUPS ];
extern	float	Host_NitroFuel[ MAX_PLAYERS ];
//extern	float	Host_NitroFuelUsed[ MAX_PLAYERS ];
extern	float	Host_NitroFuelUsed;
extern	int16_t	Host_PowerLevel[ MAX_PLAYERS ];
extern	float	Host_OrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
extern	BYTE	Host_CopyOfNumOfOrbitals[ MAX_PLAYERS ];
extern	float	Host_CopyOfOrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	int16_t	Host_CopyOfPowerLevel[ MAX_PLAYERS ];
extern	float	Host_CopyOfNitroFuel[ MAX_PLAYERS ];
extern	float	Host_CopyOfGeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfSussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfPyroliteAmmo[ MAX_PLAYERS ];
extern	int16_t	Host_PickupsGot[ MAX_PLAYERS ][ MAXPICKUPTYPES ];
extern	PICKUP	Pickups[ MAXPICKUPS ];
extern	int16_t	SecondaryFromPickupTab[ MAXSECONDARYWEAPONS * 2 ];

// statistics (stats.c)
extern void UpdateKillStats(int Killer, int Victim, int WeaponType, int Weapon);	// update the statistics

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;
extern int PlayerMessageColour;
extern int PickupMessageColour;
extern int TauntMessageColour;
extern int MyMessageColour;

// sfx volume (Title.c)
extern SLIDER FlagSfxSlider;
float FlagVolume;

// watch player mode select player (Title.c)
extern SLIDER WatchPlayerSelect;

// calculates distance between you and a bike (Sfx.c)
float ReturnDistanceVolumeVector( VECTOR *sfxpos, u_int16_t sfxgroup, VECTOR *listenerpos, u_int16_t listenergroup, long *vol, VECTOR *sfxvector );

// stats used to send to other players if you are the host (stats.c)
extern u_int8_t	KillStats[MAX_PLAYERS][MAX_PLAYERS];	
extern u_int8_t	BonusStats[MAX_PLAYERS];
extern u_int8_t	KillCounter[MAX_PLAYERS];	

bool msg_is_valid( int msg_type )
{
	switch( msg_type )
	{
	case MSG_YOUQUIT:
	case MSG_BIKENUM:
	case MSG_HEREIAM:
	case MSG_INIT:
	case MSG_VERYSHORTUPDATE:
	case MSG_UPDATE:
	case MSG_FUPDATE:
	case MSG_VERYSHORTFUPDATE:
	case MSG_GROUPONLY_VERYSHORTFUPDATE:
	case MSG_DROPPICKUP:
	case MSG_VERYSHORTDROPPICKUP:
	case MSG_KILLPICKUP:
	case MSG_TEAMGOALS:
	case MSG_SHOCKWAVE:
	case MSG_BGOUPDATE:
	case MSG_PRIMBULLPOSDIR:
	case MSG_SECBULLPOSDIR:
	case MSG_TITANBITS:
	case MSG_SHIPHIT:
	case MSG_SHORTSHIPHIT:
	case MSG_SHIPDIED:
	case MSG_REQTIME:
	case MSG_SETTIME:
	case MSG_STATUS:
	case MSG_NETSETTINGS:
	case MSG_LONGSTATUS:
	case MSG_SHORTPICKUP:
	case MSG_SHORTREGENSLOT:
	case MSG_SHORTTRIGGER:
	case MSG_SHORTTRIGVAR:
	case MSG_SHORTMINE:
	case MSG_TEXTMSG:
	case MSG_VERYSHORTINTERPOLATE:
	case MSG_INTERPOLATE:
    case MSG_SHIPHEALTH:
		return true;
	}
	return false;
}

char* msg_to_str( int msg_type )
{
	switch( msg_type )
	{
	case MSG_YOUQUIT:                        return "MSG_YOUQUIT";                      break;
	case MSG_BIKENUM:                        return "MSG_BIKENUM";                      break;
	case MSG_HEREIAM:                        return "MSG_HEREIAM";                      break;
	case MSG_INIT:                           return "MSG_INIT";                         break;
	case MSG_VERYSHORTUPDATE:                return "MSG_VERYSHORTUPDATE";              break;
	case MSG_UPDATE:                         return "MSG_UPDATE";                       break;
	case MSG_FUPDATE:                        return "MSG_FUPDATE";                      break;
	case MSG_VERYSHORTFUPDATE:               return "MSG_VERYSHORTFUPDATE";             break;
	case MSG_GROUPONLY_VERYSHORTFUPDATE:     return "MSG_GROUPONLY_VERYSHORTFUPDATE";   break;
	case MSG_DROPPICKUP:                     return "MSG_DROPPICKUP";                   break;
	case MSG_VERYSHORTDROPPICKUP:            return "MSG_VERYSHORTDROPPICKUP";          break;
	case MSG_KILLPICKUP:                     return "MSG_KILLPICKUP";                   break;
	case MSG_TEAMGOALS:                      return "MSG_TEAMGOALS";                    break;
	case MSG_SHOCKWAVE:                      return "MSG_SHOCKWAVE";                    break;
	case MSG_BGOUPDATE:                      return "MSG_BGOUPDATE";                    break;
	case MSG_PRIMBULLPOSDIR:                 return "MSG_PRIMBULLPOSDIR";               break;
	case MSG_SECBULLPOSDIR:                  return "MSG_SECBULLPOSDIR";                break;
	case MSG_TITANBITS:                      return "MSG_TITANBITS";                    break;
	case MSG_SHIPHIT:                        return "MSG_SHIPHIT";                      break;
	case MSG_SHORTSHIPHIT:                   return "MSG_SHORTSHIPHIT";                 break;
	case MSG_SHIPDIED:                       return "MSG_SHIPDIED";                     break;
	case MSG_REQTIME:                        return "MSG_REQTIME";                      break;
	case MSG_SETTIME:                        return "MSG_SETTIME";                      break;
	case MSG_STATUS:                         return "MSG_STATUS";                       break;
	case MSG_NETSETTINGS:                    return "MSG_NETSETTINGS";                  break;
	case MSG_LONGSTATUS:                     return "MSG_LONGSTATUS";                   break;
	case MSG_SHORTPICKUP:                    return "MSG_SHORTPICKUP";                  break;
	case MSG_SHORTREGENSLOT:                 return "MSG_SHORTREGENSLOT";               break;
	case MSG_SHORTTRIGGER:                   return "MSG_SHORTTRIGGER";                 break;
	case MSG_SHORTTRIGVAR:                   return "MSG_SHORTTRIGVAR";                 break;
	case MSG_SHORTMINE:                      return "MSG_SHORTMINE";                    break;
	case MSG_TEXTMSG:                        return "MSG_TEXTMSG";                      break;
	case MSG_VERYSHORTINTERPOLATE:           return "MSG_VERYSHORTINTERPOLATE";         break;
	case MSG_INTERPOLATE:                    return "MSG_INTERPOLATE";                  break;
    case MSG_SHIPHEALTH:                     return "MSG_SHIPHEALTH";                   break;
	}
	return "UNKNOWN";
}

void set_player_name( int BikeNum, char* name )
{
	if( !name || BikeNum < 0 || BikeNum >= MAX_PLAYERS ) return;
	strncpy( &Names[BikeNum][0], name, MAXSHORTNAME );
	Names[BikeNum][MAXSHORTNAME-1] = 0;
}

void set_my_player_name( void )
{
	set_player_name( WhoIAm, &biker_name[0] );
}

void SendANormalUpdate( void )
{
	VECTOR	Move_Off;

	if( !UseShortPackets )
	{
		ShortGlobalShip.Flags = BuildShipFlags(WhoIAm);

		ShortGlobalShip.GroupImIn	= (BYTE) Ships[WhoIAm].Object.Group;    // 
		ShortGlobalShip.Pos			= Ships[WhoIAm].Object.Pos		;	  // x , y , z position
		ShortGlobalShip.Move_Off	= Ships[WhoIAm].Move_Off;	  // Last Movement..x , y , z
		ShortGlobalShip.Quat		= Ships[WhoIAm].Object.Quat;	  // Final inverse view Quat.......after banking.. 
		ShortGlobalShip.Status		= MyGameStatus;
		ShortGlobalShip.Angle		= Ships[WhoIAm].Object.Angle;
#ifdef	SHORTBANK
		ShortGlobalShip.Bank			= (int16_t) (Ships[ WhoIAm ].Object.Bank * SHORTBANKMODIFIER);
#else
		ShortGlobalShip.Bank = Ships[ WhoIAm ].Object.Bank;
#endif
		SendGameMessage(MSG_UPDATE, 0, 0, 0, 0);
	}
	else
	{
		VeryShortGlobalShip.Flags = BuildShipFlags(WhoIAm);
		VeryShortGlobalShip.GroupImIn			= (BYTE) Ships[WhoIAm].Object.Group;    // 
		VeryShortGlobalShip.Pos.x					= (int16_t) Ships[WhoIAm].Object.Pos.x;
		VeryShortGlobalShip.Pos.y					= (int16_t) Ships[WhoIAm].Object.Pos.y;
		VeryShortGlobalShip.Pos.z					= (int16_t) Ships[WhoIAm].Object.Pos.z;
		Move_Off = Ships[WhoIAm].Move_Off;
		NormaliseVector( &Move_Off );
		VeryShortGlobalShip.Move_Off_Scalar	= (u_int16_t) ( 256.0F * VectorLength( &Ships[WhoIAm].Move_Off ) );
		VeryShortGlobalShip.Move_Off.x			= (int16_t) (Move_Off.x * 32767.0F);
		VeryShortGlobalShip.Move_Off.y			= (int16_t) (Move_Off.y * 32767.0F);
		VeryShortGlobalShip.Move_Off.z			= (int16_t) (Move_Off.z * 32767.0F);
		VeryShortGlobalShip.Quat.w				= (int16_t) (Ships[WhoIAm].Object.Quat.w * 32767.0F);
		VeryShortGlobalShip.Quat.x				= (int16_t) (Ships[WhoIAm].Object.Quat.x * 32767.0F);
		VeryShortGlobalShip.Quat.y				= (int16_t) (Ships[WhoIAm].Object.Quat.y * 32767.0F);
		VeryShortGlobalShip.Quat.z				= (int16_t) (Ships[WhoIAm].Object.Quat.z * 32767.0F);
		VeryShortGlobalShip.Status				= MyGameStatus;
		VeryShortGlobalShip.Angle.x				= (int16_t) (Ships[WhoIAm].Object.Angle.x * SHORTANGLEMODIFIERPACK );
		VeryShortGlobalShip.Angle.y				= (int16_t) (Ships[WhoIAm].Object.Angle.y * SHORTANGLEMODIFIERPACK );
		VeryShortGlobalShip.Angle.z				= (int16_t) (Ships[WhoIAm].Object.Angle.z * SHORTANGLEMODIFIERPACK );
		VeryShortGlobalShip.Bank					= (int16_t) (Ships[ WhoIAm ].Object.Bank * SHORTBANKMODIFIER);
		SendGameMessage(MSG_VERYSHORTUPDATE, 0, 0, 0, 0);
	}
}

 
void NetworkGameUpdate()
{
	int i;
	VECTOR	Move_Off;

	if( ( Ships[WhoIAm].Object.Flags & ( SHIP_PrimFire | SHIP_SecFire | SHIP_MulFire ) ) )
		Ships[WhoIAm].Object.Noise = 1.0F;

	if( network_state != NETWORK_CONNECTED )
	{
		// Has to be done to stop missiles getting stuck in walls....!!!!!!!!!!!!!
		Ships[ WhoIAm ].Object.Flags &=  ~( SHIP_PrimFire | SHIP_SecFire | SHIP_MulFire );
	}
	else
	{
		ReceiveGameMessages();
	
		// if health changed, notify other players
        if( Ships[WhoIAm].Object.Hull != PrevHull || Ships[WhoIAm].Object.Shield != PrevShield )
        {
            SendGameMessage(MSG_SHIPHEALTH, 0, 0, 0, 0);
			PrevHull = Ships[WhoIAm].Object.Hull; 
			PrevShield = Ships[WhoIAm].Object.Shield; 
        }

		// Update each bike's health colour timer
		for(i = 0; i < MAX_PLAYERS+1; i++)
		{
			ShipHealthColourInterval[i] -= framelag;
			if( ShipHealthColourInterval[i] <= 0.0F )
				ShipHealthColour[i] = YELLOW; // default
		}
	
		if( ( Ships[WhoIAm].Object.Flags & ( SHIP_PrimFire | SHIP_SecFire | SHIP_MulFire ) ) )
		{
			if( !UseShortPackets )
			{
				FShortGlobalShip.Flags					= BuildShipFlags(WhoIAm);
				FShortGlobalShip.GroupImIn				= (BYTE) Ships[WhoIAm].Object.Group;    // 
				FShortGlobalShip.Primary				= PrimaryToFireLookup[ Ships[WhoIAm].Primary ];
				FShortGlobalShip.Secondary				= SecondaryToFireLookup[ Ships[WhoIAm].Secondary ];
				FShortGlobalShip.Pos					= Ships[WhoIAm].Object.Pos		;	  // x , y , z position
				FShortGlobalShip.Move_Off				= Ships[WhoIAm].Move_Off;	  // Last Movement..x , y , z
				FShortGlobalShip.Quat					= Ships[WhoIAm].Object.Quat;// Final inverse view Quat.......after banking.. 
				FShortGlobalShip.PrimPowerLevel			= (BYTE) Ships[ WhoIAm ].PrimPowerLevel;
				FShortGlobalShip.Angle					= Ships[ WhoIAm ].Object.Angle;
#ifdef	SHORTBANK
				FShortGlobalShip.Bank					= (int16_t) (Ships[ WhoIAm ].Object.Bank * SHORTBANKMODIFIER);
#else
				FShortGlobalShip.Bank					= Ships[ WhoIAm ].Object.Bank;
#endif
				SendGameMessage(MSG_FUPDATE, 0, 0, 0, (BYTE)(Ships[WhoIAm].Object.Flags & SHIP_SecFire | SHIP_MulFire));
			}
			else
			{
				FVeryShortGlobalShip.Flags				= BuildShipFlags(WhoIAm);
				FVeryShortGlobalShip.GroupImIn			= (BYTE) Ships[WhoIAm].Object.Group;    // 
				FVeryShortGlobalShip.Pos.x				= (int16_t) Ships[WhoIAm].Object.Pos.x;
				FVeryShortGlobalShip.Pos.y				= (int16_t) Ships[WhoIAm].Object.Pos.y;
				FVeryShortGlobalShip.Pos.z				= (int16_t) Ships[WhoIAm].Object.Pos.z;
				Move_Off = Ships[WhoIAm].Move_Off;
				NormaliseVector( &Move_Off );
				FVeryShortGlobalShip.Move_Off_Scalar	= (u_int16_t) ( 256.0F * VectorLength( &Ships[WhoIAm].Move_Off ) );
				FVeryShortGlobalShip.Move_Off.x			= (int16_t) (Move_Off.x * 32767.0F);
				FVeryShortGlobalShip.Move_Off.y			= (int16_t) (Move_Off.y * 32767.0F);
				FVeryShortGlobalShip.Move_Off.z			= (int16_t) (Move_Off.z * 32767.0F);
				FVeryShortGlobalShip.Quat.w				= (int16_t) (Ships[WhoIAm].Object.Quat.w * 32767.0F);
				FVeryShortGlobalShip.Quat.x				= (int16_t) (Ships[WhoIAm].Object.Quat.x * 32767.0F);
				FVeryShortGlobalShip.Quat.y				= (int16_t) (Ships[WhoIAm].Object.Quat.y * 32767.0F);
				FVeryShortGlobalShip.Quat.z				= (int16_t) (Ships[WhoIAm].Object.Quat.z * 32767.0F);
				FVeryShortGlobalShip.Angle.x			= (int16_t) (Ships[WhoIAm].Object.Angle.x * SHORTANGLEMODIFIERPACK );
				FVeryShortGlobalShip.Angle.y			= (int16_t) (Ships[WhoIAm].Object.Angle.y * SHORTANGLEMODIFIERPACK );
				FVeryShortGlobalShip.Angle.z			= (int16_t) (Ships[WhoIAm].Object.Angle.z * SHORTANGLEMODIFIERPACK );
				FVeryShortGlobalShip.Bank				= (int16_t) (Ships[ WhoIAm ].Object.Bank * SHORTBANKMODIFIER);
				FVeryShortGlobalShip.Primary			= PrimaryToFireLookup[ Ships[WhoIAm].Primary ];
				FVeryShortGlobalShip.Secondary			= SecondaryToFireLookup[ Ships[WhoIAm].Secondary ];
				FVeryShortGlobalShip.PrimPowerLevel		= (BYTE) Ships[ WhoIAm ].PrimPowerLevel;
				
				SendGameMessage(MSG_VERYSHORTFUPDATE, 0, 0, 0, (BYTE)(Ships[WhoIAm].Object.Flags & SHIP_SecFire | SHIP_MulFire));
			}
			Ships[ WhoIAm ].Object.Flags &=  ~( SHIP_PrimFire | SHIP_SecFire | SHIP_MulFire );
			Interval = NetUpdateInterval;
		
		}
		else
		{
			Interval -= framelag;
			if( Interval <= 0.0F )
			{
				Interval = NetUpdateInterval;
				SendANormalUpdate();
			}
		}

 		HostDutyTimer -= framelag;

		if( HostDutyTimer <= 0.0F )
		{
			HostDutyTimer = NetUpdateIntervalHostDuties;
			HostDuties = false;
			// If someone has joined or somebody requests it then send everyone a stats update
			if( IsHost == true )
			{			
				for( i = 0 ; i < MAX_PLAYERS ; i++ )
				{
					if( ( i != WhoIAm ) && (GameStatus[i] == STATUS_Joining ) ) 
					{
						if( Ships[i].RegenSlots != 0 )
						{
							SendGameMessage( MSG_SHORTREGENSLOT, 0, (BYTE) i, 0, 0 );
							HostDuties = true;
							Ships[i].RegenSlots = 0;
						}
						else if( Ships[i].Triggers != 0 ) 
						{
							SendGameMessage( MSG_SHORTTRIGGER, 0, (BYTE) i, 0, 0 );
							HostDuties = true;
							Ships[i].Triggers = 0;
						}
						else if( Ships[i].TrigVars != 0 )
						{
							SendGameMessage( MSG_SHORTTRIGVAR, 0, (BYTE) i, 0, 0 );
							HostDuties = true;
							Ships[i].TrigVars = 0;
						}
						else if( Ships[i].Mines != 0 )
						{
							SendGameMessage( MSG_SHORTMINE, 0, (BYTE) i, 0, 0 );
							HostDuties = true;
							Ships[i].Mines = 0;
						}
						else if(	Ships[i].Pickups != 0 )
						{
							SendGameMessage( MSG_SHORTPICKUP, 0, (BYTE) i, 0, 0 );
							HostDuties = true;
							Ships[i].Pickups = 0;
						}
						// reset them untill we get another request.....
					}
				}
			}
		}
	}

	// The Host can Dynamicaly change settings
	if( IsHost && !CurrentMenu )
	{
		// changed packet rate
		if ( OldPPSValue != PacketsSlider.value )
		{
			OldPPSValue = PacketsSlider.value;
			NetUpdateInterval	= (60.0F / PacketsSlider.value);
			SendGameMessage(MSG_NETSETTINGS, 0, 0, 0, 0);
			AddColourMessageToQue(SystemMessageColour, "%d %s" , PacketsSlider.value , PACKETS_PER_SECOND_SET );
		}
		// changed collision perspective
		if ( OldColPerspective != ColPerspective )
		{
			OldColPerspective = ColPerspective;
			SendGameMessage(MSG_NETSETTINGS, 0, 0, 0, 0);
			if(ColPerspective == COLPERS_Forsaken)
				AddColourMessageToQue( SystemMessageColour, "SHOOTER NOW DECIDES COLLISIONS" );
			else if(ColPerspective == COLPERS_Descent)
				AddColourMessageToQue( SystemMessageColour, "TARGET NOW DECIDES COLLISIONS" );
					
		}
		// changed short packets
		if ( OldUseShortPackets != UseShortPackets )
		{
			OldUseShortPackets = UseShortPackets;
			SendGameMessage(MSG_NETSETTINGS, 0, 0, 0, 0);
			if(UseShortPackets)
				AddColourMessageToQue( SystemMessageColour, "SHORT PACKETS ENABLED" );
			else
				AddColourMessageToQue( SystemMessageColour, "SHORT PACKETS DISABLED" );
				
		}
	}
}


void PrimBullPosDirSend( u_int16_t OwnerType, u_int16_t OwnerID, u_int16_t BulletID, int8_t Weapon,
						u_int16_t Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16_t PowerLevel, float PLevel )
{
	TempPrimBullPosDir.OwnerType = OwnerType;
	TempPrimBullPosDir.OwnerID = OwnerID;
	TempPrimBullPosDir.BulletID = BulletID;
	TempPrimBullPosDir.Weapon = Weapon;
	TempPrimBullPosDir.Group = Group;
	TempPrimBullPosDir.Pos = *Pos;
	TempPrimBullPosDir.Offset = *Offset;
	TempPrimBullPosDir.Dir = *Dir;
	TempPrimBullPosDir.Up = *Up;
	TempPrimBullPosDir.PowerLevel = PowerLevel;
	TempPrimBullPosDir.PLevel = PLevel;
	SendGameMessage(MSG_PRIMBULLPOSDIR, 0, 0, 0, 0);
}

void SecBullPosDirSend( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon )
{
	TempSecBullPosDir.OwnerType = OwnerType;
	TempSecBullPosDir.Owner = Owner;
	TempSecBullPosDir.BulletID = BulletID;
	TempSecBullPosDir.Group = Group;
	TempSecBullPosDir.Pos = *Pos;
	TempSecBullPosDir.Offset = *Offset;
	TempSecBullPosDir.Dir = *Dir;
	TempSecBullPosDir.UpVector = *UpVector;
	TempSecBullPosDir.DropDir = *DropDir;
	TempSecBullPosDir.Weapon = Weapon;
	SendGameMessage(MSG_SECBULLPOSDIR, 0, 0, 0, 0);
}

void TitanBitsSend( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon, VECTOR * Directions )
{
	int16_t	Count;

	TempTitanBits.OwnerType = OwnerType;
	TempTitanBits.Owner		= Owner;
	TempTitanBits.BulletID		= BulletID;
	TempTitanBits.Group			= Group;
	TempTitanBits.Pos			= *Pos;
	TempTitanBits.Offset		= *Offset;
	TempTitanBits.UpVector	= *UpVector;
	TempTitanBits.DropDir		= *DropDir;
	TempTitanBits.Weapon		= Weapon;
	for( Count = 0; Count < NUMTITANBITS; Count++ )
		TempTitanBits.Directions[ Count ] = Directions[ Count ];

	SendGameMessage(MSG_TITANBITS, 0, 0, 0, 0);
}

void	IHitYou( BYTE you, float Damage, VECTOR * Recoil, VECTOR * Point, VECTOR * Dir, float Force, BYTE WeaponType, BYTE Weapon, bool FramelagRecoil )
{
	VECTOR	Recoil_Off;		
	if( Ships[ you ].Object.Mode != LIMBO_MODE )
	{
		if( !UseShortPackets )
		{
			if( FramelagRecoil ) TempShipHit.OneOffExternalForce = true;
			else TempShipHit.OneOffExternalForce = false;
			TempShipHit.Damage = Damage;
			TempShipHit.Recoil = *Recoil;
			TempShipHit.Point = *Point;
			TempShipHit.Point.x -= Ships[ you ].Object.Pos.x;
			TempShipHit.Point.y -= Ships[ you ].Object.Pos.y;
			TempShipHit.Point.z -= Ships[ you ].Object.Pos.z;
			TempShipHit.Dir = *Dir;
			TempShipHit.Force = Force;
			TempShipHit.WeaponType = WeaponType;
			TempShipHit.Weapon = Weapon;
			SendGameMessage( MSG_SHIPHIT, Ships[you].network_player, you, 0, 0 );
		}
		else
		{
			if( FramelagRecoil ) ShortTempShipHit.OneOffExternalForce = 1;
			else ShortTempShipHit.OneOffExternalForce = 0;
			ShortTempShipHit.Damage = Damage;

			Recoil_Off = *Recoil;
			NormaliseVector( &Recoil_Off );
			ShortTempShipHit.Recoil_Scalar = (u_int16_t) ( 256.0F * VectorLength( Recoil ) );
			ShortTempShipHit.Recoil.x = (int16_t) (Recoil_Off.x * 32767.0F);
			ShortTempShipHit.Recoil.y = (int16_t) (Recoil_Off.y * 32767.0F);
			ShortTempShipHit.Recoil.z = (int16_t) (Recoil_Off.z * 32767.0F);
			ShortTempShipHit.Point.x = (int16_t)(Point->x - Ships[ you ].Object.Pos.x);
			ShortTempShipHit.Point.y = (int16_t)(Point->y - Ships[ you ].Object.Pos.y);
			ShortTempShipHit.Point.z = (int16_t)(Point->z - Ships[ you ].Object.Pos.z);
			ShortTempShipHit.Dir.x = (int16_t) (Dir->x * 32767.0F);
			ShortTempShipHit.Dir.y = (int16_t) (Dir->y * 32767.0F);
			ShortTempShipHit.Dir.z = (int16_t) (Dir->z * 32767.0F);
			ShortTempShipHit.Force = Force;
			ShortTempShipHit.WeaponType = WeaponType;
			ShortTempShipHit.Weapon = Weapon;
			SendGameMessage( MSG_SHORTSHIPHIT, Ships[you].network_player, you, 0, 0 );

		}
	}
}


void	DropPickupSend( VECTOR * Pos, u_int16_t Group, VECTOR * Dir, float Speed, int16_t Type, u_int16_t IDCount, int16_t RegenSlot, bool Sparkle, float LifeCount, u_int16_t TriggerMod )
{
	if( !UseShortPackets )
	{
		TempPickup.IDCount = IDCount;
		TempPickup.Type = Type;
		TempPickup.Group = Group;
		TempPickup.Pos = *Pos;
		TempPickup.Dir = *Dir;
		TempPickup.Speed = Speed;
		TempPickup.RegenSlot = RegenSlot;
		TempPickup.Sparkle = Sparkle;
		TempPickup.LifeCount = LifeCount;
		TempPickup.TriggerMod = TriggerMod;
		SendGameMessage( MSG_DROPPICKUP, 0, 0, 0, 0 );
	}else{
		VeryShortTempPickup.IDCount = IDCount;
		VeryShortTempPickup.Type = (BYTE)Type;
		VeryShortTempPickup.Group = (BYTE) Group;
		VeryShortTempPickup.Pos.x = (int16_t) Pos->x;
		VeryShortTempPickup.Pos.y = (int16_t) Pos->y;
		VeryShortTempPickup.Pos.z = (int16_t) Pos->z;
		VeryShortTempPickup.Dir.x = (int16_t) (Dir->x * 32767.0F);
		VeryShortTempPickup.Dir.y = (int16_t) (Dir->y * 32767.0F);
		VeryShortTempPickup.Dir.z = (int16_t) (Dir->z * 32767.0F);
		VeryShortTempPickup.Speed = Speed;
		VeryShortTempPickup.RegenSlot = (BYTE) RegenSlot;
		VeryShortTempPickup.Sparkle = Sparkle;
		VeryShortTempPickup.LifeCount = LifeCount;
		VeryShortTempPickup.TriggerMod = TriggerMod;
		SendGameMessage( MSG_VERYSHORTDROPPICKUP, 0, 0, 0, 0 );
	}
}

void	KillPickupSend( u_int16_t Owner, u_int16_t IDCount, int16_t Style )
{
	TempKillPickup.Owner = Owner;
	TempKillPickup.IDCount = IDCount;
	TempKillPickup.Style = Style;
	SendGameMessage( MSG_KILLPICKUP, 0, 0, 0, 0 );
}

void	TeamGoalsSend( u_int16_t * TeamGoals )
{
	int i;
	for( i = 0; i < MAX_TEAMS; i++ ) TempTeamGoals.TeamGoals[ i ] = TeamGoals[ i ];
	SendGameMessage( MSG_TEAMGOALS, 0, 0, 0, 0 );
}

void	CreateShockwaveSend( u_int16_t OwnerShip, u_int16_t Owner, VECTOR * Pos, u_int16_t Group, float ShockwaveSize, BYTE Weapon )
{
	float dist;
	int i;

	TempShockwave.Owner = Owner;
	TempShockwave.Pos = *Pos;
	TempShockwave.Group = Group;
	TempShockwave.Weapon = Weapon;
	TempShockwave.ShockwaveSize = ShockwaveSize;


	// for every player in the game
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		// excluding myself and those who aren't in normal mode
		if((GameStatus[ i ] ==  STATUS_Normal) && (i != WhoIAm))
		{
			// if they are close to me
			dist = ReturnDistanceVolumeVector( &Ships[ i ].Object.Pos, Ships[ i ].Object.Group, &Ships[ Current_Camera_View ].Object.Pos, Ships[ Current_Camera_View ].Object.Group, NULL, NULL );
			if( (dist >= 0.0F) && ( dist <= 500.0F ))
			{
				// send them shockwave
				SendGameMessage(MSG_SHOCKWAVE, Ships[i].network_player, 0, 0, 0 );
			}
		}
	}
}

void	UpdateBGObjectSend( u_int16_t BGObject, int16_t State, float Time )
{
	TempBGOUpdate.BGObject = BGObject;
	TempBGOUpdate.State = State;
	TempBGOUpdate.Time = Time;
	SendGameMessage( MSG_BGOUPDATE, 0, 0, 0, 0 );
}

void	ShipDiedSend( BYTE WeaponType, BYTE Weapon )
{
	TempDied.WeaponType = WeaponType;
	TempDied.Weapon = Weapon;
	SendGameMessage( MSG_SHIPDIED, 0, 0, 0, 0 );
}

void	RequestTime( void  )
{
	SendGameMessage( MSG_REQTIME, 0, 0, 0, 0 );
}

void	SetTime( float Time )
{
	TempTimeSet.Time = Time;
	SendGameMessage( MSG_SETTIME, 0, 0, 0, 0 );
}

extern float real_framelag;
static float tracker_timer = 0;
void reset_tracker()
{
	tracker_timer = 0;
}
void update_tracker()
{
	if(!IsHost || !tracker_enabled)
		return;
	if( tracker_timer <= 0 )
	{
		send_tracker_update( tracker_server, tracker_port );
		tracker_timer = 20; // 3 times a minute, timeout is 1 minute
	}
	tracker_timer -= real_framelag; // minus elapsed time
}

// (stats.c)
extern void InitScoreSortTab(int Player); 

void SetupNetworkGame()
{
	int16_t i,Count;

	host_network_player = NULL;

	for( i = 0 ; i < 256 ; i++ )
	{
		RealPacketSize[i] = 0;
	}

	RealPacketSize[MSG_UPDATE]                         = sizeof(UPDATEMSG);
	RealPacketSize[MSG_HEREIAM]                        = sizeof(HEREIAMMSG);
	RealPacketSize[MSG_INIT]                           = sizeof(INITMSG);
	RealPacketSize[MSG_SHIPHIT]                        = sizeof(SHIPHITMSG);
	RealPacketSize[MSG_PRIMBULLPOSDIR]                 = sizeof(PRIMBULLPOSDIRMSG);
	RealPacketSize[MSG_SECBULLPOSDIR]                  = sizeof(SECBULLPOSDIRMSG);
	RealPacketSize[MSG_SHIPDIED]                       = sizeof(SHIPDIEDMSG);
	RealPacketSize[MSG_DROPPICKUP]                     = sizeof(DROPPICKUPMSG);
	RealPacketSize[MSG_KILLPICKUP]                     = sizeof(KILLPICKUPMSG);
	RealPacketSize[MSG_STATUS]                         = sizeof(STATUSMSG);
	RealPacketSize[MSG_NETSETTINGS]                    = sizeof(NETSETTINGSMSG);
	RealPacketSize[MSG_SHORTPICKUP]                    = sizeof(SHORTPICKUPMSG);
	RealPacketSize[MSG_SHOCKWAVE]                      = sizeof(SHOCKWAVEMSG);
	RealPacketSize[MSG_FUPDATE]                        = sizeof(FUPDATEMSG);
	RealPacketSize[MSG_SHORTMINE]                      = sizeof(SHORTMINEMSG);
	RealPacketSize[MSG_TEXTMSG]                        = sizeof(TEXTMSG);
	RealPacketSize[MSG_SHORTREGENSLOT]                 = sizeof(SHORTREGENSLOTMSG);
	RealPacketSize[MSG_SHORTTRIGGER]                   = sizeof(SHORTTRIGGERMSG);
	RealPacketSize[MSG_SHORTTRIGVAR]                   = sizeof(SHORTTRIGVARMSG);
#ifdef DEMO_SUPPORT
	RealPacketSize[MSG_INTERPOLATE]                    = sizeof(INTERPOLATEMSG);
	RealPacketSize[MSG_VERYSHORTINTERPOLATE]           = sizeof(VERYSHORTINTERPOLATEMSG);
#endif
	RealPacketSize[MSG_BGOUPDATE]                      = sizeof(BGOUPDATEMSG);
	RealPacketSize[MSG_LONGSTATUS]                     = sizeof(LONGSTATUSMSG);
	RealPacketSize[MSG_SETTIME]                        = sizeof(SETTIMEMSG);
	RealPacketSize[MSG_REQTIME]                        = sizeof(REQTIMEMSG);
	RealPacketSize[MSG_BIKENUM]                        = sizeof(BIKENUMMSG);
	RealPacketSize[MSG_VERYSHORTUPDATE]                = sizeof(VERYSHORTUPDATEMSG);
	RealPacketSize[MSG_VERYSHORTFUPDATE]               = sizeof(VERYSHORTFUPDATEMSG);
	RealPacketSize[MSG_TEAMGOALS]                      = sizeof(TEAMGOALSMSG);
	RealPacketSize[MSG_YOUQUIT]                        = sizeof(YOUQUITMSG);
	RealPacketSize[MSG_SHORTSHIPHIT]                   = sizeof(SHORTSHIPHITMSG);
	RealPacketSize[MSG_TITANBITS]                      = sizeof(TITANBITSMSG);
	RealPacketSize[MSG_GROUPONLY_VERYSHORTFUPDATE]     = sizeof(GROUPONLY_VERYSHORTFUPDATEMSG);
	RealPacketSize[MSG_VERYSHORTDROPPICKUP]            = sizeof(VERYSHORTDROPPICKUPMSG);
    RealPacketSize[MSG_SHIPHEALTH]                     = sizeof(SHIPHEALTHMSG);

	for( i = 0; i < 256; i++ )
	{
		if( RealPacketSize[i] < 1 ) continue;
		DebugPrintf("Packet Size: %s \t\t= %d\n",msg_to_str(i),RealPacketSize[i]);
	}

	if ( Debug )
		for( i = 0 ; i < 256 ; i++ )
		{
			PacketGot[i] = 0;
			PacketSize[i] = 0;

		}

	memset(&Ships,			0,				sizeof(Ships) );
	memset(&Names,			0,				sizeof(SHORTNAMETYPE) );
	memset(&GameStatus,		STATUS_Null,	sizeof(GameStatus) );
    memset(&PlayerHealths,  0,              sizeof(PlayerHealths));
    memset(&ShipHealthColourInterval, 0.0F, sizeof(ShipHealthColourInterval));

	JustGenerated = true;
	
	for(i=0; i<(MAX_PLAYERS+1); i++)
	{
		// everyone starts off normal....
		GameStatus[i] = STATUS_Null;
		CanDoDamage[i] = true;
		Ships[i].BikeNum = ( i % MAXBIKETYPES );
		Ships[i].ModelNum = (u_int16_t) -1;
		Ships[i].Object.Type = OBJECT_TYPE_SHIP;
		Ships[i].Object.Mode = NORMAL_MODE;
		Ships[i].Object.Bank = 0.0F;
		Ships[i].Object.Shield	= Start_Shield;
		Ships[i].Object.Hull	= Start_Hull;
		Ships[i].JustRecievedPacket = true;
		Ships[i].Object.light = (u_int16_t) -1;
		for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;
		Ships[i].NumMultiples = 0;
		MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.Quat );
		Ships[i].Object.Pos.x = 0.0F;;
		Ships[i].Object.Pos.y = 0.0F;;
		Ships[i].Object.Pos.z = 0.0F;;
		Ships[i].Object.Group = 0;
		Ships[i].enable = 0;
		Ships[i].Object.Mat = TempMatrix;
		Ships[i].Object.FinalMat = TempMatrix;
		Ships[i].Object.FinalInvMat = TempMatrix;
		// reset external and internal force vectors to ship movement
		Ships[i].Object.ExternalForce.x = 0.0F;
		Ships[i].Object.ExternalForce.y = 0.0F;
		Ships[i].Object.ExternalForce.z = 0.0F;
		Ships[i].Object.InternalForce.x = 0.0F;
		Ships[i].Object.InternalForce.y = 0.0F;
		Ships[i].Object.InternalForce.z = 0.0F;
		Ships[i].Object.Autolevel = 0.0F;
		Ships[i].Object.BobCount = 0.0F;

		for( Count = 0; Count < 12; Count++ ) Ships[i].TempLines[ Count ] = (u_int16_t) -1;
	
		// (stats.c)
		InitScoreSortTab((int) i);

        PlayerHealths[i].WhoIAm = i;
        PlayerHealths[i].Hull = 0;
        PlayerHealths[i].Shield = 0;
	}

	reset_tracker();
}

void InitShipStructure( int i , bool ResetScore )
{
	int16_t Count;


//	memset(&Ships[i], 0, sizeof(GLOBALSHIP) );

	CanDoDamage[i] = true;

	Ships[i].BikeNum = ( i % MAXBIKETYPES );
	Ships[i].ShieldHullCount = 0;
	
	Ships[i].Object.Type = OBJECT_TYPE_SHIP;
	Ships[i].Object.Mode = NORMAL_MODE;
	Ships[i].Object.Bank = 0.0F;
	Ships[i].Object.Shield	= Start_Shield;
	Ships[i].Object.Hull	= Start_Hull;
	Ships[i].JustRecievedPacket = true;
	Ships[i].Object.light = (u_int16_t) -1;
	for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;
	Ships[i].NumMultiples = 0;
	MakeQuat( 0.0F, 0.0F, 0.0F, &Ships[i].Object.Quat );
	
	Ships[i].Object.Pos.x = 0.0F;;
	Ships[i].Object.Pos.y = 0.0F;;
	Ships[i].Object.Pos.z = 0.0F;;
	Ships[i].Object.Group = 0;
	
	Ships[i].enable = 0;

	Ships[i].Object.Mat = TempMatrix;
	Ships[i].Object.FinalMat = TempMatrix;
	Ships[i].Object.FinalInvMat = TempMatrix;

	// reset external and internal force vectors to ship movement
	Ships[i].Object.ExternalForce.x = 0.0F;
	Ships[i].Object.ExternalForce.y = 0.0F;
	Ships[i].Object.ExternalForce.z = 0.0F;
	Ships[i].Object.InternalForce.x = 0.0F;
	Ships[i].Object.InternalForce.y = 0.0F;
	Ships[i].Object.InternalForce.z = 0.0F;
	Ships[i].Object.Autolevel = 0.0F;
	Ships[i].Object.BobCount = 0.0F;

	PrevHull = 0;
	PrevShield = 0;
	
	if( ResetScore )
		ResetIndividualStats(i); // (stats.c)

	for( Count = 0; Count < 12; Count++ ) Ships[i].TempLines[ Count ] = (u_int16_t) -1;

	Ships[i].network_player = NULL;
}




void DestroyGame( void )
{
	int	i;
	VECTOR	DirVector = { 0.0F, 0.0F, 0.0F };

	DebugPrintf("DestroyGame()\n");

	if( PlayDemo )
		return;
	
	MyGameStatus = STATUS_Left;
	IsHost = false;

	if ( WhoIAm < MAX_PLAYERS )
	{
		DebugPrintf("Destroy game pos 1\n");

		Ships[WhoIAm].enable = 0;

		KillOwnersSecBulls( WhoIAm );

		if( GodMode ) LoseAllWeapons();

		ScatterWeapons( &DirVector, MAXPICKUPS );
		RegeneratePickups();

		ResetAllStats(); // stats.c
	}

	network_cleanup();

	if( Debug )
		for( i = 0 ; i < 256 ; i++ )
			if( PacketGot[i] )
				DebugPrintf("num %3d quantity %12d size %12d\n", i, PacketGot[i] , PacketSize[i] );

}



void initShip( u_int16_t i )
{
	u_int16_t	spos;
	int16_t	Count;

	Ships[i].Object.NodeNetwork = 0;
	Ships[i].Object.NearestNode = NULL;
	Ships[i].Object.Type = OBJECT_TYPE_SHIP;

    Ships[i].enable = 1;
    Ships[i].Pickups = ( ( MAXPICKUPS + ( MAXGENPICKUPCOUNT - 1 ) ) / MAXGENPICKUPCOUNT	);
    Ships[i].RegenSlots = ( ( NumRegenPoints + ( MAXGENREGENSLOTCOUNT - 1 ) ) / MAXGENREGENSLOTCOUNT );
    Ships[i].Mines = ( ( MAXSECONDARYWEAPONBULLETS + ( MAXGENMINECOUNT - 1 ) ) / MAXGENMINECOUNT );
    Ships[i].Triggers = ( ( NumOfTriggers + ( MAXGENTRIGGERCOUNT - 1 ) ) / MAXGENTRIGGERCOUNT );
    Ships[i].TrigVars = ( ( NumOfTrigVars + ( MAXGENTRIGVARCOUNT - 1 ) ) / MAXGENTRIGVARCOUNT );

    Ships[i].NumMultiples = 0;
	for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;

	if ( Mloadheader.state == true)
	{
		spos = (i+RandomStartPosModify) % num_start_positions;
		InitShipStartPos( i, spos );
	}
	else
	{
		InitShipStartPos( i, 0 );
	}

	PrevHull = 0;
	PrevShield = 0;
	KillAllPickups();
}
void smallinitShip( u_int16_t i )
{
	u_int16_t	spos;
	int16_t	Count;

	Ships[i].Object.NodeNetwork = 0;
	Ships[i].Object.NearestNode = NULL;
	Ships[i].Object.Type = OBJECT_TYPE_SHIP;

    Ships[i].enable = 1;
    Ships[i].Pickups = 0;
    Ships[i].RegenSlots = 0;
    Ships[i].Mines = 0;
    Ships[i].Triggers = 0;
    Ships[i].TrigVars = 0;
    Ships[i].NumMultiples = 0;
	for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[i].OrbModels[ Count ] = (u_int16_t) -1;

	NumMissedPickups = 0;
	NumMissedInitPickups = 0;
	NumMissedKillPickups = 0;

	NumMissedMines = 0;
	NumMissedInitMines = 0;
	NumMissedKillMines = 0;

	if ( Mloadheader.state == true)
	{
		spos = (i+RandomStartPosModify) % num_start_positions;
		InitShipStartPos( i, spos );
	}
	else
	{
		InitShipStartPos( i, 0 );
	}
	PrevHull = 0;
	PrevShield = 0;
}

void UpdatePlayer( network_player_t * from, BYTE ShipNum )
{
	if( ! Ships[ShipNum].network_player )
		Ships[ShipNum].network_player = from;
	if( Names[ShipNum][0] == 0 )
		set_player_name( ShipNum, from->name );
}

void network_event_player_name( network_player_t * player )
{
	int i;
	DebugPrintf("network_event_player_name\n");
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		if( ( i != WhoIAm ) && (player == Ships[i].network_player) )
		{
			DebugPrintf("Recieved name %s from player %d\n", player->name, i);
			set_player_name( i, player->name );
			NextworkOldBikeNum = -1;
			return;
		}
	}
	return;
}

void network_event_player_left( network_player_t * player )
{
	int i;

	// we have left the game
	if( player == NULL )
	{
		if( MyGameStatus == STATUS_Normal )
			AddColourMessageToQue(SystemMessageColour, "You have left the game!" );
	}

	// someone left the game
	else
	{
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( ( i != WhoIAm ) && (player == Ships[i].network_player) )
			{	
				if( MyGameStatus == STATUS_Normal )
	   				AddColourMessageToQue(SystemMessageColour, "%s %s", &Names[i][0] , HAS_LEFT_THE_GAME );

				if( Ships[i].Object.light != (u_int16_t) -1  )
				{
					KillUsedXLight(Ships[i].Object.light);
					Ships[i].Object.light = (u_int16_t) -1;
				}

				// Meth: Interesting seems that they toyd with idea of
				//       blowing up the players mines and bullets when
				//       they leave the game.
				//KillOwnersSecBulls( (u_int16_t) i );

				Ships[i].enable = 0;

				if ( GameStatus[i] == STATUS_StartingMultiplayer )
					GameStatus[i] = STATUS_Null;	// ensure slot is freed up if player has quit from titles

				else if( GameStatus[i] != STATUS_Left )
					GameStatus[i] = STATUS_LeftCrashed;

				InitShipStructure(i , false );

				//memset(Names[i],0,sizeof(Names[i]));
			}
		}
		reset_tracker();
	}
}

void network_event_new_host( network_player_t * player )
{
	int i;
	DebugPrintf("network_event_new_host\n");

	// if we need to send to host later
	host_network_player = player;

	// we have become the host
	if( player == NULL)
	{
		switch ( MyGameStatus )
		{
		case STATUS_StartingMultiplayer:
			if ( TeamGame )	PrintErrorMessage ( YOU_HAVE_BECOME_THE_HOST , 0, NULL, ERROR_DONTUSE_MENUFUNCS );
			else			PrintErrorMessage ( YOU_HAVE_BECOME_THE_HOST , 1, &MENU_NEW_HostWaitingToStart, ERROR_DONTUSE_MENUFUNCS );
			break;
		default:
			AddColourMessageToQue( SystemMessageColour, YOU_HAVE_BECOME_THE_HOST );
		}

		IsHost = true;					// I have Become the host

		PacketsSlider.value = (int) (60.0F / NetUpdateInterval);
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( ( i != WhoIAm ) && ( Ships[i].Object.Flags & SHIP_IsHost ) )
			{
				Ships[i].enable = 0;
				GameStatus[i] = STATUS_Left;
			}
		}

		reset_tracker();
	}

	// someone else has become the host
	else
	{
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
			if( player == Ships[i].network_player )
			{
				AddColourMessageToQue(SystemMessageColour, "%s %s", &Names[i][0] , "has become the host." );
				break;
			}
	}
}

void network_event_player_joined( network_player_t * player )
{
	DebugPrintf("network_event_player_joined\n");
	
	// we have joined the game
	if( player == NULL )
	{
		DebugPrintf("We have joined the game...\n");

		MyGameStatus = STATUS_GetPlayerNum;
		GetPlayerNumCount1 = 0.0F;
		GetPlayerNumCount2 = 60.0F * 30.0F;	// 30 Seconds...
		GetPlayerNumCount = 0;

		// Stops us going straight in to the game....
		OverallGameStatus = STATUS_Null;
	}

	// someone else joined the game
	else
	{
		DebugPrintf("network_event_player_joined: player '%s' joined the game.\n", player->name);
		if( MyGameStatus == STATUS_Normal && !TeamGame )
		{
			AddColourMessageToQue(SystemMessageColour, "%s %s", player->name, IS_JOINING_THE_GAME );
		}
	}
}

void network_event_new_message( network_player_t * from, BYTE * data, int size )
{
	//DebugPrintf("network_event_new_message: type = %s\n",msg_to_str(*data));
#ifdef DEMO_SUPPORT
	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	if( RecordDemo && ( MyGameStatus == STATUS_Normal ) )
	{
		TempTime -= GameStartedTime;
		Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
		Demo_fwrite( &size, sizeof(size), 1, DemoFp );
		Demo_fwrite( &from_network_id, sizeof(from_network_id), 1, DemoFp );
		Demo_fwrite( MsgPnt, nBytes , 1, DemoFp );
	}
#endif
	RecPacketSize = size;
	if ( RecPacketSize > MaxRecPacketSize )
		MaxRecPacketSize = RecPacketSize;
	BytesPerSecRec += size;
    EvaluateMessage( from, size, data );
}

void network_event( network_event_type_t type, void* data )
{
        network_player_t * player = (network_player_t *) data;  // used by most events
        switch( type )
        {
        case NETWORK_JOIN:
                network_event_player_joined( player );
                break;
        case NETWORK_LEFT:
                network_event_player_left( player );
                break;
        case NETWORK_HOST:
                network_event_new_host( player );
                break;
	case NETWORK_NAME:
                network_event_player_name( player );
                break;
        case NETWORK_DATA:
                {
                        network_packet_t * packet = (network_packet_t*) data;
						network_event_new_message( packet->from, packet->data, packet->size );
                }
                break;
        default:
			DebugPrintf("network_event: unknown network event type %d\n", type);
        }
}

void ReceiveGameMessages( void )
{
	int i;

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
		OldGameStatus[i] = GameStatus[i];

	if( MyGameStatus == STATUS_Normal )
	{
		BytesPerSecTimer -= framelag;
		if( BytesPerSecTimer < 0.0F )
		{
			BytesPerSecTimer = 71.0F;
			CurrentBytesPerSecRec = BytesPerSecRec;
			CurrentBytesPerSecSent = BytesPerSecSent;

			if( CurrentBytesPerSecRec > MaxCurrentBytesPerSecRec )
				MaxCurrentBytesPerSecRec = CurrentBytesPerSecRec;
			if( CurrentBytesPerSecSent > MaxCurrentBytesPerSecSent )
				MaxCurrentBytesPerSecSent = CurrentBytesPerSecSent;
			BytesPerSecRec = 0;
			BytesPerSecSent = 0;
		}

		// Stuff to handle Kills And deaths....
		if( NextworkOldBikeNum != Ships[WhoIAm].BikeNum )
		{
			NextworkOldBikeNum = Ships[WhoIAm].BikeNum;
			SendGameMessage( MSG_BIKENUM, 0, 0, 0, 0 );
		}
	}
	else
	{
		BytesPerSecRec					= 0;
		BytesPerSecSent					= 0;
		CurrentBytesPerSecRec			= 0;
		CurrentBytesPerSecSent		= 0;
		MaxCurrentBytesPerSecRec	= 0;
		MaxCurrentBytesPerSecSent	= 0;
		RecPacketSize = MaxRecPacketSize = 0;
		BytesPerSecTimer = 71.0F;
	}

	update_tracker();

	network_pump();

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		if( OldGameStatus[i] == STATUS_Joining && GameStatus[i] != STATUS_Joining )
		{
			// $$$
			NextworkOldBikeNum = -1;
		}
	}
}

extern px_timer_t last_mine_timer;

void EvaluateMessage( network_player_t * from, DWORD len , BYTE * MsgPnt )
{
    LPSHIPHEALTHMSG                 lpShipHealth;
    LPUPDATEMSG							lpUpdate;
    LPVERYSHORTUPDATEMSG			lpVeryShortUpdate;
    LPFUPDATEMSG						lpFUpdate;
    LPVERYSHORTFUPDATEMSG		lpVeryShortFUpdate;
    LPGROUPONLY_VERYSHORTFUPDATEMSG		lpGroupOnly_VeryShortFUpdate;
    LPINITMSG								lpInit;
    LPSHIPHITMSG							lpShipHit;
    LPSHORTSHIPHITMSG				lpShortShipHit;
    LPSHIPDIEDMSG						lpShipDied;
    LPPRIMBULLPOSDIRMSG				lpPrimBullPosDir;
    LPSECBULLPOSDIRMSG				lpSecBullPosDir;
    LPTITANBITSMSG						lpTitanBits;
    LPDROPPICKUPMSG					lpDropPickup;
    LPVERYSHORTDROPPICKUPMSG   lpVeryShortDropPickup;
    LPKILLPICKUPMSG						lpKillPickup;
    LPTEAMGOALSMSG					lpTeamGoals;
    LPSHOCKWAVEMSG					lpShockwave;
    LPBGOUPDATEMSG					lpBGOUpdate;
    LPSTATUSMSG							lpStatus;
    LPLONGSTATUSMSG					lpLongStatus;
    LPSHORTPICKUPMSG					lpShortPickup;
    LPSHORTREGENSLOTMSG			lpShortRegenSlot;
    LPSHORTTRIGGERMSG				lpShortTrigger;
    LPSHORTTRIGVARMSG				lpShortTrigVar;
    LPSHORTMINEMSG					lpShortMine;
    LPTEXTMSG								lpTextMsg;
#ifdef DEMO_SUPPORT
	LPINTERPOLATEMSG					lpInterpolate;
	LPVERYSHORTINTERPOLATEMSG	lpVeryShortInterpolate;
#endif
	LPBIKENUMMSG						lpBikeNumMsg;
	LPYOUQUITMSG						lpYouQuitMsg;
	LPNETSETTINGSMSG					lpNetSettingsMsg;
	int					i;
	BYTE				OldMode;
	BYTE				OldStatus;
	int16_t				Count;
    char				methodstr[256];
	char				teamstr[256];
    LPSETTIMEMSG	lpSetTime;
    LPREQTIMEMSG	lpReqTime;
	VECTOR	Point;
	VECTOR	Recoil;
	VECTOR	Dir;
	float	Force;
	u_int16_t	Pickup;

	switch (*MsgPnt)
	{
		case MSG_SHIPHEALTH:
			DebugPrintf("received msg_shiphealth\n");
			lpShipHealth = (LPSHIPHEALTHMSG)MsgPnt;
			if( lpShipHealth->WhoIAm != WhoIAm)
			{
				if(lpShipHealth->Hull < PlayerHealths[lpShipHealth->WhoIAm].Hull ||
					lpShipHealth->Shield < PlayerHealths[lpShipHealth->WhoIAm].Shield )
				{
					ShipHealthColour[lpShipHealth->WhoIAm] = RED;
					ShipHealthColourInterval[lpShipHealth->WhoIAm] = 30.0F;
				}
				else if(lpShipHealth->Hull > PlayerHealths[lpShipHealth->WhoIAm].Hull ||
					lpShipHealth->Shield > PlayerHealths[lpShipHealth->WhoIAm].Shield )
				{
					ShipHealthColour[lpShipHealth->WhoIAm] = GREEN;
					ShipHealthColourInterval[lpShipHealth->WhoIAm] = 30.0F;
				}
				
				PlayerHealths[lpShipHealth->WhoIAm].Hull = lpShipHealth->Hull;
				PlayerHealths[lpShipHealth->WhoIAm].Shield = lpShipHealth->Shield;
			}
			return;
	}
     
	// check if it's a valid packet

	if( ! msg_is_valid( *MsgPnt ) )
	{
		DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping for unknown type: %d\n",
			from->name, from->ip, from->port, *MsgPnt );
		return;
	}

	// check the size of the packet is proper for message type

	if( RealPacketSize[*MsgPnt] != len )
	{
		DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) for invalid size of %d expected %d\n",
			from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt, len, RealPacketSize[*MsgPnt]);
		return;
	}

	// check player id is valid

	{
		int id = *(MsgPnt+1);

		// make sure player id in valid range

		if ( *MsgPnt != MSG_HEREIAM && ( id < 0 || id >= MAX_PLAYERS ))
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) for player id being out of range: %d\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt, id );
			return;
		}

		// make sure packet doens't appear to come from my self

		if ( id == WhoIAm ) // next check would also catch this since GameStatus[] isn't maintained for your self..
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) for using my player id: %d\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt, id );
			return;
		}

		// make sure player id is a valid player

		if ( 
			*MsgPnt != MSG_HEREIAM && // player id not valid yet
			*MsgPnt != MSG_INIT && *MsgPnt != MSG_STATUS && *MsgPnt != MSG_LONGSTATUS && // will add to following structures
			(GameStatus[id] == STATUS_Left || GameStatus[id] == STATUS_LeftCrashed || GameStatus[id] == STATUS_Null) 
		){
			DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) for using inactive player id: %d\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt, id );
			return;
		}
	}

	// messages that only the host should send to you

	switch (*MsgPnt)
	{
#ifdef DEMO_SUPPORT
	case MSG_INTERPOLATE:
	case MSG_VERYSHORTINTERPOLATE:
#endif
	case MSG_NETSETTINGS:
	case MSG_TEAMGOALS:
	case MSG_YOUQUIT:
	case MSG_INIT:
	case MSG_SETTIME:
	case MSG_SHORTPICKUP:
	case MSG_SHORTMINE:
	case MSG_SHORTTRIGVAR:
	case MSG_SHORTTRIGGER:
	case MSG_SHORTREGENSLOT:
	case MSG_LONGSTATUS:

		if(IsHost || (host_network_player != NULL && host_network_player != from))
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) because only the host ",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt);
			if(IsHost)
			{
				DebugPrintf_("(ME)");
			}
			else
			{
				DebugPrintf_("%s (%s:%n)",
					host_network_player->name, host_network_player->ip, host_network_player->port );
			}
			DebugPrintf_(" is allowed to send this.\n");
			return;
		}

	default:
		break;
	}

	// messages that only the host should receive

	switch (*MsgPnt)
	{
	case MSG_REQTIME:
	case MSG_HEREIAM:
		if( ! IsHost )
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) dropping %s (%d) because I am not the host...\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt );
		}
	default:
		break;
	}

	//DebugPrintf("EvaluateMessage: message %s got past initial checks\n",msg_to_str(*MsgPnt));
				

	// set flag sfx volume
	FlagVolume = FlagSfxSlider.value / ( FlagSfxSlider.max / GLOBAL_MAX_SFX );
	
	if( *(MsgPnt+1) < MAX_PLAYERS )
	{
		timer_run( &LastPacketTime[*(MsgPnt+1)] );

		if ( Debug )
		{
			PacketGot[*MsgPnt]++;
			PacketSize[*MsgPnt]+=len;
		}

	}

	
	if( MyGameStatus == STATUS_GetPlayerNum )
 		if ( *MsgPnt != MSG_INIT )
			return;

	if  ( ( ( ( MyGameStatus >= STATUS_InitView_0 ) && ( MyGameStatus <= STATUS_InitView_9 ) ) ||
		( MyGameStatus == STATUS_Joining ) ||
		( MyGameStatus == STATUS_PreStartingMultiplayerSynch ) ||
		( MyGameStatus == STATUS_WaitingToStartMultiPlayerClient ) ||
		( MyGameStatus == STATUS_WaitingToStartTeamGame ) ) )
	{
		switch (*MsgPnt)
		{
		    case MSG_SECBULLPOSDIR:
				lpSecBullPosDir = (LPSECBULLPOSDIRMSG)MsgPnt;

				if( SecondaryWeaponAttribs[ lpSecBullPosDir->SecBullPosDir.Weapon ].SecType == SEC_MINE )
				{
					if( NumMissedMines != MAXMISSEDMINES )
					{
						DebugPrintf( "MissedInitMine() %d\n", NumMissedMines );

						MissedInitMines[ NumMissedInitMines ].OwnerType	= lpSecBullPosDir->SecBullPosDir.OwnerType;
						MissedInitMines[ NumMissedInitMines ].Owner		= lpSecBullPosDir->SecBullPosDir.Owner;
						MissedInitMines[ NumMissedInitMines ].BulletID		= lpSecBullPosDir->SecBullPosDir.BulletID;
						MissedInitMines[ NumMissedInitMines ].Group			= lpSecBullPosDir->SecBullPosDir.Group;
						MissedInitMines[ NumMissedInitMines ].Pos			= lpSecBullPosDir->SecBullPosDir.Pos;
						MissedInitMines[ NumMissedInitMines ].Offset		= lpSecBullPosDir->SecBullPosDir.Offset;
						MissedInitMines[ NumMissedInitMines ].Dir				= lpSecBullPosDir->SecBullPosDir.Dir;
						MissedInitMines[ NumMissedInitMines ].UpVector	= lpSecBullPosDir->SecBullPosDir.UpVector;
						MissedInitMines[ NumMissedInitMines ].DropDir		= lpSecBullPosDir->SecBullPosDir.DropDir;
						MissedInitMines[ NumMissedInitMines ].Weapon		= lpSecBullPosDir->SecBullPosDir.Weapon;
						MissedMines[ NumMissedMines ].Type = MM_TYPE_INIT;
						MissedMines[ NumMissedMines ].Struct = &MissedInitMines[ NumMissedInitMines ];
						NumMissedInitMines++;
						NumMissedMines++;
					}
					else
					{
			   			DebugPrintf( "Overflowed mine que ( INIT )\n" );
					}
				}
				return;
				break;

			case MSG_DROPPICKUP:
				lpDropPickup = (LPDROPPICKUPMSG)MsgPnt;

				if( NumMissedPickups != MAXMISSEDPICKUPS )
		 		{
		 			MissedInitPickups[ NumMissedInitPickups ].Pos				= lpDropPickup->PickupInfo.Pos;
		 			MissedInitPickups[ NumMissedInitPickups ].Group			= lpDropPickup->PickupInfo.Group;
		 			MissedInitPickups[ NumMissedInitPickups ].Dir				= lpDropPickup->PickupInfo.Dir;
		 			MissedInitPickups[ NumMissedInitPickups ].Speed			= lpDropPickup->PickupInfo.Speed;
		 			MissedInitPickups[ NumMissedInitPickups ].Type				= lpDropPickup->PickupInfo.Type;
		 			MissedInitPickups[ NumMissedInitPickups ].Owner			= lpDropPickup->WhoIAm;
		 			MissedInitPickups[ NumMissedInitPickups ].ID					= lpDropPickup->PickupInfo.IDCount;
		 			MissedInitPickups[ NumMissedInitPickups ].RegenSlot		= lpDropPickup->PickupInfo.RegenSlot;
		 			MissedInitPickups[ NumMissedInitPickups ].Sparkle			= lpDropPickup->PickupInfo.Sparkle;
		 			MissedInitPickups[ NumMissedInitPickups ].LifeCount		= lpDropPickup->PickupInfo.LifeCount;
		 			MissedInitPickups[ NumMissedInitPickups ].TriggerMod		= lpDropPickup->PickupInfo.TriggerMod;
		 			MissedPickups[ NumMissedPickups ].Type		= MPU_TYPE_INIT;
		 			MissedPickups[ NumMissedPickups ].Struct	= &MissedInitPickups[ NumMissedInitPickups ];
#if DEBUG_PICKUPS
	   	 	   		DebugPrintf( "Added Init '%s' to Que\n", Messages[ lpDropPickup->PickupInfo.Type ] );
#endif
		 	   		NumMissedInitPickups++;
		 	   		NumMissedPickups++;
		 	   	}
		 	   	else
		 	   	{
#if DEBUG_PICKUPS
   	 				DebugPrintf( "Overflowed pickup que ( INIT )\n" );
#endif
	 			}
				return;
				break;

			case MSG_VERYSHORTDROPPICKUP:
				lpVeryShortDropPickup = (LPVERYSHORTDROPPICKUPMSG)MsgPnt;

				if( NumMissedPickups != MAXMISSEDPICKUPS )
		 		{
		 			MissedInitPickups[ NumMissedInitPickups ].Pos.x			= (float)lpVeryShortDropPickup->PickupInfo.Pos.x;
		 			MissedInitPickups[ NumMissedInitPickups ].Pos.y			= (float)lpVeryShortDropPickup->PickupInfo.Pos.y;
		 			MissedInitPickups[ NumMissedInitPickups ].Pos.z			= (float)lpVeryShortDropPickup->PickupInfo.Pos.z;
		 			MissedInitPickups[ NumMissedInitPickups ].Dir.x				= lpVeryShortDropPickup->PickupInfo.Dir.x * ONEOVER32767;
		 			MissedInitPickups[ NumMissedInitPickups ].Dir.y				= lpVeryShortDropPickup->PickupInfo.Dir.y * ONEOVER32767;
		 			MissedInitPickups[ NumMissedInitPickups ].Dir.z				= lpVeryShortDropPickup->PickupInfo.Dir.z * ONEOVER32767;
		 			MissedInitPickups[ NumMissedInitPickups ].Group			= lpVeryShortDropPickup->PickupInfo.Group;
		 			MissedInitPickups[ NumMissedInitPickups ].Speed			= lpVeryShortDropPickup->PickupInfo.Speed;
		 			MissedInitPickups[ NumMissedInitPickups ].Type				= lpVeryShortDropPickup->PickupInfo.Type;
		 			MissedInitPickups[ NumMissedInitPickups ].Owner			= lpVeryShortDropPickup->WhoIAm;
		 			MissedInitPickups[ NumMissedInitPickups ].ID					= lpVeryShortDropPickup->PickupInfo.IDCount;
		 			MissedInitPickups[ NumMissedInitPickups ].RegenSlot		= lpVeryShortDropPickup->PickupInfo.RegenSlot;
		 			MissedInitPickups[ NumMissedInitPickups ].Sparkle			= lpVeryShortDropPickup->PickupInfo.Sparkle;
		 			MissedInitPickups[ NumMissedInitPickups ].LifeCount		= lpVeryShortDropPickup->PickupInfo.LifeCount;
		 			MissedInitPickups[ NumMissedInitPickups ].TriggerMod		= lpVeryShortDropPickup->PickupInfo.TriggerMod;
		 			MissedPickups[ NumMissedPickups ].Type		= MPU_TYPE_INIT;
		 			MissedPickups[ NumMissedPickups ].Struct	= &MissedInitPickups[ NumMissedInitPickups ];
#if DEBUG_PICKUPS
	   	 	   		DebugPrintf( "Added Init '%s' to Que\n", Messages[ lpVeryShortDropPickup->PickupInfo.Type ] );
#endif
		 	   		NumMissedInitPickups++;
		 	   		NumMissedPickups++;
		 	   	}
		 	   	else
		 	   	{
#if DEBUG_PICKUPS
   	 				DebugPrintf( "Overflowed pickup que ( INIT )\n" );
#endif
	 			}
				return;
				break;

			case MSG_KILLPICKUP:
   				lpKillPickup = (LPKILLPICKUPMSG)MsgPnt;
   				if( NumMissedPickups != MAXMISSEDPICKUPS )
   				{
   					MissedKillPickups[ NumMissedKillPickups ].Owner = lpKillPickup->KillPickupInfo.Owner;
   					MissedKillPickups[ NumMissedKillPickups ].ID		= lpKillPickup->KillPickupInfo.IDCount;
   					MissedKillPickups[ NumMissedKillPickups ].Style	= lpKillPickup->KillPickupInfo.Style;
   					MissedPickups[ NumMissedPickups ].Type			= MPU_TYPE_KILL;
   					MissedPickups[ NumMissedPickups ].Struct		= &MissedKillPickups[ NumMissedKillPickups ];
#if DEBUG_PICKUPS
					DebugPrintf( "Added Kill pickup of Owner '%s', ID %d, to que\n",
								&Names[ lpKillPickup->KillPickupInfo.Owner ][ 0 ],
								lpKillPickup->KillPickupInfo.IDCount );
#endif
   					NumMissedKillPickups++;
   					NumMissedPickups++;
   				}
   				else
   				{
#if DEBUG_PICKUPS
   					DebugPrintf( "Overflowed pickup que ( KILL )\n" );
#endif
				}
				return;
				break;


			case MSG_SHORTREGENSLOT:
			case MSG_SHORTTRIGGER:
			case MSG_SHORTTRIGVAR:
			case MSG_SHORTMINE:
			case MSG_SHORTPICKUP:
			case MSG_YOUQUIT:
			case MSG_BIKENUM:
			case MSG_STATUS:
			case MSG_LONGSTATUS:
			case MSG_TEAMGOALS:
				break;

			default:
				return;
		}
	}
	else
	{
		// Some messages should be ignored when im not in normal multiplayer mode
		if(
			MyGameStatus != STATUS_Normal &&
			MyGameStatus != STATUS_PlayingDemo
		)
		{
			switch (*MsgPnt)
			{
		    case MSG_SECBULLPOSDIR:
			case MSG_DROPPICKUP:
			case MSG_VERYSHORTDROPPICKUP:
			case MSG_KILLPICKUP:
			case MSG_VERYSHORTFUPDATE:
			case MSG_GROUPONLY_VERYSHORTFUPDATE:
			case MSG_FUPDATE:
            case MSG_SHIPHEALTH:
			case MSG_TEAMGOALS:
				return;

			default:
				break;
			}
		}
	}


	if (MyGameStatus == STATUS_StartingMultiplayer)
	{
		switch (*MsgPnt)
		{
		case MSG_VERYSHORTUPDATE:
		case MSG_UPDATE:
		case MSG_INIT:
		case MSG_STATUS:
		case MSG_LONGSTATUS:
		case MSG_HEREIAM:
		case MSG_TEXTMSG:
		case MSG_YOUQUIT:
 		case MSG_BIKENUM:
			break;
		default:
			return;
		}
	}
    switch( *MsgPnt )
    {


	case MSG_YOUQUIT:

		// I have been asked by the host to quit......
		lpYouQuitMsg = (LPYOUQUITMSG)MsgPnt;
		if( lpYouQuitMsg->You == WhoIAm )
		{
			IMustQuit = true;
			SetTitleNotify( KICKED_HOST );
		}
		return;


    case MSG_VERYSHORTFUPDATE:

		lpVeryShortFUpdate = (LPVERYSHORTFUPDATEMSG)MsgPnt;
		if( lpVeryShortFUpdate->WhoIAm != WhoIAm )
		{
			OldMode = Ships[lpVeryShortFUpdate->WhoIAm].Object.Mode;
			Ships[lpVeryShortFUpdate->WhoIAm].PacketDelay = PacketDelay;
			CheckCharging( lpVeryShortFUpdate->WhoIAm, Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Flags, lpVeryShortFUpdate->ShortGlobalShip.Flags );
			if( !( Ships[lpVeryShortFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) && ( lpVeryShortFUpdate->ShortGlobalShip.Flags & SHIP_Multiple ) )
			{
				for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ lpVeryShortFUpdate->WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
				Ships[lpVeryShortFUpdate->WhoIAm].NumMultiples = (BYTE)((lpVeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				CreateOrbitPulsar( lpVeryShortFUpdate->WhoIAm );
			}
			else
			{
				if( ( Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_Multiple ) &&
					( ((BYTE)((lpVeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15)) != Ships[lpVeryShortFUpdate->WhoIAm].NumMultiples ) )
				{
					Ships[lpVeryShortFUpdate->WhoIAm].NumMultiples = (BYTE)((lpVeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
					CreateOrbitPulsar( lpVeryShortFUpdate->WhoIAm );
				}
			}
			UnPackShipFlags( lpVeryShortFUpdate->WhoIAm , lpVeryShortFUpdate->ShortGlobalShip.Flags );
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Pos.x		= (float) lpVeryShortFUpdate->ShortGlobalShip.Pos.x;
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Pos.y		= (float) lpVeryShortFUpdate->ShortGlobalShip.Pos.y;
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Pos.z		= (float) lpVeryShortFUpdate->ShortGlobalShip.Pos.z;
			Ships[lpVeryShortFUpdate->WhoIAm].Move_Off.x		= (lpVeryShortFUpdate->ShortGlobalShip.Move_Off.x * ONEOVER32767) * (lpVeryShortFUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 );
			Ships[lpVeryShortFUpdate->WhoIAm].Move_Off.y		= (lpVeryShortFUpdate->ShortGlobalShip.Move_Off.y * ONEOVER32767) * (lpVeryShortFUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 );
			Ships[lpVeryShortFUpdate->WhoIAm].Move_Off.z		= (lpVeryShortFUpdate->ShortGlobalShip.Move_Off.z * ONEOVER32767) * (lpVeryShortFUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 ); 
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Group	= (int16_t) lpVeryShortFUpdate->ShortGlobalShip.GroupImIn;
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Quat.w	= (float)(lpVeryShortFUpdate->ShortGlobalShip.Quat.w * ONEOVER32767 );
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Quat.x	= (float)(lpVeryShortFUpdate->ShortGlobalShip.Quat.x * ONEOVER32767 );
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Quat.y	= (float)(lpVeryShortFUpdate->ShortGlobalShip.Quat.y * ONEOVER32767 );
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Quat.z	= (float)(lpVeryShortFUpdate->ShortGlobalShip.Quat.z * ONEOVER32767 );
			Ships[lpVeryShortFUpdate->WhoIAm].LastAngle.x		= (float)(lpVeryShortFUpdate->ShortGlobalShip.Angle.x * SHORTANGLEMODIFIERUNPACK );
			Ships[lpVeryShortFUpdate->WhoIAm].LastAngle.y		= (float)(lpVeryShortFUpdate->ShortGlobalShip.Angle.y * SHORTANGLEMODIFIERUNPACK );
			Ships[lpVeryShortFUpdate->WhoIAm].LastAngle.z		= (float)(lpVeryShortFUpdate->ShortGlobalShip.Angle.z * SHORTANGLEMODIFIERUNPACK );
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Bank		= (float) (lpVeryShortFUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
			Ships[lpVeryShortFUpdate->WhoIAm].Primary			= lpVeryShortFUpdate->ShortGlobalShip.Primary;
			Ships[lpVeryShortFUpdate->WhoIAm].Secondary		= lpVeryShortFUpdate->ShortGlobalShip.Secondary;
			Ships[lpVeryShortFUpdate->WhoIAm].PrimPowerLevel = (float) lpVeryShortFUpdate->ShortGlobalShip.PrimPowerLevel;
			Ships[lpVeryShortFUpdate->WhoIAm].JustRecievedPacket = true;
			Ships[lpVeryShortFUpdate->WhoIAm].Object.Noise		= 1.0F;
			

			// Need This for missiles to work....
			SetShipBankAndMat( &Ships[lpVeryShortFUpdate->WhoIAm].Object );
			
			if( ( Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_PrimFire ) || ( Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_MulFire ) )
				FirePrimaryWeapons( lpVeryShortFUpdate->WhoIAm );					// Fire Primary Weapons	
			if( ( Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_SecFire ) )
				FireSecondaryWeapons( lpVeryShortFUpdate->WhoIAm );					// Fire Secondary Weapons

			Ships[ lpVeryShortFUpdate->WhoIAm ].Primary		= ConvPrimToNormWeapon( Ships[ lpVeryShortFUpdate->WhoIAm ].Primary );
			Ships[ lpVeryShortFUpdate->WhoIAm ].Secondary = ConvSecToNormWeapon( Ships[ lpVeryShortFUpdate->WhoIAm ].Secondary );

/* Added by DC 30/6/98 */
			if( ( OldMode == DEATH_MODE ) && ( Ships[lpVeryShortFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) ||
				( OldMode == NORMAL_MODE ) && ( Ships[lpVeryShortFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) )
			{
				ThrowOutRider( lpVeryShortFUpdate->WhoIAm );
				PlayPannedSfx( SFX_BikeExplode, Ships[ lpVeryShortFUpdate->WhoIAm ].Object.Group , &Ships[lpVeryShortFUpdate->WhoIAm].Object.Pos, 0.0F );
				StopTaunt();
			}
/* ------------------- */

			DemoShipInit[ lpVeryShortFUpdate->WhoIAm ] = true;
			return;
		}
		else
		{
			DebugPrintf("someone else is in your slot..%d\n", lpVeryShortFUpdate->WhoIAm);
			return;
		}


    case MSG_GROUPONLY_VERYSHORTFUPDATE:

		lpGroupOnly_VeryShortFUpdate = (LPGROUPONLY_VERYSHORTFUPDATEMSG)MsgPnt;
		if( lpGroupOnly_VeryShortFUpdate->WhoIAm != WhoIAm )
		{
			OldMode = Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Mode;
			CheckCharging( lpGroupOnly_VeryShortFUpdate->WhoIAm, Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Flags, lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags );

			if( !( Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) && ( lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags & SHIP_Multiple ) )
			{
				for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
				Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].NumMultiples = (BYTE)((lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				CreateOrbitPulsar( lpGroupOnly_VeryShortFUpdate->WhoIAm );
			}
			else
			{
				if( ( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_Multiple ) &&
					( ((BYTE)((lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15)) != Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].NumMultiples ) )
				{
					Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].NumMultiples = (BYTE)((lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
					CreateOrbitPulsar( lpGroupOnly_VeryShortFUpdate->WhoIAm );
				}
			}
			UnPackShipFlags( lpGroupOnly_VeryShortFUpdate->WhoIAm , lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags );
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Pos.x		= (float) lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Pos.x;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Pos.y		= (float) lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Pos.y;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Pos.z		= (float) lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Pos.z;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Group		= (int16_t) lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.GroupImIn;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Quat.w	= (float)(lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Quat.w * ONEOVER32767 );
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Quat.x	= (float)(lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Quat.x * ONEOVER32767 );
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Quat.y	= (float)(lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Quat.y * ONEOVER32767 );
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Quat.z	= (float)(lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Quat.z * ONEOVER32767 );
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Bank		= (float) (lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Primary				= lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Primary;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Secondary			= lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Secondary;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].PrimPowerLevel	= (float) lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.PrimPowerLevel;
			Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Noise		= 1.0F;
			
			// Need This for missiles to work....
			SetShipBankAndMat( &Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object );
			
			if( ( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_PrimFire ) || ( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_MulFire ) )
				FirePrimaryWeapons( lpGroupOnly_VeryShortFUpdate->WhoIAm );					// Fire Primary Weapons	
			if( ( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Flags & SHIP_SecFire ) )
				FireSecondaryWeapons( lpGroupOnly_VeryShortFUpdate->WhoIAm );					// Fire Secondary Weapons

			Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Primary = ConvPrimToNormWeapon( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Primary );
			Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Secondary = ConvSecToNormWeapon( Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Secondary );

			if( ( OldMode == DEATH_MODE ) && ( Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) ||
				( OldMode == NORMAL_MODE ) && ( Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) )
			{
				ThrowOutRider( lpGroupOnly_VeryShortFUpdate->WhoIAm );
				PlayPannedSfx( SFX_BikeExplode, Ships[ lpGroupOnly_VeryShortFUpdate->WhoIAm ].Object.Group , &Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Pos, 0.0F );
				StopTaunt();
			}

			DemoShipInit[ lpGroupOnly_VeryShortFUpdate->WhoIAm ] = true;
			return;
		}
		else
		{
			DebugPrintf("someone else is in your slot..%d\n", lpGroupOnly_VeryShortFUpdate->WhoIAm);
			return;
		}


    case MSG_VERYSHORTUPDATE:

		lpVeryShortUpdate = (LPVERYSHORTUPDATEMSG) MsgPnt;

		UpdatePlayer( from, lpVeryShortUpdate->WhoIAm );

		if (MyGameStatus == STATUS_StartingMultiplayer)
		{
			//  need to get host game status...
			if( lpVeryShortUpdate->ShortGlobalShip.Flags & SHIP_IsHost  )
				OverallGameStatus = lpVeryShortUpdate->ShortGlobalShip.Status;
			// and fill out GameStatus
			GameStatus[lpVeryShortUpdate->WhoIAm] = lpVeryShortUpdate->ShortGlobalShip.Status;
			return;
		}
		else
		{
			if( lpVeryShortUpdate->WhoIAm != WhoIAm )
			{
				Ships[lpVeryShortUpdate->WhoIAm].PacketDelay = PacketDelay;
				CheckCharging( lpVeryShortUpdate->WhoIAm, Ships[ lpVeryShortUpdate->WhoIAm ].Object.Flags, lpVeryShortUpdate->ShortGlobalShip.Flags );
				OldMode = Ships[lpVeryShortUpdate->WhoIAm].Object.Mode;
				OldStatus = GameStatus[lpVeryShortUpdate->WhoIAm];
				Ships[lpVeryShortUpdate->WhoIAm].headlights = !!( lpVeryShortUpdate->ShortGlobalShip.Flags & SHIP_Light );
				if( !( Ships[lpVeryShortUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) && ( lpVeryShortUpdate->ShortGlobalShip.Flags & SHIP_Multiple ) )
				{
					for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ lpVeryShortUpdate->WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
					Ships[lpVeryShortUpdate->WhoIAm].NumMultiples = (BYTE)((lpVeryShortUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
					CreateOrbitPulsar( lpVeryShortUpdate->WhoIAm );
				}
				else
				{
					if( ( Ships[ lpVeryShortUpdate->WhoIAm ].Object.Flags & SHIP_Multiple ) &&
						( ((BYTE)((lpVeryShortUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15)) != Ships[lpVeryShortUpdate->WhoIAm].NumMultiples ) )
					{
						Ships[lpVeryShortUpdate->WhoIAm].NumMultiples = (BYTE)((lpVeryShortUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
						CreateOrbitPulsar( lpVeryShortUpdate->WhoIAm );
					}
				}
				UnPackShipFlags( lpVeryShortUpdate->WhoIAm , lpVeryShortUpdate->ShortGlobalShip.Flags );
				Ships[lpVeryShortUpdate->WhoIAm].Object.Pos.x		= (float) lpVeryShortUpdate->ShortGlobalShip.Pos.x;
				Ships[lpVeryShortUpdate->WhoIAm].Object.Pos.y		= (float) lpVeryShortUpdate->ShortGlobalShip.Pos.y;
				Ships[lpVeryShortUpdate->WhoIAm].Object.Pos.z		= (float) lpVeryShortUpdate->ShortGlobalShip.Pos.z;
				Ships[lpVeryShortUpdate->WhoIAm].Move_Off.x		= (lpVeryShortUpdate->ShortGlobalShip.Move_Off.x * ONEOVER32767) * (lpVeryShortUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 );
				Ships[lpVeryShortUpdate->WhoIAm].Move_Off.y		= (lpVeryShortUpdate->ShortGlobalShip.Move_Off.y * ONEOVER32767) * (lpVeryShortUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 );
				Ships[lpVeryShortUpdate->WhoIAm].Move_Off.z		= (lpVeryShortUpdate->ShortGlobalShip.Move_Off.z * ONEOVER32767) * (lpVeryShortUpdate->ShortGlobalShip.Move_Off_Scalar * ONEOVER256 ); 
				Ships[lpVeryShortUpdate->WhoIAm].Object.Group		= (int16_t) lpVeryShortUpdate->ShortGlobalShip.GroupImIn;
				Ships[lpVeryShortUpdate->WhoIAm].Object.Quat.w	= (float)(lpVeryShortUpdate->ShortGlobalShip.Quat.w * ONEOVER32767 );
				Ships[lpVeryShortUpdate->WhoIAm].Object.Quat.x		= (float)(lpVeryShortUpdate->ShortGlobalShip.Quat.x * ONEOVER32767 );
				Ships[lpVeryShortUpdate->WhoIAm].Object.Quat.y	= (float)(lpVeryShortUpdate->ShortGlobalShip.Quat.y * ONEOVER32767 );
				Ships[lpVeryShortUpdate->WhoIAm].Object.Quat.z		= (float)(lpVeryShortUpdate->ShortGlobalShip.Quat.z * ONEOVER32767 );
				Ships[lpVeryShortUpdate->WhoIAm].LastAngle.x		= (float)(lpVeryShortUpdate->ShortGlobalShip.Angle.x * SHORTANGLEMODIFIERUNPACK );
				Ships[lpVeryShortUpdate->WhoIAm].LastAngle.y		= (float)(lpVeryShortUpdate->ShortGlobalShip.Angle.y * SHORTANGLEMODIFIERUNPACK );
				Ships[lpVeryShortUpdate->WhoIAm].LastAngle.z		= (float)(lpVeryShortUpdate->ShortGlobalShip.Angle.z * SHORTANGLEMODIFIERUNPACK );
				Ships[lpVeryShortUpdate->WhoIAm].Object.Bank		= (float) (lpVeryShortUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
				Ships[lpVeryShortUpdate->WhoIAm].JustRecievedPacket = true;
				GameStatus[lpVeryShortUpdate->WhoIAm]				= lpVeryShortUpdate->ShortGlobalShip.Status;

				if( lpVeryShortUpdate->ShortGlobalShip.Flags & SHIP_IsHost  )
					OverallGameStatus = lpVeryShortUpdate->ShortGlobalShip.Status;
				
#ifdef DEMO_SUPPORT
					if( CheckForName( lpVeryShortUpdate->WhoIAm ) )
					{
						if( RecordDemo )
						{
							// Best way I can Think of to send the names to the demo file...
							TempTime = 1;
							Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
							size = sizeof( NAMEMSG );
							Demo_fwrite( &size, sizeof(int), 1, DemoFp );
							Demo_fwrite( &my_network_id, sizeof(DPID), 1, DemoFp );
							msg = MSG_NAME;
							Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
							Demo_fwrite( &lpVeryShortUpdate->WhoIAm, sizeof(BYTE), 1, DemoFp );
							Demo_fwrite( &Names[lpVeryShortUpdate->WhoIAm][0], 8, 1, DemoFp );
						}
					}
#endif

				if( ( OldMode == DEATH_MODE ) && ( Ships[lpVeryShortUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) ||
					( OldMode == NORMAL_MODE ) && ( Ships[lpVeryShortUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) )
				{
					ThrowOutRider( lpVeryShortUpdate->WhoIAm );
					PlayPannedSfx( SFX_BikeExplode, Ships[ lpVeryShortUpdate->WhoIAm ].Object.Group , &Ships[lpVeryShortUpdate->WhoIAm].Object.Pos, 0.0F );
					StopTaunt();
				}
				
				if( ( ( OldMode == LIMBO_MODE ) && ( Ships[lpVeryShortUpdate->WhoIAm].Object.Mode == NORMAL_MODE ) )
					|| ( ( OldStatus != STATUS_Normal) && (GameStatus[lpVeryShortUpdate->WhoIAm] == STATUS_Normal ) ) )
				{
					CreateReGen( lpVeryShortUpdate->WhoIAm );
				}

				DemoShipInit[ lpVeryShortUpdate->WhoIAm ] = true;				
				return;
			}
			else
			{
				DebugPrintf("someone else is in your slot..%d\n", lpVeryShortUpdate->WhoIAm);
				return;
			}
		}


    case MSG_UPDATE:

		lpUpdate = (LPUPDATEMSG) MsgPnt;

		UpdatePlayer( from, lpUpdate->WhoIAm );

		if (MyGameStatus == STATUS_StartingMultiplayer)
		{
			// ...and need to get host game status...
			if( lpUpdate->ShortGlobalShip.Flags & SHIP_IsHost  )
				OverallGameStatus = lpUpdate->ShortGlobalShip.Status;
			return;
		}
		else
		{
			if( lpUpdate->WhoIAm != WhoIAm )
			{
				Ships[lpUpdate->WhoIAm].PacketDelay = PacketDelay;
				CheckCharging( lpUpdate->WhoIAm, Ships[ lpUpdate->WhoIAm ].Object.Flags, lpUpdate->ShortGlobalShip.Flags );
				OldMode = Ships[lpUpdate->WhoIAm].Object.Mode;
				OldStatus = GameStatus[lpUpdate->WhoIAm];
				Ships[lpUpdate->WhoIAm].headlights = !!( lpUpdate->ShortGlobalShip.Flags & SHIP_Light );
				if( !( Ships[lpUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) && ( lpUpdate->ShortGlobalShip.Flags & SHIP_Multiple ) )
				{
					for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ lpUpdate->WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
					Ships[lpUpdate->WhoIAm].NumMultiples = (BYTE)((lpUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
					CreateOrbitPulsar( lpUpdate->WhoIAm );
				}
				else
				{
					if( ( Ships[ lpUpdate->WhoIAm ].Object.Flags & SHIP_Multiple ) &&
						( ((BYTE)((lpUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15)) != Ships[lpUpdate->WhoIAm].NumMultiples ) )
					{
						Ships[lpUpdate->WhoIAm].NumMultiples = (BYTE)((lpUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
						CreateOrbitPulsar( lpUpdate->WhoIAm );
					}
				}

				UnPackShipFlags( lpUpdate->WhoIAm , lpUpdate->ShortGlobalShip.Flags );
				Ships[lpUpdate->WhoIAm].Object.Pos		= lpUpdate->ShortGlobalShip.Pos;
				Ships[lpUpdate->WhoIAm].Move_Off		= lpUpdate->ShortGlobalShip.Move_Off;
				Ships[lpUpdate->WhoIAm].Object.Group	= (int16_t) lpUpdate->ShortGlobalShip.GroupImIn;
				Ships[lpUpdate->WhoIAm].Object.Quat	= lpUpdate->ShortGlobalShip.Quat;
				Ships[lpUpdate->WhoIAm].LastAngle		= lpUpdate->ShortGlobalShip.Angle;
#ifdef	SHORTBANK
				Ships[lpUpdate->WhoIAm].Object.Bank	= (float) (lpUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
#else
				Ships[lpUpdate->WhoIAm].Object.Bank	= lpUpdate->ShortGlobalShip.Bank;
#endif
				Ships[lpUpdate->WhoIAm].JustRecievedPacket = true;
				GameStatus[lpUpdate->WhoIAm] = lpUpdate->ShortGlobalShip.Status;

				if( lpUpdate->ShortGlobalShip.Flags & SHIP_IsHost  )
					OverallGameStatus = lpUpdate->ShortGlobalShip.Status;

#ifdef DEMO_SUPPORT
					if( CheckForName( lpUpdate->WhoIAm ) )
					{
						if( RecordDemo )
						{
							// Best way I can Think of to send the names to the demo file...
							TempTime = 1;
							Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
							size = sizeof( NAMEMSG );
							Demo_fwrite( &size, sizeof(int), 1, DemoFp );
							Demo_fwrite( &my_network_id, sizeof(DPID), 1, DemoFp );
							msg = MSG_NAME;
							Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
							Demo_fwrite( &lpUpdate->WhoIAm, sizeof(BYTE), 1, DemoFp );
							Demo_fwrite( &Names[lpUpdate->WhoIAm][0], 8, 1, DemoFp );
						}
					}
#endif

				if( ( OldMode == DEATH_MODE ) && ( Ships[lpUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) ||
					( OldMode == NORMAL_MODE ) && ( Ships[lpUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) )
				{
					ThrowOutRider( lpUpdate->WhoIAm );
					PlayPannedSfx( SFX_BikeExplode, Ships[ lpUpdate->WhoIAm ].Object.Group , &Ships[lpUpdate->WhoIAm].Object.Pos, 0.0F );
					StopTaunt();
				}

				if( ( ( OldMode == LIMBO_MODE ) && ( Ships[lpUpdate->WhoIAm].Object.Mode == NORMAL_MODE ) )
					|| ( ( OldStatus != STATUS_Normal) && (GameStatus[lpUpdate->WhoIAm] == STATUS_Normal ) ) )
				{
					CreateReGen( lpUpdate->WhoIAm );
				}

				DemoShipInit[ lpUpdate->WhoIAm ] = true;				
				return;
			}
			else
			{
				DebugPrintf("someone else is in your slot..%d\n", lpUpdate->WhoIAm);
				return;
			}
		}

    case MSG_FUPDATE:

		lpFUpdate = (LPFUPDATEMSG)MsgPnt;
		if( lpFUpdate->WhoIAm != WhoIAm )
		{
			OldMode = Ships[lpFUpdate->WhoIAm].Object.Mode;
			Ships[lpFUpdate->WhoIAm].PacketDelay = PacketDelay;
			CheckCharging( lpFUpdate->WhoIAm, Ships[ lpFUpdate->WhoIAm ].Object.Flags, lpFUpdate->ShortGlobalShip.Flags );

			if( !( Ships[lpFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) && ( lpFUpdate->ShortGlobalShip.Flags & SHIP_Multiple ) )
			{
				for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ lpFUpdate->WhoIAm ].OrbModels[ Count ] = (u_int16_t) -1;
				Ships[lpFUpdate->WhoIAm].NumMultiples = (BYTE)((lpFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				CreateOrbitPulsar( lpFUpdate->WhoIAm );
			}
			else
			{
				if( ( Ships[ lpFUpdate->WhoIAm ].Object.Flags & SHIP_Multiple ) &&
					( ((BYTE)((lpFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15)) != Ships[lpFUpdate->WhoIAm].NumMultiples ) )
				{
					Ships[lpFUpdate->WhoIAm].NumMultiples = (BYTE)((lpFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
					CreateOrbitPulsar( lpFUpdate->WhoIAm );
				}
			}
			UnPackShipFlags( lpFUpdate->WhoIAm , lpFUpdate->ShortGlobalShip.Flags );
			Ships[lpFUpdate->WhoIAm].Object.Pos		= lpFUpdate->ShortGlobalShip.Pos;
			Ships[lpFUpdate->WhoIAm].Move_Off	= lpFUpdate->ShortGlobalShip.Move_Off;
			Ships[lpFUpdate->WhoIAm].Object.Group  = lpFUpdate->ShortGlobalShip.GroupImIn;
			Ships[lpFUpdate->WhoIAm].Primary	= lpFUpdate->ShortGlobalShip.Primary;
			Ships[lpFUpdate->WhoIAm].Secondary	= lpFUpdate->ShortGlobalShip.Secondary;
			Ships[lpFUpdate->WhoIAm].Object.Quat	= lpFUpdate->ShortGlobalShip.Quat;
			Ships[lpFUpdate->WhoIAm].PrimPowerLevel = (float) lpFUpdate->ShortGlobalShip.PrimPowerLevel;
			Ships[lpFUpdate->WhoIAm].LastAngle	= lpFUpdate->ShortGlobalShip.Angle;
#ifdef	SHORTBANK
			Ships[lpFUpdate->WhoIAm].Object.Bank	= (float) (lpFUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
#else
			Ships[lpFUpdate->WhoIAm].Object.Bank	= lpFUpdate->ShortGlobalShip.Bank;
#endif
			Ships[lpFUpdate->WhoIAm].JustRecievedPacket = true;

			// Need This for missiles to work....
			SetShipBankAndMat( &Ships[lpFUpdate->WhoIAm].Object );
			Ships[lpFUpdate->WhoIAm].Object.Noise = 1.0F;
			
			if( ( Ships[ lpFUpdate->WhoIAm ].Object.Flags & SHIP_PrimFire ) || ( Ships[ lpFUpdate->WhoIAm ].Object.Flags & SHIP_MulFire ) )
				FirePrimaryWeapons( lpFUpdate->WhoIAm );					// Fire Primary Weapons	
			if( ( Ships[ lpFUpdate->WhoIAm ].Object.Flags & SHIP_SecFire ) )
				FireSecondaryWeapons( lpFUpdate->WhoIAm );					// Fire Secondary Weapons

			Ships[ lpFUpdate->WhoIAm ].Primary = ConvPrimToNormWeapon( Ships[ lpFUpdate->WhoIAm ].Primary );
			Ships[ lpFUpdate->WhoIAm ].Secondary = ConvSecToNormWeapon( Ships[ lpFUpdate->WhoIAm ].Secondary );

			if( ( OldMode == DEATH_MODE ) && ( Ships[lpFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) ||
				( OldMode == NORMAL_MODE ) && ( Ships[lpFUpdate->WhoIAm].Object.Mode == LIMBO_MODE ) )
			{
				ThrowOutRider( lpFUpdate->WhoIAm );
				PlayPannedSfx( SFX_BikeExplode, Ships[ lpFUpdate->WhoIAm ].Object.Group , &Ships[lpFUpdate->WhoIAm].Object.Pos, 0.0F );
				StopTaunt();
			}

			DemoShipInit[ lpFUpdate->WhoIAm ] = true;
			return;
		}
		else
		{
			DebugPrintf("someone else is in your slot..%d\n", lpFUpdate->WhoIAm);
			return;
		}


	case MSG_HEREIAM:

		if( IsHost && !PlayDemo )
		{
			LPHEREIAMMSG msg = (LPHEREIAMMSG) MsgPnt;
			SendGameMessage(MSG_INIT, from, msg->WhoIAm, 0, msg->MPVersion);
			SendGameMessage(MSG_STATUS, from, 0, 0, 0);
		}
		return;

    case MSG_INIT:

		lpInit = (LPINITMSG) MsgPnt;

		// save host network pointer
		host_network_player = from;

		//
		// Check if we are allowed in game
		//
		
		WhoIAm	= lpInit->YouAre;
		DebugPrintf("MSG_INIT: Host says my player id is: %d\n",WhoIAm);

		// if it's max players or over then it's a special message
		// tells us we are not allowed in game and reason
		if( WhoIAm >= MAX_PLAYERS )
			return;
		
		//
		// Check if we have the level
		//

		if (lpInit->LevelName[sizeof(lpInit->LevelName)-1] != 0)
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) %s (%d) setting last byte of LevelName null\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt );
			lpInit->LevelName[sizeof(lpInit->LevelName)-1] = 0;
			return;
		}

		DebugPrintf("MSG_INIT: host says level is %s\n", lpInit->LevelName );

		NewLevelNum = FindSameLevel( &lpInit->LevelName[0] );

		if( NewLevelNum == -1 )
		{
			DebugPrintf("client cannot find level %s\n", lpInit->LevelName );
			WhoIAm = MAX_PLAYERS+1;
			return;
		}

		//
		//  Coppy everything the host told us
		//

		MaxKills					= lpInit->MaxKills;
		OverallGameStatus			= lpInit->OverallGameStatus;
		NetUpdateInterval			= lpInit->NetUpdateInterval;
		PacketsSlider.value			= (int) (60.0F / NetUpdateInterval);
		ColPerspective				= lpInit->ColPerspective;
		UseShortPackets				= lpInit->UseShortPackets;
		RandomStartPosModify		= lpInit->RandomStartPosModify;
		BountyBonusInterval			= lpInit->BountyBonusInterval;
		TeamGame					= lpInit->TeamGame;
		CaptureTheFlag				= lpInit->CaptureTheFlag;
		CTF							= lpInit->CTF;
		BountyHunt					= lpInit->BountyHunt;
		ResetKillsPerLevel			= lpInit->ResetKillsPerLevel;
		TimeLimit.value				= lpInit->TimeLimit;
		RandomPickups				= lpInit->RandomPickups;
		CopyOfSeed1					= lpInit->Seed1;
		CopyOfSeed2					= lpInit->Seed2;
		NumPrimaryPickups			= lpInit->NumPrimaryPickups;

		memmove( PlayerReady,	lpInit->PlayerReady,	sizeof(PlayerReady));//memcpy
		memmove( GameStatus,		lpInit->GameStatus,		sizeof(GameStatus));//memcpy
		memmove( TeamNumber,		lpInit->TeamNumber,		sizeof(TeamNumber));//memcpy
		memmove( KillStats,		lpInit->KillStats,		sizeof(KillStats));//memcpy
		memmove( KillCounter,	lpInit->KillCounter,	sizeof(KillCounter));//memcpy
		memmove( BonusStats,		lpInit->BonusStats,		sizeof(BonusStats));//memcpy

		memset( PickupValid, 0, sizeof(PickupValid) );  // turn off all weapons
		UnpackPickupInfo( lpInit->PickupFlags );	// which weapons are enabled

		//
		//  post processing
		//

		Current_Camera_View = WhoIAm;
		WatchPlayerSelect.value = WhoIAm;

		if ( TeamGame )	GetPlayerNumMenu = &MENU_NEW_WatchTeamSelect;
		else			GetPlayerNumMenu = (MENU*) NewJoinItem.Value;

		if( TimeLimit.value )	CountDownOn = true;
		else					CountDownOn	= false;

		if ( CaptureTheFlag || CTF )
			GoalScore = lpInit->GoalScore;

		if ( CTF )
		{					
			switch ( lpInit->CTF_Type )
			{
			case CTF_STANDARD:
				NeedFlagAtHome			= true;
				OwnFlagTeleportsHome	= true;
				CanCarryOwnFlag			= false;
				break;
			case CTF_MUST_CARRY:
				NeedFlagAtHome			= true;
				OwnFlagTeleportsHome	= false;
				CanCarryOwnFlag			= true;
				break;
			case CTF_NO_RETURN:
				NeedFlagAtHome			= false;
				OwnFlagTeleportsHome	= false;
				CanCarryOwnFlag			= false;
				break;
			case CTF_CAN_CARRY:
				NeedFlagAtHome			= false;
				OwnFlagTeleportsHome	= false;
				CanCarryOwnFlag			= true;
				break;
			case CTF_ONE_TOUCH:
				NeedFlagAtHome			= false;
				OwnFlagTeleportsHome	= true;
				CanCarryOwnFlag			= false;
				break;
			}
		}

		Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );

		// now that i know my ship number i can set my name
		set_my_player_name();

		// update status
		MyGameStatus = STATUS_StartingMultiplayer;

		// tell everyone my status (includes my ship number and from->name)
		// they can now use this message to relate my name to my ship number
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
		return;


    case MSG_DROPPICKUP:

		lpDropPickup = (LPDROPPICKUPMSG)MsgPnt;
#if DEBUG_PICKUPS
		if( lpDropPickup->WhoIAm != (u_int16_t) -1 ) DebugPrintf( "PACKET OWNER '%s' : Init '%s', Owner '%s', ID %d\n", &Names[ lpDropPickup->WhoIAm ][ 0 ], Messages[ lpDropPickup->PickupInfo.Type ], &Names[ lpDropPickup->WhoIAm ][ 0 ], lpDropPickup->PickupInfo.IDCount );
		else DebugPrintf( "PACKET OWNER '%s' : Init '%s', No Owner, ID %d\n", &Names[ lpDropPickup->WhoIAm ][ 0 ], Messages[ lpDropPickup->PickupInfo.Type ], lpDropPickup->PickupInfo.IDCount );
#endif
	 	Pickup = InitOnePickup( &lpDropPickup->PickupInfo.Pos, lpDropPickup->PickupInfo.Group,
		 				   &lpDropPickup->PickupInfo.Dir, lpDropPickup->PickupInfo.Speed,
		 				   lpDropPickup->PickupInfo.Type, lpDropPickup->WhoIAm,
	 					   lpDropPickup->PickupInfo.IDCount, lpDropPickup->PickupInfo.RegenSlot,
	 					   lpDropPickup->PickupInfo.Sparkle, lpDropPickup->PickupInfo.LifeCount,
						   lpDropPickup->PickupInfo.TriggerMod );
		return;


    case MSG_VERYSHORTDROPPICKUP:

		lpVeryShortDropPickup = (LPVERYSHORTDROPPICKUPMSG)MsgPnt;
		TempPickup.Pos.x = (float) lpVeryShortDropPickup->PickupInfo.Pos.x;
		TempPickup.Pos.y = (float) lpVeryShortDropPickup->PickupInfo.Pos.y;
		TempPickup.Pos.z = (float) lpVeryShortDropPickup->PickupInfo.Pos.z;
		TempPickup.Dir.x = lpVeryShortDropPickup->PickupInfo.Dir.x * ONEOVER32767;
		TempPickup.Dir.y = lpVeryShortDropPickup->PickupInfo.Dir.y * ONEOVER32767;
		TempPickup.Dir.z = lpVeryShortDropPickup->PickupInfo.Dir.z * ONEOVER32767;
		TempPickup.IDCount = lpVeryShortDropPickup->PickupInfo.IDCount;
		TempPickup.Type = lpVeryShortDropPickup->PickupInfo.Type;
		TempPickup.Group = lpVeryShortDropPickup->PickupInfo.Group;
		TempPickup.Speed = lpVeryShortDropPickup->PickupInfo.Speed;
		TempPickup.RegenSlot = lpVeryShortDropPickup->PickupInfo.RegenSlot;
		TempPickup.Sparkle = lpVeryShortDropPickup->PickupInfo.Sparkle;
		TempPickup.LifeCount = lpVeryShortDropPickup->PickupInfo.LifeCount;
		TempPickup.TriggerMod = lpVeryShortDropPickup->PickupInfo.TriggerMod;
#if DEBUG_PICKUPS
		if( lpVeryShortDropPickup->WhoIAm != (u_int16_t) -1 ) DebugPrintf( "PACKET OWNER '%s' : Init '%s', Owner '%s', ID %d\n", &Names[ lpVeryShortDropPickup->WhoIAm ][ 0 ], Messages[ TempPickup.Type ], &Names[ lpVeryShortDropPickup->WhoIAm ][ 0 ], TempPickup.IDCount );
		else DebugPrintf( "PACKET OWNER '%s' : Init '%s', No Owner, ID %d\n", &Names[ lpVeryShortDropPickup->WhoIAm ][ 0 ], Messages[ TempPickup.Type ], TempPickup.IDCount );
#endif
	 	Pickup = InitOnePickup( &TempPickup.Pos, TempPickup.Group,
		 				   &TempPickup.Dir, TempPickup.Speed,
		 				   TempPickup.Type, lpVeryShortDropPickup->WhoIAm,
	 					   TempPickup.IDCount, TempPickup.RegenSlot,
	 					   TempPickup.Sparkle, TempPickup.LifeCount,
						   TempPickup.TriggerMod );
		return;


    case MSG_KILLPICKUP:

   		lpKillPickup = (LPKILLPICKUPMSG)MsgPnt;
		if( PlayDemo && ( Current_Camera_View == lpKillPickup->WhoIAm ) && ( lpKillPickup->KillPickupInfo.Style == PICKUPKILL_Immediate ) )
		{
			SfxForCollectPickup( lpKillPickup->KillPickupInfo.Owner, lpKillPickup->KillPickupInfo.IDCount );
		}
#if DEBUG_PICKUPS
		if( lpKillPickup->KillPickupInfo.Owner != (u_int16_t) -1 ) DebugPrintf( "PACKET OWNER '%s' : Kill Pickup Owner '%s', ID %d\n", &Names[ lpKillPickup->WhoIAm ][ 0 ], &Names[ lpKillPickup->KillPickupInfo.Owner ][ 0 ], lpKillPickup->KillPickupInfo.IDCount );
		else DebugPrintf( "PACKET OWNER '%s' : Kill Pickup No Owner, ID %d\n", &Names[ lpKillPickup->WhoIAm ][ 0 ], lpKillPickup->KillPickupInfo.IDCount );
#endif
		KillPickup( lpKillPickup->KillPickupInfo.Owner, lpKillPickup->KillPickupInfo.IDCount, lpKillPickup->KillPickupInfo.Style );
		return;

    case MSG_TEAMGOALS:
		lpTeamGoals = (LPTEAMGOALSMSG)MsgPnt;
		SetTeamGoals( &lpTeamGoals->TeamGoalsInfo.TeamGoals[ 0 ] );
   		return;

    case MSG_SHOCKWAVE:
   		lpShockwave = (LPSHOCKWAVEMSG)MsgPnt;
   		CreateShockwave( OWNER_SHIP,
						 lpShockwave->ShockwaveInfo.Owner,
   						 &lpShockwave->ShockwaveInfo.Pos,
   						 lpShockwave->ShockwaveInfo.Group,
   						 lpShockwave->ShockwaveInfo.ShockwaveSize,
						 lpShockwave->ShockwaveInfo.Weapon );
   		return;

    case MSG_BGOUPDATE:
   		lpBGOUpdate = (LPBGOUPDATEMSG)MsgPnt;
   		UpdateBGObject( lpBGOUpdate->BGOUpdateInfo.BGObject,
						lpBGOUpdate->BGOUpdateInfo.State,
						lpBGOUpdate->BGOUpdateInfo.Time );
   		return;

    case MSG_PRIMBULLPOSDIR:
   		lpPrimBullPosDir = (LPPRIMBULLPOSDIRMSG)MsgPnt;
		InitOnePrimBull( lpPrimBullPosDir->PrimBullPosDir.OwnerType,
						lpPrimBullPosDir->PrimBullPosDir.OwnerID,
						lpPrimBullPosDir->PrimBullPosDir.BulletID,
						lpPrimBullPosDir->PrimBullPosDir.Weapon,
						lpPrimBullPosDir->PrimBullPosDir.Group,
						&lpPrimBullPosDir->PrimBullPosDir.Pos,
						&lpPrimBullPosDir->PrimBullPosDir.Offset,
						&lpPrimBullPosDir->PrimBullPosDir.Dir,
						&lpPrimBullPosDir->PrimBullPosDir.Up,
						lpPrimBullPosDir->PrimBullPosDir.PowerLevel,
						lpPrimBullPosDir->PrimBullPosDir.PLevel, false );
   		return;


    case MSG_SECBULLPOSDIR:

   		lpSecBullPosDir = (LPSECBULLPOSDIRMSG)MsgPnt;
		InitOneSecBull( lpSecBullPosDir->SecBullPosDir.OwnerType,
						lpSecBullPosDir->SecBullPosDir.Owner,
						lpSecBullPosDir->SecBullPosDir.BulletID,
						lpSecBullPosDir->SecBullPosDir.Group,
						&lpSecBullPosDir->SecBullPosDir.Pos,
						&lpSecBullPosDir->SecBullPosDir.Offset,
						&lpSecBullPosDir->SecBullPosDir.Dir,
						&lpSecBullPosDir->SecBullPosDir.UpVector,
						&lpSecBullPosDir->SecBullPosDir.DropDir,
						lpSecBullPosDir->SecBullPosDir.Weapon, false );
		return;


    case MSG_TITANBITS:

   		lpTitanBits = (LPTITANBITSMSG)MsgPnt;
		for( Count = 0; Count < NUMTITANBITS; Count++ )
		{
			 i = InitOneSecBull( lpTitanBits->TitanBits.OwnerType,
								lpTitanBits->TitanBits.Owner,
								(u_int16_t) ( Count + lpTitanBits->TitanBits.BulletID ),
								lpTitanBits->TitanBits.Group,
								&lpTitanBits->TitanBits.Pos,
								&lpTitanBits->TitanBits.Offset,
								&lpTitanBits->TitanBits.Directions[ Count ],
								&lpTitanBits->TitanBits.UpVector,
								&lpTitanBits->TitanBits.DropDir,
								lpTitanBits->TitanBits.Weapon, false );
			 if( i != (u_int16_t) -1 )
				 SecBulls[i].FramelagAddition = GlobalFramelagAddition;
		}
		return;


	// Someone is claiming to have hit me...
    case MSG_SHIPHIT:

   		lpShipHit = (LPSHIPHITMSG)MsgPnt;

		if( lpShipHit->You == WhoIAm )
   		{
			lpShipHit->ShipHit.Point.x += Ships[ WhoIAm ].Object.Pos.x;
			lpShipHit->ShipHit.Point.y += Ships[ WhoIAm ].Object.Pos.y;
			lpShipHit->ShipHit.Point.z += Ships[ WhoIAm ].Object.Pos.z;

			if( ( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE ) || ( Ships[ WhoIAm ].Object.Mode == DEATH_MODE ) )
			{
				if( Ships[ WhoIAm ].Object.Mode == DEATH_MODE )
				{
					lpShipHit->ShipHit.Recoil.x *= 0.1F;
					lpShipHit->ShipHit.Recoil.y *= 0.1F;
					lpShipHit->ShipHit.Recoil.z *= 0.1F;
					lpShipHit->ShipHit.Force *= 0.1F;
				}
				if( lpShipHit->ShipHit.OneOffExternalForce ) ForceExternalOneOff( WhoIAm, &lpShipHit->ShipHit.Recoil );
				else ForceExternal( WhoIAm, &lpShipHit->ShipHit.Recoil );
				if( lpShipHit->ShipHit.Force )
					RotateExternal( WhoIAm, &lpShipHit->ShipHit.Point, &lpShipHit->ShipHit.Dir, lpShipHit->ShipHit.Force );
			}
			if( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE )
			{

   				if( Random_Range( 16 ) )
					PlayPannedSfx( SFX_ShipHit, Ships[ WhoIAm ].Object.Group , &lpShipHit->ShipHit.Point, 0.0F );
				else
   					PlaySfx( SFX_BikerPain , 1.0F );

				Ships[WhoIAm].ShipThatLastHitMe = lpShipHit->WhoHitYou;

				// set damage argument for DoDamage() function
   				Ships[WhoIAm].Damage = lpShipHit->ShipHit.Damage;

   				// do the damage...
   				if ( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )
   				{
					// if I died...
   					Ships[WhoIAm].ShipThatLastKilledMe = lpShipHit->WhoHitYou;
   					Ships[WhoIAm].Object.Mode = DEATH_MODE;
   					Ships[WhoIAm].Timer = 0.0F;
   					// print up who killed me
					GetDeathString( lpShipHit->ShipHit.WeaponType, lpShipHit->ShipHit.Weapon, &methodstr[0] );

					if (TeamGame && ( TeamNumber[Ships[WhoIAm].ShipThatLastKilledMe] == TeamNumber[WhoIAm]))
						strcpy(&teamstr[0], FROM_YOUR_OWN_TEAM );
					else
						strcpy(&teamstr[0], "");

   					AddColourMessageToQue(KillMessageColour, "%s %s %s %s",
							&Names[Ships[WhoIAm].ShipThatLastKilledMe][0],
							"KILLED YOU WITH ", &methodstr[0], &teamstr[0] );
					ShipDiedSend( lpShipHit->ShipHit.WeaponType, lpShipHit->ShipHit.Weapon );
   				}
			}
   		}
   		return;


	// Someone is claiming to have hit me...
    case MSG_SHORTSHIPHIT:
		
   		lpShortShipHit = (LPSHORTSHIPHITMSG)MsgPnt;

		if( lpShortShipHit->You == WhoIAm )
   		{
			Point.x = lpShortShipHit->ShipHit.Point.x + Ships[ WhoIAm ].Object.Pos.x;
			Point.y = lpShortShipHit->ShipHit.Point.y + Ships[ WhoIAm ].Object.Pos.y;
			Point.z = lpShortShipHit->ShipHit.Point.z + Ships[ WhoIAm ].Object.Pos.z;
			Recoil.x = (lpShortShipHit->ShipHit.Recoil.x * ONEOVER32767) * (lpShortShipHit->ShipHit.Recoil_Scalar * ONEOVER256 );
			Recoil.y = (lpShortShipHit->ShipHit.Recoil.y * ONEOVER32767) * (lpShortShipHit->ShipHit.Recoil_Scalar * ONEOVER256 );
			Recoil.z = (lpShortShipHit->ShipHit.Recoil.z * ONEOVER32767) * (lpShortShipHit->ShipHit.Recoil_Scalar * ONEOVER256 ); 
			Force = lpShortShipHit->ShipHit.Force;
			Dir.x = (float)(lpShortShipHit->ShipHit.Dir.x * ONEOVER32767);
			Dir.y = (float)(lpShortShipHit->ShipHit.Dir.y * ONEOVER32767);
			Dir.z = (float)(lpShortShipHit->ShipHit.Dir.z * ONEOVER32767);

			if( ( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE ) || ( Ships[ WhoIAm ].Object.Mode == DEATH_MODE ) )
			{
				if( Ships[ WhoIAm ].Object.Mode == DEATH_MODE )
				{
					Recoil.x *= 0.1F;
					Recoil.y *= 0.1F;
					Recoil.z *= 0.1F;
					Force *= 0.1F;
				}
				if( lpShortShipHit->ShipHit.OneOffExternalForce != 0 ) ForceExternalOneOff( WhoIAm, &Recoil );
				else ForceExternal( WhoIAm, &Recoil );
				if( Force ) RotateExternal( WhoIAm, &Point, &Dir, Force );
			}
			if( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE )
			{
				if( ( lpShortShipHit->ShipHit.WeaponType == WEPTYPE_Secondary ) &&
					( lpShortShipHit->ShipHit.Weapon == SCATTERMISSILE ) )
				{
					switch( ColPerspective )
					{
						case COLPERS_Forsaken:
							if( !GodMode )
							{
								ScatterWeapons( &NullVector, MAXSCATTERED );
								PlayPannedSfx( SFX_Scattered, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );
							}
							break;

						case COLPERS_Descent:
							break;
					}
				}

   				if( Random_Range( 16 ) )
					PlayPannedSfx( SFX_ShipHit, Ships[ WhoIAm ].Object.Group , &Point, 0.0F );
				else
   					PlaySfx( SFX_BikerPain , 1.0F );

				Ships[WhoIAm].ShipThatLastHitMe	= lpShortShipHit->WhoHitYou;
   				Ships[WhoIAm].Damage					= lpShortShipHit->ShipHit.Damage;

   				// do the damage...
   				if ( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )
   				{
					// if I died...
   					Ships[WhoIAm].ShipThatLastKilledMe	= lpShortShipHit->WhoHitYou;
   					Ships[WhoIAm].Object.Mode				= DEATH_MODE;
   					Ships[WhoIAm].Timer						= 0.0F;
   					// print up who killed me
					GetDeathString( lpShortShipHit->ShipHit.WeaponType, lpShortShipHit->ShipHit.Weapon, &methodstr[0] );

					if (TeamGame && ( TeamNumber[Ships[WhoIAm].ShipThatLastKilledMe] == TeamNumber[WhoIAm]))
						strcpy(&teamstr[0], FROM_YOUR_OWN_TEAM );
					else
						strcpy(&teamstr[0], "");

					// called in TOL OFF multiplayer!!
   					AddColourMessageToQue( KillMessageColour, "%s %s %s %s",
							&Names[Ships[WhoIAm].ShipThatLastKilledMe][0],
							"KILLED YOU WITH", &methodstr[0]  ,&teamstr[0] );
					// update stats 1 (stats.c) -- somebody killed me
					UpdateKillStats(lpShortShipHit->WhoHitYou,WhoIAm,lpShortShipHit->ShipHit.WeaponType, lpShortShipHit->ShipHit.Weapon);
					// kill me :(
					ShipDiedSend( lpShortShipHit->ShipHit.WeaponType, lpShortShipHit->ShipHit.Weapon );
   				}
			}
   		}
   		return;


	// Someone has Died....Did I kill Them ??
	// this is called in multiplayer
    case MSG_SHIPDIED:

		lpShipDied = (LPSHIPDIEDMSG)MsgPnt;

		// print a message reflecting the weapon
		GetDeathString( lpShipDied->WeaponType, lpShipDied->Weapon, &methodstr[0] );
		
		// if you killed them
		if( WhoIAm == lpShipDied->WhoKilledMe )
		{
			if( TeamGame )
			{
				if( TeamNumber[lpShipDied->WhoIAm] == TeamNumber[WhoIAm] )
				{
					if( !GodMode )
					{
						// you killed someone on your own team
						AddColourMessageToQue( KillMessageColour, "%s %s %s %s" " %s",
							"YOU KILLED", &Names[lpShipDied->WhoIAm][0], "WITH", &methodstr[0], ON_YOUR_OWN_TEAM );
						// update stats 7 (stats.c) -- you killed someone on your own team
						UpdateKillStats(WhoIAm, lpShipDied->WhoIAm, lpShipDied->WeaponType, lpShipDied->Weapon);
					}
				}
				// if they weren't on your team
				else
				{
					// bounty team game
					if ( BountyHunt )
					{
						if ( ( Ships[ WhoIAm ].Object.Flags | Ships[ lpShipDied->WhoIAm ].Object.Flags ) & SHIP_CarryingBounty )
						{
							// you killed the person who was carrying the bounty
							if ( Ships[ lpShipDied->WhoIAm ].Object.Flags & SHIP_CarryingBounty )
								AddColourMessageToQue( KillMessageColour, "%s %s %s %s" "%s", 
									"YOU KILLED", &Names[lpShipDied->WhoIAm][0], WITH_THE_BOUNTY, "WITH ", &methodstr[0] );
							// you had the bounty and killed someone
							else
								AddColourMessageToQue( KillMessageColour, "%s %s %s" "%s",
									"YOU KILLED", &Names[lpShipDied->WhoIAm][0], "WITH ", &methodstr[0] );
							
							PlaySfx( SFX_BIKER_VP, 1.0F );
							AddKill();
							// update stats 8 (stats.c) -- you killed someone in a team bounty game
							UpdateKillStats(WhoIAm, lpShipDied->WhoIAm, lpShipDied->WeaponType, lpShipDied->Weapon);
						}
						else
						{
							// you killed someone who wasnt carrying the bounty (team bounty) (no points for you!!)
							AddColourMessageToQue( KillMessageColour, NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY,
								Names[ lpShipDied->WhoIAm ] );
						}
					}
					else if( !GodMode )
					// any other team game
					{
						// you killed someone in a team game
						AddColourMessageToQue( KillMessageColour, "%s %s %s" "%s", "YOU KILLED", &Names[lpShipDied->WhoIAm][0], "WITH ", &methodstr[0] );
						AddKill();
						// update stats 9 (stats.c) -- you killed someone in a team game
						UpdateKillStats(WhoIAm, lpShipDied->WhoIAm, lpShipDied->WeaponType, lpShipDied->Weapon);
						
						if ( !Random_Range( 4 ) )
							PlaySfx( SFX_BIKER_VP, 1.0F );
					}
				}
			}
			// if not  team game
			else
			{
				// standard bounty hunt
				if ( BountyHunt )
				{
					if ( ( Ships[ WhoIAm ].Object.Flags | Ships[ lpShipDied->WhoIAm ].Object.Flags ) & SHIP_CarryingBounty )
					{
						// you killed the person who was carrying the bounty
						if ( Ships[ lpShipDied->WhoIAm ].Object.Flags & SHIP_CarryingBounty )
							AddColourMessageToQue( KillMessageColour, "%s %s %s %s" "%s", "YOU KILLED", &Names[lpShipDied->WhoIAm][0], WITH_THE_BOUNTY, "WITH ", &methodstr[0] );
						// you had the bounty and killed someone
						else
							AddColourMessageToQue( KillMessageColour, "%s %s %s" "%s", "YOU KILLED", &Names[lpShipDied->WhoIAm][0], "WITH ", &methodstr[0] );
						
						PlaySfx( SFX_BIKER_VP, 1.0F );
						AddKill();
						// update stats 10 (stats.c) -- you killed someone in a bounty game
						UpdateKillStats(WhoIAm, lpShipDied->WhoIAm, lpShipDied->WeaponType, lpShipDied->Weapon);
					}
					else
					{
						// you killed someone who wasnt carrying the bounty (no points for you!!)
						AddColourMessageToQue( KillMessageColour, NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY,
							Names[ lpShipDied->WhoIAm ] );
					}
				}
				// not a team game or bounty hunt
				else if( !GodMode )
				{
   					// you killed someone
					AddColourMessageToQue( KillMessageColour, "%s %s %s" "%s", "YOU KILLED", &Names[lpShipDied->WhoIAm][0], "WITH ", &methodstr[0] );
					AddKill();
					// update stats 2 (stats.c) -- you killed someone 
					UpdateKillStats(WhoIAm,lpShipDied->WhoIAm,lpShipDied->WeaponType,lpShipDied->Weapon); 
					if ( !Random_Range( 4 ) )
						PlaySfx( SFX_BIKER_VP, 1.0F );
				}
			}
		}
		// if you were not who killed them
		else
		{				
			if( lpShipDied->WhoIAm == lpShipDied->WhoKilledMe )
			{
				// gee someone killed themselves...
				AddColourMessageToQue(KillMessageColour, "%s %s %s", &Names[lpShipDied->WhoIAm][0], "KILLED HIMSELF WITH", &methodstr[0] );
			}
			else
			{
				if (TeamGame && (TeamNumber[lpShipDied->WhoIAm] == TeamNumber[lpShipDied->WhoKilledMe]))
					strcpy (&teamstr[0], ON_HIS_OWN_TEAM);
				else
					strcpy (&teamstr[0], "");

				// gee someone killed somebody...who cares...
				AddColourMessageToQue( KillMessageColour, "%s %s %s %s %s" " %s", &Names[lpShipDied->WhoKilledMe][0], "KILLED", &Names[lpShipDied->WhoIAm][0], "WITH", &methodstr[0], &teamstr[0] );
			}

			// update stats 3 (stats.c) -- somebody killed someone
			UpdateKillStats(lpShipDied->WhoKilledMe,lpShipDied->WhoIAm,lpShipDied->WeaponType,lpShipDied->Weapon);
		}
		return;


/*===================================================================
	This is where the host gets told what state he thinks we are in
===================================================================*/
    case MSG_STATUS:
   		lpStatus = (LPSTATUSMSG)MsgPnt;

		if ( !STATUS_VALID_PLAYER(GameStatus[lpStatus->WhoIAm]) && STATUS_VALID_PLAYER(lpStatus->Status) )
			reset_tracker();

		if( ( GameStatus[lpStatus->WhoIAm] != STATUS_Normal) && (lpStatus->Status == STATUS_Normal ) ) 
			CreateReGen( lpStatus->WhoIAm );

		//DebugPrintf("setting status for player %d to %x\n", lpStatus->WhoIAm, lpStatus->Status );
		GameStatus[lpStatus->WhoIAm] = lpStatus->Status;

		if( GameStatus[lpStatus->WhoIAm] == STATUS_Left )
		{
			// This player has quit...
			Ships[lpStatus->WhoIAm].enable		= 0;
			Ships[lpStatus->WhoIAm].Pickups		= 0;
			Ships[lpStatus->WhoIAm].RegenSlots	= 0;
			Ships[lpStatus->WhoIAm].Mines			= 0;
			Ships[lpStatus->WhoIAm].Triggers		= 0;
			Ships[lpStatus->WhoIAm].TrigVars		= 0;
		}
		else
		{
	//		DebugPrintf("%s status = %d\n", Names[lpStatus->WhoIAm], GameStatus[lpStatus->WhoIAm] );
			Ships[lpStatus->WhoIAm].Pickups		= lpStatus->Pickups;
			Ships[lpStatus->WhoIAm].RegenSlots	= lpStatus->RegenSlots;
			Ships[lpStatus->WhoIAm].Mines			= lpStatus->Mines;
			Ships[lpStatus->WhoIAm].Triggers		= lpStatus->Triggers;
			Ships[lpStatus->WhoIAm].TrigVars		= lpStatus->TrigVars;

			if( !IsHost )
   			{
   				if( lpStatus->IsHost )
   				{
					BYTE tempstatus;
					Ships[lpStatus->WhoIAm].Object.Flags |= SHIP_IsHost;
					
					// its a status change from the Host...
					tempstatus = OverallGameStatus;
   					OverallGameStatus = lpStatus->Status;
					if ( tempstatus != OverallGameStatus )
						DebugPrintf("Setting overall game status to %d\n", lpStatus->Status );
				}
   			}

			TeamNumber[lpStatus->WhoIAm]	= lpStatus->TeamNumber;
			PlayerReady[lpStatus->WhoIAm]		= lpStatus->IAmReady;

			// update network pointer and name
			UpdatePlayer( from, lpStatus->WhoIAm );

#ifdef DEMO_SUPPORT
			if( !PlayDemo )
			{
				if( CheckForName( lpStatus->WhoIAm ) )
				{
					if( RecordDemo )
					{
						// Best way I can Think of to send the names to the demo file...
						TempTime = 1;
						Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
						size = sizeof( NAMEMSG );
						Demo_fwrite( &size, sizeof(int), 1, DemoFp );
						Demo_fwrite( &my_network_id, sizeof(DPID), 1, DemoFp );
						msg = MSG_NAME;
						Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
						Demo_fwrite( &lpStatus->WhoIAm, sizeof(BYTE), 1, DemoFp );
						Demo_fwrite( &Names[lpStatus->WhoIAm][0], 8, 1, DemoFp );
					}
				}
			}
#endif
		}

		return;


	case MSG_NETSETTINGS:

			lpNetSettingsMsg = (LPNETSETTINGSMSG)MsgPnt;

			// clients must update their settings
			if(!IsHost)
			{
				// only the host can change the settings
				if(lpNetSettingsMsg->IsHost)
				{
					// if in normal mode display text message of changes
					if( MyGameStatus == STATUS_Normal )
					{
						// collision perspective changed by the host
						if( ColPerspective != lpNetSettingsMsg->CollisionPerspective)
						{
							// shooter decides
							if( lpNetSettingsMsg->CollisionPerspective == COLPERS_Forsaken)
								AddColourMessageToQue(SystemMessageColour, "SHOOTER NOW DECIDES COLLISIONS");
							// target decides
							else
								AddColourMessageToQue(SystemMessageColour, "TARGET NOW DECIDES COLLISIONS");
						}
						// packet rate changed by the host
						if( NetUpdateInterval != lpNetSettingsMsg->PacketsPerSecond )
							AddColourMessageToQue(SystemMessageColour, "%.0f %s" , ( 60.0F / lpNetSettingsMsg->PacketsPerSecond ) , PACKETS_PER_SECOND_SET );
						// short packets changed by the host
						if( UseShortPackets != lpNetSettingsMsg->ShortPackets)
						{
							// short packets enabled
							if( lpNetSettingsMsg->ShortPackets == true)
								AddColourMessageToQue(SystemMessageColour, "SHORT PACKETS ENABLED BY HOST");
							// short packets disabled
							else
								AddColourMessageToQue(SystemMessageColour, "SHORT PACKETS DISABLED BY HOST");
						}
					}
					ColPerspective = lpNetSettingsMsg->CollisionPerspective;
					UseShortPackets = lpNetSettingsMsg->ShortPackets;
					NetUpdateInterval = lpNetSettingsMsg->PacketsPerSecond;
					PacketsSlider.value = (int) (60.0F / NetUpdateInterval);
				}
			}
			return;

    case MSG_LONGSTATUS:

   		lpLongStatus = (LPLONGSTATUSMSG)MsgPnt;

		if ( !STATUS_VALID_PLAYER(GameStatus[lpLongStatus->WhoIAm]) && STATUS_VALID_PLAYER(lpLongStatus->Status.Status) )
			reset_tracker();

		if( ( GameStatus[lpLongStatus->WhoIAm] != STATUS_Normal) && (lpLongStatus->Status.Status == STATUS_Normal ) ) 
			CreateReGen( lpLongStatus->WhoIAm );

		GameStatus[lpLongStatus->WhoIAm]			= lpLongStatus->Status.Status;
		DebugPrintf("%s LongStatus = %d\n", Names[lpLongStatus->WhoIAm], GameStatus[lpLongStatus->WhoIAm] );
		Ships[lpLongStatus->WhoIAm].Pickups		= lpLongStatus->Status.Pickups;
		Ships[lpLongStatus->WhoIAm].RegenSlots	= lpLongStatus->Status.RegenSlots;
		Ships[lpLongStatus->WhoIAm].Mines			= lpLongStatus->Status.Mines;
		Ships[lpLongStatus->WhoIAm].Triggers		= lpLongStatus->Status.Triggers;
		Ships[lpLongStatus->WhoIAm].TrigVars		= lpLongStatus->Status.TrigVars;
		
		if( !IsHost )
   		{
   			if( lpLongStatus->Status.IsHost )
   			{
				RandomStartPosModify = lpLongStatus->RandomStartPosModify;
				Ships[lpLongStatus->WhoIAm].Object.Flags |= SHIP_IsHost;
				NewLevelNum = FindSameLevel( &lpLongStatus->LevelName[0] );
				if( NewLevelNum == -1 )
				{
					char buf[ 128 ];
					sprintf( buf, DID_NOT_HAVE_LEVEL /*"you did not have the level chosen by the host ( %s )"*/,
						lpLongStatus->LevelName );
					SetTitleNotify( buf );
					SpecialDestroyGame();
					return;
				}
				LevelList.selected_item = NewLevelNum;
				// its a status change from the Host...
   				OverallGameStatus = lpLongStatus->Status.Status;
			}
   		}
		TeamNumber[lpLongStatus->WhoIAm] = lpLongStatus->Status.TeamNumber;
		PlayerReady[lpLongStatus->WhoIAm] = lpLongStatus->Status.IAmReady;

		// update network pointer and name
		UpdatePlayer( from, lpLongStatus->WhoIAm );

#ifdef DEMO_SUPPORT
		if( !PlayDemo )
		{
			if( CheckForName( lpLongStatus->WhoIAm ) )
			{
				if( RecordDemo )
				{
					// Best way I can Think of to send the names to the demo file...
					TempTime = 1;
					Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
					size = sizeof( NAMEMSG );
					Demo_fwrite( &size, sizeof(int), 1, DemoFp );
					Demo_fwrite( &my_network_id, sizeof(DPID), 1, DemoFp );
					msg = MSG_NAME;
					Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
					Demo_fwrite( &lpLongStatus->WhoIAm, sizeof(BYTE), 1, DemoFp );
					Demo_fwrite( &Names[lpLongStatus->WhoIAm][0], 8, 1, DemoFp );
				}
			}
		}
#endif

		return;


    case MSG_SHORTPICKUP:

   		lpShortPickup = (LPSHORTPICKUPMSG)MsgPnt;
   		if( lpShortPickup->Pickups == Ships[WhoIAm].Pickups )
   		{
#if DEBUG_PICKUPS
			DebugPrintf( "Message from host to init pickups\n" );
#endif
			if( lpShortPickup->HowManyPickups != 0 )
			{
				Ships[WhoIAm].Pickups -= 1;
				RegenPickupList( &lpShortPickup->ShortPickup[0] , lpShortPickup->HowManyPickups );
			}
			else
				Ships[WhoIAm].Pickups = 0;
		}
		return;

    case MSG_SHORTREGENSLOT:
		lpShortRegenSlot = (LPSHORTREGENSLOTMSG)MsgPnt;
   		if( lpShortRegenSlot->RegenSlots == Ships[WhoIAm].RegenSlots )
   		{
#if DEBUG_PICKUPS
			DebugPrintf( "Message from host to init regenslots\n" );
#endif
			if( lpShortRegenSlot->HowManyRegenSlots != 0 )
			{
				Ships[WhoIAm].RegenSlots -= 1;
				RegenRegenSlotList( &lpShortRegenSlot->ShortRegenSlot[0] , lpShortRegenSlot->HowManyRegenSlots );
			}
			else
				Ships[WhoIAm].RegenSlots = 0;
		}
		return;


    case MSG_SHORTTRIGGER:

		lpShortTrigger = (LPSHORTTRIGGERMSG)MsgPnt;
   		if( lpShortTrigger->Triggers == Ships[WhoIAm].Triggers )
   		{
  			DebugPrintf( "Message from host to init trigger\n" );
			if( lpShortTrigger->HowManyTriggers != 0 )
			{
				Ships[WhoIAm].Triggers -= 1;
				RegenTriggerList( &lpShortTrigger->ShortTrigger[0] , lpShortTrigger->HowManyTriggers );
			}
			else
				Ships[WhoIAm].Triggers -= 1;
		}
		return;


    case MSG_SHORTTRIGVAR:

		lpShortTrigVar = (LPSHORTTRIGVARMSG)MsgPnt;
   		if( lpShortTrigVar->TrigVars == Ships[WhoIAm].TrigVars )
   		{
  			DebugPrintf( "Message from host to init trigvar\n" );
			if( lpShortTrigVar->HowManyTrigVars != 0 )
			{
				Ships[WhoIAm].TrigVars -= 1;
				RegenTrigVarList( &lpShortTrigVar->ShortTrigVar[0] , lpShortTrigVar->HowManyTrigVars );
			}
			else
				Ships[WhoIAm].TrigVars -= 1;
		}
		return;


    case MSG_SHORTMINE:

   		lpShortMine = (LPSHORTMINEMSG)MsgPnt;
   		if( lpShortMine->Mines == Ships[WhoIAm].Mines )
   		{
			if( Ships[WhoIAm].Mines == ( ( MAXSECONDARYWEAPONBULLETS / MAXGENMINECOUNT	) + 1 ) )
				timer_run( &last_mine_timer );

  			DebugPrintf( "Message from host to init mines\n" );

			if( lpShortMine->HowManyMines != 0 )
			{
	   			Ships[WhoIAm].Mines -= 1;
	   			RegenMineList( &lpShortMine->ShortMine[0] , lpShortMine->HowManyMines );
			}
			else
				Ships[WhoIAm].Mines = 0;
   		}
   		return;


    case MSG_TEXTMSG:

		lpTextMsg = (LPTEXTMSG)MsgPnt;
		if( lpTextMsg->Text[MAXTEXTMSG-1] != 0 )
		{
			DebugPrintf("EvaluateMessage: from %s (%s:%d) %s (%d) forcing last byte to be null\n",
				from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt );
			lpTextMsg->Text[MAXTEXTMSG-1] = 0;
		}
		switch (lpTextMsg->TextMsgType)
		{
			case TEXTMSGTYPE_FlagDriftedIn:
			case TEXTMSGTYPE_FlagEscaped:
			case TEXTMSGTYPE_ReturnedFlag:
			case TEXTMSGTYPE_ReturningFlag:
			case TEXTMSGTYPE_ScoredWithFlag:
			case TEXTMSGTYPE_CaptureFlagMessage:
			case TEXTMSGTYPE_JoiningTeamGame:
				if(!TeamGame)
				{
					DebugPrintf("EvaluateMessage: from %s (%s:%d) %s (%d) dropping team message (%d) because not in team game\n",
						from->name, from->ip, from->port, msg_to_str(*MsgPnt), *MsgPnt, lpTextMsg->TextMsgType );
					return;
				}
			break;
		}
		switch (lpTextMsg->TextMsgType)
		{
			case TEXTMSGTYPE_QuickTauntWhisper:
				if( TeamNumber[WhoIAm] != TeamNumber[lpTextMsg->WhoIAm] )
					return;
				AddPlayerMessageToQue(PlayerMessageColour, "%s whispers %s", &Names[lpTextMsg->WhoIAm][0],  &lpTextMsg->Text[0] );
				return;
			case TEXTMSGTYPE_Taunt1:
			case TEXTMSGTYPE_Taunt2:
			case TEXTMSGTYPE_Taunt3:
				AddColourMessageToQue(TauntMessageColour, "%s says %s", &Names[lpTextMsg->WhoIAm][0],  &lpTextMsg->Text[0] );
				return;
			case TEXTMSGTYPE_QuickTaunt:
				AddPlayerMessageToQue(PlayerMessageColour, "%s says %s", &Names[lpTextMsg->WhoIAm][0],  &lpTextMsg->Text[0] );
				// received version request
				if(strcmp(&lpTextMsg->Text[0], (const char *) "version") == 0)
				{
					// display my version number
					AddColourMessageToQue(SystemMessageColour, "%s", YourVersion );
					// send my version number back
					strncpy( (char *)&QuickText.text, PXVersion , sizeof(PXVersion) );
					SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_QuickTaunt, 0);
					QuickText.text[0] = 0; // clean message buffer
				}
				return;
			case TEXTMSGTYPE_JoiningTeamGame:
				AddColourMessageToQue(SystemMessageColour, "%s %s", &Names[lpTextMsg->WhoIAm][0] ,IS_JOINING_THE_GAME);
				return;
			case TEXTMSGTYPE_EnteredWatchMode:
				AddColourMessageToQue(SystemMessageColour, "%s %s", &Names[lpTextMsg->WhoIAm][0] ,"ENTERED WATCH MODE");
				return;
			case TEXTMSGTYPE_ExitedWatchMode:
				AddColourMessageToQue(SystemMessageColour, "%s %s", &Names[lpTextMsg->WhoIAm][0] ,"EXITED WATCH MODE");
				return;
			case TEXTMSGTYPE_TitleMessage:
				AddTitleMessage(lpTextMsg);
				return;
			case TEXTMSGTYPE_CaptureFlagMessage:
				// this message is instead formatted by the sender
				// AddColourMessageToQue(FlagMessageColour, "%s on the %s team has got the flag", Names[ lpTextMsg->WhoIAm ], TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ] );
				AddColourMessageToQue(FlagMessageColour, "%s", lpTextMsg->Text );
				if ( TeamNumber[ WhoIAm ] == TeamNumber[ lpTextMsg->WhoIAm ] )
					PlaySfx( SFX_MyTeamGotFlag, FlagVolume );
				else
					PlaySfx( SFX_OtherTeamGotFlag , FlagVolume );
				return;
			case TEXTMSGTYPE_ScoredWithFlag:
				AddColourMessageToQue(FlagMessageColour, THE_COLOUR_TEAM_HAVE_SCORED, TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ] );
				if ( TeamNumber[ WhoIAm ] == TeamNumber[ lpTextMsg->WhoIAm ] )
					PlaySfx( SFX_MyTeamScored, FlagVolume );
				else
					PlaySfx( SFX_OtherTeamScored , FlagVolume );
				// update bonus 6 (stats.c) -- someone else scored with flag
				UpdateBonusStats((int) lpTextMsg->WhoIAm, GoalScoreSlider.value);
				return;
			case TEXTMSGTYPE_ReturningFlag:
				if ( lpTextMsg->WhoIAm != WhoIAm )
					AddColourMessageToQue(FlagMessageColour, THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG,TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ]);
				return;
			case TEXTMSGTYPE_ReturnedFlag:
				AddColourMessageToQue(FlagMessageColour, THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED,TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ] );
				return;
			case TEXTMSGTYPE_FlagDriftedIn:
				AddColourMessageToQue(FlagMessageColour, THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL,TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ] );
				return;
			case TEXTMSGTYPE_FlagEscaped:
				AddColourMessageToQue(FlagMessageColour, THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL, TeamName[ TeamNumber[ lpTextMsg->WhoIAm ] ] );
				return;
			case TEXTMSGTYPE_BountyMessage:
				AddColourMessageToQue(FlagMessageColour, "%s %s", Names[ lpTextMsg->WhoIAm ] , HAS_GOT_THE_BOUNTY);
				PlaySfx( SFX_OtherTeamGotFlag, FlagVolume );
				return;
			case TEXTMSGTYPE_SpeechTaunt:
				PlayRecievedSpeechTaunt( lpTextMsg->WhoIAm, lpTextMsg->Text[ 0 ] );
				return;
			default:
				DebugPrintf("EvaluateMessage: Invalid MSG_TEXTMSG of type: %d\n",lpTextMsg->TextMsgType);
		}
		return;

#ifdef DEMO_SUPPORT

    case MSG_INTERPOLATE:

		lpInterpolate = (LPINTERPOLATEMSG)MsgPnt;
		Ships[lpInterpolate->WhoIAm].OldPos = Ships[lpInterpolate->WhoIAm].Object.Pos;
		Ships[lpInterpolate->WhoIAm].NextPos = lpInterpolate->NextPos;
		Ships[lpInterpolate->WhoIAm].OldQuat = Ships[lpInterpolate->WhoIAm].Object.Quat;
		Ships[lpInterpolate->WhoIAm].NextQuat = lpInterpolate->NextQuat;
		Ships[lpInterpolate->WhoIAm].OldBank = Ships[lpInterpolate->WhoIAm].Object.Bank;
		Ships[lpInterpolate->WhoIAm].NextBank = lpInterpolate->NextBank;
//		Ships[lpInterpolate->WhoIAm].OldTime = GameElapsedTime;
		Ships[lpInterpolate->WhoIAm].OldTime = DemoTimeSoFar;
		Ships[lpInterpolate->WhoIAm].NextTime = lpInterpolate->NextTime;
		Ships[lpInterpolate->WhoIAm].DemoInterpolate = true;
		return;

    case MSG_VERYSHORTINTERPOLATE:

		lpVeryShortInterpolate = (LPVERYSHORTINTERPOLATEMSG)MsgPnt;
		Ships[lpVeryShortInterpolate->WhoIAm].NextPos.x		= (float) lpVeryShortInterpolate->NextPos.x;
		Ships[lpVeryShortInterpolate->WhoIAm].NextPos.y		= (float) lpVeryShortInterpolate->NextPos.y;
		Ships[lpVeryShortInterpolate->WhoIAm].NextPos.z		= (float) lpVeryShortInterpolate->NextPos.z;
		Ships[lpVeryShortInterpolate->WhoIAm].NextQuat.w	= (float)(lpVeryShortInterpolate->NextQuat.w * ONEOVER32767 );
		Ships[lpVeryShortInterpolate->WhoIAm].NextQuat.x	= (float)(lpVeryShortInterpolate->NextQuat.x * ONEOVER32767 );
		Ships[lpVeryShortInterpolate->WhoIAm].NextQuat.y	= (float)(lpVeryShortInterpolate->NextQuat.y * ONEOVER32767 );
		Ships[lpVeryShortInterpolate->WhoIAm].NextQuat.z	= (float)(lpVeryShortInterpolate->NextQuat.z * ONEOVER32767 );
		Ships[lpVeryShortInterpolate->WhoIAm].NextBank		= (float) (lpVeryShortInterpolate->NextBank / SHORTBANKMODIFIER);
		Ships[lpVeryShortInterpolate->WhoIAm].NextTime		= lpVeryShortInterpolate->NextTime;
		Ships[lpVeryShortInterpolate->WhoIAm].DemoInterpolate = true;
		Ships[lpVeryShortInterpolate->WhoIAm].OldPos			= Ships[lpVeryShortInterpolate->WhoIAm].Object.Pos;
		Ships[lpVeryShortInterpolate->WhoIAm].OldQuat		= Ships[lpVeryShortInterpolate->WhoIAm].Object.Quat;
		Ships[lpVeryShortInterpolate->WhoIAm].OldBank		= Ships[lpVeryShortInterpolate->WhoIAm].Object.Bank;
		Ships[lpVeryShortInterpolate->WhoIAm].OldTime		= DemoTimeSoFar;
		return;

#endif

    case MSG_SETTIME:

		lpSetTime = (LPSETTIMEMSG)MsgPnt;
		if( IllegalTime )
		{
			Countdown_Float = lpSetTime->TimeInfo.Time;
			IllegalTime = false;
		}
		return;


    case MSG_REQTIME:

		lpReqTime = (LPREQTIMEMSG)MsgPnt;
		if( IsHost ) SetTime( Countdown_Float );
		return;

    case MSG_BIKENUM:

		lpBikeNumMsg = (LPBIKENUMMSG)MsgPnt;

	  	if( PlayDemo )
		{
			// if we are playing end game demo, we want to substitute the displayed bike for the players bike
			switch( GameCompleted )
			{
			case GAMECOMPLETE_WithoutAllCrystals:
			case GAMECOMPLETE_WithAllCrystalsExceptLast:
				Ships[lpBikeNumMsg->WhoIAm].BikeNum = player_config->bike;
				break;
			default:
				Ships[lpBikeNumMsg->WhoIAm].BikeNum = (int16_t) lpBikeNumMsg->BikeNum;
			}
		}
		else
			Ships[lpBikeNumMsg->WhoIAm].BikeNum = (int16_t) lpBikeNumMsg->BikeNum;

		return;
	}

	DebugPrintf("Evaluate Message: from %s (%s:%d) unhandled message of type: %s (%d)\n",
			from->name, from->ip, from->port,
			msg_to_str(*MsgPnt), *MsgPnt);
}

typedef enum {
	CHANNEL_RESERVED,			// cannot use channel 0 !!!
	CHANNEL_MAIN,				// default channel send pkts reliable or unreliabe, sequenced or unsequenced
	CHANNEL_BIKE_POSITIONS,		// position updates are unreliable|sequenced... late pkts will get dropped...
} channel_t;

void SendGameMessage( BYTE msg, network_player_t * to, BYTE ShipNum, BYTE Type, BYTE mask )
{
    LPSHIPHEALTHMSG                     lpShipHealth;
    LPVERYSHORTUPDATEMSG				lpVeryShortUpdate;
    LPUPDATEMSG							lpUpdate;
    LPFUPDATEMSG						lpFUpdate;
	LPVERYSHORTFUPDATEMSG				lpVeryShortFUpdate;
	LPGROUPONLY_VERYSHORTFUPDATEMSG		lpGroupOnly_VeryShortFUpdate;
    LPHEREIAMMSG						lpHereIAm;
    LPINITMSG							lpInit;
    LPPRIMBULLPOSDIRMSG					lpPrimBullPosDir;
    LPSECBULLPOSDIRMSG					lpSecBullPosDir;
    LPTITANBITSMSG						lpTitanBits;
    LPSHIPHITMSG						lpShipHit;
    LPSHORTSHIPHITMSG					lpShortShipHit;
    LPSHIPDIEDMSG						lpShipDied;
    LPDROPPICKUPMSG						lpDropPickup;
    LPVERYSHORTDROPPICKUPMSG			lpVeryShortDropPickup;
    LPKILLPICKUPMSG						lpKillPickup;
    LPTEAMGOALSMSG						lpTeamGoals;
    LPSHOCKWAVEMSG						lpShockwave;
    LPBGOUPDATEMSG						lpBGOUpdate;
    LPSTATUSMSG							lpStatus;
    LPLONGSTATUSMSG						lpLongStatus;
    LPSHORTPICKUPMSG					lpShortPickup;
    LPSHORTREGENSLOTMSG					lpShortRegenSlot;
    LPSHORTTRIGGERMSG					lpShortTrigger;
    LPSHORTTRIGVARMSG					lpShortTrigVar;
    LPSHORTMINEMSG						lpShortMine;
    LPTEXTMSG							lpTextMsg;
	LPBIKENUMMSG						lpBikeNumMsg;
	LPYOUQUITMSG						lpYouQuitMsg;
	LPSETTIMEMSG						lpSetTime;
	LPREQTIMEMSG						lpReqTime;
	LPNETSETTINGSMSG					lpNetSettingsMsg;

	// network variables
	DWORD			nBytes = 0;
	int				flags  = 0;
	channel_t		channel = CHANNEL_MAIN;

	// temp variables
	int				i;

	// text variables
	int				MessageColour = GREEN;
	char			VersionMessage[30];

	// set flag sfx volume
	FlagVolume = FlagSfxSlider.value / ( FlagSfxSlider.max / GLOBAL_MAX_SFX );

	if( PlayDemo )
		return;

	switch( msg )
    {

    case MSG_SHIPHEALTH:
        DebugPrintf("net_msg: MSG_SHIPHEALTH\n");

        lpShipHealth = (LPSHIPHEALTHMSG)&CommBuff[0];
        lpShipHealth->MsgCode = msg;
        lpShipHealth->WhoIAm = WhoIAm;
        lpShipHealth->Hull = (u_int8_t) Ships[WhoIAm].Object.Hull;
        lpShipHealth->Shield = (u_int8_t) Ships[WhoIAm].Object.Shield;
        nBytes = sizeof( SHIPHEALTHMSG );
		flags |= NETWORK_RELIABLE;
        break;

    case MSG_YOUQUIT:
    	DebugPrintf("net_msg: MSG_YOUQUIT\n");

		lpYouQuitMsg = (LPYOUQUITMSG)&CommBuff[0];
        lpYouQuitMsg->MsgCode = msg;
		lpYouQuitMsg->WhoIAm = WhoIAm;
		lpYouQuitMsg->You = ShipNum;
		nBytes = sizeof( YOUQUITMSG );
		to = Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
		break;


    case MSG_BIKENUM:
    	DebugPrintf("net_msg: MSG_BIKENUM\n");

		lpBikeNumMsg = (LPBIKENUMMSG)&CommBuff[0];
        lpBikeNumMsg->MsgCode = msg;
		lpBikeNumMsg->WhoIAm = WhoIAm;
		lpBikeNumMsg->BikeNum = (BYTE) Ships[WhoIAm].BikeNum;
		nBytes = sizeof( BIKENUMMSG );
		flags |= NETWORK_RELIABLE;
		break;

	case MSG_HEREIAM:
		DebugPrintf("net_msg: MSG_HEREIAM\n");

        lpHereIAm = (LPHEREIAMMSG)&CommBuff[0];
        lpHereIAm->MsgCode = msg;
        lpHereIAm->WhoIAm = WhoIAm;
		lpHereIAm->MPVersion = PXMPVINT;
		nBytes = sizeof( HEREIAMMSG );
		flags |= NETWORK_RELIABLE;
        break;


	// should probably break this up into small MSG_INIT_(N) steps...
	// instead of MSG_HEREIAM have a MSG_REQUEST_INIT which specifies the INIT (N) to reply with
	// the host can keep track of where the player is for security
	// this would reduce the need for one giant paacket instead having smaller ones
	// and the first thing that should be done is negotiate valid player number and version numbers
	// other wise whats the point of sending all this extra stuff...

    case MSG_INIT:
    	DebugPrintf("net_msg: MSG_INIT\n");

		// only host is allowed to send a MSG_INIT
		if( !IsHost )
			return;

		// MSG_INIT only needs to be sent once
		if( ShipNum != UNASSIGNED_SHIP )
			return;

		// MSG_INIT only needs to be sent once
		for( i = 0; i < MAX_PLAYERS; i++ )
			if( Ships[i].network_player == to ) // we have already assigned this user
			{
				// at this point enet reliability will assure that the last sent message
				// does reach there...  If it doesn't... Well then the player has some
				// really bad connection... and the connection will most likely shutdown...
				// none the less probably better not to let such a problem player in...
				DebugPrintf("Ignoring MSG_INIT request from player %s (%d) %s\n",
							Ships[i].network_player->name, i,
							"because I already replied to him...");
				return;
			}

		// setup the basic message flags
		flags |= NETWORK_RELIABLE;
		nBytes = sizeof( INITMSG );
		lpInit = (LPINITMSG)&CommBuff[0];
		lpInit->MsgCode	= msg;
		lpInit->WhoIAm  = WhoIAm;

		//
		// Check player has right version to join the game
		//

		if( PXMPVINT != mask )
		{
			lpInit->YouAre = MAX_PLAYERS+3; // bad version
			goto send;
		}

		//
		//  Assign the player a slot (aka unique player number across all computers)
		//

		{
			int new_player_id = -1;

			//
			// check to see if max players reached
			//

			int player_count  = 1; // count myself
			for( i = 0; i < MAX_PLAYERS; i++ )
				if(	! (GameStatus[i] == STATUS_Left || GameStatus[i] == STATUS_LeftCrashed || GameStatus[i] == STATUS_Null) )
					player_count++;
			
			DebugPrintf("MSG_INIT: current player_count %d\n",player_count);

			if( player_count >= MaxPlayersSlider.value )
			{
				DebugPrintf("MSG_INIT: game is full... denying connection...\n");
				lpInit->YouAre = MAX_PLAYERS+2;
				goto send;
			}

			//
			// find a player slot for new player
			//

			// first look for null slots so we can preserve scores of players who left
			for( i = 0; i < MAX_PLAYERS; i++ )
				if(	i != WhoIAm ) // host slot is maintained in MyGameStatus not GameStatus[WhoIAm]
					if( GameStatus[i] == STATUS_Null )
					{
						new_player_id = i; // found an unused slot
						DebugPrintf("MSG_INIT: player assigned to unused slot\n");
						break;
					}

			// if we didn't find a never used slot then assign a slot from a player who left
			if( new_player_id == -1 )
				for( i = 0; i < MAX_PLAYERS; i++ )
					if(	i != WhoIAm ) // host slot is maintained in MyGameStatus not GameStatus[WhoIAm]
						if(	GameStatus[i] == STATUS_Left || GameStatus[i] == STATUS_LeftCrashed )
						{
							new_player_id = i; // found an old slot
							DebugPrintf("MSG_INIT: player assigned to reused slot\n");
							break;
						}

			DebugPrintf("MSG_INIT: new_player_id set to %d\n", new_player_id);

			lpInit->YouAre = (BYTE) new_player_id;	// tell new player who he is

			//
			// setup the new player
			//

			InitShipStructure( new_player_id, true );							// zero out player and the old score
			GameStatus[new_player_id]			= STATUS_GetPlayerNum;			// player now goes into "get player number" stage
			TeamNumber[new_player_id]			= 0;							// starts off on team zero
			Ships[new_player_id].network_player	= to;							// save network pointer
			set_player_name( new_player_id, to->name );							// copy in their name
			
			//
			// the current game state is saved and propagated to the player over the next few frames
			// this is why you see the screen that says, "mines left, pickups left, etc..."
			//

			if(	MyGameStatus == STATUS_Normal ) // should this be here? maybe this info isn't ready unless in game already?
			{
				CopyPickups(	(u_int16_t) new_player_id );					
				CopyRegenSlots( (u_int16_t) new_player_id );
				CopyTriggers(	(u_int16_t) new_player_id );
				CopyTrigVars(	(u_int16_t) new_player_id );
				CopyMines(		(u_int16_t) new_player_id );
			}
		}

		//
		//  If they got this far they are allowed into the game
		//  so tell them everything they need to know about game
		//

		lpInit->OverallGameStatus		= MyGameStatus;					// games overall status is equal to host status
		lpInit->Seed1					= CopyOfSeed1;
		lpInit->Seed2					= CopyOfSeed2;
		lpInit->ColPerspective			= (BYTE)ColPerspective;
		lpInit->NetUpdateInterval		= NetUpdateInterval;
		lpInit->RandomPickups			= RandomPickups;
		lpInit->NumPrimaryPickups		= (BYTE)NumPrimaryPickups;
		lpInit->MaxKills				= (BYTE)MaxKills;
		lpInit->GoalScore				= GoalScore;
		lpInit->BountyBonusInterval		= BountyBonusInterval;
		lpInit->RandomStartPosModify	= RandomStartPosModify;
		lpInit->TeamGame				= TeamGame;
		lpInit->CTF						= CTF;
		lpInit->CaptureTheFlag			= CaptureTheFlag;
		lpInit->BountyHunt				= BountyHunt;
		lpInit->ResetKillsPerLevel		= ResetKillsPerLevel;
		lpInit->UseShortPackets			= UseShortPackets;
		lpInit->CTF_Type				= CTFSlider.value;
		lpInit->TimeLimit				= TimeLimit.value;

		PackPickupInfo( lpInit->PickupFlags );												// currently enabled pickups

		memmove( lpInit->KillStats,		KillStats,		sizeof(lpInit->KillStats));			// other players stats stats//memcpy
		memmove( lpInit->KillCounter,	KillCounter,	sizeof(lpInit->KillCounter));		// other players kills counts//memcpy
		memmove( lpInit->BonusStats,		BonusStats,		sizeof(lpInit->KillCounter));		// other players bonus stats//memcpy
		memmove( lpInit->PlayerReady,	PlayerReady,	sizeof(lpInit->PlayerReady));		// other players ready status//memcpy
		memmove( lpInit->GameStatus,		GameStatus,		sizeof(lpInit->GameStatus));		// other players game status//memcpy
		memmove( lpInit->TeamNumber,		TeamNumber,		sizeof(lpInit->TeamNumber));		// what team everyone is on//memcpy

		strncpy( lpInit->LevelName, ShortLevelNames[NewLevelNum], 32 );						// level we are on
       
		break;


    case MSG_VERYSHORTUPDATE: // short packets on
    	//DebugPrintf("net_msg: MSG_VERYSHORTUPDATE\n");

        lpVeryShortUpdate = (LPVERYSHORTUPDATEMSG)&CommBuff[0];
        lpVeryShortUpdate->MsgCode = msg;
        lpVeryShortUpdate->WhoIAm = WhoIAm;
		lpVeryShortUpdate->ShortGlobalShip = VeryShortGlobalShip;
        nBytes = sizeof( VERYSHORTUPDATEMSG );
		channel = CHANNEL_BIKE_POSITIONS;
		flags = NETWORK_SEQUENCED;
        break;


    case MSG_UPDATE: // short packets off
    	//DebugPrintf("net_msg: MSG_UPDATE\n");

        lpUpdate = (LPUPDATEMSG)&CommBuff[0];
        lpUpdate->MsgCode = msg;
        lpUpdate->WhoIAm = WhoIAm;
				lpUpdate->ShortGlobalShip = ShortGlobalShip;
        nBytes = sizeof( UPDATEMSG );
				channel = CHANNEL_BIKE_POSITIONS;
				flags = NETWORK_SEQUENCED;
        break;

	// following 3 pkts are bullet pkts
	// they also contain the player position information
	// so they must be sent on the player position channel
	
	// BUG: we should really break apart fire updates and position updates
	// InitOnePrimBull() and InitOneSecBull() support sending over network
	// probably just use those and turn off the f updates...

    case MSG_FUPDATE: // short packets off + fire weapon data
    	DebugPrintf("net_msg: MSG_FUPDATE\n");

		//DebugPrintf("F UPDATE\n");

        lpFUpdate = (LPFUPDATEMSG)&CommBuff[0];
        lpFUpdate->MsgCode = msg;
        lpFUpdate->WhoIAm = WhoIAm;
		lpFUpdate->ShortGlobalShip = FShortGlobalShip;
        nBytes = sizeof( FUPDATEMSG );
		channel = CHANNEL_BIKE_POSITIONS;
		flags = NETWORK_SEQUENCED;
		// uncomment to make secondaries reliable
		//if(mask)
		//	flags |= NETWORK_RELIABLE;
		Interval = NetUpdateInterval; // reset pps timer
        break;


    case MSG_VERYSHORTFUPDATE: // short packets on + fire weapon data
    	DebugPrintf("net_msg: MSG_VERYSHORTFUPDATE\n");

		//DebugPrintf("F UPDATE\n");

        lpVeryShortFUpdate = (LPVERYSHORTFUPDATEMSG)&CommBuff[0];
        lpVeryShortFUpdate->MsgCode = msg;
        lpVeryShortFUpdate->WhoIAm = WhoIAm;
		lpVeryShortFUpdate->ShortGlobalShip = FVeryShortGlobalShip;
        nBytes = sizeof( VERYSHORTFUPDATEMSG );
		channel = CHANNEL_BIKE_POSITIONS;
		flags = NETWORK_SEQUENCED;
		// uncomment to make secondaries reliable
		//if(mask)
		//	flags |= NETWORK_RELIABLE;
		Interval = NetUpdateInterval; // reset pps timer
        break;


    case MSG_GROUPONLY_VERYSHORTFUPDATE:
    	DebugPrintf("net_msg: MSG_GROUPONLY_VERYSHORTFUPDATE\n");

		//DebugPrintf("F UPDATE GROUP ONLY\n");

        lpGroupOnly_VeryShortFUpdate = (LPGROUPONLY_VERYSHORTFUPDATEMSG)&CommBuff[0];
        lpGroupOnly_VeryShortFUpdate->MsgCode = msg;
        lpGroupOnly_VeryShortFUpdate->WhoIAm = WhoIAm;
		lpGroupOnly_VeryShortFUpdate->ShortGlobalShip = GroupOnly_FVeryShortGlobalShip;
        nBytes = sizeof( GROUPONLY_VERYSHORTFUPDATEMSG );
		channel = CHANNEL_BIKE_POSITIONS;
		flags = NETWORK_SEQUENCED;
		// uncomment to make secondaries reliable
		//if(mask)
		//	flags |= NETWORK_RELIABLE;
        break;


    case MSG_DROPPICKUP:
    	DebugPrintf("net_msg: MSG_DROPPICKUP\n");

        lpDropPickup = (LPDROPPICKUPMSG) &CommBuff[0];
        lpDropPickup->MsgCode = msg;
        lpDropPickup->WhoIAm = WhoIAm;
        lpDropPickup->PickupInfo = TempPickup;
        nBytes = sizeof( DROPPICKUPMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_VERYSHORTDROPPICKUP:
    	DebugPrintf("net_msg: MSG_VERYSHORTDROPPICKUP\n");

        lpVeryShortDropPickup = (LPVERYSHORTDROPPICKUPMSG) &CommBuff[0];
        lpVeryShortDropPickup->MsgCode = msg;
        lpVeryShortDropPickup->WhoIAm = WhoIAm;
        lpVeryShortDropPickup->PickupInfo = VeryShortTempPickup;
        nBytes = sizeof( VERYSHORTDROPPICKUPMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_KILLPICKUP:
    	DebugPrintf("net_msg: MSG_KILLPICKUP\n");

        lpKillPickup = (LPKILLPICKUPMSG) &CommBuff[0];
        lpKillPickup->MsgCode = msg;
        lpKillPickup->WhoIAm = WhoIAm;
        lpKillPickup->KillPickupInfo = TempKillPickup;
        nBytes = sizeof( KILLPICKUPMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_TEAMGOALS:
    	DebugPrintf("net_msg: MSG_TEAMGOALS\n");

        lpTeamGoals = (LPTEAMGOALSMSG) &CommBuff[0];
        lpTeamGoals->MsgCode = msg;
        lpTeamGoals->WhoIAm = WhoIAm;
        lpTeamGoals->TeamGoalsInfo = TempTeamGoals;
        nBytes = sizeof( TEAMGOALSMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHOCKWAVE:
    	DebugPrintf("net_msg: MSG_SHOCKWAVE\n");

        lpShockwave = (LPSHOCKWAVEMSG) &CommBuff[0];
        lpShockwave->MsgCode = msg;
        lpShockwave->WhoIAm = WhoIAm;
        lpShockwave->ShockwaveInfo = TempShockwave;
        nBytes = sizeof( SHOCKWAVEMSG );
        break;


    case MSG_BGOUPDATE:
    	DebugPrintf("net_msg: MSG_BGOUPDATE\n");

        lpBGOUpdate = (LPBGOUPDATEMSG) &CommBuff[0];
        lpBGOUpdate->MsgCode = msg;
        lpBGOUpdate->WhoIAm = WhoIAm;
        lpBGOUpdate->BGOUpdateInfo = TempBGOUpdate;
        nBytes = sizeof( BGOUPDATEMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_PRIMBULLPOSDIR:
    	DebugPrintf("net_msg: MSG_PRIMBULLPOSDIR\n");
        lpPrimBullPosDir = (LPPRIMBULLPOSDIRMSG)&CommBuff[0];
        lpPrimBullPosDir->MsgCode = msg;
        lpPrimBullPosDir->WhoIAm = WhoIAm;
        lpPrimBullPosDir->PrimBullPosDir = TempPrimBullPosDir;
        nBytes = sizeof( PRIMBULLPOSDIRMSG );
        break;

	// appears to be mines..

    case MSG_SECBULLPOSDIR:
    	DebugPrintf("net_msg: MSG_SECBULLPOSDIR\n");
        lpSecBullPosDir = (LPSECBULLPOSDIRMSG)&CommBuff[0];
        lpSecBullPosDir->MsgCode = msg;
        lpSecBullPosDir->WhoIAm = WhoIAm;
        lpSecBullPosDir->SecBullPosDir = TempSecBullPosDir;
        nBytes = sizeof( SECBULLPOSDIRMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_TITANBITS:
    	DebugPrintf("net_msg: MSG_TITANBITS\n");

        lpTitanBits = (LPTITANBITSMSG)&CommBuff[0];
        lpTitanBits->MsgCode = msg;
        lpTitanBits->WhoIAm = WhoIAm;
        lpTitanBits->TitanBits = TempTitanBits;
        nBytes = sizeof( TITANBITSMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHIPHIT:
    	DebugPrintf("net_msg: MSG_SHIPHIT\n");
		
        lpShipHit = (LPSHIPHITMSG)&CommBuff[0];
        lpShipHit->MsgCode = msg;
        lpShipHit->You = ShipNum;
	    lpShipHit->WhoHitYou = WhoIAm;
        lpShipHit->ShipHit = TempShipHit;
        nBytes = sizeof( SHIPHITMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTSHIPHIT:
    	DebugPrintf("net_msg: MSG_SHORTSHIPHIT\n");

        lpShortShipHit = (LPSHORTSHIPHITMSG)&CommBuff[0];
        lpShortShipHit->MsgCode = msg;
        lpShortShipHit->You = ShipNum;
		lpShortShipHit->WhoHitYou = WhoIAm;
        lpShortShipHit->ShipHit = ShortTempShipHit;
        nBytes = sizeof( SHORTSHIPHITMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHIPDIED:
    	DebugPrintf("net_msg: MSG_SHIPDIED\n");

        lpShipDied = (LPSHIPDIEDMSG)&CommBuff[0];
        lpShipDied->MsgCode = msg;
        lpShipDied->WhoIAm = WhoIAm;
		lpShipDied->Pos = Ships[WhoIAm].Object.Pos;
		lpShipDied->WhoKilledMe = Ships[WhoIAm].ShipThatLastKilledMe;
		lpShipDied->Type	= Type;
		lpShipDied->WeaponType = TempDied.WeaponType;
		lpShipDied->Weapon = TempDied.Weapon;
		nBytes = sizeof( SHIPDIEDMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_REQTIME:
    	DebugPrintf("net_msg: MSG_REQTIME\n");
        lpReqTime = (LPREQTIMEMSG)&CommBuff[0];
        lpReqTime->MsgCode = msg;
        lpReqTime->WhoIAm = WhoIAm;
		nBytes = sizeof( REQTIMEMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SETTIME:
    	DebugPrintf("net_msg: MSG_SETTIME\n");

        lpSetTime = (LPSETTIMEMSG)&CommBuff[0];
        lpSetTime->MsgCode = msg;
        lpSetTime->WhoIAm = WhoIAm;
		lpSetTime->TimeInfo = TempTimeSet;
		nBytes = sizeof( SETTIMEMSG );
		flags |= NETWORK_RELIABLE;
        break;

    case MSG_STATUS:
    	DebugPrintf("net_msg: MSG_STATUS\n");

		lpStatus							= (LPSTATUSMSG)&CommBuff[0];
        lpStatus->MsgCode					= msg;
        lpStatus->WhoIAm					= WhoIAm;
        lpStatus->IsHost					= IsHost;
		lpStatus->Status					= MyGameStatus;
		lpStatus->TeamNumber				= TeamNumber[WhoIAm];
		lpStatus->IAmReady					= PlayerReady[WhoIAm];
		lpStatus->Pickups					= Ships[WhoIAm].Pickups;	 
		lpStatus->RegenSlots				= Ships[WhoIAm].RegenSlots;
		lpStatus->Mines						= Ships[WhoIAm].Mines;		 
		lpStatus->Triggers					= Ships[WhoIAm].Triggers;	 
		lpStatus->TrigVars					= Ships[WhoIAm].TrigVars;	 
		nBytes = sizeof( STATUSMSG );
		flags |= NETWORK_RELIABLE;
        break;

	case MSG_NETSETTINGS:
		DebugPrintf("net_msg: MSG_NETSETTINGS\n");

		lpNetSettingsMsg						= (LPNETSETTINGSMSG)&CommBuff[0];
        lpNetSettingsMsg->MsgCode				= msg;
        lpNetSettingsMsg->WhoIAm				= WhoIAm;
        lpNetSettingsMsg->IsHost				= IsHost;
		lpNetSettingsMsg->PacketsPerSecond		= NetUpdateInterval;
		lpNetSettingsMsg->CollisionPerspective	= ColPerspective;
		lpNetSettingsMsg->ShortPackets			= UseShortPackets;
		nBytes = sizeof( NETSETTINGSMSG );
		flags |= NETWORK_RELIABLE;
		break;

    case MSG_LONGSTATUS:
    	DebugPrintf("net_msg: MSG_LONGSTATUS\n");

		lpLongStatus = (LPLONGSTATUSMSG)&CommBuff[0];
        lpLongStatus->MsgCode				= msg;
        lpLongStatus->WhoIAm				= WhoIAm;
        lpLongStatus->Status.MsgCode		= msg;
        //lpLongStatus->Status.WhoIAm			= WhoIAm; // do not use this field
        lpLongStatus->Status.IsHost			= IsHost;
		// telling everyone what I am currently doing....
		lpLongStatus->Status.Status			= MyGameStatus;
		lpLongStatus->Status.TeamNumber		= TeamNumber[WhoIAm];
		lpLongStatus->Status.IAmReady		= PlayerReady[WhoIAm];
		lpLongStatus->Status.Pickups		= Ships[WhoIAm].Pickups;	 
		lpLongStatus->Status.RegenSlots		= Ships[WhoIAm].RegenSlots;
		lpLongStatus->Status.Mines			= Ships[WhoIAm].Mines;		 
		lpLongStatus->Status.Triggers		= Ships[WhoIAm].Triggers;	 
		lpLongStatus->Status.TrigVars		= Ships[WhoIAm].TrigVars;
		strncpy( &lpLongStatus->LevelName[0], &ShortLevelNames[NewLevelNum][0], 32 );
		{ // send update if level change
			static int16_t MyNewLevelNum = 0;
			if(MyNewLevelNum != NewLevelNum)
				reset_tracker();
			MyNewLevelNum = NewLevelNum;
		}
		lpLongStatus->RandomStartPosModify = RandomStartPosModify;
		nBytes = sizeof( LONGSTATUSMSG );
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTPICKUP:
    	DebugPrintf("net_msg: MSG_SHORTPICKUP\n");

		lpShortPickup = (LPSHORTPICKUPMSG)&CommBuff[0];
        lpShortPickup->MsgCode	= msg;
        lpShortPickup->WhoIAm	= WhoIAm;
        lpShortPickup->Pickups	= Ships[ShipNum].Pickups;
		GenPickupList( ShipNum, &lpShortPickup->ShortPickup[0] , &lpShortPickup->HowManyPickups , Ships[ShipNum].Pickups );
		nBytes = sizeof( SHORTPICKUPMSG );
		to = Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTREGENSLOT:
    	DebugPrintf("net_msg: MSG_SHORTREGENSLOT\n");

		lpShortRegenSlot = (LPSHORTREGENSLOTMSG)&CommBuff[0];
        lpShortRegenSlot->MsgCode		= msg;
        lpShortRegenSlot->WhoIAm		= WhoIAm;
        lpShortRegenSlot->RegenSlots	= Ships[ShipNum].RegenSlots;
		GenRegenSlotList( ShipNum, &lpShortRegenSlot->ShortRegenSlot[0] , &lpShortRegenSlot->HowManyRegenSlots , Ships[ShipNum].RegenSlots );
		nBytes	= sizeof( SHORTREGENSLOTMSG );
		to = Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTTRIGGER:
    	DebugPrintf("net_msg: MSG_SHORTTRIGGER\n");

		lpShortTrigger = (LPSHORTTRIGGERMSG)&CommBuff[0];
        lpShortTrigger->MsgCode		= msg;
        lpShortTrigger->WhoIAm		= WhoIAm;
        lpShortTrigger->Triggers	= Ships[ShipNum].Triggers;
		GenTriggerList( ShipNum, &lpShortTrigger->ShortTrigger[0] , &lpShortTrigger->HowManyTriggers, Ships[ShipNum].Triggers );
		nBytes	= sizeof( SHORTTRIGGERMSG );
		to	= Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTTRIGVAR:
    	DebugPrintf("net_msg: MSG_SHORTTRIGVAR\n");

		lpShortTrigVar = (LPSHORTTRIGVARMSG)&CommBuff[0];
        lpShortTrigVar->MsgCode		= msg;
        lpShortTrigVar->WhoIAm		= WhoIAm;
        lpShortTrigVar->TrigVars	= Ships[ShipNum].TrigVars;
		GenTrigVarList( ShipNum, &lpShortTrigVar->ShortTrigVar[0] , &lpShortTrigVar->HowManyTrigVars, Ships[ShipNum].TrigVars );
        nBytes = sizeof( SHORTTRIGVARMSG );
		to = Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
        break;


    case MSG_SHORTMINE:
    	DebugPrintf("net_msg: MSG_SHORTMINE\n");

		lpShortMine				= (LPSHORTMINEMSG)&CommBuff[0];
        lpShortMine->MsgCode	= msg;
        lpShortMine->WhoIAm		= WhoIAm;
        lpShortMine->Mines		= Ships[ShipNum].Mines;
		GenMineList( ShipNum, &lpShortMine->ShortMine[0] , &lpShortMine->HowManyMines, Ships[ShipNum].Mines );
		nBytes	= sizeof( SHORTMINEMSG );
		to = Ships[ShipNum].network_player;
		flags |= NETWORK_RELIABLE;
        break;

    case MSG_TEXTMSG:
    	DebugPrintf("net_msg: MSG_TEXTMSG\n");

		lpTextMsg = (LPTEXTMSG)&CommBuff[0];
        lpTextMsg->MsgCode	= msg;
        lpTextMsg->WhoIAm	= WhoIAm;
        
		switch( Type )
		{
			case TEXTMSGTYPE_Taunt1:
				strncpy( &lpTextMsg->Text[0]	, &MacroText1.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = TauntMessageColour;
				break;
			case TEXTMSGTYPE_Taunt2:
				strncpy( &lpTextMsg->Text[0]	, &MacroText2.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = TauntMessageColour;
				break;
			case TEXTMSGTYPE_Taunt3:
				strncpy( &lpTextMsg->Text[0]	, &MacroText3.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = TauntMessageColour;
				break;
			case TEXTMSGTYPE_TitleMessage:
				strncpy( &lpTextMsg->Text[0]	, &MacroText4.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = SystemMessageColour;
				break;
			case TEXTMSGTYPE_JoiningTeamGame:
	//			strncpy( &lpTextMsg->Text[0]	, "%s is joining the game" , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = SystemMessageColour;
				break;
			case TEXTMSGTYPE_EnteredWatchMode:
	//			strncpy( &lpTextMsg->Text[0]	, "%s entered watch mode" , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = SystemMessageColour;
				break;
			case TEXTMSGTYPE_ExitedWatchMode:
	//			strncpy( &lpTextMsg->Text[0]	, "%s exited watch mode" , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = SystemMessageColour;
				break;
			case TEXTMSGTYPE_QuickTaunt:
				strncpy( &lpTextMsg->Text[0]	, &QuickText.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = MyMessageColour;
				AddPlayerMessageToQue( MessageColour, "%s", (char*) &lpTextMsg->Text[0] );
				// sending version request
				if(strcmp(&lpTextMsg->Text[0], (const char *) "version") == 0)
				{
					// display my version number
					AddColourMessageToQue(SystemMessageColour, "%s", YourVersion );
					VersionMessage[0] = 0;
				}
				break;
			case TEXTMSGTYPE_QuickTauntWhisper:
				strncpy( &lpTextMsg->Text[0]	, &QuickTextWhisper.text[0] , MAXTEXTMSG );
				lpTextMsg->TextMsgType = Type;
				MessageColour = MyMessageColour;
				AddPlayerMessageToQue( MessageColour, "%s", (char*) &lpTextMsg->Text[0] );
				break;
			case TEXTMSGTYPE_CaptureFlagMessage:
				lpTextMsg->TextMsgType = Type;
				strncpy( &lpTextMsg->Text[0]	, CTFMessage , MAXTEXTMSG );
				PlaySfx( SFX_MyTeamGotFlag , FlagVolume );
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_ScoredWithFlag:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				PlaySfx( SFX_MyTeamScored, FlagVolume );
				MessageColour = FlagMessageColour;
				flags |= NETWORK_RELIABLE;
				break;
			case TEXTMSGTYPE_ReturningFlag:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_ReturnedFlag:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_FlagDriftedIn:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_FlagEscaped:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_BountyMessage:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = 0;
				PlaySfx( SFX_MyTeamGotFlag , FlagVolume );
				MessageColour = FlagMessageColour;
				break;
			case TEXTMSGTYPE_SpeechTaunt:
				lpTextMsg->TextMsgType = Type;
				lpTextMsg->Text[0] = CurrentTauntVariant;
				MessageColour = PlayerMessageColour;
				break;
			default:
				lpTextMsg->TextMsgType = TEXTMSGTYPE_Taunt1;
		}
		// put a null at the end just in case...
		lpTextMsg->Text[MAXTEXTMSG-1] = 0;
		nBytes = sizeof( TEXTMSG );

		// quick taunt already delt with
		if (MyGameStatus != STATUS_StartingMultiplayer && Type != TEXTMSGTYPE_QuickTaunt && Type != TEXTMSGTYPE_QuickTauntWhisper)
			AddColourMessageToQue( MessageColour, "%s", (char*) &lpTextMsg->Text[0] );
		break;

	}

// goto send if you want to break out of the switch
send:
	
	// only record if message is sent to whole of the group....
#ifdef DEMO_SUPPORT
	if( RecordDemo )
	{
		if(
		   ( MyGameStatus == STATUS_Normal )
		&& ( !to )
		&& ( msg != MSG_INIT ) 
		&& ( msg != MSG_HEREIAM ) 
		&& ( msg != MSG_SHORTPICKUP    ) 
		&& ( msg != MSG_SHORTREGENSLOT ) 
		&& ( msg != MSG_SHORTTRIGGER   ) 
		&& ( msg != MSG_SHORTTRIGVAR   ) 
		&& ( msg != MSG_SHORTMINE      ) )
		{
			QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
			TempTime = TempTime - GameStartedTime;
			Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
			Demo_fwrite( &nBytes, sizeof(int), 1, DemoFp );
			Demo_fwrite( &my_network_id, sizeof(DPID), 1, DemoFp );
			Demo_fwrite( &CommBuff[0], nBytes, 1, DemoFp );
		}
	}
#endif

	BytesPerSecSent += nBytes;

	//DebugPrintf("Sending message type, %s  bytes %lu\n", msg_to_str(msg), nBytes);

	if(!to)
		network_broadcast( (void*) &CommBuff[0], nBytes, flags, channel );
	else
		network_send( to, (void*) &CommBuff[0], nBytes, flags, channel );

}

/*===================================================================
	Procedure	:		Read packet stuff from a file and pass it on..
	Input		:		nothing
	Output		:		nothing
===================================================================*/

void DemoPlayingNetworkGameUpdate()
{
#ifdef DEMO_SUPPORT
    DWORD               nBytes;
	int i;
	size_t	size;

	while( 1 && DemoFp)
	{
		if( DemoTimeSoFar )
		{
			if( DemoTimeSoFar > GameElapsedTime )
			{
			 	if (ferror(DemoFp) || feof(DemoFp) )
				{
					fclose( DemoFp );
					DemoFp = NULL;
					SpecialDestroyGame();
					return;
				}
				return;
			}
		}else{
			size = fread( &DemoTimeSoFar , sizeof(LONGLONG), 1, DemoFp );
			if( size != 1 || ferror(DemoFp) || feof(DemoFp) ) 
			{
				PreDemoEndMyGameStatus = MyGameStatus;

				if( Debug )
					for( i = 0 ; i < 256 ; i++ )
						if( PacketGot[i] )
							DebugPrintf("num %3d quantity %12d size %12d\n", i, PacketGot[i] , PacketSize[i] );
								
				QueryPerformanceCounter((LARGE_INTEGER *) &DemoEndedTime);

				fclose( DemoFp );
				DemoFp = NULL;

				SpecialDestroyGame();

				TimeDiff = DemoEndedTime - DemoStartedTime;
				DemoAvgFps = DemoGameLoops /  ( (float) TimeDiff / (float) Freq );
//				DebugPrintf("Average Demo Framrate %d.%02d\n",   (int)( DemoAvgFps * 100 ) / 100,
//															 	(int)( DemoAvgFps * 100 ) % 100 
//															 	);                       
//				DebugPrintf("In %d Gameloops\n",DemoGameLoops);
			
				
				return;
			}
	
			if( DemoTimeSoFar > GameElapsedTime )
				return;
		}
		
		fread( &nBytes , sizeof(DWORD), 1, DemoFp );
		fread( &from_network_id , sizeof(DPID), 1, DemoFp );
		fread( &ReceiveCommBuff[0] , nBytes , 1, DemoFp );
		
		// During Demo Playback we dont want to interperate any System messages....
		if ( from_network_id != DPID_SYSMSG ) EvaluateMessage( nBytes , &ReceiveCommBuff[0] );
		DemoTimeSoFar = 0;

  
	}
#endif
}


/*===================================================================
	Procedure	:		Read packet stuff from a file if its a ship update..
						scan ahead to find the next one and write in a
						interpolate msg..
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void DemoClean( void )
{
#ifdef DEMO_SUPPORT
    DWORD       nBytes;
	size_t		size;
	fpos_t		Currentpos; 
	LONGLONG	DemoTimeSoFar2;
	DPID		from_dcoID2;
    DWORD       nBytes2;
    LPUPDATEMSG			lpUpdate;
    LPFUPDATEMSG		lpFUpdate;
    LPVERYSHORTUPDATEMSG lpVeryShortUpdate;
    LPVERYSHORTFUPDATEMSG		lpVeryShortFUpdate;
	INTERPOLATEMSG Interpolate;
	VERYSHORTINTERPOLATEMSG VeryShortInterpolate;
	bool		IsShortPackets = false;
	int			FoundOne;

	while(1)
	{
		size = fread( &DemoTimeSoFar , sizeof(LONGLONG), 1, DemoFp );
   		// check for end of file...
		if( !size )	return;
		fread( &nBytes , sizeof(DWORD), 1, DemoFp );
		fread( &from_network_id , sizeof(DPID), 1, DemoFp );
		fread( &CommBuff[0] , nBytes , 1, DemoFp );

#if 0
		// Special model num correction stuff...And Power Level Stuff
		switch( CommBuff[0] )
		{
		case MSG_UPDATE:
			lpUpdate = (LPUPDATEMSG) &CommBuff[0];
			break;
		case MSG_FUPDATE:
			break;
		}
#endif
		
		// write out the message..
		fwrite( &DemoTimeSoFar, sizeof(LONGLONG), 1, DemoFpClean );
		fwrite( &nBytes, sizeof(int), 1, DemoFpClean );
		fwrite( &from_network_id, sizeof(DPID), 1, DemoFpClean );
		fwrite( &CommBuff[0], nBytes, 1, DemoFpClean );

		if ( ( from_network_id != DPID_SYSMSG ) && ( CommBuff[0] == MSG_INTERPOLATE ) )
		{
			// Has allready been cleaned...
			return;
		}

		// check if its an update message....
		if ( ( from_network_id != DPID_SYSMSG ) && ( ( CommBuff[0] == MSG_UPDATE ) ||
												( CommBuff[0] == MSG_FUPDATE ) ||
												( CommBuff[0] == MSG_VERYSHORTUPDATE ) ||
												( CommBuff[0] == MSG_VERYSHORTFUPDATE ) ) )
		{
			// we have one update message we now have to search for another...
			Interpolate.MsgCode = MSG_INTERPOLATE;
			VeryShortInterpolate.MsgCode = MSG_VERYSHORTINTERPOLATE;
			switch( CommBuff[0] )
			{
			case MSG_UPDATE:
				lpUpdate = (LPUPDATEMSG) &CommBuff[0];
				Interpolate.WhoIAm = lpUpdate->WhoIAm;
				break;
			case MSG_FUPDATE:
				lpFUpdate = (LPFUPDATEMSG) &CommBuff[0];
				Interpolate.WhoIAm = lpFUpdate->WhoIAm;
				break;
			case MSG_VERYSHORTUPDATE:
				lpVeryShortUpdate = (LPVERYSHORTUPDATEMSG) &CommBuff[0];
				VeryShortInterpolate.WhoIAm = lpVeryShortUpdate->WhoIAm;
				IsShortPackets = true;
				break;
			case MSG_VERYSHORTFUPDATE:
				lpVeryShortFUpdate = (LPVERYSHORTFUPDATEMSG) &CommBuff[0];
				VeryShortInterpolate.WhoIAm = lpVeryShortFUpdate->WhoIAm;
				IsShortPackets = true;
				break;
			}
			fgetpos( DemoFp , &Currentpos );	// store the current position..so we can go back to it..

			FoundOne = 0;
			while( FoundOne == 0 )
			{
				size = fread( &DemoTimeSoFar2 , sizeof(LONGLONG), 1, DemoFp );
				// check for end of file...
				if( !size )
				{
					FoundOne = -1;
					break;
				}

				fread( &nBytes2 , sizeof(DWORD), 1, DemoFp );
				fread( &from_dcoID2 , sizeof(DPID), 1, DemoFp );
				fread( &CommBuff[0] , nBytes2 , 1, DemoFp );


				if ( ( from_dcoID2 != DPID_SYSMSG ) && ( ( CommBuff[0] == MSG_UPDATE ) ||
														( CommBuff[0] == MSG_FUPDATE ) ||
														( CommBuff[0] == MSG_VERYSHORTUPDATE ) ||
														( CommBuff[0] == MSG_VERYSHORTFUPDATE ) ) )
				{
					switch( CommBuff[0] )
					{
					case MSG_UPDATE:
						lpUpdate =(LPUPDATEMSG) &CommBuff[0];
						if( Interpolate.WhoIAm == lpUpdate->WhoIAm )
						{
							// found next update for this ship
							Interpolate.NextPos = lpUpdate->ShortGlobalShip.Pos;
							Interpolate.NextQuat = lpUpdate->ShortGlobalShip.Quat;
#ifdef	SHORTBANK
							Interpolate.NextBank	= (float) (lpUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
#else
							Interpolate.NextBank	= lpUpdate->ShortGlobalShip.Bank;
#endif
							Interpolate.NextTime = DemoTimeSoFar2;
							FoundOne = 1;
						}
						break;
					case MSG_FUPDATE:
						lpFUpdate = (LPFUPDATEMSG) &CommBuff[0];
						if( Interpolate.WhoIAm == lpFUpdate->WhoIAm )
						{
							// found next update for this ship
							Interpolate.NextPos = lpFUpdate->ShortGlobalShip.Pos;
							Interpolate.NextQuat = lpFUpdate->ShortGlobalShip.Quat;
#ifdef	SHORTBANK
							Interpolate.NextBank	= (float) (lpFUpdate->ShortGlobalShip.Bank / SHORTBANKMODIFIER);
#else
							Interpolate.NextBank	= lpFUpdate->ShortGlobalShip.Bank;
#endif
							Interpolate.NextTime = DemoTimeSoFar2;
							FoundOne = 1;
						}
						break;
					case MSG_VERYSHORTUPDATE:
						lpVeryShortUpdate =(LPVERYSHORTUPDATEMSG) &CommBuff[0];
						if( VeryShortInterpolate.WhoIAm == lpVeryShortUpdate->WhoIAm )
						{
							// found next update for this ship
							VeryShortInterpolate.NextPos = lpVeryShortUpdate->ShortGlobalShip.Pos;
							VeryShortInterpolate.NextQuat	= lpVeryShortUpdate->ShortGlobalShip.Quat;
							VeryShortInterpolate.NextBank	= lpVeryShortUpdate->ShortGlobalShip.Bank;
							VeryShortInterpolate.NextTime = DemoTimeSoFar2;
							FoundOne = 1;
						}
						break;
					case MSG_VERYSHORTFUPDATE:
						lpVeryShortFUpdate = (LPVERYSHORTFUPDATEMSG) &CommBuff[0];
						if( VeryShortInterpolate.WhoIAm == lpVeryShortFUpdate->WhoIAm )
						{
							// found next update for this ship
							VeryShortInterpolate.NextPos = lpVeryShortFUpdate->ShortGlobalShip.Pos;
							VeryShortInterpolate.NextQuat	= lpVeryShortFUpdate->ShortGlobalShip.Quat;
							VeryShortInterpolate.NextBank	= lpVeryShortFUpdate->ShortGlobalShip.Bank;
							VeryShortInterpolate.NextTime = DemoTimeSoFar2;
							FoundOne = 1;
						}
						break;
					}
				}
			}
			if( FoundOne == 1 )
			{
				if( !IsShortPackets )
				{
					fwrite( &DemoTimeSoFar, sizeof(LONGLONG), 1, DemoFpClean );
					nBytes = sizeof( INTERPOLATEMSG );
					fwrite( &nBytes, sizeof(int), 1, DemoFpClean );
					fwrite( &from_network_id, sizeof(DPID), 1, DemoFpClean );
					fwrite( &Interpolate, nBytes, 1, DemoFpClean );
				}else{
					fwrite( &DemoTimeSoFar, sizeof(LONGLONG), 1, DemoFpClean );
					nBytes = sizeof( VERYSHORTINTERPOLATEMSG );
					fwrite( &nBytes, sizeof(int), 1, DemoFpClean );
					fwrite( &from_network_id, sizeof(DPID), 1, DemoFpClean );
					fwrite( &VeryShortInterpolate, nBytes, 1, DemoFpClean );
				}
			}
			// set the position of the file back...
			fsetpos( DemoFp , &Currentpos );
		}
	}
#endif
}

/*===================================================================
	Procedure	:		Create a regen effect...
	Input		:		u_int16_t ship....
	Output		:		nothing
===================================================================*/


void CreateReGen( u_int16_t ship )
{
	u_int16_t	Model;

	if( ship == WhoIAm )
	{
		if ( !Random_Range( 4 ) )
			PlaySfxWithTrigger( SFX_PlayerGenerate, SFX_BIKER_GP );
		else
			PlaySfx( SFX_PlayerGenerate, 1.0F );
	}else{
		PlayPannedSfx( SFX_PlayerGenerate, Ships[ ship ].Object.Group , &Ships[ ship ].Object.Pos, 0.0F );
	}

	Model =	FindFreeModel();
	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_SHIP;
		Models[ Model ].Owner = ship;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = Ships[ship].Object.Pos;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Regen;
		Models[ Model ].MaxScale = 2.0F;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].Group = Ships[ship].Object.Group;
		Models[ Model ].LifeCount = 600.0F;
	}
}

/*===================================================================
	Procedure	:		FindSameLevel....
	Input		:		char * Name
	Output		:		int -1 no level....
===================================================================*/
extern	int16_t			NumLevels;
int FindSameLevel( char * Name )
{
	int i;
    for (i = 0; i < NumLevels; i++)
	{
		if( strcasecmp( (char*) &ShortLevelNames[i][0] , Name ) == 0 ) 
		{
			return i;
		}
    }
	return -1;
}

void Demo_fwrite( const void *buffer, size_t size, size_t count , FILE *stream )
{
	if( !RecordDemo || !DemoFp )
		return;
	fwrite( buffer, size , count , stream );
}


/*===================================================================
	Procedure	:		StopDemoRecording
	Input		:		char * Name
	Output		:		int -1 no level....
===================================================================*/
void StopDemoRecording( void )
{
	if( DemoFp )	// make sure that changing level stop any demo from recording!!!!
	{
		fclose( DemoFp );
		DemoFp = NULL;
		RecordDemo = false;
		PlayDemo = false;
	}
}

/*===================================================================
	Procedure	:		Build Ship Flags...
	Input		:		BYTE Player
	Output		:		u_int32_t Flags
===================================================================*/
u_int32_t BuildShipFlags( BYTE Player )
{
	u_int32_t Flags;

	Flags = Ships[Player].Object.Flags;    // Flags

	Flags &= ~( SHIP_IsHost + SHIP_Enable + SHIP_Invul + SHIP_PowerLevelBit1+SHIP_PowerLevelBit2 + SHIP_ModeBit1 + SHIP_ModeBit2 +
				SHIP_Shield + SHIP_HullBit1+SHIP_HullBit2 + SHIP_NumMultiplesBit1 + SHIP_NumMultiplesBit2 + SHIP_NumMultiplesBit3 + SHIP_NumMultiplesBit4 );
	if( IsHost ) Flags |= SHIP_IsHost;
	if( Ships[ Player ].enable ) Flags |= SHIP_Enable;
	if( Ships[Player].InvulTimer ) Flags |= SHIP_Invul;
	Flags |= (Ships[Player].Object.PowerLevel&3) << SHIP_PowerLevel_Bit1;
	Flags |= (Ships[Player].Object.Mode&3) << SHIP_Mode_Bit1;
	if( Ships[Player].Object.Shield > 0.0F )
	{
		Flags |= SHIP_Shield;
	}
	if( Ships[Player].Object.Hull > 64.0F )
	{
		Flags |= SHIP_HullBit1+SHIP_HullBit2;
	}else{
		Flags |= ( (u_int32_t) ( Ships[Player].Object.Hull * (1.0F/16.0F) ) ) << SHIP_Hull_Bit1;
	}
	Flags |= (Ships[ Player ].NumMultiples&15) << SHIP_NumMultiples_Bit1;
	if( Ships[Player].headlights ) Flags |= SHIP_Light;
	return Flags;
}



/*===================================================================
	Procedure	:		Build Ship Flags...
	Input		:		BYTE Player
	Output		:		u_int32_t Flags
===================================================================*/
void UnPackShipFlags( BYTE Player , u_int32_t Flags )
{
	Ships[ Player ].Object.Flags = Flags;
	if( ( Flags & SHIP_Enable ) ) Ships[ Player ].enable = 1;
	else Ships[ Player ].enable = 0;
	if( ( Flags & SHIP_Invul ) ) Ships[ Player ].Invul = true;
	else Ships[ Player ].Invul = false;

	Ships[Player].Object.PowerLevel = (int16_t)((Flags >> SHIP_PowerLevel_Bit1 ) & 3);
	Ships[Player].Object.Mode = (BYTE)((Flags >> SHIP_Mode_Bit1 ) & 3);

	if( ( Flags & SHIP_Shield ) ) Ships[ Player ].Object.Shield = 1.0F;
	else Ships[ Player ].Object.Shield = 0.0F;
 	Ships[Player].Object.Hull = ( ( (Flags>>SHIP_Hull_Bit1) & 3 ) + 1 ) * 16.0F;

	Ships[Player].NumMultiples = (BYTE)((Flags >> SHIP_NumMultiples_Bit1 ) & 15);
}

/*===================================================================
	Procedure	:		Set Ship Bank and Mat..
	Input		:		OBJECT *
	Output		:		void
===================================================================*/
void SetShipBankAndMat( OBJECT * ShipObjPnt )
{
	QUAT	StepQuat;
	MakeQuat( 0.0F , 0.0F , ShipObjPnt->Bank, &StepQuat );
	QuatMultiply(  &ShipObjPnt->Quat , &StepQuat , &ShipObjPnt->FinalQuat );
	QuatToMatrix( &ShipObjPnt->FinalQuat, &ShipObjPnt->FinalMat );
	MatrixTranspose( &ShipObjPnt->FinalMat, &ShipObjPnt->FinalInvMat );
}


/*===================================================================
	Procedure	:		Update Ammo And Validate Message...
	Input		:		void	*	Message
	Output		:		bool		True/False ( Valid Message )
===================================================================*/
bool UpdateAmmoAndValidateMessage( void * Message )
{
	BYTE					Weapon = 0;
	BYTE					TempWeapon;
	BYTE					Player;
	BYTE		*			MsgPnt;
	BYTE					MsgType;
    LPDROPPICKUPMSG			lpDropPickup;
    LPVERYSHORTDROPPICKUPMSG lpVeryShortDropPickup;
    LPFUPDATEMSG			lpFUpdate;
    LPVERYSHORTFUPDATEMSG	lpVeryShortFUpdate;
    LPGROUPONLY_VERYSHORTFUPDATEMSG	lpGroupOnly_VeryShortFUpdate;
    LPPRIMBULLPOSDIRMSG		lpPrimBullPosDir;
    LPSECBULLPOSDIRMSG		lpSecBullPosDir;
	int16_t					PowerLevel;
	float					Ammo;
	int16_t					Count, Count2;
	float					TempFloat;
	BYTE					NumMultiples;

	MsgPnt = (BYTE *) Message;
	MsgType = MsgPnt[ 0 ];
	Player = MsgPnt[ 1 ];

	PowerLevel = Host_PowerLevel[ Player ]; //Ships[ Player ].Object.PowerLevel;

	switch( MsgType )
    {
		case MSG_DROPPICKUP:
			lpDropPickup = (LPDROPPICKUPMSG)MsgPnt;

			switch( lpDropPickup->PickupInfo.Type )
			{
				case PICKUP_GeneralAmmo:
					Host_GeneralAmmo[ lpDropPickup->WhoIAm ] -= GENERALAMMOPERPOD;
					if( Host_GeneralAmmo[ lpDropPickup->WhoIAm ] < 0.0F ) Host_GeneralAmmo[ lpDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_PyroliteAmmo:
					Host_PyroliteAmmo[ lpDropPickup->WhoIAm ] -= PYROLITEAMMOPERPOD;
					if( Host_PyroliteAmmo[ lpDropPickup->WhoIAm ] < 0.0F ) Host_PyroliteAmmo[ lpDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_SussGunAmmo:
					Host_SussGunAmmo[ lpDropPickup->WhoIAm ] -= SUSSGUNAMMOPERPOD;
					if( Host_SussGunAmmo[ lpDropPickup->WhoIAm ] < 0.0F ) Host_SussGunAmmo[ lpDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_Trojax: 
				case PICKUP_Pyrolite:
				case PICKUP_Transpulse:
				case PICKUP_SussGun:
				case PICKUP_Laser:
					Host_PrimaryWeaponsGot[ Player ][ ( lpDropPickup->PickupInfo.Type - PICKUP_Trojax ) + 1 ] = 0;
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
				case PICKUP_Parasite:
					for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
					{
						if( SecondaryFromPickupTab[ ( Count * 2 ) ] == lpDropPickup->PickupInfo.Type )
						{
							Weapon = (BYTE) Count;
							break;
						}
					}

					if( Host_SecondaryAmmo[ Player ][ Weapon ] > SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] )
						Host_SecondaryAmmo[ Player ][ Weapon ] -= SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ];
					else
					{
						Host_SecondaryAmmo[ Player ][ Weapon ] = 0;
						Host_SecondaryWeaponsGot[ Player ][ Weapon ] = 0;
					}
					break;
				default:
					break;
			}
			break;


		case MSG_VERYSHORTDROPPICKUP:
			lpVeryShortDropPickup = (LPVERYSHORTDROPPICKUPMSG)MsgPnt;

			switch( lpVeryShortDropPickup->PickupInfo.Type )
			{
				case PICKUP_GeneralAmmo:
					Host_GeneralAmmo[ lpVeryShortDropPickup->WhoIAm ] -= GENERALAMMOPERPOD;
					if( Host_GeneralAmmo[ lpVeryShortDropPickup->WhoIAm ] < 0.0F ) Host_GeneralAmmo[ lpVeryShortDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_PyroliteAmmo:
					Host_PyroliteAmmo[ lpVeryShortDropPickup->WhoIAm ] -= PYROLITEAMMOPERPOD;
					if( Host_PyroliteAmmo[ lpVeryShortDropPickup->WhoIAm ] < 0.0F ) Host_PyroliteAmmo[ lpVeryShortDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_SussGunAmmo:
					Host_SussGunAmmo[ lpVeryShortDropPickup->WhoIAm ] -= SUSSGUNAMMOPERPOD;
					if( Host_SussGunAmmo[ lpVeryShortDropPickup->WhoIAm ] < 0.0F ) Host_SussGunAmmo[ lpVeryShortDropPickup->WhoIAm ] = 0.0F;
					break;

				case PICKUP_Trojax: 
				case PICKUP_Pyrolite:
				case PICKUP_Transpulse:
				case PICKUP_SussGun:
				case PICKUP_Laser:
					Host_PrimaryWeaponsGot[ Player ][ ( lpVeryShortDropPickup->PickupInfo.Type - PICKUP_Trojax ) + 1 ] = 0;
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
				case PICKUP_Parasite:
					for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
					{
						if( SecondaryFromPickupTab[ ( Count * 2 ) ] == lpVeryShortDropPickup->PickupInfo.Type )
						{
							Weapon = (BYTE) Count;
							break;
						}
					}

					if( Host_SecondaryAmmo[ Player ][ Weapon ] > SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ] )
					{
						Host_SecondaryAmmo[ Player ][ Weapon ] -= SecondaryFromPickupTab[ ( Weapon * 2 ) + 1 ];
					}
					else
					{
						Host_SecondaryAmmo[ Player ][ Weapon ] = 0;
						Host_SecondaryWeaponsGot[ Player ][ Weapon ] = 0;
					}
					break;

				default:
					break;
			}
			break;


		case MSG_VERYSHORTFUPDATE:
			lpVeryShortFUpdate = (LPVERYSHORTFUPDATEMSG)MsgPnt;

			if( ( Ships[ Player ].SuperNashramTimer == 0.0F ) )
			{
				if( ( Ships[lpVeryShortFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) )
				{
					NumMultiples = (BYTE) ((lpVeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				}
				else
				{
					NumMultiples = 0;
				}

				if( NumMultiples < Host_NumOfOrbitals[ Player ] )
				{
					if( Host_NumOfOrbitals[ Player ] > 1 )
					{
						for( Count = 0; Count < ( Host_NumOfOrbitals[ Player ] - 1 ); Count++ )
						{
							for( Count2 = ( Count + 1 ); Count2 < Host_NumOfOrbitals[ Player ]; Count2++ )
							{
								if( Host_OrbAmmo[ Player ][ Count ] < Host_OrbAmmo[ Player ][ Count2 ] )
								{
									TempFloat = Host_OrbAmmo[ Player ][ Count2 ];
									Host_OrbAmmo[ Player ][ Count2 ] = Host_OrbAmmo[ Player ][ Count ];
									Host_OrbAmmo[ Player ][ Count ] = TempFloat;
								}
							}
						}
					}

					NumOrbs += ( Host_NumOfOrbitals[ Player ] - NumMultiples );
					Host_NumOfOrbitals[ Player ] = NumMultiples;
				}
			}

			if( ( lpVeryShortFUpdate->ShortGlobalShip.Flags & SHIP_MulFire ) )
			{
				for( Count = 0; Count < Host_NumOfOrbitals[ Player ]; Count++ )
				{
//					if( !( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
					if( Ships[ Player ].SuperNashramTimer == 0.0F )
					{
						Host_OrbAmmo[ Player ][ Count ] -= (float) PrimaryWeaponAttribs[ ORBITPULSAR ].AmmoUsage[ PowerLevel ];

						if( Host_OrbAmmo[ Player ][ Count ] < 0.0F )
						{
							if( Host_NumOfOrbitals[ Player ] )
							{
								Host_NumOfOrbitals[ Player ]--;
								Host_OrbAmmo[ Player ][ Count ] = Host_OrbAmmo[ Player ][ Host_NumOfOrbitals[ Player ] ]; 
								NumOrbs++;
							}
						}
					}
				}
			}

			if( ( lpVeryShortFUpdate->ShortGlobalShip.Flags & SHIP_SecFire ) )
			{
				Weapon = lpVeryShortFUpdate->ShortGlobalShip.Secondary;
				TempWeapon = ConvSecToNormWeapon( Weapon );

//				AddMessageToQue( "Update:FireSecondary : %s, Got = %d, Ammo = %d", SecondaryDescription[ TempWeapon ],
//																			Host_SecondaryWeaponsGot[ Player ][ TempWeapon ],
//																			Host_SecondaryAmmo[ Player ][ TempWeapon ] );

				if( !Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] ) return( false );
				if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) return( false );

				switch( Weapon )
				{
					case MUGMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case MULTIPLEMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case TITANSTARSHRAPNEL:
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYTENTACLE:
					case ENEMYDEPTHCHARGE:
						Host_SecAmmoUsed[ TempWeapon ]++;
						if( Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					case PURGEMINE:
					case PINEMINE:
					case QUANTUMMINE:
					case SPIDERMINE:
						if( Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					default:
						break;
				}
			}

			if( lpVeryShortFUpdate->ShortGlobalShip.Flags & SHIP_PrimFire )
			{
				Weapon = lpVeryShortFUpdate->ShortGlobalShip.Primary;

				if( Weapon >= MAXPRIMARYWEAPONS ) break;
				if( !Host_PrimaryWeaponsGot[ Player ][ Weapon ] ) return( false );
				Ammo = GetPrimaryAmmoUsage( Player, Weapon );

				switch( Weapon )
				{
					case PULSAR:
					case TRANSPULSE_CANNON:
					case LASER:
					case NME_BULLET1:
					case NME_PULSAR:
					case NME_TRANSPULSE:
					case NME_LASER:
					case NME_LIGHTNING:
					case FLAMES:
					case NME_POWERLASER:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
							Host_GeneralAmmoUsed += Ammo;
							Host_GeneralAmmo[ Player ] -= Ammo;
							if( Host_GeneralAmmo[ Player ] < 0.0F ) Host_GeneralAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );
						break;

					case TROJAX:
					case NME_TROJAX:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );
						break;

					case PYROLITE_RIFLE:
					case NME_PYROLITE:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_PyroliteAmmo[ Player ] == 0.0F ) return( false );
							Host_PyroliteAmmoUsed += Ammo;
							Host_PyroliteAmmo[ Player ] -= Ammo;
							if( Host_PyroliteAmmo[ Player ] < 0.0F ) Host_PyroliteAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_PyroliteAmmo[ Player ] );
						break;

					case SUSS_GUN:
					case NME_SUSS_GUN:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_SussGunAmmo[ Player ] == 0.0F ) return( false );
							Host_SussGunAmmoUsed += Ammo;
							Host_SussGunAmmo[ Player ] -= Ammo;
							if( Host_SussGunAmmo[ Player ] < 0.0F ) Host_SussGunAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_SussGunAmmo[ Player ] );
						break;

					case ORBITPULSAR:
						break;

					default:
						break;
				}
			}
			break;

		case MSG_GROUPONLY_VERYSHORTFUPDATE:
			lpGroupOnly_VeryShortFUpdate = (LPGROUPONLY_VERYSHORTFUPDATEMSG)MsgPnt;

			if( ( Ships[ Player ].SuperNashramTimer == 0.0F ) )
			{
				if( ( Ships[lpGroupOnly_VeryShortFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) )
				{
					NumMultiples = (BYTE) ((lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				}
				else
				{
					NumMultiples = 0;
				}

				if( NumMultiples < Host_NumOfOrbitals[ Player ] )
				{
					if( Host_NumOfOrbitals[ Player ] > 1 )
					{
						for( Count = 0; Count < ( Host_NumOfOrbitals[ Player ] - 1 ); Count++ )
						{
							for( Count2 = ( Count + 1 ); Count2 < Host_NumOfOrbitals[ Player ]; Count2++ )
							{
								if( Host_OrbAmmo[ Player ][ Count ] < Host_OrbAmmo[ Player ][ Count2 ] )
								{
									TempFloat = Host_OrbAmmo[ Player ][ Count2 ];
									Host_OrbAmmo[ Player ][ Count2 ] = Host_OrbAmmo[ Player ][ Count ];
									Host_OrbAmmo[ Player ][ Count ] = TempFloat;
								}
							}
						}
					}

					NumOrbs += ( Host_NumOfOrbitals[ Player ] - NumMultiples );
					Host_NumOfOrbitals[ Player ] = NumMultiples;
				}
			}

			if( ( lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags & SHIP_MulFire ) )
			{
				for( Count = 0; Count < Host_NumOfOrbitals[ Player ]; Count++ )
				{
//					if( !( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
					if( Ships[ Player ].SuperNashramTimer == 0.0F )
					{
						Host_OrbAmmo[ Player ][ Count ] -= (float) PrimaryWeaponAttribs[ ORBITPULSAR ].AmmoUsage[ PowerLevel ];

						if( Host_OrbAmmo[ Player ][ Count ] < 0.0F )
						{
							if( Host_NumOfOrbitals[ Player ] )
							{
								Host_NumOfOrbitals[ Player ]--;
								Host_OrbAmmo[ Player ][ Count ] = Host_OrbAmmo[ Player ][ Host_NumOfOrbitals[ Player ] ]; 
								NumOrbs++;
							}
						}
					}
				}
			}

			if( ( lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags & SHIP_SecFire ) )
			{
				Weapon = lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Secondary;
				TempWeapon = ConvSecToNormWeapon( Weapon );

//				AddMessageToQue( "Update:FireSecondary : %s, Got = %d, Ammo = %d", SecondaryDescription[ TempWeapon ],
//																			Host_SecondaryWeaponsGot[ Player ][ TempWeapon ],
//																			Host_SecondaryAmmo[ Player ][ TempWeapon ] );

				if( !Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] ) return( false );
				if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) return( false );

				switch( Weapon )
				{
					case MUGMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case MULTIPLEMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case TITANSTARSHRAPNEL:
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYTENTACLE:
					case ENEMYDEPTHCHARGE:
						Host_SecAmmoUsed[ TempWeapon ]++;
						if( Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					case PURGEMINE:
					case PINEMINE:
					case QUANTUMMINE:
					case SPIDERMINE:
						if( Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					default:
						break;
				}
			}

			if( lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Flags & SHIP_PrimFire )
			{
				Weapon = lpGroupOnly_VeryShortFUpdate->ShortGlobalShip.Primary;

				if( Weapon >= MAXPRIMARYWEAPONS ) break;
				if( !Host_PrimaryWeaponsGot[ Player ][ Weapon ] ) return( false );
				Ammo = GetPrimaryAmmoUsage( Player, Weapon );

				switch( Weapon )
				{
					case PULSAR:
					case TRANSPULSE_CANNON:
					case LASER:
					case NME_BULLET1:
					case NME_PULSAR:
					case NME_TRANSPULSE:
					case NME_LASER:
					case NME_LIGHTNING:
					case FLAMES:
					case NME_POWERLASER:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
							Host_GeneralAmmoUsed += Ammo;
							Host_GeneralAmmo[ Player ] -= Ammo;
							if( Host_GeneralAmmo[ Player ] < 0.0F ) Host_GeneralAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );
						break;

					case TROJAX:
					case NME_TROJAX:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );
						break;

					case PYROLITE_RIFLE:
					case NME_PYROLITE:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_PyroliteAmmo[ Player ] == 0.0F ) return( false );
							Host_PyroliteAmmoUsed += Ammo;
							Host_PyroliteAmmo[ Player ] -= Ammo;
							if( Host_PyroliteAmmo[ Player ] < 0.0F ) Host_PyroliteAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_PyroliteAmmo[ Player ] );
						break;

					case SUSS_GUN:
					case NME_SUSS_GUN:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_SussGunAmmo[ Player ] == 0.0F ) return( false );
							Host_SussGunAmmoUsed += Ammo;
							Host_SussGunAmmo[ Player ] -= Ammo;
							if( Host_SussGunAmmo[ Player ] < 0.0F ) Host_SussGunAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_SussGunAmmo[ Player ] );
						break;

					case ORBITPULSAR:
						break;

					default:
						break;
				}
			}
			break;

		case MSG_FUPDATE:
			lpFUpdate = (LPFUPDATEMSG)MsgPnt;

			if( ( Ships[ Player ].SuperNashramTimer == 0.0F ) )
			{
				if( ( Ships[lpFUpdate->WhoIAm].Object.Flags & SHIP_Multiple ) )
				{
					NumMultiples = (BYTE) ((lpFUpdate->ShortGlobalShip.Flags >> SHIP_NumMultiples_Bit1 ) & 15);
				}
				else
				{
					NumMultiples = 0;
				}

				if( NumMultiples < Host_NumOfOrbitals[ Player ] )
				{
					if( Host_NumOfOrbitals[ Player ] > 1 )
					{
						for( Count = 0; Count < ( Host_NumOfOrbitals[ Player ] - 1 ); Count++ )
						{
							for( Count2 = ( Count + 1 ); Count2 < Host_NumOfOrbitals[ Player ]; Count2++ )
							{
								if( Host_OrbAmmo[ Player ][ Count ] < Host_OrbAmmo[ Player ][ Count2 ] )
								{
									TempFloat = Host_OrbAmmo[ Player ][ Count2 ];
									Host_OrbAmmo[ Player ][ Count2 ] = Host_OrbAmmo[ Player ][ Count ];
									Host_OrbAmmo[ Player ][ Count ] = TempFloat;
								}
							}
						}
					}

					NumOrbs += ( Host_NumOfOrbitals[ Player ] - NumMultiples );
					Host_NumOfOrbitals[ Player ] = NumMultiples;
				}
			}

			if( lpFUpdate->ShortGlobalShip.Flags & SHIP_MulFire )
			{
				for( Count = 0; Count < Host_NumOfOrbitals[ Player ]; Count++ )
				{
//					if( !( Ships[ Player ].Object.Flags & SHIP_SuperNashram ) )
					if( ( Ships[ Player ].SuperNashramTimer == 0.0F ) )
					{
						Host_OrbAmmo[ Player ][ Count ] -= (float) PrimaryWeaponAttribs[ ORBITPULSAR ].AmmoUsage[ PowerLevel ];

						if( Host_OrbAmmo[ Player ][ Count ] < 0.0F )
						{
							if( Host_NumOfOrbitals[ Player ] )
							{
								Host_NumOfOrbitals[ Player ]--;
								Host_OrbAmmo[ Player ][ Count ] = Host_OrbAmmo[ Player ][ Host_NumOfOrbitals[ Player ] ]; 
								NumOrbs++;
							}
						}
					}
				}
			}

			if( lpFUpdate->ShortGlobalShip.Flags & SHIP_SecFire )
			{
				Weapon		= lpFUpdate->ShortGlobalShip.Secondary;
				TempWeapon	= ConvSecToNormWeapon( Weapon );

				if( !Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] ) return( false );
				if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) return( false );

//				AddMessageToQue( "Update:FireSecondary : %s, Got = %d, Ammo = %d", SecondaryDescription[ TempWeapon ],
//																			Host_SecondaryWeaponsGot[ Player ][ TempWeapon ],
//																			Host_SecondaryAmmo[ Player ][ TempWeapon ] );

				switch( Weapon )
				{
					case MUGMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case MULTIPLEMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case TITANSTARSHRAPNEL:
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYTENTACLE:
					case ENEMYDEPTHCHARGE:
						Host_SecAmmoUsed[ TempWeapon ]++;
						Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					case PURGEMINE:
					case PINEMINE:
					case QUANTUMMINE:
					case SPIDERMINE:
						Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
						if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
						break;

					default:
						break;
				}
			}

			if( lpFUpdate->ShortGlobalShip.Flags & SHIP_PrimFire )
			{
				Weapon = lpFUpdate->ShortGlobalShip.Primary;

				if( Weapon >= MAXPRIMARYWEAPONS ) break;
				if( !Host_PrimaryWeaponsGot[ Player ][ Weapon ] ) return( false );

				Ammo = GetPrimaryAmmoUsage( Player, Weapon );

				switch( Weapon )
				{
					case PULSAR:
					case TRANSPULSE_CANNON:
					case LASER:
					case NME_BULLET1:
					case NME_PULSAR:
					case NME_TRANSPULSE:
					case NME_LASER:
					case NME_LIGHTNING:
					case FLAMES:
					case NME_POWERLASER:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
							Host_GeneralAmmoUsed += Ammo;
							Host_GeneralAmmo[ Player ] -= Ammo;
							if( Host_GeneralAmmo[ Player ] < 0.0F ) Host_GeneralAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );

						break;

					case TROJAX:
					case NME_TROJAX:
//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );
						break;

					case PYROLITE_RIFLE:
					case NME_PYROLITE:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_PyroliteAmmo[ Player ] == 0.0F ) return( false );
							Host_PyroliteAmmoUsed += Ammo;
							Host_PyroliteAmmo[ Player ] -= Ammo;
							if( Host_PyroliteAmmo[ Player ] < 0.0F ) Host_PyroliteAmmo[ Player ] = 0.0F;
						}
		
//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_PyroliteAmmo[ Player ] );
						break;

					case SUSS_GUN:
					case NME_SUSS_GUN:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
						{
							if( Host_SussGunAmmo[ Player ] == 0.0F ) return( false );
							Host_SussGunAmmoUsed += Ammo;
							Host_SussGunAmmo[ Player ] -= Ammo;
							if( Host_SussGunAmmo[ Player ] < 0.0F ) Host_SussGunAmmo[ Player ] = 0.0F;
						}

//						AddMessageToQue( "Update:FirePrimary : %s, Got = %hd, Ammo = %f", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_SussGunAmmo[ Player ] );
						break;

					case ORBITPULSAR:
						break;

					default:
						break;
				}
			}
			break;

		case MSG_PRIMBULLPOSDIR:
		    lpPrimBullPosDir = (LPPRIMBULLPOSDIRMSG)MsgPnt;
		
			if( lpPrimBullPosDir->PrimBullPosDir.OwnerType != OWNER_SHIP ) break;

			Weapon = lpPrimBullPosDir->PrimBullPosDir.Weapon; 

//			AddMessageToQue( "PrimBullPosDir : %s, Got = %d, Ammo = %d", PrimaryDescription[ Weapon ],
//																	Host_PrimaryWeaponsGot[ Player ][ Weapon ],
//																	Host_GeneralAmmo[ Player ] );

			switch( Weapon )
			{
				case PULSAR:
				case TRANSPULSE_CANNON:
				case LASER:
				case NME_BULLET1:
				case NME_PULSAR:
				case NME_TRANSPULSE:
				case NME_LASER:
				case NME_LIGHTNING:
				case FLAMES:
				case NME_POWERLASER:
						if( Ships[ Player ].SuperNashramTimer == 0.0F )
					{
						if( Host_GeneralAmmo[ Player ] == 0.0F ) return( false );
						Host_GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						Host_GeneralAmmo[ Player ] -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( Host_GeneralAmmo[ Player ] < 0.0F ) Host_GeneralAmmo[ Player ] = 0.0F;
					}
					break;

				case TROJAX:
				case NME_TROJAX:
					break;

				case PYROLITE_RIFLE:
				case NME_PYROLITE:
					if( Ships[ Player ].SuperNashramTimer == 0.0F )
					{
						if( Host_PyroliteAmmo[ Player ] == 0.0F ) return( false );
						Host_PyroliteAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						Host_PyroliteAmmo[ Player ] -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( Host_PyroliteAmmo[ Player ] < 0.0F ) Host_PyroliteAmmo[ Player ] = 0.0F;
					}
					break;

				case SUSS_GUN:
				case NME_SUSS_GUN:
					if( Ships[ Player ].SuperNashramTimer == 0.0F )
					{
						if( Host_SussGunAmmo[ Player ] == 0.0F ) return( false );
						Host_SussGunAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						Host_SussGunAmmo[ Player ] -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( Host_SussGunAmmo[ Player ] < 0.0F ) Host_SussGunAmmo[ Player ] = 0.0F;
					}
					break;

				case ORBITPULSAR:
					break;

				default:
					break;
			}
			break;

		case MSG_SECBULLPOSDIR:
   			lpSecBullPosDir = (LPSECBULLPOSDIRMSG)MsgPnt;

			if( lpSecBullPosDir->SecBullPosDir.OwnerType != OWNER_SHIP ) break;

			Weapon		= lpSecBullPosDir->SecBullPosDir.Weapon;
			TempWeapon	= ConvSecToNormWeapon( Weapon );

//			AddMessageToQue( "SecBullPosDir : %s, Got = %d, Ammo = %d", SecondaryDescription[ TempWeapon ],
//																		Host_SecondaryWeaponsGot[ Player ][ TempWeapon ],
//																		Host_SecondaryAmmo[ Player ][ TempWeapon ] );

			switch( Weapon )
			{
				case MUGMISSILE:
				case SOLARISMISSILE:
				case THIEFMISSILE:
				case SCATTERMISSILE:
				case GRAVGONMISSILE:
				case MULTIPLEMISSILE:
				case TITANSTARMISSILE:
				case PINEMISSILE:
				case TITANSTARSHRAPNEL:
				case ENEMYSPIRALMISSILE:
				case ENEMYHOMINGMISSILE:
				case ENEMYBLUEHOMINGMISSILE:
				case ENEMYFIREBALL:
				case ENEMYTENTACLE:
				case ENEMYDEPTHCHARGE:
					break;

				case PURGEMINE:
				case PINEMINE:
				case QUANTUMMINE:
				case SPIDERMINE:
					if( !Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] ) return( false );
					if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) return( false );

					Host_SecondaryAmmo[ Player ][ TempWeapon ]--;
					if( !Host_SecondaryAmmo[ Player ][ TempWeapon ] ) Host_SecondaryWeaponsGot[ Player ][ TempWeapon ] = 0;
					break;

				default:
					break;
			}
			break;

		case MSG_TITANBITS:
			break;

		default:
			break;
	}

	return( true );
}


