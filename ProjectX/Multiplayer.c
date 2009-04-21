
#define IDIRECTPLAY2_OR_GREATER

#include <stdio.h>
#include "windows.h"
#include "typedefs.h"
#include "main.h"
#include <dplay.h>
#include "comm.h"
#include "d3dappi.h"
#include "title.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
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

// required version of Direct Play is 6.0 (4.6.0.318)
#define DPLAY_VERSION_HI	(4)
#define DPLAY_VERSION_LO	(6)
#define DPLAY_REVISION_HI	(0)
#define DPLAY_REVISION_LO	(318)

/*
 * Externals
 */
extern BOOL OKToProcessKeys;
extern BOOL	OKToJoinSession;
extern int OldPPSValue;
extern int OldColPerspective;
extern int OldUseShortPackets;

BOOL ResetKillsPerLevel = FALSE;
BOOL bTCP = FALSE;

extern uint8 QuickStart;

extern BOOL HarmTeamMates;
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

extern char *DemoFileName( char *demoname );
extern char *DemoName( char *demofilename );
extern void SetMultiplayerPrefs( void );

extern LIST	MySessionsList;
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
extern	DPID					dcoID;    // player id
extern	int16					Lives;

extern	BOOL	BountyHunt;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	TEXT	DemoGameName;
extern	BOOL	RecordDemo;
extern	LIST	ServiceProvidersList;
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

extern	LPDPSESSIONDESC2                    glpdpSD;

extern	LPDIRECTPLAY4A                       glpDP;				// directplay object pointer
extern	LPDIRECTPLAYLOBBY2A					lpDPlayLobby;		//Lobby stuff...
extern	LPDPLCONNECTION						glpdplConnection;	// connection settings
extern	TEXT TCPAddress;
extern	DPID	dcoID;
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
extern	int16	PreferedMaxPlayers;

extern	LIST	SessionsList;
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

BOOL ServiceProviderSet = FALSE;

BOOL IPAddressExists = FALSE;

uint32	IPAddress = 0;
char    IPAddressText[16];
char    ServiceProviderShortName[16];

DPSESSIONDESC2			Old_Session;
 
DPCAPS					ServiceProviderCaps;
DWORD					SugestedEnumSessionsTimeout = 0;


DWORD                   Old_WhoIAm = (DWORD) -1;
DWORD					Old_Kills = 0;
DWORD					Old_Deaths = 0;
DWORD					Old_TeamNumber = 0;
char					Old_Name[256] = { 0 };
BOOL					Rejoining = FALSE;

DPID					PlayerIDs[MAX_PLAYERS];

DPID					TeamIDs[MAX_TEAMS][MAX_PLAYERS];
int						TeamMembers[MAX_TEAMS];

MENU  *				GetPlayerNumMenu;
float	Bodge	= 1.0F;

GUID	ServiceProvidersGuids[MAXSERVICEPROVIDERS];

DPSESSIONDESC2	Sessions[MAXSESSIONS];
char SessionNames[ MAXSESSIONS ][ 128 ];

DPSESSIONDESC2	SessionsCopy[MAXSESSIONS];
LIST	SessionsListCopy;


BOOL	SessionsRefresh[MAXSESSIONS];
BOOL	SessionsRefreshActive = FALSE;
GUID	gSPGuid;            // currently selected service provider guid

BOOL	Modem2Modem = FALSE;

uint16	RandomStartPosModify = 0;							

void DebugPrintf( const char * format, ... );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );
void InitMySessionsList(void);
void DrawFlatMenuItem( MENUITEM *Item );
void GetLevelName( char *buf, int bufsize, int level );


/********************************************\
|
|  CurrentServiceProviderNameShort
|
\********************************************/

void GetServiceProviderShortName( void )
{

	int size = 16;
    LPGUID  lpGuid;

	lpGuid = (LPGUID)&ServiceProvidersGuids[ServiceProvidersList.selected_item];

	if ( ! ServiceProviderSet )
		return;

	memset( &ServiceProviderShortName, 0, size );

	if ( IsEqualGuid( lpGuid, (GUID*)&DPSPGUID_TCPIP  ) )

		_snprintf ( ServiceProviderShortName, size, "TCPIP" );
		
	else if ( IsEqualGuid( lpGuid, (GUID*)&DPSPGUID_IPX    ) )

		_snprintf ( ServiceProviderShortName, size, "IPX" );

	else if ( IsEqualGuid( lpGuid, (GUID*)&DPSPGUID_SERIAL ) )
		
		_snprintf ( ServiceProviderShortName, size, "SERIAL" );

	else if ( IsEqualGuid( lpGuid, (GUID*)&DPSPGUID_MODEM  ) )
		
		_snprintf ( ServiceProviderShortName, size, "MODEM" );

	//Msg("Set %s",&ServiceProviderShortName);

}



