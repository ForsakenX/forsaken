
//
// Networking functions mainly used by title.c
//

#include <stdio.h>

#include "main.h"
#include "net.h"
#include "render.h"
#include "title.h"

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "multiplayer.h"
#include "mxload.h"
#include "mxaload.h"
#include <stdio.h>
#include "local.h"
#include "text.h"
#include "ships.h"
#include "loadsave.h"
#include "pickups.h"
#include "primary.h"
#include "controls.h"
#include "util.h"
#include "demo.h"
#include "string.h"
#include "util.h"
#include "net_tracker.h"
#include "oct2.h"

/*
 * Externals
 */

extern int OldPPSValue;
extern int OldColPerspective;
extern int OldUseShortPackets;

bool ResetKillsPerLevel;
bool TintBikeTeamColor;
bool MyResetKillsPerLevel;
bool IpOnCLI = false;

extern u_int8_t QuickStart;
extern bool BrightShips;
extern bool BikeExhausts;
extern int32_t ColPerspective;

extern bool NoMenuBack;
extern bool Debug;

extern bool NeedFlagAtHome;
extern bool OwnFlagTeleportsHome;
extern bool CanCarryOwnFlag;
extern SLIDER CTFSlider;

extern	bool	UseShortPackets;
extern bool	Panel;

extern MENUITEM TeamGameHostMenuItem;
extern BYTE	PreSynchupStatus;
extern bool DemoShipInit[];
extern	float NetUpdateInterval;
extern	bool	BrightShips;
extern	bool	MyBrightShips;

extern float Pulse;
extern char *EmptyString;

extern	bool					IsHost;   // is the user hosting/joining a game
extern	int16_t					Lives;

extern	bool	BountyHunt;
extern	bool	CaptureTheFlag;
extern	bool	CTF;
extern	TEXT	DemoGameName;
extern	bool	RecordDemo;
extern	LIST	PlayersList;
extern	LIST	LevelList;
extern	LIST	TeamList[MAX_TEAMS];
extern	MENUITEM	TeamItem;
extern	MENUITEM	NewTeamItem;
extern	bool	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	SLIDER	TimeLimit;
extern	SLIDER	MyTimeLimit;
extern	SLIDER	DemoSpeed;
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	MaxKillsSlider;
extern	int16_t	MaxKills;
extern	char	biker_name[256];
extern	float	framelag;
extern char MyName[];
extern char NickName[];
extern	BYTE                    WhoIAm;
extern	int16_t					Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16_t					StatsCount;
extern	int16_t	SelectedBike;
extern	SLIDER	NumOfPlayersSlider;
extern	int MenuStackLevel;
extern	BYTE	GameStatus[];	// Game Status for every Ship...
extern	SLIDER  PacketsSlider;
extern	TEXT TCPAddress;
extern	bool	CountDownOn;
extern	float	GetPlayerNumCount1;
extern	float	GetPlayerNumCount2;
extern	int		GetPlayerNumCount;
extern	FILE	*	DemoFp;
extern	FILE	*	DemoFpClean;
bool ChangeLevel( void );
bool InitLevels( char *levels_list );
extern	int16_t		LevelNum;

#ifdef DEMO_SUPPORT
extern	bool	PlayDemo;
extern	LONGLONG	GameElapsedTime;		// when the game started
extern	LIST	DemoList;
extern	float Demoframelag;
extern	LONGLONG	DemoTimeSoFar;
#endif

extern	MENUITEM	JoinItem;
extern	MENUITEM	NewJoinItem;
extern  MENUITEM	WatchTeamSelectionItem;
extern int CameraStatus;
extern	BYTE					OverallGameStatus;
extern char *CurrentLevelsList;
extern	bool AutoSelectConnection;
extern  MENUSTATE MenuState;
extern  char TeamCurrentScore[MAX_TEAMS][64];
#ifdef DEMO_SUPPORT
extern	LONGLONG	DemoStartedTime;		// when the game started
extern	LONGLONG	DemoEndedTime;		// when the game started
extern	int32_t		DemoGameLoops;
#endif
extern	int		GameType;
extern	u_int16_t		Seed1;
extern	u_int16_t		Seed2;
extern	u_int16_t		CopyOfSeed1;
extern	u_int16_t		CopyOfSeed2;
extern	bool		RandomPickups;

