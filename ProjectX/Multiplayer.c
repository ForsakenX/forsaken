
//
// Networking functions mainly used by title.c
//

#include <stdio.h>
#include "windows.h"
#include "typedefs.h"
#include "main.h"
#include "net_dplay.h"
#include "d3dappi.h"
#include "title.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "winsock.h"
#include "MultiPlayer.h"
#include "mxload.h"
#include "mxaload.h"
#include <stdio.h>
#include "Local.h"
#include "text.h"
#include "ships.h"
#include "LoadSave.h"
#include "pickups.h"
#include "primary.h"
#include "controls.h"
#include "xmem.h"
#include "util.h"
#include "demo.h"
#include "string.h"
#include "net_dplay.h"

/*
 * Externals
 */
extern BOOL OKToProcessKeys;
extern int OldPPSValue;
extern int OldColPerspective;
extern int OldUseShortPackets;

BOOL ResetKillsPerLevel = FALSE;
BOOL bTCP = FALSE;

extern uint8 QuickStart;
extern BOOL BrightShips;
extern BOOL BikeExhausts;
extern int32 ColPerspective;

// registry.c
extern LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);
extern LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
extern LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);

extern BOOL NoMenuBack;

extern void DebugLastError( void );
extern BOOL Debug;

extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;
extern SLIDER CTFSlider;

extern	BOOL	UseShortPackets;

extern void SetMultiplayerPrefs( void );
extern BOOL	Panel;

extern MENUITEM TeamGameHostMenuItem;
extern BYTE	PreSynchupStatus;
extern int CurrentLoadingStep;
extern BOOL DemoShipInit[];
extern	float DPlayUpdateInterval;
extern	BOOL	BrightShips;
extern	BOOL	MyBrightShips;

extern float Pulse;
extern char *EmptyString;

extern	BOOL					IsHost;   // is the user hosting/joining a game
extern	network_id_t				dcoID;    // player id
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
extern	SLIDER	DemoSpeed;
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	MaxKillsSlider;
extern	int16	MaxKills;

extern	TEXT	MultiPlayerGameName;
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
extern	BOOL	PlayDemo;
extern	LONGLONG	GameStartedTime;		// when the game started
extern	LONGLONG	GameElapsedTime;		// when the game started
extern	LONGLONG	TempGameElapsedTime;		// when the game started
extern	LIST	DemoList;
extern	float Demoframelag;
extern	LONGLONG	DemoTimeSoFar;
extern	BOOL	AutoDetail;
void SetOurRenderStates( MENUITEM *item );
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

extern	LONGLONG	DemoStartedTime;		// when the game started
extern	LONGLONG	DemoEndedTime;		// when the game started
extern	int32		DemoGameLoops;

extern BOOL PreventFlips;
extern	int		GameType;
extern	uint16		Seed1;
extern	uint16		Seed2;
extern	uint16		CopyOfSeed1;
extern	uint16		CopyOfSeed2;
extern	BOOL		RandomPickups;
extern	BOOL	PolygonText;
extern	BOOL	NoSFX;

/*
 * Globals to this module
 */

network_id_t					PlayerIDs[MAX_PLAYERS];
network_id_t					TeamIDs[MAX_TEAMS][MAX_PLAYERS];
int						TeamMembers[MAX_TEAMS];

MENU  *				GetPlayerNumMenu;
float	Bodge	= 1.0F;

BOOL	Modem2Modem = FALSE;

uint16	RandomStartPosModify = 0;							

