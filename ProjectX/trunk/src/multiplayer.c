
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
#include "winsock.h"
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
#include "xmem.h"
#include "util.h"
#include "demo.h"
#include "string.h"
#include "util.h"
#include "net_tracker.h"

/*
 * Externals
 */

extern int OldPPSValue;
extern int OldColPerspective;
extern int OldUseShortPackets;

BOOL ResetKillsPerLevel;
BOOL MyResetKillsPerLevel;
BOOL IpOnCLI = FALSE;

extern uint8 QuickStart;
extern BOOL BrightShips;
extern BOOL BikeExhausts;
extern int32 ColPerspective;

extern BOOL NoMenuBack;

extern void DebugLastError( void );
extern BOOL Debug;

extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;
extern SLIDER CTFSlider;

extern	BOOL	UseShortPackets;
extern BOOL	Panel;

extern MENUITEM TeamGameHostMenuItem;
extern BYTE	PreSynchupStatus;
extern BOOL DemoShipInit[];
extern	float NetUpdateInterval;
extern	BOOL	BrightShips;
extern	BOOL	MyBrightShips;

extern float Pulse;
extern char *EmptyString;

extern	BOOL					IsHost;   // is the user hosting/joining a game
extern	int16					Lives;

extern	BOOL	BountyHunt;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	TEXT	DemoGameName;
extern	BOOL	RecordDemo;
extern	LIST	PlayersList;
extern	LIST	LevelList;
extern	LIST	TeamList[MAX_TEAMS];
extern	MENUITEM	TeamItem;
extern	MENUITEM	NewTeamItem;
extern	BOOL	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	SLIDER	TimeLimit;
extern	SLIDER	MyTimeLimit;
extern	SLIDER	DemoSpeed;
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	MaxKillsSlider;
extern	int16	MaxKills;
extern	char	biker_name[256];
extern	float	framelag;
extern char MyName[];
extern char NickName[];
extern	BYTE					Current_Camera_View;		// which object is currently using the camera view....
extern	BYTE                    WhoIAm;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	int16					Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16					StatsCount;
extern	SHORTNAMETYPE			Names;	// all the players short Names....
extern	int16	SelectedBike;
extern	SLIDER	NumOfPlayersSlider;
extern	int MenuStackLevel;
extern	BYTE	GameStatus[];	// Game Status for every Ship...
extern	SLIDER  PacketsSlider;
extern	TEXT TCPAddress;
extern	BOOL	CountDownOn;
extern	float	GetPlayerNumCount1;
extern	float	GetPlayerNumCount2;
extern	int		GetPlayerNumCount;
extern	FILE	*	DemoFp;
extern	FILE	*	DemoFpClean;
BOOL ChangeLevel( void );
BOOL InitLevels( char *levels_list );
extern	int16		LevelNum;
extern	int16		NewLevelNum;

#ifdef DEMO_SUPPORT
extern	BOOL	PlayDemo;
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
extern	BOOL AutoSelectConnection;
extern	LONGLONG	Freq;
extern  MENUSTATE MenuState;
extern  char TeamCurrentScore[MAX_TEAMS][64];
#ifdef DEMO_SUPPORT
extern	LONGLONG	DemoStartedTime;		// when the game started
extern	LONGLONG	DemoEndedTime;		// when the game started
extern	int32		DemoGameLoops;
#endif
extern	int		GameType;
extern	uint16		Seed1;
extern	uint16		Seed2;
extern	uint16		CopyOfSeed1;
extern	uint16		CopyOfSeed2;
extern	BOOL		RandomPickups;
extern	BOOL	NoSFX;

/*
 * Globals to this module
 */

int						TeamMembers[MAX_TEAMS];

MENU  *				GetPlayerNumMenu;
float	Bodge	= 1.0F;

BOOL	Modem2Modem = FALSE;

uint16	RandomStartPosModify = 0;							

void DrawFlatMenuItem( MENUITEM *Item );
void GetLevelName( char *buf, int bufsize, int level );
void InitDemoList( MENU * Menu );
void RestoreDemoSettings( void );