/*
 * Globals to this module
 */

int						TeamMembers[MAX_TEAMS];

MENU  *				GetPlayerNumMenu;
float	Bodge	= 1.0F;

bool	Modem2Modem = false;

u_int16_t	RandomStartPosModify = 0;							

void DrawFlatMenuItem( MENUITEM *Item );
void GetLevelName( char *buf, int bufsize, int level );
void InitDemoList( MENU * Menu );
void RestoreDemoSettings( void );

void SetUpGameSubType( int type )
{
	switch ( CTFSlider.value )
	{
	case CTF_MUST_CARRY:
		NeedFlagAtHome			= true;
		OwnFlagTeleportsHome	= false;
		CanCarryOwnFlag			= true;
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
	case CTF_NO_RETURN:
		NeedFlagAtHome			= false;
		OwnFlagTeleportsHome	= false;
		CanCarryOwnFlag			= false;
		break;
	case CTF_STANDARD:
	default:
		NeedFlagAtHome			= true;
		OwnFlagTeleportsHome	= true;
		CanCarryOwnFlag			= false;
	}
}

void SetUpGameType( int type ) 
{
	TeamGame = false;
	CTF = false;
	NeedFlagAtHome			= false;
	OwnFlagTeleportsHome	= false;
	CanCarryOwnFlag			= false;
	CaptureTheFlag = false;
	BountyHunt = false;
	
	switch ( type )
	{
	case GAME_Normal:
		break;
	case GAME_Team:
		TeamGame = true;
		break;
	case GAME_CTF:
		CTF = true;
		TeamGame = true;

		SetUpGameSubType( CTFSlider.value );

		break;
	case GAME_FlagChase:
		CaptureTheFlag = true;
		TeamGame = true;
		break;
	case GAME_BountyHunt:
		BountyHunt = true;
		break;
	case GAME_TeamBountyHunt:
		BountyHunt = true;
		TeamGame = true;
		break;
	}
}


/*===================================================================
	Procedure	:	Hosting a session...
	Input		:	nothing
	Output		:	nothing
===================================================================*/
extern void SetGamePrefs( void );
extern TEXT local_port_str;
extern bool	PickupValid[ MAXPICKUPTYPES ];
extern bool	MyPickupValid[ MAXPICKUPTYPES ];
extern SLIDER MyPacketsSlider;
extern int32_t MyColPerspective;
extern bool MyRandomPickups;
extern bool MyUseShortPackets;

// these settings get over ridden when you join a game
// so we need a separate copy of them to backup our settings
void copy_in_my_settings( void )
{
	memset( PickupValid, 0, sizeof(PickupValid) );
	memmove( PickupValid, MyPickupValid, sizeof(PickupValid) );//memcpy

	TimeLimit.value		= MyTimeLimit.value;
	PacketsSlider.value	= MyPacketsSlider.value;

	ColPerspective		= MyColPerspective;
	ResetKillsPerLevel	= MyResetKillsPerLevel;
	RandomPickups		= MyRandomPickups;
	UseShortPackets		= MyUseShortPackets;
}