/*
 * IsEqualGuid
 *
 * Determines if two guids are equal
 */
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2)
{
   return (
      ((PLONG) lpguid1)[0] == ((PLONG) lpguid2)[0] &&
      ((PLONG) lpguid1)[1] == ((PLONG) lpguid2)[1] &&
      ((PLONG) lpguid1)[2] == ((PLONG) lpguid2)[2] &&
      ((PLONG) lpguid1)[3] == ((PLONG) lpguid2)[3]);
}

void InitDemoList( MENU * Menu );
void RestoreDemoSettings( void );
void GetMultiplayerPrefs( void );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Service Providers
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetServiceProviders( MENU * Item )
{

	DWORD size;
	static GUID last_service_provider;
	LPVOID *ptr;
	char temp[256];
	LPVOID lpAddress = NULL;
	DWORD  dwAddressSize = 0;

	/* default not set */
	ServiceProviderSet = FALSE;

	/* default settings */
	ServiceProvidersList.items			= 0;
	ServiceProvidersList.top_item		= 0;
	ServiceProvidersList.display_items	= 8;
	ServiceProvidersList.selected_item	= 0;
	ServiceProvidersList.FuncInfo		= NULL;

	/* default use tcpip */
	ptr = (LPVOID) &DPSPGUID_TCPIP;

	// try to get last provider used
	size = sizeof( temp );
	if ( RegGet( "ServiceProvider", (LPBYTE)temp, &size ) == ERROR_SUCCESS )
	{
		// convert the guid string from the registry into a GUID object
		if ( GUIDFromString( temp, &last_service_provider ) == S_OK )
		{
			// set to last used provider
			ptr = (LPVOID) &last_service_provider;
		}
		else
			Msg("Unable to convert Session GUID from string\n");
	}

	/* create a direct play lobby object to query for service providers */	
	DPlayCreateLobby();

	// get a list of the supported providers
	// and select ptr as the default
	DirectPlayEnumerate( EnumServiceProviders, ptr );

	// reset the ip address exists flag
	IPAddressExists = FALSE;

	// taken out cause of laggy wsa startup
	// leave for later in case new way to get ip found

	// if were using tcpip
	// then setup the IPAddress global
	//if (IsEqualGUID( &last_service_provider, &DPSPGUID_TCPIP ))
		// get the ip address
		//if (GetIPAdd())	IPAddressExists = TRUE;

	// release created direct play object
	DPlayRelease();

	// set ServiceProviderShortName global to the selected provider: tcpip, modem, serial, ipx
	GetServiceProviderShortName();

}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	DirectPlayEnumerate callback. Stores the service provider information.
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL WINAPI EnumServiceProviders(LPGUID lpGuid, LPTSTR lpSpName, DWORD dwMajorVersion,
						         DWORD dwMinorVersion, LPVOID lpv)
{
	
	if( ServiceProvidersList.items < MAXSERVICEPROVIDERS )
	{
		strncpy( ServiceProvidersList.item[ServiceProvidersList.items] , lpSpName , sizeof(ServiceProvidersList.item[0])  );
		ServiceProvidersGuids[ServiceProvidersList.items] = *lpGuid;
		if ( lpv && IsEqualGUID( lpGuid, (LPGUID) lpv ) )
		{
			ServiceProvidersList.selected_item = ServiceProvidersList.items;
			ServiceProviderSet = TRUE;
		}

		if( strlen(lpSpName ) >= sizeof(ServiceProvidersList.item[0]) && (CameraStatus == CAMERA_AtStart))
		{
			strcpy( ServiceProvidersList.item[ServiceProvidersList.items] + sizeof(ServiceProvidersList.item[0]) - 4 , "..." );
		}
		ServiceProvidersList.items++;
	}
    return(TRUE);
}

////
// StringFromGUID
//		only used by next function so leave it here
//      if we ever need to reuse it then put it somewhere better
////

static const BYTE GuidMap[] = { 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 
                                8, 9, '-', 10, 11, 12, 13, 14, 15 }; 

static const char szDigits[] = "0123456789ABCDEF"; 

BOOL StringFromGUID(LPCGUID lpguid, LPSTR lpsz) 
{ 
    int i; 
 
    const BYTE * pBytes = (const BYTE *) lpguid; 
 
    *lpsz++ = '{'; 
 
    for (i = 0; i < sizeof(GuidMap); i++) 
    { 
        if (GuidMap[i] == '-') 
        { 
            *lpsz++ = '-'; 
        } 
        else 
        { 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0xF0) >> 4 ]; 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0x0F) ]; 
        } 
    } 
    *lpsz++ = '}'; 
    *lpsz   = '\0'; 
 
    return TRUE; 
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	A Provider has been chosen create a directplay object
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