void SetUpGameSubType( int type )
{
	switch ( CTFSlider.value )
	{
	case CTF_MUST_CARRY:
		NeedFlagAtHome			= TRUE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= TRUE;
		break;
	case CTF_CAN_CARRY:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= TRUE;
		break;
	case CTF_ONE_TOUCH:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= TRUE;
		CanCarryOwnFlag			= FALSE;
		break;
	case CTF_NO_RETURN:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= FALSE;
		break;
	case CTF_STANDARD:
	default:
		NeedFlagAtHome			= TRUE;
		OwnFlagTeleportsHome	= TRUE;
		CanCarryOwnFlag			= FALSE;
	}
}

void SetUpGameType( int type ) 
{
	TeamGame = FALSE;
	CTF = FALSE;
	NeedFlagAtHome			= FALSE;
	OwnFlagTeleportsHome	= FALSE;
	CanCarryOwnFlag			= FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	
	switch ( type )
	{
	case GAME_Normal:
		break;
	case GAME_Team:
		TeamGame = TRUE;
		break;
	case GAME_CTF:
		CTF = TRUE;
		TeamGame = TRUE;

		SetUpGameSubType( CTFSlider.value );

		break;
	case GAME_FlagChase:
		CaptureTheFlag = TRUE;
		TeamGame = TRUE;
		break;
	case GAME_BountyHunt:
		BountyHunt = TRUE;
		break;
	case GAME_TeamBountyHunt:
		BountyHunt = TRUE;
		TeamGame = TRUE;
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
extern BOOL	PickupValid[ MAXPICKUPTYPES ];
extern BOOL	MyPickupValid[ MAXPICKUPTYPES ];
extern SLIDER MyPacketsSlider;
extern int32 MyColPerspective;
extern BOOL MyRandomPickups;
extern BOOL MyUseShortPackets;

// these settings get over ridden when you join a game
// so we need a separate copy of them to backup our settings
void copy_in_my_settings( void )
{
	memset( PickupValid, 0, sizeof(PickupValid) );
	memcpy( PickupValid, MyPickupValid, sizeof(PickupValid) );

	TimeLimit.value		= MyTimeLimit.value;
	PacketsSlider.value	= MyPacketsSlider.value;

	ColPerspective		= MyColPerspective;
	ResetKillsPerLevel	= MyResetKillsPerLevel;
	RandomPickups		= MyRandomPickups;
	UseShortPackets		= MyUseShortPackets;
}

extern MENU MENU_NEW_CreateGame;
extern  BYTE          MyGameStatus;
BOOL StartAHostSession ( MENUITEM * Item )
{
	int i;
	Uint32	ms = SDL_GetTicks();
	uint32	Seed;
	network_return_t rv;

	SetGamePrefs();

	copy_in_my_settings();

	Seed = timeGetTime();
	Seed1 = (uint16) ( ( Seed >> 16 ) & 0xffff );
	Seed2 = (uint16) ( Seed & 0xffff );
	CopyOfSeed1 = Seed1;
	CopyOfSeed2 = Seed2;

	
#ifdef DEMO_SUPPORT
	PlayDemo = FALSE;
#endif

	IsHost = TRUE;

	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	CTF = FALSE;

	SetUpGameType( GameType );

	// set bike mode to normal
	SetBikeMods( 0 );

	MaxKills = MaxKillsSlider.value;

	RandomStartPosModify = (uint16) ( ms * 71.42857143 );

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
		return TRUE;
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
		CountDownOn = TRUE;
	}
	else
	{
		CountDownOn = FALSE;
	}


#ifdef DEMO_SUPPORT
	if( RecordDemo )
	{
		uint32 mp_version = MULTIPLAYER_VERSION;
		uint32 flags;
		time_t now_time;
		struct tm *now;

		RecordDemo = TRUE;
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
		DemoFp = fopen( DemoFileName( DemoGameName.text ) , "wb" );
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

	return TRUE;
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
		NumOfPlayersSlider.redraw_req = TRUE;
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
		PlayerReady[ i ] = FALSE;
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
	BOOL OkayToStart = TRUE;
	int16 TeamScore[MAX_TEAMS];

	UpdateSessions( NULL );
	memset(&TeamScore, 0, sizeof(int16) * MAX_TEAMS);

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

					OkayToStart = FALSE;
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