extern MENU MENU_NEW_CreateGame;
extern  BYTE          MyGameStatus;
void StartAHostSession ( MENUITEM * Item )
{
	int i;
	u_int32_t	ms = SDL_GetTicks();
	u_int32_t	Seed;
	network_return_t rv;

	SetGamePrefs();

	copy_in_my_settings();

	Seed = SDL_GetTicks();
	Seed1 = (u_int16_t) ( ( Seed >> 16 ) & 0xffff );
	Seed2 = (u_int16_t) ( Seed & 0xffff );
	CopyOfSeed1 = Seed1;
	CopyOfSeed2 = Seed2;

	
#ifdef DEMO_SUPPORT
	PlayDemo = false;
#endif

	IsHost = true;

	TeamGame = false;
	CaptureTheFlag = false;
	BountyHunt = false;
	CTF = false;

	SetUpGameType( GameType );

	// set bike mode to normal
	SetBikeMods( 0 );

	MaxKills = MaxKillsSlider.value;

	RandomStartPosModify = (u_int16_t) ( ms * 71.42857143 );

	local_port = atoi(local_port_str.text);
	SetGamePrefs();

	rv = network_setup( &biker_name[0], local_port );

	if( rv != NETWORK_OK )
	{
		char error_str[600] = "Failed to setup network!";
		switch( rv )
		{
		case NETWORK_ERROR_INIT:
			break;
		case NETWORK_ERROR_BIND:
			{
				char * str = "";
				if( local_port < 1024 )
					str = "Ports bellow 1024 are normally restricted.";
				sprintf(error_str, "%s %s %s %s",
						"The selected port is in use or invalid.", 
						str,
						"Disable the program using the port or",
						"select a different local port under network options." );
			}
			break;
		}
		PrintErrorMessage(error_str, 2, &MENU_NEW_CreateGame, 0);
		return;
	}

	network_host();

	NetUpdateInterval	= 60.0F / PacketsSlider.value;
	OldPPSValue = PacketsSlider.value;
	OldColPerspective = ColPerspective;
	OldUseShortPackets = UseShortPackets;

	SetupNetworkGame();

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].network_player = NULL;

	if ( TeamGame )
		MenuChange( &NewTeamItem );
	else
		MenuChange( Item );

	MyGameStatus = STATUS_StartingMultiplayer;
	
	Current_Camera_View = 0;				// set camera to that view
//	Ships[WhoIAm].enable = 1;
	Ships[WhoIAm].Pickups = 0;
	Ships[WhoIAm].RegenSlots = 0;
	Ships[WhoIAm].Mines = 0;
	Ships[WhoIAm].Triggers = 0;
	Ships[WhoIAm].TrigVars = 0;

	set_my_player_name();

	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	NewLevelNum = LevelList.selected_item;	// I Select Which Level We Start on...

	if( TimeLimit.value )
	{
		CountDownOn = true;
	}
	else
	{
		CountDownOn = false;
	}


