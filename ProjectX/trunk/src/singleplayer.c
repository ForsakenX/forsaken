
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


extern BOOL IsHost;   // is the user hosting/joining a game
extern BYTE Current_Camera_View; // which object is currently using the camera view....
extern	BYTE	GameStatus[];	// Game Status for every Ship...
extern uint16 RandomStartPosModify;
extern BOOL PlayDemo;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	int16	SelectedBike;
extern	GLOBALSHIP              Ships[MAX_PLAYERS+1];
extern	SHORTNAMETYPE			Names;	// all the players short Names....
extern	char	biker_name[256];
extern	BOOL	CountDownOn;
extern	BOOL	TeamGame;
extern  int16   NewLevelNum;
extern  BYTE          MyGameStatus;
extern SLIDER WatchPlayerSelect;
extern BOOL SwitchedToWatchMode;

BOOL LoadASinglePlayerGame( MENUITEM * Item )
{
	int i;
	PlayDemo = FALSE;
	IsHost = TRUE;
	RandomStartPosModify = 0;
	SetBikeMods( (uint16) (SelectedBike+2) );
	SetupNetworkGame();
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	WhoIAm = 0;								// I was the first to join...
	Current_Camera_View = 0;				// set camera to that view
	WatchPlayerSelect.value = 0;
	SwitchedToWatchMode = FALSE;
	Ships[WhoIAm].enable = 1;
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
	set_my_player_name();
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	

	NewLevelNum = -1;
	PreInGameLoad( Item );

	if( NewLevelNum == -1 )
		return FALSE;

	CountDownOn = FALSE;

	MyGameStatus = STATUS_TitleLoadGameStartingSinglePlayer;

	return TRUE;
}

BOOL StartASinglePlayerGame( MENUITEM * Item )
{
	int i;

	PlayDemo = FALSE;
	IsHost = TRUE;
	// reset all bollocks...
	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	CTF = FALSE;

	RandomStartPosModify = 0;

	SetBikeMods( (uint16) (SelectedBike+2) );

	SetupNetworkGame();
	
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
		GameStatus[i] = STATUS_Null;
	
	WhoIAm = 0;								// I was the first to join...
	Current_Camera_View = 0;				// set camera to that view
	WatchPlayerSelect.value = 0;
	SwitchedToWatchMode = FALSE;
	Ships[WhoIAm].enable = 1;
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
	set_my_player_name();
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	CountDownOn = FALSE;

	MyGameStatus = STATUS_StartingSinglePlayer;

	return TRUE;
}