void DebugPrintf( const char * format, ... );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );
void DrawFlatMenuItem( MENUITEM *Item );
void GetLevelName( char *buf, int bufsize, int level );
void InitDemoList( MENU * Menu );
void RestoreDemoSettings( void );
void GetMultiplayerPrefs( void );

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
	case GAME_CaptureFlag:
		CaptureTheFlag = TRUE;
		TeamGame = TRUE;
		break;
	case GAME_BountyHunt:
		BountyHunt = TRUE;
		break;
	case GAME_TeamBounty:
		BountyHunt = TRUE;
		TeamGame = TRUE;
		break;
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Hosting a session...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StartAHostSession ( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;
	uint32		Seed;

	network_initialize( NULL );

	SetMultiplayerPrefs();

	Seed = timeGetTime();
	Seed1 = (uint16) ( ( Seed >> 16 ) & 0xffff );
	Seed2 = (uint16) ( Seed & 0xffff );
	CopyOfSeed1 = Seed1;
	CopyOfSeed2 = Seed2;

	PlayDemo = FALSE;
	IsHost = TRUE;

	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	CTF = FALSE;

	DebugPrintf("Setting up game type.\n");
	SetUpGameType( GameType );

	DebugPrintf("Setting up bike modes.\n");
	SetBikeMods( 0 );

	MaxKills = MaxKillsSlider.value;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = (uint16) ( ( TempTime * 71.42857143 ) / Freq );

	DebugPrintf("d3d FlipToGDISurface.\n");
	d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);

	// create session
	if( ! network_host( &MultiPlayerGameName.text[0]) )
	{
		Msg("Failed to create Direct Play Session!");
		return FALSE;
	}

	// create player
	if( ! network_create_player( &dcoID, &biker_name[0] ) )
	{
		Msg("Failed to create Direct Play Player!");
	    return FALSE;
	}

	DPlayUpdateInterval	= 60.0F / PacketsSlider.value;
	OldPPSValue = PacketsSlider.value;
	OldColPerspective = ColPerspective;
	OldUseShortPackets = UseShortPackets;

	SetupDplayGame();

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].dcoID = dcoID;

	DebugPrintf("MenuChange.\n");
	if ( TeamGame )
		MenuChange( &NewTeamItem );
	else
		MenuChange( Item );

	DebugPrintf("Other Bullshit.\n");

	MyGameStatus = STATUS_StartingMultiplayer;
	
	Current_Camera_View = 0;				// set camera to that view