#ifdef DEMO_SUPPORT
	if( RecordDemo )
	{
		u_int32_t mp_version = MULTIPLAYER_VERSION;
		u_int32_t flags;
		time_t now_time;
		struct tm *now;

		RecordDemo = true;
		time( &now_time );
		now = localtime( &now_time );
#ifndef HOST_CHOOSES_DEMO_NAME
		if ( now )
		{
			sprintf( DemoGameName.text, "%s's Demo %d.%02d %d-%d-%d",
				biker_name,
				now->tm_hour, now->tm_min,
				1 + now->tm_mon, now->tm_mday,
				1900 + now->tm_year );
		}
		else
		{
			sprintf( DemoGameName.text, "%s's Demo",
				biker_name );
		}
#endif
		DemoFp = file_open( DemoFileName( DemoGameName.text ) , "wb" );
		setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...

		Demo_fwrite( &mp_version, sizeof( mp_version ), 1, DemoFp );
		flags = 0;
		if( TeamGame )
			flags |= TeamGameBit;
		if( CTF )
			flags |= CTFGameBit;
		if( CaptureTheFlag )
			flags |= FlagGameBit;
		if ( BountyHunt )
			flags |= BountyGameBit;

		Demo_fwrite( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
		Demo_fwrite( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
		Demo_fwrite( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
		PackPickupInfo( &PackedInfo[ 0 ] );
		Demo_fwrite( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );

		Demo_fwrite( &flags, sizeof( flags ), 1, DemoFp );
		Demo_fwrite( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );
		for( i = 0 ; i < 256 ; i++ )
		{
			Demo_fwrite( &ShortLevelNames[NewLevelNum][i], sizeof(char), 1, DemoFp );
			if( ShortLevelNames[NewLevelNum][i] == 0 )
			{
				break;
			}
		}

		// Best way I can Think of to send the Host Name to the demo file...
		Temp = 1;
		ms = 1;
		Demo_fwrite( &ms, sizeof(LONGLONG), 1, DemoFp );
		size = sizeof( NAMEMSG );
		Demo_fwrite( &size, sizeof(int), 1, DemoFp );
		Demo_fwrite( &Temp, sizeof(DWORD), 1, DemoFp );
		msg = MSG_NAME;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// Whos Name it is..
		Demo_fwrite( &biker_name[0], 7, 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// terminator for name..

	}
#endif
	
	BrightShips = MyBrightShips;

	return;
}

/*===================================================================
	Procedure	:	Get The Players In the Current Session...
	Input		:	MENUITEM * Item
	Output		:	nothing
===================================================================*/
void GetPlayersInCurrentSession( MENUITEM *Item )
{
	int i;

	PlayersList.items = 0;
	PlayersList.top_item = 0;
	PlayersList.selected_item = -1;
	
	NumOfPlayersSlider.value = 0;

	for ( i = 0; i < MAX_PLAYERS; i++ )
		if( ( GameStatus[ i ] == MyGameStatus ) || ( i == WhoIAm ) )
		{
			NumOfPlayersSlider.value++;
			strcpy( PlayersList.item[ PlayersList.items++ ], Names[ i ] );
		}

	if (NumOfPlayersSlider.value != NumOfPlayersSlider.oldvalue)
		NumOfPlayersSlider.redraw_req = true;
}

/*===================================================================
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
===================================================================*/
void GoToSynchup ( MENUITEM * Item )
{

	if( TeamGame )
	{
		if (CameraStatus == CAMERA_AtStart)
		{
			MenuChange( &TeamItem );
		}else
		{
			MenuChange( &NewTeamItem );
		}
	}else{

		if (CameraStatus != CAMERA_AtStart)
		{	
			LastMenu = CurrentMenu;
			VduClear();
		}

		CurrentMenu = NULL;
		CurrentMenuItem = NULL;
		MenuStackLevel = 0;
	}

}
/*===================================================================
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
===================================================================*/
void TeamGoToSynchup ( MENUITEM * Item )
{
	PreSynchupStatus = MyGameStatus;
	MyGameStatus = STATUS_WaitingToStartTeamGame;
	MenuChangeEx( &MENU_NEW_GeneralLoading );
	MenuDraw( CurrentMenu );	// because menu processing will not be done when MyGameStatus changes

}


/*===================================================================
	Procedure	:	Bail out of a multiplayer game before it starts...
	Input		:	MENU * Menu
	Output		:	nothing
===================================================================*/
void BailMultiplayer( MENU * Menu )
{
	MyGameStatus = STATUS_Left;
    if ( WhoIAm < MAX_PLAYERS )
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
	if(IsHost)
		send_tracker_finished( tracker_server, tracker_port );
	network_cleanup();
	MenuRestart( &MENU_Start );
	MyGameStatus = STATUS_Title;
}

void BailMultiplayerFrontEnd( MENU *Menu )
{
	BailMultiplayer( Menu );
}

void InitTeamLists( MENU *Menu )
{
	int i;

	TeamList[0] = PlayersList;
	TeamList[0].selected_item = -1;
	TeamList[0].display_items = 12;
	TeamMembers[0] = PlayersList.items;
	
	for (i = 1; i < MAX_TEAMS; i++)
	{
		TeamList[i].items = 0;
		TeamList[i].top_item = 0;
		TeamList[i].display_items = 12;			// hard wired in because MAX_PLAYERS will increase
		TeamList[i].selected_item = -1;

		TeamMembers[i] = 0;
	}

	for ( i = 0; i < MAX_PLAYERS; i++ )
		PlayerReady[ i ] = false;
}

void InitExistingGameJoin( MENU *Menu )
{
	int i;
	
	InitTeamLists( NULL );

	for (i = 0; i < MAX_TEAMS; i++)
	{
		strcpy(TeamCurrentScore[i], ": 0");
	}

	InitTitleMessaging();
}

/*===================================================================
	Procedure	:	Set the Team player lists..
	Input		:	MENU * Menu
	Output		:	nothing
===================================================================*/
void InitTeamSelection( MENU *Menu )
{
	int i;
	MENUITEM *item;

	InitTeamLists( NULL );

	for( item = Menu->Item; item->x >=0; item++ )
	{
		if ( item->FuncDraw == DrawReadyButton )
			item->FuncSelect = PlayerConfirm;

		if 	( ( item->Variable == &TeamList[0] ) ||
		    ( item->Variable == &TeamList[1] ) ||
		    ( item->Variable == &TeamList[2] ) ||
		    ( item->Variable == &TeamList[3] ) ) 
		{
			item->FuncSelect = SelectTeamList;
		}
	}

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
		if( IsHost )
			TeamNumber[i] = 255;

//	for (i = 0; i < TeamList[0].items; i++)
//		TeamNumber[i] = 0;

	TeamNumber[WhoIAm] = 0;		// start myself off in team 0

	// if we went into this menu automatically (ie. we are not the host), Menustate
	// will still be in whatever state it was in in the last menu, so reset...
	MenuState = MENUSTATE_Select;

	InitTitleMessaging();
}

void CheckForMenuChange(int *dummy)
{
	if (TeamNumber[0] != 255)
		MenuChange( &WatchTeamSelectionItem );

	GetPlayersInCurrentSession ( NULL );
}

void DistributeTeamsToLists(int *dummy)
{
	int team, player, num_players;
	bool OkayToStart = true;
	int16_t TeamScore[MAX_TEAMS];

	UpdateSessions( NULL );
	memset(&TeamScore, 0, sizeof(int16_t) * MAX_TEAMS);

	for (team = 0; team < MAX_TEAMS; team++)
	{
		TeamList[team].items = 0;
		TeamList[team].top_item = 0;
		TeamList[team].display_items = 12;
	}

	num_players = 0;
	
	for ( player = 0; player < MAX_PLAYERS; player++ )	// server dosn't count as player
	{
		// player has left, therefore reset their team number...
		if ((GameStatus[player] == STATUS_Left) || (GameStatus[player] == STATUS_LeftCrashed))
		{
			TeamNumber[player] = 255;
		}

		team = TeamNumber[player];
		if (team < MAX_TEAMS)
		{
			if ( ( player == WhoIAm ) || ( GameStatus[ player ] == STATUS_StartingMultiplayer ) || ( GameStatus[ player ] == STATUS_Normal ) )
			{
				num_players++;
				
				if ( !PlayerReady[player] && ( GameStatus[ player ] != STATUS_Normal ) )
				{
					if (Pulse >= 0.5F)			 
					{
						TeamList[team].item[TeamList[team].items++][0] = 0;
					}
					else
					{
						strcpy(&TeamList[team].item[TeamList[team].items++][0], &Names[player][0]);
					}

					OkayToStart = false;
				}else
				{
					strcpy(&TeamList[team].item[TeamList[team].items++][0], &Names[player][0]);
				}
			}
		}
	}

	if ( IsHost && OkayToStart && num_players )
	{
		TeamGoToSynchup(NULL);
	}

	for (team = 0; team < MAX_TEAMS; team++)
	{
		sprintf(TeamCurrentScore[team], ": %d",TeamScore[team]);
	}
}