BOOL ExitProviderChosen ( MENUITEM * Item )
{
	HRESULT hr;
    LPGUID  lpGuid;
	char sp_guidtext[ 256 ];

	// get a pointer to the guid
	lpGuid = (LPGUID ) &ServiceProvidersGuids[ServiceProvidersList.selected_item];

	// Check for modem
	Modem2Modem = FALSE;
	if (IsEqualGUID( lpGuid, &DPSPGUID_MODEM) )
		Modem2Modem = TRUE;

	// remember the selection
	gSPGuid = *lpGuid;

	IPAddressExists = FALSE;

	if (
		OnceServiceProviderChosen(
		  &ServiceProvidersGuids[ServiceProvidersList.selected_item],
		  lpDPlayLobby,
		  &glpDP,
		  &TCPAddress.text[0]
		)
		!= DP_OK
	)
	{
		PrintErrorMessage ( CONNECTION_INITIALIZATION_ERROR, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	if (IsEqualGUID( lpGuid, &DPSPGUID_TCPIP) )
	{
		if (GetIPAdd())
			IPAddressExists = TRUE;
	}

//	if ( !CheckLegalIP() )
//	{
//		DPlayRelease();
//		MenuBack();
//		return FALSE;
//	}

	ZeroMemory(&ServiceProviderCaps,sizeof(ServiceProviderCaps));
    ServiceProviderCaps.dwSize = sizeof(DPCAPS);

	hr = IDirectPlayX_GetCaps( glpDP , &ServiceProviderCaps , 0);
	if( hr != DP_OK )
	{
		PrintErrorMessage ( COULDNT_GET_SERVICE_PROVIDER_CAPS, 2, NULL,
			                ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	SugestedEnumSessionsTimeout = ServiceProviderCaps.dwTimeout * 2;
	
//	if (!AutoSelectConnection)
//		MenuChange ( Item );

	if ( (CameraStatus == CAMERA_AtStart) && Item )
		MenuChange ( Item );

	if( Modem2Modem )
	{
		MaxPlayersSlider.value = 2;
		MaxPlayersSlider.max = 2;
	}
	else
	{
		MaxPlayersSlider.max = MAX_PLAYERS;
// Commented out by Methods
// Sunday July 6th
// note: why in the world is this done?
//		MaxPlayersSlider.value = PreferedMaxPlayers;
	}

	// convert guid to string...
	if ( StringFromGUID( &gSPGuid, sp_guidtext ) )
	{
		RegSetA( "ServiceProvider",  (LPBYTE)&sp_guidtext,  sizeof( sp_guidtext ) );
	}

	return TRUE;
}

BOOL RefreshDPlay ( void )
{
	HRESULT hr;
    LPGUID  lpGuid;

	// release any previously created Dplay Objects...
	DPlayRelease();

	// get a pointer to the guid
	lpGuid = (LPGUID ) &ServiceProvidersGuids[ServiceProvidersList.selected_item];


	if ((hr = OnceServiceProviderChosen( &ServiceProvidersGuids[ServiceProvidersList.selected_item] , lpDPlayLobby, &glpDP , &TCPAddress.text[0])) != DP_OK)
	{
		PrintErrorMessage ( CONNECTION_INITIALIZATION_ERROR, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	ZeroMemory(&ServiceProviderCaps,sizeof(ServiceProviderCaps));
    ServiceProviderCaps.dwSize = sizeof(DPCAPS);

	hr = IDirectPlayX_GetCaps( glpDP , &ServiceProviderCaps , 0);
	if( hr != DP_OK )
	{
		PrintErrorMessage ( COULDNT_GET_SERVICE_PROVIDER_CAPS, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	SugestedEnumSessionsTimeout = ServiceProviderCaps.dwTimeout * 2;

	return TRUE;
}

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
	HRESULT hr;
	int i;
	DWORD		Temp;
	BYTE		msg;
	int			size;
	LONGLONG	TempTime;
	uint32		Seed;
	uint32		PackedInfo[ MAX_PICKUPFLAGS ];

	// setup the chose directplay object
	ExitProviderChosen(NULL);

	if ( MaxPlayersSlider.max != 2 )
		PreferedMaxPlayers = MaxPlayersSlider.value;
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
	DebugPrintf("DPlayCreateSession.\n");
	if ((hr = DPlayCreateSession( &MultiPlayerGameName.text[0])) != DP_OK)
	{
		Msg("!DPlayCreateSession");
		return FALSE;
	}

	// create player
	DebugPrintf("DPlayCreatePlayer.\n");
	if ((hr = DPlayCreatePlayer(&dcoID, &biker_name[0], NULL, NULL, 0)) != DP_OK)
	{
		Msg("!DPlayCreatePlayer");
	    return FALSE;
	}

	DPlayUpdateInterval	= 60.0F / PacketsSlider.value;
	OldPPSValue = PacketsSlider.value;
	OldColPerspective = ColPerspective;
	OldUseShortPackets = UseShortPackets;
	
	DebugPrintf("SetupDplayGame.\n");
	SetupDplayGame();
	
	DebugPrintf("DPlayGetSessionDesc.\n");
	DPlayGetSessionDesc();
	

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
	
	BrightShips = MyBrightShips;

	DebugPrintf("Done.\n");

	return TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Current Sessions
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetCurrentSessions( MENU *Menu )
{

	// we are a joiner
	IsHost = FALSE;
	IsPseudoHost = FALSE;

	// reset session list
	SessionsList.items = 0;
	SessionsList.top_item = 0;
	SessionsList.display_items = 8;
	SessionsList.selected_item = -1;

	// reset player list
	PlayersList.items = 0;
	PlayersList.top_item = 0;
	PlayersList.display_items = 16;
	PlayersList.selected_item = -1;

	// look bellow
	GetCurrentSessions_ReScan(NULL);

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Current Sessions
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetCurrentSessions_ReScan( MENUITEM *Item )
{
	int i;

	// if we are currently refreshing
	if( SessionsRefreshActive )

		// then dont start again
		return;

	// set refreshing flag on
	SessionsRefreshActive = TRUE;
	
	// set the refreshed flag to false for all sessions in list
	for( i = 0 ; i < MAXSESSIONS ; i++ )
		SessionsRefresh[i] = FALSE;

	// Enumerate Sessions
	// and we will decide the timeout
	DPlayEnumSessions(

		SugestedEnumSessionsTimeout,	// interval which dplay broadcasts for games

		EnumSessions,					// callback called with each found session

		(LPVOID) NULL,					// pointer for user to pass in data
		
		// dwFlags

			DPENUMSESSIONS_ALL			// Enumerate all active sessions even if:
										// not accepting new players
										// player limit has been reached
										// new players have been disabled
										// joining has been disabled

		|	DPENUMSESSIONS_PASSWORDREQUIRED // also retrieve passworded sessions

		|	DPENUMSESSIONS_ASYNC		// Enumerate current sessions in the cache.
										// Does not block returns immediately.
										// Starts asynchronous process if not started.
										// Updates continue until canceled
		);

}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	EnumSessions callback. Inserts session description information in the Sessions List
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/


BOOL WINAPI EnumSessions(LPCDPSESSIONDESC2 lpDPSessionDesc, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext)
{
	int i;

	//
	// DPESC_TIMEOUT
	// Means all sessions have been enumerated
	// And we have been called one more time to let us know
	//

    if(dwFlags & DPESC_TIMEDOUT)
	{

		// we are done refreshing the list
		SessionsRefreshActive = FALSE;

		// set defaults
		SessionsListCopy.items			= 0;	//
		SessionsListCopy.top_item		= 0;	//
		SessionsListCopy.display_items	= 8;	//
		SessionsListCopy.selected_item	= -1;	// nothing selected

		// for each session
		for( i = 0 ; i < SessionsList.items ; i ++ )
		{

			// if the session was not refreshed
			if( ! SessionsRefresh[i] )

				// don't copy it to the list
				break;

			// copy the memory of the session pointer previously stored into SessionsCopy
			memcpy(
				&SessionsCopy[SessionsListCopy.items],
				&Sessions[i],
				sizeof(DPSESSIONDESC2)
				);

			// copy the memory of the session name previously stored into SessionsListCopy
			memcpy(
				&SessionsListCopy.item[SessionsListCopy.items],
				&SessionsList.item[i],
				sizeof(SessionsList.item[i])
				);

			// if this is the selected item
			if( i == SessionsList.selected_item )

				// set the selected_item in the SessionsListCopy
				SessionsListCopy.selected_item = SessionsListCopy.items;

			// up count of items coppied
			SessionsListCopy.items++;

		}

		// copy SessionsCopy into Sessions
		memcpy(&Sessions ,&SessionsCopy , sizeof(Sessions) );

		// copy SessionsListCopy into SessionsList
		memcpy(&SessionsList ,&SessionsListCopy , sizeof(SessionsList) );

		// for each SessionsList
		for( i = 0 ; i < SessionsList.items ; i ++ )

			// we have refreshed this item
			SessionsRefresh[i] = TRUE;

		// were done so tell enumerate sessions to stop calling us
		return FALSE;

	}

	//
	//  We are being called with a new session
	//

	// for each session in our existing list
	for( i = 0 ; i < SessionsList.items ; i++ )

		// if the current session guid equals an existing guid
		if(	IsEqualGuid(
				(const LPGUID)&(lpDPSessionDesc->guidInstance),
				(const LPGUID)&Sessions[i].guidInstance)
				)
		{

			// we have refreshed this entry
			SessionsRefresh[ i ] = TRUE;

			// we allready have this entry in the list
			// tell enum sessions to keep enumerating
			return TRUE;

		}

	// have we reached the limit of storable sessions ?
	if( SessionsList.items > MAXSESSIONS )
	{
		// tell developers
		DebugPrintf("EnumSessions: has reached MAXSESSIONS\n");

		// we can't save anymore sessions
		// tell enumsessions to keep refreshing
		// since we need to hit the primary block above
		// to copy any new sessions into the displayed list
		return TRUE;
	}

	//
	//  add this session to the globals
	//

	// store away pointer to session description
	Sessions[SessionsList.items] = *lpDPSessionDesc;

	// copy name of session into SessionNames
	strncpy(
		SessionNames[ SessionsList.items ],				// reciever
		lpDPSessionDesc->lpszSessionNameA,				// giver
		sizeof( SessionNames[ SessionsList.items ] )	// size to give
		);

	// copy name of session into SessionsList
	strncpy(
		SessionsList.item[SessionsList.items],			// reciever
		lpDPSessionDesc->lpszSessionNameA,				// giver
		sizeof(SessionsList.item[0])					// size to give
		);

	// if
	if(
		// the size of the given name
		strlen(
			lpDPSessionDesc->lpszSessionNameA
		// is bigger than our holder
		) >= sizeof(SessionsList.item[0])
	)
		// append "..." to the end of the name
		strcpy( SessionsList.item[SessionsList.items] + sizeof(SessionsList.item[0]) - 4 , "..." );

	// we have refreshed this session
	SessionsRefresh[SessionsList.items] = TRUE;

	// up the count of items
	SessionsList.items++;

	// were done tell enum sessions to keep enumerating
    return TRUE;

}

void GetSessionInfo ( LPDPSESSIONDESC2 sd )
{
	int32	Time;

	// new additions ( previously in MSG_INIT )
	if( sd->dwUser3 & HarmTeamMatesBit )
		HarmTeamMates = TRUE;
	else
		HarmTeamMates = FALSE;

	if( sd->dwUser3 & BrightShipsBit )
		BrightShips = TRUE;
	else
		BrightShips = FALSE;

	if( sd->dwUser3 & ResetKillsPerLevelBit )
		ResetKillsPerLevel = TRUE;
	else
		ResetKillsPerLevel = FALSE;
	
	if( sd->dwUser3 & BikeExhaustBit )
		BikeExhausts = TRUE;
	else
		BikeExhausts = FALSE;
	   
	Time = ( ( sd->dwUser3 & GameTimeBit ) >> GameTimeBit_Shift );

	if( Time )
	{
		TimeLimit.value = Time;
		CountDownOn = TRUE;
	}
	else
		CountDownOn = FALSE;

	RandomStartPosModify = (WORD) (sd->dwUser2 & 0xffff);
	
	if( sd->dwUser3 & TeamGameBit )
		TeamGame = TRUE;
	else
		TeamGame = FALSE;
	if( sd->dwUser3 & FlagGameBit )
		CaptureTheFlag = TRUE;
	else
		CaptureTheFlag = FALSE;

	CTF = ( sd->dwUser3 & CTFGameBit ) ? TRUE : FALSE;
	
	// new addition ( previously in MSG_INIT )
	if ( CTF )
		SetUpGameSubType( CTF_Type_Decode( sd->dwUser3 ) );

	BountyHunt = ( sd->dwUser3 & BountyGameBit ) ? TRUE : FALSE;
	UseShortPackets = ( sd->dwUser3 & ShortPacketsBit ) ? TRUE : FALSE;
	MaxKills = (int16)( ( sd->dwUser2 & MaxKillsBits ) >> MaxKills_Shift );
}

BOOL JoinASessionEx ( MENU *Menu )
{
	MENUITEM item;

	memset( &item, 0, sizeof( MENUITEM ) );
	item.Value = Menu;
	return JoinASession( &item );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Join a Session...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL JoinASession ( MENUITEM * Item )
{
	HRESULT hr;
    LPGUID  lpGuid;
	DPSESSIONDESC2 temp_sd;

	PlayDemo = FALSE;

	SetBikeMods( 0 );

	// get a pointer to the guid
	lpGuid = (LPGUID ) &Sessions[SessionsList.selected_item].guidInstance;

	// open session
	if ((hr = DPlayOpenSession( lpGuid)) != DP_OK)
	{
		PrintErrorMessage ( COULDNT_OPEN_SESSION, 1, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	// create player
	if ((hr = DPlayCreatePlayer(&dcoID, &biker_name[0], NULL, NULL, 0)) != DP_OK)
	{
		PrintErrorMessage ( COULDNT_CREATE_PLAYER, 1, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	DPlayGetSessionDesc();
	GetSessionInfo( glpdpSD );
	SetupDplayGame();
	Rejoining = FALSE;
	temp_sd = Sessions[SessionsList.selected_item];

	// zero Some stuff cos they might have changed..or they might be pointers...
	temp_sd.dwCurrentPlayers			= 0;
	Old_Session.dwCurrentPlayers		= 0;
	temp_sd.lpszSessionNameA		= 0;
	Old_Session.lpszSessionNameA	= 0;
	temp_sd.lpszPasswordA				= 0;
	Old_Session.lpszPasswordA		= 0;
	temp_sd.dwFlags						= 0;
	Old_Session.dwFlags					= 0;
	temp_sd.dwSize						= 0;
	Old_Session.dwSize					= 0;
	temp_sd.dwMaxPlayers				= 0;
	Old_Session.dwMaxPlayers			= 0;
	temp_sd.dwReserved1				= 0;
	Old_Session.dwReserved1			= 0;
	temp_sd.dwReserved2				= 0;
	Old_Session.dwReserved2			= 0;
	temp_sd.dwUser2						= 0;
	Old_Session.dwUser2				= 0;
	temp_sd.dwUser3						= 0;
	Old_Session.dwUser3				= 0;

	if( ( Mymemcmp( (BYTE*) &Old_Session , (BYTE*) &temp_sd , sizeof(DPSESSIONDESC2 ) ) == TRUE ) &&
		( _stricmp( (char*) &biker_name[0], (char*) &Old_Name[0] ) == 0 ) )
	{
	   	Rejoining = TRUE;
	}
	
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
	Procedure	:	EnumeratePlayer callback. Inserts player information into the PlayersList
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL WINAPI EnumPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext)
{

	if( PlayersList.items < MAX_PLAYERS )
	{
		PlayerIDs[PlayersList.items] = pidID;
		strncpy( PlayersList.item[PlayersList.items] , lpName->lpszShortNameA , sizeof(PlayersList.item[0])  );
		if( strlen(lpName->lpszShortNameA ) >= sizeof(PlayersList.item[0]) )
		{
			strcpy( PlayersList.item[PlayersList.items] + sizeof(PlayersList.item[0]) - 4 , "..." );
		}
		PlayersList.items++;
	}
    return(TRUE);
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get The Players In the Current Session...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetPlayersInCurrentSession( MENUITEM *Item )
{
	int i;
#ifdef DEBUG_ENUM_PLAYERS
	char buf[ 128 ];
#endif
	
	//Bodge -= framelag;
	//if( Bodge <= 0.0F )
	{
#ifdef ENUM_PLAYERS_ON_TITLES
		DPlayGetSessionDesc();

		NumOfPlayersSlider.value = (int) glpdpSD->dwCurrentPlayers;

		if (NumOfPlayersSlider.value != NumOfPlayersSlider.oldvalue)
		{	
//		 	NumOfPlayersSlider.oldvalue = NumOfPlayersSlider.value;
			NumOfPlayersSlider.redraw_req = TRUE;
		}
		
		PlayersList.items = 0;
		PlayersList.top_item = 0;
		PlayersList.selected_item = -1;
		DPlayEnumPlayers( NULL, EnumPlayers, (LPVOID) NULL, 0);
#else
		PlayersList.items = 0;
		PlayersList.top_item = 0;
		PlayersList.selected_item = -1;
		
		NumOfPlayersSlider.value = 0;

		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
#ifdef DEBUG_ENUM_PLAYERS
			sprintf( buf, "player %d status %x", i, GameStatus[ i ] );
			Print4x5Text( buf, 10, 10 * i + 50, 2 );
#endif			
			if( ( GameStatus[ i ] == MyGameStatus ) || ( i == WhoIAm ) )
			{
				NumOfPlayersSlider.value++;
				strcpy( PlayersList.item[ PlayersList.items++ ], Names[ i ] );
			}
		}

		if (NumOfPlayersSlider.value != NumOfPlayersSlider.oldvalue)
		{	
			NumOfPlayersSlider.redraw_req = TRUE;
		}
#endif
		Bodge = 30.0F;
	}
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
    if ( ( glpDP != NULL ) && ( dcoID != 0 ) && ( WhoIAm < MAX_PLAYERS ) )
    {
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
	}

	ChangeServiceProvider( Menu );
}

void BailMultiplayerFrontEnd( MENU *Menu )
{
	int selected_item;

	selected_item = ServiceProvidersList.selected_item;

	BailMultiplayer( Menu );

	GetServiceProviders( NULL );
	ServiceProvidersList.selected_item = selected_item;

	ExitProviderChosen( NULL );

	SessionsList.selected_item = -1;
	InitMySessionsList();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Changeing Service Provider...
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangeServiceProvider( MENU * Menu )
{	 
	if( dcoID )
	{
		DPlayDestroyPlayer(dcoID);
		dcoID = 0;
	}
	DPlayRelease();
	MyGameStatus = STATUS_Title;
	ServiceProvidersList.selected_item = -1;
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
	DPID	TempID;
	DPID	TempIDs[MAX_PLAYERS];
	DPID	*SourceIDs;
	DPID	*DestIDs;

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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	My Memory Compare...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL Mymemcmp( BYTE * buf1 , BYTE * buf2 , int size )
{
	while( size--)
	{
		if( *buf1++ != *buf2++ )
		{
			return FALSE;
		}
	}
	return TRUE;
}


#define WSA_DEBUG
int GetIPAdd( void )
{
	int result;
	WSADATA wsaData;
	struct hostent *hp;
	static char hostname[1024];
	uint32 LoBytes = 0x000000FF;

	// initialize wsa
	result = WSAStartup( 0x0101, &wsaData );
	if ( result != 0 && Debug )
	{
		switch( result )
		{
		case WSASYSNOTREADY:
			DebugPrintf("Underlying network subsystem is not ready for network communication.");
			break;
		case WSAVERNOTSUPPORTED:
			DebugPrintf("Version of Windows Sockets requested is not provided by this implementation.");
			break;
		case WSAEINPROGRESS:
			DebugPrintf("A blocking Windows Sockets 1.1 operation is in progress.");
			break;
		case WSAEPROCLIM:
			DebugPrintf("Limit on the number of tasks supported has been reached.");
			break;
		case WSAEFAULT:
			DebugPrintf("The lpWSAData parameter is not a valid pointer.");
			break;
		default:
			DebugPrintf("WSAStartup failed\n");
		}
		return 0;
	}

	// get the hostname of the current machine
	result = gethostname( hostname, sizeof(hostname) );
	if( result == SOCKET_ERROR && Debug )
	{
		result = WSAGetLastError();
		switch( result )
		{
		case WSAEFAULT:
			DebugPrintf("%s %s",
				"name is a NULL pointer or is not a valid part of the user address space.\n",
				"Or the buffer size specified is too small to hold the complete host name.\n");
			break;
		case WSANOTINITIALISED:
			DebugPrintf("A successful WSAStartup call must occur before using this function.\n");
			break;
		case WSAENETDOWN:
			DebugPrintf("The network subsystem has failed.\n");
			break;
		case WSAEINPROGRESS:
			DebugPrintf("%s %s",
				"A blocking Windows Sockets 1.1 call is in progress,\n",
				"or the service provider is still processing a callback function.\n");
			break;
		default:
			DebugPrintf("gethostname failed\n");
			break;
		}
		return 0;
	}
	
	// get the host by the name
	hp = gethostbyname ( hostname );	
	if ( !hp )
	{
#ifdef WSA_DEBUG
		Msg("gethostbyname failed");
#endif
		return 0;
	}

	// convert the data into standard decimal format
	IPAddress = *(uint32 *) *hp->h_addr_list;
	sprintf( IPAddressText, "%d.%d.%d.%d",
		                     IPAddress			& LoBytes,
							(IPAddress >> 8)	& LoBytes,
							(IPAddress >> 16)	& LoBytes,
							(IPAddress >> 24)	& LoBytes);

	// cleanup the wsa
	WSACleanup();

	// success
	return 1;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	We Are About to Start a Demo Playback...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void StartDemoPlayback( MENUITEM * Item )
{
	char buf[256];
	int i;
	uint32 mp_version;
	uint32 flags;
	uint32 PackedInfo[ MAX_PICKUPFLAGS ];

	TeamGame = FALSE;
	CountDownOn = FALSE;

	if( DemoList.item[0][0] == 0 )
	{
		// there are no demos...
		return;
	}
	LevelNum = -1;
	NewLevelNum = -1;

	for ( i = 0; i < MAX_PLAYERS; i++ )
		DemoShipInit[ i ] = FALSE;
	DemoShipInit[ MAX_PLAYERS ] = TRUE;
	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);

	DemoFp = fopen( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );

	if( !DemoFp )
	{
		// Couldnt find the selected demo...
		return;
	}

	setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...
	

	fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
	if ( (mp_version > MULTIPLAYER_VERSION) || (mp_version < DEMO_MULTIPLAYER_VERSION) )
	{
		// incompatible multiplayer version
		fclose( DemoFp );
		return;
	}

	fread( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
	fread( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
	fread( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
	fread( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );
	UnpackPickupInfo( &PackedInfo[ 0 ] );

	fread( &flags, sizeof( flags ), 1, DemoFp );
	TeamGame = ( flags & TeamGameBit ) ? TRUE : FALSE;
	CTF = ( flags & CTFGameBit ) ? TRUE : FALSE;
	CaptureTheFlag = ( flags & FlagGameBit ) ? TRUE : FALSE;
	BountyHunt = ( flags & BountyGameBit ) ? TRUE : FALSE;

	fread( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );

	for( i = 0 ; i < 256 ; i++ )
	{
		fread( &buf[i], sizeof(char), 1, DemoFp );
		if( buf[i] == 0 )
		{
			break;
		}
	}
	
    for (i = 0; i < NumLevels; i++)
	{

		if( _stricmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 ) 
		{
			NewLevelNum = i;
			break;
		}
    }
	
	if( ( NewLevelNum == -1 ) || ( i == 256 ) )
	{
		fclose( DemoFp );
		return;
	}
	MenuAbort();
	ReleaseView();
	DestroySound( DESTROYSOUND_All );	// ReleaseView will not do a DestroySound if MyGameStatus == STATUS_Title

	PlayDemo = TRUE;

	MyGameStatus = STATUS_ChangeLevelPostPlayingDemo;
	WhoIAm = MAX_PLAYERS;

//	RandomStartPosModify = 0;
	SetupDplayGame();

	ChangeLevel();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Clean up A demo File...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void StartDemoCleaning( MENUITEM * Item )
{
	char buf[256];
	int i;
	uint32 mp_version;
	uint32 flags;
	char *clean_name;
	uint16	TempSeed1, TempSeed2;
	BOOL	TempRandomPickups;
	uint32	TempPackedInfo[ MAX_PICKUPFLAGS ];

	NewLevelNum = -1;

	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);
	DemoFp = fopen( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );
	if ( !DemoFp )
	{
		// can't open file
		return;
	}

	setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...
	
	fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
	if ( (mp_version > MULTIPLAYER_VERSION) || (mp_version < DEMO_MULTIPLAYER_VERSION) )
	{
		// incompatible multiplayer version
		fclose( DemoFp );
		return;
	}

	fread( &TempSeed1, sizeof( TempSeed1 ), 1, DemoFp );
	fread( &TempSeed2, sizeof( TempSeed2 ), 1, DemoFp );
	fread( &TempRandomPickups, sizeof( TempRandomPickups ), 1, DemoFp );
	fread( &TempPackedInfo[ 0 ], sizeof( TempPackedInfo ), 1, DemoFp );

	fread( &flags, sizeof( flags ), 1, DemoFp );
	TeamGame = ( flags & TeamGameBit ) ? TRUE : FALSE;
	CTF = ( flags & CTFGameBit ) ? TRUE : FALSE;
	CaptureTheFlag = ( flags & FlagGameBit ) ? TRUE : FALSE;
	BountyHunt = ( flags & BountyGameBit ) ? TRUE : FALSE;

	fread( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );

	for( i = 0 ; i < 256 ; i++ )
	{
		fread( &buf[i], sizeof(char), 1, DemoFp );
		if( buf[i] == 0 )
		{
			break;
		}
	}
	
    for (i = 0; i < NumLevels; i++)
	{

		if( _stricmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 )
		{
			NewLevelNum = i;
			break;
		}
    }
	
	if( ( NewLevelNum == -1 ) || ( i == 256 ) )
	{
		fclose( DemoFp );
		return;
	}

	clean_name = _tempnam( ".", "dmo" );
	if ( !clean_name )
	{
		// unable to create unique temporary filename
		return;
	}
	DebugPrintf( "temp demo clean name = %s\n", clean_name );
//	DemoFpClean = fopen( DemoFileName( DemoGameName.text ) , "wbc" );
	DemoFpClean = fopen( clean_name , "wbc" );
	setvbuf( DemoFpClean, NULL, _IONBF , 0 );		// size of stream buffer...

	fwrite( &mp_version, sizeof( mp_version ), 1, DemoFpClean );

	fwrite( &TempSeed1, sizeof( TempSeed1 ), 1, DemoFpClean );
	fwrite( &TempSeed2, sizeof( TempSeed2 ), 1, DemoFpClean );
	fwrite( &TempRandomPickups, sizeof( TempRandomPickups ), 1, DemoFpClean );
	fwrite( &TempPackedInfo[ 0 ], sizeof( TempPackedInfo ), 1, DemoFpClean );

	fwrite( &flags, sizeof( flags ), 1, DemoFpClean );
	fwrite( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFpClean );
	for( i = 0 ; i < 256 ; i++ )
	{
		fwrite( &buf[i], sizeof(char), 1, DemoFpClean );
		if( buf[i] == 0 )
		{
			break;
		}
	}

	DemoClean();
	
	fclose( DemoFp );
	fclose( DemoFpClean );
	if ( !DeleteFile( DemoFileName( DemoList.item[DemoList.selected_item] ) ) )
	{
		DebugPrintf( "DeleteFile( %s ) failed\n", DemoFileName( DemoList.item[DemoList.selected_item] ) );
		DebugLastError();
	}
	if ( !MoveFile( clean_name, DemoFileName( DemoList.item[DemoList.selected_item] ) ) )
	{
		DebugPrintf( "MoveFile( %s, %s ) failed\n",
			clean_name, DemoFileName( DemoList.item[DemoList.selected_item] ) );
		DebugLastError();
	}
	free( clean_name );

	if (CameraStatus != CAMERA_AtStart)
		MenuBack();
	else
		MenuExit();
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Starting a Single Player Game...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StartASinglePlayerGame( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;

	PlayDemo = FALSE;
	IsHost = TRUE;
	AutoDetail = TRUE;
	// reset all bollocks...
	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	CTF = FALSE;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;

	SetBikeMods( (uint16) (SelectedBike+2) );

	SetupDplayGame();
	
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
		GameStatus[i] = STATUS_Null;
	
	WhoIAm = 0;								// I was the first to join...

	Ships[WhoIAm].dcoID = 0;
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].enable = 1;
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;																
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	CountDownOn = FALSE;

	MyGameStatus = STATUS_StartingSinglePlayer;

	return TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Starting a Single Player Game...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL LoadASinglePlayerGame( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;
	PlayDemo = FALSE;
	IsHost = TRUE;
	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;
	SetBikeMods( (uint16) (SelectedBike+2) );
	SetupDplayGame();
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].dcoID = 0;
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].enable = 1;
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;																
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	

	NewLevelNum = -1;
	PreInGameLoad( Item );

	if( NewLevelNum == -1 )
		return FALSE;

	CountDownOn = FALSE;

	MyGameStatus = STATUS_TitleLoadGameStartingSinglePlayer;

	return TRUE;
}


