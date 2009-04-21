
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

void GetServiceProviders( MENU * Item );
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

void SetUpGameType( int type );
BOOL RefreshDPlay ( void );
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);


char * CurrentServiceProviderShortName( void );

#endif	// MULTIPLAYER_INCLUDED