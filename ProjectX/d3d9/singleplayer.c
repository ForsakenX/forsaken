
#include <stdio.h>
#include "windows.h"
#include "typedefs.h"
#include "main.h"
#include "net.h"
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


extern BOOL IsHost;   // is the user hosting/joining a game
extern BYTE Current_Camera_View; // which object is currently using the camera view....
extern	BYTE	GameStatus[];	// Game Status for every Ship...
extern uint16 RandomStartPosModify;
extern BOOL PlayDemo;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	int16	SelectedBike;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	SHORTNAMETYPE			Names;	// all the players short Names....
extern	char	biker_name[256];
extern	BOOL	CountDownOn;
extern	BOOL	TeamGame;

BOOL LoadASinglePlayerGame( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;
	PlayDemo = FALSE;
	IsHost = TRUE;
	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;
	SetBikeMods( (uint16) (SelectedBike+2) );
	SetupNetworkGame();
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	WhoIAm = 0;								// I was the first to join...
	Current_Camera_View = 0;				// set camera to that view
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
	LONGLONG	TempTime;

	PlayDemo = FALSE;
	IsHost = TRUE;
	// reset all bollocks...
	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	CTF = FALSE;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;

	SetBikeMods( (uint16) (SelectedBike+2) );

	SetupNetworkGame();
	
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
		GameStatus[i] = STATUS_Null;
	
	WhoIAm = 0;								// I was the first to join...
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].enable = 1;
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
	set_my_player_name();
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	CountDownOn = FALSE;

	MyGameStatus = STATUS_StartingSinglePlayer;

	return TRUE;
}