//	Ships[WhoIAm].enable = 1;
	Ships[WhoIAm].Pickups = 0;
	Ships[WhoIAm].RegenSlots = 0;
	Ships[WhoIAm].Mines = 0;
	Ships[WhoIAm].Triggers = 0;
	Ships[WhoIAm].TrigVars = 0;
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;
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
		TempTime = 1;
		Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
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

	DebugPrintf("Done.\n");

	return TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Join a Session...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL JoinASession ( MENUITEM * Item )
{
	PlayDemo = FALSE;

	SetBikeMods( 0 );

	// open session
	if( ! network_join() )
	{
		PrintErrorMessage ( COULDNT_OPEN_SESSION, 1, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	// create player
	if( ! network_create_player(&dcoID, &biker_name[0]) )
	{
		PrintErrorMessage ( COULDNT_CREATE_PLAYER, 1, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	SetupDplayGame();
	
	WhoIAm = 0xff;
	MyGameStatus = STATUS_GetPlayerNum;
	GetPlayerNumCount1 = 0.0F;
	GetPlayerNumCount2 = 60.0F * 30.0F;	// 30 Seconds...
	GetPlayerNumCount = 0;

	if ( TeamGame )
		GetPlayerNumMenu = &MENU_NEW_WatchTeamSelect;
	else
		GetPlayerNumMenu = (MENU*) Item->Value;

	// Stops us going straight in to the game....
	OverallGameStatus = STATUS_Null;

	return TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Select a Session and show players in it..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SelectSession( MENUITEM *Item )
{
	if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
	{
		JoinASession (&NewJoinItem);
	}
	else
	{
		JoinASession ( &JoinItem );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get The Players In the Current Session...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TeamGoToSynchup ( MENUITEM * Item )
{
	/*
	// put there team numbers backin..
	//ReAssembleTeamNumbers();
	LastMenu = CurrentMenu;
	VduClear();
	MenuAbort();
	*/
	PreSynchupStatus = MyGameStatus;
	MyGameStatus = STATUS_WaitingToStartTeamGame;
	MenuChangeEx( &MENU_NEW_GeneralLoading );
	MenuDraw( CurrentMenu );	// because menu processing will not be done when MyGameStatus changes

}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Bail out of a multiplayer game before it starts...
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void BailMultiplayer( MENU * Menu )
{
	MyGameStatus = STATUS_Left;
    if ( ( dcoID != 0 ) && ( WhoIAm < MAX_PLAYERS ) )
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set the Team player lists..
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitTeamSelection( MENU *Menu )
{
	int i, j;
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
	{
		TeamIDs[0][i] = PlayerIDs[i];

		for (j = 1; j < MAX_TEAMS; j++) 
		{
			TeamIDs[j][i] = 0;
		}
		if( IsHost )
			TeamNumber[i] = 255;
	}

//	for (i = 0; i < TeamList[0].items; i++)
//		TeamNumber[i] = 0;

	TeamNumber[WhoIAm] = 0;		// start myself off in team 0

	// if we went into this menu automatically (ie. we are not the host), Menustate
	// will still be in whatever state it was in in the last menu, so reset...
	MenuState = MENUSTATE_Select;

	InitTitleMessaging();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Swap a list item from 1 list to another....
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SwapListItem( LIST * Source , LIST * Dest )
{
	int i;
	char TempItem[32];
	LIST	TempList;

	if( ( Source->selected_item != -1) && ( Source->items != 0 ) )
	{
		TempList.items = 0;
		TempList.top_item = 0;
		TempList.display_items = MAX_PLAYERS;
		TempList.selected_item = -1;
	

		strcpy( &TempItem[0] , Source->item[Source->selected_item] );
		
		for( i = 0 ; i < Source->items ; i++ )
		{
			if( i != Source->selected_item )
			{
				strcpy( &TempList.item[TempList.items][0] , Source->item[i]);
				TempList.items++;
			}
		}
		for( i = 0 ; i < TempList.items ; i++ )
		{
			strcpy( Source->item[i] , &TempList.item[i][0]);
		}
		Source->items = TempList.items;
		Source->top_item = 0;
		Source->display_items = MAX_PLAYERS;
		Source->selected_item = -1;

		//strcpy( Dest->item[Dest->items] , &TempItem[0]);
		//Dest->items++;

		// *** changed to put new item at top of list, so it is always visible ***

		TempList = *Dest;

		Dest->items = 0;
		Dest->top_item = 0;
		Dest->selected_item = 0;

		strcpy( Dest->item[Dest->items++], &TempItem[0]);

		for (i = 0; i < TempList.items; i++ )
		{
			strcpy( Dest->item[Dest->items++], TempList.item[i]);
		}
	}

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Swap a Teasm id from 1 list to another..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SwapTeamIDs( LIST * Source , LIST * Dest )
{
	int i,e, sourceteam, destteam;
	network_id_t	TempID;
	network_id_t	TempIDs[MAX_PLAYERS];
	network_id_t	*SourceIDs;
	network_id_t	*DestIDs;

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		TempIDs[i] = 0;
	}

	sourceteam = 0;
	destteam = 0;
	
	for (i = 0; i < MAX_TEAMS; i++)
	{
		if (Source == &TeamList[i])
		{
			SourceIDs = TeamIDs[i];
			sourceteam = i;
		}
		if (Dest == &TeamList[i])
		{
			DestIDs = TeamIDs[i];
			destteam = i;
		}
	}

	TempID = SourceIDs[Source->selected_item];

	e = 0;
	for( i = 0 ; i < TeamMembers[sourceteam]; i++ )
	{
		if( i != Source->selected_item )
		{
			TempIDs[e] = SourceIDs[i];
			e++;
		}
	}
	for( i = 0 ; i < e ; i++ )
	{
		SourceIDs[i] = TempIDs[i];
	}

	e = 0;
	DestIDs[TeamMembers[destteam]] = TempID;

	TeamMembers[sourceteam]--;
	TeamMembers[destteam]++;

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Re Assemble Team Numbers...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReAssembleTeamNumbers(int *dummy)
{
	int i,e, team;

	UpdateSessions( NULL );

	for (team = 0; team < MAX_TEAMS; team++)
	{
 		for( i = 0 ; i < TeamMembers[team] ; i++ )
		{
			for( e = 0 ; e < MAX_PLAYERS ; e ++ )
			{
				if (e != WhoIAm)
				{
					if( PlayerIDs[e] == TeamIDs[team][i] )
					{
						TeamNumber[e] = team;
					}
				}
			}
		}
	}

	DistributeTeamsToLists( NULL );
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
