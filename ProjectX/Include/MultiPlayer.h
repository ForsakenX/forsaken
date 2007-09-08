/*
 * The X Men, November 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 34 $
 *
 * $Header: /PcProjectX/include/MultiPlayer.h 34    27/08/98 20:12 Philipy $
 *
 * $Log: /PcProjectX/include/MultiPlayer.h $
 * 
 * 34    27/08/98 20:12 Philipy
 * manual / auto server mode now selectable from menus
 * text now displayed when server in game & not rendering
 * dynamic PPS setting re-enabled for server
 * 
 * 33    20/08/98 15:28 Philipy
 * You can now join server based games after being launched by a lobby.
 * Level name is updated after pseudohost selects level
 * AVI can now play if no sound hw exists
 * started gamespy support
 * 
 * 32    5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 31    20/07/98 12:27 Philipy
 * nicer handling of uninstalled levels
 * 
 * 30    14/07/98 11:15 Philipy
 * various patch bugs
 * pseudohost quitting in titles
 * 
 * 29    7/07/98 18:05 Philipy
 * added lobby autostart code ( when all players have recieved init msg )
 * added num primary weapons menu option ( propergated to other players &|
 * server )
 * extracted new title text for localisation
 * 
 * 28    10/06/98 10:12 Collinsd
 * 
 * 27    22/05/98 17:51 Philipy
 * more work on session info
 * 
 * 26    20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 25    4/27/98 4:02p Phillipd
 * 
 * 24    5/04/98 17:10 Collinsd
 * Hacked demos to work.
 * 
 * 23    15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 22    6/03/98 17:37 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 21    2/23/98 2:02p Phillipd
 * Load Save now works.
 * 
 * 20    24/01/98 17:38 Philipy
 * fixed multiplayer join-quit-join bug
 * fixed attract mode loading wrong level
 * 
 * 19    1-10-97 8:26p Philipy
 * IP address now only displayed when hosting IP game
 * 
 * 18    9/30/97 4:21p Phillipd
 * 
 * 17    9/18/97 9:42a Phillipd
 * Proper Single Player Started
 * 
 * 16    14-09-97 6:57p Philipy
 * joining existing team game - you can now select which team to go into,
 * and view current scores
 * 
 * 15    1-09-97 5:35p Philipy
 * changes made for 3 & 4 player team game
 * 
 * 14    6/07/97 3:12p Phillipd
 * 
 * 13    3/11/97 5:41p Phillipd
 * 
 * 12    2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 11    2/20/97 11:10a Phillipd
 * Changed the way a Player Get his Player Number...
 * Much more stable and friendly...
 * 
 * 10    2/03/97 5:16p Phillipd
 * Translusceny is now controlled by global execute buffers.... which is
 * much better...
 * 
 * 9     12/23/96 6:04p Phillipd
 * 
 * 8     12/17/96 2:32p Phillipd
 * 
 * 7     12/17/96 9:20a Phillipd
 * 
 * 6     12/14/96 5:04p Phillipd
 * 
 * 5     12/13/96 12:52p Phillipd
 * 
 * 4     12/03/96 10:40a Phillipd
 * Multiplay can now back out
 * 
 * 3     11/28/96 10:00a Phillipd
 * Multiplayer join changed beyond all recognition,,,
 * 
 * 2     11/18/96 3:26p Phillipd
 * 
 * 1     11/18/96 12:23p Phillipd
 * New menu multiplayer stuff..
 */
#ifndef MULTIPLAYER_INCLUDED
#define MULTIPLAYER_INCLUDED

/*
 * Defines....
 */
#define MAXSERVICEPROVIDERS 8
#define MAXSESSIONS 32


extern DPSESSIONDESC2 Sessions[];
extern BOOL	SessionsRefresh[];


/*
 * Application messages
 */

BOOL WINAPI EnumServiceProviders(LPGUID lpGuid, LPTSTR lpSpName, DWORD dwMajorVersion,
		                         DWORD dwMinorVersion, LPVOID lpv);
BOOL WINAPI EnumSessions(LPCDPSESSIONDESC2 lpDPSessionDesc, LPDWORD lpdwTimeOut, DWORD dwFlags, 
                        LPVOID lpContext);
BOOL WINAPI EnumPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext);
BOOL WINAPI EnumLobbyPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext);

void GetServiceProviders( MENU *Menu );
BOOL ExitProviderChosen ( MENUITEM * Item );
BOOL StartAHostSession ( MENUITEM * Item );
void GetCurrentSessions( MENU *Menu );
void GetCurrentSessions_ReScan( MENUITEM *Item );
BOOL JoinASession ( MENUITEM * Item );
BOOL JoinASessionEx ( MENU *Menu );
void SelectSession( MENUITEM *Item );
void GetPlayersInCurrentSession( MENUITEM *Item );
void GoToSynchup ( MENUITEM * Item );

void BailMultiplayer( MENU * Menu );
void ChangeServiceProvider( MENU * Menu );
void ChangeServiceProviderPseudoHost( MENU * Menu );
void SwapListItem( LIST * Source , LIST * Dest );
void TeamGoToSynchup ( MENUITEM * Item );
void InitTeamSelection( MENU *Menu );
void InitTeamLists( MENU *Menu );
void InitExistingGameJoin( MENU *Menu );
void SwapTeamIDs( LIST * Source , LIST * Dest );
void ReAssembleTeamNumbers(int *dummy);
void CheckForMenuChange(int *dummy);

BOOL Mymemcmp( BYTE * buf1 , BYTE * buf2 , int size );
int CheckLegalIP( void );
int GetIPAdd( void );
void BailMultiplayerFrontEnd( MENU * Menu );
void StartDemoPlayback( MENUITEM * Item );
void StartDemoCleaning( MENUITEM * Item );
void DistributeTeamsToLists(int *dummy);

BOOL StartASinglePlayerGame( MENUITEM * Item );

BOOL LoadASinglePlayerGame( MENUITEM * Item );
BOOL InGameLoadASinglePlayerGame( MENUITEM * Item );
BOOL SplashDemoOK( char *demofile );
void GetSessionInfo ( LPDPSESSIONDESC2 sd );
BOOL StartAHostSessionServer( MENUITEM * Item );
void StartAPseudoHostSession( MENUITEM *Item );

void SetUpGameType( int type );
BOOL RefreshDPlay ( void );
HRESULT GUIDFromString( char *lpStr, GUID * pGuid);
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
void ExitServerSetup( MENU *Menu );


char * CurrentServiceProviderShortName( void );

#endif	// MULTIPLAYER_INCLUDED